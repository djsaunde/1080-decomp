#ifndef GAME_SELECT_H
#define GAME_SELECT_H

#ifndef __cplusplus
#include "gdl/mnode.h"  /* QNode (the SelModel base) */
#include "gdl/window.h" /* Window (SelectWin.scene +0xC) */
#include "game/camera.h" /* MCamera (GenCamera.camera) */
#else
/* The edg (NCC) donors build without -I include and edg rejects the gdl headers' offset
 * asserts: they get this mirror of include/gdl/window.h's Window with the members the
 * split-screen code (SelectionMgr_setSplit / _update) touches. Keep the two in step. */
typedef struct Window {
    char pad000[0xB4];
    f32 aspect;             /* 0x0B4 */
    s32 width;              /* 0x0B8 */
    s32 height;             /* 0x0BC */
    s32 originX;            /* 0x0C0 */
    s32 originY;            /* 0x0C4 */
    s32 vpW;                /* 0x0C8 */
    s32 vpH;                /* 0x0CC */
    s32 vpX;                /* 0x0D0 */
    s32 vpY;                /* 0x0D4 */
} Window;
#endif

/*
 * genproc.uso (splat segment timproc_uso_b5): the select screens (agent-c runs 459 / 464).
 *
 * A SelectionMgr owns one SelectWin per player. Each SelectWin walks up to four stages
 * (character, board, course, level: stage bits 1/2/4/8, index 0..3) over the lists of a
 * SelItemsMgr; each list holds up to 12 SelItems.
 *
 * Every class is a cfront Node subclass: the vtable pointer is at +0x28, and virtual slot N
 * is the {s16 delta, fn} pair whose function word is at vtbl + 8*N + 0xC.
 * Only the fields with a known use are named; the rest is padding.
 */

/* ---- HostIO parm records (game_libs Parms_addParm(parm, owner, tag, 1)): each parm is a child
 * object of its Parms block, named by a 3-letter tag (the Data string the ctor passes). The ctor
 * stores the parm-type vtable at +0xC and the default value at +0x10. The field names below are
 * those tags (SelectWin_ctor / SelItemParms_ctor / SelItemInfo_ctor, Data 0x1140..0x11AC and
 * 0x127C..0x12F4). ---- */
typedef struct SelParmI {       /* int parm (type vtable D+0x1A0 in the ctor) */
    char hdr[0xC];
    s32 vptr;               /* 0x0C */
    s32 value;              /* 0x10 */
    s32 unk14;              /* 0x14: 0 */
} SelParmI;                 /* 0x18 */

typedef struct SelParmF {       /* float parm (type vtable D+0x158) */
    char hdr[0xC];
    s32 vptr;
    f32 value;              /* 0x10 */
    s32 unk14;
} SelParmF;                 /* 0x18 */

typedef struct SelParmV3 {      /* vec3 parm with a range (min / max vec3 passed to the ctor) */
    char hdr[0xC];
    s32 vptr;
    f32 v[3];               /* 0x10 */
    s32 unk1C;
} SelParmV3;                /* 0x20 */

typedef struct SelParmRGBA {    /* colour parm (type vtable D+0x188): RGBA, each 0..1 */
    char hdr[0xC];
    s32 vptr;
    f32 rgba[4];            /* 0x10 */
    s32 unk20;
} SelParmRGBA;              /* 0x24 */

/* SelItem::Parms (SelItemParms_ctor 0x2B74, at SelItem +0x2C): the item's screen layout. */
typedef struct SelItemParms {
    s32 vptr;               /* 0x000 */
    s32 unk004;
    SelParmI xpn;           /* 0x008: label x (SelItem_drawLabel / the camera lerp) */
    SelParmI ypn;           /* 0x020: label y */
    SelParmI jnx, jny;      /* 0x038 / 0x050 */
    SelParmI xp0, yp0;      /* 0x068 / 0x080 */
    SelParmI xfp, yfp;      /* 0x098 / 0x0B0 */
    SelParmI xfc, yfc;      /* 0x0C8 / 0x0E0 */
    SelParmF cfv, cf2;      /* 0x0F8 / 0x110: -20 */
    SelParmV3 off;          /* 0x128: camera offset (range +-1500; SelItem_reset clears it,
                             * SelectWin_lerpCamToItem adds off * t to the node position) */
    SelParmV3 of2;          /* 0x148 */
} SelItemParms;             /* 0x168 */

/* SelItem::Info (SelItemInfo_ctor 0x32C8, at SelItem +0x194): the item's model pose. */
typedef struct SelItemInfo {
    s32 vptr;               /* 0x000 */
    s32 unk004;
    SelParmI ixn;           /* 0x008: label sprite row (24-byte rows) */
    SelParmI am0, am1, am2, am3, am4; /* 0x020..0x080: 2 */
    SelParmV3 pos;          /* 0x098: range +-2500 / 1500 / 2500 */
    SelParmV3 rot;          /* 0x0B8: range +-pi; each item class's update copies rot and scl
                             * into its model's view[4] (see the model structs below) */
    SelParmF chy;           /* 0x0D8: 0 */
    SelParmF scl;           /* 0x0F0: 0 */
} SelItemInfo;              /* 0x108 */

