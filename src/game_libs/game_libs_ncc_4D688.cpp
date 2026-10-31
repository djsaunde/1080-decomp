/* IDO 7.1 NCC (C++) donor for gl_func_0004D688 -- EXACT (agent-g run 256), spliced over the post0b placeholder on
 * both build paths via REPLACE_FUNC_BODY (only gl_func_0004D688 = `Obj::run()` is taken from this object).
 * The debug-console event loop (agent-c run 137 NCC goto form; agent-g runs 192 / 204: member function, for(;;) loop
 * holding &g2 / 1, source-order edg switch = beq chain, static guard pair, k homed above e, dead multi-def on e;
 * agent-g run 256: the old-n copy of the 'd' toggle through the dead e, `e = (Ev *)n; n++; if ((int)e & 1)`, which
 * puts it in a0 like the target).
 * TRANSLATION UNIT: the loop never returns, so ugen emits `.align 5` before the dead epilogue. In a standalone object
 * that pads 5 nops (run() at offset 0, epilogue at 0x38C); the target has none because run() was NOT first in its
 * file. The guard static is shared with gl_func_0004D468 (it stores the 0x1234567812345678 magic that run() checks),
 * so this file reconstructs the unit from gl_func_0004D354 (the nearest preceding function whose start puts run()'s
 * epilogue on a 32-byte boundary: 0x334 + 0x38C = 0x6C0): 4D354 / 4D39C / 4D3E4 (the post0b C bodies as extern "C"),
 * 4D468 (the exact NCC donor body, see game_libs_ncc_4D468.cpp for its notes), 4D658, then run(). Every compiled
 * size equals the target's (4D3E4 scores 99.70 under NCC; only run() is spliced). The guard's .bss reloc is renamed
 * gl_func_0004D688_bss by the splice (pinned to 0x3CB00, the address gl_func_0004D468_bss already uses); calls go
 * through gl_func_00000000 (the USO's zero jal). */
extern "C" {
int gl_func_00000000(...);
extern char D_00000000[];
extern char D_4D688_g1[];
extern int D_4D688_g2;
extern char D_4D688_g3[];
extern int D_4D468_x, D_4D468_y, D_4D468_z;
extern char D_4D468_w[];
extern char D_4D468_a2[];
}
static long long sG;
extern "C" void gl_func_0004D354(int *arg0) {
    gl_func_00000000(arg0);
    gl_func_00000000((char*)arg0 + 0x10);
    gl_func_00000000((char*)arg0 + 0x10);
    arg0[0x60/4] = 1;
    arg0[0x64/4] = 0;
}
extern "C" void game_libs_func_0004D39C(int *a0, int **a1) {
    if (*a1 != 0) {
        *(int*)((char*)a0 + 0x50) = (int)*a1;
        *(int*)((char*)a0 + 0x54) = *(int*)((char*)*a1 + 0x54);
        *(int*)((char*)*(int**)((char*)*a1 + 0x54) + 0x50) = (int)a0;
        *(int*)((char*)*a1 + 0x54) = (int)a0;
    } else {
        *(int*)((char*)a0 + 0x50) = (int)a0;
        *(int*)((char*)a0 + 0x54) = (int)a0;
        *a1 = a0;
    }
}
extern "C" void gl_func_0004D3E4(int node, int *head) {
    if (*head == 0) {
        gl_func_00000000(D_00000000 + 0x201E4, head, node);
    }
    if (*(int *)(node + 0x50) == *(int *)(node + 0x54) && node == *(int *)(node + 0x54)) {
        *head = 0;
        return;
    }
    *head = *(int *)(*head + 0x50);
    *(int *)(*(int *)(node + 0x54) + 0x50) = *(int *)(node + 0x50);
    *(int *)(*(int *)(node + 0x50) + 0x54) = *(int *)(node + 0x54);
}
struct E { E() {} char pad[0x78]; };
void *operator new(unsigned int n, int al);
struct S4D { int w0, w4, w8, wC; int w10; int w14; int w18; int w1C, w20, w24, w28, w2C; char pad30[0x14]; int w44; };
inline void bind(int k, int h, int id) { if (0) { h = 0; } gl_func_00000000(k, h, id); }
extern "C" S4D *gl_func_0004D468(S4D *s) {
    if (s != 0 || (s = (S4D *)gl_func_00000000(0x50)) != 0) {
    gl_func_00000000(D_00000000 + 0x201F4, 0);
    gl_func_00000000(D_00000000 + 0x3EB00, 0xB0000B70, 0x100);
    s->w44 = 0;
    s->w0 = 0;
    s->w4 = 0;
    s->w8 = 0;
    s->wC = 0;
    for (int i = 0; i < 16; i++) {
        int p = gl_func_00000000(0x78, 16u); gl_func_00000000(p ? p : 0, s);
    }
    { int p = gl_func_00000000(0x78, 16); s->w14 = p ? p : 0; }
    D_4D468_x = gl_func_00000000(0xC00, 16);
    D_4D468_y = 0x19000;
    D_4D468_z = gl_func_00000000(D_4D468_y + 4, 64);
    ((unsigned *)D_4D468_z)[(unsigned)D_4D468_y / 4] = 0x12345678u;
    s->w2C = gl_func_00000000(0x400, 16);
    gl_func_00000000(D_00000000 + 0x3C950, 4, D_4D468_a2, s, D_00000000 + 0x3EB00, 11);
    sG = 0x1234567812345678LL;
    bind(9, *(int *)(D_4D468_w + 0x1C), 102);
    { int h = *(int *)(D_4D468_w + 0x1C); gl_func_00000000(4, h, 101); }
    { int h = *(int *)(D_4D468_w + 0x1C); gl_func_00000000(14, h, 108); }
    gl_func_00000000(D_00000000 + 0x3C950);
    s->w20 = 0;
    s->w1C = 0;
    s->w24 = 0;
    s->w28 = 0;
    gl_func_00000000();
    s->w20 = 0;
    s->w1C = 0;
    }
    return s;
}

