/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29, Mar 13 1998) EXACT donor for game_uso_func_00000B3C
 * (0xB3C..0x1644, 706 words), compiled by tools/ncc721.sh (edgcpfe -> IDO 7.1 uopt / ugen / as1, the NCC
 * flags) and spliced over a placeholder in both build trees (agent-h run 355).
 *
 * Why this front end: B3C's `switch (s->0x26C)` has 4 dense labels (0..3) and the target lowers it as a
 * 4-entry jump table (`sltiu at,v0,4; beqz; sll; lui; addu; lw; jr`, table at RoData +0x50). No IDO-era
 * front end does that: cfe tables only from 5 labels and the IDO 7.1 NCC edg (2.19) never emits xjp.
 * The period-correct MIPSpro 7.2 / 7.2.1 edgcpfe (EDG 2.29: table iff n >= 4 and size <= 2n) does, and
 * this body is byte-exact under it (docs/TOOLING_DECOMP.md "Toolchain forensics III", run 352).
 * Source = the cfe NM body (99.9986) with the phantom `case 4:` dropped, `ob[0] = tv` stored through
 * `*(char **)((char *)ob + 0)` (edg forwards a plain store) and `x == 1` instead of `1 == x` in the
 * first && test.
 *
 * RoData layout: the target's `lw t,0x50(at)` is an in-place %lo (the game_uso Yay0 block is raw .text,
 * no reloc bake), so the table must sit at donor .rodata+0x50. The two layout-only functions below
 * re-create what precedes it in the unit's RoData: game_uso_func_00000000's 1/6 literal (+0x0) and
 * game_uso_func_00000940's three tables (+0x4 / +0x20 / +0x38). The splice copies only
 * game_uso_func_00000B3C; its renamed .rodata reloc (game_uso_func_00000B3C_rodata) is pinned to 0 in
 * undefined_syms_auto.txt, so lui 0 / lw 0x50 both come out of the in-place addends. */
extern "C" {
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b, c; } Tri3i;
typedef struct { int w[16]; } B3CBlk40;

float game_uso_func_000000A0(float *a, float *b);
void game_uso_func_000000E0(Vec3 *out, Vec3 **ctrl, float *weights);
void game_uso_func_0000052C(int unused, Vec3 *a1, Vec3 *a2, int *a3, Vec3 *p4);
int game_uso_func_00000674(int *a0);
extern char import_80020098;
void game_uso_func_00000AEC(int *a0, int a1);
void game_uso_func_00000940(char *arg0);
Vec3 *game_uso_func_000023D4(Vec3 *out, char *a1);
/* 2714's second parameter is unsigned (agent-g run 239, kit CCLVII): the literal-1 arguments are then
 * materialised by `addiu a1,zero,1`, not copied from the `tmp = 1` LR. */
void game_uso_func_00002714(int *a0, unsigned int a1, int a2);
void game_uso_func_00002FC8(int *a0, int a1, int a2);
/* basis evaluator; prototyped so the float t travels in $a1 (blank jal 0 = the unit's func_00000000) */
void game_uso_func_00000000(f32 *out, f32 t);

/* layout only: game_uso RoData +0x0 (the 1/6 literal) */
void game_uso_func_00000000_lay(float *out, float t) { out[0] = 0.16666667f; }

/* layout only: the cfe-exact game_uso_func_00000940 body (identical under this front end); its three
 * tables are RoData +0x4 / +0x20 / +0x38 */