/* A cfront virtual-table entry: slot N of a genproc vtable is at vtbl + 8 + 8 * N, and a call is
 * fn(this + delta, ...). */
typedef struct SelVEntry {
    short delta;            /* 0x0 (short: the edg donors have no s16) */
    short pad2;
    void *fn;               /* 0x4 */
} SelVEntry;

/* The SelItem vtable (base Data 0x2F4; CharacterItem / BoardItem / CourseItem / TesuriItem /
 * LevelItem 0x5E4 / 0x6D4 / 0x7C4 / 0x8B4 / 0x9A4). Slot names from the BoardItem / SelItem
 * overrides (renames.tsv; slots 10-20 are pure in SelItem, 21-26 empty, 27 = loadInfo) and from
 * the SelectWin state calls (agent-c run 530). */
typedef struct SelItemVtbl {
    char head[0x8];
    SelVEntry base[10];     /* 0x08: slots 0..9 (Node; slot 8 = loadData) */
    SelVEntry updateHighlight; /* 0x58: slot 10 (fade the model highlight toward a target) */
    SelVEntry focus;        /* 0x60: slot 11 (the cursor moves onto the item) */
    SelVEntry unfocus;      /* 0x68: slot 12 */
    SelVEntry onStageLeave; /* 0x70: slot 13 */
    SelVEntry onStageReturn; /* 0x78: slot 14 */
    SelVEntry setChosen;    /* 0x80: slot 15 (item, flag) */
    SelVEntry clearChosen;  /* 0x88: slot 16 (item, flag) */
    SelVEntry toggleVariant; /* 0x90: slot 17 (flags slot-17 toggle) */
    SelVEntry playPickAnim; /* 0x98: slot 18 */
    SelVEntry playAcceptAnim; /* 0xA0: slot 19 */
    SelVEntry confirm;      /* 0xA8: slot 20 */
    SelVEntry copyState;    /* 0xB0: slot 21 (item, the other player's item) */
    SelVEntry setAlpha;     /* 0xB8: slot 22 (item, f32 alpha) */
    SelVEntry drawLabel;    /* 0xC0: slot 23 (item, win, colour, colour) */
    SelVEntry drawSubLabel; /* 0xC8: slot 24 */
    SelVEntry drawStats;    /* 0xD0: slot 25 (item, win) */
    SelVEntry drawArrows;   /* 0xD8: slot 26 (item, win, frame / 8) */
    SelVEntry loadInfo;     /* 0xE0: slot 27 */
} SelItemVtbl;              /* 0xE8 */

/* View a raw vtable pointer (the state functions load item->vptr into a char * / int * local)
 * as SelItemVtbl. */
#define SELVT(v) ((SelItemVtbl *)(v))

typedef struct SelItem {
    char pad000[0x28];
    SelItemVtbl *vptr;      /* 0x028 */
    SelItemParms parms;     /* 0x02C: xpn.value 0x44 / ypn.value 0x5C = label position;
                             * off.v 0x164 */
    SelItemInfo info;       /* 0x194: ixn.value 0x1AC; rot.v 0x25C; scl.value 0x294 */
    char *node;             /* 0x29C: display node (+0xDC position) */
    f32 alpha;              /* 0x2A0: setAlpha */
    f32 enabled;            /* 0x2A4: 0 = not selectable (locked) */
    char pad2A8[0x2AC - 0x2A8];
    s32 listIndex;          /* 0x2AC: LevelList_createItem stores the list's count (the item's own
                             * index); LevelItem_drawStats lights the slot (0..2) equal to it */
    s32 id;                 /* 0x2B0: the value written out by SelectWin_commitSelection */
    s32 flags;              /* 0x2B4: written out with the board id; Character bit 17 / Board bit 5 = slot 17 toggle */
} SelItem;                  /* 0x2B8; the subclasses add the model pointer at 0x2B8 */

/* the common subclass tail (CharacterItem / BoardItem / CourseItem / TesuriItem / LevelItem) */
typedef struct SelItemEx {
    SelItem base;           /* 0x000 */
    char *model;            /* 0x2B8: SelCharacter / SelBoard / SelCourseMap / SelTesuri QNode state */
    s32 barCount;           /* 0x2BC: drawStats bar growth (to 10) */
    s32 barTimer;           /* 0x2C0 */
} SelItemEx;

/* ---- per-class display models (SelItemEx.model; the QNode state each item class
 * creates). Only the fields the item virtuals touch are named; `view` is the
 * four floats each class's update copies in from the item every frame: view[0] = info.rot.v[2],
 * view[1] = rot.v[0], view[2] = rot.v[1], view[3] = info.scl.value (the same mapping in all five
 * classes; only the statement order differs, a matching lever),
 * `highlight` the f32 that updateHighlight fades toward its target, `focused`
 * the int that forces the highlight target to 1. ---- */

