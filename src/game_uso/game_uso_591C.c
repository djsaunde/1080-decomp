/* game_uso_func_0000591C: EXACT donor, IDO 7.1 cfe -O2 C (agent-c run 237, from the
 * in-unit NM body: agent-e / agent-g / agent-c runs 182-236, 99.977 -> 100),
 * REPLACE_FUNC_BODY-spliced over the placeholder in src/game_uso/game_uso.c on both
 * build paths (GAMEUSO_591C_DONOR). The superseded in-unit body (with its decode
 * history) is kept under GAMEUSO_591C_OLD_DECODE in game_uso.c.
 *
 * The last two cells (docs/IDO_CODEGEN.md kit for agent-c run 237):
 *  - `v0 * 1 == 0x10000`: uopt input swaps ==/!= when the left operand is not an
 *    isvar, so the multiply-by-one gives the target's const-first `bne v1, v0`.
 *  - every nested float selector's inner join goes through `scale` (dead after the
 *    transform dispatch; `fsel = c3 ? A : (scale = c2 ? B : C, scale)`): scale's
 *    single LR is coloured f14 (it interferes with x f0 / z f2 / fsel f12 at the
 *    head), so the inner value is f14 at all ten sites while the 0x204 selector can
 *    be fsel (f12) and the dead `if (z) {}` keeps z above fsel in priority. With
 *    cfe's own join temp the temp took f2 once z no longer reached a join block. */
