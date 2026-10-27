/* IDO 7.1 NCC (C++) donor for gl_func_000525F0 (0x84 / 33 words): Self::reg(Tgt *t), a C++ member function.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (symbols renamed in the Makefile, names only).
 * (agent-c run 180) The "lazy factory call gl_func(0, &D + 0x20FC0, self)" of the cfe decode is cfront's
 * `new Foo(cfg, this)` with an out-of-line constructor (this == 0 -> the ctor allocates), and the second call is
 * a member call on the Foo's +0x10 subobject. cfe capped at 99.55 on the obj spill reload (v1 vs the target's v0,
 * ugen temploc reuse); under NCC the same logic emits the target's fresh v0 reload: EXACT. */
extern "C" {
extern char D_00000000[];
}
struct Self;
struct Tgt { int x0; int x4; char pad8[0xC]; struct Foo *x14; };
struct Sub { void add(Tgt *t); };
struct Foo { char pad[0x10]; Sub s; Foo(char *cfg, Self *o); };
struct Self { char pad[0x2C]; Foo *obj; void reg(Tgt *t); };
void Self::reg(Tgt *t) {
    if (obj == 0) obj = new Foo(D_00000000 + 0x20FC0, this);
    Foo *f = obj;
    f->s.add(t);
    if (t->x14 != 0) t->x4 = 1;
    t->x14 = f;
}