/* The common 0x10C head of the five models (agent-c run 525). Every createItem
 * (CharacterList / BoardList / CourseList / LevelList, SelItemsMgr's Tesuri items) allocates
 * the model, runs QNode_ctor(model, name, 0, 0, 0), stores the class vtable at +0x28, then
 * allocates a 0x16C child QNode (QNode_ctor, its +0x108 / 0x10C / 0x110 set to one RoData
 * constant, +0x124 = 1.0), stores it at +0x108, MNode_addChild(model, inner) and clears or
 * sets inner->vqs.trans. SelectWin_ctor hands the three Tesuri models' inner to the 0.5f
 * setup calls. The edg (NCC) donors see the QNode base as opaque bytes (no gdl headers). */
typedef struct SelModel {
#ifndef __cplusplus
    QNode qnode;            /* 0x000 */
#else
    char qnode[0x104];      /* 0x000 */
#endif
    s32 unk104;             /* 0x104 */
    char *inner;            /* 0x108: the child QNode (0x16C) */
} SelModel;                 /* 0x10C */

typedef struct SelCharModel {   /* SelCharacter (vtbl Data 0x57C) */
    SelModel head;          /* 0x000 */
    s32 anim;               /* 0x10C: the SelCharAnim player (+0x64 / +0xB0 reset on each anim) */
    f32 view[4];            /* 0x110 */
    f32 alpha;              /* 0x120: setAlpha (createItem: 0) */
    f32 unk124;             /* 0x124: createItem: RoData 0x3A8 */
    f32 unk128;             /* 0x128: createItem: RoData 0x3AC */
    char pad12C[0x130 - 0x12C];
    f32 highlight;          /* 0x130 (createItem: RoData 0x3A4) */
    s32 unk134;             /* 0x134: createItem: 0 */
    s32 focused;            /* 0x138 */
    s32 chosen;             /* 0x13C: setChosen / clearChosen */
    s32 animSet;            /* 0x140: arg 2 of every anim change */
    s32 animMode;           /* 0x144: 0 focus, 8 onStageLeave, 0x10 confirm, 0x20 unfocus */
} SelCharModel;

typedef struct SelBoardModel {  /* SelBoard (vtbl Data 0x444) */
    SelModel head;          /* 0x000 */
    f32 spin;               /* 0x10C: reset to 0 on focus */
    f32 view[4];            /* 0x110 */
    f32 alpha;              /* 0x120 */
    f32 highlight;          /* 0x124 */
    char pad128[0x12C - 0x128];
    s32 focused;            /* 0x12C */
    s32 lit;                /* 0x130: set by focus / onStageReturn, cleared by unfocus / onStageLeave */
    s32 chosen;             /* 0x134 */
} SelBoardModel;

typedef struct SelCourseModel { /* SelCourseMap (vtbl Data 0x514) */
    SelModel head;          /* 0x000 */
    f32 view[4];            /* 0x10C */
    f32 alpha;              /* 0x11C */
    char pad120[0x124 - 0x120];
    f32 highlight;          /* 0x124 */
    f32 mapFade;            /* 0x128: CourseItem_update fades it while `lit` */
    char pad12C[0x130 - 0x12C];
    s32 focused;            /* 0x130 */
    s32 lit;                /* 0x134: onStageReturn sets, unfocus clears */
    s32 chosen;             /* 0x138 */
    s32 unk13C;             /* 0x13C: copied by copyState */
} SelCourseModel;

typedef struct SelTesuriModel { /* SelTesuri (vtbl Data 0x4AC) */
    SelModel head;          /* 0x000 */
    f32 view[4];            /* 0x10C */
    f32 alpha;              /* 0x11C */
    char pad120[0x124 - 0x120];
    f32 highlight;          /* 0x124 */
    char pad128[0x134 - 0x128];
    s32 focused;            /* 0x134 */
    s32 chosen;             /* 0x138 */
} SelTesuriModel;

typedef struct SelLevelModel {  /* LevelItem's model */
    SelModel head;          /* 0x000 */
    f32 view[4];            /* 0x10C */
    f32 alpha;              /* 0x11C */
    char pad120[0x124 - 0x120];
    f32 highlight;          /* 0x124 */
    char pad128[0x130 - 0x128];
    s32 focused;            /* 0x130 */
    char pad134[0x138 - 0x134];
    s32 chosen;             /* 0x138 */
} SelLevelModel;

