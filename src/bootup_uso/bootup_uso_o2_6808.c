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
 * snd_soundObjCreate_0000057C+0x38 table ref). Reloc-free target lui 0 / lw 0 globals are
 * base-0 pins; the six flag RMWs on snd_seqEndDbg_00000188+0x3C are base-0x188 pins.
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
#include "uso_call.h"
#include "game/input.h"
#include "game/envobj.h"
#include "gdl/polyshape.h"

typedef struct { int w; } S4_55A0;
typedef struct { f32 x, y, z; } V3_6808;

extern int func_00000000();
extern int gPolyGeomMode_3EEC8_8005EE6C[];         /* mode words: [0], [1] */
extern int gPolyGeomMode_3EEC8_8005EE6C_6808r[];   /* read alias (same USO import) */
extern char *gEnvSettings_6808;         /* root object slot */
extern int gInputMgr_800200FC[];         /* settings: [0x3C/4], [0x68/4] */
extern int import_8001FFF0;
extern char *D_bu_7770;
extern char *D_bu_776C[];
extern int D_bu_1EBDC[];
extern char sStr_Geometry_7EB8, sStr_ItemMgr_7EC4, sStr_Sun_Flare_7ECC, sStr_rom_flare5_bin_7ED8, sStr_sun_7EE8, sStr_len_7EEC, sStr_lt0_7EF0, sStr_lt1_7EF4,
    gStrBG_7EF8, sStr_CourseShape_7EFC, sStr_Map_7F08, sStr_MorphShape_7F0C, sStr_morphpolyshape_7F18, sStr_ParseCmds_7F28, sStr_Colours_7F34, sStr_Init_7F3C,
    sStr_MapLighter_7F44, sStr_ItemMgr_7F50, sStr_st0_7F58, sStr_no_st0_7F5C, sStr_gol_7F64, sStr_no_gol_7F68, sStr_Markers_7F70, sStr_MarkerMgr_7F78;
extern int gGfxLib_8005C108_6808_0[], gGfxLib_8005C108_6808_1[], gGfxLib_8005C108_6808_2[],
    gGfxLib_8005C108_6808_3[], gGfxLib_8005C108_6808_4[], gGfxLib_8005C108_6808_5[];
extern int gPolyFlags_3EEC4_8005EE64_6808_0, gPolyFlags_3EEC4_8005EE64_6808_1, gPolyFlags_3EEC4_8005EE64_6808_2, gPolyFlags_3EEC4_8005EE64_6808_3,
    gPolyFlags_3EEC4_8005EE64_6808_4, gPolyFlags_3EEC4_8005EE64_6808_5, gPolyFlags_3EEC4_8005EE64_6808_6, gPolyFlags_3EEC4_8005EE64_6808_7;

