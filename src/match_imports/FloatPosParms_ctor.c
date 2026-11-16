/* Native original-compiler source port from src/bootup_uso/bootup_uso_tail4.c. Target bytes unchanged. */
#include "context_bootup_uso_tail4.h"
#include "gdl/gadget.h" /* GdlParms head + GdlParmF (float parm, type vtable Data 0xC83C) */
char *FloatPosParms_ctor(char *arg0) {
    Val13D40 x;
    Val13D40 y;
    char pad[12];
    char *c;
    Val13D40 p;
    if (arg0 == 0) {
        arg0 = bootup_1438C_alloc(0x38);
        if (arg0 == 0) goto end;
    }
    {
        char *s;
        s = arg0;
        if (s != 0 || (s = bootup_1438C_alloc(8)) != 0) {
            PARMS(s)->name = (char *)&sTimeGadgetParmsTags_CACC[3]; /* the Parms name string */
            PARMS(s)->unk04 = 0;
        }
    }
    x = sFloatParmsTags_CAE8[1];
    p.v = x.v;
    c = arg0 + 8;
    if (c != 0 || (c = bootup_1438C_alloc(0x18)) != 0) {
        bootup_1438C_init(c, arg0, p, 1);
        PARMF(c)->type = &D_0000C83C; /* float parm type */
        PARMF(c)->unk14 = 0;
        PARMF(c)->value = 30.0f;
    }
    y = sFloatParmsTags_CAE8[2];
    p.v = y.v;
    c = arg0 + 0x20;
    if (c != 0 || (c = bootup_1438C_alloc(0x18)) != 0) {
        bootup_1438C_init(c, arg0, p, 1);
        PARMF(c)->type = &D_0000C83C; /* float parm type */
        PARMF(c)->unk14 = 0;
        PARMF(c)->value = 30.0f;
    }
end:
    return arg0;
}