/* SelItemEx with the model typed per class. */
#define SEL_ITEM_CLASS(T, M) typedef struct T { SelItem base; M *model; s32 barCount; s32 barTimer; } T
SEL_ITEM_CLASS(SelCharacterItem, SelCharModel);
/* BoardItem is 0x2C8 (BoardList_createItem allocates 0x2C8 and clears blink): */
typedef struct SelBoardItem {
    SelItem base;           /* 0x000 */
    SelBoardModel *model;   /* 0x2B8 */
    s32 barCount;           /* 0x2BC */
    s32 barTimer;           /* 0x2C0 */
    s32 blink;              /* 0x2C4: BoardItem_drawStats counts it up once per draw and shows the
                             * extra (233, 247, 0) sprite while bit 3 is set (8 frames on / 8 off) */
} SelBoardItem;             /* 0x2C8 */
SEL_ITEM_CLASS(SelCourseItem, SelCourseModel);
SEL_ITEM_CLASS(SelTesuriItem, SelTesuriModel);
SEL_ITEM_CLASS(SelLevelItem, SelLevelModel);

typedef struct SelItemList {
    char pad00[0x28];
    void *vptr;             /* 0x28 */
    s32 cfg2C;              /* 0x2C..0x34: the SelItemsMgr's three CommonInfo words, copied by the ctor */
    char *parent;           /* 0x30 */
    s32 cfg34;              /* 0x34 */
    char *dispItems;        /* 0x38: the SelItemsMgr's DispItems node; createItem adds each model under it */
    SelItem *items[12];     /* 0x3C */
    s32 count;              /* 0x6C */
} SelItemList;

/* "SelItemsMgr" (SelItemsMgr_ctor, alloc 0x60; node names from the Data strings it passes). */
typedef struct SelItemsMgr {
    char pad00[0x28];
    void *vptr;             /* 0x28 */
    s32 cfg2C;              /* 0x2C: CommonInfo_init: (D+0x134)->0x84 (the S70 lists copy the same three) */
    char *parent;           /* 0x30: (D+0x134)->0x80; the ctor adds dispItems under it */
    s32 cfg34;              /* 0x34: (D+0x134)->0x8C */
    char *dispItems;        /* 0x38: "DispItems" node (0x2C, vtbl Data 0xCC4); the lists and the item
                             * models are added under it */
    s32 count;              /* 0x3C */
    SelItemList *lists[4];  /* 0x40: character, board, course, level (the stage index) */
    char *commonInfo;       /* 0x50: "Common Info" (0x30; +0x2C = this); Node_Load'ed with ctor arg 2 */
    struct SelTesuriItem *tesuri[3]; /* 0x54: the three "Tesuri Item"s (TesuriItem vtbl Data 0x8B4,
                             * "Tesuri" QNode model); [2] is built first and only when the game
                             * mode (import 800200CC +0x34) is not 2 */
} SelItemsMgr;              /* 0x60 */

/* SelectWin::Parms (SelectWin_ctor, at SelectWin +0x2C). Defaults in the comments. */
typedef struct SelectWinParms {
    s32 vptr;               /* 0x000 */
    s32 unk004;
    SelParmF cwt;           /* 0x008 (win 0x044): 0.45 */
    SelParmF voy;           /* 0x020 (0x05C): 0 */
    SelParmI ysp;           /* 0x038 (0x074): 0x18 */
    SelParmI hxp, hyp;      /* 0x050 / 0x068 (0x08C / 0x0A4): 0x78 */
    SelParmF msp;           /* 0x080 (0x0BC): 0.036, the transition step per frame
                             * (SelectWin_update: transition += msp) */
    SelParmF zsp, lsp;      /* 0x098 / 0x0B0 (0x0D4 / 0x0EC): 0.036 */
    SelParmI lfw;           /* 0x0C8 (0x104): 0x99, idleTimer reload (frames until the focused
                             * item is chosen without input) */
    SelParmI sfw;           /* 0x0E0 (0x11C): 0x99 */
    SelParmRGBA cc[9];      /* 0x0F8: cc0..cc8 (cc8.rgba = win 0x254, passed to the item draw slot) */
    SelParmRGBA c12;        /* 0x23C (rgba = win 0x278, the other colour passed to the draw slot) */
    SelParmRGBA cc9;        /* 0x260 */
    SelParmRGBA c11, c10;   /* 0x284 / 0x2A8 */
    SelParmI clx, cly;      /* 0x2CC / 0x2E4: -4 / 0xD */
    SelParmI cox, coy;      /* 0x2FC / 0x314: -0x10 / -0x15 */
    SelParmI cff, cdf, ctf; /* 0x32C / 0x344 / 0x35C: 0x73 / 0x62 / 0x62 */
    SelParmI tpy;           /* 0x374: 0xD */
} SelectWinParms;           /* 0x38C */

struct SelectWin;
struct MNode;
struct MCamera;

/* A select-screen camera rig (gen_createCamera_0000283C, alloc 0x20; GenScene.cam / cam2 =
 * SelectWin.scene). It builds two 0xB4 MNodes (Node_ctor + MNode_initMatrices, local
 * translation 0): the first goes to +0x14, the second to +0x10 and becomes the MCamera's
 * look-at target (MCamera_ctor(0, target, 0, 0, 0)); both are appended to bg's child list,
 * and target is moved to (0, 0, -400). The 3D Window renders camera (game_create3DWindow(...,
 * camera)); its fov (MCamera +0x14C) is the ctor's arg 3 (0x3C one player / 0x29 two). */
