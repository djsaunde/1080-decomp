#ifndef GAME_COURSE_H
#define GAME_COURSE_H

/*
 * The course / world data model (agent-g run 480). docs/1080_COURSE.md is the port-author overview.
 *
 * A race is laid out from three kinds of data:
 *   1. the WORLD REGISTRY (bootup WorldRegistry_create_00001E08): a list of WorldCourse records, one per
 *      course ("Test Grid", "One Make", "HalfPipe", ...), each pointing at a course DESCRIPTOR: a
 *      command stream in bootup Data (0x67F0..0x6E30) that game_createMap interprets;
 *   2. the course's ITEMS (rom/<course>_item.bin, ItemMgr "ItemMgr", bootup Data 0x776C): typed
 *      placed objects. Types are the 3-char names of item_registerTemplates and the template table at
 *      bootup Data 0x7B6C (CourseItemTmpl): st0 (start), gol (goal), cam (camera points), chk / gtl /
 *      gtr (race gates), en1 / en2 (ending points), tr1..tr3 (trees), snw / fog / ...;
 *   3. the course's MARKERS (rom/<course>_mark.bin, ItemMgr "MarkerMgr", bootup Data 0x7768): typed
 *      CourseMarker points (game.uso) that the AI riders steer through: mrk, mk1..mk9, mkl, mkr.
 * Item and marker files are both libgdl ItemMgr streams: per type, a list of ColourItem-derived
 * instances (a Node with a position at +0x30 read as three ints) plus each type's Parms.
 */

/* ---- course descriptor (bootup Data; WorldCourse.desc) ----
 * A stream of commands. Each command is one header word, (argc << 16) | op, followed by argc
 * argument words; COURSE_OP_END (argc 0) ends the stream. Ops are game_createMap's switch values
 * (its jump table, bootup RoData 0x5B4). */
#define COURSE_CMD_ARGC(w) ((w) >> 16)
#define COURSE_CMD_OP(w)   ((w) & 0xFFFF)

#define COURSE_OP_SHAPE    0   /* (uso path, shape import): load the course-shape USO ("rom/mapN_data.uso")
                                  under memtrack "CourseShape"; the import is the course PolyShape */
#define COURSE_OP_ITEMS    1   /* (file): the item file ("rom/<course>_item.bin"), loaded into the ItemMgr */
#define COURSE_OP_MARKERS  2   /* (file): the marker file ("rom/<course>_mark.bin"), loaded into the MarkerMgr */
#define COURSE_OP_START    3   /* (x, y, z): default start position (ints); item "st0" overrides it */
#define COURSE_OP_GOAL     4   /* (x, y, z): default goal position (ints); item "gol" overrides it */
#define COURSE_OP_MAP      5   /* (angle, end, add): MapProp parms (defaults 40, 20, 1; see MapProp) */
#define COURSE_OP_BG       6   /* (uso path, 7 args): background texture USO ("rom/bgNtex.uso") and the
                                  game_createBGBox arguments (0x100, five imports of the bg USO, 0);
                                  skipped in mode 6 */
#define COURSE_OP_SUN      7   /* (2 ints, unused here): sun ("sun"), lens flare ("len", rom/flare5.bin) and
                                  the two lights ("lt0", "lt1") */
#define COURSE_OP_ENV      8   /* (file, arg): environment file ("rom/<course>_env.bin"), loaded into
                                  EnvSettings; arg (0x100) goes to item_registerTemplates */
#define COURSE_OP_ENV_LT2  9   /* (file, arg): COURSE_OP_ENV when settings +0x68 < 2 */
#define COURSE_OP_ENV_EQ2  10  /* (file, arg): COURSE_OP_ENV when settings +0x68 == 2 */
#define COURSE_OP_ENV_EQ3  11  /* (file, arg): COURSE_OP_ENV when settings +0x68 == 3 */
#define COURSE_OP_END      12

/* ---- World registry (bootup, 0x118, node name "World Registry") ----
 * Built once by WorldRegistry_create_00001E08 (GameFlow_create); game_init picks course N with
 * WorldRegistry_getCourse_00002014 and passes it to game_createMap. The same object holds the scene's root
 * nodes (game_setParams_00002060) and the player / window lists. */
typedef struct WorldCourse {
    char name[0x40];            /* 0x00: course name (str_copyEnd) */
    s32 *desc;                  /* 0x40: the course descriptor (COURSE_OP_* stream) */
    struct WorldCourse *next;   /* 0x44: next course (list order = registration order) */
} WorldCourse;                  /* 0x48 */