#define ATTACH6808(attach, list, x) { \
    L = (list); \
    attach(L + 0x10, x); \
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
    gPolyGeomMode_3EEC8_8005EE6C[1] = gPolyGeomMode_3EEC8_8005EE6C_6808r[1] | 0x20000;
    startY = 0.0f;
    startZ = 0.0f;
    FCALL(memtrack_begin)(&sStr_Geometry_7EB8, 0);
    t = (char *)FCALL(EnvSettings_ctor)(0);
    gEnvSettings_6808 = t;
    ATTACH6808(FCALL(list_append), (char *)obj->bgRoot, t);
    goalY = 0.0f;
    goalX = 0.0f;
    goalZ = 1.0f;
    D_bu_7770 = (char *)FCALL(ItemMgr_ctor)(0, &sStr_ItemMgr_7EC4, -1);
    envArg = 0;
    do {
        count = COURSE_CMD_ARGC(*p);
        t = (char *)p++;
        switch (op = COURSE_CMD_OP(*(int *)t)) {
        case COURSE_OP_END:
            break;
        case COURSE_OP_SUN: {
            FCALL(memtrack_begin)(&sStr_Sun_Flare_7ECC, 0);
            o = (char *)FCALL(Sun_ctor)(0);
            t = (char *)FCALL(LensFlare_create)(0, &sStr_rom_flare5_bin_7ED8, o);
            FCALL(MNode_addChild)(o, t);
            t78.w = *(int *)&sStr_sun_7EE8;
            FCALL(map_addEntry_00007204)(gEnvSettings_6808, o, t78);
            t74.w = *(int *)&sStr_len_7EEC;
            FCALL(map_addEntry_00007204)(gEnvSettings_6808, t, t74);
            ATTACH6808(FCALL(list_append), (char *)obj->ogRoot, o);
            FCALL(MNode_addChild)(o, obj->light1);
            va = (char *)obj->light0;
            FCALL(map_addEntry_00007204)(gEnvSettings_6808, va + 0 * (t70.w = *(int *)&sStr_lt0_7EF0), t70);
            vb = (char *)obj->light1;
            FCALL(map_addEntry_00007204)(gEnvSettings_6808, vb + 0 * (t6C.w = *(int *)&sStr_lt1_7EF4), t6C);
            FCALL(memtrack_end)();
            break;
        }
        case COURSE_OP_ENV:
            envArg = p[1];
            FCALL(Node_Load)(gEnvSettings_6808, 1, p[0]);
            break;
        case COURSE_OP_ENV_LT2:
            if (INPUT_MGR(gInputMgr_800200FC).level < 2) {
                envArg = p[1];
                FCALL(Node_Load)(gEnvSettings_6808, 1, p[0]);
            }
            break;
        case COURSE_OP_ENV_EQ2:
            if (INPUT_MGR(gInputMgr_800200FC).level == 2) {
                FCALL(Node_Load)(gEnvSettings_6808, 1, p[0]);
                envArg = p[1];
            }
            break;
        case COURSE_OP_ENV_EQ3:
            if (INPUT_MGR(gInputMgr_800200FC).level == 3) {
                FCALL(Node_Load)(gEnvSettings_6808, 1, p[0]);
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
                FCALL(memtrack_begin)(&gStrBG_7EF8, 0);
                FCALL(game_loadUso)(gInputMgr_800200FC, p[0]);
                o = (char *)FCALL(game_createBGBox)(p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                t = o;
                ATTACH6808(FCALL(list_append), (char *)obj->bgRoot, t);
                ENVSET(gEnvSettings_6808)->bgBox = (Node *)o;
                FCALL(memtrack_end)();
            }
            break;
        case COURSE_OP_SHAPE:
            FCALL(memtrack_begin)(&sStr_CourseShape_7EFC, 0);
            FCALL(game_loadUso)(gInputMgr_800200FC, p[0]);
            shapeImport = p[1];
            FCALL(memtrack_end)();
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
    FCALL(memtrack_end)(); /* no bbsplit: the dead return blocks v0, the settings word takes v1 */
    if (INPUT_MGR(gInputMgr_800200FC).gameMode == 6 || INPUT_MGR(gInputMgr_800200FC).gameMode == 11) {
        mapEnd = 5;
        mapAdd = 2;
    }
    gD = (char **)(int)D_bu_776C; /* int-cast base keeps &D_bu_776C held in s4 */
    *gD = 0; if (1) {}
    D_bu_1EBDC[0] = 6;
    FCALL(item_registerTemplates)(D_bu_7770, arg2, envArg);
    {
        t = D_bu_7770;
        ATTACH6808(FCALL(list_append), (char *)obj->egRoot, t);
    }
    FCALL(memtrack_begin)(&sStr_Map_7F08, 0);
    if (hasMapParms != 0) {
        gGfxLib_8005C108_6808_0[0x3C / 4] = gGfxLib_8005C108_6808_1[0x3C / 4] | 8;
    } else {
        gGfxLib_8005C108_6808_2[0x3C / 4] = gGfxLib_8005C108_6808_3[0x3C / 4] & ~8;
    }
    /* flag RMWs on the dummy-home local g00 inside do { } while (0): each RMW gets its own
     * block, so the block-granular v0 conflict with the preceding call's return and with the
     * final return copy is gone (docs/IDO_CODEGEN.md kit CCXXI) */
    D_bu_1EBDC[0] = 6; do { g00 = gPolyFlags_3EEC4_8005EE64_6808_1; g00 &= ~8; g00 |= 0x100; gPolyFlags_3EEC4_8005EE64_6808_0 = g00; } while (0); if (1) {} gPolyGeomMode_3EEC8_8005EE6C[1] = 0xA8002;
    gPolyGeomMode_3EEC8_8005EE6C[0] = 0;
    FCALL(memtrack_begin)(&sStr_MorphShape_7F0C, 0);
    p = (int *)FCALL(MorphPoly_ctor)(0, &sStr_morphpolyshape_7F18, 0, 0);
    FCALL(memtrack_end)();
    FCALL(memtrack_begin)(&sStr_ParseCmds_7F28, 0);
    FCALL(BasePolyShape_parse)(((char *)p), shapeImport);
    FCALL(memtrack_end)();
    FCALL(memtrack_begin)(&sStr_Colours_7F34, 0);
    g0 = ((BasePolyShape *)p)->nVerts; ((BasePolyShape *)p)->vtxColours = (u32 *)FCALL(mem_new)(g0 * 4);
    FCALL(memtrack_end)();
    FCALL(memtrack_begin)(&sStr_Init_7F3C, 0);
    FCALL(MorphPoly_init)(((char *)p));
    FCALL(memtrack_end)();
    do { g00 = gPolyFlags_3EEC4_8005EE64_6808_3; g00 &= ~0x100; gPolyFlags_3EEC4_8005EE64_6808_2 = g00; } while (0);
    g00 = (int)p; if (1) { g00 += 0x30; } *(int *)g00 |= 0x40;
    gGfxLib_8005C108_6808_4[0x3C / 4] = gGfxLib_8005C108_6808_5[0x3C / 4] & ~8;
    FCALL(memtrack_end)();
    FCALL(memtrack_begin)(&sStr_MapLighter_7F44, 0);
    {
        o = (char *)func_00000000(0, ((char *)p), obj->light0, obj->light1);
        t = o;
        ATTACH6808(FCALL(list_append), (char *)obj->bgRoot, t);
        func_00000000(o);
    }
    FCALL(memtrack_end)();
    D_bu_1EBDC[0] = 1;
    res = (char *)FCALL(MapProp_ctor)(0, ((char *)p), mapAngle, mapEnd, mapAdd);
    rec = (int *)res; if (1) { rec += 0x18 / 4; } *rec &= ~0x40;
    import_8001FFF0 = FCALL(CollShape_ctorPoly)(0, ((char *)p));
    FCALL(memtrack_begin)(&sStr_ItemMgr_7F50, 0);
    if (itemsFile != 0) {
        FCALL(Node_Load)(D_bu_7770, 1, itemsFile);
    }
    FCALL(memtrack_end)();
    ((MapProp *)res)->start[0] = startX;
    ((MapProp *)res)->start[1] = startY;
    ((MapProp *)res)->start[2] = startZ;
    ((MapProp *)res)->goal[0] = goalX;
    ((MapProp *)res)->goal[1] = goalY;
    ((MapProp *)res)->goal[2] = goalZ;
    {
        e4.w = *(int *)&sStr_st0_7F58;
        va = (char *)FCALL(ItemMgr_firstItemOfType)(D_bu_7770, e4, 0);
        if (va != 0) {
            v4C = *(V3_6808 *)((ColourSubItem *)va)->pos; /* the st0 / gol item's position */
            ((MapProp *)res)->start[0] = v4C.x;
            ((MapProp *)res)->start[1] = v4C.y;
            ((MapProp *)res)->start[2] = v4C.z;
        } else {
            FCALL(sys_fatal)(&sStr_no_st0_7F5C);
        }
    }
    {
        e0.w = *(int *)&sStr_gol_7F64;
        vb = (char *)FCALL(ItemMgr_firstItemOfType)(D_bu_7770, e0, 0);
        if (vb != 0) {
            v4C = *(V3_6808 *)((ColourSubItem *)vb)->pos; /* the st0 / gol item's position */
            ((MapProp *)res)->goal[0] = v4C.x;
            ((MapProp *)res)->goal[1] = v4C.y;
            ((MapProp *)res)->goal[2] = v4C.z;
        } else {
            FCALL(sys_fatal)(&sStr_no_gol_7F68);
        }
    }
    ATTACH6808(FCALL(list_append), (char *)obj->bgRoot, res);
    D_bu_1EBDC[0] = 6;
    FCALL(memtrack_begin)(&sStr_Markers_7F70, 0);
    *gD = (char *)FCALL(ItemMgr_ctor)(0, &sStr_MarkerMgr_7F78, -1);
    do { g00 = gPolyFlags_3EEC4_8005EE64_6808_5; g00 |= 8; gPolyFlags_3EEC4_8005EE64_6808_4 = g00; } while (0);
    FCALL(CourseMarker_registerTemplates)(*gD);
    if (markersFile != 0) {
        FCALL(Node_Load)(*gD, 1, markersFile);
    }
    if (arg2 == 1) {
        t = *gD;
        ATTACH6808(FCALL(list_append), (char *)obj->fgRoot, t);
    }
    FCALL(memtrack_end)();
    do { g00 = gPolyFlags_3EEC4_8005EE64_6808_7; g00 &= ~8; gPolyFlags_3EEC4_8005EE64_6808_6 = g00; } while (0);
    return res;
}
