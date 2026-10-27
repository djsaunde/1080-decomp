/* IDO 7.1 NCC (C++) NM-only comparison donor for func_00008BD4 (agent-g run 157).
 * The cfe body (91.85) was capped by amt being spilled to its arg home around each
 * call; under NCC amt lives in $f20 as in the target (sdc1/ldc1 f20, frame save).
 * The two m2c copy chains (imp -> impB -> impA, nn -> impB -> nnA) are one inline
 * helper returning a named non-POD V by value (r -> shared return temp at sp+0x84
 * -> destination), products held in locals. Compound `+=` / `|=` on the ctx
 * fields are written `x = x + 1` (edg takes the lvalue address for compound
 * assignment: an extra addiu per site). 91.85 -> 96.67, 208 words (= cfe; target 209).
 * The skip set as three chained `if (!skip) skip = code == K;` statements (one bnez per
 * test like the target): 96.67 -> 96.83, 209 words = target.
 * RESIDUAL: the code copy into a1 for the skip chain, negamt colour (target ft4),
 * homes (inline locals take 6 frame words; frame 0xC8 vs 0xB8).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef float f32;
extern "C" {
extern char D_00000000;
extern int func_00000000(...);
extern void func_8bd4_a(void *, void *, void *, float *, float);
extern void func_8bd4_b(char *, float, float *, float);
extern void func_8bd4_heavy(void *, void *, void *);
}
struct V { f32 x, y, z; V() {} };
static inline V scaledBy(const float *d, f32 s) {
    V r; f32 hx = d[0] * s; f32 hy = d[1] * s; f32 hz = d[2] * s;
    r.x = hx; r.y = hy; r.z = hz;
    return r;
}
extern "C" void func_00008BD4(char *b, float amt, float *dir, int a3) {
    unsigned short fl = *(unsigned short *)(b + 0x174);
    int code;
    int skip;
    V impA, nnA;
    float t;
    float q;
    if (fl & 0x1F0) {
        if (amt < 0.0f) {
            code = (int)func_00000000(*(int *)(*(char **)&D_00000000 + 0x84), *(int *)(b + 0x14C));
            skip = code == 0x61;
            if (!skip) skip = code == 0x62;
            if (!skip) skip = code == 0x63;
            if (!skip) {
                *(int *)((*(char **)(b + 0x170)) + 0x938) = *(int *)((*(char **)(b + 0x170)) + 0x938) + 1;
                *(short *)((*(char **)(b + 0x170)) + 0x9A4) = (short)code;
                if (amt < -30.0f) amt = -30.0f;
                {
                    unsigned short *fp = (unsigned short *)(*(char **)(b + 0x170) + 0xA10);
                    *fp |= *(unsigned short *)(b + 0x174);
                }
                impA = scaledBy(dir, (float)((double)-amt * 0.5));
                {
                    float *ap = (float *)(*(char **)(b + 0x170) + 0xB4);
                    ap[0] += impA.x;
                    ap[1] += impA.y;
                    ap[2] += impA.z; if (1) {} /* run 182 (lever-sweep bbsplit) */
                }
                {
                    float *tp = (float *)(*(char **)(b + 0x170) + 0x93C);
                    tp[0] += dir[0];
                    tp[1] += dir[1];
                    tp[2] += dir[2];
                }
                {
                    float *mp = (float *)(*(char **)(b + 0x170) + 0x968);
                    *mp += amt;
                }
                nnA = scaledBy(dir, -amt); /* run 182 (lever-sweep de-naming): no named negamt */
                func_8bd4_heavy(*(char **)(b + 0x170), b + 0x120, &nnA);
            }
        }
        return;
    }
    if (fl & 0xF) {
        if (amt < 50.0f) {
            *(int *)(*(char **)(b + 0x170) + 0x938) = *(int *)(*(char **)(b + 0x170) + 0x938) + 1;
            {
                unsigned short *gp = (unsigned short *)(*(char **)(b + 0x170) + 0xA10);
                *gp |= *(unsigned short *)(b + 0x174);
            }
            {
                float *np = (float *)(*(char **)(b + 0x170) + 0x968);
                *np += amt;
            }
            {
                float *wp = (float *)(*(char **)(b + 0x170) + 0x93C);
                wp[0] += dir[0];
                wp[1] += dir[1];
                wp[2] += dir[2];
            }
            {
                t = *(float *)(*(char **)(b + 0x170) + 0x970);
                if (t < 0.0f) {
                    q = -t;
                } else {
                    q = t;
                }
                func_8bd4_a(*(char **)(b + 0x170), b + 0x120, b + 0x114, dir, q * q);
            }
            func_8bd4_b(b, amt, dir, 0.1f);
        }
    }
}