typedef struct WorldPlayer {     /* 8 bytes (game_addPlayer_000020AC) */
    char *rider;                /* 0x0: the SnowBoarder */
    s32 arg;                    /* 0x4: game_addPlayer arg 3, passed back by game_forEachPlayer_000020D8 */
} WorldPlayer;

typedef struct WorldRegistry {
    char node[0x28];            /* 0x00: Node (name at 0xC = "World Registry") */
    char *vptr;                 /* 0x28: Cfront vtable (bootup Data, D_1E08_d) */
    WorldCourse head;           /* 0x2C: list sentinel; head.next (0x70) = first course */
    WorldCourse *tail;          /* 0x74: last course (WorldRegistry_addCourse_00001F78 appends here) */
    s32 numCourses;             /* 0x78 */
    s32 ogRoot;                 /* 0x7C: Node "og" (game_init -> game_setParams_00002060); createMap
                                   attaches the sun (COURSE_OP_SUN) */
    s32 fgRoot;                 /* 0x80: Node "fg"; createMap attaches the marker manager in mode 1 */
    s32 bgRoot;                 /* 0x84: Node "bg"; createMap attaches EnvSettings, the BG box, the map
                                   lighter and the MapProp */
    s32 egRoot;                 /* 0x88: Node "eg"; createMap attaches the item manager */
    s32 mainGame;               /* 0x8C: Node "MainGame" */
    s32 light0;                 /* 0x90: light passed to the sun / flare entries and the map lighter */
    s32 light1;                 /* 0x94 */
    struct MapProp *map;        /* 0x98: the course map, game_createMap's result (game_init stores it);
                                   every game_setup* passes its start[0] / start[2] (+0xC4 / +0xCC) to the
                                   rider builds (agent-h runs 496, 524) */
    char pad9C[0xC0 - 0x9C];
    s32 numPlayers;             /* 0xC0: game_clearPlayers / game_addPlayer / game_forEachPlayer */
    WorldPlayer players[8];     /* 0xC4: the InputMgr reads players[i].rider->input */
    s32 numWindows;             /* 0x104: game_clearWindows / game_addWindow */
    s32 windows[4];             /* 0x108: camera windows (game_addWindow appends); RaceProcess swaps its
                                   replayCam / savedCam into windows[0] / [1] */
} WorldRegistry;

/* Field access through an untyped `char *` registry pointer (the game_* bodies; agent-h run 496) */
#define WORLDREG(p) ((WorldRegistry *)(p))
/* The course map of an untyped registry pointer: a MapProp whose start / goal hold the course's st0 / gol
 * positions (agent-h run 524) */
#define WORLD_MAP(w) (WORLDREG(w)->map)

/* ---- item templates (bootup Data 0x7B6C: {name, create, flags} until a NULL name) ----
 * hel hem bal ba2 chk gtr gtl cfr snw fog sn2 fo2 sn3 fo3; item_create<Name> builds the ItemType
 * (gate types: 0x4C / 0x50). */
typedef struct CourseItemTmpl {
    char *name;                 /* 3-char type name */
    void *(*create)();          /* item_createChk etc. */
    s32 flags;                  /* chk 2, gtr / gtl 1, cfr 0x20, snw / fog 0x100, sn2 / fo2 0x200, ... */
} CourseItemTmpl;

/* ---- race gates (game_libs RaceTracker_findGates) ----
 * Items of type "chk" (checkpoint) and "gtl" / "gtr" (the left / right gates of a fork) found by
 * ItemMgr_queryType(itemMgr, type, pos, 0) nearest the rider's lastPos (SnowBoarder +0xDC).
 * RaceEntry.gate / gateA / gateB (include/gdl/race.h) point at them. Only the fields findGates /
 * RaceTracker_passCheckpoints read are named (agent-g run 485).
 * The course coordinate is world z: RaceTracker.courseZ / courseLength are the MapProp start.z (0xCC) and
 * goal.z (0xD8) - start.z, RaceEntry.progress is on the same axis, and findGates orders a fork's two gates
 * by `gate->z - rider->dnode.cvqs.trans[2]` (the rider +0xE4 it subtracts; the last-integrate position). passCheckpoints moves
 * entries[0].gate to gate->next once gate->z <= entries[0].progress, then calls a gate test (gate, rider
 * pos, entry); a non-zero result is reported through the tracker's vtable (the +0x60 / +0x64 slot). Note: findGates looks up with
 * entries[0]'s rider and stores the "chk" result in entries[0].gate on every iteration (only the fork
 * pair is per entry), so checkpoints are tracked for the first rider only.
 *
 * The gate class itself is in checkpoint.uso (USO module / file id 50, ROM Info record 0x7F6D1A), which is
 * not split, so only the fields game_libs reads are typed here (agent-g run 488). bootup's item templates
 * item_createChk (0x4C) / item_createGtr / item_createGtl (0x50) first store SNodeItemType_vtbl (Data
 * 0x1EFC0, Sym 414) at +0x28, then a checkpoint.uso export (import 50: chk Sym 418, gtr and gtl share
 * Sym 420) at +0x28 and another at +0x48 (Syms 417 / 419 / 421: the per-type descriptor). */
