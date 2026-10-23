/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void timproc_uso_b5_func_0000117C(TimB5SlotController *a0) {
    TimB5Screen *slot;
    s32 *flags;
    TimB5SlotController *b;
    b = a0;
    timproc_uso_b5_func_0000896C(a0->field_38[a0->field_34]);
    timproc_uso_b5_func_00008AFC(b->field_38[b->field_34]);
    slot = b->field_38[b->field_34];
    if ((1 << b->field_34) & *(s32 *)(slot->field_41C + 0x50)) {
        if (b->field_2C != 0) {
            flags = (s32 *)(*(char **)(slot->field_40C + 0x38) + 0x18);
            do {
                do {
                    *flags = *flags | 4;
                } while (0);
            } while (0);
            slot = b->field_38[b->field_34 ^ 1];
            if (slot != 0) {
                flags = (s32 *)(*(char **)(slot->field_40C + 0x38) + 0x18);
                do {
                    do {
                        *flags = *flags & ~4;
                    } while (0);
                } while (0);
            }
        } else {
            flags = (s32 *)(*(char **)(slot->field_40C + 0x38) + 0x18);
            do {
                do {
                    *flags = *flags & ~4;
                } while (0);
            } while (0);
            slot = b->field_38[b->field_34 ^ 1];
            if (slot != 0) {
                flags = (s32 *)(*(char **)(slot->field_40C + 0x38) + 0x18);
                do {
                    do {
                        *flags = *flags & ~4;
                    } while (0);
                } while (0);
            }
        }
        timproc_uso_b5_func_04CCA8(b);
    } else {
        flags = (s32 *)(*(char **)(slot->field_40C + 0x38) + 0x18);
        do {
            do {
                *flags = *flags | 4;
            } while (0);
        } while (0);
        slot = b->field_38[b->field_34 ^ 1];
        if (slot != 0) {
            flags = (s32 *)(*(char **)(slot->field_40C + 0x38) + 0x18);
            do {
                do {
                    *flags = *flags & ~4;
                } while (0);
            } while (0);
        }
    }
    b->field_34 = b->field_34 + 1;
}
