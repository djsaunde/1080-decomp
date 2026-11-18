/* n64ref: a headless N64 reference runner for the 1080 PC port (plan 22).
 *
 * mupen64plus-core (cached interpreter) + angrylion-rdp-plus (pixel-accurate software RDP, headless
 * output: headless_vdac.c) + the cxd4 LLE RSP. The frontend is also the input plugin: it plays
 * the port's .input scripts (port/host/input_script.c format) against the game's own step
 * counter, SysLib.frameCount, read from RDRAM, so the same script drives the emulator and the
 * port. Frames are written as PNG (the VI output, as a TV would show it).
 *
 *   n64ref --rom ROM --out DIR [--input SCRIPT] [--vis LIST] [--steps LIST] [--every-vi N]
 *          [--max-vi N] [--step-addr 0x8XXXXXXX] [--step-lag K] [--scan] [--log-steps]
 *          [--memsize 4|8] [--interp] [--sram FILE]
 *
 *   --vis 100,200-210     write vNNNNNN.png at these VI counts
 *   --steps 480,900       write sSSSSSS.png for these game steps: the last VI on which
 *                         frameCount == S + K (K = --step-lag, default 2: the frame the game built
 *                         in step S is scanned out while it runs step S + 2; measured over the
 *                         title demo, plan 22.2), i.e. the frame the port's PORT_NATIVE_PNG
 *                         native_sS.png / capture.sh's _sS.png show
 *   --interp              the pure interpreter (default: the cached interpreter; this build's
 *                         x86-64 dynarec dies in the boot code)
 *   --every-vi N          also every N VIs (vNNNNNN.png)
 *   --max-vi N            stop after N VIs (default: the last capture + 2, or 3000)
 *   --step-addr A         KSEG0 address of SysLib.frameCount (default 0x8005C6F4: gSysLib, bootup
 *                         Bss + 0x3C740, + 0x14; bootup's Bss is at 0x8001FFA0 on a 4 MiB console;
 *                         0 = none, steps read 0; --scan finds it again)
 *   --dump-ram LIST       write ram_vNNNNNN.bin (big-endian RDRAM) at these VIs
 *   --scan                print RDRAM words that count like frameCount (VIs 600 / 700 / 800)
 *   --log-steps           print "S step vi" whenever the step changes (steps.txt in DIR)
 *   --sram FILE           start from this SRAM image (copied into DIR; the port's .sram is
 *                         the same 32 KiB big-endian image)
 *   --vi-mode 0|1         angrylion VI mode: 0 = filtered, the TV picture (VI AA / divot / dither filter,
 *                         the VI's own scaling; default), 1 = unfiltered, the raw framebuffer (320x240
 *                         for 1080), pixel-aligned with the port's frames
 *   --fb                  write the framebuffer the VI scans out (VI_ORIGIN / VI_WIDTH, RGBA5551 or
 *                         RGBA8888 from RDRAM), unscaled: 320x240 for 1080, as the port renders it;
 *                         no VI filtering (the default writes the VI output)
 *   --input-shift K       the pads read at step S get the script's state of step S - K (default -1:
 *                         the PIF read of step S feeds pad_update of step S + 1; the port's
 *                         host_read_pads timing, measured on the menus, plan 22.2)
 *   --memsize 4|8         RDRAM MiB (default 4: a stock console without the Expansion Pak)
 *   --watch A:N[,A:N..]   every VI from step --watch-from on, append a record to DIR/watch.bin:
 *                         u32 vi, u32 step (little-endian), then the N bytes at each KSEG0 address A
 *                         (big-endian, as the N64 sees them). The last record of a step holds that step's
 *                         results (the game finishes its update within the step's 2 VIs); used for the
 *                         per-step rider state of plan 24 (port/tools/n64ref/vs2p-golden.py)
 *   --watch-from S        the first step --watch records (default 0)
 *
 * The ROM is only read (into memory). Saves go to DIR.
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>

#define M64P_CORE_PROTOTYPES 1
#include "m64p_common.h"
#include "m64p_config.h"
#include "m64p_debugger.h"
#include "m64p_frontend.h"
#include "m64p_plugin.h"
#include "m64p_types.h"

/* ---------------------------------------------------------------- core entry points */
static void *gCore;
static ptr_CoreStartup pCoreStartup;
static ptr_CoreShutdown pCoreShutdown;
static ptr_CoreAttachPlugin pCoreAttachPlugin;
static ptr_CoreDoCommand pCoreDoCommand;
static ptr_ConfigOpenSection pConfigOpenSection;
static ptr_ConfigSetParameter pConfigSetParameter;
static ptr_ConfigSaveFile pConfigSaveFile;
static ptr_DebugMemGetPointer pDebugMemGetPointer;
static ptr_PluginStartup pPluginStartup[4];

