/* IDO 7.1 NCC (C++) donor for gl_func_000478FC (0x1B8 / 110 words): for every
 * child whose mask matches, mark it (flag 2), remember the 0x80 one, rebuild
 * its offset from the anchor when flag 0x40 is clear (count it, callee, dirty
 * = 1), then OR the dirty state into the owner and rescale when set; clear
 * flag 2 otherwise. The flag word is a Flags sub-object reached through a
 * body-scope reference (the target's addiu v0,s0,196 in both arms); the three
 * owner resets are written in the reverse of their store order.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
struct V3 { float x, y, z; };
struct Flags { int v;
    void set(int m) { v |= m; }
    void clr(int m) { v &= ~m; } };
struct Obj { char pad[0xA0]; V3 pos; char padac[0x18]; Flags fl; char padc8[8]; int mask; char padd4[0xC]; int dirty; V3 d; char padf0[0xC]; V3 s; char pad108[0x1C]; V3 scale; };
struct Link { Obj *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Obj *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Own { char pad[0x18C]; Link *head; char pad190[4]; Obj *sel; char pad198[0xAC]; int any; int cnt; };
extern "C" void gl_func_000478FC(Own *ow, V3 *p, int mask, float k, int force) {
    Iter it(ow->head);
    ow->any = 0;
    ow->cnt = 0;
    ow->sel = 0;
    for (Obj *o = it.next(); o; o = it.next()) {
        Flags &f = o->fl;
        if (o->mask & mask) {
            if (o->fl.v & 0x80) ow->sel = o;
            f.set(2);
            if (!(o->fl.v & 0x40)) {
                ow->cnt++;
                o->d.x = o->pos.x - p->x;
                o->d.y = o->pos.y - p->y;
                o->d.z = o->pos.z - p->z;
                gl_func_00000000(&o->d);
                o->dirty = 1;
            }
            if (ow->any = ow->any | o->dirty | force) {
                o->s.x = o->scale.x * k;
                o->s.y = o->scale.y * k;
                o->s.z = o->scale.z * k;
            }
        } else f.clr(2);
    }
}
