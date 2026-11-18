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
#include "egl_window.h"
#include "port_fast3d.h"
#include "port_fast3d_native.h"
#include "../port_fb_impl.h"

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

struct Native {
    std::vector<NativeTask> tasks;
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
        s->gfx->mGameWindowViewport.x = (int32_t)vx;
        s->gfx->mGameWindowViewport.y = (int32_t)vy;
        s->gfx->mGameWindowViewport.width = vw;
        s->gfx->mGameWindowViewport.height = vh;
        s->gfx->mCurDimensions.width = vw;
        s->gfx->mCurDimensions.height = vh;
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
    N.tasks.clear();
    N.reads.clear();
    N.depths.clear();
    N.depthSet.clear();
    N.asked.clear();
    return drawn;
}

int port_fast3d_depth_now(uint32_t img, uint32_t idx) {
    State* s = sState;
    if (s == nullptr || idx >= LIVE_FB_W * LIVE_FB_H || sNative.lastZimg == 0 ||
        (img & 0xFFFFFFu) != sNative.lastZimg) {
        return -1;
    }
    uint16_t z = 0;
    const uint32_t x = idx % LIVE_FB_W, y = idx / LIVE_FB_W;
    /* two pixels: Fast3D's one-pixel path (GfxRenderingAPIOGL::GetPixelDepth) reads GL_DEPTH_STENCIL straight
     * from the framebuffer, which fails here (GL_INVALID_OPERATION, the value left uninitialised: 0x15xx); the
     * blit path of two or more pixels reads right */
    s->gfx->ReadbackRequestDepth(x, y);
    s->gfx->ReadbackRequestDepth(x == 0 ? 1 : x - 1, y);
    s->gfx->ServiceReadbacks();
    return s->gfx->ReadbackGetDepth(x, y, &z) ? (int)z : -1;
}

void port_fast3d_end_frame() {
    if (sState != nullptr) {
        sState->gfx->EndFrame();
    }
}
#endif