static void *must_sym(void *h, const char *n) {
    void *p = dlsym(h, n);
    if (!p) {
        fprintf(stderr, "n64ref: missing symbol %s\n", n);
        exit(2);
    }
    return p;
}

/* ---------------------------------------------------------------- options */
typedef struct {
    uint32_t a, b;
} Range;
static Range gVis[256], gSteps[256], gDumps[64];
static int gNVis, gNSteps, gNDumps;
static unsigned gEveryVi, gMaxVi, gStepLag = 2;
static uint32_t gStepAddr = 0x8005C6F4;
static int gInputShift = -1;
static int gScan, gLogSteps, gMemSize = 4, gInterp, gViMode, gRawFb;
static uint32_t *gViRegs;
static const char *gOut = ".";
static FILE *gStepLog;
static Range gWatch[16];
static int gNWatch;
static uint32_t gWatchFrom;
static FILE *gWatchFile;

static int parse_ranges(const char *s, Range *r, int max) {
    int n = 0;
    while (*s && n < max) {
        char *e;
        unsigned long a = strtoul(s, &e, 0), b = a;
        if (*e == '-') b = strtoul(e + 1, &e, 0);
        r[n].a = (uint32_t)a;
        r[n].b = (uint32_t)b;
        n++;
        if (*e == ',') e++;
        s = e;
    }
    return n;
}
static int in_ranges(const Range *r, int n, uint32_t v) {
    for (int i = 0; i < n; i++)
        if (v >= r[i].a && v <= r[i].b) return 1;
    return 0;
}
static uint32_t ranges_max(const Range *r, int n) {
    uint32_t m = 0;
    for (int i = 0; i < n; i++)
        if (r[i].b > m) m = r[i].b;
    return m;
}

/* ---------------------------------------------------------------- input script */
typedef struct {
    uint32_t first, last;
    uint16_t buttons;
    uint8_t pad, hasStick;
    int8_t x, y;
} Ev;
static Ev *gEv;
static int gNEv, gCapEv, gNumPads = 1;
static const struct {
    const char *n;
    uint16_t bit;
} kBtn[] = {
    { "A", 0x8000 },  { "B", 0x4000 },    { "Z", 0x2000 },    { "START", 0x1000 }, { "DU", 0x0800 },
    { "DD", 0x0400 }, { "DL", 0x0200 },   { "DR", 0x0100 },   { "UP", 0x0800 },    { "DOWN", 0x0400 },
    { "LEFT", 0x0200 }, { "RIGHT", 0x0100 }, { "L", 0x0020 }, { "R", 0x0010 },     { "CU", 0x0008 },
    { "CD", 0x0004 }, { "CL", 0x0002 },   { "CR", 0x0001 },
};
static Ev *add_ev(void) {
    if (gNEv == gCapEv) {
        gCapEv = gCapEv ? gCapEv * 2 : 64;
        gEv = realloc(gEv, gCapEv * sizeof *gEv);
    }
    memset(&gEv[gNEv], 0, sizeof *gEv);
    return &gEv[gNEv++];
}
static int parse_when(const char *t, uint32_t prev, uint32_t *f, uint32_t *l) {
    char *e;
    int rel = 0;
    if (*t == '+') rel = 1, t++;
    unsigned long a = strtoul(t, &e, 10), b;
    if (e == t) return -1;
    if (rel) a += prev;
    *f = *l = (uint32_t)a;
    if (*e == '-' || *e == ':') {
        char sep = *e;
        t = e + 1;
        if (sep == '-' && rel) {
            if (*t != '+') return -1;
            t++;
        }
        b = strtoul(t, &e, 10);
        if (e == t) return -1;
        if (sep == '-') {
            if (rel) b += prev;
            *l = (uint32_t)b;
        } else
            *l = (uint32_t)(a + b - 1);
    }
    return *e ? -1 : 0;
}
static void load_script(const char *path) {
    FILE *f = fopen(path, "r");
    char line[1024];
    uint32_t prev = 0;
    int ln = 0;
    if (!f) {
        perror(path);
        exit(2);
    }
    while (fgets(line, sizeof line, f)) {
        char *tok[64], *p = line, *h = strchr(line, '#');
        int n = 0;
        ln++;
        if (h) *h = 0;
        while ((p = strtok(n ? NULL : line, " \t\r\n")) && n < 64) tok[n++] = p;
        if (!n) continue;
        if (!strcasecmp(tok[0], "PADS") && n == 2) {
            gNumPads = atoi(tok[1]);
            continue;
        }
        if (!strcasecmp(tok[0], "RUMBLE")) continue;
        if (strcasecmp(tok[0], "F") || n < 2) {
            fprintf(stderr, "n64ref: %s:%d: bad line\n", path, ln);
            exit(2);
        }
        uint32_t a, b;
        if (parse_when(tok[1], prev, &a, &b)) {
            fprintf(stderr, "n64ref: %s:%d: bad step range\n", path, ln);
            exit(2);
        }
        prev = a;
        Ev *e = add_ev();
        e->first = a, e->last = b;
        for (int i = 2; i < n; i++) {
            const char *t = tok[i];
            if ((t[0] == 'P' || t[0] == 'p') && t[1] >= '1' && t[1] <= '4' && !t[2]) {
                if (e->buttons || e->hasStick) {
                    e = add_ev();
                    e->first = a, e->last = b;
                }
                e->pad = t[1] - '1';
                if (e->pad + 1 > gNumPads) gNumPads = e->pad + 1;
                continue;
            }
            if (!strcasecmp(t, "STICK") && i + 1 < n) {
                long x = strtol(tok[i + 1], &p, 10), y = (*p == ',') ? strtol(p + 1, NULL, 10) : 0;
                e->hasStick = 1;
                e->x = x < -128 ? -128 : x > 127 ? 127 : x;
                e->y = y < -128 ? -128 : y > 127 ? 127 : y;
                i++;
                continue;
            }
            int ok = 0;
            for (unsigned k = 0; k < sizeof kBtn / sizeof kBtn[0]; k++)
                if (!strcasecmp(t, kBtn[k].n)) e->buttons |= kBtn[k].bit, ok = 1;
            if (!ok && t[0] == '0' && (t[1] == 'x' || t[1] == 'X')) e->buttons |= (uint16_t)strtoul(t, NULL, 16), ok = 1;
            if (!ok) {
                fprintf(stderr, "n64ref: %s:%d: bad token %s\n", path, ln, t);
                exit(2);
            }
        }
    }
    fclose(f);
}

