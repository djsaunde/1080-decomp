/* Native original-compiler source port from src/bootup_uso/bootup_uso_pool_D900.c. Target bytes unchanged. */
#include "context_bootup_uso_pool_D900.h"
void func_0000D900(BootupState *st, BootupD900Message *m) {
    BootupD900Word name;
    int opcode;
    s32 *desc;
    int pad_first[2];
    float clamped;
    Vec3 impulse;
    float magnitude;
    float y, z;
    Vec3 stage;
    Vec3 scaled;
    int pad_low[7];
    Vec3 delta;
    BootupD900Payload *payload;
    char *found;
    opcode = m->field_00;
    if (opcode == 20) goto event20;
    if (opcode == 9) goto event9;
    if (opcode == 6) goto event6;
    if (opcode == 1000) goto event1000;
    if (opcode == 1001) goto event1001;
    if (opcode == 1003) goto event1003;
    if (opcode == 1002) goto event1002;
    if (opcode == 1004) goto event1004;
    if (opcode == 1005) goto event1005;
    if (opcode == 1006) goto event1005;
    if (opcode != 1007) { goto other; } else { goto event1005; }
event20:
    if (1) {
        if (st->field_A80 != 0) return;
        payload = m->field_04;
        magnitude = payload->field_0C;
        if ((double)magnitude < D_00000988 && payload->field_28->field_0C == 0x74) {
            bu_d900_a68((int *)st->field_808);
            return;
        }
        clamped = magnitude;
        if (magnitude < 0.0f) magnitude = 0.0f;
        else {
            magnitude = clamped;
            if (clamped > 50.0f) magnitude = 50.0f;
            else magnitude = clamped;
        }
        if (1) { clamped = magnitude; }
        y = clamped;
        clamped = payload->field_00.x * clamped;
        magnitude = payload->field_00.y * magnitude;
        z = payload->field_00.z * y;
        scaled.x = clamped; scaled.y = magnitude; scaled.z = z;
        stage = scaled;
        impulse = stage;
        st->field_B4 += impulse.x;
        st->field_B8 += impulse.y;
        st->field_BC += impulse.z;
        st->field_318.x += impulse.x;
        st->field_318.y += impulse.y;
        st->field_318.z += impulse.z;
        clamped = st->field_3BC.x * impulse.x + st->field_3BC.y * impulse.y + st->field_3BC.z * impulse.z;
        st->field_A1C = (float)((double)st->field_A1C +
            (double)clamped * 0.2);
        clamped = st->field_3C8.x * impulse.x + st->field_3C8.y * impulse.y + st->field_3C8.z * impulse.z;
        st->field_A24 = (float)((double)st->field_A24 +
            (double)-clamped * 0.5);
        st->field_A10 = (u16)st->field_A10 | 0x1F0;
        st->field_9A4 = payload->field_28->field_0C;
        st->field_A80 = 10;
        return;
    }
event9:
    if (1) { bu_d900_cae8(st); return; }
event6:
    if (1) {
        delta = m->field_04->field_E0;
        delta.x *= 4.0f;
        delta.y *= 4.0f;
        delta.z *= 4.0f;
        st->field_B4 += delta.x;
        st->field_B8 += delta.y;
        st->field_BC += delta.z;
        st->field_318.z = (float)0;
        st->field_318.y = (float)0;
        st->field_318.x = (float)0;
        return;
    }
event1000:
    if (1) {
        st->field_A54 = 1;
        st->field_A58 |= 4;
        bu_d900_game6e88(st->field_804);
        bu_d900_gl34810(bu_d900_desc8aec + 0x4C);
        return;
    }
event1001:
    if (1) {
        st->field_A54 = 2;
        st->field_A58 |= 4;
        bu_d900_game6ecc(st->field_804);
        bu_d900_gl34810(bu_d900_desc8aec + 0x54);
        return;
    }
event1003:
    if (1) {
        st->field_A54 = 3;
        st->field_A58 |= 4;
        bu_d900_gl34810(bu_d900_desc8b44 + 4);
        bu_d900_game6f28(st->field_804);
        return;
    }
event1002:
    if (1) { st->field_A54 = 4; return; }
event1004:
    if (1) { st->field_A54 = 5; return; }
event1005:
    if (1) {
        st->field_A54 = 6;
        bu_d900_gl34810(bu_d900_desc8b44 + 0x10);
        if (st->field_848 == 2) {
            desc = (s32 *)(bu_d900_desc8b44 + 0x24);
            bu_d900_game11258(st->field_840);
        } else {
            desc = (s32 *)(bu_d900_desc8b44 + 0x28);
            bu_d900_game112e0(st->field_840);
        }
        name.value = *desc;
        found = bu_d900_gl3eb3c(bu_d900_global, name, 0);
        if (found != 0) {
            bu_d900_gl65360(st, found + 0x30);
            return;
        }
        bu_d900_gl61478(bu_d900_desc8b44 + 0x2C, desc);
        return;
    }
other:
    if (1) { bu_d900_gl65c54(st, m); }
}
