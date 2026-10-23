/* IDO 7.1 -O2 donor for gl_func_00026D64 -- sound-command queue drain
 * (230/230 words; st_size 0x398).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post.c.o (the
 * 29CCC / 273B8 / 31334 recipe): the switch (code) jumptable lands in this
 * unit's local .rodata at +0; the splice renames the reloc to
 * gl_func_00026D64_rodata, pinned to 0xF50, so the `lw t2,0xF50(at)` %lo links
 * as the USO ships it (an in-unit compile cannot bake the %lo).
 *
 * Load-bearing shapes (run 132, agent-c: 43 raw -> exact):
 *  - NO mask on the CUR_D increment; the two dead u8 masks go in the flags
 *    read-modify-writes instead (`obj->flags = (obj->flags & 0xFF) | 4` in
 *    case 0x41 and `| 1` in case 0x4C): each burns one ugen t-register at the
 *    point where the target's temps run one ahead.
 *  - case 0x41 compares against a raw `*(f32 *)((u8 *)obj + 0x2C)` read: the
 *    struct-field spelling gives `c.eq.s $f4,$f0`, the raw read the target's
 *    `c.eq.s $f0,$f4`.
 *  - the levers documented above gl_func_00026D64 in game_libs_post.c (the
 *    extern struct-array base, per-access cursor aliases, dead masks in the
 *    prologue, `if (ent) {}` / `if (cur) {}` empty tests). */
#include "common.h"

extern int gl_func_0003ADFC();
extern int gl_func_0003BA24();