#include "common.h"
extern char import_80020110;
extern int import_8006EF48;
typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b, c; } Tri3i;
extern int game_uso_func_055750();
float game_uso_func_00003ED4(Vec3 *a0, Vec3 *a1, int *a2);
void game_uso_func_0000A3C4();
void game_uso_func_0000A604(char *);
void game_uso_func_0000A7F8(char *);
int game_uso_func_00007ACC();
float game_uso_func_00007A98(int *);
int game_uso_func_00007C1C();
void *game_uso_func_00008CD8();
void game_uso_func_000074D8(char *);
void game_uso_func_0000751C(char *);
void game_uso_func_00007448(char *);
void game_uso_func_00007424(void *);
int game_uso_func_00009B88();
void game_uso_func_0000A7D8(int *);
int game_uso_func_0000A0E8(char *, char *, char *);
void game_uso_func_0000AB98(void *);
void game_uso_func_000071A4(int *);
int game_uso_func_00006FA8(int *);
void game_uso_func_00007538();
void game_uso_func_0000591C(int *a0) {
    Vec3 copy2;
    Vec3 staged_axis;
    int out_flags;
    char *helper_ptr;
    char *hit_parent;
    char *hit_obj;
    int state_flag;
    int resolved_state;
    float metric_a;
    Vec3 transform_out;
    int *self;
    int out_w;
    int hit_out;
    Vec3 scratch_xz;
    char *sub;
    float yaw_metric;
    float accel_metric;
    int v0;
    Vec3 stage;
    int vsel;
    int t0;
    Vec3 scaled_axis;
    float *src;
    int *child_flags;
    float fsel;
    float f2v;
    float dist_sq;
    float dot;
    float f12v;
    float yaw_raw;
    float neg;
    float *accel_params;
    Vec3 res_a;
    Vec3 res_b;
    Vec3 res_c;
    Vec3 pos_a;
    Vec3 pos_c;
    Vec3 delta_a;
    Vec3 delta_b;
    Vec3 *p;
    float x;
    float *hsrc;
    Vec3 diff_a;
    float z;
    float *axis_src;
    float scale;
    int bits;
    Vec3 *scratch_dst;
    Vec3 diff_b;
    Vec3 *delta_a_dst;
    Vec3 *delta_b_dst;
    Vec3 pos_e;
    Vec3 pos_g;
    Vec3 delta_e;
    Vec3 delta_g;
    Vec3 *delta_e_dst;
    Vec3 diff_e;
    Vec3 diff_g;
    Vec3 *delta_g_dst;
    self = a0;
    if (*(int*)((char*)&import_80020110 + 0x78) != 0) return;
    if (import_8006EF48 == 0) return;
    v0 = *(int*)((char*)self + 0x68);
    if (v0 & 1) {
        game_uso_func_0000A3C4();
        return;
    }
    if (v0 & 2) {
        game_uso_func_0000A604(self);
        return;
    }
    if (v0 & 4) {
        game_uso_func_0000A7F8(self);
        return;
    }
    *(Tri3i *)&staged_axis = *(Tri3i *)(*(char**)((char*)self + 0x30) + 0xB4);
    {
        axis_src = *(float **)((char*)self + 0x30);
        axis_src = (float *)((int)axis_src + 0x318);
        fsel = *(float*)((char*)self + 0xA8);
        scale = *axis_src * fsel;
        z = *(float *)((int)axis_src + 4) * fsel; if (z) {}
        x = *(float *)((int)axis_src + 8) * fsel;
        scaled_axis.z = x; if (1) {}
        scaled_axis.y = z;
        scaled_axis.x = scale;
    }
    *(Tri3i *)&stage = *(Tri3i *)&scaled_axis;
    *(Tri3i *)&copy2 = *(Tri3i *)&stage;
    out_flags = 0;
    staged_axis.x += copy2.x;
    staged_axis.y += copy2.y;
    staged_axis.z += copy2.z;
    helper_ptr = (char*)game_uso_func_00007ACC(self, &hit_parent, &staged_axis, &hit_obj);
    if (helper_ptr == 0) return;
    state_flag = *(int*)(helper_ptr + 0x84);
    if (hit_obj != 0) {
        resolved_state = state_flag;
    } else {
        resolved_state = 0;
    }
    metric_a = game_uso_func_00007A98(self);
    t0 = 0;
    switch (v0 = *(int*)((char*)self + 0x74)) {
    case 0:
        if (metric_a <= *(float*)((char*)self + 0xD8)) {
            if (((v0 = state_flag) == 0) && (hit_parent != 0)
                    && (*(int*)(hit_parent + 0x84) == 0)) {
                if (*(float*)((char*)self + 0xB4) <= *(float*)(*(char**)((char*)self + 0x30) + 0x348)) {
                    *(int*)((char*)self + 0x74) = 1;
                    t0 = 1;
                }
            }
        }
        break;
    case 1:
        v0 = *(int*)((char*)self + 0x2C);
        fsel = v0 == 3 ? *(float*)((char*)self + 0x3FC) : (scale = v0 == 2 ? *(float*)((char*)self + 0x3E4) : *(float*)((char*)self + 0x3CC), scale);
        if (!(metric_a <= fsel) || state_flag != 0 || hit_parent == 0
                || *(int*)(hit_parent + 0x84) != 0
                || !(*(float*)((char*)self + 0xB4)
                     <= *(float*)(*(char**)((char*)self + 0x30) + 0x348))) {
            *(int*)((char*)self + 0x74) = 0;
        }
        break;
    }
    p = 0;
    if (1) { p = &scratch_xz; }
    switch (v0 = *(int*)((char*)self + 0x74)) {
    case 0:
        v0 = game_uso_func_00007C1C(&stage, self, helper_ptr, hit_parent,
            &staged_axis, &out_w);
        *(Tri3i *)&res_a = *(Tri3i *)v0;
        *(Tri3i *)&res_c = *(Tri3i *)&res_a;
        x = res_c.x; scale = res_c.y; z = res_c.z; transform_out.x = x; transform_out.y = scale; transform_out.z = z;
        break;
    case 1:
        v0 = (int)game_uso_func_00008CD8(&stage, self, helper_ptr, hit_parent, t0);
        *(Tri3i *)&res_b = *(Tri3i *)v0;
        *(Tri3i *)&res_c = *(Tri3i *)&res_b;
        if (1) {}
        x = res_c.x; scale = res_c.y; z = res_c.z; transform_out.x = x; transform_out.y = scale; transform_out.z = z;
        break;
    }
    {
        scratch_dst = p;
        src = (float *)((int)*(char**)((char*)self + 0x30) + 968);
        if (scratch_dst != 0 || (scratch_dst = (Vec3*)game_uso_func_055750(0xC + 0 * (int)src)) != 0) {
            fsel = src[0 * ((int)src | 1)];
            z = *(float *)((int)src + 8 + 0 * (int)src);
            do { } while (0);
            scratch_dst->x = fsel;
            scratch_dst->z = z;
            scratch_dst->y = 0.0f;
        }
    }
    yaw_raw = game_uso_func_00003ED4(p, &transform_out, &hit_out);
    fsel = yaw_raw < 0.0f ? -yaw_raw : yaw_raw;
    yaw_metric = fsel;
    neg = -yaw_raw;
    sub = *(char**)((char*)self + 0x30);
    {
        axis_src = (float *)(*(char**)((char*)a0 + 0x30) + 1784);
        *(float*)((char*)self + 0x3C) =
            (neg * (1.0f + (*(float*)(sub + 0x348) / *(float*)((char*)self + 0xB0)))
             * *(float*)((char*)self + 0xAC)) / *(float *)((int)axis_src + 16);
    }
    f2v = *(float*)((char*)self + 0x3C);
    fsel = f2v < 0.0f ? -f2v : f2v;
    accel_metric = fsel;
    v0 = *(int*)((char*)self + 0x74);
    switch (v0) {
    case 0:
    bits = *(int*)((char*)self + 0x6C);
    if (bits & 1) {
        if ((resolved_state & 1) && !(resolved_state & 2)) {
            *(int*)((char*)self + 0x6C) = bits & ~1;
            *(int*)((char*)self + 0x48) = 0x14;
            if (resolved_state & 4) {
                out_flags = 0x20;
            } else if (resolved_state & 0x800) {
                out_flags = 0x80;
            }
            goto tail_common;
        }
        if (hit_parent == 0 || !(*(int*)(hit_parent + 0x84) & 2))
            goto tail_common;
        p = 0;
        if (1) { p = &pos_a; }
        src = (float *)((int)helper_ptr + 48);
        if (p != 0 || (p = (Vec3*)game_uso_func_055750(0xC)) != 0) {
            fsel = *src;
            z = *(float *)((int)src + 8);
            p->y = 0.0f;
            p->z = z;
            p->x = fsel;
        }
        p = 0;
        if (1) { p = &diff_a; }
        hsrc = (float *)((int)hit_parent + 48);
        {
            delta_a_dst = p;
            if (delta_a_dst != 0 || (delta_a_dst = (Vec3*)game_uso_func_055750(0xC)) != 0) {
                x = pos_a.x - *hsrc;
                z = pos_a.z - *(float *)((int)hsrc + 8);
                delta_a_dst->y = 0.0f;
                delta_a_dst->x = x;
                delta_a_dst->z = z;
            }
        }
        stage = *p;
        delta_a = stage;
        p = 0;
        if (1) { p = &pos_c; }
        child_flags = (int *)((int)*(char**)((char*)self + 0x30) + 180);
        if (p != 0 || (p = (Vec3*)game_uso_func_055750(0xC)) != 0) {
            fsel = *(float *)child_flags;
            z = *(float *)((int)child_flags + 8);
            p->y = 0.0f;
            p->x = fsel;
            p->z = z;
        }
        p = 0;
        if (1) { p = &diff_b; }
        accel_params = (float *)((int)hit_parent + 48 + 0 * (int)child_flags);
        {
            child_flags = (int *)p;
            if (child_flags != 0 || (child_flags = (int *)game_uso_func_055750(0xC)) != 0) {
                x = pos_c.x - *accel_params;
                z = pos_c.z - *(float *)((int)accel_params + 8 + 0 * (int)accel_params);
                ((Vec3 *)child_flags)->y = 0.0f;
                ((Vec3 *)child_flags)->x = x;
                ((Vec3 *)child_flags)->z = z;
            }
        }
        stage = *(Vec3 *)((int)p + 0 * (int)accel_params);
        delta_b = stage;
        sub = *(char**)((char*)a0 + 0x30);
        f2v = *(float*)((char*)self + 0xC0) * *(float*)(sub + 0x348);
        dist_sq = (delta_a.x * delta_a.x) + (delta_a.z * delta_a.z);
        if (0.0f == dist_sq) {
        f12v = 0.0f;
    } else {
        dot = (delta_a.x * delta_b.x) + (delta_a.z * delta_b.z);
        f12v = (dot * dot) / dist_sq;
    }
        if (f12v <= f2v * f2v) {
            *(int*)((char*)self + 0x6C) = *(int*)((char*)self + 0x6C) & ~1;
            *(int*)((char*)self + 0x48) = 0x14;
        }
        goto tail_common;
    }
    if (bits != 0) goto tail_common;
    if (hit_parent == 0 || !((v0 = *(int*)(hit_parent + 0x84)) & 1)) goto yaw_region;
    if (v0 & 0x804) {
        if (*(int*)(sub + 0x908) != 0) {
            if (!(metric_a <= -2000.0f)) goto yaw_region;
        }
    }
    p = 0;
    if (1) { p = &pos_e; }
    src = (float *)((int)helper_ptr + 48);
    if (p != 0 || (p = (Vec3*)game_uso_func_055750(0xC)) != 0) {
        fsel = *src;
        z = *(float *)((int)src + 8);
        p->y = 0.0f;
        p->z = z;
        p->x = fsel;
    }
    p = 0;
    if (1) { p = &diff_e; }
    hsrc = (float *)((int)hit_parent + 48);
    {
        delta_e_dst = p;
        if (delta_e_dst != 0 || (delta_e_dst = (Vec3*)game_uso_func_055750(0xC)) != 0) {
            x = pos_e.x - *hsrc;
            z = pos_e.z - *(float *)((int)hsrc + 8);
            delta_e_dst->y = 0.0f;
            delta_e_dst->x = x;
            delta_e_dst->z = z;
        }
    }
    stage = *p;
    delta_e = stage;
    p = 0;
    if (1) { p = &pos_g; }
    child_flags = (int *)((int)*(char**)((char*)self + 0x30) + 180);
    if (p != 0 || (p = (Vec3*)game_uso_func_055750(0xC)) != 0) {
        fsel = *(float *)child_flags;
        z = *(float *)((int)child_flags + 8);
        p->y = 0.0f;
        p->x = fsel;
        p->z = z;
    }
    p = 0;
    if (1) { p = &diff_g; }
    accel_params = (float *)((int)hit_parent + 48 + 0 * (int)child_flags);
    {
        child_flags = (int *)p;
        if (child_flags != 0 || (child_flags = (int *)game_uso_func_055750(0xC)) != 0) {
            x = pos_g.x - *accel_params;
            z = pos_g.z - *(float *)((int)accel_params + 8 + 0 * (int)accel_params);
            ((Vec3 *)child_flags)->y = 0.0f;
            ((Vec3 *)child_flags)->x = x;
            ((Vec3 *)child_flags)->z = z;
        }
    }
    stage = *(Vec3 *)((int)p + 0 * (int)accel_params);
    delta_g = stage;
    sub = *(char**)((char*)self + 0x30);
    f2v = (*(float*)((char*)self + 0xC0) + 12.0f) * *(float*)(sub + 0x348);
    dist_sq = (delta_e.x * delta_e.x) + (delta_e.z * delta_e.z);
    if (0.0f == dist_sq) {
        f12v = 0.0f;
    } else {
        dot = (delta_e.x * delta_g.x) + (delta_e.z * delta_g.z);
        f12v = (dot * dot) / dist_sq;
    }
    if (f12v <= f2v * f2v) {
        out_flags = 1;
    }
yaw_region:
    sub = *(char**)((char*)self + 0x30);
    dot = *(float*)(sub + 0x348);
    if (*(float*)((char*)self + 0xBC) <= yaw_metric) {
        if (dot < 50.0f) {
            out_flags |= 0x40;
        }
    }
    if (dot < *(float*)((char*)self + 0xB4)) {
        if (hit_out == 0) {
            if ((double)yaw_metric < 0.125) {
                if (*(int*)((char*)&import_80020110 + 0x78) == 0) {
                    out_flags |= 4;
                }
            }
        }
    }
    if (1.0f < (dot / *(float*)((char*)self + 0xB8)) * accel_metric) {
        out_flags |= 8;
    }
    if (state_flag & 8) {
        if (*(int*)((char*)self + 0x4C4) > 0) {
            game_uso_func_000074D8(self);
            *(int*)((char*)self + 0x4C4) = 0;
        }
        goto tail_common;
    }
    v0 = *(int*)((char*)self + 0x4C4);
    if (v0 * 1 == 0x10000) {
        v0 = *(int*)((char*)self + 0x2C);
        fsel = v0 == 3 ? *(float*)((char*)self + 0x2DC) : (scale = v0 == 2 ? *(float*)((char*)self + 0x2C4) : *(float*)((char*)self + 0x2AC), scale);
        if (metric_a < fsel) {
            game_uso_func_0000751C(self);
            v0 = *(int*)((char*)self + 0x2C);
            fsel = v0 == 3 ? *(float*)((char*)self + 0x324) : (scale = v0 == 2 ? *(float*)((char*)self + 0x30C) : *(float*)((char*)self + 0x2F4), scale);
            *(int*)((char*)self + 0x4C4) = (int)fsel;
        }
        goto tail_common;
    } else if (v0 > 0) {
        *(int*)((char*)self + 0x4C4) = v0 - 1;
        if (*(int*)((char*)self + 0x4C4) == 0) {
            game_uso_func_000074D8(self);
        }
    } else {
        v0 = *(int*)((char*)self + 0x2C);
        if (v0 != 0) {
            fsel = v0 == 3 ? *(float*)((char*)self + 0x3B4) : (scale = v0 == 2 ? *(float*)((char*)self + 0x39C) : *(float*)((char*)self + 0x384), scale);
            if (fsel < metric_a) {
                *(int*)((char*)self + 0x4C4) = 0x10000;
                game_uso_func_00007448(self);
            }
        }
    }
    v0 = *(int*)((char*)self + 0x2C);
    fsel = v0 == 3 ? *(float*)((char*)self + 0x12C) : (scale = v0 == 2 ? *(float*)((char*)self + 0x114) : *(float*)((char*)self + 0xFC), scale);
    sub = *(char**)((char*)self + 0x30);
    if (fsel < metric_a) {
        if (*(int*)(sub + 0x9CC) != 0) {
            f2v = *(float*)((char*)self + 0xC8);
        } else {
            f2v = 1.0f;
        }
        if (((accel_metric <= f2v * *(float*)((char*)self + 0xC4)) + 0 * (int)(*a0))) {
            out_flags |= 2;
            goto tail_common;
        }
    }
    if (resolved_state & 0x800) {
        if ((*(int*)(sub + 0x908) == 0) || (metric_a <= -2000.0f)) {
            out_flags |= 0x80;
        }
    }
tail_common:
        v0 = *(unsigned int*)((char*)self + 0x2C);
        break;
    case 1:
    default:
        v0 = *(unsigned int*)((char*)self + 0x2C);
        break;
    }
    fsel = v0 == 1 ? *(float*)((char*)self + 0x264) : (scale = v0 == 2 ? *(float*)((char*)self + 0x27C) : *(float*)((char*)self + 0x294), scale);
    sub = *(char**)((char*)self + 0x30);
    {
        child_flags = (int *)((int)sub + 2648);
        if ((metric_a < fsel) && (((int)metric_a & 3) == 0)) {
            *child_flags |= 0x4000;
        } else {
            *child_flags &= ~0x4000;
        }
    }
    if (*(float*)(*(char**)((char*)self + 0x30) + 0x348) < 30.0f) {
        { int cnt; cnt = *(int*)((char*)self + 0x54) + 1;
        *(int*)((char*)self + 0x54) = cnt;
        if (child_flags) {}
        if (cnt >= 75) {
            *(int*)((char*)self + 0x54) = 0;
            game_uso_func_00007424(self);
        } }
    } else {
        *(int*)((char*)self + 0x54) = 0;
    }
    if (state_flag & 0x20) {
        if (game_uso_func_00009B88(self, helper_ptr, hit_parent) == 0) {
            game_uso_func_0000A7D8(self);
            goto commit_flags;
        }
    }
    if (state_flag & 0x40) {
        if (game_uso_func_0000A0E8(self, helper_ptr, hit_parent) == 0) {
            game_uso_func_0000AB98(self);
            goto commit_flags;
        }
    }
    if (resolved_state & 0x400) {
        game_uso_func_000071A4(self);
        *(int*)((char*)self + 0x40) = game_uso_func_00006FA8(self);
        goto commit_flags;
    }
    if (resolved_state & 0x10) {
        *(int*)((char*)self + 0x7C) = *(int*)(helper_ptr + 0x6C);
        if (resolved_state & 0x200) {
            v0 = *(int*)((char*)self + 0x2C);
            fsel = v0 == 3 ? *(float*)((char*)self + 0x24C) : (scale = v0 == 2 ? *(float*)((char*)self + 0x234) : *(float*)((char*)self + 0x21C), scale);
            if (!(fsel <= metric_a)) {
                v0 = *(int*)(*(char**)((char*)self + 0x30) + 0x908);
                if (v0 == 0) goto commit_flags;
                yaw_raw = *(float*)(v0 + 0xBC);
                f2v = yaw_raw < 0.0f ? -yaw_raw : yaw_raw;
                if (((int)f2v % 5) != 0) goto commit_flags;
            }
            *(int*)((char*)self + 0x40) = *(int*)((char*)self + 0x7C);
        } else if (resolved_state & 0x100) {
            v0 = *(int*)((char*)self + 0x2C);
            if (v0 != 0) {
                fsel = v0 == 3 ? *(float*)((char*)self + 0x1BC) : (scale = v0 == 2 ? *(float*)((char*)self + 0x1A4) : *(float*)((char*)self + 0x18C), scale);
                if (!(metric_a <= fsel)) goto commit_flags;
            }
            *(int*)((char*)self + 0x40) = *(int*)((char*)self + 0x7C);
        } else if (resolved_state & 0x80) {
            v0 = *(int*)((char*)self + 0x2C);
            fsel = v0 == 3 ? *(float*)((char*)self + 0x174) : (scale = v0 == 2 ? *(float*)((char*)self + 0x15C) : *(float*)((char*)self + 0x144), scale);
            if (!(metric_a <= fsel)) goto commit_flags;
            *(int*)((char*)self + 0x40) = *(int*)((char*)self + 0x7C);
        } else {
            v0 = *(int*)((char*)self + 0x2C);
            fsel = v0 == 3 ? *(float*)((char*)self + 0x204) : (scale = v0 == 2 ? *(float*)((char*)self + 0x1EC) : *(float*)((char*)self + 0x1D4), scale);
            if (fsel <= metric_a) {
                *(int*)((char*)self + 0x40) = *(int*)((char*)self + 0x7C);
            } else if (0.0f < metric_a) {
                *(int*)((char*)self + 0x68) |= 1;
            } else {
                v0 = *(int*)(*(char**)((char*)self + 0x30) + 0x908);
                if (v0 == 0) goto commit_flags;
                yaw_raw = *(float*)(v0 + 0xBC);
                fsel = yaw_raw < 0.0f ? -yaw_raw : yaw_raw;
                if (((int)fsel % 5) != 0) goto commit_flags;
                *(int*)((char*)self + 0x40) = *(int*)((char*)self + 0x7C);
            }
        }
    } else if (hit_parent != 0 && *(int*)(hit_parent + 0x2C) == 0) {
        if (*(int*)((char*)self + 0x40) != 0) {
            *(int*)((char*)self + 0x40) = *(int*)(hit_parent + 0x6C);
        }
    } else if (hit_parent == 0) {
        if (*(int*)((char*)self + 0x40) != 0) {
            *(int*)((char*)self + 0x40) = *(int*)(helper_ptr + 0x6C);
        } else {
            out_flags |= 0x10;
        }
    }
commit_flags:
    game_uso_func_00007538(self, out_flags);
}
