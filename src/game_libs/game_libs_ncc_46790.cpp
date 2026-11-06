/* IDO 7.1 NCC (C++) donor for gl_func_00046790: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 75.10; q declared before dst puts dst at sp+0 as in the target, 96.54; the pixel store through an inline put(unsigned short *&, v) gives the target a2 reload, 96.61).
 * agent-c run 160: the glyph column as `(u8)(c & 15)`: the folded byte cast burns one ugen ring slot (IDO_CODEGEN folded-byte-mask-burns-ring-slot-42f4c),
 * which realigns every later t-register in the function, 96.64 -> 99.25. Residual: the `(c / 16) * 1024` shift and the
 * loop's high-nibble reads one ring slot apart (t8 / t9 swapped).
 * agent-g run 305: EXACT. ugen's temporaries come from a FIFO free queue (IDO_CODEGEN kit CCCXCI); replaying it, the
 * target needs ONE extra temporary between the div and the row shift and no (u8) burn. The row offset written as two
 * shifts `(c / 16) << 5 << 5` (c / 16 is 0..7 here, so the shifts are well-defined and equal * 1024) gives ugen
 * `sll $24,$14,5; sll $25,$24,5`, which as1 merges into the single `sll t9,t6,10` of the target: 99.25 -> 100.
 * Spliced over a placeholder on both build trees via REPLACE_FUNC_BODY; D_46790_g / D_46790_font pinned 0 (blank USO words). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern void gl_proto_41148(int, int, int, int, int, double);
extern char *o;
extern char *c;
extern short idx;
extern float q[4];
extern char D_46790_g[];
extern unsigned char D_46790_font[];
}
static inline void put(unsigned short *&d, unsigned short v) { *d++ = v; }
extern "C" void gl_func_00046790(char *o, unsigned char ch, int x, int y) {
    unsigned short **q;
    unsigned short *dst;
    int c;
    int idx;
    int i, j;
    unsigned short px;

    if (1) { q = &dst; }
    dst = *(unsigned short **)(*(char **)(D_46790_g + 576) + *(int *)(o + 516) * 4 + 244);
    dst += y * 320 + x;
    c = ch;
    if (c < 128) {
        idx = ((c / 16) << 5 << 5) + (c & 15) * 8;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                px = 0;
                if (idx & 1) {
                    if (D_46790_font[idx >> 1] & 0xF) px = 0xFFFE;
                } else {
                    if (D_46790_font[idx >> 1] & 0xF0) px = 0xFFFE;
                }
                put(dst, px);
                idx++;
            }
            dst += 312;
            idx += 120;
        }
    }
}