typedef struct GenCamera {
    char *bg;               /* 0x00: WorldRegistry.bgRoot (GameFlow.game, = GenRoom.bg) */
    char *fg;               /* 0x04: WorldRegistry.fgRoot */
    char *mainGame;         /* 0x08: WorldRegistry.mainGame */
    Window *view;           /* 0x0C: the screen Window (SelectionMgr_setSplit sizes it) */
    struct MNode *target;   /* 0x10: the look-at node; SelectWin.camPos copies its local
                             * translation (mtx[3], +0x60..0x68), lerpCamToItem moves it */
    struct MNode *anchor;   /* 0x14: the first MNode (SelectWin_init passes it to setCamToItem) */
    struct MCamera *camera; /* 0x18: MCamera_ctor; state 6 turns pitch / yaw while rotEnable */
    s32 unk1C;              /* 0x1C */
} GenCamera;                /* 0x20 */

typedef struct SelectionMgr {
    char pad00[0x2C];
    char *sceneSet;         /* 0x2C: ctor arg 5, a GenScene (gen_createScene): room->modifier
                             * +0x38 / +0x3C get the two windows; cam / cam2 are handed to each
                             * SelectWin as its scene */
    s32 numPlayers;         /* 0x30 */
    struct SelectWin *wins[2]; /* 0x34 */
    /* The 2-player split screen (agent-c run 522: SelectionMgr_ctor, SelectionMgr_update
     * (ncc_1F14), SelectionMgr_setSplit (ncc_1DB0), SelectionMgr_finish). `split` is
     * wins[0]->viewH and 0xF0 - split is wins[1]'s; 0x78 is the even split. When one
     * window waits in state 8 (board accepted) and the other joins it, the waiting
     * window slides to the full screen: zoomWin picks it, splitT ramps 0 -> 1 by 0.05
     * and split follows (120 +- splitT * 120). */
    s32 waitMask;           /* 0x3C: bit 0 / 1 = wins[0] / wins[1] in SelectWin state 8 (rebuilt
                             * every update; 3 = both) */
    s32 prevWaitMask;       /* 0x40: last update's waitMask */
    s32 split;              /* 0x44: wins[0]->viewH, a height in lines (0x78 even, 0xF0 = wins[0]
                             * full screen) */
    s32 zoomWin;            /* 0x48: 1 = wins[0] / 2 = wins[1] takes the screen, 0 = none;
                             * finish sends the result of that window */
    s32 busy;               /* 0x4C: 1 while the split slides */
    s32 shownMask;          /* 0x50: bit 0 / 1 = wins[0] / wins[1] has a nonzero viewH */
    f32 splitT;             /* 0x54: slide progress 0..1 (also the fov / aspect blend) */
} SelectionMgr;             /* 0x58 (SelectionMgr_ctor allocates 0x58) */