extern "C" void gl_func_0004D658(int a0) {
    gl_func_00000000(D_00000000 + 0x000201FC);
    gl_func_00000000(a0);
}
#define DSTR(o) (D_00000000 + (o))
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
    gl_func_00000000(D_4D688_g1);
    for (;;) {
        if (sG != 0x1234567812345678LL) {
            gl_func_00000000(DSTR(0x20224));
        }
        int k; /* run 204: homed above e */
        Ev *e = ((Q *)D_4D688_g1)->ev;
        D_4D688_g2 = 0;
        gl_func_00000000(e, &e->key, 1);
        k = e->key;
        if (0) { e = 0; } /* run 204: dead multi-def (register LR spilled to its home), not &e */
        D_4D688_g2 = 1;
        switch (k) {
        case 'e':
            m20 = 0;
            if (m18 != 0) {
                gl_func_00000000(DSTR(0x20240), k);
                Child *c = mC;
                if (c != 0 && c->m5C != 0) {
                    m3C = DSTR(0x20248);
                    gl_func_00000000(c);
                    m40 = DSTR(0x20250);
                    m20 = 1;
                } else {
                    gl_func_00000000(this);
                    gl_func_00000000(this);
                }
                m18 = 0;
            } else if (keep != 0) {
                keep = 0;
                mC->m5C = gl_func_00000000((char *)mC + 0x10, k);
                m18 = pend;
                gl_func_00000000();
                m3C = DSTR(0x2025C);
                gl_func_00000000(DSTR(0x20260));
                gl_func_00000000(m18);
                gl_func_00000000(m18);
                m40 = DSTR(0x20268);
                m20 = 1;
            } else {
                gl_func_00000000(this, k);
                gl_func_00000000(this);
            }
            break;
        case 'f':
            m1C = 0;
            gl_func_00000000(DSTR(0x2026C), k);
            break;
        case 'd':
            e = (Ev *)n; /* agent-g run 256: the old n through the dead e (its poll web reloads into a0; kit CCLXX / CCLXXX) */
            n++;
            if ((int)e & 1) {
                gl_func_00000000(D_4D688_g1, k);
                gl_func_00000000(D_4D688_g1);
            }
            pend = cur;
            if (cur != 0) {
                if (mC != 0) {
                    gl_func_00000000();
                    keep = 1; /* after the call (local; lever-sweep order 95.45 -> 95.71) */
                } else {
                    m18 = cur;
                    gl_func_00000000();
                    m3C = DSTR(0x20270);
                    gl_func_00000000(DSTR(0x20274));
                    gl_func_00000000(m18);
                    gl_func_00000000(m18);
                    m40 = DSTR(0x2027C);
                    m20 = 1;
                }
            }
            m3C = DSTR(0x20280);
            gl_func_00000000(DSTR(0x20288));
            cur = gl_func_00000000();
            gl_func_00000000(DSTR(0x20290));
            {
                void *p = m44;
                m3C = DSTR(0x20298); /* run 204: back before m24++ */
                m24++;
                if (p != 0 && (m24 & 1)) {
                    gl_func_00000000(p, 'd', 1);
                }
            }
            break;
        case 'g':
        case 'h':
            if (mC != 0) {
                gl_func_00000000(DSTR(0x202A0), k);
            }
            if (m4 != 0) {
                gl_func_00000000(DSTR(0x202B4));
            }
            m4 = m8; /* run 204: no named t (target reads m8 into a ring temp) */
            m8 = 0;
            m28 = 0;
            if (m18 == 0) {
                gl_func_00000000(this);
            }
            gl_func_00000000(DSTR(0x202D0));
            break;
        case 'l':
            gl_func_00000000(D_4D688_g3, k);
            gl_func_00000000();
            m48 = 1;
            break;
        default:
            gl_func_00000000(DSTR(0x202D4), k);
            break;
        }
    }
}