typedef struct RaceGate {
    char unk00[0x64];
    f32 z;                      /* 0x64: the gate's course coordinate (world z) */
    char unk68[0x84 - 0x68];
    struct RaceGate *next;      /* 0x84: the next gate along the course */
} RaceGate;

/* ---- course markers (game.uso, the AI path) ----
 * Marker types, in the order of the game.uso type-name table at Data 0x548 (SnowAI.targetNode /
 * altNode index it). CourseMarker_registerTemplates registers all twelve in the MarkerMgr. */
#define MARKER_MRK  0           /* "mrk": the generic marker (SnowAI_isAtFinish looks up type 0) */
#define MARKER_MK1  1           /* "mk1".."mk9": numbered path markers 1..9 */
#define MARKER_MK9  9
#define MARKER_MKL  10          /* "mkl": left branch */
#define MARKER_MKR  11          /* "mkr": right branch */
#define MARKER_NUM_TYPES 12

/* CourseMarker.flags bits (the "flg" parm) */
#define MARKER_F_REDIRECT 0x10  /* use the linked marker (+0x2C) instead (SnowAI_pickFork) */
#define MARKER_F_FINISH   0x400 /* the finish (SnowAI_isAtFinish -> SnowAI_enterFinish) */

/* one Parms entry (game_libs Parms_addParm(parm, owner, tag, 1); value at +0x10) */
typedef struct CourseParm {
    char hdr[0xC];              /* 0x00: list link / owner / tag */
    char *vptr;                 /* 0x0C: parm type vtable (float / int / vec) */
    s32 value;                  /* 0x10: the value (f32 for float parms) */
    s32 value2;                 /* 0x14 */
} CourseParm;                   /* 0x18 */

typedef struct CourseFParm {    /* a float parm: CourseParm with an f32 value */
    char hdr[0xC];
    char *vptr;                 /* 0x0C */
    f32 value;                  /* 0x10 */
    s32 value2;                 /* 0x14 */
} CourseFParm;

/* "CourseMarker::Parms" (CourseMarkerTmpl_create): wid (float, default 1), ext (int), flg (int) */
typedef struct CourseMarkerParms {
    s32 name;                   /* 0x00: "CourseMarker::Parms" */
    s32 list;                   /* 0x04 */
    CourseFParm wid;            /* 0x08: marker width (scanSegments accepts a rider up to 250 * wid
                                   behind the marker) */
    CourseParm ext;             /* 0x20: the marker type of the other branch at a fork (SnowAI_steer
                                   loads altNode from it) */
    CourseParm flg;             /* 0x38: MARKER_F_* */
} CourseMarkerParms;            /* 0x50 */

/* The 0x40-byte item that game.uso's three ItemType_create factories build (game_ItemType_create_00003A28 /
 * _0000AC78 / _00011A94, each the create method of an ItemType): a libgdl ColourItem (0x3C: Node + vptr + link
 * + position; ColourItem_ctor) whose vptr is then set to import_8006ED80, plus one word cleared. When the type's
 * unk40 node is set it becomes the item's child. */
typedef struct ColourSubItem {
    char node[0x28];            /* 0x00: Node */
    char *vptr;                 /* 0x28: import_8006ED80 */
    s32 link;                   /* 0x2C: ColourItem link */
    f32 pos[3];                 /* 0x30 */
    s32 unk3C;                  /* 0x3C: 0 after the ctor; no reader in game.uso */
} ColourSubItem;                /* 0x40 */

/* CourseMarker instance (game.uso CourseMarkerTmpl_create: 0x8C, vtbl Data 0x578). The base is the
 * libgdl ColourItem (0x3C): Node + vptr + link + position. */
typedef struct CourseMarker {
    char node[0x28];            /* 0x00: Node */
    char *vptr;                 /* 0x28: Data 0x578 */
    struct CourseMarker *link;  /* 0x2C: ColourItem +0x2C (0 after the ctor): the next marker; a marker and
                                   its link form one path segment (SnowAI_scanSegments); also the marker
                                   used instead when MARKER_F_REDIRECT and the second finish test of
                                   SnowAI_isAtFinish */
    f32 pos[3];                 /* 0x30: position (loaded as ints) */
    CourseMarkerParms parms;    /* 0x3C: wid value = 0x54, ext value = 0x6C, flg value = 0x84 */
} CourseMarker;                 /* 0x8C */

