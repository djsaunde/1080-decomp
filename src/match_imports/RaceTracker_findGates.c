/* Native original-compiler source port from src/game_libs/game_libs_tail.c. Target bytes unchanged. */
#include "context_game_libs_tail.h"
#include "game/bootup_uso_data.h"
void RaceTracker_findGates(GameLibsC784 *self) {
    char *table;
    struct D0D0_Key key68;
    struct D0D0_Key key64;
    struct D0D0_Key key60;
    f32 base;
    register s32 available;
    s32 offset;
    s32 i;
    D0D0_Node *first;
    D0D0_Node *second;
    GameLibsC784Record *records;
    table = (char *)&D_00000000;
    offset = 0;
    i = 0;
    if (self->count > 0) {
        do {
            ((GameLibsC784Record *)((char *)self->records + offset))->field_0C = -6553600.0f;
            /* the gate-kind parm tags (bootup Data, read as a word; the local table is the codegen lever) */
            key68.field_00 = *(s32 *)(table + kTag_chk_D680);
            self->records->gate = gl_d0d0_import_3EAE0(gl_d0d0_root_7770, key68, (char *)self->records->field_00 + 0xDC, 0);
            key64.field_00 = *(s32 *)(table + kTag_gtl_D684);
            first = gl_d0d0_import_3EAE0(gl_d0d0_root_7770, key64, (char *)self->records->field_00 + 0xDC, 0);
            key60.field_00 = *(s32 *)(table + kTag_gtr_D688);
            second = gl_d0d0_import_3EAE0(gl_d0d0_root_7770, key60, (char *)self->records->field_00 + 0xDC, 0);
            available = second != 0;
            if (available) {}
            if ((first == 0) || (available == 0)) {
                ((GameLibsC784Record *)((char *)self->records + offset))->field_54 = 0;
            } else {
                records = self->records;
                base = ((D0D0_Root *)records->field_00)->field_E4;
                if (first->field_64 - base < second->field_64 - base) {
                    ((GameLibsC784Record *)((char *)records + offset))->field_54 = first;
                    ((GameLibsC784Record *)((char *)self->records + offset))->field_58 = second;
                } else {
                    ((GameLibsC784Record *)((char *)records + offset))->field_54 = second;
                    ((GameLibsC784Record *)((char *)self->records + offset))->field_58 = first;
                }
            }
            i++;
            offset += 0x60;
        } while (i < self->count);
    }
}
