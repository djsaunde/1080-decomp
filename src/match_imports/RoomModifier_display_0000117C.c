/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void RoomModifier_display_0000117C(TimB5SlotController *a0) {
    TimB5Screen *slot;
    s32 *flags;
    TimB5SlotController *b;
    b = a0;
    SelectWin_setGlobalFade_0000896C(a0->field_38[a0->field_34]);
    SelectWin_dimOtherItems_00008AFC(b->field_38[b->field_34]);
    slot = b->field_38[b->field_34];
    /* slot is a SelectWin (game/select.h): field_40C = itemsMgr, field_41C = owner (SelectionMgr) */
    if ((1 << b->field_34) & *(s32 *)&((SelectionMgr *)slot->field_41C)->shownMask) {
        if (b->field_2C != 0) {
            flags = (s32 *)&((Node *)((SelItemsMgr *)slot->field_40C)->dispItems)->doFlags; /* DispItems doFlags: bit 4 = DoDisplay */
            do {
                do {
                    *flags = *flags | 4;
                } while (0);
            } while (0);
            slot = b->field_38[b->field_34 ^ 1];
            if (slot != 0) {
                flags = (s32 *)&((Node *)((SelItemsMgr *)slot->field_40C)->dispItems)->doFlags; /* DispItems doFlags: bit 4 = DoDisplay */
                do {
                    do {
                        *flags = *flags & ~4;
                    } while (0);
                } while (0);
            }
        } else {
            flags = (s32 *)&((Node *)((SelItemsMgr *)slot->field_40C)->dispItems)->doFlags; /* DispItems doFlags: bit 4 = DoDisplay */
            do {
                do {
                    *flags = *flags & ~4;
                } while (0);
            } while (0);
            slot = b->field_38[b->field_34 ^ 1];
            if (slot != 0) {
                flags = (s32 *)&((Node *)((SelItemsMgr *)slot->field_40C)->dispItems)->doFlags; /* DispItems doFlags: bit 4 = DoDisplay */
                do {
                    do {
                        *flags = *flags & ~4;
                    } while (0);
                } while (0);
            }
        }
        gen_imp_Node_display(b);
    } else {
        flags = (s32 *)&((Node *)((SelItemsMgr *)slot->field_40C)->dispItems)->doFlags; /* DispItems doFlags: bit 4 = DoDisplay */
        do {
            do {
                *flags = *flags | 4;
            } while (0);
        } while (0);
        slot = b->field_38[b->field_34 ^ 1];
        if (slot != 0) {
            flags = (s32 *)&((Node *)((SelItemsMgr *)slot->field_40C)->dispItems)->doFlags; /* DispItems doFlags: bit 4 = DoDisplay */
            do {
                do {
                    *flags = *flags & ~4;
                } while (0);
            } while (0);
        }
    }
    b->field_34 = b->field_34 + 1;
}
