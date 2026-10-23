/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void timproc_uso_b5_func_00001460(char *arg0) {
    volatile int pad[4];
    char *o, *reg;
    float white;
    float denom = 255.0f;
    float retA[1];
    reg = arg0 + 0x10;
    o = (char *)timproc_uso_b5_func_068D54(0, &timproc_uso_b5_D_807FE8FC + 0x108C);
    *(char **)(arg0 + 0x38) = o;
    white = (float)255 / denom;
    *(float *)(o + 0x124) = white;
    *(float *)(o + 0x128) = (float)255 / denom;
    *(float *)(o + 0x12C) = (float)255 / denom;
    *(float *)(o + 0x130) = (float)255 / denom;
    o = *(char **)(arg0 + 0x38);
    o += 0xC4;
    *(int *)o |= 0x40;
    o = *(char **)(arg0 + 0x38);
    o += 0xC4;
    *(int *)o |= 0x80;
    o = *(char **)(arg0 + 0x38);
    *(int *)(o + 0xD0) = 1;
    o = *(char **)(arg0 + 0x38);
    retA[0] = white;
    timproc_uso_b5_func_062880(&import_8005C108, o);
    o = *(char **)(arg0 + 0x38);
    timproc_uso_b5_func_07ACE0(reg, o);
    if (*(int *)(o + 0x14) != 0) {
        *(int *)(o + 0x4) = 1;
    }
    *(char **)(o + 0x14) = arg0;
    o = (char *)timproc_uso_b5_func_068D54(0, &timproc_uso_b5_D_807FE90C + 0x109C);
    *(char **)(arg0 + 0x3C) = o;
    *(float *)(o + 0x124) = 1.0f;
    *(float *)(o + 0x128) = 1.0f;
    *(float *)(o + 0x12C) = 1.0f;
    *(float *)(o + 0x130) = 1.0f;
    o = *(char **)(arg0 + 0x3C);
    *(int *)(o + 0xD0) = 2;
    timproc_uso_b5_func_062880(&import_8005C108, *(char **)(arg0 + 0x3C));
    o = *(char **)(arg0 + 0x3C);
    timproc_uso_b5_func_07ACE0(reg, o);
    if (*(int *)(o + 0x14) != 0) {
        *(int *)(o + 0x4) = 1;
    }
    *(char **)(o + 0x14) = arg0;
    o = (char *)timproc_uso_b5_func_068D54(0, &timproc_uso_b5_D_807FE918 + 0x10A8);
    *(char **)(arg0 + 0x40) = o;
    *(float *)(o + 0x124) = 174.0f / denom;
    *(float *)(o + 0x128) = 113.0f / denom;
    *(float *)(o + 0x12C) = 61.0f / denom;
    *(float *)(o + 0x130) = retA[0];
    o = *(char **)(arg0 + 0x40);
    o += 0xC4;
    *(int *)o |= 0x40;
    o = *(char **)(arg0 + 0x40);
    o += 0xC4;
    *(int *)o |= 0x80;
    o = *(char **)(arg0 + 0x40);
    *(int *)(o + 0xD0) = 4;
    timproc_uso_b5_func_062880(&import_8005C108, *(char **)(arg0 + 0x40));
    o = *(char **)(arg0 + 0x40);
    timproc_uso_b5_func_07ACE0(reg, o);
    if (*(int *)(o + 0x14) != 0) {
        *(int *)(o + 0x4) = 1;
    }
    *(char **)(o + 0x14) = arg0;
}
