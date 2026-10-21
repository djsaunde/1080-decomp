/* IDO 7.1 NCC (C++) donor for func_0000E800 (0xBC / 47 words) -- reads three
 * config ints by key, applies them, then installs a handler pointer at
 * func_00000008+0x20 and calls the root object's virtual slot 10
 * (vptr +0x28: lh +0x58 / lw +0x5C).
 *
 * C++ (agent-c run 116). Needed beyond the cfe logic:
 * - the root object is &D_00000000 itself and the call is a real virtual call;
 * - each blank first argument is its own symbol (bu_e800_g1..g4, bu_e800_v,
 *   all 0): one shared symbol is CSE'd into an s-reg;
 * - a named `root` local declared first gives frame 0x30 (z/x/y homes
 *   40/36/32, every named edg local takes a slot);
 * - the last config call and the handler store share one source line; on two
 *   lines as1 schedules the handler's addiu after `lui v1`.
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. */
extern "C" {
void func_00000000(...);
extern char D_00000000;
extern char bu_e800_g1, bu_e800_g2, bu_e800_g3, bu_e800_g4, bu_e800_v;
extern char D_00008D34, D_00008D48, D_00008D5C;
extern char func_00000008[];
}
struct VO { char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void f(); };
extern "C" void func_0000E800(void) {
    VO *root;
    int z, x, y;
    func_00000000();
    z = 0; x = 0; y = 0;
    func_00000000(&bu_e800_g1, &D_00008D34, &x);
    func_00000000(&bu_e800_g2, &D_00008D48, &y);
    func_00000000(&bu_e800_g3, &D_00008D5C, &z);
    func_00000000(x, z, y);
    func_00000000(&bu_e800_g4); *(void **)(func_00000008 + 0x20) = &bu_e800_v;
    root = (VO *)&D_00000000;
    root->f();
}
