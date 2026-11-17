/* port/gfx/replay: render a DLMP dump (../dlump.h) offscreen and write a PNG.
 * PC port plan section 19 (M2), option (b): the 32-bit game dumps, this 64-bit
 * process renders, so screenshot tests need neither a window nor 32-bit GL.
 *
 *   dlreplay in.dlmp out.png [--scale N]
 *   LIBGL_ALWAYS_SOFTWARE=1 dlreplay ...   llvmpipe: deterministic pixels for tests
 *
 * This is the BRING-UP interpreter, not the M2 renderer: it proves the dump ->
 * 64-bit GL -> PNG path with a minimal F3DEX 1.x subset (G_MTX load / mul /
 * push, G_POPMTX, G_VTX, G_TRI1, G_MOVEMEM viewport, G_SETGEOMETRYMODE ZBUFFER /
 * culling, FILL-mode G_FILLRECT colour and Z clears, G_SETSCISSOR) with Gouraud
 * vertex colours. Textures, the combiner, lighting, fog and the blender are
 * Fast3D's job: the plan is libultraship's interpreter fed from the same memory
 * image (plan 19.3). GL: an EGL pbuffer on Mesa's surfaceless platform (falling
 * back to the default display), legacy compatibility profile, no window. */

#include <GL/gl.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dlump.h"
#include "egl_png.h"

/* ---------------- the dump's memory image ---------------- */

typedef struct Chunk {
    uint32_t kind, addr, size, aux;
    const uint8_t *data;
} Chunk;

static Chunk *sChunks;
static int sNChunks;
static const DlumpHeader *sHdr;

static const uint8_t *mem(uint32_t addr, uint32_t size) {
    int i;
    for (i = 0; i < sNChunks; i++) {
        const Chunk *c = &sChunks[i];
        if (c->size != 0 && c->kind != DLUMP_ERROR && c->addr <= addr && (uint64_t)addr + size <= (uint64_t)c->addr + c->size) {
            return c->data + (addr - c->addr);
        }
    }
    return NULL;
}

static int load(const char *path) {
    FILE *f = fopen(path, "rb");
    long n;
    uint8_t *d;
    size_t off;
    uint32_t i;
    if (f == NULL) {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    d = malloc((size_t)n);
    if (d == NULL || fread(d, 1, (size_t)n, f) != (size_t)n) {
        fclose(f);
        return 0;
    }
    fclose(f);
    sHdr = (const DlumpHeader *)d;
    if (n < 64 || memcmp(sHdr->magic, DLUMP_MAGIC, 8) != 0 || sHdr->version != DLUMP_VERSION ||
        sHdr->byteOrderMark != DLUMP_BYTE_ORDER_MARK) {
        fprintf(stderr, "dlreplay: %s: not a v1 DLMP in host byte order\n", path);
        return 0;
    }
    sChunks = calloc(sHdr->chunkCount + 1, sizeof(Chunk));
    off = sHdr->headerSize;
    for (i = 0; i < sHdr->chunkCount; i++) {
        const DlumpChunk *c = (const DlumpChunk *)(d + off);
        if (off + sizeof(*c) > (size_t)n || off + sizeof(*c) + c->size > (size_t)n) {
            fprintf(stderr, "dlreplay: truncated chunk %u\n", i);
            return 0;
        }
        sChunks[i].kind = c->kind;
        sChunks[i].addr = c->addr;
        sChunks[i].size = c->size;
        sChunks[i].aux = c->aux;
        sChunks[i].data = (const uint8_t *)(c + 1);
        off += sizeof(*c) + ((c->size + 3) & ~3u);
    }
    sNChunks = (int)sHdr->chunkCount;
    return 1;
}

/* ---------------- a minimal F3DEX 1.x interpreter ---------------- */

typedef struct { float m[4][4]; } M4;
typedef struct { float clip[4]; float col[4]; } RVtx;

static int sW = 320, sH = 240, sScale = 1;
static M4 sProj, sMv[10];
static int sMvTop;
static RVtx sVtx[64];
static uint32_t sGeom, sOtherH, sFill, sCimg, sZimg, sSeg[16];
static int sTris, sFills;

static void m_mul(M4 *r, const M4 *a, const M4 *b) { /* r = a * b (row vectors) */
    M4 t;
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            float s = 0;
            for (k = 0; k < 4; k++) {
                s += a->m[i][k] * b->m[k][j];
            }
            t.m[i][j] = s;
        }
    }
    *r = t;
}

