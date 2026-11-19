/* port/gfx/fast3d: f3dreplay, render a DLMP dump (../dlump.h) with libultraship's
 * Fast3D interpreter (pinned + the plan-14 patch series) and write a PNG.
 * PC port plan section 19.6. 64-bit host, EGL offscreen, no SDL / ImGui.
 *
 *   f3dreplay in.dlmp out.png [--then in2.dlmp ...] [--scale N] [--ucode f3dex|line3d] [--readback out.rb] [--fb-out out.rgba16]
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
 * PNG: each RGBA5551 channel must equal the 8-bit one >> 3 (row 0 = the top).
 *
 * The dump's DLUMP_FB_* chunks (the framebuffer model, plan 19.10: what the game's CPU read from
 * or wrote into the colour / depth images) are replayed as the in-process renderer serves them
 * (FbModel below); --fb-out writes the read-back colour image (320x240 big-endian RGBA5551).
 *
 * --then (repeatable, run 849): the later gfx tasks of the same frame, drawn into the SAME frame
 * after in.dlmp, as the RDP does: one Run whose root calls each task's root in order, so the colour
 * and depth images carry across the tasks (a split-screen frame is one task per half, then the
 * overlay task; replayed alone, each shows the rest of the screen black). Each dump's lists are
 * widened from its own words (the game patches shared lists between tasks: LOD / view toggles),
 * the other data is mapped once, the later dump winning where they differ (reported). A task that
 * draws only into a narrower offscreen image (render to texture, run 855) is rendered in a pass of its
 * own first and its pixels are fed to the tasks that sample that image (Offscreen below). */

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
#ifdef USE_OPENGLES
#include <GLES3/gl3.h> /* f3dreplay-gles (plan 31) */
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

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
#include "port_fast3d_n64.h"
#include "egl_window.h"

namespace Fast {
void GfxSetInstance(std::shared_ptr<Interpreter> gfx);
}

/* ---------------- the dump ---------------- */

struct Chunk {
    uint32_t kind, addr, size, aux;
    const uint8_t* data;
};

struct WideDl;
/* one gfx task's dump; sDumps is reserved up front, so the Chunk data pointers stay valid */
struct Dump {
    std::vector<uint8_t> file;
    const DlumpHeader* hdr;
    std::vector<Chunk> chunks;
    std::vector<WideDl>* dls;
};
static std::vector<Dump> sDumps;
static std::vector<uint8_t> sFile;
static const DlumpHeader* sHdr;
static std::vector<Chunk> sChunks; /* every dump's chunks, in task order */

