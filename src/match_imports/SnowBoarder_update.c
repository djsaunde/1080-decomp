/* Native original-compiler source port from src/bootup_uso/bootup_uso.c. Target bytes unchanged. */
#include "context_bootup_uso.h"
#include "gdl/node.h"
#include "gdl/pad.h"

/* st->field_800 is the rider's PadCtl (held / pressed / injectPending); the two indirect calls
 * go through Cfront vtable slots (VT_SLOT): slot 4 of the rider's own vtable (message 9) and
 * slot 3 of the field_804 / field_840 nodes. */
void SnowBoarder_update(BootupState *st) {
    char *a0_2, *vt;
    int fl, v0, t5;
    int sp58;
    float f0, f2;
    bootup_D440_062380(bootup_D440_desc0 + 0x54);
    v0 = st->field_A58 & 0x100;
    if (v0 != 0) {
        vt = (char *)st->field_800;
        if (PADCTL(vt)->pressed & 0x20) {
            st->field_A28 ^= 1;
            vt = (char *)*(BootupInput *volatile *)&st->field_800;
        }
        if (st->field_A28 != 0) {
            if (*(volatile int *)&PADCTL(vt)->pressed & 0x10) {
                v0 = st->field_A58 & 0x100;
                goto block_9;
            }
        } else {
            if (*(volatile int *)&PADCTL(vt)->pressed & 0x10) {
                st->field_A58 ^= 4;
            }
            v0 = st->field_A58 & 0x100;
            goto block_9;
        }
    } else {
    block_9:
        if (v0 != 0) {
            a0_2 = (char *)st->field_800;
            if ((!!(*(volatile int *)&PADCTL(a0_2)->held & 0x80)) & ((*(int *)&PADCTL(a0_2)->held & 0x40) != 0) /* member form swaps the and operands */) {
                sp58 = 9;
                vt = (char *)st->field_28;
                a0_2 = (char *)(VT_SLOT(vt, 4).delta + (int)st);
                ((void (*)(char *, int *))VT_SLOT(vt, 4).fn)(a0_2, &sp58);
            }
        }
        if (st->field_A58 & 4) {
            vt = (char *)st->field_800;
            PADCTL(vt)->injectPending = 1;
            bootup_D440_062380(bootup_D440_desc1 + 0x5C);
            fl = (int)st->field_804;
            vt = NODE(fl)->vptr;
            ((void (*)(char *))VT_SLOT(vt, 3).fn)((char *)(fl + VT_SLOT(vt, 3).delta));
            bootup_D440_062388(bootup_D440_desc2 + 0x64);
        }
        a0_2 = st->field_808;
        if (st->field_A58 & 0x80) {
            st->field_A1C = -st->field_A1C;
        }
        bootup_D440_000A94((BootupCodeContextPrefix *)a0_2, st->field_9A4);
        st->field_9A4 = 0;
        fl = (int)st->field_840;
        vt = NODE(fl)->vptr;
        ((void (*)(char *))VT_SLOT(vt, 3).fn)((char *)(fl + VT_SLOT(vt, 3).delta));
        st->field_A10 = 0;
        st->field_A1C = 0.0f;
        st->field_A20 = 0.0f;
        st->field_A24 = 0.0f;
        if (st->field_A58 & 0x800) {
            bootup_D440_062380(bootup_D440_desc3 + 0x6C);
            bootup_D440_00BF8C(st);
            bootup_D440_062388(bootup_D440_desc4 + 0x8);
            if (st->field_A58 & 0x40000) {
                f0 = st->field_318.y;
                f2 = (f0 < 0.0f) ? -f0 : f0;
                v0 = (int)((f2 - 100.0f) / (float)4);
                st->field_A84 = v0;
                if (v0 < 0) {
                    st->field_A84 = 0;
                }
                bootup_D440_000A50(a0_2, st->field_A84);
                if (st->field_8B8 & 1) {
                    st->field_81C->field_94 = st->field_A84;
                }
                if (st->field_8B8 & 4) {
                    st->field_818->field_94 = st->field_A84;
                }
            }
        }
        bootup_D440_062380(bootup_D440_desc5 + 0x14);
        st->field_A38 = st->field_3C8.x * st->field_318.x
                               + st->field_3C8.y * st->field_318.y
                               + st->field_3C8.z * st->field_318.z;
        bootup_D440_0779E8(st->field_840);
        if (st->field_A5C != 0) {
            f2 = (float)(((double)bootup_D440_075E90(((((int)st->field_DC.x ^ (int)st->field_DC.z) >> 5) ^ 0x12345678)) - 0.5) * 30.0 * (double)((float)st->field_A5C / 100.0f));
            vt = (char *)st->field_824;
            vt += 0x64;
            *(float *)vt += f2;
            vt = (char *)st->field_828;
            vt += 0x64;
            *(float *)vt += f2;
            vt = (char *)st->field_830;
            vt += 0x64;
            *(float *)vt += f2;
            vt = (char *)st->field_834;
            vt += 0x64;
            *(float *)vt = (float)((double)*(float *)vt + (double)f2 * 0.5);
            vt = (char *)st->field_82C;
            vt += 0x64;
            *(float *)vt = (float)((double)*(float *)vt + (double)f2 * 0.5);
            vt = (char *)st->field_838;
            vt += 0x64;
            *(float *)vt = (float)((double)*(float *)vt - (double)f2 * 0.5);
        }
        bootup_D440_062388(bootup_D440_desc6 + 0x20);
        bootup_D440_062380(bootup_D440_desc7 + 0x2C);
        bootup_D440_054FE0(st);
        bootup_D440_062388(bootup_D440_desc8 + 0x38);
        bootup_D440_02258C(bootup_D440_root0[0x48 / 4], st, st->field_348 / (float)2);
        bootup_D440_0225F8(bootup_D440_root1[0x48 / 4], st, st->field_DC.x, st->field_DC.z);
        bootup_D440_0225B8(bootup_D440_root2[0x48 / 4], st, st->field_A14);
        fl = (int)st->field_8DC;
        if (fl != 0) {
            bootup_D440_000610(a0_2, &st->field_DC, st->field_348,
                          -st->field_968.x, st->field_968.z,
                          st->field_9D0 + D_00000980,
                          (int)st->field_9A2, (char *)fl, st->field_A58);
        }
        bootup_D440_062388(bootup_D440_desc9 + 0x44);
    }
}
