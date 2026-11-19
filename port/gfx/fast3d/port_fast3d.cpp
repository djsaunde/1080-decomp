/* port/gfx/fast3d: the in-process renderer, PC port plan section 19.7 (option (a)).
 *
 * port_gfx_task (../port_gfx.c) calls port_fast3d_task for every M_GFXTASK when the port is
 * built with -DPORT_GFX_FAST3D=ON. It does in process what f3dreplay does behind a dump, and
 * handles the three embedding gaps run 824 found (plan 19.6):
 *
 * 1. Low addresses. gfx_set_timg_handler_rdp drops a texture at or below 0x0FFFFFFF that no
 *    loaded module owns (patch 0005). The game's lists point into its heap (sysapi.c's arena,
 *    in the executable's .bss: a module owns it), the USO images and the brk / mmap heap (no
 *    module owns them). Before each task every writable mapping below 0x10000000 in
 *    /proc/self/maps that is not registered yet is registered. 1080 never uses a segment
 *    (plan 11.1: segment 0 = 0 only), so no segmented address can be mistaken for memory.
 * 2. The ucode. Interpreter::Init resets it to F3DEX2; it is set to ucode_f3dex_line3d after.
 * 3. The view. With no LUS GUI nothing sets mGameWindowViewport, and Fast3D would draw into an
 *    offscreen game framebuffer that only the GUI composites. It is set to the whole window
 *    before every frame.
 *
 * The lists. On the 32-bit game (the shipping configuration of (a)) Fast3D's F3DGfx
 * {uintptr_t w0, w1} is 8 bytes, the game's own Gfx: the root is handed over in place (plan
 * decision 1). On a 64-bit host (tests, and later M6) every list reachable from the root is
 * widened to 16-byte F3DGfx per task, G_DL operands pointed at the widened targets, as in
 * f3dreplay; the other pointer operands must be valid host addresses as they are (the 4 GB
 * arena of plan 11.5 item 7 with base 0). Lists are re-widened every task, because the game
 * rewrites some in place (PolyShape_clipRows).
 *
 * The window. Here it is the EGL pbuffer of ../replay/egl_png.c (headless; this machine has no
 * SDL). A desktop build swaps EglWindow for LUS's SDL window backend; nothing else changes.
 * PORT_GFX_PNG=<dir> writes <dir>/frame_NNNNNN.png after every task.
 *
 * Render to texture (run 859). A task whose colour images are all narrower than the 320-wide frame
 * (the name-entry board's replay: G_SETCIMG width 192, scissor 192x96) draws into an offscreen image
 * that a later task loads as a texture. Fast3D ignores G_SETCIMG and draws it into the window's
 * top-left corner, so after such a task that corner is read back (patch 0003, big-endian RGBA5551)
 * into the image in game memory and the textures cached over it are dropped (patch 0004): the
 * next frame's overlay task samples it, one frame later, as on the N64. The game masks G_SETCIMG to
 * 24 bits; the full pointer is the one registered writable mapping that holds the whole image. */

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#include "fast/interpreter.h"
#include "fast/backends/gfx_opengl.h"
#include "fast/debug/GfxDebugger.h"
#include "ship/config/ConsoleVariable.h"
#include "ship/resource/ResourceManager.h"

#include "../replay/egl_png.h"
#include "port_fast3d_n64.h"
#include "egl_window.h"
#include "live_backend.h" /* live_depth_now */
#include "port_fast3d.h"
#include "port_fast3d_native.h"
#include "../port_fb_impl.h"
extern "C" {
#include "../../settings.h"
}

namespace Fast {
void GfxSetInstance(std::shared_ptr<Interpreter> gfx);
}

namespace {

struct State {
    uint32_t w = 0, h = 0, seq = 0;
    std::shared_ptr<Ship::ConsoleVariable> cvars;
    std::shared_ptr<Ship::ResourceManager> resources;
    std::unique_ptr<EglWindow> window; /* the pbuffer backend (tests, PORT_GFX_FAST3D) */
    Fast::GfxWindowBackend* wapi = nullptr; /* = window, or the native host's LiveWindow (plan 21.9) */
    std::unique_ptr<Fast::GfxRenderingAPIOGL> rapi;
    std::shared_ptr<Fast::Interpreter> gfx;
    std::set<std::pair<uintptr_t, uintptr_t>> registered;
    const char* pngDir = nullptr;
};
State* sState;

/* gap 1: vouch for every writable low mapping (heap, .bss, USO images, anonymous mmaps) */
void register_low_mappings(State& s) {
    FILE* f = std::fopen("/proc/self/maps", "r");
    if (f == nullptr) {
        return;
    }
    char line[512];
    while (std::fgets(line, sizeof(line), f) != nullptr) {
        unsigned long long a, b;
        char perms[8];
        if (std::sscanf(line, "%llx-%llx %7s", &a, &b, perms) != 3 || perms[1] != 'w' || a >= 0x10000000ull) {
            continue;
        }
        if (b > 0x10000000ull) {
            b = 0x10000000ull;
        }
        if (s.registered.insert({ (uintptr_t)a, (uintptr_t)b }).second) {
            gfx_register_raw_address_range((const void*)(uintptr_t)a, (size_t)(b - a));
        }
    }
    std::fclose(f);
}

/* render to texture: the task's colour images (G_SETCIMG), Z images and scissor, from its raw lists */
struct Offscreen {
    bool off = false;
    uint32_t img24 = 0, width = 0, height = 0;
};

Offscreen offscreen_target(uint32_t root) {
    Offscreen o;
    std::vector<uint32_t> zimgs;
    std::vector<std::pair<uint32_t, uint32_t>> cimgs; /* (w0, w1) */
    uint32_t stack[32], pc = root, sp = 0, lry = 0;
    for (uint32_t n = 0; n < (1u << 20) && pc != 0 && (pc & 7) == 0; n++) {
        const uint32_t* g = (const uint32_t*)(uintptr_t)pc;
        const uint32_t w0 = g[0], w1 = g[1];
        pc += 8;
        switch (w0 >> 24) {
            case 0x06: /* G_DL: push unless it is a branch */
                if (((w0 >> 16) & 0xFF) == 0) {
                    if (sp == 32) {
                        return o;
                    }
                    stack[sp++] = pc;
                }
                pc = w1;
                break;
            case 0xB8: /* G_ENDDL */
                pc = sp == 0 ? 0 : stack[--sp];
                break;
            case 0xFF:
                cimgs.push_back({ w0, w1 });
                break;
            case 0xFE:
                zimgs.push_back(w1 & 0xFFFFFF);
                break;
            case 0xED: /* G_SETSCISSOR: lower-right y, 10.2 */
                lry = std::max(lry, (w1 & 0xFFF) >> 2);
                break;
            default:
                break;
        }
    }
    for (const auto& c : cimgs) {
        const uint32_t width = (c.first & 0xFFF) + 1, a = c.second & 0xFFFFFF;
        if (width >= 320) {
            return Offscreen(); /* it draws into the frame */
        }
        if (std::find(zimgs.begin(), zimgs.end(), a) != zimgs.end() || ((c.first >> 19) & 3) != 2) {
            continue; /* the Z image's clear, or not 16-bit */
        }
        o.off = true;
        o.img24 = a;
        o.width = width;
    }
    o.height = std::min<uint32_t>(lry != 0 ? lry : 240, 240);
    return o;
}

#if UINTPTR_MAX > 0xFFFFFFFFu
/* the 64-bit list widening: one F3DGfx copy per list start, G_DL operands rewritten */
struct Widener {
    std::unordered_map<uint32_t, std::unique_ptr<std::vector<Fast::F3DGfx>>> lists;
    int unresolved = 0;

