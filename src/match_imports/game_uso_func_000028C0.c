/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
void game_uso_func_000028C0(GameUsoHomingPrefix *a0) {
    GameUsoVectorOutput *out_home[1];
    GameUsoVectorOutput *s_home[1];
    GameUsoVectorOutput *pose;
    Vec3 dcopy;
    Vec3 refv1;
    union { Vec3 value; volatile Vec3 reload; } v2;
    Vec3 dc2;
    float mag[1];
    GameUsoVectorOutput *mode_out, *mode_ref; s32 pad1[2];
    float scratch[3];
    s32 pad2[2];
    Vec3 diff;
    s32 pad3[4];
    Vec3 axis;
    float refv2[3];
    Vec3 ssv;
    union { Vec3 value; volatile Vec3 reload; } axisc;
    float mag2;
    int mode;
    float f, ax, ay, az;
    s32 unused_self_home;
    do { out_home[0] = a0->field_14; } while (0);
    do { s_home[0] = ((GameUsoRotationSource *)a0->field_3C)->field_38; } while (0);
    do { mode = a0->field_40; } while (0);
    do { if (mode == 0) { return; } } while (0);
    if (mode == 1) {
        mode_out = out_home[0];
        mode_ref = s_home[0];
        refv1.x = mode_out->field_A0.x;
        refv1.y = mode_out->field_A0.y;
        refv1.z = mode_out->field_A0.z;
        v2.value.x = mode_ref->field_A0.x;
        v2.value.y = mode_ref->field_A0.y;
        v2.value.z = mode_ref->field_A0.z;
        dcopy = *game_uso_28C0_rotate((Vec3 *)scratch, (GameUsoRotationSource *)a0->field_3C);
        if (a0) { }
        v2.value.x = v2.value.x + dcopy.x;
        v2.value.y = v2.value.y + dcopy.y;
        v2.value.z = v2.value.z + dcopy.z;
        do {
        diff.x = (f = v2.reload.x - refv1.x),
        diff.y = (ax = v2.reload.y - refv1.y),
        diff.z = (ay = v2.reload.z - refv1.z);
        } while (0);
        *(Vec3 *)scratch = diff;
        dc2 = *(Vec3 *)scratch;
        mag[0] = game_uso_func_070238(ay = dc2.x * dc2.x + dc2.y * dc2.y + dc2.z * dc2.z);
        game_uso_28C0_normalize(&dc2);
        f = mag[0] * a0->field_5C;
        dc2.x = dc2.x * f;
        dc2.y = dc2.y * f;
        dc2.z = dc2.z * f;
        pose = out_home[0];
        game_uso_func_072EE8((char *)pose + 0x30, &dc2);
        out_home[0]->field_A0.x = out_home[0]->field_60.x;
        out_home[0]->field_A0.y = out_home[0]->field_60.y;
        out_home[0]->field_A0.z = out_home[0]->field_60.z;
    } else {
        char *ss;
        pose = out_home[0];
        refv2[0] = pose->field_A0.x;
        refv2[1] = pose->field_A0.y;
        refv2[2] = pose->field_A0.z;
        ss = (char *)a0->field_38;
        ssv.x = *(float *)(ss + 0xA0);
        ss += 0x70;
        ssv.y = *(float *)(ss + 0x34);
        ssv.z = *(float *)(ss + 0x38);
        if (a0->field_40 == 3) {
            f = -1000.0f;
        } else {
            f = 1000.0f;
        }
        do {
        mag2 = 0.0f;
        axis.x = mag2;
        axis.y = mag2;
        axis.z = f;
        } while (0);
        axisc.value = axis;
        pose = s_home[0];
        f = pose->field_70[2].field_00[0] * axisc.value.z + (pose->field_70[1].field_00[0] * axisc.value.y + pose->field_70[0].field_00[0] * axisc.value.x);
        ax = pose->field_70[2].field_00[1] * axisc.value.z + (pose->field_70[1].field_00[1] * axisc.value.y + pose->field_70[0].field_00[1] * axisc.value.x);
        axisc.value.z = (az = pose->field_70[2].field_00[2] * axisc.value.z + (pose->field_70[1].field_00[2] * axisc.value.y + pose->field_70[0].field_00[2] * axisc.value.x));
        axisc.value.x = f; axisc.value.y = ax;
        ssv.x = ssv.x + axisc.reload.x;
        ssv.y = ssv.y + axisc.reload.y;
        ssv.z = ssv.z + axisc.reload.z;
        ssv.x = ssv.x - refv2[0];
        ssv.y = ssv.y - refv2[1];
        ssv.z = ssv.z - refv2[2];
        mag2 = game_uso_func_070238(ssv.x * ssv.x + ssv.y * ssv.y + ssv.z * ssv.z);
        game_uso_28C0_normalize(&ssv);
        f = (0, mag2) * a0->field_5C;
        ssv.x = ssv.x * f;
        ssv.y = ssv.y * f;
        ssv.z = ssv.z * f;
        pose = out_home[0];
        game_uso_func_072EE8((char *)pose + 0x30, &ssv);
        out_home[0]->field_A0.x = out_home[0]->field_60.x;
        out_home[0]->field_A0.y = out_home[0]->field_60.y;
        out_home[0]->field_A0.z = out_home[0]->field_60.z;
    }
}