/* ---------------------------------------------------------------- RDRAM + step */
static uint32_t *gRam; /* host-order words */
static uint32_t gVi;
static uint32_t ram32(uint32_t a) {
    a &= 0x7FFFFF;
    if (a + 4 > (uint32_t)gMemSize << 20) return 0;
    return gRam[a >> 2];
}
static uint32_t cur_step(void) {
    return gStepAddr ? ram32(gStepAddr) : 0;
}

/* ---------------------------------------------------------------- input plugin (this exe) */
static CONTROL *gControls;
EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle h, void *c, void (*d)(void *, int, const char *)) {
    (void)h, (void)c, (void)d;
    return M64ERR_SUCCESS;
}
EXPORT m64p_error CALL PluginShutdown(void) { return M64ERR_SUCCESS; }
EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *t, int *v, int *api, const char **name, int *caps) {
    if (t) *t = M64PLUGIN_INPUT;
    if (v) *v = 0x020000;
    if (api) *api = 0x020100;
    if (name) *name = "n64ref scripted input";
    if (caps) *caps = 0;
    return M64ERR_SUCCESS;
}
EXPORT void CALL InitiateControllers(CONTROL_INFO info) {
    gControls = info.Controls;
    for (int i = 0; i < 4; i++) {
        gControls[i].Present = i < gNumPads;
        gControls[i].RawData = 0;
        gControls[i].Plugin = PLUGIN_NONE; /* no pak: the port reports none either (RUMBLE aside) */
    }
}
EXPORT void CALL GetKeys(int pad, BUTTONS *k) {
    uint32_t s = cur_step() - gInputShift;
    uint16_t btn = 0;
    int8_t x = 0, y = 0;
    for (int i = 0; i < gNEv; i++) {
        Ev *e = &gEv[i];
        if (e->pad != pad || s < e->first || s > e->last) continue;
        btn |= e->buttons;
        if (e->hasStick) x = e->x, y = e->y;
    }
    k->Value = (uint32_t)(uint16_t)((btn >> 8) | (btn << 8)) | (uint32_t)(uint8_t)x << 16 | (uint32_t)(uint8_t)y << 24;
}
EXPORT void CALL ControllerCommand(int c, unsigned char *cmd) { (void)c, (void)cmd; }
EXPORT void CALL ReadController(int c, unsigned char *cmd) { (void)c, (void)cmd; }
EXPORT void CALL RomOpen(void) {}
EXPORT void CALL RomClosed(void) {}
EXPORT void CALL SDL_KeyDown(int m, int k) { (void)m, (void)k; }
EXPORT void CALL SDL_KeyUp(int m, int k) { (void)m, (void)k; }
EXPORT void CALL RenderCallback(void) {}

