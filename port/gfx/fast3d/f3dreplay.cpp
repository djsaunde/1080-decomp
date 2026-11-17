/* port/gfx/fast3d: f3dreplay, render a DLMP dump (../dlump.h) with libultraship's
 * Fast3D interpreter (pinned + the plan-14 patch series) and write a PNG.
 * PC port plan section 19.6. 64-bit host, EGL offscreen, no SDL / ImGui.
 *
 *   f3dreplay in.dlmp out.png [--scale N] [--ucode f3dex|line3d] [--readback out.rb]
 *   LIBGL_ALWAYS_SOFTWARE=1 f3dreplay ...   llvmpipe, for the golden tests
 *
 * The dump's memory image is rebuilt AT ITS ORIGINAL ADDRESSES: every chunk is
 * copied into anonymous pages mapped at the same (32-bit) address in this 64-bit
 * process (MAP_FIXED_NOREPLACE; the low 4 GB of a PIE process is empty). So
 * every pointer operand a command carries (G_VTX, G_MTX, G_MOVEMEM, G_SETTIMG,
 * G_SETCIMG / ZIMG) is already a valid host pointer once zero-extended, and
 * nothing in the vertex / matrix / texture data needs rewriting. This is the
 * "4 GB arena" of plan 11.5 item 7 with the arena base at 0.
 *
 * Only the lists themselves are converted: the game's Gfx is two u32 words (8
 * bytes); Fast3D's F3DGfx is two uintptr_t (16 bytes here). Each DL chunk is
 * widened into its own F3DGfx array, and every G_DL operand (call or branch) is
 * rewritten to the widened copy of its target. A target outside every DL chunk
 * stays as it was and is reported; the dumper records such a gap as an ERROR.
 *
 * --readback exercises the patch-3 read-back API the way the game's CPU readers will use
 * it (plan 11.2): the whole 320x240 frame as big-endian RGBA5551 and the depth at a 16 x 12
 * grid of points are requested before Run() and serviced at its end. The answers go to
 * out.rb ("1080RB01", u32 width, height, point count, the pixels, then {u16 x, y, z} per
 * point; host order except the big-endian pixels), and the pixels are checked against the
 * PNG: each RGBA5551 channel must equal the 8-bit one >> 3 (row 0 = the top). */

#include <sys/mman.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#include "fast/interpreter.h"
#include "fast/backends/gfx_opengl.h"
#include "fast/backends/gfx_window_manager_api.h"
#include "fast/debug/GfxDebugger.h"
#include "ship/config/ConsoleVariable.h"
#include "ship/resource/ResourceManager.h"

extern "C" {
#include "../dlump.h"
}
#include "../replay/egl_png.h"
#include "egl_window.h"

namespace Fast {
void GfxSetInstance(std::shared_ptr<Interpreter> gfx);
}

/* ---------------- the dump ---------------- */

struct Chunk {
    uint32_t kind, addr, size, aux;
    const uint8_t* data;
};

static std::vector<uint8_t> sFile;
static const DlumpHeader* sHdr;
static std::vector<Chunk> sChunks;

static bool load(const char* path) {
    FILE* f = std::fopen(path, "rb");
    if (f == nullptr) {
        std::fprintf(stderr, "f3dreplay: cannot open %s\n", path);
        return false;
    }
    std::fseek(f, 0, SEEK_END);
    long n = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    sFile.resize((size_t)n);
    if (std::fread(sFile.data(), 1, (size_t)n, f) != (size_t)n) {
        std::fclose(f);
        return false;
    }
    std::fclose(f);
    sHdr = (const DlumpHeader*)sFile.data();
    if (n < 64 || std::memcmp(sHdr->magic, DLUMP_MAGIC, 8) != 0 || sHdr->version != DLUMP_VERSION ||
        sHdr->byteOrderMark != DLUMP_BYTE_ORDER_MARK) {
        std::fprintf(stderr, "f3dreplay: %s: not a v1 DLMP in host byte order\n", path);
        return false;
    }
    size_t off = sHdr->headerSize;
    for (uint32_t i = 0; i < sHdr->chunkCount; i++) {
        const DlumpChunk* c = (const DlumpChunk*)(sFile.data() + off);
        if (off + sizeof(*c) > (size_t)n || off + sizeof(*c) + c->size > (size_t)n) {
            std::fprintf(stderr, "f3dreplay: truncated chunk %u\n", i);
            return false;
        }
        sChunks.push_back({ c->kind, c->addr, c->size, c->aux, (const uint8_t*)(c + 1) });
        off += sizeof(*c) + ((c->size + 3) & ~3u);
    }
    return true;
}

static bool has_data(const Chunk& c) {
    return c.size != 0 && c.kind != DLUMP_ERROR && c.kind != DLUMP_IMAGE && c.kind != DLUMP_END;
}

/* Map every data chunk at its own address. Pages are merged first, so chunks
 * that share a page share one mapping. */