typedef struct SelectWin {
    char pad000[0x28];
    void *vptr;             /* 0x028 */
    SelectWinParms parms;   /* 0x02C */
    s32 player;             /* 0x3B8: 0 / 1 */
    s32 numPlayers;         /* 0x3BC: 1 / 2 */
    s32 stageMask;          /* 0x3C0: enabled stage bits */
    s32 stage;              /* 0x3C4: stage index 0..3 */
    s32 stageBit;           /* 0x3C8: 1 << stage */
    s32 state;              /* 0x3CC: SelectWin_update's switch (0..10) */
    s32 curItem[4];         /* 0x3D0: cursor per stage */
    s32 chosenItem[4];      /* 0x3E0 */
    s32 prevStage;          /* 0x3F0: saveFocus */
    s32 prevItem;           /* 0x3F4 */
    s32 prevItemActive;     /* 0x3F8: 1 from beginTransition; SelectWin_update keeps updating the
                             * previous focus item while set; cleared once fade reaches 0 */
    s32 transCountdown;     /* 0x3FC: reloaded from transCountdownInit by every beginTransition,
                             * SelectWin_update counts it down by 4 to 0 (16 frames); no reader
                             * found in genproc */
    s32 zoomTimer;          /* 0x400: state-5 (item zoom) frames left: 0x37 from state3 / state4,
                             * (s32)(idleTimer reload * 0.75) from beginTransition; state5
                             * decrements it and at 0 accepts the item without input */
    s32 transCountdownInit; /* 0x404: 0x40 (SelectWin_init) */
    s32 forceAccept;        /* 0x408: state3 treats it like an accept press
                             * (input_checkPressed(gInputMgr, 0x100)): state 4 or the zoom state 5;
                             * cleared there and at the end of SelectWin_update; never set
                             * inside genproc */
    SelItemsMgr *itemsMgr;  /* 0x40C */
    char *sceneSet;         /* 0x410: ctor arg 8 = SelectionMgr.sceneSet */
    GenCamera *scene;       /* 0x414: ctor arg 9 = sceneSet +4 (player 0) / +8 (player 1):
                             * GenScene.cam / cam2 */
    char *fadeProc;         /* 0x418: ctor arg 2 (SelectionMgr_ctor's arg 1); commitSelection
                             * fades it out with Process_fadeOut(fadeProc, -1, 0) */
    SelectionMgr *owner;    /* 0x41C */
    f32 colors[6][4];       /* 0x420: RGBA / 255, set by SelectWin_init: {90,90,90}, {255,145,58},
                             * {255,255,255}, {13,13,13}, {161,91,35}, {70,70,70} (alpha 255);
                             * no reader inside genproc */
    f32 fade;               /* 0x480 */
    f32 fadeTarget;         /* 0x484: state1 ramps fade toward it; 1 when a state shows its items,
                             * 0 at beginTransition / when leaving */
    s32 idleTimer;          /* 0x488: parms.lfw frames; counted down in the stage state, at < 0
                             * the focused item is chosen automatically (state 1) */
    s32 blink;              /* 0x48C: frame counter, arrows blink on bit 3 */
    s32 frame;              /* 0x490 */
    f32 fovInit;            /* 0x494: the camera fov at SelectWin_init (scene->0xC->0x70 +0x14C,
                             * also stored to fovTarget / fovBase) */
    f32 fovTarget;          /* 0x498: SelectWin_update: fovBase + f(fade) * item zoom fov */
    f32 fovBase;            /* 0x49C */
    f32 unk4A0;             /* 0x4A0 */
    /* Cheat input (pad sequences typed on the character / board stage; state3 /
     * state6 match them, SelectWin_resetCheat clears them). */
    s32 cheatChar;          /* 0x4A4: matched character-stage sequence + 1 (charCheatIndex) */
    s32 cheatBoard;         /* 0x4A8: matched board-stage sequence + 1 (boardCheatIndex) */
    s32 cheatStep;          /* 0x4AC: position in the sequence being typed */
    s32 cheatTimer;         /* 0x4B0: input timeout, reloaded from Data 0xF18 per key */
    s32 busy;               /* 0x4B4: set by beginTransition */
    s32 stageLock;          /* 0x4B8: 1 when the board stage is accepted (state7 -> state 8),
                             * 0 when the transition ends; in 2 players the other window's
                             * state7 ORs it into its busy test */
    f32 transition;         /* 0x4BC: 0 -> 1 at parms.msp per frame while busy (SelectWin_update);
                             * drives the camera lerps and the fov blend */
    f32 fovFrom;            /* 0x4C0: the camera fov (scene->0xC->0x70 +0x14C, degrees) at
                             * beginTransition; blended toward fovTarget by transition */
    f32 camPos[3];          /* 0x4C4: beginTransition copies scene->0x10 +0x60..0x68 */
    s32 font;               /* 0x4D0: gui_fontCreate(0, font, 0x48, 0xDD, 3, 13) (SelectWin_init) */
    s32 eventFlags;         /* 0x4D4: ctor arg 11 (0 / 0x80), ORed into the select event */
    s32 viewH;              /* 0x4D8: the window's screen height in lines: 0x78 (2 players) /
                             * 0xF0 (full screen); SelectionMgr_setSplit / _update write it as
                             * split / 0xF0 - split and give SELWIN_VIEW() the same height (vpH).
                             * Was centerX (agent-c run 530: the split is vertical) */
    s32 *out[5];            /* 0x4DC: commitSelection output pointers */
} SelectWin;                /* 0x4F0 (SelectWin_ctor allocates 0x4F0) */

/* View a raw `char *` window pointer as SelectWin (the state functions keep their
 * char * parameter: their `arg0 + stage * 4` cluster offsets are matching levers).
 * Exception: `eventFlags` stays a raw `FW(w, 0x4D4)` / `*(int *)(w + 0x4D4)` read in
 * every state function. As an `|` operand, the struct-member form takes its temp
 * register later and renumbers t6..t8 (checked in state0/3/4/6/7/8/9, agent-c run 484). */
#define SELWIN(p) ((SelectWin *)(p))
/* The screen Window of a SelectWin (scene +0xC; scene +0xC +0x70 is its camera, fov at +0x14C).
 * The two-player split is vertical: SelectionMgr_setSplit gives wins[0] height = vpH = split at
 * originY = vpY = 0 and wins[1] height = vpH = 0xF0 - split at originY = vpY = split, and
 * blends aspect (1.333 + 1.3333 * t). */
#define SELWIN_VIEW(w) ((w)->scene->view)
/* The window's camera as the Window draws it (view->camera; the same MCamera as scene->camera). */
#define SELWIN_CAMERA(w) ((MCamera *)SELWIN_VIEW(w)->camera)
/* View a raw item pointer (the item virtuals take char * / int *) as SelItem. */
#define SELITEM(p) ((SelItem *)(p))

