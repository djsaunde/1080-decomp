/* port/gfx/fast3d: presenter, the live window of the 32-bit game on a machine with no 32-bit GL
 * (PC port plan 19.11, option (c) of 19.1). 64-bit, X11 + EGL, libultraship's Fast3D.
 *
 *   presenter [--shm NAME] [--scale N] [--png DIR] [--png-every N] [--png-steps A-B,C,...] [--frames N]
 *             [--inject STEP:KEY:d|u,...] [--js /dev/input/js0] [--stay]
 *             [--mute] [--audio-latency MS] [--audio-dump FILE] [--audio-stats] [--audio-open-delay MS]
 *             [--input SCRIPT]
 *
 * It creates /dev/shm/NAME (live_shm.h), opens a window, and then, while the game
 * (tenshoe with PORT_LIVE=NAME, port/gfx/port_live.c) runs:
 *   - renders each frame the game publishes (one game step's gfx tasks, each the DLMP image
 *     PORT_GFX_DUMP would write) into the window, the way f3dreplay --then composites a step;
 *     when it falls behind it renders only the newest frame;
 *   - turns the keyboard (and a Linux joystick, if one is present: /dev/input/js0, else the
 *     first evdev gamepad, live_gamepad.cpp) into pad 1 and writes it back into the shared
 *     header, where the game's host_read_pads picks it up each step;
 *   - plays the game's audio from the shm audio ring through PulseAudio on its own thread
 *     (live_audio.cpp, plan 19.12; --mute drains it silently, --audio-latency sets the buffer,
 *     default 80 ms, --audio-dump FILE writes the raw PCM received, --audio-stats prints a summary line
 *     (port/tools/audio-check.sh), --audio-open-delay MS waits that long after the window is mapped
 *     before opening PulseAudio: default 2500, see live_audio.h on WSLg).
 * port/tools/play.sh starts both.
 *
 * The memory image. As in f3dreplay, every data chunk is copied to ITS OWN (32-bit) address
 * in this 64-bit process, so the pointer operands of the commands are valid as they are; only
 * the lists are widened to 16-byte F3DGfx with their G_DL operands rewritten. Unlike the
 * one-shot f3dreplay, the arena persists across frames: 1 MB blocks are mapped on first use
 * (MAP_FIXED_NOREPLACE) and registered with Fast3D (patch 0005). Fast3D's texture cache is
 * keyed by address, so a texture or TLUT chunk whose bytes differ from what the arena held
 * invalidates the textures around it (patch 0004) before it is copied in.
 *
 * Keys (pad 1): arrows / WASD = stick, X or Space = A, Z = B, C = Z trigger, Enter = START,
 * Q = L, E = R, I / J / K / L = C-up / left / down / right, T / F / G / H = D-pad,
 * Shift = half stick, Esc = quit.
 *
 * --inject (tests): at the first rendered frame of game step >= STEP, send KEY (an X keysym
 * name: Return, x, Left, ...) as a synthetic KeyPress (d) / KeyRelease (u) to the window with
 * XSendEvent; it comes back through the X server into the same event loop as a real key.
 * --png DIR writes DIR/live_SEQ_sSTEP.png every --png-every frames (default 0 = never).
 * --fb-log prints the render-to-texture passes and read-back replies of every frame; --no-fb-reply
 * creates no reply file (the game keeps its headless depth-clear model: an A/B switch).
 * --frames N exits after N rendered frames (tells the game to quit). --stay keeps the window
 * open after the game ends, and a game started again on the same name (PORT_LIVE) is shown.
 *
 * --input SCRIPT (run 865): pad 1..4 follow a port/host/input_script.c script (tenshoe --input's
 * format and step timing) instead of the keyboard / joystick, written into the shm pads in lockstep
 * with the game's reads (live_shm.h padScripted): the script reaches the game through the same
 * presenter -> shm -> host_read_pads path as live input, and a race script plays out exactly as
 * headless. --inject keys still go through the window (F11, Esc), not into the pads.
 *
 * Lifetime: the game ending (or dying: checked every second) closes the presenter (unless --stay);
 * Esc / the window's close button / SIGINT / SIGTERM / SIGHUP / a lost X display tell the game to
 * quit (shm quit flag), and a game whose presenter was killed ends by itself (port_live.c checks the
 * presenter's pid). The window opens before the shm file exists, so a missing display fails before
 * any game can attach. At exit the frame times are summarised (render ms mean / p50 / p95 / p99 /
 * max, frames over one 30 Hz step, frames skipped because the presenter was behind).
 *
 * Read-backs: the frame's DLUMP_FB_READ / FB_DEPTH requests are answered after its Run (the -fb reply
 * file, run 859); a depth read the game makes now (LiveDepthAsk, plan 22.10) is answered from the last
 * frame drawn while the game waits, as the native game reads it (plan 22.4). A frame published while
 * osViBlack is on (LiveFrame.flags) is drawn and shown black (plan 22.10). CPU writes (FB_WRITE, debug
 * paths only) are not composited; see live_shm.h. */

#include <fcntl.h>
#include <linux/joystick.h>
#include <poll.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <thread>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <EGL/egl.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
/* Xlib's macros collide with names in the LUS headers */
#undef None
#undef Status
#undef Bool
#undef True
#undef False
#undef Success
#undef Always

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
#include "../live_shm.h"
#include "../../host.h" /* HostPad + the script reader (port/host/input_script.c, linked in) */
}
#include "../replay/egl_png.h"
#include "live_audio.h"
#include "live_gamepad.h"
#include "live_window.h"
#include "live_backend.h"
#include "port_fast3d_n64.h"

namespace Fast {
void GfxSetInstance(std::shared_ptr<Interpreter> gfx);
}

static double now_s() {
    return live_now_s();
}

/* the window, Fast3D's backend over it and the input: live_window.cpp / live_backend.h (shared with the
 * native 64-bit host, plan 21.9) */
using Win = LiveX11;
using Input = LiveInput;
static Win sWin;

/* ---------------- the persistent memory image ---------------- */

