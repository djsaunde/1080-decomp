/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004D688 (0x3B8 / 238 words): the debug-console event
 * loop as `void Obj::run()` (renamed with objcopy). agent-c run 137 (extern "C" goto form, 83.62) rewritten by
 * agent-g run 192 as a member function with a structured for(;;) loop -> 95.45 standalone, same length as the target.
 * Load-bearing:
 *   - the dispatch is an edg `switch` in source case order ('e', 'f', 'd', 'g'/'h', 'l', default: plain `beq K`
 *     chain + `b default`, kits LXI / CXLV); under cfe it would be a jumptable.
 *   - for(;;) instead of a goto ladder: uopt hoists &D_4D688_g2 and the constant 1 into s7 / s1 as the target does.
 *   - the 0x3CB00 guard pair is ONE static long long compared against 0x1234567812345678 (agent-c run 137): each
 *     half re-materialises the constant in at; two int compares let uopt hoist the magic into a ninth s-register.
 *   - `if (0) gl_func_00034458(&e);` keeps the polled event object in a stack home (spilled around the poll).
 *   - decode fix: the 'd' toggle is a POST-increment test (`if (n++ & 1)`: the target masks the old value).
 *   - the 'l' case passes a third lui-zero global (D_4D688_g3), not the s5 base.
 *   - m24 volatile: the target re-reads the counter after its increment (same length as the target).
 *   - locals initialised as statements in the order n, cur, pend, keep.
 * Residual: the polled object is reloaded into a ring temp (target a0), which shifts the t-ring by one through the
 * 'e' case; `keep = 1` copies s1 (target materialises 1); a 5-word alignment pad before the dead epilogue.
 * agent-g run 204: the target's poll-object slot sp+56 is a REGISTER LR spilled to its named home (reloaded into
 * a0), not an address-taken local: `int k;` declared before e (k takes sp+60, e sp+56) and the `&e` keep
 * replaced by a dead multi-def `if (0) { e = 0; }`; the t-ring lines up. Then a statement grid: m3C before the
 * volatile m24++, and the 'g'/'h' handover as `m4 = m8; m8 = 0; m28 = 0;` (no named t): 53 -> 2 words off
 * (standalone 95.76 -> 96.95). Left: the old-n copy of `n++ & 1` in v0 (target a0, hoisted into the switch delay).
 * Still NON_MATCHING: build/non_matching/ only. */
extern "C" {
int gl_func_00034458(...);
extern char D_00000000[];
extern char D_4D688_g1[];
extern int D_4D688_g2;
extern char D_4D688_g3[];
}
#define DSTR(o) (D_00000000 + (o))
static long long sG;
struct Ev { char pad[0x1C]; int key; };
struct Q { char pad[0x1C]; Ev *ev; };
struct Child { char pad[0x5C]; int m5C; };
struct Obj {
    int m0, m4, m8;
    Child *mC;
    int m10, m14;
    int m18, m1C, m20;
    volatile int m24;
    int m28;
    char pad2c[0x3C - 0x2C];
    char *m3C, *m40;
    void *m44;
    int m48;
    void run();
};
void Obj::run()
{
    int n, pend, cur, keep;
    m48 = 0;
    m18 = 0;
    m40 = DSTR(0x2021C);
    m10 = 0;
    n = 0;
    cur = 0;
    pend = 0;
    keep = 0;
    gl_func_00034458(D_4D688_g1);
    for (;;) {
        if (sG != 0x1234567812345678LL) {
            gl_func_00034458(DSTR(0x20224));
        }
        int k; /* run 204: homed above e */
        Ev *e = ((Q *)D_4D688_g1)->ev;
        D_4D688_g2 = 0;
        gl_func_00034458(e, &e->key, 1);
        k = e->key;
        if (0) { e = 0; } /* run 204: dead multi-def (register LR spilled to its home), not &e */
        D_4D688_g2 = 1;
        switch (k) {
        case 'e':
            m20 = 0;
            if (m18 != 0) {
                gl_func_00034458(DSTR(0x20240), k);
                Child *c = mC;
                if (c != 0 && c->m5C != 0) {
                    m3C = DSTR(0x20248);
                    gl_func_00034458(c);
                    m40 = DSTR(0x20250);
                    m20 = 1;
                } else {
                    gl_func_00034458(this);
                    gl_func_00034458(this);
                }
                m18 = 0;
            } else if (keep != 0) {
                keep = 0;
                mC->m5C = gl_func_00034458((char *)mC + 0x10, k);
                m18 = pend;
                gl_func_00034458();
                m3C = DSTR(0x2025C);
                gl_func_00034458(DSTR(0x20260));
                gl_func_00034458(m18);
                gl_func_00034458(m18);
                m40 = DSTR(0x20268);
                m20 = 1;
            } else {
                gl_func_00034458(this, k);
                gl_func_00034458(this);
            }
            break;
        case 'f':
            m1C = 0;
            gl_func_00034458(DSTR(0x2026C), k);
            break;
        case 'd':
            if (n++ & 1) {
                gl_func_00034458(D_4D688_g1, k);
                gl_func_00034458(D_4D688_g1);
            }
            pend = cur;
            if (cur != 0) {
                if (mC != 0) {
                    gl_func_00034458();
                    keep = 1; /* after the call (local; lever-sweep order 95.45 -> 95.71) */
                } else {
                    m18 = cur;
                    gl_func_00034458();
                    m3C = DSTR(0x20270);
                    gl_func_00034458(DSTR(0x20274));
                    gl_func_00034458(m18);
                    gl_func_00034458(m18);
                    m40 = DSTR(0x2027C);
                    m20 = 1;
                }
            }
            m3C = DSTR(0x20280);
            gl_func_00034458(DSTR(0x20288));
            cur = gl_func_00034458();
            gl_func_00034458(DSTR(0x20290));
            {
                void *p = m44;
                m3C = DSTR(0x20298); /* run 204: back before m24++ */
                m24++;
                if (p != 0 && (m24 & 1)) {
                    gl_func_00034458(p, 'd', 1);
                }
            }
            break;
        case 'g':
        case 'h':
            if (mC != 0) {
                gl_func_00034458(DSTR(0x202A0), k);
            }
            if (m4 != 0) {
                gl_func_00034458(DSTR(0x202B4));
            }
            m4 = m8; /* run 204: no named t (target reads m8 into a ring temp) */
            m8 = 0;
            m28 = 0;
            if (m18 == 0) {
                gl_func_00034458(this);
            }
            gl_func_00034458(DSTR(0x202D0));
            break;
        case 'l':
            gl_func_00034458(D_4D688_g3, k);
            gl_func_00034458();
            m48 = 1;
            break;
        default:
            gl_func_00034458(DSTR(0x202D4), k);
            break;
        }
    }
}
