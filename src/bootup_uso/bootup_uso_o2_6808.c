/* IDO 7.1 -O2 (cfe) donor for game_createMap -- bootup scene/screen constructor
 * (0x948 / 594 words): record-stream dispatch loop (13-way jumptable), then the
 * widget-tree build / attach tail. EXACT (agent-g run 223, 99.98 -> 100).
 *
 * The loop interprets a course descriptor (WorldCourse.desc, include/game/course.h).
 * The case labels are the real op values of the ROM jump table (bootup RoData 0x5B4;
 * agent-g run 480). Until run 480 they were numbered in source order, which compiled
 * to the same .text because the donor's own table is replaced by the pinned ROM one.
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into bootup_uso.c.o (the 2DA4 / E270
 * recipe) over an empty placeholder: the switch's donor-local .rodata reloc is
 * renamed game_createMap_rodata and pinned to 0x5B4 (the target's
 * func_0000057C+0x38 table ref). Reloc-free target lui 0 / lw 0 globals are
 * base-0 pins; the six flag RMWs on func_00000188+0x3C are base-0x188 pins.
 *
 * Frame levers (run 223):
 *  - NO cfe temporaries. A call result inside a compound expression (the old
 *    `func_00000000(0) + 0 * (int)(*arg1)` fold) and a struct-valued comma
 *    argument (`(t70.w = X, t70)`) each make cfe allocate a temp word UNDER the
 *    lowest named home (shared by all such sites), which sat between v4C and
 *    the op spill and pushed the frame to 248 once the homes were right.
 *  - `int gE = 0;` after gD: the target has one more named home above the
 *    struct ring t78..t6C (dead, keeps its home).
 *  - t70 / t6C: the target's arg-slot store BEFORE the jal with the home store
 *    in its delay slot comes from evaluating the member store inside an
 *    earlier argument, `va + 0 * (t70.w = X)` (folded zero-code operand);
 *    a plain statement before the call gives home-store-first. */
#include "common.h"
#include "game/course.h"

typedef struct { int w; } S4_55A0;
typedef struct { f32 x, y, z; } V3_6808;

extern int func_00000000();
extern int import_8005EE6C[];         /* mode words: [0], [1] */
extern int import_8005EE6C_6808r[];   /* read alias (same USO import) */
extern char *import_8001FFC0;         /* root object slot */
extern int import_800200FC[];         /* settings: [0x3C/4], [0x68/4] */
extern int import_8001FFF0;
extern char *D_bu_7770;
extern char *D_bu_776C[];
extern int D_bu_1EBDC[];
extern char D_00007EB8, D_00007EC4, D_00007ECC, D_00007ED8, D_00007EE8, D_00007EEC, D_00007EF0, D_00007EF4,
    D_00007EF8, D_00007EFC, D_00007F08, D_00007F0C, D_00007F18, D_00007F28, D_00007F34, D_00007F3C,
    D_00007F44, D_00007F50, D_00007F58, D_00007F5C, D_00007F64, D_00007F68, D_00007F70, D_00007F78;
extern int import_8005C108_6808_0[], import_8005C108_6808_1[], import_8005C108_6808_2[],
    import_8005C108_6808_3[], import_8005C108_6808_4[], import_8005C108_6808_5[];
extern int import_8005EE64_6808_0, import_8005EE64_6808_1, import_8005EE64_6808_2, import_8005EE64_6808_3,
    import_8005EE64_6808_4, import_8005EE64_6808_5, import_8005EE64_6808_6, import_8005EE64_6808_7;

#define ATTACH6808(list, x) { \
    L = (list); \
    func_00000000(L + 0x10, x); \
    if (*(int *)(x + 0x14) != 0) *(int *)(x + 0x4) = 1; \
    *(int *)(x + 0x14) = (int)L; }