    Fast::F3DGfx* widen(uint32_t addr, int depth) {
        auto it = lists.find(addr);
        if (it != lists.end()) {
            return it->second->data();
        }
        if (addr == 0 || (addr & 7) != 0 || depth > 32) {
            unresolved++;
            return nullptr;
        }
        auto v = std::make_unique<std::vector<Fast::F3DGfx>>();
        const uint32_t* g = (const uint32_t*)(uintptr_t)addr;
        for (size_t i = 0; i < (1u << 20); i++) {
            Fast::F3DGfx c;
            c.words.w0 = g[i * 2];
            c.words.w1 = g[i * 2 + 1];
            v->push_back(c);
            uint32_t op = g[i * 2] >> 24;
            if (op == 0xB8 || (op == 0x06 && ((g[i * 2] >> 16) & 0xFF) == 1)) { /* G_ENDDL, G_DL NOPUSH (branch) */
                break;
            }
        }
        std::vector<Fast::F3DGfx>* list = v.get();
        lists.emplace(addr, std::move(v));
        for (size_t i = 0; i < list->size(); i++) {
            if (((*list)[i].words.w0 >> 24) == 0x06) {
                Fast::F3DGfx* t = widen((uint32_t)(*list)[i].words.w1, depth + 1);
                if (t != nullptr) {
                    (*list)[i].words.w1 = (uintptr_t)t;
                }
            }
        }
        return list->data();
    }
};
#endif

} // namespace

static void init_interpreter(State* s, uint32_t width, uint32_t height) {
    s->w = width;
    s->h = height;
    s->cvars = std::make_shared<Ship::ConsoleVariable>();
    s->resources = std::make_shared<Ship::ResourceManager>();
    s->rapi = std::make_unique<Fast::GfxRenderingAPIOGL>(s->cvars, s->resources);
    s->gfx = std::make_shared<Fast::Interpreter>();
    Fast::GfxSetInstance(s->gfx);
    s->gfx->Init(s->wapi, s->rapi.get(), "1080", false, width, height, 0, 0, s->cvars, s->resources);
    Fast::gfx_set_target_ucode(ucode_f3dex_line3d); /* gap 2: after Init */
    port_fast3d_n64_rapi(s->rapi.get()); /* plan 25: N64 clipping, shading, filtering */
    port_fast3d_n64_interp();
    s->gfx->SetGfxDebugger(std::make_shared<Fast::GfxDebugger>());
    s->gfx->SetNativeDimensions(320.0f, 240.0f);
    const char* png = std::getenv("PORT_GFX_PNG");
    s->pngDir = (png != nullptr && *png != '\0') ? png : nullptr;
    sState = s;
}

extern "C" int port_fast3d_init(unsigned int width, unsigned int height) {
    if (sState != nullptr) {
        return 0;
    }
    if (!gl_init((int)width, (int)height)) {
        std::fprintf(stderr, "port_fast3d: no GL context\n");
        return -1;
    }
    State* s = new State();
    s->window = std::make_unique<EglWindow>(width, height);
    s->wapi = s->window.get();
    init_interpreter(s, width, height);
    return 0;
}

/* plan 21.9: on the native host's window (its GL context is current) */
int port_fast3d_init_window(Fast::GfxWindowBackend* wapi, uint32_t width, uint32_t height) {
    if (sState != nullptr) {
        return 0;
    }
    State* s = new State();
    s->wapi = wapi;
    init_interpreter(s, width, height);
    return 0;
}

extern "C" int port_fast3d_task(unsigned int root) {
    return port_fast3d_task_fb(root, nullptr);
}