/* ---------------------------------------------------------------- PNG */
static void png_chunk(FILE *f, const char *type, const uint8_t *d, uint32_t n) {
    uint8_t b[4] = { n >> 24, n >> 16, n >> 8, n };
    fwrite(b, 1, 4, f);
    fwrite(type, 1, 4, f);
    if (n) fwrite(d, 1, n, f);
    uint32_t c = crc32(crc32(0, (const uint8_t *)type, 4), d, n);
    uint8_t cb[4] = { c >> 24, c >> 16, c >> 8, c };
    fwrite(cb, 1, 4, f);
}
struct px {
    uint8_t r, g, b, a;
};
static void write_png(const char *path, const struct px *pix, uint32_t w, uint32_t h, uint32_t hout) {
    if (!hout) hout = h;
    size_t rl = 1 + (size_t)w * 3, raw = rl * hout;
    uint8_t *buf = malloc(raw), *p = buf;
    for (uint32_t y = 0; y < hout; y++) {
        const struct px *s = pix + (size_t)(y * h / hout) * w;
        *p++ = 0;
        for (uint32_t x = 0; x < w; x++) *p++ = s[x].r, *p++ = s[x].g, *p++ = s[x].b;
    }
    uLongf zn = compressBound(raw);
    uint8_t *z = malloc(zn);
    compress2(z, &zn, buf, raw, 6);
    FILE *f = fopen(path, "wb");
    if (!f) {
        perror(path);
        exit(1);
    }
    static const uint8_t sig[8] = { 0x89, 'P', 'N', 'G', 13, 10, 26, 10 };
    fwrite(sig, 1, 8, f);
    uint8_t ihdr[13] = { w >> 24, w >> 16, w >> 8, w, hout >> 24, hout >> 16, hout >> 8, hout, 8, 2, 0, 0, 0 };
    png_chunk(f, "IHDR", ihdr, 13);
    png_chunk(f, "IDAT", z, (uint32_t)zn);
    png_chunk(f, "IEND", NULL, 0);
    fclose(f);
    free(buf);
    free(z);
}

/* ---------------------------------------------------------------- per-VI hook */
typedef unsigned long (*last_frame_fn)(const struct px **, uint32_t *, uint32_t *, uint32_t *);
static last_frame_fn pLastFrame;
static uint32_t *gSnap[3];
static uint32_t gPrevStep = 0xFFFFFFFF;
/* --steps: hold the latest frame per wanted step, flushed when the step moves past S + lag */
static struct px *gHeld;
static uint32_t gHeldW, gHeldH, gHeldHout, gHeldStep = 0xFFFFFFFF, gHeldVi;
static int gStopping;