struct Arena {
    static constexpr uint64_t BLOCK = 1u << 20;
    std::set<uint64_t> blocks; /* mapped 1 MB blocks (base addresses) */
    std::set<uint64_t> failed;
    bool ensure(uint32_t addr, uint32_t size) {
        uint64_t a = addr & ~(BLOCK - 1), e = ((uint64_t)addr + size + BLOCK - 1) & ~(BLOCK - 1);
        bool ok = true;
        for (; a < e; a += BLOCK) {
            if (blocks.count(a) != 0) {
                continue;
            }
            if (failed.count(a) != 0) {
                ok = false;
                continue;
            }
            void* p = mmap((void*)a, BLOCK, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
            if (p != (void*)a) {
                if (p != MAP_FAILED) {
                    munmap(p, BLOCK);
                }
                std::fprintf(stderr, "presenter: cannot map the game's 0x%llx block at its own address\n",
                             (unsigned long long)a);
                failed.insert(a);
                ok = false;
                continue;
            }
            blocks.insert(a);
            gfx_register_raw_address_range((const void*)a, BLOCK); /* patch 0005 */
        }
        return ok;
    }
};

/* ---------------- one frame ---------------- */

struct Chunk {
    uint32_t kind, addr, size, aux;
    const uint8_t* data;
};

struct WideDl {
    uint32_t addr, n;
    std::vector<Fast::F3DGfx> cmds;
};

struct FrameStats {
    uint64_t bytes = 0, texBytesChanged = 0;
    uint32_t tasks = 0, invalidations = 0, unresolved = 0, badTasks = 0;
    uint32_t rttTasks = 0, rttBytes = 0;                             /* render to texture */
    uint32_t depthFar = 0, depthNear = 0, depthOther = 0, colorServed = 0; /* the replies */
};

/* what one task needs beyond its lists: render to texture, its Z image, its read requests */
struct TaskInfo {
    Fast::F3DGfx* root = nullptr;
    bool off = false;                   /* draws only into an offscreen image (render to texture) */
    uint32_t img24 = 0, width = 0, height = 0;
    bool hasZimg = false;
    Fast::F3DGfx zimg{};                /* its first G_SETZIMG (the prologue, plan 19.9) */
};

/* f3dreplay's offscreen_target (run 855): every colour image narrower than 320, 16-bit, not the Z image;
 * the height from the scissor */
static bool offscreen_target(const std::vector<Chunk>& chunks, TaskInfo* t) {
    std::vector<uint32_t> zimgs;
    for (const Chunk& c : chunks) {
        if (c.kind == DLUMP_IMAGE && (c.aux >> 24) == 0xFE) {
            zimgs.push_back(c.addr & 0xFFFFFF);
        }
    }
    bool any = false;
    for (const Chunk& c : chunks) {
        if (c.kind != DLUMP_IMAGE || (c.aux >> 24) != 0xFF) {
            continue;
        }
        const uint32_t width = (c.aux & 0xFFF) + 1, a = c.addr & 0xFFFFFF;
        if (width >= 320) {
            return false;
        }
        if (std::find(zimgs.begin(), zimgs.end(), a) != zimgs.end() || ((c.aux >> 19) & 3) != 2) {
            continue;
        }
        t->img24 = a;
        t->width = width;
        any = true;
    }
    if (!any) {
        return false;
    }
    t->height = 0;
    for (const Chunk& c : chunks) {
        if (c.kind != DLUMP_DL) {
            continue;
        }
        for (uint32_t i = 0; i + 8 <= c.size; i += 8) {
            uint32_t w[2];
            std::memcpy(w, c.data + i, 8);
            if ((w[0] >> 24) == 0xED) {
                t->height = std::max(t->height, (w[1] & 0xFFF) >> 2);
            }
        }
    }
    t->height = std::min<uint32_t>(t->height ? t->height : 240, 240);
    return true;
}

static bool has_data(uint32_t kind) {
    return kind == DLUMP_TASK || kind == DLUMP_VTX || kind == DLUMP_MTX || kind == DLUMP_MOVEMEM ||
           kind == DLUMP_TEXTURE || kind == DLUMP_TLUT;
}

struct Renderer {
    std::shared_ptr<Ship::ConsoleVariable> cvars;
    std::shared_ptr<Ship::ResourceManager> resources;
    LiveWindow window{ &sWin };
    std::unique_ptr<Fast::GfxRenderingAPIOGL> rapi;
    std::shared_ptr<Fast::Interpreter> gfx;
    Arena arena;
    /* the lists of the current frame: one vector per task (kept until the next frame) */
    std::vector<std::unique_ptr<std::vector<WideDl>>> dls;
    std::vector<Fast::F3DGfx> frameList, prologue, offList, offPrologue;
    /* the current frame's tasks, texture ranges (task, addr, size) and read requests */
    std::vector<TaskInfo> infos;
    struct TexRange {
        size_t task;
        uint32_t addr, size;
    };
    std::vector<TexRange> texRanges;
    std::vector<uint32_t> fbReads;                         /* colour images (FB_READ addr) */
    std::vector<std::pair<uint32_t, uint32_t>> fbDepths;   /* (Z image, pixel index) */
    std::set<std::pair<uint32_t, uint32_t>> fbDepthSet, fbAsked; /* all answered / the ones the game asked */
    std::vector<std::vector<uint16_t>> rttPixels, readPixels;
    LiveFbReply* reply = nullptr;                          /* /dev/shm/NAME-fb */
    uint32_t lastZimg = 0, lastStep = 0; /* the Z image (low 24 bits) and step of the last frame drawn (plan 22.10) */

    void init(uint32_t w, uint32_t h) {
        cvars = std::make_shared<Ship::ConsoleVariable>();
        resources = std::make_shared<Ship::ResourceManager>();
        rapi = std::make_unique<Fast::GfxRenderingAPIOGL>(cvars, resources);
        gfx = std::make_shared<Fast::Interpreter>();
        Fast::GfxSetInstance(gfx);
        gfx->Init(&window, rapi.get(), "1080", false, w, h, 0, 0, cvars, resources);
        Fast::gfx_set_target_ucode(ucode_f3dex_line3d); /* after Init, which resets it to F3DEX2 */
        port_fast3d_n64_rapi(rapi.get()); /* plan 25: N64 clipping, shading, filtering */
        port_fast3d_n64_interp();
        gfx->SetGfxDebugger(std::make_shared<Fast::GfxDebugger>());
        gfx->SetNativeDimensions(320.0f, 240.0f);
    }

