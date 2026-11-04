/* IDO 7.1 -O2 donor for gl_func_00040304 -- transform-command handler
 * (0x33C / 207 words; 207/207 word-exact apart from reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 29CCC / 44B78 / 52CD4 recipe, docs/MATCHING_WORKFLOW.md#donor-splice-
 * switch-jumptable-rodata-rename). The C switch over op 0x6D..0x72 puts its
 * 6-entry jumptable at donor .rodata +0 and the 1/32767 literal right behind
 * it at +0x18; the splice renames that reloc to gl_func_00040304_rodata,
 * pinned to 0x1AE4 in undefined_syms_auto.txt (USO RoData: table +0x1AE4,
 * literal +0x1AFC). The 0.01f scale is a memory load of D_40304_k001 (+0x1AE0).
 *
 * Load-bearing (agent-g run 264, kits CCC / CCCII / CCCIV):
 *  - case 0x72's sp is a DIFFERENT expression item from case 0x6D's
 *    &self->scl (`(int)&self->scl | 0 * (int)v`, no code), and &self->m30 is
 *    mentioned first (`m3`, dead): case 0x72's m30 spill then takes 36(sp).
 *  - case 0x6D passes &self->scl as the third argument too (the target sets
 *    a2 = self + 0xC0 before the call), with the a1 copy spelled as a folded
 *    `| 0 * (int)cmd`: item a2, `or a1,a2,zero`, cmd a3 = target.
 *  - frame map by decl order (Vec3 z/x/y at 132/108/60); if (1) after the
 *    p load / first quat product / before `y = *sp`; the dead `while (0)`
 *    anchor and the code-free float tests set the FP colour order. */
#include "common.h"

typedef struct { float x, y, z; } Vec3;
typedef struct { float m[16]; } Mtx44_40304;
typedef struct {
    char pad0[0x30];
    Mtx44_40304 m30;      /* 0x30 */
    Mtx44_40304 m70;      /* 0x70 */
    float padB0;
    Vec3 pos;       /* 0xB4 */
    Vec3 scl;       /* 0xC0 */
    float q[4];     /* 0xCC */
    Vec3 pos2;      /* 0xDC */
    Vec3 scl2;      /* 0xE8 */
    float q2[4];    /* 0xF4 */
} Obj40304;
typedef struct { int op; short **cur; } Cmd40304;
extern float D_40304_k001;
extern int gl_func_00000000();
extern void gl_func_00040304_quat(float *, float, float, float);

void gl_func_00040304(Obj40304 *self, Cmd40304 *cmd) {
    Vec3 z;
    Vec3 *sp;
    short **p;
    float kt;
    Vec3 x;
    float pad2[8];
    void *m3;
    Vec3 y;
    short *v;
    float a, b, c, d;

    m3 = &self->m30;
    switch (cmd->op) {
        case 0x6D:
            gl_func_00000000(cmd, (Vec3 *)((int)&self->scl | 0 * (int)cmd), &self->scl);
            x = self->scl;
            self->scl2.x = x.x; self->scl2.y = x.y; self->scl2.z = x.z;
            break;
        case 0x6E:
            gl_func_00000000(cmd, &z);
            gl_func_00040304_quat(self->q, z.x, z.y, z.z);
            x = *(Vec3 *)self->q;
            self->q2[0] = x.x; self->q2[1] = x.y; self->q2[2] = x.z; self->q2[3] = self->q[3];
            break;
        case 0x6F:
            gl_func_00000000(cmd, &self->pos);
            x = self->pos;
            self->pos2.x = x.x; self->pos2.y = x.y; self->pos2.z = x.z;
            break;
        case 0x72:
            while (0) { a = 0; b = 0; kt = 0; d = kt * (1.0f / 32767.0f); }
            p = cmd->cur; if (1) {} cmd->cur = p + 1; v = *p;
            sp = (Vec3 *)((int)&self->scl | 0 * (int)v);
            a = v[3] * (1.0f / 32767.0f);
            if (1) {}
            b = v[4] * (1.0f / 32767.0f);
            c = v[5] * (1.0f / 32767.0f);
            d = v[6] * (1.0f / 32767.0f);
            if (a != b) {} if (a != b) {}
            self->q[0] = a;
            self->q[1] = b;
            self->q[2] = c;
            self->q[3] = d;
            self->pos.x = v[0];
            self->pos.y = v[1];
            self->pos.z = v[2];
            kt = D_40304_k001;
            if (a != kt) {} if (a != (1.0f / 32767.0f)) {} if (b != (1.0f / 32767.0f)) {} if (b != (1.0f / 32767.0f)) {}
            a = v[7] * kt;
            b = v[8] * kt;
            c = v[9] * kt;
            self->scl.x = a;
            self->scl.y = b;
            sp->z = c;
            x = self->pos;
            self->pos2.x = x.x; self->pos2.y = x.y; self->pos2.z = x.z;
            if (1) {}
            y = *sp;
            self->scl2.x = y.x; self->scl2.y = y.y; self->scl2.z = y.z;
            y = *(Vec3 *)self->q;
            self->q2[0] = y.x; self->q2[1] = y.y; self->q2[2] = y.z; self->q2[3] = self->q[3];
            gl_func_00000000(&self->pos2, &self->m30, sp);
            self->m70 = self->m30;
            break;
        case 0x70:
        case 0x71:
        default:
            gl_func_00000000(self, cmd);
            break;
    }
}