extern "C" int port_fast3d_task_fb(unsigned int root, const PortFbRequests* rq) {
    State* s = sState;
    int unresolved = 0;
    if (s == nullptr && port_fast3d_init(320, 240) != 0) {
        return -1;
    }
    s = sState;
    register_low_mappings(*s);
#if UINTPTR_MAX > 0xFFFFFFFFu
    Widener wd;
    Gfx* list = (Gfx*)wd.widen(root, 0);
    unresolved = wd.unresolved;
    if (list == nullptr) {
        return unresolved;
    }
#else
    Gfx* list = (Gfx*)(uintptr_t)root;
#endif
    /* gap 3: the game view is the whole window, every frame */
    s->gfx->mGameWindowViewport.x = 0;
    s->gfx->mGameWindowViewport.y = 0;
    s->gfx->mGameWindowViewport.width = s->w;
    s->gfx->mGameWindowViewport.height = s->h;
    /* the framebuffer model (plan 19.10): patch 0003 requests go in before Run() and are served at
     * its end; the colour frame lands in a staging buffer and is copied into a game image only if
     * the task drew into it */
    std::vector<std::vector<uint16_t>> staging;
    if (rq != nullptr) {
        staging.resize((size_t)rq->nColor);
        for (int i = 0; i < rq->nColor; i++) {
            staging[i].assign((size_t)PORT_FB_W * PORT_FB_H, 0);
            s->gfx->ReadbackRequestRgba5551(0, 0, PORT_FB_W, PORT_FB_H, staging[i].data(), true);
        }
        for (int i = 0; i < rq->nDepth; i++) {
            s->gfx->ReadbackRequestDepth((uint32_t)(rq->depthIdx[i] % PORT_FB_W), (uint32_t)(rq->depthIdx[i] / PORT_FB_W));
        }
    }
    /* render to texture: the offscreen image's region of the window, read back at the end of Run */
    const Offscreen off = offscreen_target(root);
    std::vector<uint16_t> rtt;
    if (off.off) {
        rtt.assign((size_t)off.width * off.height, 0);
        s->gfx->ReadbackRequestRgba5551(0, 0, off.width, off.height, rtt.data(), true);
    }
    std::unordered_map<Mtx*, MtxF> noReplacements;
    s->gfx->StartFrame();
    s->gfx->Run(list, noReplacements);
    glFinish();
    if (off.off) {
        const size_t bytes = rtt.size() * 2;
        uintptr_t img = 0;
        int found = 0;
        for (uintptr_t hi = 0; hi < 16; hi++) {
            const uintptr_t a = (hi << 24) | off.img24;
            for (const auto& r : s->registered) {
                if (a >= r.first && a + bytes <= r.second) {
                    img = a;
                    found++;
                    break;
                }
            }
        }
        if (found == 1) {
            std::memcpy((void*)img, rtt.data(), bytes);
            gfx_texture_cache_invalidate_range((const void*)(img > 0x8000 ? img - 0x8000 : 0), bytes + (img > 0x8000 ? 0x8000 : img));
        } else {
            static int warned;
            if (warned++ < 4) {
                std::fprintf(stderr, "port_fast3d: render to texture: image 0x%06x %ux%u is in %d mappings (not stored)\n",
                             off.img24, off.width, off.height, found);
            }
        }
    }
    if (rq != nullptr) {
        const uintptr_t cimg = (uintptr_t)s->gfx->mRdp->color_image_address & 0xFFFFFF;
        const uintptr_t zimg = (uintptr_t)s->gfx->mRdp->z_buf_address & 0xFFFFFF;
        for (int i = 0; i < rq->nColor; i++) {
            if (((uintptr_t)rq->color[i] & 0xFFFFFF) == cimg) {
                std::memcpy((void*)rq->color[i], staging[i].data(), staging[i].size() * 2);
                /* a colour image can be a texture too (Mat_fromFrameBuffer) */
                gfx_texture_cache_invalidate_range(rq->color[i], staging[i].size() * 2);
            }
        }
        for (int i = 0; i < rq->nDepth; i++) {
            uint16_t z;
            if (((uintptr_t)rq->depthImg[i] & 0xFFFFFF) == zimg &&
                s->gfx->ReadbackGetDepth((uint32_t)(rq->depthIdx[i] % PORT_FB_W),
                                         (uint32_t)(rq->depthIdx[i] / PORT_FB_W), &z)) {
                ((uint16_t*)rq->depthImg[i])[rq->depthIdx[i]] = (uint16_t)((z >> 8) | (z << 8)); /* big-endian */
            }
        }
        if (rq->nWrite > 0) {
            /* TODO (plan 19.10): composite CPU-written rectangles (debug text, the splash) over the
             * window; f3dreplay does it on the read pixels. Only debug paths write today. */
            static bool warned;
            if (!warned) {
                warned = true;
                std::fprintf(stderr, "port_fast3d: CPU framebuffer writes are not composited in process yet\n");
            }
        }
    }
    if (s->pngDir != nullptr) {
        std::vector<uint8_t> px((size_t)s->w * s->h * 4);
        char path[1024];
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, (GLsizei)s->w, (GLsizei)s->h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
        std::snprintf(path, sizeof(path), "%s/frame_%06u.png", s->pngDir, s->seq);
        write_png(path, px.data(), (int)s->w, (int)s->h);
    }
    s->gfx->EndFrame();
    s->seq++;
    return unresolved;
}

extern "C" void port_fast3d_last_images(unsigned int* cimg, unsigned int* zimg) {
    *cimg = sState != nullptr ? (unsigned int)(uintptr_t)sState->gfx->mRdp->color_image_address : 0;
    *zimg = sState != nullptr ? (unsigned int)(uintptr_t)sState->gfx->mRdp->z_buf_address : 0;
}

extern "C" void port_fast3d_invalidate(const void* addr, size_t size) {
    if (sState != nullptr) {
        gfx_texture_cache_invalidate_range(addr, size);
    }
}