/* N64 Mtx: words 0..7 integer halves, 8..15 fraction halves, as host-order u32s. */
static void m_from_n64(M4 *r, const uint8_t *p) {
    uint32_t w[16];
    int i;
    memcpy(w, p, 64);
    for (i = 0; i < 16; i++) {
        uint32_t iw = w[i / 2], fw = w[8 + i / 2];
        int32_t hi = (int16_t)((i & 1) ? (iw & 0xFFFF) : (iw >> 16));
        uint32_t lo = (i & 1) ? (fw & 0xFFFF) : (fw >> 16);
        r->m[i / 4][i % 4] = (float)((hi * 65536) + (int32_t)lo) / 65536.0f;
    }
}

static uint32_t seg(uint32_t a) {
    uint32_t b = sSeg[(a >> 24) & 15];
    return b ? b + (a & 0xFFFFFF) : a;
}

static void fill_rect(uint32_t w0, uint32_t w1) {
    int lrx = (w0 >> 14) & 0x3FF, lry = (w0 >> 2) & 0x3FF, ulx = (w1 >> 14) & 0x3FF, uly = (w1 >> 2) & 0x3FF;
    uint32_t c = sFill >> 16;
    if (((sOtherH >> 20) & 3) != 3) {
        return; /* only FILL-mode rects (clears) in bring-up */
    }
    glEnable(GL_SCISSOR_TEST);
    glScissor(ulx * sScale, (sH - lry - 1) * sScale, (lrx - ulx + 1) * sScale, (lry - uly + 1) * sScale);
    if (sCimg == sZimg) { /* 1080's Z clear: the colour image pointed at the Z buffer */
        glDepthMask(GL_TRUE);
        glClearDepth(1.0);
        glClear(GL_DEPTH_BUFFER_BIT);
    } else {
        glClearColor(((c >> 11) & 31) / 31.0f, ((c >> 6) & 31) / 31.0f, ((c >> 1) & 31) / 31.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glDisable(GL_SCISSOR_TEST);
    sFills++;
}

static void tri(int a, int b, int c) {
    const int idx[3] = { a, b, c };
    int i;
    if (sGeom & 0x1) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    /* G_CULL_FRONT 0x1000 / G_CULL_BACK 0x2000; N64 front = counter-clockwise on screen */
    if (sGeom & 0x3000) {
        glEnable(GL_CULL_FACE);
        glCullFace((sGeom & 0x3000) == 0x3000 ? GL_FRONT_AND_BACK : (sGeom & 0x2000) ? GL_BACK : GL_FRONT);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glBegin(GL_TRIANGLES);
    for (i = 0; i < 3; i++) {
        const RVtx *v = &sVtx[idx[i] & 63];
        glColor4fv(v->col);
        glVertex4fv(v->clip);
    }
    glEnd();
    sTris++;
}

static void run(uint32_t root) {
    uint32_t stack[DLUMP_DL_STACK], pc = root;
    int sp = 0, n = 0;
    for (;;) {
        const uint8_t *p = mem(pc, 8);
        uint32_t w0, w1;
        if (p == NULL || n++ > (1 << 22)) {
            fprintf(stderr, "dlreplay: list 0x%08x not in the dump\n", pc);
            return;
        }
        memcpy(&w0, p, 4);
        memcpy(&w1, p + 4, 4);
        pc += 8;
        switch (w0 >> 24) {
            case 0x06:
                if (((w0 >> 16) & 0xFF) == 0) {
                    if (sp >= DLUMP_DL_STACK) {
                        return;
                    }
                    stack[sp++] = pc;
                }
                pc = seg(w1);
                break;
            case 0xB8:
                if (sp == 0) {
                    return;
                }
                pc = stack[--sp];
                break;
            case 0xBC:
                if ((w0 & 0xFF) == 6) {
                    sSeg[((w0 >> 8) & 0xFFFF) / 4 & 15] = w1;
                }
                break;
            case 0x01: { /* G_MTX: params 1 PROJECTION, 2 LOAD, 4 PUSH */
                const uint8_t *m = mem(seg(w1), 64);
                uint32_t prm = (w0 >> 16) & 0xFF;
                M4 t;
                if (m == NULL) {
                    break;
                }
                m_from_n64(&t, m);
                if (prm & 1) {
                    if (prm & 2) {
                        sProj = t;
                    } else {
                        m_mul(&sProj, &t, &sProj);
                    }
                } else {
                    if ((prm & 4) && sMvTop < 9) {
                        sMv[sMvTop + 1] = sMv[sMvTop];
                        sMvTop++;
                    }
                    if (prm & 2) {
                        sMv[sMvTop] = t;
                    } else {
                        m_mul(&sMv[sMvTop], &t, &sMv[sMvTop]);
                    }
                }
                break;
            }
            case 0xBD:
                if (sMvTop > 0) {
                    sMvTop--;
                }
                break;
            case 0x03: /* G_MOVEMEM: viewport only */
                if (((w0 >> 16) & 0xFF) == 0x80) {
                    const int16_t *vp = (const int16_t *)mem(seg(w1), 16);
                    if (vp != NULL) {
                        float sx = vp[0] / 4.0f, sy = vp[1] / 4.0f, tx = vp[4] / 4.0f, ty = vp[5] / 4.0f;
                        glViewport((int)((tx - sx) * sScale), (int)((sH - (ty + sy)) * sScale),
                                   (int)(2 * sx * sScale), (int)(2 * sy * sScale));
                    }
                }
                break;
            case 0x04: { /* F3DEX 1: 04 | v0*2 << 16 | n << 10 | n*16-1 */
                uint32_t v0 = ((w0 >> 16) & 0xFF) / 2, cnt = (w0 >> 10) & 0x3F, i;
                const int16_t *s = (const int16_t *)mem(seg(w1), cnt * 16);
                M4 mvp;
                if (s == NULL) {
                    break;
                }
                m_mul(&mvp, &sMv[sMvTop], &sProj);
                for (i = 0; i < cnt && v0 + i < 64; i++, s += 8) {
                    const uint8_t *cn = (const uint8_t *)(s + 6);
                    float o[4] = { s[0], s[1], s[2], 1.0f };
                    RVtx *v = &sVtx[v0 + i];
                    int j, k;
                    for (j = 0; j < 4; j++) {
                        v->clip[j] = 0;
                        for (k = 0; k < 4; k++) {
                            v->clip[j] += o[k] * mvp.m[k][j];
                        }
                    }
                    for (j = 0; j < 4; j++) {
                        v->col[j] = cn[j] / 255.0f;
                    }
                }
                break;
            }
            case 0xBF: /* F3DEX 1 G_TRI1: w1 = flag << 24 | v0*2 << 16 | v1*2 << 8 | v2*2 */
                tri(((w1 >> 16) & 0xFF) / 2, ((w1 >> 8) & 0xFF) / 2, (w1 & 0xFF) / 2);
                break;
            case 0xB6:
                sGeom &= ~w1;
                break;
            case 0xB7:
                sGeom |= w1;
                break;
            case 0xBA: { /* SETOTHERMODE_H: shift = (w0 >> 8) & 0xFF, len = w0 & 0xFF */
                uint32_t sh = (w0 >> 8) & 0xFF, len = w0 & 0xFF;
                uint32_t mask = (len >= 32 ? 0xFFFFFFFFu : ((1u << len) - 1)) << sh;
                sOtherH = (sOtherH & ~mask) | (w1 & mask);
                break;
            }
            case 0xF7:
                sFill = w1;
                break;
            case 0xF6:
                fill_rect(w0, w1);
                break;
            case 0xFF:
                sCimg = seg(w1);
                break;
            case 0xFE:
                sZimg = seg(w1);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    uint8_t *px;
    int i;
    if (argc < 3) {
        fprintf(stderr, "usage: dlreplay in.dlmp out.png [--scale N]\n");
        return 2;
    }
    for (i = 3; i + 1 < argc; i++) {
        if (strcmp(argv[i], "--scale") == 0) {
            sScale = atoi(argv[++i]);
        }
    }
    if (sScale < 1) {
        sScale = 1;
    }
    if (!load(argv[1])) {
        return 1;
    }
    if (!gl_init(sW * sScale, sH * sScale)) {
        fprintf(stderr, "dlreplay: no EGL / GL context\n");
        return 1;
    }
    for (i = 0; i < 4; i++) {
        sProj.m[i][i] = 1;
        sMv[0].m[i][i] = 1;
    }
    glViewport(0, 0, sW * sScale, sH * sScale);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    run(sHdr->rootAddr);
    glFinish();
    px = malloc((size_t)sW * sScale * sH * sScale * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, sW * sScale, sH * sScale, GL_RGBA, GL_UNSIGNED_BYTE, px);
    if (!write_png(argv[2], px, sW * sScale, sH * sScale)) {
        fprintf(stderr, "dlreplay: cannot write %s\n", argv[2]);
        return 1;
    }
    printf("dlreplay: %s -> %s: %d triangles, %d fill rects\n", argv[1], argv[2], sTris, sFills);
    return 0;
}