static bool map_arena() {
    const uint64_t pg = 4096;
    std::vector<std::pair<uint64_t, uint64_t>> r;
    for (const Chunk& c : sChunks) {
        if (has_data(c)) {
            r.push_back({ c.addr & ~(pg - 1), ((uint64_t)c.addr + c.size + pg - 1) & ~(pg - 1) });
        }
    }
    std::sort(r.begin(), r.end());
    std::vector<std::pair<uint64_t, uint64_t>> m;
    for (auto& x : r) {
        if (!m.empty() && x.first <= m.back().second) {
            m.back().second = std::max(m.back().second, x.second);
        } else {
            m.push_back(x);
        }
    }
    for (auto& x : m) {
        void* p = mmap((void*)x.first, x.second - x.first, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
        if (p != (void*)x.first) {
            std::fprintf(stderr, "f3dreplay: cannot map the dump's 0x%llx..0x%llx at its own address\n",
                         (unsigned long long)x.first, (unsigned long long)x.second);
            return false;
        }
        /* patch 0005: Fast3D drops texture loads from addresses below 0x10000000 that no
         * loaded module owns; vouch for the arena */
        gfx_register_raw_address_range((const void*)x.first, x.second - x.first);
    }
    for (const Chunk& c : sChunks) {
        if (has_data(c)) {
            std::memcpy((void*)(uintptr_t)c.addr, c.data, c.size);
        }
    }
    return true;
}

/* ---------------- the lists, widened to F3DGfx ---------------- */

struct WideDl {
    uint32_t addr, n;
    std::vector<Fast::F3DGfx> cmds;
};
static std::vector<WideDl> sDls;
static int sUnresolved;

static Fast::F3DGfx* wide(uint32_t addr) {
    for (WideDl& d : sDls) {
        if (addr >= d.addr && addr < d.addr + d.n * 8 && ((addr - d.addr) & 7) == 0) {
            return &d.cmds[(addr - d.addr) / 8];
        }
    }
    return nullptr;
}

static void widen_lists() {
    for (const Chunk& c : sChunks) {
        if (c.kind != DLUMP_DL || c.size < 8) {
            continue;
        }
        WideDl d;
        d.addr = c.addr;
        d.n = c.size / 8;
        d.cmds.resize(d.n);
        for (uint32_t i = 0; i < d.n; i++) {
            uint32_t w[2];
            std::memcpy(w, c.data + i * 8, 8);
            d.cmds[i].words.w0 = w[0];
            d.cmds[i].words.w1 = w[1];
        }
        sDls.push_back(std::move(d));
    }
    /* the vectors are final now: rewrite G_DL (0x06) operands to the widened targets */
    for (WideDl& d : sDls) {
        for (Fast::F3DGfx& g : d.cmds) {
            if (((g.words.w0 >> 24) & 0xFF) == 0x06) {
                Fast::F3DGfx* t = wide((uint32_t)g.words.w1);
                if (t != nullptr) {
                    g.words.w1 = (uintptr_t)t;
                } else {
                    std::fprintf(stderr, "f3dreplay: G_DL target 0x%08x is not in the dump\n", (uint32_t)g.words.w1);
                    sUnresolved++;
                }
            }
        }
    }
}

/* ---------------- main ---------------- */

/* the read-back request (before Run) and its check / file (after) */
struct Readback {
    static constexpr uint32_t W = 320, H = 240, STEP = 20;
    std::vector<uint16_t> px;
    std::vector<std::pair<uint32_t, uint32_t>> pts;

    void request(Fast::Interpreter& gfx) {
        px.assign((size_t)W * H, 0);
        gfx.ReadbackRequestRgba5551(0, 0, W, H, px.data(), true);
        for (uint32_t y = STEP / 2; y < H; y += STEP) {
            for (uint32_t x = STEP / 2; x < W; x += STEP) {
                pts.push_back({ x, y });
                gfx.ReadbackRequestDepth(x, y);
            }
        }
    }
    /* frame = glReadPixels of the window at scale 1 (bottom row first) */
    bool finish(Fast::Interpreter& gfx, const char* path, const uint8_t* frame) {
        uint32_t bad = 0, far = 0, near = 0, missing = 0;
        for (uint32_t y = 0; y < H; y++) {
            for (uint32_t x = 0; x < W; x++) {
                const uint8_t* b = (const uint8_t*)&px[(size_t)y * W + x];
                uint32_t v = ((uint32_t)b[0] << 8) | b[1]; /* big-endian in memory */
                const uint8_t* f = frame + ((size_t)(H - 1 - y) * W + x) * 4;
                if (((v >> 11) & 31) != (uint32_t)(f[0] >> 3) || ((v >> 6) & 31) != (uint32_t)(f[1] >> 3) ||
                    ((v >> 1) & 31) != (uint32_t)(f[2] >> 3)) {
                    bad++;
                }
            }
        }
        std::vector<uint16_t> z;
        for (auto& p : pts) {
            uint16_t d = 0;
            if (!gfx.ReadbackGetDepth(p.first, p.second, &d)) {
                missing++;
            }
            (d == 0xFFFC ? far : near)++;
            z.push_back(d);
        }
        FILE* f = std::fopen(path, "wb");
        if (f == nullptr) {
            std::fprintf(stderr, "f3dreplay: cannot write %s\n", path);
            return false;
        }
        uint32_t hdr[3] = { W, H, (uint32_t)pts.size() };
        std::fwrite("1080RB01", 1, 8, f);
        std::fwrite(hdr, 4, 3, f);
        std::fwrite(px.data(), 2, px.size(), f);
        for (size_t i = 0; i < pts.size(); i++) {
            uint16_t rec[3] = { (uint16_t)pts[i].first, (uint16_t)pts[i].second, z[i] };
            std::fwrite(rec, 2, 3, f);
        }
        std::fclose(f);
        std::printf("readback: %u / %u pixels differ from the frame; depth: %u far (0xFFFC), %u nearer, %u missing\n",
                    bad, W * H, far, near, missing);
        return bad == 0 && missing == 0;
    }
};

int main(int argc, char** argv) {
    int scale = 1;
    const char* readbackPath = nullptr;
    UcodeHandlers ucode = ucode_f3dex_line3d;
    if (argc < 3) {
        std::fprintf(stderr, "usage: f3dreplay in.dlmp out.png [--scale N] [--ucode f3dex|line3d] [--readback out.rb]\n");
        return 2;
    }
    for (int i = 3; i + 1 < argc; i++) {
        if (std::strcmp(argv[i], "--scale") == 0) {
            scale = std::max(1, std::atoi(argv[++i]));
        } else if (std::strcmp(argv[i], "--ucode") == 0) {
            ucode = std::strcmp(argv[++i], "f3dex") == 0 ? ucode_f3dex : ucode_f3dex_line3d;
        } else if (std::strcmp(argv[i], "--readback") == 0) {
            readbackPath = argv[++i];
        }
    }
    if (!load(argv[1]) || !map_arena()) {
        return 1;
    }
    widen_lists();
    Fast::F3DGfx* root = wide(sHdr->rootAddr);
    if (root == nullptr) {
        std::fprintf(stderr, "f3dreplay: the root list 0x%08x is not in the dump\n", sHdr->rootAddr);
        return 1;
    }
    const uint32_t w = 320 * scale, h = 240 * scale;
    if (!gl_init((int)w, (int)h)) {
        std::fprintf(stderr, "f3dreplay: no EGL / GL context\n");
        return 1;
    }

    auto cvars = std::make_shared<Ship::ConsoleVariable>();
    auto resources = std::make_shared<Ship::ResourceManager>();
    EglWindow window(w, h);
    Fast::GfxRenderingAPIOGL rapi(cvars, resources);
    auto gfx = std::make_shared<Fast::Interpreter>();
    Fast::GfxSetInstance(gfx);
    gfx->Init(&window, &rapi, "1080", false, w, h, 0, 0, cvars, resources);
    Fast::gfx_set_target_ucode(ucode); /* after Init, which resets it to F3DEX2 */
    gfx->SetGfxDebugger(std::make_shared<Fast::GfxDebugger>());
    gfx->SetNativeDimensions(320.0f, 240.0f);

    /* LUS's ImGui layer normally reports where the game view sits in the window (Fast3dGui);
     * with no GUI the view IS the window, so Fast3D draws straight into the pbuffer instead of
     * an offscreen game framebuffer that only the GUI would composite */
    gfx->mGameWindowViewport.x = 0;
    gfx->mGameWindowViewport.y = 0;
    gfx->mGameWindowViewport.width = w;
    gfx->mGameWindowViewport.height = h;

    Readback rb;
    if (readbackPath != nullptr) {
        if (scale != 1) {
            std::fprintf(stderr, "f3dreplay: --readback needs --scale 1\n");
            return 2;
        }
        rb.request(*gfx);
    }
    std::unordered_map<Mtx*, MtxF> noReplacements;
    gfx->StartFrame();
    gfx->Run((Gfx*)root, noReplacements);
    glFinish();

    std::vector<uint8_t> px((size_t)w * h * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, (GLsizei)w, (GLsizei)h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    if (!write_png(argv[2], px.data(), (int)w, (int)h)) {
        std::fprintf(stderr, "f3dreplay: cannot write %s\n", argv[2]);
        return 1;
    }
    if (readbackPath != nullptr && !rb.finish(*gfx, readbackPath, px.data())) {
        std::fprintf(stderr, "f3dreplay: read-back check failed\n");
        return 4;
    }
    std::printf("f3dreplay: %s -> %s: %zu lists widened, %d unresolved G_DL\n", argv[1], argv[2], sDls.size(),
                sUnresolved);
    return sUnresolved != 0 ? 3 : 0;
}
