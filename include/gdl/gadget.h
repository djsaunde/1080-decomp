#ifndef GDL_GADGET_H
#define GDL_GADGET_H

#include "gdl/node.h"

/* A libgdl tunable parameter (0x18), built in place inside a "<Class>::Parms" object by a
 * ctor call (parm, owner Parms, 4-byte tag by value, 1) and then typed: +0xC = the parm type
 * vtable (bootup Data 0xC764 = int parm, 0xC83C = float parm), +0x10 = the default value.
 * The tags are the 4-byte strings after each Parms name in the Data section ("xps", "yps",
 * "lbx", ...). HostIO can edit the value at run time; the owning class reads +0x10 directly
 * (agent-h run 507). */
typedef struct GdlParm {
    /* 0x00 */ char head[0xC];      /* filled by the parm base ctor */
    /* 0x0C */ s32 type;            /* parm type vtable */
    /* 0x10 */ s32 value;
    /* 0x14 */ s32 unk14;           /* 0 */
} GdlParm;

typedef struct GdlParmF {
    /* 0x00 */ char head[0xC];
    /* 0x0C */ s32 type;            /* the float parm type (Data 0xC83C) */
    /* 0x10 */ f32 value;
    /* 0x14 */ s32 unk14;
} GdlParmF;

typedef char GdlParm_size_check[sizeof(GdlParm) == 0x18 ? 1 : -1];

/* "TimeGadget::Parms" (0x98, Data 0xCAAC; ctor TimeGadget_ctor_00013D40, the base ctor's
 * call on +0x2C): the screen position (xps, yps) and the offsets of the two label sprites
 * from it (lbx / lby, l2x / l2y). Defaults 30, 30, 2, -12, 2, -12; each gadget ctor then
 * sets the label offsets (TimeGadgetParms_setLabelPos_00013FEC: TimeGadget -78, -1, -51, 0).
 * CoolGadgetParms_ctor builds the same six tags (Data 0xCC38). */
typedef struct TimeGadgetParms {
    /* 0x00 */ s32 name;            /* "TimeGadget::Parms" */
    /* 0x04 */ s32 unk04;           /* 0 */
    /* 0x08 */ GdlParm xps;         /* screen x of the text */
    /* 0x20 */ GdlParm yps;         /* screen y of the text */
    /* 0x38 */ GdlParm lbx;         /* label1 offset x (from xps) */
    /* 0x50 */ GdlParm lby;         /* label1 offset y */
    /* 0x68 */ GdlParm l2x;         /* label2 offset x */
    /* 0x80 */ GdlParm l2y;         /* label2 offset y */
} TimeGadgetParms;

typedef char TimeGadgetParms_size_check[sizeof(TimeGadgetParms) == 0x98 ? 1 : -1];

/* "TimeGadget::Parms" for the CountDownGadget (0x68, Data 0xCB60; game_libs.c
 * TimeGadgetParms_ctor): xps / yps and the shadow offset shx / shy (defaults 30, 30, 4, -2). */
typedef struct CountDownParms {
    /* 0x00 */ s32 name;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ GdlParm xps;
    /* 0x20 */ GdlParm yps;
    /* 0x38 */ GdlParm shx;         /* second text pass offset x (from xps) */
    /* 0x50 */ GdlParm shy;         /* second text pass offset y */
} CountDownParms;

typedef char CountDownParms_size_check[sizeof(CountDownParms) == 0x68 ? 1 : -1];

/* A HUD gadget (game_libs 0x1CC-0x3430). Every *Gadget_ctor allocates 0x13C
 * (BoardsGadget 0x140), builds the 0x10C Node-derived base in place (a
 * Node_ctor call, the base vtable at +0x28, a call on +0x2C = the Parms ctor), then sets the
 * class name, makes the two sprite sub-objects (import ctor with ids 0x5xxxx),
 * hands them to TimeGadgetParms_set104_00014004 as label1 / label2 and normalises its
 * colour by 255. *_release frees the two sprites; the *_set<Value> methods sprintf into
 * text. TimeGadget_display_000140C4 / BestGadget_display_00014228 (bootup_uso_tail4.c) draw
 * label1 at xps + lbx with labelAlpha, label2 at xps + l2x with alpha, then the text with the
 * font at (xps, yps) (agent-h run 507). */
