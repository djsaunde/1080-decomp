/* Native original-compiler source port from src/bootup_uso/bootup_uso.c. Target bytes unchanged. */
#include "context_bootup_uso.h"
BootObject90 *func_00008124(BootContext44 *arg0) {
    BootObject90 *v;
    BootObject90 *o;
    BootGroup38 *u;
    struct OneI first;
    struct OneI second;
    BootGroup38 *s;
    BootTail18 *tail;
    BootGroup38 *group;
    BootChild18 *c;
    int mode;
    struct OneI p;
    BootObject90 *base;
    o = boot8124_alloc(0x90);
    if (o != 0) {
        base = o;
        boot8124_object_init(base);
        o->f28 = &D_00007878;
        u = (BootGroup38 *)((char *)o + 0x3C);
        if (u != 0 || (u = boot8124_alloc(0x38)) != 0) {
            s = u;
            if (s != 0 || (s = boot8124_alloc(8)) != 0) {
                s->f00 = &D_00007FC4;
                s->f04 = 0;
            }
            first.x = D_00007FCC.x;
            p = first;
            c = (BootChild18 *)((char *)u + 8);
            if (c != 0 || (c = boot8124_alloc(0x18)) != 0) {
                group = u;
                mode = 1;
                boot8124_child(c, group, p, mode);
                c->base.f0C = &D_00007540;
                c->f14 = 0;
                c->f10 = 200.0f;
            }
            second.x = D_00007FD0.x;
            p = second;
            c = (BootChild18 *)((char *)u + 0x20);
            if (c != 0 || (c = boot8124_alloc(0x18)) != 0) {
                group = u;
                mode = 1;
                boot8124_child(c, group, p, mode);
                c->base.f0C = &D_00007540;
                c->f14 = 0;
                c->f10 = 300.0f;
            }
        }
        tail = (BootTail18 *)((char *)o + 0x74);
        u = (BootGroup38 *)tail;
        if (u != 0 || (u = boot8124_alloc(0x18)) != 0) {
            tail = (BootTail18 *)u;
            boot8124_tail_init(tail);
        }
        o->f8C = 0;
    }
    v = o;
    o = (BootObject90 *)arg0->f40;
    if (arg0->f40 != 0) {
        boot8124_link(&v->f10, (BootOwner18 *)o);
        if (((BootOwner18 *)o)->f14 != 0) {
            ((BootOwner18 *)o)->f04 = 1;
        }
        ((BootOwner18 *)o)->f14 = v;
    }
    return v;
}