/* the scanned-out framebuffer, decoded from RDRAM (--fb) */
static struct px *gFbPix;
static unsigned long grab_fb(const struct px **pix, uint32_t *w, uint32_t *h, uint32_t *ho) {
    uint32_t status = gViRegs[0], origin = gViRegs[1] & 0xFFFFFF, width = gViRegs[2] & 0xFFF;
    uint32_t vs = gViRegs[10], ys = gViRegs[13] & 0xFFF;
    uint32_t lines = ((((vs & 0x3FF) - ((vs >> 16) & 0x3FF)) / 2) * ys + 1023) / 1024;
    int type = status & 3;
    /* osViBlack: libultra's __osViSwapContext writes V_START = 0 (no lines), and a blank mode has no
     * type: the TV shows black, whatever the framebuffer holds */
    int black = type < 2 || !lines || (gViRegs[9] & 0x3FF03FF) == 0;
    if (black) width = 320, lines = 240;
    if (lines < width * 3 / 4 && width * 3 / 4 <= 480) lines = width * 3 / 4; /* the whole 4:3 buffer (237 scanned) */
    *pix = NULL;
    if (!width || width > 640 || lines > 480) return 0;
    if (!gFbPix || *w != width || *h != lines) gFbPix = realloc(gFbPix, (size_t)width * lines * sizeof *gFbPix);
    *w = width, *h = *ho = lines;
    uint8_t *ram = (uint8_t *)gRam;
    for (uint32_t y = 0; y < lines; y++)
        for (uint32_t x = 0; x < width; x++) {
            struct px *d = &gFbPix[(size_t)y * width + x];
            if (black) {
                d->r = d->g = d->b = 0;
            } else if (type == 2) {
                uint32_t a = (origin + (y * width + x) * 2) & 0x7FFFFE;
                uint16_t c = ram[a ^ 2] | ram[(a ^ 2) + 1] << 8; /* host-order words: halfword index ^ 1 */
                /* as the VI fetches it (angrylion RGBA16_R/G/B): the 5 bits on top, the low 3 zero (plan 23.5) */
                d->r = ((c >> 11) & 31) << 3, d->g = ((c >> 6) & 31) << 3, d->b = ((c >> 1) & 31) << 3;
            } else {
                uint32_t c = gRam[((origin + (y * width + x) * 4) & 0x7FFFFC) >> 2];
                d->r = c >> 24, d->g = c >> 16, d->b = c >> 8;
            }
            d->a = 255;
        }
    *pix = gFbPix;
    return 1;
}
static unsigned long get_frame(const struct px **pix, uint32_t *w, uint32_t *h, uint32_t *ho) {
    if (gRawFb) {
        static uint32_t fw, fh;
        unsigned long r = grab_fb(pix, &fw, &fh, ho);
        *w = fw, *h = fh;
        return r;
    }
    return pLastFrame(pix, w, h, ho);
}

static void save_frame(const char *name) {
    const struct px *pix;
    uint32_t w, h, ho;
    char path[4096];
    get_frame(&pix, &w, &h, &ho);
    if (!pix) return;
    snprintf(path, sizeof path, "%s/%s", gOut, name);
    write_png(path, pix, w, h, ho);
}

static void scan(void) {
    uint32_t words = ((uint32_t)gMemSize << 20) / 4;
    int vi[3] = { 600, 700, 800 };
    for (int k = 0; k < 3; k++)
        if (gVi == (uint32_t)vi[k]) {
            gSnap[k] = malloc(words * 4);
            memcpy(gSnap[k], gRam, words * 4);
        }
    if (gVi != 800) return;
    printf("scan: words counting 15..60 per 100 VIs over VIs 600..800 (addr: v600 v700 v800 | +0xC)\n");
    for (uint32_t i = 0; i < words; i++) {
        uint32_t a = gSnap[0][i], b = gSnap[1][i], c = gSnap[2][i];
        if (a > 100000 || b <= a || c <= b) continue;
        if (b - a < 15 || b - a > 250 || c - b < 15 || c - b > 250) continue;
        uint32_t list = i >= 2 ? gSnap[2][i - 2] : 0;
        printf("  0x%08X: %u %u %u | list 0x%08X%s\n", 0x80000000u + i * 4, a, b, c, list,
               (list >> 24) == 0x80 ? "  <- SysLib.frameCount candidate" : "");
    }
    gStopping = 1;
    pCoreDoCommand(M64CMD_STOP, 0, NULL);
}

