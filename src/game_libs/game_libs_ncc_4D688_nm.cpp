/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004D688 (0x3B8 / 238 words), agent-g run 192.
 * Event-loop driver `void Obj::run()` (renamed with objcopy). The target's dispatch is an edg `switch` in source
 * case order ('e', 'f', 'd', 'g'/'h', 'l', default: `beq K` chain + `b default`, kit CXLV); under cfe the same
 * switch becomes a jumptable, which is why the old cfe body used an if/goto chain. Under NCC the loop also holds
 * &D_4D688_g2 and the constant 1 in callee-saved registers as the target does (s7 / s1).
 * Decode fixes vs the cfe body: the 'd' case toggle is a POST-increment test (`if (n++ & 1)`: the target masks the
 * old value, `or a0,s3; andi; addiu s3,s3,1`); the 'l' case passes a third lui-zero global (D_4D688_g3, not s5).
 * Residual: the target re-materialises the 0x12345678 magic (lui at / ori at per compare) while uopt hoists it into
 * a ninth callee-saved register here (s8; frame 88 vs 80), and the counter m24 is re-read after its increment
 * (volatile m24 reproduces the reload but costs 2 words: not kept). Same length as the target.
 * Still NON_MATCHING: build/non_matching/ only. */
extern "C" {
int gl_func_00034458(...);
extern char D_00000000[];
extern char D_4D688_g1[];
extern int D_4D688_g2;
extern char D_4D688_g3[];
}
#define DSTR(o) (D_00000000 + (o))
struct Ev { char pad[0x1C]; int key; };
struct Q { char pad[0x1C]; Ev *ev; };
struct Child { char pad[0x5C]; int m5C; };
struct Obj {
    int m0, m4, m8;
    Child *mC;
    int m10, m14;
    int m18, m1C, m20, m24, m28;
    char pad2c[0x3C - 0x2C];
    char *m3C, *m40;
    void *m44;
    int m48;
    void run();
};
void Obj::run()
{
    int n = 0;
    int pend = 0;
    int cur = 0;
    int keep = 0;
    m48 = 0;
    m18 = 0;
    m40 = DSTR(0x2021C);
    m10 = 0;
    gl_func_00034458(D_4D688_g1);
    for (;;) {
        if ((*(int *)0x3CB00 != 0x12345678) || (*(int *)0x3CB04 != 0x12345678)) {
            gl_func_00034458(DSTR(0x20224));
        }
        Ev *e = ((Q *)D_4D688_g1)->ev;
        D_4D688_g2 = 0;
        gl_func_00034458(e, &e->key, 1);
        int k = e->key;
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
                    keep = 1;
                    gl_func_00034458();
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
                m3C = DSTR(0x20298);
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
            {
                int t = m8;
                m8 = 0;
                m28 = 0;
                m4 = t;
            }
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
