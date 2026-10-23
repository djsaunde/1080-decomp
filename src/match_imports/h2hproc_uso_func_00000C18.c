/* Native original-compiler source port from src/h2hproc_uso/h2hproc_uso.c. Target bytes unchanged. */
#include "context_h2hproc_uso.h"
void h2hproc_uso_func_00000C18(H2H_C18Context *a0) {
    int unused0;
    int *made0, *made1;
    int idx;
    H2HLinkNode *created;
    H2H_C18Flags *binding;
    int *allocation;
    H2H_C18Slot *slot;
    void *node;
    int *flags;
    int bits;
    idx = a0->field_6B8;
    if (idx == -1) slot = ((H2H_C18Global*)&D_00000000)->field_134->field_C4[0].field_00;
    else slot = *(H2H_C18Slot**)((char*)((H2H_C18Global*)&D_00000000)->field_134 + 0xC4 + idx * 8);
    flags = &((H2H_C18Global*)&D_00000000)->field_134->field_C4[0].field_00->field_A58;
    *flags |= 0x8000;
    flags = (int*)((H2H_C18Global*)&D_00000000)->field_134->field_C4[1].field_00;
    flags = (int*)((char*)flags + 0xA58);
    *flags |= 0x8000;
    made0 = h2hproc_C18_make0((int*)((H2H_C18Global*)&D_00000000)->field_134, 0, ((H2H_C18Global*)&D_00000000)->field_134->field_80, (int*)slot, slot->field_800);
    made1 = h2hproc_C18_make1((int*)((H2H_C18Global*)&D_00000000)->field_134, 1, ((H2H_C18Global*)&D_00000000)->field_134->field_80, (int*)slot, slot->field_800);
    a0->field_520 = ((H2H_C18Global*)&D_00000000)->field_134->field_108;
    flags = &a0->field_520->field_18;
    *flags &= ~8;
    flags = (int*)a0->field_520;
    flags = (int*)((char*)flags + 0x18);
    *flags &= ~4;
    if (1) {}
    flags = (int*)((H2H_C18Global*)&D_00000000)->field_134->field_10C;
    {
        int *p;
        int t;
        p = flags;
        if (1) { p += 6; }
        *p &= ~8;
        *p &= ~4;
    }
    if (1) {}
    binding = (H2H_C18Flags*)h2hproc_C18_bind((int*)((H2H_C18Global*)&D_00000000)->field_134, &h2hproc_C18_data_Sym140 + 0x1C, made0);
    a0->field_51C = binding;
    ((H2H_C18Global*)&D_00000000)->field_134->field_108 = binding;
    allocation = h2hproc_C18_alloc(0x80);
    if (allocation) h2hproc_C18_init(allocation, 1);
    created = (H2HLinkNode*)h2hproc_C18_create(0, allocation, (int*)slot, (int*)a0->field_51C, made0, made1);
    node = ((H2H_C18Global*)&D_00000000)->field_134->field_84;
    h2hproc_C18_link((int)node + 0x10, (int*)created, (int)node);
    if (created->field_14) created->field_04 = 1;
    created->field_14 = node;
    h2hproc_C18_finish((int*)a0->field_51C, (int*)a0);
    a0->field_4F0 &= 0xFEFFFFFF;
}