/* The select-screen room (gen_buildRoom_0000131C: a 0x60 "<Node>"; gen_createScene_00008DB4
 * builds it). gen_createLights / gen_createVRBox fill the lights and the sky box. */
typedef struct GenRoom {
    char node[0x2C];        /* 0x00: Node */
    char *bg;               /* 0x2C: the world's "bg" node (GameFlow.game = WorldRegistry,
                             * bgRoot); the room is appended to modifier, modifier to bg */
    char *fg;               /* 0x30: WorldRegistry.fgRoot ("fg") */
    char *mainGame;         /* 0x34: WorldRegistry.mainGame ("MainGame") */
    char *ambient;          /* 0x38: BaseLight "Ambient Light", colour 1, flags |= 0xC0, mask 1 */
    char *local;            /* 0x3C: BaseLight "Local Light", colour 1, mask 2 */
    char *ambient2;         /* 0x40: BaseLight "Ambient Light", colour (174, 113, 61) / 255,
                             * flags |= 0xC0, mask 4 */
    char *walls[6];         /* 0x44: the "VRBox" / "TheBox" sky box faces: U64BGWall_ctor with
                             * (pitch, yaw) (0,0) (0,90) (0,180) (0,270) (-90,0) (-270,0), all
                             * children of the "BGBox2" BGBox (gen_createVRBox) */
    char *modifier;         /* 0x5C: RoomModifier_ctor */
} GenRoom;                  /* 0x60 */

/* The select scene (gen_createScene_00008DB4, alloc 0xC; = SelectionMgr.sceneSet). It also
 * builds the "MainGame" node with its "bg" / "fg" children (game_setParams(app, fg, bg,
 * MainGame)) and hangs MainGame under the app root. */
typedef struct GenScene {
    GenRoom *room;          /* 0x0: gen_buildRoom(0, numPlayers) */
    GenCamera *cam;         /* 0x4: player 0's camera: gen_createCamera(0, 1, 1, 0x3C) with 1 player,
                             * (0, 1, 2, 0x29) with 2 (= SelectWin.scene of player 0) */
    GenCamera *cam2;        /* 0x8: 2 players only: gen_createCamera(0, 2, 3, 0x29); its
                             * view (a ScreenWindow) unk1B8 gets cam's view */
} GenScene;                 /* 0xC */

/* The character-select process (GenSelectChar_ctor, alloc 0x58): a 0x50 Process (itself a Node;
 * the ctor's inlined base ctors allocate 0x50 / 0x2C) whose +0x10 child list gets the
 * SelItemsMgr(s), the SelectionMgr and the Fader. */
typedef struct GenSelectChar {
    char process[0x50];     /* 0x00: Process (Node; +0x2C / +0x30 / +0x48 cleared by the ctor) */
    s32 twoPlayer;          /* 0x50: 0 when the ctor's player count (arg 1, gen_createScene's a1) is 1,
                             * else 1 (one SelItemsMgr vs two); GenSelectChar_initLabels derives
                             * the label ids from it (n * 9, n * 2 + 0x10, ...) */
    s32 unk54;              /* 0x54: 0 */
} GenSelectChar;            /* 0x58 */

#ifndef __cplusplus /* edg (the NCC donors ncc_1DB0 / ncc_1F14 include this header) rejects the
                     * &((T *)0)->f constant form; the C builds check the offsets */
