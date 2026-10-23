/* Native original-compiler source port from src/bootup_uso/bootup_uso.c. Target bytes unchanged. */
#include "context_bootup_uso.h"
BootupObjectBase *func_000084A0(BootContext44 *arg0) {
    BootupObjectBase *v;
    BootupObjectBase *o;
    BootGroup38 *u;
    struct OneI first;
    struct OneI second;
    BootGroup38 *s;
    BootupObjectBase *base;
    BootGroup38 *group;
    BootChild18 *c;
    int mode;
    struct OneI p;
    BootContext44 *g;
    g = bootup_84A0_singleton;
    if (g->f40 == 0) bootup_84A0_func_074E8(g);
    o = bootup_84A0_func_055750(0x74);
    if (o != 0) {
        base = o;
        bootup_84A0_func_051C28(base);
        o->field_28 = &D_00007968;
        u = (BootGroup38 *)((char *)o + 0x3C);
        if (u != 0 || (u = bootup_84A0_func_055750(0x38)) != 0) {
            s = u;
            if (s != 0 || (s = bootup_84A0_func_055750(8)) != 0) {
                s->f00 = &D_00007FE4;
                s->f04 = 0;
            }
            first.x = D_00007FEC; p = first;
            c = (BootChild18 *)((char *)u + 8);
            if (c != 0 || (c = bootup_84A0_func_055750(0x18)) != 0) {
                group = u;
                mode = 1;
                bootup_84A0_func_04A188(c, group, p, mode);
                c->base.f0C = &D_00007540;
                c->f14 = 0;
                c->f10 = 300.0f;
            }
            second.x = D_00007FF0; p = second;
            c = (BootChild18 *)((char *)u + 0x20);
            if (c != 0 || (c = bootup_84A0_func_055750(0x18)) != 0) {
                group = u;
                mode = 0;
                bootup_84A0_func_04A188(c, group, p, mode);
                c->base.f0C = bootup_84A0_expected_07620 + 0x70; *(s32 *)&c->f10 = 1;
                c->f14 = 0;
            }
        }
    }
    v = o;
    o = (BootupObjectBase *)arg0->f40;
    if (arg0->f40 != 0) {
        bootup_84A0_func_07ACE0(&v->field_10, o);
        if (o->field_14 != 0) o->field_04 = 1;
        o->field_14 = v;
    }
    return v;
}
