/* IDO 7.1 NCC (C++) NM donor for SnowBoarder_handleMsg (event handler, 0x4C4 / 305
 * words). The dispatcher is an edg case-order compare chain (20, 9, 6, 1000,
 * 1001, 1003, 1002, 1004, 1005..1007, default), so the function is C++.
 * 10 raw words left (agent-c run 125):
 * - the operator* temps (r sp+56, return temp sp+68) sit 16 bytes below the
 *   target's (72 / 84): the float->double compare temp and the clamp temps are
 *   allocated before them here and after them in the target;
 * - case 6 keeps 0.0 in f12 (target f2);
 * - bu_d900_k990 needs a pin at 0x990 (target spells it SndHandle_triggerEvent1_0000098C + 4).
 * What fixed the rest: the clamp as an inline function with if-returns into a
 * named k (`mov.s f0,f2` copy + 0/50 sharing f12), Vec3 ctor forwarding to
 * set() (x/y/z product temps f0/f2/f14 and the f12 copy of k), dot products in
 * natural x + y + z order, a named Data *p held across the stores, block locals
 * k / s and a block-scope int r for the frame.
 * Spliced into the NON_MATCHING comparison object only.
 * agent-g run 543: field form over include/game/snowboarder.h (included by relative path; its asserts are
 * C-only). Same bytes. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#include "../../include/game/snowboarder.h"
struct Vec3 {
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
};
inline Vec3 operator*(const Vec3 &a, float k) { Vec3 r(a.x * k, a.y * k, a.z * k); return r; }
inline float clampf(float v, float lo, float hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct S4 { int w; };
struct Ob { char pad[0xC]; unsigned short id; };
struct Data { Vec3 pos; float speed; char pad10[0x28 - 0x10]; Ob *o; char pad2c[0xE0 - 0x2C]; Vec3 vel; };
struct Ev { int type; Data *data; };
extern "C" {
int func_00000000(...);
extern double kRo_SnowBoarder_handleMsg_988;
extern double bu_d900_k990;
extern char bu_d900_aec[], bu_d900_b44[];
extern int D_00000000;
}
extern "C" void SnowBoarder_handleMsg(SnowBoarder *self, Ev *ev) {
    S4 sp8C;
    Data *p;
    S4 *sp84;
    Vec3 vA;
    Vec3 d;
    float k;
    float s;
    switch (ev->type) {
    case 20:
        if (self->msgCooldown == 0) {
            p = ev->data;
            if (p->speed < kRo_SnowBoarder_handleMsg_988 && p->o->id == 0x74) {
                func_00000000(self->sound); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is SndHandle_setFlag1A (TextReloc order, 15 jals = 15 sites) */
                return;
            }
            k = clampf(p->speed, 0.0f, 50.0f);
            d = p->pos * k;
            self->dnode.vqs.trans[0] += d.x;
            self->dnode.vqs.trans[1] += d.y;
            self->dnode.vqs.trans[2] += d.z;
            self->dnode.vel[0] += d.x;
            self->dnode.vel[1] += d.y;
            self->dnode.vel[2] += d.z;
            s = self->dnode.axes[1][0] * d.x + self->dnode.axes[1][1] * d.y + self->dnode.axes[1][2] * d.z;
            self->bumpSide = self->bumpSide + s * bu_d900_k990;
            s = self->dnode.axes[2][0] * d.x + self->dnode.axes[2][1] * d.y + self->dnode.axes[2][2] * d.z;
            self->hitFlags |= 0x1F0;
            self->bumpFwd = self->bumpFwd + -s * 0.5;
            self->contactSurface = p->o->id;
            self->msgCooldown = 10;
        }
        break;
    case 9:
        func_00000000(self); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is SnowBoarder_reset */
        break;
    case 6: {
        Vec3 v = ev->data->vel;
        v.x *= 4.0f;
        v.y *= 4.0f;
        v.z *= 4.0f;
        self->dnode.vqs.trans[0] += v.x;
        self->dnode.vqs.trans[1] += v.y;
        self->dnode.vqs.trans[2] += v.z;
        self->dnode.vel[2] = 0.0f;
        self->dnode.vel[1] = 0.0f;
        self->dnode.vel[0] = 0.0f;
        break;
    }
    case 1000:
        self->raceResult = 1;
        self->flags |= 4;
        func_00000000(self->ai); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is SnowAI_onWin */
        func_00000000(bu_d900_aec + 0x4C); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is dbg_printf */
        break;
    case 1001:
        self->raceResult = 2;
        self->flags |= 4;
        func_00000000(self->ai); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is SnowAI_onLose */
        func_00000000(bu_d900_aec + 0x54); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is dbg_printf */
        break;
    case 1003:
        self->raceResult = 3;
        self->flags |= 4;
        func_00000000(bu_d900_b44 + 0x4); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is dbg_printf */
        func_00000000(self->ai); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is SnowAI_onTimeout */
        break;
    case 1002:
        self->raceResult = 4;
        break;
    case 1004:
        self->raceResult = 5;
        break;
    case 1005:
    case 1006:
    case 1007:
        self->raceResult = 6;
        func_00000000(bu_d900_b44 + 0x10); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is dbg_printf */
        if (self->controller == 2) {
            sp84 = (S4 *)(bu_d900_b44 + 0x24);
            func_00000000(self->anim); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is RiderAnim_startEndingCpu_00011258 */
        } else {
            sp84 = (S4 *)(bu_d900_b44 + 0x28);
            func_00000000(self->anim); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is RiderAnim_startEnding_000112E0 */
        }
        sp8C.w = sp84->w;
        int r = func_00000000(D_00000000, sp8C, 0, self); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is ItemMgr_firstItemOfType */
        if (r != 0) {
            func_00000000(self, r + 0x30); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is DNode_resetXform_00065360 */
            return;
        }
        func_00000000(bu_d900_b44 + 0x2C, sp84); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is sys_fatal */
        break;
    default:
        func_00000000(self); /* UNNAMED: no-reloc NM-only donor (spliced into the NON_MATCHING object, its bytes are not the ROM), so this call has no ROM reloc; the ROM jal here is DNode_handleMsg */
        break;
    }
}
