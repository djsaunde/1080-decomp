#ifndef GAME_SELECT_H
#define GAME_SELECT_H

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

typedef struct SelItem {
    char pad000[0x28];
    void *vptr;             /* 0x028 */
    SelItemParms parms;     /* 0x02C: xpn.value 0x44 / ypn.value 0x5C = label position;
                             * off.v 0x164 */
    SelItemInfo info;       /* 0x194: ixn.value 0x1AC; rot.v 0x25C; scl.value 0x294 */
    char *node;             /* 0x29C: display node (+0xDC position) */
    f32 alpha;              /* 0x2A0: setAlpha */
    f32 enabled;            /* 0x2A4: 0 = not selectable (locked) */
    char pad2A8[0x2B0 - 0x2A8];
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

typedef struct SelCharModel {   /* SelCharacter (vtbl Data 0x57C) */
    char pad000[0x10C];
    s32 anim;               /* 0x10C: the SelCharAnim player (+0x64 / +0xB0 reset on each anim) */
    f32 view[4];            /* 0x110 */
    f32 alpha;              /* 0x120: setAlpha */
    char pad124[0x130 - 0x124];
    f32 highlight;          /* 0x130 */
    char pad134[0x138 - 0x134];
    s32 focused;            /* 0x138 */
    s32 chosen;             /* 0x13C: setChosen / clearChosen */
    s32 animSet;            /* 0x140: arg 2 of every anim change */
    s32 animMode;           /* 0x144: 0 focus, 8 onStageLeave, 0x10 confirm, 0x20 unfocus */
} SelCharModel;

typedef struct SelBoardModel {  /* SelBoard (vtbl Data 0x444) */
    char pad000[0x10C];
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
    char pad000[0x10C];
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
    char pad000[0x10C];
    f32 view[4];            /* 0x10C */
    f32 alpha;              /* 0x11C */
    char pad120[0x124 - 0x120];
    f32 highlight;          /* 0x124 */
    char pad128[0x134 - 0x128];
    s32 focused;            /* 0x134 */
    s32 chosen;             /* 0x138 */
} SelTesuriModel;

typedef struct SelLevelModel {  /* LevelItem's model */
    char pad000[0x10C];
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
SEL_ITEM_CLASS(SelBoardItem, SelBoardModel);
SEL_ITEM_CLASS(SelCourseItem, SelCourseModel);
SEL_ITEM_CLASS(SelTesuriItem, SelTesuriModel);
SEL_ITEM_CLASS(SelLevelItem, SelLevelModel);

typedef struct SelItemList {
    char pad00[0x3C];
    SelItem *items[12];     /* 0x3C */
    s32 count;              /* 0x6C */
} SelItemList;

typedef struct SelItemsMgr {
    char pad00[0x3C];
    s32 count;              /* 0x3C */
    SelItemList *lists[4];  /* 0x40: character, board, course, level (the stage index) */
} SelItemsMgr;

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

typedef struct SelectionMgr {
    char pad00[0x2C];
    char *sceneSet;         /* 0x2C: ctor arg 5; +0 root (its +0x5C +0x38 / +0x3C get the two
                             * windows), +4 / +8 the per-player scene handed to each SelectWin */
    s32 numPlayers;         /* 0x30 */
    struct SelectWin *wins[2]; /* 0x34 */
    char pad3C[0x48 - 0x3C];
    s32 unk48;              /* 0x48 */
    s32 busy;               /* 0x4C */
} SelectionMgr;

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
    char *scene;            /* 0x414: ctor arg 9 = sceneSet +4 (player 0) / +8 (player 1);
                             * +0xC camera, +0x10 / +0x14 the camera nodes */
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
    s32 centerX;            /* 0x4D8: 0x78 (2 players) / 0xF0 */
    s32 *out[5];            /* 0x4DC: commitSelection output pointers */
} SelectWin;                /* 0x4F0 (SelectWin_ctor allocates 0x4F0) */

/* View a raw `char *` window pointer as SelectWin (the state functions keep their
 * char * parameter: their `arg0 + stage * 4` cluster offsets are matching levers).
 * Exception: `eventFlags` stays a raw `FW(w, 0x4D4)` / `*(int *)(w + 0x4D4)` read in
 * every state function. As an `|` operand, the struct-member form takes its temp
 * register later and renumbers t6..t8 (checked in state0/3/4/6/7/8/9, agent-c run 484). */
#define SELWIN(p) ((SelectWin *)(p))
/* View a raw item pointer (the item virtuals take char * / int *) as SelItem. */
#define SELITEM(p) ((SelItem *)(p))

/* The select-screen room (gen_buildRoom_0000131C: a 0x60 "<Node>"; gen_createScene_00008DB4
 * builds it). gen_createLights / gen_createVRBox fill the lights and the sky box. */
typedef struct GenRoom {
    char node[0x2C];        /* 0x00: Node */
    char *world;            /* 0x2C: (app +0x134) +0x84; the room is appended to modifier,
                             * modifier to world */
    char *unk30;            /* 0x30: (app +0x134) +0x80 */
    char *unk34;            /* 0x34: (app +0x134) +0x8C */
    char *ambient;          /* 0x38: BaseLight "Ambient Light", colour 1, flags |= 0xC0, mask 1 */
    char *local;            /* 0x3C: BaseLight "Local Light", colour 1, mask 2 */
    char *ambient2;         /* 0x40: BaseLight "Ambient Light", colour (174, 113, 61) / 255,
                             * flags |= 0xC0, mask 4 */
    char *walls[6];         /* 0x44: the "VRBox" / "TheBox" sky box faces: U64BGWall_ctor with
                             * (pitch, yaw) (0,0) (0,90) (0,180) (0,270) (-90,0) (-270,0), all
                             * children of the "BGBox2" BGBox (gen_createVRBox) */
    char *modifier;         /* 0x5C: RoomModifier_ctor */
} GenRoom;                  /* 0x60 */

#define SELECT_ASSERT(name, cond) typedef char select_assert_##name[(cond) ? 1 : -1]
SELECT_ASSERT(parmi_size, sizeof(SelParmI) == 0x18);
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
SELECT_ASSERT(itemex_model, (int)&((SelItemEx *)0)->model == 0x2B8);
SELECT_ASSERT(itemex_size, sizeof(SelItemEx) == 0x2C4);
SELECT_ASSERT(charmodel_mode, (int)&((SelCharModel *)0)->animMode == 0x144);
SELECT_ASSERT(boardmodel_chosen, (int)&((SelBoardModel *)0)->chosen == 0x134);
SELECT_ASSERT(coursemodel_13c, (int)&((SelCourseModel *)0)->unk13C == 0x13C);
SELECT_ASSERT(tesurimodel_chosen, (int)&((SelTesuriModel *)0)->chosen == 0x138);
SELECT_ASSERT(levelmodel_chosen, (int)&((SelLevelModel *)0)->chosen == 0x138);
SELECT_ASSERT(charitem_size, sizeof(SelCharacterItem) == 0x2C4);
SELECT_ASSERT(list_count, (int)&((SelItemList *)0)->count == 0x6C);
SELECT_ASSERT(mgr_lists, (int)&((SelItemsMgr *)0)->lists == 0x40);
SELECT_ASSERT(selmgr_scene, (int)&((SelectionMgr *)0)->sceneSet == 0x2C);
SELECT_ASSERT(selmgr_busy, (int)&((SelectionMgr *)0)->busy == 0x4C);
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

#endif