extern "C" int port_fast3d_read_rgba(unsigned char* dst) {
    State* s = sState;
    if (s == nullptr) {
        return -1;
    }
    std::vector<uint8_t> px((size_t)s->w * s->h * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, (GLsizei)s->w, (GLsizei)s->h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    for (uint32_t y = 0; y < s->h; y++) {
        std::memcpy(dst + (size_t)y * s->w * 4, px.data() + (size_t)(s->h - 1 - y) * s->w * 4, (size_t)s->w * 4);
    }
    return 0;
}

/* ---------------- the native 64-bit game (plan 21.9, port_fast3d_native.h) ----------------
 * The step's tasks are queued (lists widened at submit time) and drawn at the step's end as one frame:
 * presenter.cpp's Renderer::frame on the game's own memory instead of a DLMP copy. */
#if UINTPTR_MAX > 0xFFFFFFFFu
namespace {

struct NativeTask {
    std::unique_ptr<Widener> wd;
    Fast::F3DGfx* root = nullptr;
    Offscreen off;
    bool hasZimg = false;
    Fast::F3DGfx zimg{};
    std::vector<uint32_t> timgs; /* G_SETTIMG addresses: where an offscreen image is sampled */
};

/* ---- plan 27: widescreen HUD anchoring and high-fps interpolation ----
 * Both act on the composited frame's widened lists before Fast3D runs them:
 *   - plan_frame walks the chain once per step and records, per RDP rectangle, its key (the G_SETTIMG image and
 *     the rectangle's ordinal among those with that image) and its widescreen placement (anchor shift / stretch),
 *     and snapshots the step's matrices (G_MTX) and vertices (G_VTX) as they are at the step's end;
 *   - rect_hook (patch 0018's gfx_set_rect_hook) moves each rectangle while Fast3D draws it;
 *   - an interpolated frame runs the same chain with Fast3D's matrix replacements, patch 0018's vertex
 *     replacements and the rectangles lerped, between the previous step's snapshot and this one's. */
struct RectPlan {
    uint64_t key = 0;
    int32_t shift = 0;    /* U10.2 x shift (anchoring) */
    bool stretch = false; /* full width: over the whole view */
    bool lerp = false;
};

struct Snap {
    bool valid = false;
    uint32_t step = 0;
    std::unordered_map<uint32_t, std::array<int32_t, 16>> mtx; /* the raw Mtx words, by game address */
    std::vector<std::pair<uint32_t, uint8_t>> mtxOrder;         /* (address, G_MTX parameters) in list order */
    std::unordered_map<uint32_t, std::vector<uint8_t>> vtx;     /* the G_VTX sources, by game address */
    std::unordered_map<uint64_t, std::array<int32_t, 4>> rects; /* corners (U10.2) by key */
};

struct Interp {
    Snap prev, cur;
    std::unordered_map<const void*, RectPlan> plan;
    bool hookOn = false;
    float alpha = 1.0f; /* < 1: an interpolated frame is being drawn */
    std::unordered_map<Mtx*, MtxF> mtxRep;
    std::unordered_map<const void*, const void*> vtxRep;
    std::vector<std::vector<uint8_t>> vtxStore;
    float camChange = 0;  /* the projection x view change into the previous step (cut detection) */
    bool cut = false;     /* the step is a camera cut: not interpolated */
    uint32_t frames = 0, cuts = 0, snappedMtx = 0, snappedVtx = 0, lerpedRects = 0;
    bool log = false;
};
Interp sInterp;

struct Native {
    std::vector<NativeTask> tasks;
    std::vector<NativeTask> last; /* the last drawn step's tasks: their widened lists, for redraws */
    Fast::F3DGfx* lastRoot = nullptr;
    std::vector<uint32_t> reads;                          /* colour images asked for */
    std::vector<std::pair<uint32_t, uint32_t>> depths;    /* (Z image, pixel): asked + windows */
    std::set<std::pair<uint32_t, uint32_t>> depthSet, asked;
    std::vector<Fast::F3DGfx> frameList, prologue, offList, offPrologue;
    std::vector<std::vector<uint16_t>> rttPixels, readPixels;
    uint32_t lastZimg = 0; /* the last frame's Z image (low 24 bits): port_fast3d_depth_now */
};
Native sNative;

/* the task's G_SETCIMG / G_SETZIMG (low 24 bits), its first G_SETZIMG command, its G_SETTIMG addresses */
void walk_task(uint32_t root, NativeTask& t, uint32_t* imgs, int maxImgs, int* nImgs) {
    uint32_t stack[32], pc = root, sp = 0;
    *nImgs = 0;
    for (uint32_t n = 0; n < (1u << 20) && pc != 0 && (pc & 7) == 0; n++) {
        const uint32_t* g = (const uint32_t*)(uintptr_t)pc;
        const uint32_t w0 = g[0], w1 = g[1];
        pc += 8;
        switch (w0 >> 24) {
            case 0x06:
                if (((w0 >> 16) & 0xFF) == 0) {
                    if (sp == 32) {
                        return;
                    }
                    stack[sp++] = pc;
                }
                pc = w1;
                break;
            case 0xB8:
                pc = sp == 0 ? 0 : stack[--sp];
                break;
            case 0xFE:
                if (!t.hasZimg) {
                    t.hasZimg = true;
                    t.zimg.words.w0 = w0;
                    t.zimg.words.w1 = w1;
                }
                [[fallthrough]];
            case 0xFF:
                if (*nImgs < maxImgs) {
                    imgs[(*nImgs)++] = w1 & 0xFFFFFFu;
                }
                break;
            case 0xFD:
                if (t.timgs.size() < 4096) {
                    t.timgs.push_back(w1);
                }
                break;
            default:
                break;
        }
    }
}

Fast::F3DGfx* chain(std::vector<Fast::F3DGfx>& lst, std::vector<Fast::F3DGfx>& pro, const std::vector<Fast::F3DGfx*>& rs,
                    const NativeTask& first) {
    lst.assign(rs.size() + 1, Fast::F3DGfx{});
    for (size_t i = 0; i < rs.size(); i++) {
        lst[i].words.w0 = 0x06000000u; /* G_DL push */
        lst[i].words.w1 = (uintptr_t)rs[i];
    }
    lst[rs.size()].words.w0 = 0xB8000000u; /* G_ENDDL */
    pro.clear();
    if (!first.hasZimg) {
        return lst.data();
    }
    pro.resize(3);
    pro[0] = first.zimg;
    pro[1].words.w0 = 0x06000000u;
    pro[1].words.w1 = (uintptr_t)lst.data();
    pro[2].words.w0 = 0xB8000000u;
    pro[2].words.w1 = 0;
    return pro.data();
}

/* an offscreen pass's pixels (big-endian RGBA5551) into its image in game memory. G_SETCIMG holds only
 * the low 24 bits: the full address is the one an onscreen task samples (a G_SETTIMG inside the image,
 * as the presenter's rtt_store finds it from the texture chunks), else the one registered mapping
 * that holds the whole image */
uint32_t native_store_rtt(State& s, const Offscreen& o, const std::vector<uint16_t>& px) {
    const uint32_t bytes = o.width * o.height * 2;
    std::set<uintptr_t> cands;
    for (const NativeTask& t : sNative.tasks) {
        if (t.off.off) {
            continue;
        }
        for (uint32_t a : t.timgs) {
            const uint32_t base = (a & 0xFF000000u) | o.img24;
            if (a >= base && a < base + bytes) {
                cands.insert(base);
            }
        }
    }
    if (cands.empty()) {
        for (uintptr_t hi = 0; hi < 128; hi++) {
            const uintptr_t a = (hi << 24) | o.img24;
            for (const auto& r : s.registered) {
                if (a >= r.first && a + bytes <= r.second) {
                    cands.insert(a);
                    break;
                }
            }
        }
    }
    if (cands.size() != 1) {
        static int warned;
        if (warned++ < 4) {
            std::fprintf(stderr, "port_fast3d: render to texture: image 0x%06x %ux%u has %zu candidates (not stored)\n",
                         o.img24, o.width, o.height, cands.size());
        }
        return 0;
    }
    const uintptr_t img = *cands.begin();
    std::memcpy((void*)img, px.data(), bytes);
    const uintptr_t lo = img > 0x8000 ? img - 0x8000 : 0;
    gfx_texture_cache_invalidate_range((const void*)lo, img + bytes - lo);
    return bytes;
}

/* walk a composited chain of widened lists as Fast3D runs it (G_DL push / branch, G_ENDDL, the 3-word
 * TEXRECT); fn(cmd) for every other command */
template <typename Fn> void walk_chain(Fast::F3DGfx* root, Fn&& fn) {
    Fast::F3DGfx* stack[32];
    Fast::F3DGfx* pc = root;
    int sp = 0;
    for (uint32_t n = 0; n < (1u << 21) && pc != nullptr; n++) {
        const uint32_t w0 = (uint32_t)pc->words.w0;
        const uintptr_t w1 = pc->words.w1;
        switch (w0 >> 24) {
            case 0x06:
                if (w1 < 0x10000) {
                    return;
                }
                if (((w0 >> 16) & 0xFF) == 0) {
                    if (sp == 32) {
                        return;
                    }
                    stack[sp++] = pc + 1;
                }
                pc = (Fast::F3DGfx*)w1;
                continue;
            case 0xB8:
                pc = sp == 0 ? nullptr : stack[--sp];
                continue;
            default:
                fn(pc);
                pc += ((w0 >> 24) == 0xE4 || (w0 >> 24) == 0xE5) ? 3 : 1;
                break;
        }
    }
}

bool low_ptr(uintptr_t a, size_t n) {
    return a >= 0x10000 && a + n <= 0x80000000u;
}

/* the view's width over the native 4:3 at its height: 1 = 4:3 */
float wide_factor(uint32_t vw, uint32_t vh) {
    return vh == 0 ? 1.0f : ((float)vw / (float)vh) / (4.0f / 3.0f);
}

void detect_cut();

/* the step's rectangles, matrices and vertices (plan 27). A = the view's wide factor (widescreen) */
void plan_frame(Fast::F3DGfx* root, uint32_t step, float A, bool edges) {
    Interp& I = sInterp;
    I.prev = std::move(I.cur);
    I.cur = Snap();
    I.plan.clear();
    Snap& S = I.cur;
    S.valid = true;
    S.step = step;
    struct R {
        const Fast::F3DGfx* cmd;
        uint64_t key;
        int32_t c[4]; /* ulx, uly, lrx, lry (U10.2) */
        bool z, full;
        int group;
    };
    std::vector<R> rs;
    std::unordered_map<uint64_t, uint32_t> ordinal;
    uint32_t otherL = 0, timg = 0;
    walk_chain(root, [&](Fast::F3DGfx* cmd) {
        const uint32_t w0 = (uint32_t)cmd->words.w0;
        const uintptr_t w1 = cmd->words.w1;
        switch (w0 >> 24) {
            case 0x01: /* G_MTX */
                if (low_ptr(w1, 64)) {
                    std::array<int32_t, 16> m;
                    std::memcpy(m.data(), (const void*)w1, 64);
                    S.mtx[(uint32_t)w1] = m;
                    S.mtxOrder.push_back({ (uint32_t)w1, (uint8_t)(w0 >> 16) });
                }
                break;
            case 0x04: { /* G_VTX (F3DEX 1): n = bits 10..15 */
                const size_t bytes = (size_t)((w0 >> 10) & 0x3F) * 16;
                if (bytes != 0 && low_ptr(w1, bytes)) {
                    auto& v = S.vtx[(uint32_t)w1];
                    if (v.size() < bytes) {
                        v.assign((const uint8_t*)w1, (const uint8_t*)w1 + bytes);
                    }
                }
                break;
            }
            case 0xB9: { /* G_SETOTHERMODE_L: shift, length */
                const uint32_t sft = (w0 >> 8) & 0xFF, len = w0 & 0xFF;
                const uint32_t mask = (len >= 32 ? 0xFFFFFFFFu : ((1u << len) - 1)) << sft;
                otherL = (otherL & ~mask) | ((uint32_t)w1 & mask);
                break;
            }
            case 0xEF: /* G_RDPSETOTHERMODE */
                otherL = (uint32_t)w1;
                break;
            case 0xFD:
                timg = (uint32_t)w1;
                break;
            case 0xE4:
            case 0xE5:
            case 0xF6: {
                const bool fill = (w0 >> 24) == 0xF6;
                R r;
                r.cmd = cmd;
                r.c[2] = (int32_t)((w0 >> 12) & 0xFFF);
                r.c[3] = (int32_t)(w0 & 0xFFF);
                r.c[0] = (int32_t)((w1 >> 12) & 0xFFF);
                r.c[1] = (int32_t)(w1 & 0xFFF);
                if (r.c[2] < r.c[0] || r.c[3] < r.c[1]) {
                    break; /* drawn as nothing (patch 0011) */
                }
                const uint64_t base = ((uint64_t)(fill ? 0xFFFFFFFFu : timg) << 24);
                r.key = base | ordinal[base]++;
                r.z = (otherL & 0x10) != 0; /* Z_CMP: a billboard in the 3D scene */
                r.full = r.c[0] <= 0 && r.c[2] >= 319 * 4;
                r.group = -1;
                S.rects[r.key] = { r.c[0], r.c[1], r.c[2], r.c[3] };
                rs.push_back(r);
                break;
            }
            default:
                break;
        }
    });
    /* widescreen placement: 2D drawn in consecutive, touching rectangles (a HUD panel: its background, label and
     * digits) forms a group; a group at the native screen's left or right edge moves to the view's edge.
     * Full-width rectangles (the Fader's mask and frame copy) stretch over the view. */
    const int32_t margin = (int32_t)lroundf(160.0f * (A - 1.0f) * 4.0f);
    struct G {
        int32_t x0, y0, x1, y1;
        int n;
    };
    std::vector<G> groups;
    const int32_t near = 8 * 4;
    for (R& r : rs) {
        if (r.z || r.full) {
            continue;
        }
        G* g = groups.empty() ? nullptr : &groups.back();
        if (g != nullptr && r.c[0] <= g->x1 + near && r.c[2] >= g->x0 - near && r.c[1] <= g->y1 + near &&
            r.c[3] >= g->y0 - near) {
            g->x0 = std::min(g->x0, r.c[0]);
            g->y0 = std::min(g->y0, r.c[1]);
            g->x1 = std::max(g->x1, r.c[2]);
            g->y1 = std::max(g->y1, r.c[3]);
            g->n++;
        } else {
            groups.push_back({ r.c[0], r.c[1], r.c[2], r.c[3], 1 });
        }
        r.group = (int)groups.size() - 1;
    }
    for (const R& r : rs) {
        RectPlan p;
        p.key = r.key;
        p.lerp = !r.full;
        if (A > 1.001f) {
            if (r.full) {
                p.stretch = true;
            } else if (edges && r.group >= 0) {
                const G& g = groups[(size_t)r.group];
                if (g.x0 <= 32 * 4 && g.x1 <= 160 * 4) {
                    p.shift = -margin;
                } else if (g.x1 >= 288 * 4 && g.x0 >= 160 * 4) {
                    p.shift = margin;
                }
            }
        }
        I.plan[r.cmd] = p;
    }
    if (I.prev.valid) {
        detect_cut();
    }
}

float mtx_elem(const std::array<int32_t, 16>& a, int i, int j);
void mtx_change(const std::array<int32_t, 16>& a, const std::array<int32_t, 16>& b, float* rot, float* tr);

/* a camera cut: the first projection matrix (1080 loads projection x view) jumps. The menus' cameras swoop
 * (changes ramp up and down over several steps, up to ~0.8); a cut is a change several times the step before's */
void detect_cut() {
    Interp& I = sInterp;
    const std::array<int32_t, 16>*pa = nullptr, *pb = nullptr;
    for (const auto& e : I.prev.mtxOrder) {
        if (e.second & 1) {
            pa = &I.prev.mtx[e.first];
            break;
        }
    }
    for (const auto& e : I.cur.mtxOrder) {
        if (e.second & 1) {
            pb = &I.cur.mtx[e.first];
            break;
        }
    }
    float m = 0;
    if (pa != nullptr && pb != nullptr) {
        float rot, tr;
        mtx_change(*pa, *pb, &rot, &tr);
        m = std::max(rot, tr);
    }
    I.cut = (pa == nullptr) != (pb == nullptr) || (m > 0.3f && m > 3.0f * I.camChange) || m > 1.5f;
    if (I.log) {
        std::printf("interp: step %u camera change %.3f (before %.3f)%s\n", I.cur.step, m, I.camChange,
                    I.cut ? " cut" : "");
    }
    I.camChange = m;
}

float mtx_elem(const std::array<int32_t, 16>& a, int i, int j) {
    /* as Fast3D's GfxSpMatrix reads a fixed-point Mtx */
    const int32_t ip = a[i * 2 + j / 2];
    const uint32_t fp = (uint32_t)a[8 + i * 2 + j / 2];
    return (j & 1) == 0 ? (int32_t)(((uint32_t)ip & 0xffff0000u) | (fp >> 16)) / 65536.0f
                        : (int32_t)(((uint32_t)ip << 16) | (fp & 0xffff)) / 65536.0f;
}

/* how far a matrix moved: the largest change of its rotation / scale rows relative to their size, and of its
 * translation relative to its distance (+ 1) */
void mtx_change(const std::array<int32_t, 16>& a, const std::array<int32_t, 16>& b, float* rot, float* tr) {
    float d3 = 0, m3 = 1e-6f, dt = 0, mt = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            const float x = mtx_elem(a, i, j), y = mtx_elem(b, i, j);
            if (i < 3) {
                d3 = std::max(d3, std::fabs(x - y));
                m3 = std::max(m3, std::fabs(y));
            } else {
                dt += (x - y) * (x - y);
                mt += y * y;
            }
        }
    }
    *rot = d3 / m3;
    *tr = std::sqrt(dt) / (std::sqrt(mt) + 1.0f);
}