void *game_createMap(WorldRegistry *obj, char *arg1, s32 arg2) {
    int g0, g00;
    S4_55A0 e4, e0;
    int itemsFile;
    char *g1;
    int markersFile, shapeImport;
    char *g2;
    f32 startX, startY, startZ;
    int hasMapParms, mapAngle, mapEnd, mapAdd;
    char *g3;
    int count;
    f32 goalX, goalY, goalZ;
    char *g4;
    char *va, *vb;
    char *L, *t;
    int *rec;
    char **gD;
    int gE = 0;
    S4_55A0 t78, t74, t70, t6C;
    int *p;
    int envArg;
    char *o;
    char *res;
    int op;
    V3_6808 v4C;

    itemsFile = 0;
    markersFile = 0;
    p = ((WorldCourse *)arg1)->desc;
    hasMapParms = 0;
    mapAngle = 40;
    mapEnd = 20;
    mapAdd = 1;
    startX = 0.0f;
    import_8005EE6C[1] = import_8005EE6C_6808r[1] | 0x20000;
    startY = 0.0f;
    startZ = 0.0f;
    func_00000000(&D_00007EB8, 0);
    t = (char *)func_00000000(0);
    import_8001FFC0 = t;
    ATTACH6808((char *)obj->bgRoot, t);
    goalY = 0.0f;
    goalX = 0.0f;
    goalZ = 1.0f;
    D_bu_7770 = (char *)func_00000000(0, &D_00007EC4, -1);
    envArg = 0;
    do {
        count = COURSE_CMD_ARGC(*p);
        t = (char *)p++;
        switch (op = COURSE_CMD_OP(*(int *)t)) {
        case COURSE_OP_END:
            break;
        case COURSE_OP_SUN: {
            func_00000000(&D_00007ECC, 0);
            o = (char *)func_00000000(0);
            t = (char *)func_00000000(0, &D_00007ED8, o);
            func_00000000(o, t);
            t78.w = *(int *)&D_00007EE8;
            func_00000000(import_8001FFC0, o, t78);
            t74.w = *(int *)&D_00007EEC;
            func_00000000(import_8001FFC0, t, t74);
            ATTACH6808((char *)obj->ogRoot, o);
            func_00000000(o, obj->light1);
            va = (char *)obj->light0;
            func_00000000(import_8001FFC0, va + 0 * (t70.w = *(int *)&D_00007EF0), t70);
            vb = (char *)obj->light1;
            func_00000000(import_8001FFC0, vb + 0 * (t6C.w = *(int *)&D_00007EF4), t6C);
            func_00000000();
            break;
        }
        case COURSE_OP_ENV:
            envArg = p[1];
            func_00000000(import_8001FFC0, 1, p[0]);
            break;
        case COURSE_OP_ENV_LT2:
            if (import_800200FC[0x68 / 4] < 2) {
                envArg = p[1];
                func_00000000(import_8001FFC0, 1, p[0]);
            }
            break;
        case COURSE_OP_ENV_EQ2:
            if (import_800200FC[0x68 / 4] == 2) {
                func_00000000(import_8001FFC0, 1, p[0]);
                envArg = p[1];
            }
            break;
        case COURSE_OP_ENV_EQ3:
            if (import_800200FC[0x68 / 4] == 3) {
                func_00000000(import_8001FFC0, 1, p[0]);
                envArg = p[1];
            }
            break;
        case COURSE_OP_MAP:
            hasMapParms = 1;
            mapAngle = p[0];
            mapEnd = p[1];
            mapAdd = p[2];
            break;
        case COURSE_OP_BG:
            if (arg2 != 6) {
                func_00000000(&D_00007EF8, 0);
                func_00000000(import_800200FC, p[0]);
                o = (char *)func_00000000(p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                t = o;
                ATTACH6808((char *)obj->bgRoot, t);
                *(char **)(import_8001FFC0 + 0x3C) = o;
                func_00000000();
            }
            break;
        case COURSE_OP_SHAPE:
            func_00000000(&D_00007EFC, 0);
            func_00000000(import_800200FC, p[0]);
            shapeImport = p[1];
            func_00000000();
            break;
        case COURSE_OP_ITEMS:
            itemsFile = p[0];
            break;
        case COURSE_OP_MARKERS:
            markersFile = p[0];
            break;
        case COURSE_OP_START:
            startX = p[0];
            startY = p[1];
            startZ = p[2];
            break;
        case COURSE_OP_GOAL:
            goalX = p[0];
            goalY = p[1];
            goalZ = p[2];
            break;
        }
        p += count;
    } while (op != COURSE_OP_END);
    func_00000000(); /* no bbsplit: the dead return blocks v0, the settings word takes v1 */
    if (import_800200FC[0x3C / 4] == 6 || import_800200FC[0x3C / 4] == 11) {
        mapEnd = 5;
        mapAdd = 2;
    }
    gD = (char **)(int)D_bu_776C; /* int-cast base keeps &D_bu_776C held in s4 */
    *gD = 0; if (1) {}
    D_bu_1EBDC[0] = 6;
    func_00000000(D_bu_7770, arg2, envArg);
    {
        t = D_bu_7770;
        ATTACH6808((char *)obj->egRoot, t);
    }
    func_00000000(&D_00007F08, 0);
    if (hasMapParms != 0) {
        import_8005C108_6808_0[0x3C / 4] = import_8005C108_6808_1[0x3C / 4] | 8;
    } else {
        import_8005C108_6808_2[0x3C / 4] = import_8005C108_6808_3[0x3C / 4] & ~8;
    }
    /* flag RMWs on the dummy-home local g00 inside do { } while (0): each RMW gets its own
     * block, so the block-granular v0 conflict with the preceding call's return and with the
     * final return copy is gone (docs/IDO_CODEGEN.md kit CCXXI) */
    D_bu_1EBDC[0] = 6; do { g00 = import_8005EE64_6808_1; g00 &= ~8; g00 |= 0x100; import_8005EE64_6808_0 = g00; } while (0); if (1) {} import_8005EE6C[1] = 0xA8002;
    import_8005EE6C[0] = 0;
    func_00000000(&D_00007F0C, 0);
    p = (int *)func_00000000(0, &D_00007F18, 0, 0);
    func_00000000();
    func_00000000(&D_00007F28, 0);
    func_00000000(((char *)p), shapeImport);
    func_00000000();
    func_00000000(&D_00007F34, 0);
    g0 = *(int *)(((char *)p) + 0x40); *(int *)(((char *)p) + 0x50) = func_00000000(g0 * 4);
    func_00000000();
    func_00000000(&D_00007F3C, 0);
    func_00000000(((char *)p));
    func_00000000();
    do { g00 = import_8005EE64_6808_3; g00 &= ~0x100; import_8005EE64_6808_2 = g00; } while (0);
    g00 = (int)p; if (1) { g00 += 0x30; } *(int *)g00 |= 0x40;
    import_8005C108_6808_4[0x3C / 4] = import_8005C108_6808_5[0x3C / 4] & ~8;
    func_00000000();
    func_00000000(&D_00007F44, 0);
    {
        o = (char *)func_00000000(0, ((char *)p), obj->light0, obj->light1);
        t = o;
        ATTACH6808((char *)obj->bgRoot, t);
        func_00000000(o);
    }
    func_00000000();
    D_bu_1EBDC[0] = 1;
    res = (char *)func_00000000(0, ((char *)p), mapAngle, mapEnd, mapAdd);
    rec = (int *)res; if (1) { rec += 0x18 / 4; } *rec &= ~0x40;
    import_8001FFF0 = func_00000000(0, ((char *)p));
    func_00000000(&D_00007F50, 0);
    if (itemsFile != 0) {
        func_00000000(D_bu_7770, 1, itemsFile);
    }
    func_00000000();
    ((MapProp *)res)->start[0] = startX;
    ((MapProp *)res)->start[1] = startY;
    ((MapProp *)res)->start[2] = startZ;
    ((MapProp *)res)->goal[0] = goalX;
    ((MapProp *)res)->goal[1] = goalY;
    ((MapProp *)res)->goal[2] = goalZ;
    {
        e4.w = *(int *)&D_00007F58;
        va = (char *)func_00000000(D_bu_7770, e4, 0);
        if (va != 0) {
            v4C = *(V3_6808 *)(va + 0x30);
            ((MapProp *)res)->start[0] = v4C.x;
            ((MapProp *)res)->start[1] = v4C.y;
            ((MapProp *)res)->start[2] = v4C.z;
        } else {
            func_00000000(&D_00007F5C);
        }
    }
    {
        e0.w = *(int *)&D_00007F64;
        vb = (char *)func_00000000(D_bu_7770, e0, 0);
        if (vb != 0) {
            v4C = *(V3_6808 *)(vb + 0x30);
            ((MapProp *)res)->goal[0] = v4C.x;
            ((MapProp *)res)->goal[1] = v4C.y;
            ((MapProp *)res)->goal[2] = v4C.z;
        } else {
            func_00000000(&D_00007F68);
        }
    }
    ATTACH6808((char *)obj->bgRoot, res);
    D_bu_1EBDC[0] = 6;
    func_00000000(&D_00007F70, 0);
    *gD = (char *)func_00000000(0, &D_00007F78, -1);
    do { g00 = import_8005EE64_6808_5; g00 |= 8; import_8005EE64_6808_4 = g00; } while (0);
    func_00000000(*gD);
    if (markersFile != 0) {
        func_00000000(*gD, 1, markersFile);
    }
    if (arg2 == 1) {
        t = *gD;
        ATTACH6808((char *)obj->fgRoot, t);
    }
    func_00000000();
    do { g00 = import_8005EE64_6808_7; g00 &= ~8; import_8005EE64_6808_6 = g00; } while (0);
    return res;
}