static bool load_one(const char* path) {
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

/* load a dump into sDumps (load_one parses into the s* scratch globals; sChunks then accumulates) */
static bool load(const char* path) {
    std::vector<Chunk> before = std::move(sChunks);
    sChunks.clear();
    sFile.clear();
    if (!load_one(path)) {
        return false;
    }
    Dump d;
    d.file = std::move(sFile); /* the buffer moves, the chunk pointers into it stay valid */
    d.hdr = sHdr;
    d.chunks = sChunks;
    d.dls = nullptr;
    before.insert(before.end(), sChunks.begin(), sChunks.end());
    sChunks = std::move(before);
    sDumps.push_back(std::move(d));
    return true;
}

static bool has_data(const Chunk& c) {
    return c.size != 0 && c.kind != DLUMP_ERROR && c.kind != DLUMP_IMAGE && c.kind != DLUMP_END &&
           c.kind != DLUMP_FB_READ && c.kind != DLUMP_FB_DEPTH && c.kind != DLUMP_FB_WRITE;
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
        /* each G_SETTIMG image base too (zero page): a LOADTILE of an image's lower rows dumps only those rows,
         * and Fast3D drops a G_SETTIMG whose address no registered range holds; the next load then reads the
         * previous image with the new offsets, past its end (run 861: Trick Attack's after-goal overlay).
         * Since patch 0010 Fast3D skips those loads instead (no stale read), but the texture would be lost. */
        if (c.kind == DLUMP_DL) {
            for (uint32_t i = 0; i + 8 <= c.size; i += 8) {
                uint32_t w[2];
                std::memcpy(w, c.data + i, 8);
                if ((w[0] >> 24) == 0xFD && w[1] >= 0x10000 && w[1] < 0x10000000) {
                    r.push_back({ w[1] & ~(pg - 1), (w[1] & ~(pg - 1)) + pg });
                }
            }
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
    /* task order: a later task's bytes win; count the non-list bytes that differ from an earlier task's */
    std::vector<std::pair<uint32_t, uint32_t>> earlier, mine; /* [lo, hi) */
    uint64_t differ = 0;
    for (const Dump& dp : sDumps) {
        mine.clear();
        for (const Chunk& c : dp.chunks) {
            if (!has_data(c)) {
                continue;
            }
            if (c.kind != DLUMP_DL && !earlier.empty()) {
                for (auto& r : earlier) {
                    uint32_t lo = std::max(r.first, c.addr), hi = std::min(r.second, c.addr + c.size);
                    for (uint32_t a = lo; a < hi; a++) {
                        differ += *(const uint8_t*)(uintptr_t)a != c.data[a - c.addr];
                    }
                }
            }
            std::memcpy((void*)(uintptr_t)c.addr, c.data, c.size);
            mine.push_back({ c.addr, c.addr + c.size });
        }
        earlier.insert(earlier.end(), mine.begin(), mine.end());
    }
    if (sDumps.size() > 1) {
        std::printf("f3dreplay: %zu tasks in one frame; %llu data bytes differ between them (the later task's kept)\n",
                    sDumps.size(), (unsigned long long)differ);
    }
    return true;
}

/* ---------------- the lists, widened to F3DGfx ---------------- */

struct WideDl {
    uint32_t addr, n;
    std::vector<Fast::F3DGfx> cmds;
};
static std::vector<WideDl> sDls; /* the dump being widened (then moved into its Dump) */
static int sUnresolved;
static size_t sDlCount;

static Fast::F3DGfx* wide(uint32_t addr) {
    for (WideDl& d : sDls) {
        if (addr >= d.addr && addr < d.addr + d.n * 8 && ((addr - d.addr) & 7) == 0) {
            return &d.cmds[(addr - d.addr) / 8];
        }
    }
    return nullptr;
}

static void widen_lists_one(const std::vector<Chunk>& chunks) {
    for (const Chunk& c : chunks) {
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

/* widen each dump against its own lists; returns each dump's root (nullptr if missing) */
static std::vector<Fast::F3DGfx*> widen_lists() {
    std::vector<Fast::F3DGfx*> roots;
    for (Dump& dp : sDumps) {
        sDls.clear();
        widen_lists_one(dp.chunks);
        roots.push_back(wide(dp.hdr->rootAddr));
        sDlCount += sDls.size();
        /* moving the vector keeps each list's F3DGfx array (and the G_DL pointers into it) in place */
        dp.dls = new std::vector<WideDl>(std::move(sDls));
        sDls.clear();
    }
    return roots;
}

/* ---------------- the framebuffer model (DLUMP_FB_*, plan 19.10) ---------------- */
/* What the game's CPU did with the colour / depth images around this task, replayed the way the
 * in-process renderer serves it (port_fast3d.cpp): FB_READ and FB_DEPTH go through patch 0003's
 * read-back, FB_WRITE rectangles are composited over the frame. A chunk applies only if the
 * task's last G_SETCIMG / G_SETZIMG names its image (low 24 bits). */
struct FbModel {
    struct Read {
        uint32_t addr;
        std::vector<uint16_t> px;
    };
    struct Depth {
        uint32_t addr, idx;
    };
    std::vector<Read> reads;
    std::vector<Depth> depths;
    std::vector<const Chunk*> writes;
    void request(Fast::Interpreter& gfx) {
        for (const Chunk& c : sChunks) {
            if (c.kind == DLUMP_FB_READ) {
                reads.push_back({ c.addr, std::vector<uint16_t>((size_t)320 * 240, 0) });
            } else if (c.kind == DLUMP_FB_DEPTH) {
                for (uint32_t i = 0; i + 4 <= c.size; i += 4) {
                    uint32_t idx;
                    std::memcpy(&idx, c.data + i, 4);
                    depths.push_back({ c.addr, idx });
                    gfx.ReadbackRequestDepth(idx % 320, idx / 320);
                }
            } else if (c.kind == DLUMP_FB_WRITE && c.size >= 8) {
                writes.push_back(&c);
            }
        }
        for (Read& r : reads) {
            gfx.ReadbackRequestRgba5551(0, 0, 320, 240, r.px.data(), true);
        }
    }
    /* frame: glReadPixels of the w x h window (bottom row first); CPU writes are composited into it */
    void finish(Fast::Interpreter& gfx, uint8_t* frame, uint32_t w, uint32_t h, const char* outPath) {
        const uint32_t cimg = (uint32_t)(uintptr_t)gfx.mRdp->color_image_address & 0xFFFFFF;
        const uint32_t zimg = (uint32_t)(uintptr_t)gfx.mRdp->z_buf_address & 0xFFFFFF;
        const uint32_t sc = w / 320;
        bool wrote = false;
        for (Read& r : reads) {
            bool drawn = (r.addr & 0xFFFFFF) == cimg;
            uint32_t lit = 0;
            for (const uint16_t& v : r.px) {
                const uint8_t* b = (const uint8_t*)&v; /* big-endian RGBA5551 in memory */
                lit += ((((uint32_t)b[0] << 8) | b[1]) & 0xFFFE) != 0; /* any colour bit set */
            }
            std::printf("fb read: colour image 0x%08x: %s, %u of %u pixels non-black\n", r.addr,
                        drawn ? "drawn by this task" : "NOT this task's colour image", lit, 320 * 240);
            if (drawn && outPath != nullptr && !wrote) {
                FILE* f = std::fopen(outPath, "wb");
                if (f != nullptr) {
                    std::fwrite(r.px.data(), 2, r.px.size(), f); /* big-endian RGBA5551, the N64 image */
                    std::fclose(f);
                    wrote = true;
                }
            }
        }
        if (!depths.empty()) {
            uint32_t far = 0, near = 0, other = 0;
            for (const Depth& d : depths) {
                uint16_t z = 0;
                if ((d.addr & 0xFFFFFF) != zimg || !gfx.ReadbackGetDepth(d.idx % 320, d.idx / 320, &z)) {
                    other++;
                } else {
                    (z == 0xFFFC ? far : near)++;
                    if (std::getenv("F3D_ZLOG") != nullptr) {
                        std::printf("fb depth: (%u,%u) z %04x\n", d.idx % 320, d.idx / 320, z);
                    }
                }
            }
            std::printf("fb depth: %zu pixels: %u far (0xFFFC), %u nearer, %u not served\n", depths.size(), far,
                        near, other);
        }
        for (const Chunk* c : writes) {
            uint16_t hdr[4];
            std::memcpy(hdr, c->data, 8);
            const uint32_t x0 = hdr[0], y0 = hdr[1], rw = hdr[2], rh = hdr[3];
            if ((c->addr & 0xFFFFFF) != cimg || c->size < 8 + rw * rh * 2 || x0 + rw > 320 || y0 + rh > 240) {
                std::printf("fb write: 0x%08x %ux%u skipped (not this task's colour image)\n", c->addr, rw, rh);
                continue;
            }
            for (uint32_t y = 0; y < rh * sc; y++) {
                for (uint32_t x = 0; x < rw * sc; x++) {
                    const uint8_t* p = c->data + 8 + ((y / sc) * rw + x / sc) * 2;
                    uint32_t v = ((uint32_t)p[0] << 8) | p[1]; /* big-endian RGBA5551 */
                    uint8_t* o = frame + ((size_t)(h - 1 - (y0 * sc + y)) * w + x0 * sc + x) * 4;
                    o[0] = (uint8_t)(((v >> 11) & 31) * 255 / 31);
                    o[1] = (uint8_t)(((v >> 6) & 31) * 255 / 31);
                    o[2] = (uint8_t)(((v >> 1) & 31) * 255 / 31);
                    o[3] = 255;
                }
            }
            std::printf("fb write: 0x%08x %ux%u at (%u, %u) composited\n", c->addr, rw, rh, x0, y0);
        }
    }
};

/* ---------------- render to texture (run 855) ---------------- */
/* A task whose colour images are all narrower than the 320-wide frame draws into an offscreen image
 * that a later task samples as a texture: the name-entry board (nameent.uso NameEntry_drawBoard)
 * shows the race replay that the 3D task renders into a 192x96 RGBA16 image (G_SETCIMG width 192,
 * scissor 192x96), and the overlay task loads that image (G_SETTIMG width 192). Fast3D ignores
 * G_SETCIMG and draws every task into the frame: the replay landed in the frame's top-left corner and
 * the board showed the image's stale memory (grey). With --then, such tasks are rendered first in a
 * pass of their own; the pass's pixels are read back as big-endian RGBA5551 into the arena where the
 * other tasks load that image, and the frame is then drawn from the other tasks alone. On the N64 the
 * board shows the image the 3D task drew in the PREVIOUS frame (the overlay task runs first); here it
 * is this frame's, one frame newer. */
struct Offscreen {
    uint32_t img24 = 0, width = 0, height = 0;
};

static bool offscreen_target(const Dump& dp, Offscreen* o) {
    std::vector<uint32_t> zimgs;
    for (const Chunk& c : dp.chunks) {
        if (c.kind == DLUMP_IMAGE && (c.aux >> 24) == 0xFE) {
            zimgs.push_back(c.addr & 0xFFFFFF);
        }
    }
    bool any = false;
    for (const Chunk& c : dp.chunks) {
        if (c.kind != DLUMP_IMAGE || (c.aux >> 24) != 0xFF) {
            continue;
        }
        const uint32_t width = (c.aux & 0xFFF) + 1, a = c.addr & 0xFFFFFF;
        if (width >= 320) {
            return false; /* draws into the frame */
        }
        if (std::find(zimgs.begin(), zimgs.end(), a) != zimgs.end() || ((c.aux >> 19) & 3) != 2) {
            continue; /* the Z image as colour (its clear), or not 16-bit */
        }
        o->img24 = a;
        o->width = width;
        any = true;
    }
    if (!any) {
        return false;
    }
    o->height = 0;
    for (const Chunk& c : dp.chunks) { /* the scissor's lower-right y (G_SETSCISSOR 0xED, 10.2) */
        if (c.kind != DLUMP_DL) {
            continue;
        }
        for (uint32_t i = 0; i + 8 <= c.size; i += 8) {
            uint32_t w[2];
            std::memcpy(w, c.data + i, 8);
            if ((w[0] >> 24) == 0xED) {
                o->height = std::max(o->height, (w[1] & 0xFFF) >> 2);
            }
        }
    }
    o->height = std::min<uint32_t>(o->height ? o->height : 240, 240);
    return true;
}

/* the offscreen pass's frame (glReadPixels, bottom row first, w x h at scale sc) -> the image in the
 * arena, at every texture chunk of the other tasks that lies in it */
static uint32_t offscreen_store(const Offscreen& o, const uint8_t* frame, uint32_t w, uint32_t h, uint32_t sc) {
    uint32_t bytes = 0;
    const uint32_t size = o.width * o.height * 2;
    for (const Chunk& c : sChunks) {
        if (c.kind != DLUMP_TEXTURE && c.kind != DLUMP_TLUT) {
            continue;
        }
        const uint32_t base = (c.addr & 0xFF000000u) | o.img24;
        if (c.addr < base || c.addr >= base + size) {
            continue;
        }
        const uint32_t end = std::min(c.addr + c.size, base + size);
        for (uint32_t a = c.addr & ~1u; a + 2 <= end; a += 2) {
            if (a < c.addr) {
                continue;
            }
            const uint32_t px = (a - base) / 2, x = px % o.width, y = px / o.width;
            if (x * sc >= w || y * sc >= h) {
                continue;
            }
            const uint8_t* f = frame + ((size_t)(h - 1 - y * sc) * w + x * sc) * 4;
            const uint32_t v = ((uint32_t)(f[0] >> 3) << 11) | ((uint32_t)(f[1] >> 3) << 6) | ((uint32_t)(f[2] >> 3) << 1) | 1;
            uint8_t* m = (uint8_t*)(uintptr_t)a;
            m[0] = (uint8_t)(v >> 8); /* big-endian, as the RDP writes it */
            m[1] = (uint8_t)v;
            bytes += 2;
        }
    }
    return bytes;
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
                /* the read-back quantizes with the RDP's magic-square dither (patch 0016, plan 25) */
                static const uint8_t magic[16] = { 0, 6, 1, 7, 4, 2, 5, 3, 3, 5, 2, 4, 7, 1, 6, 0 };
                const uint32_t d = magic[(y & 3) * 4 + (x & 3)];
                auto q = [d](uint8_t c) { return (uint32_t)(c >> 3) + ((c & 7) > d && (c >> 3) < 31 ? 1 : 0); };
                if (((v >> 11) & 31) != q(f[0]) || ((v >> 6) & 31) != q(f[1]) || ((v >> 1) & 31) != q(f[2])) {
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

#if defined(USE_OPENGLES) || defined(FAST_FORCE_FB)
/* f3dreplay-gles (plan 31): on OpenGL ES Fast3D always draws into its game framebuffer (LUS patch 20) and the port
 * presents it (port_fast3d.cpp's finish_view): copy it into the window the same way, rows flipped */
static void present_game_fb(Fast::Interpreter& gfx) {
    if (!gfx.mRendersToFb || gfx.mGfxFrameBuffer == 0) {
        return;
    }
    static GLuint fbo;
    if (fbo == 0) {
        glGenFramebuffers(1, &fbo);
    }
    const auto& vp = gfx.mGameWindowViewport;
    const GLint y0 = (GLint)gfx.mGfxCurrentWindowDimensions.height - (GLint)(vp.y + vp.height);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)gfx.mGfxFrameBuffer, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);
    glBlitFramebuffer(0, (GLint)gfx.mCurDimensions.height, (GLint)gfx.mCurDimensions.width, 0, (GLint)vp.x, y0,
                      (GLint)(vp.x + vp.width), y0 + (GLint)vp.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#else
static void present_game_fb(Fast::Interpreter&) {
}
#endif

int main(int argc, char** argv) {
    int scale = 1;
    const char* readbackPath = nullptr;
    const char* fbOutPath = nullptr;
    UcodeHandlers ucode = ucode_f3dex_line3d;
    if (argc < 3) {
        std::fprintf(stderr, "usage: f3dreplay in.dlmp out.png [--then in2.dlmp ...] [--scale N] [--ucode f3dex|line3d] [--readback out.rb] [--fb-out out.rgba16]\n");
        return 2;
    }
    for (int i = 3; i + 1 < argc; i++) {
        if (std::strcmp(argv[i], "--scale") == 0) {
            scale = std::max(1, std::atoi(argv[++i]));
        } else if (std::strcmp(argv[i], "--ucode") == 0) {
            ucode = std::strcmp(argv[++i], "f3dex") == 0 ? ucode_f3dex : ucode_f3dex_line3d;
        } else if (std::strcmp(argv[i], "--readback") == 0) {
            readbackPath = argv[++i];
        } else if (std::strcmp(argv[i], "--fb-out") == 0) {
            fbOutPath = argv[++i];
        } else if (std::strcmp(argv[i], "--then") == 0) {
            ++i; /* the later tasks: collected below */
        }
    }
    std::vector<const char*> paths = { argv[1] };
    for (int i = 3; i + 1 < argc; i++) {
        if (std::strcmp(argv[i], "--then") == 0) {
            paths.push_back(argv[++i]);
        }
    }
    sDumps.reserve(paths.size());
    for (const char* p : paths) {
        if (!load(p)) {
            return 1;
        }
    }
    if (!map_arena()) {
        return 1;
    }
    std::vector<Fast::F3DGfx*> roots = widen_lists();
    for (size_t i = 0; i < roots.size(); i++) {
        if (roots[i] == nullptr) {
            std::fprintf(stderr, "f3dreplay: the root list 0x%08x is not in %s\n", sDumps[i].hdr->rootAddr, paths[i]);
            return 1;
        }
    }
    /* render to texture: with more than one task, the offscreen ones get a pass of their own */
    std::vector<size_t> offTasks;
    std::vector<Offscreen> offs;
    if (roots.size() > 1) {
        for (size_t i = 0; i < roots.size(); i++) {
            Offscreen o;
            if (offscreen_target(sDumps[i], &o)) {
                offTasks.push_back(i);
                offs.push_back(o);
            }
        }
        if (offTasks.size() == roots.size()) {
            offTasks.clear(); /* nothing draws the frame: render them as they are */
            offs.clear();
        }
    }
    std::vector<Fast::F3DGfx*> offRoots, onRoots;
    for (size_t i = 0; i < roots.size(); i++) {
        (std::find(offTasks.begin(), offTasks.end(), i) != offTasks.end() ? offRoots : onRoots).push_back(roots[i]);
    }
    /* more than one task: a frame list that calls each task's root in turn */
    std::vector<Fast::F3DGfx> frame, offFrame;
    auto chain = [](std::vector<Fast::F3DGfx>& lst, const std::vector<Fast::F3DGfx*>& rs) -> Fast::F3DGfx* {
        if (rs.size() == 1) {
            return rs[0];
        }
        lst.resize(rs.size() + 1);
        for (size_t i = 0; i < rs.size(); i++) {
            lst[i].words.w0 = 0x06000000u; /* G_DL, push: each root ends in G_ENDDL */
            lst[i].words.w1 = (uintptr_t)rs[i];
        }
        lst[rs.size()].words.w0 = 0xB8000000u;
        lst[rs.size()].words.w1 = 0;
        return lst.data();
    };
    Fast::F3DGfx* root = chain(frame, onRoots);
    Fast::F3DGfx* offRoot = offRoots.empty() ? nullptr : chain(offFrame, offRoots);
    /* The depth image carries over between frames: 1080 clears Z (a FILLRECT with the colour image set to the
     * Z buffer) before its G_SETZIMG, so the interpreter knows the Z address only from the previous frame.
     * Fast3D treats that fill as a depth clear only when the colour image equals its current Z address; in a
     * one-dump replay it is still unset, and the fill paints 0xFFFC as colour (near-white, plan 19.9). Seed it
     * with the dump's first G_SETZIMG through a prologue list: SETZIMG, then G_DL to the root. */
    std::vector<Fast::F3DGfx> prologue;
    for (WideDl& d : *sDumps[0].dls) {
        for (Fast::F3DGfx& g : d.cmds) {
            if (prologue.empty() && ((g.words.w0 >> 24) & 0xFF) == 0xFE) {
                prologue.resize(3);
                prologue[0].words.w0 = g.words.w0;
                prologue[0].words.w1 = g.words.w1;
                prologue[1].words.w0 = 0x06000000u;
                prologue[1].words.w1 = (uintptr_t)root;
                prologue[2].words.w0 = 0xB8000000u;
                prologue[2].words.w1 = 0;
            }
        }
    }
    if (!prologue.empty()) {
        root = prologue.data();
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
    port_fast3d_n64_rapi(&rapi); /* plan 25: N64 clipping, shading, filtering */
    port_fast3d_n64_interp();
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
    if (offRoot != nullptr) {
        /* the offscreen pass (render to texture, above); its Z clear is its own FILLRECT on the Z image */
        std::vector<Fast::F3DGfx> offPrologue; /* its own first G_SETZIMG, as for the frame below */
        for (WideDl& d : *sDumps[offTasks[0]].dls) {
            for (Fast::F3DGfx& g : d.cmds) {
                if (offPrologue.empty() && ((g.words.w0 >> 24) & 0xFF) == 0xFE) {
                    offPrologue.resize(3);
                    offPrologue[0].words.w0 = g.words.w0;
                    offPrologue[0].words.w1 = g.words.w1;
                    offPrologue[1].words.w0 = 0x06000000u;
                    offPrologue[1].words.w1 = (uintptr_t)offRoot;
                    offPrologue[2].words.w0 = 0xB8000000u;
                    offPrologue[2].words.w1 = 0;
                }
            }
        }
        if (!offPrologue.empty()) {
            offRoot = offPrologue.data();
        }
        gfx->StartFrame();
        gfx->Run((Gfx*)offRoot, noReplacements);
        present_game_fb(*gfx);
        glFinish();
        std::vector<uint8_t> opx((size_t)w * h * 4);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, (GLsizei)w, (GLsizei)h, GL_RGBA, GL_UNSIGNED_BYTE, opx.data());
        for (size_t k = 0; k < offs.size(); k++) {
            uint32_t n = offscreen_store(offs[k], opx.data(), w, h, scale);
            std::printf("f3dreplay: task %zu renders to texture: image 0x%06x %ux%u, %u texture bytes fed to the "
                        "other tasks\n", offTasks[k], offs[k].img24, offs[k].width, offs[k].height, n);
        }
    }
    FbModel fb;
    fb.request(*gfx);
    gfx->StartFrame();
    gfx->Run((Gfx*)root, noReplacements);
    present_game_fb(*gfx);
    glFinish();

    std::vector<uint8_t> px((size_t)w * h * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, (GLsizei)w, (GLsizei)h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    fb.finish(*gfx, px.data(), w, h, fbOutPath);
    if (!write_png(argv[2], px.data(), (int)w, (int)h)) {
        std::fprintf(stderr, "f3dreplay: cannot write %s\n", argv[2]);
        return 1;
    }
    if (readbackPath != nullptr && !rb.finish(*gfx, readbackPath, px.data())) {
        std::fprintf(stderr, "f3dreplay: read-back check failed\n");
        return 4;
    }
    std::printf("f3dreplay: %s -> %s: %zu lists widened, %d unresolved G_DL\n", argv[1], argv[2], sDlCount,
                sUnresolved);
    return sUnresolved != 0 ? 3 : 0;
}
