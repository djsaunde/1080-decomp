/* Native original-compiler source port from src/bootup_uso/bootup_uso_tail4.c. Target bytes unchanged. */
#include "context_bootup_uso_tail4.h"
BootupGrid139B0 *func_00012E00(BootupGrid139B0 *arg0, s32 arg1) { void *saved_context; s32 unused[6]; BootupGrid139B0 *sub; BootupGrid139B0 *o = arg0;
    s32 row, col, y, x, cy, off, x2, cy2, off2;
    f32 dx, dy, divisor;
    f32 half_x, half_y;
    if (arg0 == ((void*)0)) {
        o = boot12E00_alloc(0x78);
        if (o == ((void*)0)) goto done;
    }
    sub = o;
    if (o == ((void*)0)) {
        sub = boot12E00_alloc(0x2C);
        if (sub == ((void*)0)) goto fields;
    }
    boot12E00_base_init(sub, &D_0000C730);
    sub->unk28 = &boot12E00_desc_base;
fields:
    o->unk28 = &boot12E00_desc_self;
    o->unk0C = &D_0000C738;
    o->unk2C = arg1;
    o->columns = 0x20;
    o->rows = 0x20;
    o->cellCount = 0x400;
    o->accumulator = 1.0f;
    saved_context = boot12E00_exchange_context(((void*)0));
    o->unk4C = boot12E00_alloc_aligned(0x800, 0x10);
    o->unk50 = boot12E00_alloc_aligned(0x400, 0x10);
    o->grid = boot12E00_alloc(0x1000);
    o->unk54 = boot12E00_alloc(0x1000);
    boot12E00_exchange_context(saved_context);
    o->unk3C = 1.0f;
    o->unk38 = 1.0f;
    o->unk34 = 1.0f;
    o->unk30 = 1.0f;
    dx = 10.0f;
    dy = 7.5f;
    divisor = (f32)2;
    half_y = dy / divisor;
    half_x = dx / divisor;
    for (row = 0, off = 0; row != 32;) {
        for (col = 0; col != 32; col += 2) {
            y = (s32)((f32)row * 7.5f + half_y);
            x = (s32)((f32)col * 10.0f + half_x);
            cy = y;
            if (x >= 320) x = 319;
            if (y >= 240) cy = 239;
            *(s32 *)((char *)o->unk54 + off) = cy * 320 + x;
            off += 4;
            y = (s32)((f32)row * 7.5f + half_y);
            x = (s32)((f32)(col + 1) * 10.0f + half_x);
            cy = y;
            if (x >= 320) x = 319;
            if (y >= 240) cy = 239;
            *(s32 *)((char *)o->unk54 + off) = cy * 320 + x;
            off += 4;
        }
        row++;
    }
done:
    return o;
}