/* the replacements for a frame at alpha in (0, 1) between I.prev and I.cur; false: draw the step as it is (no
 * previous step, or a camera cut) */
bool build_interp(float alpha) {
    Interp& I = sInterp;
    I.mtxRep.clear();
    I.vtxRep.clear();
    I.vtxStore.clear();
    if (!I.prev.valid || !I.cur.valid || I.prev.step + 1 != I.cur.step) {
        return false;
    }
    if (I.cut) {
        I.cuts++;
        return false;
    }
    /* matrices: by address when the previous step used the same one (an object's own matrix), else by ordinal
     * among the step's matrices with the same parameters (the per-step pool) */
    std::unordered_map<uint8_t, std::vector<uint32_t>> prevByKind;
    for (const auto& e : I.prev.mtxOrder) {
        prevByKind[e.second].push_back(e.first);
    }
    std::unordered_map<uint8_t, size_t> ord;
    std::unordered_map<uint32_t, bool> done;
    for (const auto& e : I.cur.mtxOrder) {
        const size_t k = ord[e.second]++;
        if (done[e.first]) {
            continue;
        }
        done[e.first] = true;
        const std::array<int32_t, 16>* a = nullptr;
        auto it = I.prev.mtx.find(e.first);
        if (it != I.prev.mtx.end()) {
            a = &it->second;
        } else {
            auto& v = prevByKind[e.second];
            if (k < v.size()) {
                a = &I.prev.mtx[v[k]];
            }
        }
        if (a == nullptr) {
            continue;
        }
        const std::array<int32_t, 16>& b = I.cur.mtx[e.first];
        float rot, tr;
        mtx_change(*a, b, &rot, &tr);
        /* the projection x view only snaps with the whole frame (detect_cut); an object's matrix that turned more
         * than ~30 degrees or jumped is drawn as the step (an element-wise lerp would shrink it) */
        if ((e.second & 1) == 0 && (rot > 0.5f || tr > 0.25f)) {
            if (I.log && alpha == 0.5f) {
                std::printf("interp: step %u snap mtx %08x %s k %zu flags %02x rot %.3f tr %.3f\n", I.cur.step, e.first,
                            it != I.prev.mtx.end() ? "same" : "ordinal", k, e.second, rot, tr);
            }
            I.snappedMtx++;
            continue;
        }
        MtxF f;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                const float x = mtx_elem(*a, i, j), y = mtx_elem(b, i, j);
                f.mf[i][j] = x + (y - x) * alpha;
            }
        }
        I.mtxRep[(Mtx*)(uintptr_t)e.first] = f;
    }
    /* vertices written on the CPU (the riders' skinned bodies, in world space) at the same address both steps */
    I.vtxStore.reserve(I.cur.vtx.size());
    for (const auto& e : I.cur.vtx) {
        auto it = I.prev.vtx.find(e.first);
        if (it == I.prev.vtx.end() || it->second.size() < e.second.size() ||
            std::memcmp(it->second.data(), e.second.data(), e.second.size()) == 0) {
            continue;
        }
        const size_t n = e.second.size() / 16;
        int maxd = 0;
        std::vector<uint8_t> out = e.second;
        for (size_t v = 0; v < n; v++) {
            int16_t a3[3], b3[3];
            std::memcpy(a3, it->second.data() + v * 16, 6);
            std::memcpy(b3, e.second.data() + v * 16, 6);
            for (int c = 0; c < 3; c++) {
                maxd = std::max(maxd, std::abs((int)b3[c] - (int)a3[c]));
                a3[c] = (int16_t)lroundf(a3[c] + (b3[c] - a3[c]) * alpha);
            }
            std::memcpy(out.data() + v * 16, a3, 6);
        }
        if (maxd > 2000) { /* a respawn / teleport, or another object in the same buffer */
            I.snappedVtx++;
            continue;
        }
        I.vtxStore.push_back(std::move(out));
        I.vtxRep[(const void*)(uintptr_t)e.first] = I.vtxStore.back().data();
    }
    return true;
}