    static Fast::F3DGfx* wide(std::vector<WideDl>& v, uint32_t addr) {
        for (WideDl& d : v) {
            if (addr >= d.addr && addr < d.addr + d.n * 8 && ((addr - d.addr) & 7) == 0) {
                return &d.cmds[(addr - d.addr) / 8];
            }
        }
        return nullptr;
    }

    /* one task's DLMP image at p (n bytes left): its data into the arena, its lists widened;
     * returns the image's size (0 = malformed) and its widened root in *root */
    size_t task(const uint8_t* p, size_t n, Fast::F3DGfx** root, FrameStats& st) {
        const DlumpHeader* hdr = (const DlumpHeader*)p;
        const size_t taskIdx = dls.size();
        TaskInfo info;
        *root = nullptr;
        if (n < sizeof(DlumpHeader) || std::memcmp(hdr->magic, DLUMP_MAGIC, 8) != 0 || hdr->version != DLUMP_VERSION ||
            hdr->byteOrderMark != DLUMP_BYTE_ORDER_MARK) {
            return 0;
        }
        std::vector<Chunk> chunks;
        size_t off = hdr->headerSize;
        for (uint32_t i = 0;; i++) {
            if (off + sizeof(DlumpChunk) > n) {
                return 0;
            }
            const DlumpChunk* c = (const DlumpChunk*)(p + off);
            off += sizeof(DlumpChunk);
            if (c->kind == DLUMP_END) {
                break;
            }
            if (off + c->size > n || i > hdr->chunkCount) {
                return 0;
            }
            chunks.push_back({ c->kind, c->addr, c->size, c->aux, (const uint8_t*)(c + 1) });
            off += (c->size + 3) & ~3u;
        }
        /* every G_SETTIMG image base, mapped (and so vouched for, patch 0005) even when the dump only holds
         * rows further in: a LOADTILE of an image's lower rows dumps those rows, and the base can sit in a block
         * no chunk touches. Fast3D drops a G_SETTIMG it does not vouch for and the next load then reads the
         * PREVIOUS image with the new offsets, past its end (run 861: Trick Attack's after-goal overlay) */
        for (const Chunk& c : chunks) {
            if (c.kind != DLUMP_DL) {
                continue;
            }
            for (uint32_t i = 0; i + 8 <= c.size; i += 8) {
                uint32_t w[2];
                std::memcpy(w, c.data + i, 8);
                if ((w[0] >> 24) == 0xFD && w[1] >= 0x10000 && w[1] < 0x10000000) {
                    arena.ensure(w[1], 1);
                }
            }
        }
        /* the data, at its own addresses; a changed texture / TLUT drops the cached textures around it */
        for (const Chunk& c : chunks) {
            if (!has_data(c.kind) || c.size == 0 || !arena.ensure(c.addr, c.size)) {
                continue;
            }
            uint8_t* dst = (uint8_t*)(uintptr_t)c.addr;
            if ((c.kind == DLUMP_TEXTURE || c.kind == DLUMP_TLUT) && std::memcmp(dst, c.data, c.size) != 0) {
                /* the cache key is the G_SETTIMG address, which can precede a LOADTILE's first byte */
                uint32_t lo = c.addr > 0x8000 ? c.addr - 0x8000 : 0;
                gfx_texture_cache_invalidate_range((const void*)(uintptr_t)lo, c.addr + c.size - lo);
                st.invalidations++;
                st.texBytesChanged += c.size;
            }
            std::memcpy(dst, c.data, c.size);
        }
        /* render to texture, the texture ranges it may feed, the read requests (deduplicated per frame) */
        info.off = offscreen_target(chunks, &info);
        for (const Chunk& c : chunks) {
            if ((c.kind == DLUMP_TEXTURE || c.kind == DLUMP_TLUT) && c.size != 0) {
                texRanges.push_back({ taskIdx, c.addr, c.size });
            } else if (c.kind == DLUMP_FB_READ) {
                if (std::find(fbReads.begin(), fbReads.end(), c.addr) == fbReads.end() && fbReads.size() < LIVE_FB_COLOR) {
                    fbReads.push_back(c.addr);
                }
            } else if (c.kind == DLUMP_FB_DEPTH) {
                for (uint32_t i = 0; i + 4 <= c.size; i += 4) {
                    uint32_t idx;
                    std::memcpy(&idx, c.data + i, 4);
                    if (idx >= LIVE_FB_W * LIVE_FB_H) {
                        continue;
                    }
                    /* the pixel and its window: the game takes the answer a step or two later, when a
                     * moving sun samples neighbouring pixels (live_shm.h LIVE_FB_DEPTH_R) */
                    fbAsked.emplace(c.addr, idx);
                    const int px = (int)(idx % LIVE_FB_W), py = (int)(idx / LIVE_FB_W), R = LIVE_FB_DEPTH_R;
                    for (int y = std::max(0, py - R); y <= std::min(LIVE_FB_H - 1, py + R); y++) {
                        for (int x = std::max(0, px - R); x <= std::min(LIVE_FB_W - 1, px + R); x++) {
                            auto key = std::make_pair(c.addr, (uint32_t)(y * LIVE_FB_W + x));
                            if (fbDepths.size() < LIVE_FB_DEPTH && fbDepthSet.insert(key).second) {
                                fbDepths.push_back(key);
                            }
                        }
                    }
                }
            }
        }
        /* the lists, widened from this task's own words */
        auto v = std::make_unique<std::vector<WideDl>>();
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
            v->push_back(std::move(d));
        }
        for (WideDl& d : *v) {
            for (Fast::F3DGfx& g : d.cmds) {
                if (((g.words.w0 >> 24) & 0xFF) == 0x06) {
                    Fast::F3DGfx* t = wide(*v, (uint32_t)g.words.w1);
                    if (t != nullptr) {
                        g.words.w1 = (uintptr_t)t;
                    } else {
                        st.unresolved++;
                    }
                }
            }
        }
        *root = wide(*v, hdr->rootAddr);
        for (WideDl& d : *v) {
            for (Fast::F3DGfx& g : d.cmds) {
                if (!info.hasZimg && ((g.words.w0 >> 24) & 0xFF) == 0xFE) {
                    info.hasZimg = true;
                    info.zimg = g;
                }
            }
        }
        info.root = *root;
        infos.push_back(info);
        dls.push_back(std::move(v));
        return off;
    }

