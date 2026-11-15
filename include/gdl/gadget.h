#ifndef GDL_GADGET_H
#define GDL_GADGET_H

#include "gdl/node.h"

/* A HUD gadget (game_libs 0x1CC-0x3430). Every *Gadget_ctor allocates 0x13C
 * (BoardsGadget 0x140), builds the 0x10C Node-derived base in place (a
 * Node_ctor call, the base vtable at +0x28, a call on +0x2C), then sets the
 * class name, makes the two sprite sub-objects (import ctor with ids 0x5xxxx)
 * and normalises its colour by 255. *_release frees the two sprites; the
 * *_set<Value> methods sprintf into text. Field names past the Node are
 * from those uses only. */
typedef struct Gadget {
    /* 0x000 */ Node node;          /* node.name = the class name string */
    /* 0x02C */ char unk2C[0xC4 - 0x2C];
    /* 0x0C4 */ f32 colour[4];      /* r, g, b, a in 0..1 (constants / 255) */
    /* 0x0D4 */ s32 unkD4;
    /* 0x0D8 */ s32 unkD8;          /* 0xFF in every ctor */
    /* 0x0DC */ s32 unkDC;          /* 0x82 in every ctor */
    /* 0x0E0 */ s32 arg;            /* the ctor's second argument */
    /* 0x0E4 */ char text[0x28];    /* sprintf target of the *_set<Value> methods */
    /* 0x10C */ char sprite0[0x18]; /* sprite sub-object (import ctor, id 0x5000x) */
    /* 0x124 */ char sprite1[0x18]; /* second sprite sub-object */
} Gadget;

typedef char Gadget_size_check[sizeof(Gadget) == 0x13C ? 1 : -1];
typedef char Gadget_colour_check[(int)&((Gadget *)0)->colour == 0xC4 ? 1 : -1];
typedef char Gadget_text_check[(int)&((Gadget *)0)->text == 0xE4 ? 1 : -1];
typedef char Gadget_sprite1_check[(int)&((Gadget *)0)->sprite1 == 0x124 ? 1 : -1];

/* BoardsGadget (0x140, BoardsGadget_ctor 0x11A4): the same 0x10C head as
 * Gadget (colour normalised from one constant, +0xD4/+0xD8 ints), then a
 * board count and one sprite sub-object instead of two. BoardsGadget_display
 * draws `count` board icons, BoardsGadget_setCount (0x1354) stores it. */
typedef struct BoardsGadget {
    /* 0x000 */ Node node;
    /* 0x02C */ char unk2C[0xC4 - 0x2C];
    /* 0x0C4 */ f32 colour[4];
    /* 0x0D4 */ s32 unkD4;
    /* 0x0D8 */ s32 unkD8;
    /* 0x0DC */ char unkDC[0x10C - 0xDC];
    /* 0x10C */ s32 count;
    /* 0x110 */ char sprite[0x30];
} BoardsGadget;

typedef char BoardsGadget_size_check[sizeof(BoardsGadget) == 0x140 ? 1 : -1];
typedef char BoardsGadget_count_check[(int)&((BoardsGadget *)0)->count == 0x10C ? 1 : -1];

/* CountDownGadget (0xD4, CountDownGadget_ctor 0xA8C) is built on a smaller
 * 0xC4 Node-derived base. CountDownGadget_setCount (0xB94, the ms -> "%d"
 * seconds / centiseconds formatter) uses the tail; +0xC4 counts frames and
 * bit 3 blinks the last-ten-seconds text. */
typedef struct CountDownGadget {
    /* 0x00 */ Node node;
    /* 0x2C */ char unk2C[0x94 - 0x2C];
    /* 0x94 */ f32 colour[4];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;          /* 0xFF */
    /* 0xAC */ s32 arg1;
    /* 0xB0 */ s32 arg2;
    /* 0xB4 */ char secsText[8];
    /* 0xBC */ char csText[8];
    /* 0xC4 */ s32 blink;
    /* 0xC8 */ s32 unkC8;          /* the setCount 2nd arg; 0 starts the blink */
    /* 0xCC */ s32 lastSecs;
    /* 0xD0 */ s32 secs;
} CountDownGadget;

typedef char CountDownGadget_size_check[sizeof(CountDownGadget) == 0xD4 ? 1 : -1];

/* PointsPopup (0xD4, PointsGadget_ctor_00000ED0, name "PointsGadget"): one score pop-up. The 0x168
 * points list (PointsGadget_ctor_00003138) builds 8 of them at +0x2C; PointsGadget_allocSlot_00003B1C
 * (game_libs.c GlSlot3B1C is the same object) takes a free one, the RaceTracker_pop* / onGateCleared /
 * onCheckpoint functions colour it, and PointsGadget_update_00003430 (donor game_libs_o2_3430.c)
 * runs it: state 5.. counts timer down while moving pos by vel, then flies it to (100, 40) or
 * (200, 20) in 10.625 steps. Names are from those bodies (agent-h run 496). */
typedef struct PointsPopup {
    /* 0x00 */ Node node;          /* node.name = "PointsGadget" */
    /* 0x2C */ char unk2C[0x44 - 0x2C];
    /* 0x44 */ f32 x;              /* screen x: += velX; popComboBonus adds 70 */
    /* 0x48 */ char unk48[0x5C - 0x48];
    /* 0x5C */ f32 y;              /* screen y: += velY (rises past 110 in state 5); the pop functions set 70 */
    /* 0x60 */ s32 unk60;
    /* 0x64 */ f32 colour[4];      /* r, g, b, a in 0..1 (constants / 255) */
    /* 0x74 */ s32 unk74;          /* 0 (ctor) */
    /* 0x78 */ s32 alpha;          /* 255 (ctor, allocSlot, popTrickPointsCombo) */
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;          /* -18 (allocSlot) */
    /* 0x88 */ char unk88[0x94 - 0x88];
    /* 0x94 */ s32 state;          /* 0 = free; allocSlot sets 5 */
    /* 0x98 */ s32 value;          /* the points shown (allocSlot arg 1, "%d") */
    /* 0x9C */ s32 timer;          /* frames left in the current state (pop functions: 14 / 30 / 230) */
    /* 0xA0 */ s32 font;           /* the digit sprite: one of the list's sprites (+0x108 / 0x120 / 0x138 / 0x150) */
    /* 0xA4 */ f32 velX;
    /* 0xA8 */ f32 velY;
    /* 0xAC */ f32 unkAC;
    /* 0xB0 */ s32 arg;            /* ctor arg 1; allocSlot copies the list's +0x54 */
    /* 0xB4 */ char text[0xD4 - 0xB4]; /* sprintf target (PointsGadget_setValue, allocSlot) */
} PointsPopup;

typedef char PointsPopup_size_check[sizeof(PointsPopup) == 0xD4 ? 1 : -1];
typedef char PointsPopup_state_check[(int)&((PointsPopup *)0)->state == 0x94 ? 1 : -1];

#define POPUP(p) ((PointsPopup *)(p))

#endif
