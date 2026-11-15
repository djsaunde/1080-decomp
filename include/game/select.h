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

typedef struct SelItem {
    char pad000[0x28];
    void *vptr;             /* 0x028 */
    char pad02C[0x44 - 0x2C];
    s32 x;                  /* 0x044: label position (drawLabel) */
    char pad048[0x5C - 0x48];
    s32 y;                  /* 0x05C */
    char pad060[0x164 - 0x60];
    f32 unk164[3];          /* 0x164: cleared by SelItem_reset */
    char pad170[0x1AC - 0x170];
    s32 labelIndex;         /* 0x1AC: row of the label sprite table (24-byte rows) */
    char pad1B0[0x29C - 0x1B0];
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
 * four floats each class's update copies in from the item (+0x25C.. / +0x294),
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

struct SelectWin;

typedef struct SelectionMgr {
    char pad00[0x30];
    s32 numPlayers;         /* 0x30 */
    struct SelectWin *wins[2]; /* 0x34 */
    char pad3C[0x48 - 0x3C];
    s32 unk48;              /* 0x48 */
    s32 busy;               /* 0x4C */
} SelectionMgr;

typedef struct SelectWin {
    char pad000[0x28];
    void *vptr;             /* 0x028 */
    char parms[0x3B8 - 0x2C]; /* 0x02C: SelectWin::Parms (cwt voy ysp hxp ... tpy) */
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
    s32 unk3F8;             /* 0x3F8 */
    s32 unk3FC;             /* 0x3FC */
    s32 unk400;             /* 0x400 */
    s32 unk404;             /* 0x404 */
    char pad408[0x40C - 0x408];
    SelItemsMgr *itemsMgr;  /* 0x40C */
    s32 unk410;             /* 0x410 */
    char *scene;            /* 0x414 */
    s32 unk418;             /* 0x418 */
    SelectionMgr *owner;    /* 0x41C */
    char pad420[0x480 - 0x420];
    f32 fade;               /* 0x480 */
    f32 unk484;             /* 0x484 */
    s32 unk488;             /* 0x488 */
    s32 blink;              /* 0x48C: frame counter, arrows blink on bit 3 */
    s32 frame;              /* 0x490 */
    f32 unk494[3];          /* 0x494 */
    f32 unk4A0;             /* 0x4A0 */
    /* Cheat input (pad sequences typed on the character / board stage; state3 /
     * state6 match them, SelectWin_resetCheat clears them). */
    s32 cheatChar;          /* 0x4A4: matched character-stage sequence + 1 (charCheatIndex) */
    s32 cheatBoard;         /* 0x4A8: matched board-stage sequence + 1 (boardCheatIndex) */
    s32 cheatStep;          /* 0x4AC: position in the sequence being typed */
    s32 cheatTimer;         /* 0x4B0: input timeout, reloaded from Data 0xF18 per key */
    s32 busy;               /* 0x4B4: set by beginTransition */
    s32 unk4B8;             /* 0x4B8 */
    f32 unk4BC;             /* 0x4BC */
    f32 unk4C0;             /* 0x4C0: beginTransition copies scene->0xC->0x70->0x14C */
    f32 camPos[3];          /* 0x4C4: beginTransition copies scene->0x10 +0x60..0x68 */
    char pad4D0[0x4D4 - 0x4D0];
    s32 eventFlags;         /* 0x4D4: ctor arg 11 (0 / 0x80), ORed into the select event */
    s32 centerX;            /* 0x4D8: 0x78 (2 players) / 0xF0 */
    s32 *out[5];            /* 0x4DC: commitSelection output pointers */
} SelectWin;                /* 0x4F0 (SelectWin_ctor allocates 0x4F0) */

#define SELECT_ASSERT(name, cond) typedef char select_assert_##name[(cond) ? 1 : -1]
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
SELECT_ASSERT(selmgr_busy, (int)&((SelectionMgr *)0)->busy == 0x4C);
SELECT_ASSERT(win_player, (int)&((SelectWin *)0)->player == 0x3B8);
SELECT_ASSERT(win_cur, (int)&((SelectWin *)0)->curItem == 0x3D0);
SELECT_ASSERT(win_prev, (int)&((SelectWin *)0)->prevItem == 0x3F4);
SELECT_ASSERT(win_mgr, (int)&((SelectWin *)0)->itemsMgr == 0x40C);
SELECT_ASSERT(win_owner, (int)&((SelectWin *)0)->owner == 0x41C);
SELECT_ASSERT(win_fade, (int)&((SelectWin *)0)->fade == 0x480);
SELECT_ASSERT(win_busy, (int)&((SelectWin *)0)->busy == 0x4B4);
SELECT_ASSERT(win_cam, (int)&((SelectWin *)0)->camPos == 0x4C4);
SELECT_ASSERT(win_flags, (int)&((SelectWin *)0)->eventFlags == 0x4D4);
SELECT_ASSERT(win_size, sizeof(SelectWin) == 0x4F0);

#endif