/* ---- MapProp (bootup, 0xDC, MNode "Course"; game_createMap's return value) ----
 * The course map node: owns the course shape and the start / goal positions. hostio group "Map":
 * end (0..127), add (0..127), angle (0..89). MapProp_display turns end / angle / add and the view
 * into two 0..127 values in the gfx context (+0x10C / +0x110) each frame. */
typedef struct MapProp {
    char mnode[0xB4];           /* 0x00: MNode "Course" */
    s32 shape;                  /* 0xB4: the course PolyShape (asserts shp->pGrid != NULL: +0xA0) */
    s32 end;                    /* 0xB8: COURSE_OP_MAP arg 2 [20] */
    s32 angle;                  /* 0xBC: COURSE_OP_MAP arg 1 [40] */
    s32 add;                    /* 0xC0: COURSE_OP_MAP arg 3 [1] */
    f32 start[3];               /* 0xC4: start position (COURSE_OP_START, then item "st0") */
    f32 goal[3];                /* 0xD0: goal position (COURSE_OP_GOAL, then item "gol") */
} MapProp;                      /* 0xDC */

/* ---- Mountain (bootup, hostio group "Mountain") ----
 * A backdrop node drawn around the camera: Mountain_display builds scale(scaleXZ, scaleY, scaleXZ)
 * with its translation = the camera eye (gfx context +0x70 -> +0xA0) * the global scale + (0, posY, 0),
 * so the far mountains never get closer. */
typedef struct Mountain {
    char node[0x2C];            /* 0x00: Node */
    f32 posY;                   /* 0x2C: "Pos.Y" (-2000..2000) */
    f32 scaleXZ;                /* 0x30: "Scale.XZ" (0.01..1) */
    f32 scaleY;                 /* 0x34: "Scale.Y" (0.01..1) */
} Mountain;

#ifndef __cplusplus
#define COURSE_ASSERT(T, f, off) typedef char course_assert_##T##_##off[(int)&((T *)0)->f == (off) ? 1 : -1]
#define COURSE_ASSERT_SIZE(T, n) typedef char course_assert_size_##T[sizeof(T) == (n) ? 1 : -1]
COURSE_ASSERT_SIZE(WorldCourse, 0x48);
COURSE_ASSERT(WorldCourse, desc, 0x40);
COURSE_ASSERT(WorldCourse, next, 0x44);
COURSE_ASSERT_SIZE(WorldRegistry, 0x118);
COURSE_ASSERT(WorldRegistry, head, 0x2C);
COURSE_ASSERT(WorldRegistry, tail, 0x74);
COURSE_ASSERT(WorldRegistry, numCourses, 0x78);
COURSE_ASSERT(WorldRegistry, ogRoot, 0x7C);
COURSE_ASSERT(WorldRegistry, mainGame, 0x8C);
COURSE_ASSERT(WorldRegistry, light1, 0x94);
COURSE_ASSERT(WorldRegistry, map, 0x98);
COURSE_ASSERT(WorldRegistry, players, 0xC4);
COURSE_ASSERT(WorldRegistry, numWindows, 0x104);
COURSE_ASSERT(WorldRegistry, windows, 0x108);
COURSE_ASSERT_SIZE(CourseItemTmpl, 0xC);
COURSE_ASSERT(RaceGate, z, 0x64);
COURSE_ASSERT(RaceGate, next, 0x84);
COURSE_ASSERT_SIZE(CourseParm, 0x18);
COURSE_ASSERT_SIZE(CourseMarkerParms, 0x50);
COURSE_ASSERT_SIZE(CourseMarker, 0x8C);
COURSE_ASSERT_SIZE(ColourSubItem, 0x40);
COURSE_ASSERT(CourseMarker, link, 0x2C);
COURSE_ASSERT(CourseMarker, pos, 0x30);
COURSE_ASSERT(CourseMarker, parms.wid.value, 0x54);
COURSE_ASSERT(CourseMarker, parms.ext.value, 0x6C);
COURSE_ASSERT(CourseMarker, parms.flg.value, 0x84);
COURSE_ASSERT_SIZE(MapProp, 0xDC);
COURSE_ASSERT(MapProp, shape, 0xB4);
COURSE_ASSERT(MapProp, add, 0xC0);
COURSE_ASSERT(MapProp, start, 0xC4);
COURSE_ASSERT(MapProp, goal, 0xD0);
COURSE_ASSERT(Mountain, posY, 0x2C);
COURSE_ASSERT(Mountain, scaleY, 0x34);
#endif

#endif
