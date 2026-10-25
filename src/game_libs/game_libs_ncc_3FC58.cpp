/* IDO 7.1 NCC (C++) EXACT donor for gl_func_0003FC58 (0x2E8 / 186 words + 1 all-zero pad word), agent-c run 163.
 * Message pump: read one message, set flag bit 2, dispatch on msg.type, clear bit 2 of the second
 * flag word. The cfe body (85.12) spelled the compare chain with gotos; under NCC a plain switch IS
 * the target's chain (52, 22, 10, 9, 7, 8, 21, 23, 25, 27, 28; NCC never emits a jumptable).
 * Load-bearing: case 52's argument is a named `W t(msg.p12.i)` passed by const reference to virtual
 * f5 (the target's sp+0x20 slot: named locals are homed in declaration order, so six function-top
 * volatile pads after `out` put t at the bottom of the 0x180 frame); the two flag words are read and
 * written through separate symbols (the target re-forms each address). Vptrs at +0 (O0) and +0x28
 * (O28). Spliced over an empty placeholder in both build trees; the target's trailing alignment
 * word is an all-zero SUFFIX. */
extern "C" {
void gl_func_00000000(...);
extern char D_00000000[];
extern int D_3FC58_flags, D_3FC58_flagsW, D_3FC58_flags2, D_3FC58_flags2W;
extern char D_3FC58_s1[];
extern char D_3FC58_g[];
}
struct W { int w; W(int x) { w = x; } };
struct O28 { char p[0x28]; virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4(); virtual void f5(const W &); };
struct O0 { virtual void f1(); };
struct Msg {
    int type;
    int p4;
    int p8;
    union { int i; float f; } p12;
    void *p16;
    char pad14[0x34];
    void *obj;
    int p4C;
    char pad50[0x50];
};
extern "C" void gl_func_0003FC58(int arg0) {
    Msg msg;
    Msg out;
    volatile int p0, p1, p2, p3, p4, p5;
    gl_func_00000000(&msg);
    D_3FC58_flagsW = D_3FC58_flags | 4;
    switch (msg.type) {
    case 52: {
        O28 *o = (O28 *)msg.p8;
        W t(msg.p12.i);
        o->f5(t);
        break;
    }
    case 22:
        ((O28 *)msg.obj)->f1();
        gl_func_00000000(D_3FC58_s1);
        break;
    case 10:
        out = msg;
        switch (msg.p8) {
        case 1: out.p12.i = *(int *)msg.p16; break;
        case 2: out.p12.f = *(float *)msg.p16; break;
        }
        out.type = 9;
        gl_func_00000000(&out);
        break;
    case 9:
        switch (msg.p8) {
        case 0: *(short *)msg.p16 = msg.p12.i; break;
        case 1: *(int *)msg.p16 = msg.p12.i; break;
        case 2: *(float *)msg.p16 = msg.p12.f; break;
        }
        break;
    case 7:
        *(int *)msg.p16 &= msg.p12.i;
        break;
    case 8:
        *(int *)msg.p16 |= msg.p12.i;
        break;
    case 21:
        ((O0 *)msg.obj)->f1();
        if (msg.p4C) gl_func_00000000(arg0);
        break;
    case 23:
        ((O0 *)msg.obj)->f1();
        if (msg.p4C) gl_func_00000000(arg0);
        break;
    case 25:
        break;
    case 27:
        gl_func_00000000();
        break;
    case 28: {
        O28 *o = *(O28 **)(D_3FC58_g + 0x1C8);
        out.type = 25;
        out.obj = o;
        if (o == 0) {
            gl_func_00000000(D_00000000 + 0x1F3B0);
        } else {
            o->f1();
        }
        gl_func_00000000(arg0);
        break;
    }
    default:
        gl_func_00000000(D_00000000 + 0x1F3CC, msg.type);
        break;
    }
    D_3FC58_flags2W = D_3FC58_flags2 & ~4;
}
