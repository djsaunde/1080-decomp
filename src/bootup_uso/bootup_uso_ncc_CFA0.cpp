/* IDO 7.1 NCC (C++) donor for func_0000CFA0 (0x4A0 / 296 words) -- rider
 * ground-contact / lighting refresh: asks the context (func_cfa0) for the
 * surface under the rider, flags a change, re-tints from the surface colour
 * and fires the sub-object hooks.
 *
 * C++ (agent-c run 116). The cfe NM capped at 99.03 (54 words off: v-reg
 * swaps around the dispatches and the context temp). A direct NCC port of
 * the same logic was already closer (43); then:
 * - the sub-object hooks are real virtual calls (vptr at +0x28, slot 2:
 *   lh +0x18 / lw +0x1C), `o->f()` instead of a hand-written dispatch;
 * - the two 12-byte copies into st->0x8FC are V3 struct assignments;
 * - CTX is compared `st->0x8DC == CTX` (edg operand order);
 * - the context pointer is one named local whose first def is the call
 *   argument (`func_cfa0(ctx = CTX, ...)`), which colours the post-call
 *   reload a0 like the target;
 * - frame 0x88: every referenced named local takes an edg slot even when it
 *   is register-only; dst / o / vt are the original's (dead here) names and
 *   the dead `= 0` stores keep their three slots.
 * docs/IDO_CODEGEN.md#kit-xvi-agent-c-run-116. Spliced into bootup_uso.c.o
 * via REPLACE_FUNC_BODY; the bu_cfa0_* aliases are pinned in
 * undefined_syms_auto.txt (ctx 0x254 = func_0000023C+0x18, flag 0xE4,
 * k970 0x970, k978 0x978, the per-site blank args a..f and the two callees 0). */
extern "C" {
char *bu_cfa0_probe(char *, int, int, int, float, float *, float);
extern char bu_cfa0_a, bu_cfa0_b, bu_cfa0_c, bu_cfa0_d, bu_cfa0_e, bu_cfa0_f;
extern char *bu_cfa0_ctx;
extern int bu_cfa0_flag;
extern float bu_cfa0_k970;
extern double bu_cfa0_k978;
int func_00000000(...);
void bu_cfa0_v(...);
}
#define CTX bu_cfa0_ctx
struct VO { char pad[0x28]; virtual void v0(); virtual void v1(); virtual void f(); };
struct V3 { float x, y, z; };
#define DISPATCH_CFA0(off) { (*(VO **)(st + (off)))->f(); }
extern "C" void func_0000CFA0(char *st) {
    float out;
    volatile int changed;
    char *r;
    char *ctx;
    char *p;
    float c[4];
    char *dst;
    int flg;
    char *o;
    char *vt;
    char *t;
    float dp[3];

    dst = 0; o = 0; vt = 0;
    if ((*(int *)(st + 0xA58) & 0x2000) && *(char **)(st + 0x8DC) == CTX) return;
    r = bu_cfa0_probe(ctx = CTX, 2, *(int *)(st + 0xB4), *(int *)(st + 0xB8),
                  *(float *)(st + 0xBC), &out, 0.0f);
    *(char **)(st + 0xA40) = r;
    if (r == 0) return;
    ctx = CTX;
    changed = (*(float *)(ctx + 0x98) < out) || (ctx != *(char **)(st + 0x8DC));
    if ((*(int *)(st + 0xA58) & 0x10000) && *(short *)(st + 0x9A0) == 0x61 &&
        *(short *)(st + 0x904) != 9 && *(int *)(st + 0x938) != 0) {
        p = (char *)func_00000000(&bu_cfa0_a, 0xD);
        *(V3 *)*(char **)(st + 0x8FC) = *(V3 *)p;
        bu_cfa0_v(0x10);
        c[0] = 1.0f; c[1] = 1.0f; c[2] = 1.0f;
        c[3] = bu_cfa0_k970;
        bu_cfa0_v(&bu_cfa0_b, c);
        bu_cfa0_v(0x10);
        DISPATCH_CFA0(0x824);
        p = (char *)func_00000000(&bu_cfa0_c, 6);
        *(V3 *)*(char **)(st + 0x8FC) = *(V3 *)p;
    }
    if (!changed || !(*(int *)(st + 0x8B8) & 4)) {
        flg = *(int *)(st + 0x9A8);
        *(int *)(st + 0xA44) = 1;
        if ((flg & 2) && (*(int *)(st + 0xA58) & 0x1000) && *(int *)(st + 0xA4C)) {
            bu_cfa0_v(*(int *)(st + 0x850), st + 0x9D8);
            bu_cfa0_v(st);
            bu_cfa0_v(0x11);
            t = st + 0x8E0;
            bu_cfa0_v(&bu_cfa0_d, t);
            bu_cfa0_v(st);
            bu_cfa0_v(st);
            bu_cfa0_flag = 0;
        } else {
          if ((*(int *)(st + 0x8B8) & 0xA) && *(int *)(st + 0xA4C) && !(flg & 0x400)) {
            bu_cfa0_v(*(int *)(st + 0x850), st + 0x9D8, dp);
            if ((double)(dp[0] * *(float *)(st + 0x9D8) + dp[1] * *(float *)(st + 0x9DC) +
                         dp[2] * *(float *)(st + 0x9E0)) < bu_cfa0_k978) {
                float div255 = 255.0f;
                unsigned char cr = *(unsigned char *)(st + 0xA30) / 2;
                unsigned char cg = *(unsigned char *)(st + 0xA31) / 2;
                unsigned char cb = *(unsigned char *)(st + 0xA32) / 2;
                *(float *)(st + 0x9E8) = (float)(unsigned int)cr / div255;
                *(float *)(st + 0x9EC) = (float)(unsigned int)cg / div255;
                *(float *)(st + 0x9F0) = (float)(unsigned int)cb / div255;
                *(float *)(st + 0x9F4) = 112.0f / div255;
                bu_cfa0_v(&bu_cfa0_e, st + 0x9E8);
                if (*(int *)(st + 0x8B8) & 2) {
                    bu_cfa0_v(2);
                    DISPATCH_CFA0(0x814);
                    DISPATCH_CFA0(0x820);
                }
                if ((*(int *)(st + 0x8B8) & 8) && *(int *)(st + 0x938) == 0) {
                    bu_cfa0_v(8);
                    DISPATCH_CFA0(0x824);
                }
            }
            bu_cfa0_flag = 0;
          }
          t = st + 0x8E0;
        }
        if (*(int *)(st + 0x8B8) & 1) {
            bu_cfa0_v(0x11);
        } else {
            bu_cfa0_v(0x14);
        }
    } else {
        *(int *)(st + 0xA44) = 0;
        bu_cfa0_v(0x14);
        t = st + 0x8E0;
    }
    bu_cfa0_v(&bu_cfa0_f, t);
    bu_cfa0_v(st);
    bu_cfa0_v(*(int *)(CTX + 0x108));
}