/* patch 0018's rectangle hook: widescreen placement, and the lerp on an interpolated frame */
void rect_hook(const void* cmd, int32_t* ulx, int32_t* uly, int32_t* lrx, int32_t* lry, bool* stretch) {
    Interp& I = sInterp;
    auto it = I.plan.find(cmd);
    if (it == I.plan.end()) {
        return;
    }
    const RectPlan& p = it->second;
    if (I.alpha < 1.0f && p.lerp) {
        auto pr = I.prev.rects.find(p.key);
        if (pr != I.prev.rects.end()) {
            int32_t* c[4] = { ulx, uly, lrx, lry };
            int32_t d = 0;
            for (int k = 0; k < 4; k++) {
                d = std::max(d, std::abs(*c[k] - pr->second[k]));
            }
            if (d != 0 && d <= 48 * 4) {
                for (int k = 0; k < 4; k++) {
                    *c[k] = pr->second[k] + (int32_t)lroundf((*c[k] - pr->second[k]) * I.alpha);
                }
                I.lerpedRects++;
            }
        }
    }
    *ulx += p.shift;
    *lrx += p.shift;
    *stretch = p.stretch;
}

/* the settings that shape a frame (plan 27): the viewport, the render size (internal resolution), MSAA,
 * widescreen, the rectangle hook */
