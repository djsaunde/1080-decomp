/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00046790: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 75.10; q declared before dst puts dst at sp+0 as in the target, 96.54; the pixel store through an inline put(unsigned short *&, v) gives the target a2 reload, 96.61).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
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
        idx = (c / 16) * 1024 + (c & 15) * 8;
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