    /* a list that calls each root in turn behind a SETZIMG prologue (f3dreplay --then, plan 19.9: 1080
     * clears Z before its G_SETZIMG; harmless once the interpreter has seen a frame) */
    static Fast::F3DGfx* chain(std::vector<Fast::F3DGfx>& lst, std::vector<Fast::F3DGfx>& pro,
                               const std::vector<Fast::F3DGfx*>& rs, const TaskInfo& first) {
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

    /* the offscreen pass's image (read back, big-endian RGBA5551, width x height) -> the arena, at every
     * texture chunk of the onscreen tasks that lies in it (f3dreplay's offscreen_store); the textures
     * cached there are dropped */
    uint32_t rtt_store(const TaskInfo& o, const std::vector<uint16_t>& px) {
        uint32_t bytes = 0;
        const uint32_t size = o.width * o.height * 2;
        for (const TexRange& r : texRanges) {
            if (infos[r.task].off) {
                continue;
            }
            const uint32_t base = (r.addr & 0xFF000000u) | o.img24;
            if (r.addr < base || r.addr >= base + size || !arena.ensure(r.addr, r.size)) {
                continue;
            }
            const uint32_t end = std::min(r.addr + r.size, base + size);
            for (uint32_t a = (r.addr + 1) & ~1u; a + 2 <= end; a += 2) {
                std::memcpy((void*)(uintptr_t)a, &px[(a - base) / 2], 2); /* already big-endian */
                bytes += 2;
            }
            const uint32_t lo = r.addr > 0x8000 ? r.addr - 0x8000 : 0;
            gfx_texture_cache_invalidate_range((const void*)(uintptr_t)lo, end - lo);
        }
        return bytes;
    }

    /* render one LiveFrame (slot) into the window and present it */
    bool frame(const uint8_t* slot, FrameStats& st) {
        const LiveFrame* lf = (const LiveFrame*)slot;
        dls.clear();
        infos.clear();
        texRanges.clear();
        fbReads.clear();
        fbDepths.clear();
        fbDepthSet.clear();
        fbAsked.clear();
        size_t off = sizeof(LiveFrame);
        st.bytes = lf->bytes;
        for (uint32_t i = 0; i < lf->nTasks; i++) {
            Fast::F3DGfx* r;
            size_t used = task(slot + off, lf->bytes - off, &r, st);
            if (used == 0) {
                st.badTasks++;
                break;
            }
            off += used;
            if (r == nullptr) {
                st.badTasks++;
            }
        }
        /* the tasks to draw: render-to-texture ones apart, unless every task is one */
        std::vector<Fast::F3DGfx*> onRoots, offRoots;
        std::vector<size_t> offIdx;
        size_t withRoot = 0;
        for (const TaskInfo& t : infos) {
            withRoot += t.root != nullptr;
        }
        for (size_t i = 0; i < infos.size(); i++) {
            TaskInfo& t = infos[i];
            if (t.root == nullptr) {
                continue;
            }
            if (withRoot > 1 && t.off) {
                offRoots.push_back(t.root);
                offIdx.push_back(i);
            } else {
                onRoots.push_back(t.root);
            }
        }
        if (onRoots.empty()) {
            onRoots.swap(offRoots);
            for (size_t i : offIdx) {
                infos[i].off = false;
            }
            offIdx.clear();
        }
        st.tasks = (uint32_t)onRoots.size() + (uint32_t)offRoots.size();
        if (onRoots.empty()) {
            return false;
        }
        /* the window may have been resized: render at its 4:3 letterbox, straight into the window (the
         * viewport offsets are applied by Fast3D, and by patch 0003's read-backs) */
        uint32_t vx, vy, vw, vh;
        sWin.viewport(&vx, &vy, &vw, &vh);
        gfx->mGameWindowViewport.x = (int32_t)vx;
        gfx->mGameWindowViewport.y = (int32_t)vy;
        gfx->mGameWindowViewport.width = vw;
        gfx->mGameWindowViewport.height = vh;
        gfx->mCurDimensions.width = vw;
        gfx->mCurDimensions.height = vh;
        std::unordered_map<Mtx*, MtxF> noReplacements;
        /* the offscreen pass: its pixels are read back at the end of its Run (patch 0003) */
        if (!offRoots.empty()) {
            Fast::F3DGfx* offRoot = chain(offList, offPrologue, offRoots, infos[offIdx[0]]);
            rttPixels.resize(offIdx.size());
            for (size_t k = 0; k < offIdx.size(); k++) {
                const TaskInfo& o = infos[offIdx[k]];
                rttPixels[k].assign((size_t)o.width * o.height, 0);
                gfx->ReadbackRequestRgba5551(0, 0, o.width, o.height, rttPixels[k].data(), true);
            }
            gfx->StartFrame();
            gfx->Run((Gfx*)offRoot, noReplacements);
            for (size_t k = 0; k < offIdx.size(); k++) {
                st.rttBytes += rtt_store(infos[offIdx[k]], rttPixels[k]);
            }
            st.rttTasks = (uint32_t)offIdx.size();
        }
        /* the frame, from the first task's SETZIMG (f3dreplay); the game's read requests ride on its Run */
        Fast::F3DGfx* root = chain(frameList, prologue, onRoots, infos[0]);
        if (reply != nullptr) {
            readPixels.resize(fbReads.size());
            for (size_t k = 0; k < fbReads.size(); k++) {
                readPixels[k].assign((size_t)LIVE_FB_W * LIVE_FB_H, 0);
                gfx->ReadbackRequestRgba5551(0, 0, LIVE_FB_W, LIVE_FB_H, readPixels[k].data(), true);
            }
            for (const auto& d : fbDepths) {
                gfx->ReadbackRequestDepth(d.second % LIVE_FB_W, d.second / LIVE_FB_W);
            }
        }
        gfx->StartFrame();
        gfx->Run((Gfx*)root, noReplacements);
        lastZimg = (uint32_t)(uintptr_t)gfx->mRdp->z_buf_address & 0xFFFFFF;
        lastStep = lf->step;
        if (reply != nullptr) {
            answer(lf, st);
        }
        return true;
    }

    /* plan 22.10: a LiveDepthAsk, from the frame drawn last (still in Fast3D's framebuffer; the game asked
     * after publishing it): the asked pixel's +-LIVE_FB_DEPTH_R window, clipped to the image */
    void depth_now(LiveDepthAsk* a) {
        const uint32_t idx = a->idx < LIVE_FB_W * LIVE_FB_H ? a->idx : 0;
        const int px = (int)(idx % LIVE_FB_W), py = (int)(idx / LIVE_FB_W), R = LIVE_FB_DEPTH_R;
        const int x0 = std::max(0, px - R), y0 = std::max(0, py - R);
        const int x1 = std::min(LIVE_FB_W - 1, px + R), y1 = std::min(LIVE_FB_H - 1, py + R);
        a->x0 = (uint32_t)x0;
        a->y0 = (uint32_t)y0;
        a->w = (uint32_t)(x1 - x0 + 1);
        a->h = (uint32_t)(y1 - y0 + 1);
        a->step = lastStep;
        uint16_t z[LIVE_DEPTH_SIDE * LIVE_DEPTH_SIDE];
        a->ok = lastZimg != 0 && (a->img & 0xFFFFFF) == lastZimg && live_depth_now(*gfx, a->x0, a->y0, a->w, a->h, z);
        if (a->ok) {
            std::memcpy((void*)a->z, z, a->w * a->h * sizeof(z[0]));
        }
    }

    /* the replies (live_shm.h LiveFbReply): what this frame's Run read back, for the images it drew */
    void answer(const LiveFrame* lf, FrameStats& st) {
        const uint32_t cimg = (uint32_t)(uintptr_t)gfx->mRdp->color_image_address & 0xFFFFFF;
        const uint32_t zimg = (uint32_t)(uintptr_t)gfx->mRdp->z_buf_address & 0xFFFFFF;
        LiveFbReply* r = reply;
        __atomic_store_n(&r->seq, r->seq + 1, __ATOMIC_RELEASE); /* odd: writing */
        __atomic_thread_fence(__ATOMIC_SEQ_CST);
        r->frameSeq = lf->seq;
        r->step = lf->step;
        uint32_t n = 0;
        for (const auto& d : fbDepths) {
            uint16_t z = 0;
            if ((d.first & 0xFFFFFF) != zimg || !gfx->ReadbackGetDepth(d.second % LIVE_FB_W, d.second / LIVE_FB_W, &z)) {
                st.depthOther += fbAsked.count(d) != 0;
                continue;
            }
            if (fbAsked.count(d) != 0) {
                (z == 0xFFFC ? st.depthFar : st.depthNear)++;
            }
            r->depth[n].img = d.first;
            r->depth[n].idx = d.second;
            r->depth[n].z = z;
            n++;
        }
        r->nDepth = n;
        n = 0;
        for (size_t k = 0; k < fbReads.size(); k++) {
            if ((fbReads[k] & 0xFFFFFF) != cimg) {
                continue;
            }
            r->color[n].img = fbReads[k];
            std::memcpy(r->pixels[n], readPixels[k].data(), sizeof(r->pixels[n]));
            n++;
            st.colorServed++;
        }
        r->nColor = n;
        __atomic_thread_fence(__ATOMIC_SEQ_CST);
        __atomic_store_n(&r->seq, r->seq + 1, __ATOMIC_RELEASE); /* even: complete */
    }
    void present(const char* pngPath) {
        if (pngPath != nullptr) {
            glFinish();
            std::vector<uint8_t> px((size_t)sWin.w * sWin.h * 4);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, (GLsizei)sWin.w, (GLsizei)sWin.h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
            write_png(pngPath, px.data(), (int)sWin.w, (int)sWin.h);
        }
        gfx->EndFrame(); /* -> LiveWindow::SwapBuffersEnd -> eglSwapBuffers */
    }
};

/* ---------------- main ---------------- */

struct Inject {
    uint32_t step;
    KeySym ks;
    bool down;
    bool sent = false;
};

static volatile sig_atomic_t sStop;
static void on_signal(int) {
    sStop = 1;
}

/* the shm files, for the exits that do not return to main (a lost X display) */
static LiveShmHeader* sHdr;
static std::string sShmPath, sFbPath;
static int on_x_io_error(Display*) {
    std::fprintf(stderr, "presenter: lost the X display (the window server closed or restarted): telling the game "
                         "to quit\n");
    if (sHdr != nullptr) {
        __atomic_store_n(&sHdr->quit, 1, __ATOMIC_RELEASE);
    }
    if (!sShmPath.empty()) {
        unlink(sShmPath.c_str());
        unlink(sFbPath.c_str());
    }
    _exit(1);
}

/* the host calls port/host/input_script.c needs (--input) */
extern "C" void host_log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::fputs("presenter: ", stderr);
    std::vfprintf(stderr, fmt, ap);
    std::fputc('\n', stderr);
    va_end(ap);
}
extern "C" void host_fatal(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::fputs("presenter: ", stderr);
    std::vfprintf(stderr, fmt, ap);
    std::fputc('\n', stderr);
    va_end(ap);
    std::exit(1);
}
extern "C" int host_file_read_all(const char* path, void** data, host_u32* size) {
    FILE* f = std::fopen(path, "rb");
    if (f == nullptr) {
        return -1;
    }
    std::string b;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) {
        b.append(buf, n);
    }
    std::fclose(f);
    char* p = (char*)std::malloc(b.size() + 1);
    std::memcpy(p, b.data(), b.size());
    p[b.size()] = '\0';
    *data = p;
    *size = (host_u32)b.size();
    return 0;
}
extern "C" void host_free(void* p) {
    std::free(p);
}

