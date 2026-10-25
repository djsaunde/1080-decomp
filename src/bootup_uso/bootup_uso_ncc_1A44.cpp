/* IDO 7.1 NCC (C++) donor for func_00001A44 (0x1CC / 115 words): gSPMatrix +
 * gSPDisplayList draw (agent-c run 114 decode) as C++. 115/115 words (agent-g run 172).
 *
 * Under NCC: the three scale products are written camera * G (edg keeps the
 * source operand order: the camera word loads first, as in the target); the
 * gfx-context pointer is declared first (its home is the target's sp+52 spill);
 * the 0x06 display-list packet reads its w1 (a0->0x38) BEFORE the slot
 * increment (the target loads it between the count load and the count store),
 * here a block-local `_b` evaluated ahead of the packet pointer -- the cfe body
 * needed an `if (g) {}` for the same shape. The last colours (matrix count++ in
 * a1 / a2, the 0x06 packet in a0 / a3 / t0) come from which increments feed the
 * shared `_n`: the matrix count++ as `_n = ms->count++` (post-increment into _n)
 * and the 0x06 packet as an explicit `_n = d->count; d->count = _n + 1` slot;
 * the other three packets use the `_n = d->count++` slot macro.
 * Spliced over the placeholder in both bootup_uso.c.o trees via REPLACE_FUNC_BODY. */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
extern "C" {
extern int import_8005C108[];
extern int import_8005C108_g1a44[];   /* gate read alias (same import) */
extern char *D_bu_205CC;              /* bootup data sym161: camera root */
extern void func_1a44_emit(void *, float, float, float, float, float, float, float);
extern int func_1a44_emit2(void *);
}
typedef struct { u32 w0, w1; } Gw1A44;
typedef struct { Gw1A44 *buf; s32 count; } Dl1A44;
typedef struct { char m[64]; } Mtx1A44;
typedef struct { Mtx1A44 *buf; s32 count; } Ms1A44;
#define NEXT1A44(d) (_n = (d)->count++, &(d)->buf[_n])
#define NEXTX1A44(d) (_n = (d)->count, (d)->count = _n + 1, &(d)->buf[_n])
#define W1A44(pkt, a, b) { Gw1A44 *_g = (Gw1A44 *)(pkt); _g->w0 = (a); _g->w1 = (b); }
#define W1A44B(pkt, a, b) { u32 _b = (b); Gw1A44 *_g = (Gw1A44 *)(pkt); _g->w0 = (a); _g->w1 = _b; }
extern "C" void func_00001A44(char *a0) {
    char *gc;
    char *g;
    s32 _n;

    if (import_8005C108_g1a44[0x3C / 4] & 2) {
        return;
    }
    g = (char *)import_8005C108;
    gc = *(char **)(*(char **)(g + 0x254) + 0x158);
    func_1a44_emit(&(*(Ms1A44 **)(gc + 0x3C))->buf[(*(Ms1A44 **)(gc + 0x3C))->count],
                   (f32)*(s32 *)(a0 + 0x2C), (f32)*(s32 *)(a0 + 0x30), (f32)*(s32 *)(a0 + 0x34),
                   1.0f,
                   *(f32 *)(*(char **)(D_bu_205CC + 0x70) + 0xA0) * *(f32 *)(g + 0x128),
                   *(f32 *)(*(char **)(D_bu_205CC + 0x70) + 0xA4) * *(f32 *)(g + 0x12C),
                   *(f32 *)(*(char **)(D_bu_205CC + 0x70) + 0xA8) * *(f32 *)(g + 0x130));
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0x01020040,
          func_1a44_emit2(&(*(Ms1A44 **)(gc + 0x3C))->buf[(*(Ms1A44 **)(gc + 0x3C))->count]));
    _n = (*(Ms1A44 **)(gc + 0x3C))->count++;
    W1A44B(NEXTX1A44(*(Dl1A44 **)(gc + 0xC)), 0x06000000, *(u32 *)(a0 + 0x38));
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0xE7000000, 0);
    W1A44(NEXT1A44(*(Dl1A44 **)(gc + 0xC)), 0xBA000E02, 0);
}