static void vi_hook(void) {
    char name[64];
    gVi++;
    if (gStopping) return;
    if (!gRam) gRam = pDebugMemGetPointer(M64P_DBG_PTR_RDRAM);
    if (!gViRegs) gViRegs = pDebugMemGetPointer(M64P_DBG_PTR_VI_REG);
    uint32_t s = cur_step();
    if (gScan) {
        scan();
        return;
    }
    if (s != gPrevStep) {
        if (gLogSteps && gStepLog) fprintf(gStepLog, "S %u %u\n", s, gVi);
        /* the held frame of a wanted step is final once the step passes S + lag */
        if (gHeld && gHeldStep != 0xFFFFFFFF && s > gHeldStep + gStepLag) {
            char path[4096];
            snprintf(path, sizeof path, "%s/s%06u.png", gOut, gHeldStep);
            write_png(path, gHeld, gHeldW, gHeldH, gHeldHout);
            if (gStepLog) fprintf(gStepLog, "C s%06u vi %u\n", gHeldStep, gHeldVi);
            gHeldStep = 0xFFFFFFFF;
        }
        gPrevStep = s;
    }
    if (gNSteps && s >= gStepLag && in_ranges(gSteps, gNSteps, s - gStepLag)) {
        const struct px *pix;
        uint32_t w, h, ho;
        get_frame(&pix, &w, &h, &ho);
        if (pix) {
            if (w != gHeldW || h != gHeldH) gHeld = realloc(gHeld, (size_t)w * h * sizeof *gHeld);
            memcpy(gHeld, pix, (size_t)w * h * sizeof *gHeld);
            gHeldW = w, gHeldH = h, gHeldHout = ho, gHeldStep = s - gStepLag, gHeldVi = gVi;
        }
    }
    if (gWatchFile && s >= gWatchFrom && s != 0xFFFFFFFF) { /* --watch */
        uint32_t hdr[2] = { gVi, s };
        fwrite(hdr, 4, 2, gWatchFile);
        for (int i = 0; i < gNWatch; i++)
            for (uint32_t a = gWatch[i].a; a < gWatch[i].a + gWatch[i].b; a += 4) {
                uint32_t w = ram32(a);
                uint8_t b[4] = { w >> 24, w >> 16, w >> 8, w };
                fwrite(b, 1, 4, gWatchFile);
            }
    }
    if (in_ranges(gDumps, gNDumps, gVi)) { /* big-endian RDRAM image, as the N64 sees it */
        char path[4096];
        uint32_t words = ((uint32_t)gMemSize << 20) / 4;
        snprintf(path, sizeof path, "%s/ram_v%06u.bin", gOut, gVi);
        FILE *f = fopen(path, "wb");
        for (uint32_t i = 0; f && i < words; i++) {
            uint32_t w = gRam[i];
            uint8_t b[4] = { w >> 24, w >> 16, w >> 8, w };
            fwrite(b, 1, 4, f);
        }
        if (f) fclose(f);
    }
    if (in_ranges(gVis, gNVis, gVi) || (gEveryVi && gVi % gEveryVi == 0)) {
        snprintf(name, sizeof name, "v%06u.png", gVi);
        save_frame(name);
        if (gStepLog) fprintf(gStepLog, "C v%06u step %u\n", gVi, s);
    }
    if (gNSteps && gStepAddr && s > ranges_max(gSteps, gNSteps) + gStepLag + 1 && gHeldStep == 0xFFFFFFFF) {
        gStopping = 1;
        pCoreDoCommand(M64CMD_STOP, 0, NULL);
        return;
    }
    if (gVi >= gMaxVi) {
        gStopping = 1;
        pCoreDoCommand(M64CMD_STOP, 0, NULL);
    }
}

static void dbg_cb(void *ctx, int level, const char *msg) {
    (void)ctx;
    if (level <= M64MSG_WARNING || getenv("N64REF_VERBOSE")) fprintf(stderr, "core[%d]: %s\n", level, msg);
}

/* ---------------------------------------------------------------- main */
static void *load_plugin(const char *dir, const char *file, m64p_plugin_type type) {
    char path[4096];
    snprintf(path, sizeof path, "%s/%s", dir, file);
    void *h = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!h) {
        fprintf(stderr, "n64ref: %s\n", dlerror());
        exit(2);
    }
    ptr_PluginStartup st = (ptr_PluginStartup)must_sym(h, "PluginStartup");
    st(gCore, NULL, dbg_cb);
    if (pCoreAttachPlugin(type, h) != M64ERR_SUCCESS) {
        fprintf(stderr, "n64ref: attach %s failed\n", file);
        exit(2);
    }
    return h;
}

static void set_str(m64p_handle s, const char *k, const char *v) { pConfigSetParameter(s, k, M64TYPE_STRING, v); }
static void set_int(m64p_handle s, const char *k, int v) { pConfigSetParameter(s, k, M64TYPE_INT, &v); }
static void set_bool(m64p_handle s, const char *k, int v) { pConfigSetParameter(s, k, M64TYPE_BOOL, &v); }

