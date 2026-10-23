/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
BootupObjectBase *gl_func_0003800C(BootupObjectBase *o, int a1) {
    char *p;
    char *q;
    char *r;
    if (o == 0) {
        o = (BootupObjectBase *)gl_3800C_alloc(0x2C);
        if (o == 0) goto done;
    }
    p = (char *)o;
    if (o == 0) {
        p = (char *)gl_3800C_alloc(0x18);
        if (p == 0) goto link;
    }
    q = p;
    if (p == 0) {
        q = (char *)gl_3800C_alloc(0x10);
        if (q == 0) goto subc;
    }
    ((BootupObjectBase *)q)->field_08 = 0x8000; ((BootupObjectBase *)q)->field_00 = -1;
    ((BootupObjectBase *)q)->field_04 = 0;
subc:
    r = p + 0x10;
    if (p == (char *)-0x10) {
        r = (char *)gl_3800C_alloc(0x4);
        if (r == 0) goto fields;
    }
    *(int *)r = 0;
fields:
    ((BootupObjectBase *)p)->field_0c.word = a1;
    ((BootupObjectBase *)p)->field_14 = 0;
link:
    o->field_28 = (char *)&gl_3800C_table;
    gl_3800C_init((char *)o + 0x18);
    gl_3800C_set(o, 0);
    p = (char *)*(int *)&gl_3800C_counter;
    *(int *)&gl_3800C_counter = (int)p + 1;
    o->field_1c = (int)p;
    o->field_20 = gl_3800C_get();
done:
    return o;
}