#define SELECT_ASSERT(name, cond) typedef char select_assert_##name[(cond) ? 1 : -1]
SELECT_ASSERT(parmi_size, sizeof(SelParmI) == 0x18);
SELECT_ASSERT(genselectchar_size, sizeof(GenSelectChar) == 0x58);
SELECT_ASSERT(genselectchar_twoplayer, (int)&((GenSelectChar *)0)->twoPlayer == 0x50);
SELECT_ASSERT(parmv3_size, sizeof(SelParmV3) == 0x20);
SELECT_ASSERT(parmrgba_size, sizeof(SelParmRGBA) == 0x24);
SELECT_ASSERT(itemparms_size, sizeof(SelItemParms) == 0x168);
SELECT_ASSERT(iteminfo_size, sizeof(SelItemInfo) == 0x108);
SELECT_ASSERT(winparms_size, sizeof(SelectWinParms) == 0x38C);
SELECT_ASSERT(winparms_tpy, (int)&((SelectWinParms *)0)->tpy == 0x374);
SELECT_ASSERT(item_rot, (int)&((SelItem *)0)->info.rot.v == 0x25C);
SELECT_ASSERT(item_scl, (int)&((SelItem *)0)->info.scl.value == 0x294);
SELECT_ASSERT(item_xpn, (int)&((SelItem *)0)->parms.xpn.value == 0x44);
SELECT_ASSERT(item_alpha, (int)&((SelItem *)0)->alpha == 0x2A0);
SELECT_ASSERT(item_id, (int)&((SelItem *)0)->id == 0x2B0);
SELECT_ASSERT(item_size, sizeof(SelItem) == 0x2B8);
SELECT_ASSERT(vt_focus, (int)&((SelItemVtbl *)0)->focus == 0x60);
SELECT_ASSERT(vt_size, sizeof(SelItemVtbl) == 0xE8);
SELECT_ASSERT(itemex_model, (int)&((SelItemEx *)0)->model == 0x2B8);
SELECT_ASSERT(itemex_size, sizeof(SelItemEx) == 0x2C4);
SELECT_ASSERT(model_inner, (int)&((SelModel *)0)->inner == 0x108);
SELECT_ASSERT(model_size, sizeof(SelModel) == 0x10C);
SELECT_ASSERT(charmodel_mode, (int)&((SelCharModel *)0)->animMode == 0x144);
SELECT_ASSERT(boardmodel_chosen, (int)&((SelBoardModel *)0)->chosen == 0x134);
SELECT_ASSERT(coursemodel_13c, (int)&((SelCourseModel *)0)->unk13C == 0x13C);
SELECT_ASSERT(tesurimodel_chosen, (int)&((SelTesuriModel *)0)->chosen == 0x138);
SELECT_ASSERT(levelmodel_chosen, (int)&((SelLevelModel *)0)->chosen == 0x138);
SELECT_ASSERT(charitem_size, sizeof(SelCharacterItem) == 0x2C4);
SELECT_ASSERT(boarditem_size, sizeof(SelBoardItem) == 0x2C8);
SELECT_ASSERT(list_count, (int)&((SelItemList *)0)->count == 0x6C);
SELECT_ASSERT(list_disp, (int)&((SelItemList *)0)->dispItems == 0x38);
SELECT_ASSERT(item_cfv, (int)&((SelItem *)0)->parms.cfv.value == 0x134);
SELECT_ASSERT(item_pos, (int)&((SelItem *)0)->info.pos == 0x22C);
SELECT_ASSERT(mgr_lists, (int)&((SelItemsMgr *)0)->lists == 0x40);
SELECT_ASSERT(mgr_disp, (int)&((SelItemsMgr *)0)->dispItems == 0x38);
SELECT_ASSERT(mgr_tesuri, (int)&((SelItemsMgr *)0)->tesuri == 0x54);
SELECT_ASSERT(mgr_size, sizeof(SelItemsMgr) == 0x60);
SELECT_ASSERT(selmgr_scene, (int)&((SelectionMgr *)0)->sceneSet == 0x2C);
SELECT_ASSERT(selmgr_busy, (int)&((SelectionMgr *)0)->busy == 0x4C);
SELECT_ASSERT(selmgr_wait, (int)&((SelectionMgr *)0)->waitMask == 0x3C);
SELECT_ASSERT(selmgr_split, (int)&((SelectionMgr *)0)->split == 0x44);
SELECT_ASSERT(selmgr_splitt, (int)&((SelectionMgr *)0)->splitT == 0x54);
SELECT_ASSERT(selmgr_size, sizeof(SelectionMgr) == 0x58);
SELECT_ASSERT(win_player, (int)&((SelectWin *)0)->player == 0x3B8);
SELECT_ASSERT(win_cur, (int)&((SelectWin *)0)->curItem == 0x3D0);
SELECT_ASSERT(win_prev, (int)&((SelectWin *)0)->prevItem == 0x3F4);
SELECT_ASSERT(win_mgr, (int)&((SelectWin *)0)->itemsMgr == 0x40C);
SELECT_ASSERT(win_owner, (int)&((SelectWin *)0)->owner == 0x41C);
SELECT_ASSERT(win_lfw, (int)&((SelectWin *)0)->parms.lfw.value == 0x104);
SELECT_ASSERT(win_cc8, (int)&((SelectWin *)0)->parms.cc[8].rgba == 0x254);
SELECT_ASSERT(win_c12, (int)&((SelectWin *)0)->parms.c12.rgba == 0x278);
SELECT_ASSERT(win_colors, (int)&((SelectWin *)0)->colors == 0x420);
SELECT_ASSERT(win_font, (int)&((SelectWin *)0)->font == 0x4D0);
SELECT_ASSERT(win_fade, (int)&((SelectWin *)0)->fade == 0x480);
SELECT_ASSERT(win_busy, (int)&((SelectWin *)0)->busy == 0x4B4);
SELECT_ASSERT(win_cam, (int)&((SelectWin *)0)->camPos == 0x4C4);
SELECT_ASSERT(win_flags, (int)&((SelectWin *)0)->eventFlags == 0x4D4);
SELECT_ASSERT(room_walls, (int)&((GenRoom *)0)->walls == 0x44);
SELECT_ASSERT(room_size, sizeof(GenRoom) == 0x60);
SELECT_ASSERT(win_size, sizeof(SelectWin) == 0x4F0);
SELECT_ASSERT(gencam_target, (int)&((GenCamera *)0)->target == 0x10);
SELECT_ASSERT(gencam_size, sizeof(GenCamera) == 0x20);
#endif /* !__cplusplus */

#endif