void apply_view(State& s, uint32_t vx, uint32_t vy, uint32_t vw, uint32_t vh) {
    const PortSettings* ps = port_settings();
    s.gfx->mGameWindowViewport.x = (int32_t)vx;
    s.gfx->mGameWindowViewport.y = (int32_t)vy;
    s.gfx->mGameWindowViewport.width = vw;
    s.gfx->mGameWindowViewport.height = vh;
    uint32_t rw = vw, rh = vh;
    if (ps->resolution > 0 && vh != 0) {
        rh = 240u * (uint32_t)ps->resolution;
        rw = (uint32_t)lroundf((float)rh * (float)vw / (float)vh);
    }
    s.gfx->mCurDimensions.width = rw;
    s.gfx->mCurDimensions.height = rh;
    s.gfx->SetMsaaLevel((uint32_t)std::max(1, ps->msaa));
    gfx_set_widescreen(wide_factor(vw, vh) > 1.001f);
    const bool hook = wide_factor(vw, vh) > 1.001f || ps->fps > 30;
    if (hook != sInterp.hookOn) {
        gfx_set_rect_hook(hook ? rect_hook : nullptr);
        sInterp.hookOn = hook;
    }
}

/* after Run: a frame Fast3D drew into its game framebuffer (internal resolution, MSAA at another size) is
 * scaled into the window's viewport; MSAA at the window's size was resolved into the window by Run */
