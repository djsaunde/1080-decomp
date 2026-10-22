/* IDO 7.1 NCC (C++) donor for gl_func_00055470 (0x430 / 268 words): the command-stream
 * opcode dispatcher. A `switch` under edg is a compare chain in CASE order (1014, 1012,
 * ... 1018), which is why the cfe goto-ladder NM needed `if (1) goto` tricks. Shapes:
 * - `next()` = `return cmd = *(*cp)++;` (inline, its result temp is the v1 value);
 *   1012 calls it as a statement and tests `f18 & cmd` (gives `and t8,t7,v1`);
 * - the flag cases are inline Flags members on the D_00000000 block (this = a0, m = a1);
 * - 1017 / 1003 pass the 76-byte Strm by value (edg copy loops);
 * - default: a REAL cfront virtual call (vptr at +0x28 behind a 40-byte base, 7th
 *   virtual = entry +0x38: `lh delta; lw fn; jalr; addu a0,delta,obj`), the {cmd, cur}
 *   pair `vt` declared at function scope after a dead `Two pt` (frame 0x130: NCC drops
 *   unused scalars but keeps a class object), filled by an inline set() through a block
 *   int `c` and followed by `if (1) {}`.
 * gl_ref_55470_d is a distinct zero alias (the plain D_00000000 store would CSE with
 * the Flags base). Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char D_00000000[];
extern int gl_ref_55470_d;
int gl_func_00000000(...);
}
struct Two { int a, b; Two() {} Two(int x, int y) { a = x; b = y; } void set(int x, int y) { a = x; b = y; } };
struct Obj;
struct ObjPad { char pad[40]; };
struct Obj : ObjPad { virtual void m1(); virtual void m2(); virtual void m3(); virtual void m4(); virtual void m5(); virtual void m6(); virtual void m7(int *); };
struct Flags { int a; int b;
    void orA(int m) { a |= m; } void clrA(int m) { a &= ~m; }
    void orB(int m) { b |= m; } void clrB(int m) { b &= ~m; } };
struct Strm {
    int **cp; int cmd; int w2; int w3; Obj *obj; int w5, w6, w7, w8, w9, w10, w11, w12;
    int f13; int f14; int w15; int f16; int f17; int f18;
    int next() { return cmd = *(*cp)++; }
};
extern "C" void gl_func_00055470(Strm *s) {
    Two pt;
    Two vt;
    switch (s->cmd) {
    case 1014:
        break;
    case 1012:
        s->next(); if (!(s->f18 & s->cmd)) gl_func_00000000(s);
        break;
    case 1015:
        gl_func_00000000(s);
        break;
    case 1010:
        if (s->next() != s->f16) gl_func_00000000(s);
        break;
    case 1011:
        if (s->next() != s->f17) gl_func_00000000(s);
        break;
    case 1016:
        s->f14 = s->next();
        break;
    case 1000:
        break;
    case 1008:
        gl_ref_55470_d = s->next();
        break;
    case 1019:
        break;
    case 1004:
        ((Flags *)D_00000000)->orB(s->next());
        break;
    case 1005:
        ((Flags *)D_00000000)->b &= ~s->next();
        break;
    case 1006:
        ((Flags *)D_00000000)->orA(s->next());
        break;
    case 1007:
        ((Flags *)D_00000000)->a &= ~s->next();
        break;
    case 1017: {
        Strm t = *s;
        int v = s->next();
        t.cp = (int **)&v;
        gl_func_00000000(t);
        break;
    }
    case 1003: {
        Strm t = *s;
        t.w3 = 0;
        t.obj = 0;
        t.w15 = (int)s;
        gl_func_00000000(t);
        break;
    }
    case 113:
        s->f13 = 1;
    default: {
        gl_func_00000000(s);
        { int c = s->cmd; vt.set(c, (int)*s->cp); }
        if (1) {}
        s->obj->m7(&vt.a);
        *s->cp = (int *)vt.b;
        break;
    }
    case 1018:
        gl_func_00000000(D_00000000 + 0x211B0, s->next());
        break;
    }
}