void game_uso_func_00000940_lay(char *arg0) {
    s32 temp_v0;
    s32 var_a1;

    var_a1 = -1;
    switch (*(int *)((char *)*(int *)((char *)arg0 + 0x150) + 0xA54)) {
    case 0:
        temp_v0 = *(int *)((char *)&import_80020098 + 0x34);
        if ((temp_v0 == 4) || (temp_v0 == 6) || (temp_v0 == 7) || ((temp_v0 == 1) && (*(int *)((char *)&import_80020098 + 0x40) == 0))) {
            var_a1 = 1;
        } else {
            switch (*(int *)((char *)&import_80020098 + 0x64)) {
            case 2: var_a1 = 6; break;
            case 3: var_a1 = 7; break;
            case 4: var_a1 = 8; break;
            case 5: var_a1 = 7; break;
            case 6: var_a1 = 9; break;
            case 7: var_a1 = 0xA; break;
            default: var_a1 = 0; break;
            }
        }
        break;
    case 1:
        if (*(int *)((char *)&import_80020098 + 0x7C) != 0) {
            var_a1 = 0xB;
        } else {
            switch (*(int *)((char *)&import_80020098 + 0x64)) {
            case 2: var_a1 = 0xD; break;
            case 3: var_a1 = 2; break;
            case 4: var_a1 = 0xD; break;
            case 5: var_a1 = 2; break;
            case 6: var_a1 = 2; break;
            case 7: var_a1 = 0xE; break;
            default: var_a1 = 2; break;
            }
        }
        break;
    case 2:
        var_a1 = 0xC;
        break;
    case 5:
        var_a1 = 3;
        break;
    case 6:
        if (*(int *)((char *)&import_80020098 + 0x64) != 7) {
            var_a1 = 4;
        } else {
            var_a1 = 5;
        }
        break;
    }
    if (var_a1 != -1) {
        game_uso_func_00000AEC((int *)arg0, var_a1);
    }
}