extern int D_26D64_a, D_26D64_b, D_26D64_c, D_26D64_d;
#define CUR_A (*(u8 *)((char *)&D_26D64_a + 0x1B5DC))
#define CUR_B (*(u8 *)((char *)&D_26D64_b + 0x1B5DC))
#define CUR_C (*(u8 *)((char *)&D_26D64_c + 0x1B5DC))
#define CUR_D (*(u8 *)((char *)&D_26D64_d + 0x1B5DC))
typedef struct {
    /* 0x000 */ u8 flags;
    /* 0x001 */ u8 state;
    /* 0x002 */ u8 pad02[6];
    /* 0x008 */ s16 h08;
    /* 0x00A */ s16 h0A;
    /* 0x00C */ s16 h0C;
    /* 0x00E */ s16 h0E;
    /* 0x010 */ s16 h10;
    /* 0x012 */ s16 h12;
    /* 0x014 */ u8 pad14[8];
    /* 0x01C */ f32 f1C;
    /* 0x020 */ f32 f20;
    /* 0x024 */ f32 f24;
    /* 0x028 */ u8 pad28[4];
    /* 0x02C */ f32 f2C;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ f32 f34;
    /* 0x038 */ s32 w38[0x48];
    /* 0x158 */ s8 b158[8];
} Obj26D64;
typedef struct {
    /* 0x0000 */ u8 pad0[0x2048];
    /* 0x2048 */ s16 nobj;
    /* 0x204A */ u8 pad1[0x2D00 - 0x204A];
    /* 0x2D00 */ Obj26D64 objs[0x1B];
    /* 0x5300 */ u8 pad2[0x53BA - 0x2D00 - 0x1B * 0x160];
    /* 0x53BA */ u8 pending;
    /* 0x53BB */ u8 pad3;
    /* 0x53BC */ u16 masks[0x3A];
    /* 0x5430 */ u8 queue[0x100][8];
} St26D64;
extern St26D64 D_26D64_arr[];
#define g (&D_26D64_arr[0])
void gl_func_00026D64(u32 arg0) {
    s32 lo;
    s32 cur;
    s32 idx;
    u8 *ent;
    Obj26D64 *obj;  /* s2: object slot  (g + pidx*0x160 + 0x2D00) */
    s32 code;
    s32 tmp;
    u8 sel;
    s32 i;
    u16 mask;
    f32 f2;

    if (g->pending == 0) {
        CUR_A = (arg0 >> 8) & 0xFF;
    }
    lo = (u8) (arg0 & 0xFF);
    for (;;) {
        cur = CUR_B;
        idx = (cur & 0xFF) * 8;
        ent = (u8 *)g->queue + idx;
        if (lo == cur) {
            g->pending = 0;
            return;
        } if (ent) {}
        CUR_D = CUR_C + 1; /* run 132: no mask here; the t-register burns belong in the flags RMWs below (43 -> 2) */
        code = *(u8 *)ent;
        if (code == 0xF8) { if (cur) {}
            g->pending = 1;
            return;
        }
        if ((code & 0xF0) == 0xF0) {
            gl_func_0003ADFC(ent);
        } else {
            if ((s32) *(u8 *)(ent + 1) < g->nobj) {
                obj = &g->objs[*(u8 *)(ent + 1)];
                if (code & 0x80) {
                    gl_func_0003ADFC(ent);
                } else if (code & 0x40) {
                    switch (code) {
                    case 0x41:
                        if (*(f32 *)(ent + 0x4) != *(f32 *)((u8 *)obj + 0x2C)) { /* run 132: raw f2C read = target c.eq.s operand order */
                            obj->f2C = *(f32 *)(ent + 0x4);
                            obj->flags = (obj->flags & 0xFF) | 4; /* run 132: dead u8 mask = one t-register burn */
                        }
                        break;
                    case 0x42:
                        obj->h08 = *(s32 *)(ent + 0x4) * 0x30;
                        break;
                    case 0x43:
                        obj->h0C = *(s32 *)(ent + 0x4) * 0x30;
                        break;
                    case 0x44:
                        obj->h0C = *(s32 *)(ent + 0x4);
                        break;
                    case 0x45:
                        obj->h0E = *(s8 *)(ent + 0x4);
                        break;
                    case 0x46:
                        obj->b158[*(u8 *)(ent + 0x3)] = *(s8 *)(ent + 0x4);
                        break;
                    case 0x47:
                        f2 = (f32) (s32) *(u8 *)(ent + 0x2) / 127.0f;
                        goto block_24;
                    case 0x48:
                        f2 = ((f32) (s32) *(u8 *)(ent + 0x2) / 100.0f) * obj->f1C;
block_24:
                        if (obj->state != 2) {
                            obj->f24 = obj->f1C;
                            tmp = *(s32 *)(ent + 0x4);
                            if (tmp == 0) {
                                obj->f1C = f2;
                            } else {
                                obj->state = 0;
                                obj->h12 = tmp;
                                obj->f20 = (f2 - obj->f1C) / (f32) tmp;
                            }
                        }
                        break;
                    case 0x4A:
                        if (obj->state != 2) {
                            tmp = *(s32 *)(ent + 0x4);
                            if (tmp == 0) {
                                obj->f1C = obj->f24;
                            } else {
                                obj->state = 0;
                                obj->h12 = tmp;
                                obj->f20 = (obj->f24 - obj->f1C) / (f32) tmp;
                            }
                        }
                        break;
                    case 0x4C:
                        obj->f34 = *(f32 *)(ent + 0x4);
                        if (obj->f34 == 1.0f) {
                            obj->flags &= ~1;
                        } else {
                            obj->flags = (obj->flags & 0xFF) | 1;
                        }
                        break;
                    case 0x4D:
                    case 0x4E:
                        break;
                    }
                } else {
                    if (((u32) *(s32 *)obj >> 0x1F) != 0) {
                        sel = *(u8 *)(ent + 0x2);
                        if (sel < 0x10) {
                            gl_func_0003BA24(obj->w38[sel], ent);
                        } else if (sel == 0xFF) {
                            mask = g->masks[*(u8 *)(ent + 1)];
                            i = 0;
                            do {
                                if (mask & 1) {
                                    gl_func_0003BA24(obj->w38[i], ent);
                                }
                                i += 1;
                                mask = ((s32) mask >> 1) & 0xFFFF;
                            } while (i != 0x10);
                        }
                    }
                }
            }
        }
        *(u8 *)ent = 0;
    }
}
#undef g
#undef CUR_A
#undef CUR_B
#undef CUR_C
#undef CUR_D