typedef struct Gadget {
    /* 0x000 */ Node node;          /* node.name = the class name string */
    /* 0x02C */ TimeGadgetParms parms;
    /* 0x0C4 */ f32 colour[4];      /* r, g, b, a in 0..1 (constants / 255): the text colour */
    /* 0x0D4 */ s32 unkD4;          /* 0; passed with colour to the font's colour call */
    /* 0x0D8 */ s32 alpha;          /* 0xFF in every ctor: text and label2 alpha (x the global fade) */
    /* 0x0DC */ s32 labelAlpha;     /* 0x82 in every ctor: label1 alpha */
    /* 0x0E0 */ s32 font;           /* the ctor's second argument: draws text */
    /* 0x0E4 */ char text[0x20];    /* sprintf target of the *_set<Value> methods */
    /* 0x104 */ s32 label1;         /* = &sprite1 (TimeGadgetParms_set104_00014004 arg 1) */
    /* 0x108 */ s32 label2;         /* = &sprite0 */
    /* 0x10C */ char sprite0[0x18]; /* sprite sub-object (import ctor, id 0x5000x) */
    /* 0x124 */ char sprite1[0x18]; /* second sprite sub-object */
} Gadget;

typedef char Gadget_size_check[sizeof(Gadget) == 0x13C ? 1 : -1];
typedef char Gadget_colour_check[(int)&((Gadget *)0)->colour == 0xC4 ? 1 : -1];
typedef char Gadget_text_check[(int)&((Gadget *)0)->text == 0xE4 ? 1 : -1];
typedef char Gadget_label2_check[(int)&((Gadget *)0)->label2 == 0x108 ? 1 : -1];
typedef char Gadget_sprite1_check[(int)&((Gadget *)0)->sprite1 == 0x124 ? 1 : -1];

/* Field access through an untyped pointer (the bodies keep `char *` / `int *` parameters). */
#define GADGET(p)  ((Gadget *)(p))
#define TGPARMS(p) ((TimeGadgetParms *)(p))
#define PARM(p)    ((GdlParm *)(p))
#define PARMF(p)   ((GdlParmF *)(p))

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
    /* 0x2C */ CountDownParms parms;
    /* 0x94 */ f32 colour[4];
    /* 0xA4 */ s32 unkA4;          /* 0; passed with colour */
    /* 0xA8 */ s32 alpha;          /* 0xFF (x the global fade) */
    /* 0xAC */ s32 arg1;           /* font of the seconds text (secsText at xps, yps) */
    /* 0xB0 */ s32 arg2;           /* font of the centiseconds text (csText at xps + shx, yps + shy) */
    /* 0xB4 */ char secsText[8];
    /* 0xBC */ char csText[8];
    /* 0xC4 */ s32 blink;
    /* 0xC8 */ s32 unkC8;          /* the setCount 2nd arg; 0 starts the blink */
    /* 0xCC */ s32 lastSecs;
    /* 0xD0 */ s32 secs;
} CountDownGadget;

typedef char CountDownGadget_size_check[sizeof(CountDownGadget) == 0xD4 ? 1 : -1];

#define COUNTDOWN(p) ((CountDownGadget *)(p))

/* PointsPopup (0xD4, PointsGadget_ctor_00000ED0, name "PointsGadget"): one score pop-up. The 0x168
 * points list (PointsGadget_ctor_00003138) builds 8 of them at +0x2C; PointsGadget_allocSlot_00003B1C
 * (game_libs.c) takes a free one, the RaceTracker_pop* / onGateCleared /
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
