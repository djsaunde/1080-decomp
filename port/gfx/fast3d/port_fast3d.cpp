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
 * PORT_GFX_PNG=<dir> writes <dir>/frame_NNNNNN.png after every task. */

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

namespace Fast {
void GfxSetInstance(std::shared_ptr<Interpreter> gfx);
}

namespace {

struct State {
    uint32_t w = 0, h = 0, seq = 0;
    std::shared_ptr<Ship::ConsoleVariable> cvars;
    std::shared_ptr<Ship::ResourceManager> resources;
    std::unique_ptr<EglWindow> window;
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

extern "C" int port_fast3d_init(unsigned int width, unsigned int height) {
    if (sState != nullptr) {
        return 0;
    }
    if (!gl_init((int)width, (int)height)) {
        std::fprintf(stderr, "port_fast3d: no GL context\n");
        return -1;
    }
    State* s = new State();
    s->w = width;
    s->h = height;
    s->cvars = std::make_shared<Ship::ConsoleVariable>();
    s->resources = std::make_shared<Ship::ResourceManager>();
    s->window = std::make_unique<EglWindow>(width, height);
    s->rapi = std::make_unique<Fast::GfxRenderingAPIOGL>(s->cvars, s->resources);
    s->gfx = std::make_shared<Fast::Interpreter>();
    Fast::GfxSetInstance(s->gfx);
    s->gfx->Init(s->window.get(), s->rapi.get(), "1080", false, width, height, 0, 0, s->cvars, s->resources);
    Fast::gfx_set_target_ucode(ucode_f3dex_line3d); /* gap 2: after Init */
    s->gfx->SetGfxDebugger(std::make_shared<Fast::GfxDebugger>());
    s->gfx->SetNativeDimensions(320.0f, 240.0f);
    const char* png = std::getenv("PORT_GFX_PNG");
    s->pngDir = (png != nullptr && *png != '\0') ? png : nullptr;
    sState = s;
    return 0;
}

extern "C" int port_fast3d_task(unsigned int root) {
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
    std::unordered_map<Mtx*, MtxF> noReplacements;
    s->gfx->StartFrame();
    s->gfx->Run(list, noReplacements);
    glFinish();
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