void finish_view(State& s, uint32_t vx, uint32_t vy, uint32_t vw, uint32_t vh) {
    if (!s.gfx->mRendersToFb) {
        return;
    }
    if (s.gfx->mMsaaLevel > 1 && s.gfx->mCurDimensions.width == vw && s.gfx->mCurDimensions.height == vh) {
        return;
    }
    static GLuint fbo;
    if (fbo == 0) {
        glGenFramebuffers(1, &fbo);
    }
    const GLuint tex = (GLuint)(uintptr_t)s.gfx->mGfxFrameBuffer;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);
    int ww = 0, wh = 0;
    {
        uint32_t w, h;
        int32_t x, y;
        s.wapi->GetDimensions(&w, &h, &x, &y);
        ww = (int)w;
        wh = (int)h;
    }
    (void)ww;
    const int y0 = wh - (int)(vy + vh);
    /* the game framebuffer's rows run top-down (drawn flipped): blit it upside down */
    glBlitFramebuffer(0, (GLint)s.gfx->mCurDimensions.height, (GLint)s.gfx->mCurDimensions.width, 0, (GLint)vx, y0,
                      (GLint)(vx + vw), y0 + (GLint)vh, GL_COLOR_BUFFER_BIT,
                      port_settings()->scale_filter ? GL_LINEAR : GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace

int port_fast3d_queue(uint32_t root, uint32_t* imgs, int maxImgs) {
    State* s = sState;
    int n = 0;
    if (s == nullptr) {
        return 0;
    }
    NativeTask t;
    walk_task(root, t, imgs, maxImgs, &n);
    t.wd = std::make_unique<Widener>();
    t.root = t.wd->widen(root, 0);
    t.off = offscreen_target(root);
    sNative.tasks.push_back(std::move(t));
    return n;
}

void port_fast3d_ask_color(uint32_t img) {
    Native& N = sNative;
    if (std::find(N.reads.begin(), N.reads.end(), img) == N.reads.end() && N.reads.size() < LIVE_FB_COLOR) {
        N.reads.push_back(img);
    }
}

void port_fast3d_ask_depth(uint32_t img, uint32_t idx) {
    Native& N = sNative;
    if (idx >= LIVE_FB_W * LIVE_FB_H) {
        return;
    }
    /* the pixel and its window (live_shm.h LIVE_FB_DEPTH_R): the game reads a moving sun's neighbours next */
    N.asked.emplace(img, idx);
    const int px = (int)(idx % LIVE_FB_W), py = (int)(idx / LIVE_FB_W), R = LIVE_FB_DEPTH_R;
    for (int y = std::max(0, py - R); y <= std::min(LIVE_FB_H - 1, py + R); y++) {
        for (int x = std::max(0, px - R); x <= std::min(LIVE_FB_W - 1, px + R); x++) {
            auto key = std::make_pair(img, (uint32_t)(y * LIVE_FB_W + x));
            if (N.depths.size() < LIVE_FB_DEPTH && N.depthSet.insert(key).second) {
                N.depths.push_back(key);
            }
        }
    }
}

bool port_fast3d_step(uint32_t vx, uint32_t vy, uint32_t vw, uint32_t vh, LiveFbReply* reply, uint32_t step,
                      uint32_t seq, PortFast3dStep* st) {
    State* s = sState;
    Native& N = sNative;
    bool drawn = false;
    if (s == nullptr) {
        return false;
    }
    register_low_mappings(*s);
    std::vector<Fast::F3DGfx*> onRoots, offRoots;
    std::vector<size_t> offIdx;
    size_t withRoot = 0;
    for (const NativeTask& t : N.tasks) {
        withRoot += t.root != nullptr;
        st->unresolved += (uint32_t)t.wd->unresolved;
    }
    for (size_t i = 0; i < N.tasks.size(); i++) {
        NativeTask& t = N.tasks[i];
        if (t.root == nullptr) {
            continue;
        }
        if (withRoot > 1 && t.off.off) {
            offRoots.push_back(t.root);
            offIdx.push_back(i);
        } else {
            onRoots.push_back(t.root);
        }
    }
    if (onRoots.empty()) {
        onRoots.swap(offRoots);
        for (size_t i : offIdx) {
            N.tasks[i].off.off = false;
        }
        offIdx.clear();
    }
    st->tasks = (uint32_t)(onRoots.size() + offRoots.size());
    if (!onRoots.empty()) {
        drawn = true;
        apply_view(*s, vx, vy, vw, vh);
        sInterp.alpha = 1.0f;
        sInterp.plan.clear(); /* the offscreen pass's rectangles are not the frame's */
        std::unordered_map<Mtx*, MtxF> noReplacements;
        if (!offRoots.empty()) {
            Fast::F3DGfx* offRoot = chain(N.offList, N.offPrologue, offRoots, N.tasks[offIdx[0]]);
            N.rttPixels.resize(offIdx.size());
            for (size_t k = 0; k < offIdx.size(); k++) {
                const Offscreen& o = N.tasks[offIdx[k]].off;
                N.rttPixels[k].assign((size_t)o.width * o.height, 0);
                s->gfx->ReadbackRequestRgba5551(0, 0, o.width, o.height, N.rttPixels[k].data(), true);
            }
            s->gfx->StartFrame();
            s->gfx->Run((Gfx*)offRoot, noReplacements);
            for (size_t k = 0; k < offIdx.size(); k++) {
                native_store_rtt(*s, N.tasks[offIdx[k]].off, N.rttPixels[k]);
            }
            st->rttTasks = (uint32_t)offIdx.size();
        }
        Fast::F3DGfx* root = chain(N.frameList, N.prologue, onRoots, N.tasks[0]);
        if (sInterp.hookOn || port_settings()->fps > 30) {
            plan_frame(root, step, wide_factor(vw, vh), port_settings()->hud_edges != 0);
        }
        if (reply != nullptr) {
            N.readPixels.resize(N.reads.size());
            for (size_t k = 0; k < N.reads.size(); k++) {
                N.readPixels[k].assign((size_t)LIVE_FB_W * LIVE_FB_H, 0);
                s->gfx->ReadbackRequestRgba5551(0, 0, LIVE_FB_W, LIVE_FB_H, N.readPixels[k].data(), true);
            }
            for (const auto& d : N.depths) {
                s->gfx->ReadbackRequestDepth(d.second % LIVE_FB_W, d.second / LIVE_FB_W);
            }
        }
        s->gfx->StartFrame();
        s->gfx->Run((Gfx*)root, noReplacements);
        finish_view(*s, vx, vy, vw, vh);
        N.lastRoot = root;
        N.lastZimg = (uint32_t)(uintptr_t)s->gfx->mRdp->z_buf_address & 0xFFFFFF;
        if (reply != nullptr) {
            /* presenter.cpp Renderer::answer: the same reply, under the same seqlock */
            const uint32_t cimg = (uint32_t)(uintptr_t)s->gfx->mRdp->color_image_address & 0xFFFFFF;
            const uint32_t zimg = (uint32_t)(uintptr_t)s->gfx->mRdp->z_buf_address & 0xFFFFFF;
            LiveFbReply* r = reply;
            __atomic_store_n(&r->seq, r->seq + 1, __ATOMIC_RELEASE);
            __atomic_thread_fence(__ATOMIC_SEQ_CST);
            r->frameSeq = seq;
            r->step = step;
            uint32_t n = 0;
            for (const auto& d : N.depths) {
                uint16_t z = 0;
                if ((d.first & 0xFFFFFF) != zimg || !s->gfx->ReadbackGetDepth(d.second % LIVE_FB_W, d.second / LIVE_FB_W, &z)) {
                    st->depthOther += N.asked.count(d) != 0;
                    continue;
                }
                if (N.asked.count(d) != 0) {
                    (z == 0xFFFC ? st->depthFar : st->depthNear)++;
                }
                r->depth[n].img = d.first;
                r->depth[n].idx = d.second;
                r->depth[n].z = z;
                n++;
            }
            r->nDepth = n;
            n = 0;
            for (size_t k = 0; k < N.reads.size(); k++) {
                if ((N.reads[k] & 0xFFFFFF) != cimg) {
                    continue;
                }
                r->color[n].img = N.reads[k];
                std::memcpy(r->pixels[n], N.readPixels[k].data(), sizeof(r->pixels[n]));
                n++;
                st->colorServed++;
            }
            r->nColor = n;
            __atomic_thread_fence(__ATOMIC_SEQ_CST);
            __atomic_store_n(&r->seq, r->seq + 1, __ATOMIC_RELEASE);
        }
    }
    if (drawn) {
        N.last = std::move(N.tasks); /* the chain's widened lists stay valid for port_fast3d_redraw */
    }
    N.tasks.clear();
    N.reads.clear();
    N.depths.clear();
    N.depthSet.clear();
    N.asked.clear();
    return drawn;
}

bool port_fast3d_redraw(uint32_t vx, uint32_t vy, uint32_t vw, uint32_t vh, float alpha, bool* interpolated) {
    State* s = sState;
    Native& N = sNative;
    Interp& I = sInterp;
    *interpolated = false;
    if (s == nullptr || N.lastRoot == nullptr || N.last.empty()) {
        return false;
    }
    apply_view(*s, vx, vy, vw, vh);
    std::unordered_map<Mtx*, MtxF> noReplacements;
    const bool lerp = alpha < 1.0f && build_interp(alpha);
    I.alpha = lerp ? alpha : 1.0f;
    if (lerp) {
        gfx_set_vertex_replacements(&I.vtxRep);
        I.frames++;
        *interpolated = true;
    }
    s->gfx->StartFrame();
    s->gfx->Run((Gfx*)N.lastRoot, lerp ? I.mtxRep : noReplacements);
    gfx_set_vertex_replacements(nullptr);
    I.alpha = 1.0f;
    finish_view(*s, vx, vy, vw, vh);
    return true;
}

void port_fast3d_interp_stats(uint32_t* frames, uint32_t* cuts, uint32_t* snappedMtx, uint32_t* snappedVtx,
                              uint32_t* lerpedRects) {
    *frames = sInterp.frames;
    *cuts = sInterp.cuts;
    *snappedMtx = sInterp.snappedMtx;
    *snappedVtx = sInterp.snappedVtx;
    *lerpedRects = sInterp.lerpedRects;
}

void port_fast3d_interp_log(bool on) {
    sInterp.log = on;
}

int port_fast3d_depth_now(uint32_t img, uint32_t idx) {
    State* s = sState;
    if (s == nullptr || idx >= LIVE_FB_W * LIVE_FB_H || sNative.lastZimg == 0 ||
        (img & 0xFFFFFFu) != sNative.lastZimg) {
        return -1;
    }
    uint16_t z = 0;
    /* live_backend.h: the read the presenter's LiveDepthAsk answers make too (plan 22.10) */
    return live_depth_now(*s->gfx, idx % LIVE_FB_W, idx / LIVE_FB_W, 1, 1, &z) ? (int)z : -1;
}

void port_fast3d_end_frame() {
    if (sState != nullptr) {
        sState->gfx->EndFrame();
    }
}
#endif
