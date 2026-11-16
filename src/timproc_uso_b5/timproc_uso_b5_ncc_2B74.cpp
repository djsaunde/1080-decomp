/* IDO 7.1 NCC (C++) donor for SelItemParms_ctor (0x754 / 469 words,
 * composite panel constructor: 12 text / value children + two vec3 children).
 * Spliced into timproc_uso_b5.c.o via REPLACE_FUNC_BODY. The vec3 children's
 * FP constants are source literals (-1500.0f / 1500.0f); the donor's .rodata
 * pool is renamed SelItemParms_ctor_rodata and pinned to RoData
 * +0x11C (undefined_syms_auto.txt). */
typedef int s32;
typedef float f32;
#include "../../include/game/select.h"  /* NCC donors build without -I include */
#define SELF ((SelItemParms *)arg0)
#define PI(p) ((SelParmI *)(p))
#define PF(p) ((SelParmF *)(p))
#define PV(p) ((SelParmV3 *)(p))
struct TimB5V3 { f32 x, y, z; };
struct TimB5Nm { char *v; };
#define B5_NAME(sym, lo) (*(char **)((char *)&sym + (lo)))
extern "C" char D_timb5_2B74_vt0[];
extern "C" char D_timb5_2B74_vt1[];
extern "C" char D_timb5_2B74_vt2[];
extern "C" char D_timb5_2B74_vt3[];
extern "C" char D_timb5_2B74_vt4[];
extern "C" char D_timb5_2B74_vt5[];
extern "C" char D_timb5_2B74_vt6[];
extern "C" {
extern int gen_imp_mem_new(...);
extern void GenSelectChar_initLabels_00000778(char *arg0);
extern void *gen_imp_BaseLight_ctor(int, char *);
extern char timproc_uso_b5_D_807FE9A0;
extern char timproc_uso_b5_D_807FE9B0;
extern char timproc_uso_b5_D_807FE9B4;
extern char timproc_uso_b5_D_807FE9B8;
extern char timproc_uso_b5_D_807FE9BC;
extern char timproc_uso_b5_D_807FE9C0;
extern char timproc_uso_b5_D_807FE9C4;
extern char timproc_uso_b5_D_807FE9C8;
extern char timproc_uso_b5_D_807FE9CC;
extern char timproc_uso_b5_D_807FE9D0;
extern char timproc_uso_b5_D_807FE9D4;
extern char timproc_uso_b5_D_807FE9D8;
extern char timproc_uso_b5_D_807FE9DC;
extern char timproc_uso_b5_D_807FE9E0;
extern char timproc_uso_b5_D_807FE9E4;
extern int gen_imp_Parms_addParm(...);
extern char D_timb5_2B74_vt7, D_timb5_2B74_vt8, D_timb5_2B74_vt9;
extern char D_timb5_2B74_vv0, D_timb5_2B74_vv1;
extern char D_timb5_2B74_vw0, D_timb5_2B74_vw1;
}
extern "C" char *SelItemParms_ctor(char *arg0) {
    struct TimB5Nm u0;
    struct TimB5Nm u1;
    struct TimB5Nm u2;
    struct TimB5Nm u3;
    struct TimB5Nm u4;
    struct TimB5Nm u5;
    struct TimB5Nm u6;
    struct TimB5Nm u7;
    struct TimB5Nm u8;
    struct TimB5Nm u9;
    struct TimB5Nm u10;
    struct TimB5Nm u11;
    struct TimB5V3 vC;
    struct TimB5V3 vA;
    struct TimB5V3 vB;
    struct TimB5Nm u12;
    struct TimB5V3 vC2;
    struct TimB5V3 vD;
    struct TimB5V3 vE;
    struct TimB5Nm u13;
    volatile s32 padA1;
    volatile s32 padA2;
    volatile s32 padA3;
    char *c;
    struct TimB5Nm nm;
    volatile s32 padB0;
    volatile s32 padB1;
    volatile s32 padB2;
    volatile s32 padB3;
    volatile s32 padB4;
    volatile s32 padB5;
    volatile s32 padB6;
    char *cA;
    volatile s32 padC0;
    struct TimB5V3 cp0;
    volatile s32 padD0;
    struct TimB5V3 cp1;
    volatile s32 padE0;
    struct TimB5V3 cp2;
    volatile s32 padF0;
    volatile s32 padF1;
    volatile s32 padF2;
    struct TimB5V3 t;
    volatile s32 padG0;
    volatile s32 padG1;
    volatile s32 padG2;
    f32 h;
    char *cB;
    char *vt;
    f32 f;
    f32 g;

    if (1) { struct TimB5Nm *q = &u0; q = &u1; q = &u2; q = &u3; q = &u4; q = &u5; q = &u6; q = &u7; q = &u8; q = &u9; q = &u10; q = &u11; q = &u12; q = &u13; }

    if (arg0 != 0 || (arg0 = (char *)gen_imp_mem_new(0x168)) != 0) {
        vt = arg0;
        if (vt != 0 || (vt = (char *)gen_imp_mem_new(8)) != 0) {
            ((SelItemParms *)vt)->vptr = (int)((char *)&timproc_uso_b5_D_807FE9A0 + 0x1130);
            ((SelItemParms *)vt)->unk004 = 0;
        }
        u0.v = B5_NAME(timproc_uso_b5_D_807FE9B0, 0x1140); nm = u0;
        {
            if ((c = (char *)&SELF->xpn) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt0 + 0x1A0), PI(c)->value = 0x52, PI(c)->unk14 = 0);
            }
        }
        u1.v = B5_NAME(timproc_uso_b5_D_807FE9B4, 0x1144); nm = u1;
        {
            if ((c = (char *)&SELF->ypn) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt1 + 0x1A0), PI(c)->value = 0x12, PI(c)->unk14 = 0);
            }
        }
        u2.v = B5_NAME(timproc_uso_b5_D_807FE9B8, 0x1148); nm = u2;
        {
            if ((c = (char *)&SELF->jnx) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt2 + 0x1A0), PI(c)->value = 0x52, PI(c)->unk14 = 0);
            }
        }
        u3.v = B5_NAME(timproc_uso_b5_D_807FE9BC, 0x114C); nm = u3;
        {
            if ((c = (char *)&SELF->jny) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt3 + 0x1A0), PI(c)->value = 0x26, PI(c)->unk14 = 0);
            }
        }
        u4.v = B5_NAME(timproc_uso_b5_D_807FE9C0, 0x1150); nm = u4;
        {
            if ((c = (char *)&SELF->xp0) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt4 + 0x1A0), PI(c)->value = 0x52, PI(c)->unk14 = 0);
            }
        }
        u5.v = B5_NAME(timproc_uso_b5_D_807FE9C4, 0x1154); nm = u5;
        {
            if ((c = (char *)&SELF->yp0) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt5 + 0x1A0), PI(c)->value = 0x26, PI(c)->unk14 = 0);
            }
        }
        u6.v = B5_NAME(timproc_uso_b5_D_807FE9C8, 0x1158); nm = u6;
        {
            if ((c = (char *)&SELF->xfp) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt6 + 0x1A0), PI(c)->value = 0x52, PI(c)->unk14 = 0);
            }
        }
        u7.v = B5_NAME(timproc_uso_b5_D_807FE9CC, 0x115C); nm = u7;
        {
            if ((c = (char *)&SELF->yfp) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt7 + 0x1A0), PI(c)->value = 0x12, PI(c)->unk14 = 0);
            }
        }
        u8.v = B5_NAME(timproc_uso_b5_D_807FE9D0, 0x1160); nm = u8;
        {
            if ((c = (char *)&SELF->xfc) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt8 + 0x1A0), PI(c)->value = 0x52, PI(c)->unk14 = 0);
            }
        }
        u9.v = B5_NAME(timproc_uso_b5_D_807FE9D4, 0x1164); nm = u9;
        {
            if ((c = (char *)&SELF->yfc) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                (PI(c)->vptr = (int)((char *)&D_timb5_2B74_vt9 + 0x1A0), PI(c)->value = 0x12, PI(c)->unk14 = 0);
            }
        }
        u10.v = B5_NAME(timproc_uso_b5_D_807FE9D8, 0x1168); nm = u10;
        {
            if ((c = (char *)&SELF->cfv) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                PF(c)->vptr = (int)((char *)&D_timb5_2B74_vv0 + 0x158);
                PF(c)->unk14 = 0;
                PF(c)->value = -20.0f;
            }
        }
        u11 = *(struct TimB5Nm *)((char *)&timproc_uso_b5_D_807FE9DC + 0x116C); nm.v = u11.v;
        {
            if ((c = (char *)&SELF->cf2) != 0 || (c = (char *)gen_imp_mem_new(0x18)) != 0) {
                gen_imp_Parms_addParm(c, arg0, nm, 1);
                PF(c)->vptr = (int)((char *)&D_timb5_2B74_vv1 + 0x158);
                PF(c)->unk14 = 0;
                PF(c)->value = -20.0f;
            }
        }
        h = 0; vC.x = h; vC.y = h; vC.z = h;
        u12 = *(struct TimB5Nm *)((char *)&timproc_uso_b5_D_807FE9E0 + 0x1170);
        f = -1500.0f; vA.x = f; vA.y = f; vA.z = f;
        g = 1500.0f; vB.x = g; vB.y = g; vB.z = g;
        cp0 = vC; cp1 = vA; cp2 = vB;
        nm.v = u12.v;
        {
            if ((cA = (char *)&SELF->off) != 0 ||
                ((cA = (char *)gen_imp_mem_new(0x20)),
                 (f = -1500.0f),
                 (g = 1500.0f), (h = 0), (cA != 0))) {
                gen_imp_Parms_addParm(cA, arg0, nm, 1);
                f = -1500.0f;
                g = 1500.0f;
                h = 0;
                PV(cA)->vptr = (int)((char *)&D_timb5_2B74_vw0 + 0x170);
                PV(cA)->unk1C = 0;
                t = cp0;
                PV(cA)->v[0] = t.x;
                PV(cA)->v[1] = t.y;
                PV(cA)->v[2] = t.z;
            }
        }
        u13 = *(struct TimB5Nm *)((char *)&timproc_uso_b5_D_807FE9E4 + 0x1174);
        vC2.x = h; vC2.y = h; vC2.z = h;
        vD.x = f; vD.y = f; vD.z = f;
        vE.x = g; vE.y = g; vE.z = g;
        cp0 = vC2;
        cp1 = vD;
        cp2 = vE;
        nm.v = u13.v;
        {
            if ((cB = (char *)&SELF->of2) != 0 || (cB = (char *)gen_imp_mem_new(0x20)) != 0) {
                gen_imp_Parms_addParm(cB, arg0, nm, 1);
                PV(cB)->vptr = (int)((char *)&D_timb5_2B74_vw1 + 0x170);
                PV(cB)->unk1C = 0;
                t = cp0;
                PV(cB)->v[0] = t.x;
                PV(cB)->v[1] = t.y;
                PV(cB)->v[2] = t.z;
            }
        }
    }
    return arg0;
}
