#include "common.h"

extern s32 func_8000898C(s32, s32, s32);
extern void func_800084D0(s32, s32);
extern s32 func_80008BB4(s32);

/* __rmonStepBreak */
s32 __rmonSetSingleStep(s32 arg0, s32* arg1) {
    s32 result = func_8000898C(0, arg0, (s32)arg1);
    if (result & 3) {
        func_800084D0((s32)arg1 + 4, 0);
    } else {
        if ((s32)arg1 == result) {
            return 0;
        }
        if (func_80008BB4(*arg1) != 0 || (s32)arg1 + 8 == result) {
            func_800084D0(result, 0);
        } else {
            func_800084D0(result, (s32)arg1 + 8);
        }
    }
    return 1;
}

/* Initializes an rmon header struct on entry to a step/break sequence.
 * Pure sequence of stores — no branches, no calls. */
typedef struct {
    s32 field_00;
    s8  field_04;
    char pad05;
    s16 field_06;
    char pad08[4];
    s32 field_0C;
    s16 field_10;
    s16 field_12;
    char pad14[0x10];
    s16 field_24;
    s16 field_26;
    char pad28[8];
    s32 field_30;
} RmonInit;

void __rmonGetExceptionStatus(RmonInit* p) {
    p->field_0C = 1;
    p->field_10 = 2;
    p->field_12 = 0;
    p->field_30 = 0;
    p->field_24 = 2;
    p->field_26 = 4;
    p->field_04 = 4;
    p->field_06 = 0;
    p->field_00 = 0x4C;
}