/* --input: answer the game's pad reads from the script, in lockstep (live_shm.h padWant / padStep) */
static std::atomic<bool> sPadRun;
static void script_pad_thread(LiveShmHeader* H) {
    while (sPadRun.load()) {
        const live_u32 want = __atomic_load_n(&H->padWant, __ATOMIC_ACQUIRE);
        if (want != 0 && want != H->padStep) {
            HostPad hp[HOST_MAX_PADS];
            host_input_script_at(want - 1, hp);
            for (int i = 0; i < HOST_MAX_PADS && i < LIVE_PADS; i++) {
                H->pads[i].button = hp[i].button;
                H->pads[i].stick_x = hp[i].stick_x;
                H->pads[i].stick_y = hp[i].stick_y;
                H->pads[i].connected = hp[i].connected;
                H->pads[i].hasRumble = hp[i].hasRumble;
            }
            __atomic_fetch_add(&H->padSeq, 1, __ATOMIC_RELEASE);
            __atomic_store_n(&H->padStep, want, __ATOMIC_RELEASE);
        } else {
            usleep(50);
        }
    }
}

int main(int argc, char** argv) {
    std::string name = "1080live", pngDir, jsPath = "/dev/input/js0", inputScript;
    uint32_t scale = 3, pngEvery = 0, maxFrames = 0, slots = LIVE_DEFAULT_SLOTS, slotBytes = LIVE_DEFAULT_SLOT_BYTES;
    bool stay = false, fbLog = false, fbReply = true;
    LiveAudioOptions audio;
    std::vector<Inject> injects;
    std::vector<std::pair<uint32_t, uint32_t>> pngSteps; /* --png-steps ranges (empty: every step) */
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        const char* v = i + 1 < argc ? argv[i + 1] : nullptr;
        if (a == "--stay") {
            stay = true;
        } else if (a == "--mute") {
            audio.mute = true;
        } else if (a == "--fb-log") {
            fbLog = true;
        } else if (a == "--no-fb-reply") {
            fbReply = false;
        } else if (a == "--audio-stats") {
            audio.stats = true;
        } else if (v == nullptr) {
            std::fprintf(stderr, "presenter: %s needs a value\n", a.c_str());
            return 2;
        } else if (a == "--shm") {
            name = argv[++i];
        } else if (a == "--scale") {
            scale = std::max(1, std::atoi(argv[++i]));
        } else if (a == "--png") {
            pngDir = argv[++i];
        } else if (a == "--png-every") {
            pngEvery = (uint32_t)std::atoi(argv[++i]);
        } else if (a == "--png-steps") {
            /* FROM-TO[,FROM-TO...] (or single steps): only game steps in those ranges are written */
            std::string r = argv[++i];
            for (size_t p = 0; p < r.size();) {
                size_t e = r.find(',', p);
                std::string item = r.substr(p, e == std::string::npos ? std::string::npos : e - p);
                size_t d = item.find('-');
                uint32_t from = (uint32_t)std::strtoul(item.c_str(), nullptr, 10);
                uint32_t to = d == std::string::npos ? from : (uint32_t)std::strtoul(item.c_str() + d + 1, nullptr, 10);
                if (d != std::string::npos && d + 1 == item.size()) {
                    to = 0xffffffffu;
                }
                pngSteps.push_back({ from, to });
                p = e == std::string::npos ? r.size() : e + 1;
            }
        } else if (a == "--frames") {
            maxFrames = (uint32_t)std::atoi(argv[++i]);
        } else if (a == "--audio-latency") {
            audio.latencyMs = (uint32_t)std::max(20, std::atoi(argv[++i]));
        } else if (a == "--audio-open-delay") {
            audio.openDelayMs = (uint32_t)std::max(0, std::atoi(argv[++i]));
        } else if (a == "--audio-dump") {
            audio.dumpPath = argv[++i];
        } else if (a == "--input") {
            inputScript = argv[++i];
        } else if (a == "--js") {
            jsPath = argv[++i];
        } else if (a == "--slot-mb") {
            slotBytes = (uint32_t)std::atoi(argv[++i]) << 20;
        } else if (a == "--inject") {
            std::string s = argv[++i];
            size_t p = 0;
            while (p < s.size()) {
                size_t e = s.find(',', p);
                std::string item = s.substr(p, e == std::string::npos ? std::string::npos : e - p);
                size_t c1 = item.find(':'), c2 = item.rfind(':');
                if (c1 == std::string::npos || c2 == c1) {
                    std::fprintf(stderr, "presenter: bad --inject item '%s' (STEP:KEY:d|u)\n", item.c_str());
                    return 2;
                }
                Inject in;
                in.step = (uint32_t)std::strtoul(item.substr(0, c1).c_str(), nullptr, 10);
                in.ks = XStringToKeysym(item.substr(c1 + 1, c2 - c1 - 1).c_str());
                in.down = item.substr(c2 + 1) == "d";
                if (in.ks == NoSymbol) {
                    std::fprintf(stderr, "presenter: unknown keysym in '%s'\n", item.c_str());
                    return 2;
                }
                injects.push_back(in);
                p = e == std::string::npos ? s.size() : e + 1;
            }
        } else {
            std::fprintf(stderr, "usage: presenter [--shm NAME] [--scale N] [--png DIR] [--png-every N] [--frames N] "
                                 "[--inject STEP:KEY:d|u,...] [--js DEV] [--slot-mb N] [--stay] "
                                 "[--fb-log] [--no-fb-reply] [--mute] [--audio-latency MS] [--audio-dump FILE] [--audio-stats] "
                                 "[--audio-open-delay MS] [--input SCRIPT]\n");
            return 2;
        }
    }
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);
    signal(SIGHUP, on_signal);
    signal(SIGPIPE, SIG_IGN);
    if (!inputScript.empty() && host_input_script_open(inputScript.c_str()) != 0) {
        std::fprintf(stderr, "presenter: cannot use the input script %s\n", inputScript.c_str());
        return 2;
    }

    /* the window first: without a display no shm file is made, so no game attaches to a dead presenter */
    const uint32_t w = 320 * scale, h = 240 * scale;
    if (!sWin.open(w, h, "1080 Snowboarding (live)")) {
        return 1;
    }
    XSetIOErrorHandler(on_x_io_error);

    /* the shared memory: created here, the magic written last */
    std::string path = "/dev/shm/" + name;
    int fd = ::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600);
    size_t total = (size_t)LIVE_HEADER_BYTES + (size_t)slots * slotBytes;
    const size_t audioOffset = total; /* the audio ring after the slots (live_shm.h LiveAudio) */
    total += LIVE_AUDIO_DEFAULT_BYTES;
    if (fd < 0 || ftruncate(fd, (off_t)total) != 0) {
        std::fprintf(stderr, "presenter: cannot create %s\n", path.c_str());
        return 1;
    }
    uint8_t* shm = (uint8_t*)mmap(nullptr, total, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ::close(fd);
    if (shm == MAP_FAILED) {
        std::fprintf(stderr, "presenter: cannot map %s\n", path.c_str());
        return 1;
    }
    LiveShmHeader* H = (LiveShmHeader*)shm;
    std::memset(H, 0, sizeof(*H));
    H->version = LIVE_VERSION;
    H->slotCount = slots;
    H->slotBytes = slotBytes;
    H->presenterPid = (live_u32)getpid();
    H->pads[0].connected = 1;
    live_audio_layout((LiveAudio*)(shm + LIVE_AUDIO_HDR), (uint32_t)audioOffset, LIVE_AUDIO_DEFAULT_BYTES);
    /* depth reads now (plan 22.10): announced before the magic */
    LiveDepthAsk* depthAsk = (LiveDepthAsk*)(shm + LIVE_DEPTH_HDR);
    std::memset(depthAsk, 0, sizeof(*depthAsk));
    depthAsk->magic = LIVE_DEPTH_MAGIC;
    /* the read-back replies (run 859): a file of their own, ready before the magic */
    std::string fbPath = path + "-fb";
    LiveFbReply* reply = nullptr;
    int ffd = fbReply ? ::open(fbPath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0600) : -1;
    if (ffd >= 0 && ftruncate(ffd, (off_t)sizeof(LiveFbReply)) == 0) {
        void* m = mmap(nullptr, sizeof(LiveFbReply), PROT_READ | PROT_WRITE, MAP_SHARED, ffd, 0);
        if (m != MAP_FAILED) {
            reply = (LiveFbReply*)m;
            std::memset(reply, 0, sizeof(*reply));
            reply->magic = LIVE_FB_MAGIC;
            reply->version = LIVE_FB_VERSION;
        }
    }
    if (ffd >= 0) {
        ::close(ffd);
    }
    if (reply == nullptr && fbReply) {
        std::fprintf(stderr, "presenter: no %s: the game's read-backs will not be answered\n", fbPath.c_str());
    }
    sHdr = H;
    sShmPath = path;
    sFbPath = fbPath;
    std::thread padThread;
    if (!inputScript.empty()) {
        H->padScripted = 1;
        sPadRun = true;
        padThread = std::thread(script_pad_thread, H);
    }
    __atomic_store_n(&H->magic, LIVE_MAGIC, __ATOMIC_RELEASE);

    live_audio_start(shm, audio);
    Renderer R;
    R.init(w, h);
    R.reply = reply;
    Input in;
    in.openJoystick(jsPath.c_str());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    sWin.swap();
    std::printf("presenter: waiting for the game on %s (PORT_LIVE=%s)\n", path.c_str(), name.c_str());
    std::fflush(stdout);

    uint32_t rendered = 0, lastStep = 0, secFrames = 0;
    uint32_t secRtt = 0, secFar = 0, secNear = 0, secOther = 0, secColor = 0;
    uint64_t secBytes = 0;
    double secStart = now_s(), secRender = 0;
    bool quit = false;
    LivePad lastPad = {};
    std::vector<float> frameMs; /* every rendered frame's render time (the exit summary) */
    uint32_t skipped = 0, lastGamePid = 0, games = 0, blackFrames = 0, depthAnswered = 0, depthOk = 0;
    bool endedNoted = false;
    while (!quit && !sStop) {
        /* events */
        live_pump(sWin, in, lastStep);
        in.pollJoystick();
        if (in.quit) {
            quit = true;
        }
        LivePad pad = in.pad();
        if (inputScript.empty() && std::memcmp(&pad, &lastPad, sizeof(pad)) != 0) {
            H->pads[0] = pad;
            __atomic_fetch_add(&H->padSeq, 1, __ATOMIC_RELEASE);
            lastPad = pad;
        }
        /* frames */
        uint32_t head = __atomic_load_n(&H->head, __ATOMIC_ACQUIRE), tail = H->tail;
        if (head != tail) {
            /* render the newest; release the older slots first (the game may refill them) */
            skipped += head - tail - 1;
            __atomic_store_n(&H->tail, head - 1, __ATOMIC_RELEASE);
            const uint8_t* slot = shm + LIVE_HEADER_BYTES + (size_t)((head - 1) % slots) * slotBytes;
            const LiveFrame* lf = (const LiveFrame*)slot;
            FrameStats st;
            double t0 = now_s();
            bool ok = lf->bytes >= sizeof(LiveFrame) && lf->bytes <= slotBytes && R.frame(slot, st);
            lastStep = lf->step;
            if (ok && (lf->flags & LIVE_FRAME_BLACK) != 0) {
                /* osViBlack (plan 22.10, as native 22.5): drawn, its read-backs answered, shown black */
                sWin.black();
                blackFrames++;
            }
            if (ok) {
                char png[1024];
                const char* pp = nullptr;
                bool inSteps = pngSteps.empty();
                for (const auto& rg : pngSteps) {
                    inSteps |= lf->step >= rg.first && lf->step <= rg.second;
                }
                if (!pngDir.empty() && pngEvery != 0 && rendered % pngEvery == 0 && inSteps) {
                    std::snprintf(png, sizeof(png), "%s/live_%06u_s%06u.png", pngDir.c_str(), rendered, lf->step);
                    pp = png;
                }
                R.present(pp);
                rendered++;
                secFrames++;
                secBytes += st.bytes;
                secRtt += st.rttTasks;
                secFar += st.depthFar;
                secNear += st.depthNear;
                secOther += st.depthOther;
                secColor += st.colorServed;
                if (fbLog && st.rttTasks + st.depthFar + st.depthNear + st.depthOther + st.colorServed != 0) {
                    std::printf("presenter: frame %u step %u: render to texture %u (%u bytes), depth %u far / %u nearer / "
                                "%u not served, colour %u\n", rendered - 1, lf->step, st.rttTasks, st.rttBytes,
                                st.depthFar, st.depthNear, st.depthOther, st.colorServed);
                    if (R.reply != nullptr && R.reply->nDepth != 0) {
                        std::printf("presenter:   z");
                        for (uint32_t k = 0; k < R.reply->nDepth; k++) {
                            if (R.fbAsked.count({ R.reply->depth[k].img, R.reply->depth[k].idx }) == 0) {
                                continue; /* the window around them */
                            }
                            std::printf(" %u:%04x", R.reply->depth[k].idx, R.reply->depth[k].z);
                        }
                        std::printf("\n");
                    }
                }
            }
            secRender += now_s() - t0;
            frameMs.push_back((float)((now_s() - t0) * 1000.0));
            if (st.badTasks != 0 || st.unresolved != 0) {
                std::printf("presenter: step %u: %u malformed tasks, %u unresolved G_DL\n", lf->step, st.badTasks,
                            st.unresolved);
            }
            __atomic_store_n(&H->tail, head, __ATOMIC_RELEASE);
            /* injected keys: through the X server, back into the loop above */
            for (Inject& j : injects) {
                if (!j.sent && lf->step >= j.step) {
                    XEvent ev = {};
                    ev.xkey.type = j.down ? KeyPress : KeyRelease;
                    ev.xkey.display = sWin.dpy;
                    ev.xkey.window = sWin.win;
                    ev.xkey.root = DefaultRootWindow(sWin.dpy);
                    ev.xkey.keycode = XKeysymToKeycode(sWin.dpy, j.ks);
                    ev.xkey.same_screen = 1;
                    XSendEvent(sWin.dpy, sWin.win, 1, j.down ? KeyPressMask : KeyReleaseMask, &ev);
                    XFlush(sWin.dpy);
                    j.sent = true;
                }
            }
            if (maxFrames != 0 && rendered >= maxFrames) {
                quit = true;
            }
        } else if (const live_u32 ask = __atomic_load_n(&depthAsk->ask, __ATOMIC_ACQUIRE); ask != depthAsk->done) {
            /* a depth read (plan 22.10): every frame the game had published when it asked is in (tail), so the
             * framebuffer holds the newest of them */
            if ((int32_t)(H->tail - depthAsk->frames) >= 0) {
                R.depth_now(depthAsk);
                depthAnswered++;
                depthOk += depthAsk->ok;
                __atomic_store_n(&depthAsk->done, ask, __ATOMIC_RELEASE);
            }
        } else {
            if (__atomic_load_n(&H->gameExited, __ATOMIC_ACQUIRE)) {
                if (!stay) {
                    break;
                }
                if (!endedNoted) {
                    std::printf("presenter: the game ended; --stay: waiting for a new game on PORT_LIVE=%s\n",
                                name.c_str());
                    std::fflush(stdout);
                    endedNoted = true;
                }
            }
            struct pollfd pf = { ConnectionNumber(sWin.dpy), POLLIN, 0 };
            poll(&pf, 1, 1); /* X events or 1 ms, whichever first (a depth read waits for this loop) */
        }
        double t = now_s();
        if (t - secStart >= 1.0) {
            char title[160];
            std::snprintf(title, sizeof(title), "1080 Snowboarding (live) - step %u - %u fps", lastStep, secFrames);
            XStoreName(sWin.dpy, sWin.win, title);
            const LivePad shown = inputScript.empty() ? lastPad : H->pads[0];
            std::printf("presenter: %u frames/s, %.1f MB/frame, render %.1f ms/frame, step %u, dropped %u, skipped %u, "
                        "pads %04x %d,%d%s\n",
                        secFrames, secFrames ? secBytes / 1048576.0 / secFrames : 0.0,
                        secFrames ? secRender * 1000.0 / secFrames : 0.0, lastStep, H->dropped, skipped, shown.button,
                        shown.stick_x, shown.stick_y, live_audio_status().c_str());
            std::fflush(stdout);
            /* a (new) game attached: --stay shows a restarted game */
            const uint32_t gp = H->gamePid;
            if (gp != 0 && gp != lastGamePid) {
                if (games++ != 0) {
                    std::printf("presenter: game pid %u attached (game %u)\n", gp, games);
                }
                lastGamePid = gp;
                endedNoted = false;
            }
            /* a game that died without saying so (a crash) */
            if (H->gamePid != 0 && !__atomic_load_n(&H->gameExited, __ATOMIC_ACQUIRE) && kill((pid_t)H->gamePid, 0) != 0) {
                std::printf("presenter: the game (pid %u) is gone\n", H->gamePid);
                if (!stay) {
                    quit = true;
                }
                H->gamePid = 0;
            }
            secStart = t;
            secFrames = 0;
            secBytes = 0;
            secRender = 0;
        }
    }
    /* tell the game, give it a moment to notice, then remove the file */
    __atomic_store_n(&H->quit, 1, __ATOMIC_RELEASE);
    for (int i = 0; i < 200 && !__atomic_load_n(&H->gameExited, __ATOMIC_ACQUIRE) && H->gamePid != 0; i++) {
        usleep(10000);
    }
    const std::string audioEnd = live_audio_status();
    live_audio_stop();
    if (padThread.joinable()) {
        sPadRun = false;
        padThread.join();
    }
    std::printf("presenter: %u frames rendered, %u skipped (presenter behind), %u dropped by the game, %u too big%s\n",
                rendered, skipped, H->dropped, H->tooBig, audioEnd.c_str());
    std::printf("presenter: %u frames shown black (osViBlack), %u depth reads answered now (%u from the image)\n",
                blackFrames, depthAnswered, depthOk);
    if (!frameMs.empty()) {
        std::vector<float> v = frameMs;
        std::sort(v.begin(), v.end());
        double sum = 0;
        uint32_t over = 0;
        for (float x : v) {
            sum += x;
            over += x > 1000.0f / 30.0f;
        }
        auto pct = [&](double q) { return v[std::min(v.size() - 1, (size_t)(q * (double)v.size()))]; };
        std::printf("presenter: render ms: mean %.2f, p50 %.2f, p95 %.2f, p99 %.2f, max %.2f; %u frames over one "
                    "step (33.3 ms)\n", sum / v.size(), pct(0.5), pct(0.95), pct(0.99), v.back(), over);
    }
    unlink(path.c_str());
    unlink(fbPath.c_str());
    return 0;
}