/* Spline path-follow update. */
void game_uso_func_00000B3C(char *arg0) {
    Vec3 vD;          /* 0x144 */
    Vec3 vC;          /* 0x138 */
    char *ob[1];      /* 0x134: obj, a one-word stack object (memory-resident) */
    int *tbl;         /* 0x130 */
    char *seg;        /* 0x12C */
    int *kf;          /* 0x128 */
    Vec3 pos2;        /* 0x11C */
    Vec3 pos1;        /* 0x110 */
    Vec3 up;          /* 0x104 */
    f32 state[4];     /* 0xF4 */
    s32 nsub;         /* 0xF0 */
    s32 ncur;         /* 0xEC */
    s32 nseg;         /* 0xE8 */
    char *s;          /* 0xE4 */
    Vec3 zv;          /* 0xD8 */
    char *tv;
    s32 cur;
    s32 node;
    s32 selv;
    s32 st;
    char *bv;         /* 0xC0 */
    Vec3 stage;       /* 0xB4 */
    s32 tmp;
    f32 f1;
    f32 fy;           /* 0xA8 */
    Vec3 *c1[4];      /* 0x98 */
    Vec3 *c2[4];      /* 0x88 */
    Vec3 fresh;       /* 0x7C */
    f32 fz;           /* 0x78 */
    Vec3 *c3[4];      /* 0x68 */
    f32 w[4];         /* 0x58 */
    Vec3 *c4[4];      /* 0x48 */
    Vec3 v3C;         /* 0x3C */
    Vec3 v30;         /* 0x30 */

    s = arg0;
    tv = *(char **)(s + 0xF4); *(char **)((char *)ob + 0) = tv;
    tmp = *(s32 *)(*(char **)(s + 0x150) + 0xA54);
    if (tmp != *(s32 *)(s + 0x264)) {
        *(s32 *)(s + 0x264) = tmp;
        game_uso_func_00000940(s);
    }
    tmp = 1; /* tmp also holds the constant 1 (target `bne t0,v0` operand order) */
    tbl = ((int **)(s + 0x158))[*(s32 *)(s + 0x250)];
    cur = *(s32 *)(s + 0x25C);
    node = ((s32 *)((char *)tbl + 4))[*(s32 *)(s + 0x254)];
    *(s32 *)(s + 0x258) = node;
    seg = 0;
    if (1) { seg = ((char **)(s + 0x1D4))[node]; }
    kf = ((int **)(seg + 0x38))[cur];
    if (tmp == cur) {
        if (*(s32 *)(s + 0x260) == 0) {
            if (*(char **)(s + 0x268) != 0) {
                *(s32 *)(s + 0x130) = tmp;
                zv.z = 0.0f;
                zv.y = 0.0f;
                zv.x = 0.0f;
                *(Tri3i *)&stage = *(Tri3i *)&zv;
                *(f32 *)(s + 0x134) = stage.x;
                *(f32 *)(s + 0x138) = stage.y;
                *(f32 *)(s + 0x13C) = stage.z;
            } else {
                *(s32 *)(s + 0x130) = (*(s32 *)(seg + 0x30) == 0) == 0;
            }
            if (tmp == *(s32 *)(seg + 0x10)) {
                game_uso_func_00002714(*(int **)(s + 0x154), 1, *(s32 *)(s + 0x150) + 0x318);
            } else {
                game_uso_func_00002714(*(int **)(s + 0x154), 0, *(s32 *)(s + 0x150) + 0x318);
            }
            if (*(s32 *)(seg + 0x0) == 1) {
                game_uso_func_00002FC8(*(int **)(ob[0] + 0xB4), 1, *(s32 *)(s + 0x150) + 0x318);
            } else {
                game_uso_func_00002FC8(*(int **)(ob[0] + 0xB4), 0, *(s32 *)(s + 0x150) + 0x318);
            }
            *(s32 *)(s + 0x270) = 0;
            *(s32 *)(s + 0x26C) = 0;
            tv = *(char **)(s + 0x150); tv += 0xA58; *(s32 *)tv = *(s32 *)tv & ~0x2000;
        }
    }
    if ((*(s32 *)(seg + 0x0) == 1) && (*(s32 *)(seg + 0x10) == 1)) {
        tmp = 0; /* the advance flag shares tmp */
        st = *(s32 *)(s + 0x26C);
        if (st == 3) {
            tv = *(char **)(s + 0x274);
            if (!(*(s32 *)(tv + 0x10) & 0x80)) {
                tmp = 1;
                *(s32 *)(s + 0x26C) = *(s32 *)(s + 0x270);
            }
        } else {
            tv = *(char **)(s + 0x274);
            if (*(s32 *)(tv + 0x10) & 0x80) {
                *(s32 *)(s + 0x270) = st;
                *(s32 *)(s + 0x26C) = 3;
                tmp = 1;
            } else if (*(s32 *)(tv + 0x18) & 0x40) {
                tmp = st + 1;
                *(s32 *)(s + 0x26C) = tmp;
                if (tmp >= 3) {
                    *(s32 *)(s + 0x26C) = 0;
                }
                tmp = 1;
            }
        }
        if (tmp != 0) {
            /* 4 dense labels: the native 4-entry table (RoData +0x50) */
            switch (*(s32 *)(s + 0x26C)) {
            case 0:
                game_uso_func_00002714(*(int **)(s + 0x154), 1, *(s32 *)(s + 0x150) + 0x318);
                game_uso_func_00002FC8(*(int **)(ob[0] + 0xB4), 1, *(s32 *)(s + 0x150) + 0x318);
                tv = *(char **)(s + 0x150);
                tv += 0xA58; *(s32 *)tv = *(s32 *)tv & ~0x2000;
                break;
            case 1:
                game_uso_func_00002714(*(int **)(s + 0x154), 2, *(s32 *)(s + 0x150) + 0x318);
                game_uso_func_00002FC8(*(int **)(ob[0] + 0xB4), 1, *(s32 *)(s + 0x150) + 0x318);
                tv = *(char **)(s + 0x150);
                tv += 0xA58; *(s32 *)tv = *(s32 *)tv & ~0x2000;
                break;
            case 2:
                game_uso_func_00002714(*(int **)(s + 0x154), 3, *(s32 *)(s + 0x150) + 0x318);
                game_uso_func_00002FC8(*(int **)(ob[0] + 0xB4), 2, *(s32 *)(s + 0x150) + 0x318);
                tv = *(char **)(s + 0x150);
                tv += 0xA58; *(s32 *)tv = *(s32 *)tv | 0x2000;
                break;
            case 3:
                game_uso_func_00002714(*(int **)(s + 0x154), 3, *(s32 *)(s + 0x150) + 0x318);
                game_uso_func_00002FC8(*(int **)(ob[0] + 0xB4), 3, *(s32 *)(s + 0x150) + 0x318);
                tv = *(char **)(s + 0x150);
                tv += 0xA58; *(s32 *)tv = *(s32 *)tv | 0x2000;
                break;
            }
        }
    }
    f1 = 1.0;
    game_uso_func_00000000(state,
        (f1 * (f32) *(s32 *)(s + 0x260)) / (f32) *(s32 *)((char *)kf + 0x28));
    ncur = *(s32 *)(seg + 0x10);
    switch (ncur) {
    case 1:
        pos1.x = *(f32 *)(s + 0xA0);
        pos1.y = *(f32 *)(s + 0xA4);
        pos1.z = *(f32 *)(s + 0xA8);
        break;
    case 2:
    case 3:
        if (*(s32 *)(seg + 0x14) == 0) {
            *(Tri3i *)&stage = *(Tri3i *)((char *)kf + 0xC);
            pos1.x = stage.x; pos1.y = stage.y; pos1.z = stage.z;
        } else {
            c1[0] = (Vec3 *)(((s32 *)(seg + 0x34))[*(s32 *)(s + 0x25C)] + 0xC);
            c1[1] = (Vec3 *)(((s32 *)(seg + 0x38))[*(s32 *)(s + 0x25C)] + 0xC);
            c1[2] = (Vec3 *)(((s32 *)(seg + 0x3C))[*(s32 *)(s + 0x25C)] + 0xC);
            c1[3] = (Vec3 *)(((s32 *)(seg + 0x40))[*(s32 *)(s + 0x25C)] + 0xC);
            game_uso_func_000000E0(&pos1, c1, state);
        }
        break;
    }
    ncur = *(s32 *)(seg + 0x0);
    switch (ncur) {
    case 1:
        bv = *(char **)(s + 0xF4);
        pos2.x = *(f32 *)(bv + 0xA0);
        bv += 0x70;
        pos2.y = *(f32 *)(bv + 0x34);
        pos2.z = *(f32 *)(bv + 0x38);
        break;
    case 2:
    case 3:
        if (*(s32 *)(seg + 0x4) == 0) {
            *(Tri3i *)&stage = *(Tri3i *)((char *)kf + 0x0);
            pos2.x = stage.x; pos2.y = stage.y; pos2.z = stage.z;
        } else {
            c2[0] = (Vec3 *)((s32 *)(seg + 0x34))[*(s32 *)(s + 0x25C)];
            c2[1] = (Vec3 *)((s32 *)(seg + 0x38))[*(s32 *)(s + 0x25C)];
            c2[2] = (Vec3 *)((s32 *)(seg + 0x3C))[*(s32 *)(s + 0x25C)];
            c2[3] = (Vec3 *)((s32 *)(seg + 0x40))[*(s32 *)(s + 0x25C)];
            game_uso_func_000000E0(&pos2, c2, state);
        }
        break;
    }
    if ((*(s32 *)(seg + 0x0) == 1) && (*(s32 *)(seg + 0x10) == 1)) {
        fresh.x = 0.0f; fresh.y = 1.0f; fresh.z = 0.0f;
        *(Tri3i *)&stage = *(Tri3i *)&fresh;
        up.x = stage.x; up.y = stage.y; up.z = stage.z;
    } else if (*(s32 *)(seg + 0x20) == 0) {
        *(Tri3i *)&stage = *(Tri3i *)((char *)kf + 0x18);
        up.x = stage.x; up.y = stage.y; up.z = stage.z;
    } else {
        c3[0] = (Vec3 *)(((s32 *)(seg + 0x34))[*(s32 *)(s + 0x25C)] + 0x18);
        c3[1] = (Vec3 *)(((s32 *)(seg + 0x38))[*(s32 *)(s + 0x25C)] + 0x18);
        c3[2] = (Vec3 *)(((s32 *)(seg + 0x3C))[*(s32 *)(s + 0x25C)] + 0x18);
        c3[3] = (Vec3 *)(((s32 *)(seg + 0x40))[*(s32 *)(s + 0x25C)] + 0x18);
        game_uso_func_000000E0(&up, c3, state);
    }
    selv = *(s32 *)(seg + 0x28);
    if (selv == 1) {
        *(f32 *)(s + 0x14C) = *(f32 *)((char *)kf + 0x24);
    } else if (selv == 2) {
        w[0] = *(f32 *)(((s32 *)(seg + 0x34))[*(s32 *)(s + 0x25C)] + 0x24);
        w[1] = *(f32 *)(((s32 *)(seg + 0x38))[*(s32 *)(s + 0x25C)] + 0x24);
        w[2] = *(f32 *)(((s32 *)(seg + 0x3C))[*(s32 *)(s + 0x25C)] + 0x24);
        w[3] = *(f32 *)(((s32 *)(seg + 0x40))[*(s32 *)(s + 0x25C)] + 0x24);
        *(f32 *)(s + 0x14C) = game_uso_func_000000A0(w, state);
    }
    if (*(s32 *)(s + 0x268) == 0) {
        selv = *(s32 *)(seg + 0x30);
        if (selv == 1) {
            *(Tri3i *)&stage = *(Tri3i *)((char *)kf + 0x2C);
            *(f32 *)(s + 0x134) = stage.x;
            *(f32 *)(s + 0x138) = stage.y;
            *(f32 *)(s + 0x13C) = stage.z;
        } else if (selv == 2) {
            c4[0] = (Vec3 *)(((s32 *)(seg + 0x34))[*(s32 *)(s + 0x25C)] + 0x2C);
            c4[1] = (Vec3 *)(((s32 *)(seg + 0x38))[*(s32 *)(s + 0x25C)] + 0x2C);
            c4[2] = (Vec3 *)(((s32 *)(seg + 0x3C))[*(s32 *)(s + 0x25C)] + 0x2C);
            c4[3] = (Vec3 *)(((s32 *)(seg + 0x40))[*(s32 *)(s + 0x25C)] + 0x2C);
            game_uso_func_000000E0((Vec3 *)(s + 0x134), c4, state);
        }
    }
    *(Tri3i *)&vC = *(Tri3i *)game_uso_func_000023D4(&stage, *(char **)(s + 0x154));
    tv = *(char **)(s + 0x150); tv += 0xB4;
    { f1 = ((f32 *)tv)[0] + vC.x; fy = ((f32 *)tv)[1] + vC.y; fz = ((f32 *)tv)[2] + vC.z; v30.z = fz; v30.y = fy; v30.x = f1; }
    if (arg0) {}
    *(Tri3i *)&v3C = *(Tri3i *)&v30;
    *(Tri3i *)&vD = *(Tri3i *)&v3C;
    game_uso_func_0000052C((int)kf, &pos2, &pos1, (int *)seg, &vD);
    if (*(s32 *)(seg + 0x0) != 1) {
        tv = *(char **)(s + 0xF4);
        *(f32 *)(tv + 0x60) = pos2.x;
        tv += 0x30;
        *(f32 *)(tv + 0x34) = pos2.y;
        *(f32 *)(tv + 0x38) = pos2.z;
        tv = *(char **)(s + 0xF4);
        *(B3CBlk40 *)(tv + 0x70) = *(B3CBlk40 *)(tv + 0x30);
    }
    if (*(s32 *)(seg + 0x10) != 1) {
        *(f32 *)(s + 0x60) = pos1.x;
        *(f32 *)(s + 0x64) = pos1.y;
        *(f32 *)(s + 0x68) = pos1.z;
        *(B3CBlk40 *)(s + 0x70) = *(B3CBlk40 *)(s + 0x30);
    }
    *(Tri3i *)&stage = *(Tri3i *)&up;
    *(f32 *)(s + 0x140) = stage.x;
    *(f32 *)(s + 0x144) = stage.y;
    *(f32 *)(s + 0x148) = stage.z;
    nsub = *(s32 *)(s + 0x260); nsub += 1;
    ncur = *(s32 *)(s + 0x25C);
    nseg = *(s32 *)(s + 0x254);
    if (!(nsub < *(s32 *)((char *)kf + 0x28))) {
        tmp = *(s32 *)(seg + 0x34);
        ncur += 1;
        nsub = 0;
        if (tmp < ncur) goto adv; else if (ncur == tmp) { if (game_uso_func_00000674((int *)seg)) { adv: ncur = 1; nseg += 1; } }
    }
    if (nseg < tbl[9]) {
        *(s32 *)(s + 0x260) = nsub;
        *(s32 *)(s + 0x254) = nseg;
        *(s32 *)(s + 0x25C) = ncur;
    }
}

}
