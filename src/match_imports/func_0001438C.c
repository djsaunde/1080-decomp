/* Native original-compiler source port from src/bootup_uso/bootup_uso_tail4.c. Target bytes unchanged. */
#include "context_bootup_uso_tail4.h"
char *func_0001438C(char *arg0) {
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
            *(char **)(s + 0) = (char *)&func_0000CACC[3];
            *(int *)(s + 4) = 0;
        }
    }
    x = func_0000CAE8[1];
    p.v = x.v;
    c = arg0 + 8;
    if (c != 0 || (c = bootup_1438C_alloc(0x18)) != 0) {
        bootup_1438C_init(c, arg0, p, 1);
        *(char **)(c + 0xC) = &D_0000C83C;
        *(int *)(c + 0x14) = 0;
        *(float *)(c + 0x10) = 30.0f;
    }
    y = func_0000CAE8[2];
    p.v = y.v;
    c = arg0 + 0x20;
    if (c != 0 || (c = bootup_1438C_alloc(0x18)) != 0) {
        bootup_1438C_init(c, arg0, p, 1);
        *(char **)(c + 0xC) = &D_0000C83C;
        *(int *)(c + 0x14) = 0;
        *(float *)(c + 0x10) = 30.0f;
    }
end:
    return arg0;
}
