/* Native original-compiler source port from src/mgrproc_uso/mgrproc_uso.c. Target bytes unchanged. */
#include "context_mgrproc_uso.h"
void mgrproc_uso_func_00001C90(char *arg0) {
    volatile int pt_a, pt_b, pt_c, pt_d;
    float sp50[4];
    volatile int pmid;
    int sp48;
    char *p6a8;
    char *p6d8;
    int w;
    char *q;
    volatile int pad_a, pad_b, pad_c;
    if (*(int *)(arg0 + 0x4F0) & 0x10000) {
        sp50[0] = 1.0f;
        sp50[1] = 1.0f;
        sp50[2] = 1.0f;
        sp50[3] = 1.0f;
        if (*(int *)(arg0 + 0x4E4) >= 0xB) {
            float max = mgrproc_1C90_pool_5CC.field_5CC;
            if (((Mgrproc1C90State *)arg0)->field_7A0 < max) {
                ((Mgrproc1C90State *)arg0)->field_7A0 = ((Mgrproc1C90State *)arg0)->field_7A0 + mgrproc_1C90_pool_5D0.field_5D0;
            }
            if (((Mgrproc1C90State *)arg0)->field_7A4 < max) {
                ((Mgrproc1C90State *)arg0)->field_7A4 = ((Mgrproc1C90State *)arg0)->field_7A4 + mgrproc_1C90_pool_5D4.field_5D4;
            }
        } else {
            if (((Mgrproc1C90State *)arg0)->field_7A0 > 0.0f) {
                ((Mgrproc1C90State *)arg0)->field_7A0 = ((Mgrproc1C90State *)arg0)->field_7A0 - mgrproc_1C90_pool_5D8.field_5D8;
            }
        }
        import_0024E608(&import_8024CAF8, (int)((0, (float)255) * ((Mgrproc1C90State *)arg0)->field_7A0), sp50);
        import_0024F2C8(&import_80263D30);
        import_0024F34C(&import_80263D30, 0xA0, 0x9C, 3);
        import_0024EAA0(&import_8024CAF8, (int)((0, (float)255) * ((Mgrproc1C90State *)arg0)->field_7A0), arg0 + 0x380, arg0 + 0x3A4);
        import_0024F2C8(arg0 + 0x6B0);
        import_0024F34C(arg0 + 0x6B0, 0xA0, 0xB0, 3);
        while (0) { w = *(short *)(arg0 + 0x20); }
        q = *(char **)(arg0 + 0x6F0);
        w = *(short *)(q + 0x20);
        p6d8 = *(char **)(arg0 + 0x6D8);
        sp48 = 0xA0 - ((*(short *)(p6d8 + 0x20) + w + 4) / 2);
        import_0024EAA0(&import_8024CAF8, (int)((0, (float)255) * ((Mgrproc1C90State *)arg0)->field_7A0), arg0 + 0x260, arg0 + 0x284);
        p6a8 = *(char **)(arg0 + 0x6A8);
        if (*(int *)(p6a8 + 8) == (*(int *)(p6a8 + 4) + 1)) {
            import_0024F2C8(arg0 + 0x6E0);
            import_0024F34C(arg0 + 0x6E0, sp48, 0x88, 2);
            import_0024F2C8(arg0 + 0x6C8);
            q = *(char **)(arg0 + 0x6F0);
            w = *(short *)(q + 0x20);
            import_0024F34C(arg0 + 0x6C8, sp48 + w + 4, 0x88, 2);
            return;
        }
        import_0024F2C8(arg0 + 0x6C8);
        import_0024F34C(arg0 + 0x6C8, sp48, 0x88, 2);
        import_0024F2C8(arg0 + 0x6E0);
        q = *(char **)(arg0 + 0x6D8);
        w = *(short *)(q + 0x20);
        import_0024F34C(arg0 + 0x6E0, sp48 + w + 4, 0x88, 2);
    }
}