int main(int argc, char **argv) {
    const char *rom = NULL, *input = NULL, *sram = NULL;
    char bindir[4096], path[4096];
#ifdef N64REF_LIBPATH
    /* the SDL2 / pulse runtime from the unpacked debs: libpulse's own RUNPATH hides the
     * executable's RPATH, so re-exec once with LD_LIBRARY_PATH */
    if (!getenv("N64REF_REEXEC")) {
        const char *old = getenv("LD_LIBRARY_PATH");
        char *lp = malloc(strlen(N64REF_LIBPATH) + (old ? strlen(old) : 0) + 2);
        sprintf(lp, "%s%s%s", N64REF_LIBPATH, old ? ":" : "", old ? old : "");
        setenv("LD_LIBRARY_PATH", lp, 1);
        setenv("N64REF_REEXEC", "1", 1);
        execv("/proc/self/exe", argv);
        perror("execv");
        return 2;
    }
#endif
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i], *v = i + 1 < argc ? argv[i + 1] : NULL;
#define OPT(n) (!strcmp(a, n) && v && (i++, 1))
        if (OPT("--rom")) rom = v;
        else if (OPT("--out")) gOut = v;
        else if (OPT("--input")) input = v;
        else if (OPT("--vis")) gNVis = parse_ranges(v, gVis, 256);
        else if (OPT("--dump-ram")) gNDumps = parse_ranges(v, gDumps, 64);
        else if (OPT("--steps")) gNSteps = parse_ranges(v, gSteps, 256);
        else if (OPT("--every-vi")) gEveryVi = strtoul(v, NULL, 0);
        else if (OPT("--max-vi")) gMaxVi = strtoul(v, NULL, 0);
        else if (OPT("--step-addr")) gStepAddr = strtoul(v, NULL, 0);
        else if (OPT("--step-lag")) gStepLag = strtoul(v, NULL, 0);
        else if (OPT("--memsize")) gMemSize = atoi(v);
        else if (OPT("--vi-mode")) gViMode = atoi(v);
        else if (OPT("--input-shift")) gInputShift = atoi(v);
        else if (!strcmp(a, "--fb")) gRawFb = 1;
        else if (OPT("--sram")) sram = v;
        else if (OPT("--watch")) {
            /* A:N pairs: reuse Range as (address, length) */
            const char *q = v;
            while (*q && gNWatch < 16) {
                char *e;
                gWatch[gNWatch].a = strtoul(q, &e, 0);
                gWatch[gNWatch].b = *e == ':' ? strtoul(e + 1, &e, 0) : 4;
                gNWatch++;
                q = *e == ',' ? e + 1 : e;
            }
        } else if (OPT("--watch-from")) gWatchFrom = strtoul(v, NULL, 0);
        else if (!strcmp(a, "--scan")) gScan = 1;
        else if (!strcmp(a, "--log-steps")) gLogSteps = 1;
        else if (!strcmp(a, "--interp")) gInterp = 1;
        else {
            fprintf(stderr, "n64ref: bad argument %s (see the header of n64ref.c)\n", a);
            return 2;
        }
    }
    if (!rom) {
        fprintf(stderr, "n64ref: --rom is required\n");
        return 2;
    }
    if (gNSteps && !gStepAddr) {
        fprintf(stderr, "n64ref: --steps needs --step-addr (find it with --scan)\n");
        return 2;
    }
    if (!gMaxVi) {
        gMaxVi = 3000;
        if (gNVis || gNDumps) gMaxVi = (ranges_max(gVis, gNVis) > ranges_max(gDumps, gNDumps) ? ranges_max(gVis, gNVis) : ranges_max(gDumps, gNDumps)) + 2;
        if (gScan) gMaxVi = 801;
        if (gNSteps) gMaxVi = 400000; /* stops after the last step's frame, below */
    }
    if (input) load_script(input);
    mkdir(gOut, 0777);
    snprintf(path, sizeof path, "%s/steps.txt", gOut);
    gStepLog = fopen(path, "w");
    if (gNWatch) {
        snprintf(path, sizeof path, "%s/watch.bin", gOut);
        gWatchFile = fopen(path, "wb");
    }

    ssize_t n = readlink("/proc/self/exe", bindir, sizeof bindir - 1);
    bindir[n > 0 ? n : 0] = 0;
    *strrchr(bindir, '/') = 0;
    snprintf(path, sizeof path, "%s/libmupen64plus.so.2", bindir);
    gCore = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (!gCore) {
        fprintf(stderr, "n64ref: %s\n", dlerror());
        return 2;
    }
    pCoreStartup = must_sym(gCore, "CoreStartup");
    pCoreShutdown = must_sym(gCore, "CoreShutdown");
    pCoreAttachPlugin = must_sym(gCore, "CoreAttachPlugin");
    pCoreDoCommand = must_sym(gCore, "CoreDoCommand");
    pConfigOpenSection = must_sym(gCore, "ConfigOpenSection");
    pConfigSetParameter = must_sym(gCore, "ConfigSetParameter");
    pConfigSaveFile = must_sym(gCore, "ConfigSaveFile");
    pDebugMemGetPointer = must_sym(gCore, "DebugMemGetPointer");
    (void)pPluginStartup;

    snprintf(path, sizeof path, "%s/data", bindir);
    if (pCoreStartup(0x020001, gOut, path, NULL, dbg_cb, NULL, NULL) != M64ERR_SUCCESS) {
        fprintf(stderr, "n64ref: CoreStartup failed\n");
        return 2;
    }
    m64p_handle sec;
    pConfigOpenSection("Core", &sec);
    set_int(sec, "R4300Emulator", gInterp ? 0 : 1); /* cached interpreter (this build's dynarec dies at boot) */
    set_bool(sec, "DisableExtraMem", gMemSize == 4);
    set_bool(sec, "OnScreenDisplay", 0);
    set_str(sec, "SaveSRAMPath", gOut);
    set_str(sec, "SaveStatePath", gOut);
    set_str(sec, "ScreenshotPath", gOut);
    set_bool(sec, "AutoStateSlotIncrement", 0);
    pConfigOpenSection("Video-General", &sec);
    set_int(sec, "ScreenWidth", 640);
    set_int(sec, "ScreenHeight", 480);
    pConfigOpenSection("Video-AngrylionPlus", &sec);
    set_bool(sec, "Parallel", 1);
    set_int(sec, "NumWorkers", 8);
    set_int(sec, "ViMode", gViMode);
    set_bool(sec, "ViHideOverscan", 1);
    pConfigOpenSection("rsp-cxd4", &sec);
    set_bool(sec, "DisplayListToGraphicsPlugin", 0);
    set_bool(sec, "AudioListToAudioPlugin", 0);

    FILE *f = fopen(rom, "rb");
    if (!f) {
        perror(rom);
        return 2;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *buf = malloc(sz);
    if (fread(buf, 1, sz, f) != (size_t)sz) return 2;
    fclose(f);
    if (pCoreDoCommand(M64CMD_ROM_OPEN, (int)sz, buf) != M64ERR_SUCCESS) {
        fprintf(stderr, "n64ref: ROM_OPEN failed\n");
        return 2;
    }
    free(buf);
    if (sram) {
        /* mupen64plus names the SRAM <goodname>.sra in SaveSRAMPath; read its name back */
        m64p_rom_settings rs;
        pCoreDoCommand(M64CMD_ROM_GET_SETTINGS, sizeof rs, &rs);
        char dst[4096];
        snprintf(dst, sizeof dst, "%s/%s.sra", gOut, rs.goodname);
        FILE *in = fopen(sram, "rb"), *out = fopen(dst, "wb");
        if (!in || !out) {
            fprintf(stderr, "n64ref: cannot copy %s -> %s\n", sram, dst);
            return 2;
        }
        uint8_t b[32768];
        size_t k = fread(b, 1, sizeof b, in);
        fwrite(b, 1, k, out);
        fclose(in);
        fclose(out);
        fprintf(stderr, "n64ref: SRAM %s -> %s\n", sram, dst);
    }
    void *vid = load_plugin(bindir, "mupen64plus-video-angrylion-plus.so", M64PLUGIN_GFX);
    void *self = dlopen(NULL, RTLD_NOW); /* the core wants GFX, AUDIO, INPUT, RSP in order */
    if (pCoreAttachPlugin(M64PLUGIN_INPUT, self) != M64ERR_SUCCESS) {
        fprintf(stderr, "n64ref: attach input failed\n");
        return 2;
    }
    load_plugin(bindir, "mupen64plus-rsp-cxd4-sse2.so", M64PLUGIN_RSP);
    pLastFrame = (last_frame_fn)must_sym(vid, "n64ref_last_frame");
    void (*set_hook)(void (*)(void)) = (void (*)(void (*)(void)))must_sym(vid, "n64ref_set_vi_hook");
    set_hook(vi_hook);
    int zero = 0;
    pCoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_SPEED_LIMITER, &zero);
    pCoreDoCommand(M64CMD_EXECUTE, 0, NULL);
    /* flush a held step frame */
    if (gHeld && gHeldStep != 0xFFFFFFFF) {
        snprintf(path, sizeof path, "%s/s%06u.png", gOut, gHeldStep);
        write_png(path, gHeld, gHeldW, gHeldH, gHeldHout);
        if (gStepLog) fprintf(gStepLog, "C s%06u vi %u (at stop)\n", gHeldStep, gHeldVi);
    }
    fprintf(stderr, "n64ref: stopped at VI %u, step %u\n", gVi, cur_step());
    if (gStepLog) fclose(gStepLog);
    if (gWatchFile) fclose(gWatchFile);
    pCoreDoCommand(M64CMD_ROM_CLOSE, 0, NULL);
    pCoreShutdown();
    return 0;
}
