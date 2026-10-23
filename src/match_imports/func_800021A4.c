/* Native original-compiler source port from src/kernel/kernel_000.c. Target bytes unchanged. */
#include "context_kernel_000.h"
s32 func_800021A4(UsoSlot *module) {
    UsoRecord *head;
    s32 index;
    UsoRecord *record;
    s32 count;
    s16 flags;
    s32 kind, selector, section_offset;
    head = (UsoRecord*)module->field_38[1];
    if (head == 0) return 0;
    index = 0;
    count = module->field_04;
    record = head;
    if (count <= 0) goto end;
    do {
        flags = record->field_00;
        index++;
        if (flags & 0x8) goto cond;
        kind = flags & 0x7;
        selector = flags >> 4;
        if (2 == kind) {
            section_offset = selector << 2;
            goto store_lookup;
        }
        if (kind == 4) {
            section_offset = selector << 2;
            goto do_store;
        }
        selector = flags | 0x8;
        if (kind != 3) goto cond;
        goto set_flag;
    store_lookup:
    do_store: {
            UsoSlot *section = (UsoSlot*)((char*)module + section_offset);
            record->field_04 += section->field_38[0];
            record->field_00 = flags | 8;
            count = module->field_04;
            goto cond;
        }
    set_flag:
        record->field_00 = (short)selector;
        count = module->field_04;
    cond:
        record = record + 1;
    } while (index < count);
end:
    return 0;
}
