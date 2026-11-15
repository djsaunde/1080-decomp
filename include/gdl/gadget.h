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

/* A float "TimeGadget::Parms" (0x38, Data 0xCAD8; ctor FloatPosParms_ctor): two float
 * parms xps / yps (tags "xps" / "yps" = sFloatParmsTags_CAE8[1] / [2], default 30). PointsPopup's
 * screen position (agent-h run 509). */
typedef struct FloatPosParms {
    /* 0x00 */ s32 name;            /* "TimeGadget::Parms" */
    /* 0x04 */ s32 unk04;           /* 0 */
    /* 0x08 */ GdlParmF xps;
    /* 0x20 */ GdlParmF yps;
} FloatPosParms;

typedef char FloatPosParms_size_check[sizeof(FloatPosParms) == 0x38 ? 1 : -1];

/* PointsPopup (0xD4, PointsGadget_ctor_00000ED0, name "PointsGadget"): one score pop-up. The 0x168
 * points list (PointsGadget_ctor_00003138) builds 8 of them at +0x2C; PointsGadget_allocSlot_00003B1C
 * (game_libs.c) takes a free one, the RaceTracker_pop* / onGateCleared /
 * onCheckpoint functions colour it, and PointsGadget_update_00003430 (donor game_libs_o2_3430.c)
 * runs it: state 5.. counts timer down while moving pos by vel, then flies it to (100, 40) or
 * (200, 20) in 10.625 steps. Names are from those bodies (agent-h run 496). The position is the
 * float Parms at +0x2C (parms.xps.value = x at 0x44, parms.yps.value = y at 0x5C): the bootup
 * helpers PointsPopup_setPosScale (x, y, scaleX, scaleY), PointsPopup_setScale
 * (scale), PointsPopup_slideStep (y += velY up to a limit) and gadget_decodeStringB4_00014598
 * (text) work on it (callers: uso-imports.py; agent-h run 509). */
typedef struct PointsPopup {
    /* 0x00 */ Node node;          /* node.name = "PointsGadget" */
    /* 0x2C */ FloatPosParms parms; /* xps.value: screen x (+= velX; popComboBonus adds 70);
                                    * yps.value: screen y (+= velY, rises past 110 in state 5; the
                                    * pop functions set 70) */
    /* 0x64 */ f32 colour[4];      /* r, g, b, a in 0..1 (constants / 255) */
    /* 0x74 */ s32 unk74;          /* 0 (ctor) */
    /* 0x78 */ s32 alpha;          /* 255 (ctor, allocSlot, popTrickPointsCombo) */
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;          /* -18 (allocSlot) */
    /* 0x88 */ f32 scaleX;         /* setPosF arg 3; update grows it by 0.1 to 1.0 */
    /* 0x8C */ f32 scaleY;
    /* 0x90 */ s32 unk90;
    /* 0x94 */ s32 state;          /* 0 = free; allocSlot sets 5 */
    /* 0x98 */ s32 value;          /* the points shown (allocSlot arg 1, "%d") */
    /* 0x9C */ s32 timer;          /* frames left in the current state (pop functions: 14 / 30 / 230) */
    /* 0xA0 */ s32 font;           /* the digit sprite: one of the list's sprites (+0x108 / 0x120 / 0x138 / 0x150) */
    /* 0xA4 */ f32 velX;
    /* 0xA8 */ f32 velY;
    /* 0xAC */ f32 accelY;         /* update: velY += accelY */
    /* 0xB0 */ s32 arg;            /* ctor arg 1; allocSlot copies the list's +0x54 */
    /* 0xB4 */ char text[0xD4 - 0xB4]; /* sprintf target (PointsGadget_setValue, allocSlot);
                                    * gadget_decodeStringB4_00014598 deciphers it in place */
} PointsPopup;

typedef char PointsPopup_size_check[sizeof(PointsPopup) == 0xD4 ? 1 : -1];
typedef char PointsPopup_state_check[(int)&((PointsPopup *)0)->state == 0x94 ? 1 : -1];
typedef char PointsPopup_y_check[(int)&((PointsPopup *)0)->parms.yps.value == 0x5C ? 1 : -1];

#define POPUP(p) ((PointsPopup *)(p))

/* An RGBA colour in 0..1 (CoolGadget's state colours; CoolGadget_display copies one whole). */
typedef struct GadgetRGBA {
    f32 r, g, b, a;
} GadgetRGBA;

/* CoolGadget (0x198, CoolGadget_ctor 0x15FC, name Data 0xCC58; game_libs.c): the 16-segment
 * "cool" meter (RaceTracker +0x98). Same Node + TimeGadgetParms head as Gadget
 * (CoolGadgetParms_ctor, Data 0xCC38). CoolGadget_display steps cur one unit per frame toward
 * target, then seg = clamp(total - cur, 0, total) / total * 16: seg >= 12 pulses the bar colour
 * between pulseLo and pulseHi (three phase += step calls of a sine-like float import), 4..11
 * uses midColour, 0..3 lowColour. It draws label1 at xps + lbx with labelAlpha, label2 at
 * xps + l2x, then with m = seg * 4 + 2: the lit bar (barSprite, width m - 4, in the state
 * colour), the unlit rest of the bar (barSprite again, from xps + m, in `colour`) and the tip
 * (capSprite at xps + m - 3, state colour). The ctor: lowColour = midColour = {0.757, 0, 0, 0}
 * (red), pulseLo = {0.482, 0, 0, 0}, pulseHi = {1, 0.965, 0, 0} (the full bar pulses red ..
 * yellow), colour = {0.035, 0.608, 0.467, 0}, cur = target = 100, labelAlpha 0x82,
 * total = arg 1, step 0.18; label1 / label2 = &label1Sprite / &label2Sprite, label offsets
 * -73, -2, -50, -1
 * (agent-h run 509). */
typedef struct CoolGadget {
    /* 0x000 */ Node node;
    /* 0x02C */ TimeGadgetParms parms;
    /* 0x0C4 */ s32 seg;            /* bar segments lit, 0..16 */
    /* 0x0C8 */ s32 cur;            /* displayed value: steps 1 per frame toward target */
    /* 0x0CC */ s32 target;         /* CoolGadget_setValue */
    /* 0x0D0 */ s32 labelAlpha;     /* 0x82: label1 alpha (x the global fade scale) */
    /* 0x0D4 */ s32 total;          /* ctor arg 1: the full-scale value */
    /* 0x0D8 */ f32 colour[4];      /* the unlit part of the bar */
    /* 0x0E8 */ GadgetRGBA lowColour;  /* seg < 4 */
    /* 0x0F8 */ GadgetRGBA midColour;  /* 4 <= seg < 12 */
    /* 0x108 */ GadgetRGBA pulseLo;    /* seg >= 12: lerp lo .. hi */
    /* 0x118 */ GadgetRGBA pulseHi;
    /* 0x128 */ f32 phase;          /* += step, three times per frame */
    /* 0x12C */ f32 step;           /* 0.18 */
    /* 0x130 */ s32 label1;         /* CoolGadget_setLabels_000017F4 arg 1 (&label1Sprite) */
    /* 0x134 */ s32 label2;         /* arg 2 (&label2Sprite) */
    /* 0x138 */ char barSprite[0x18];   /* id 0x5000A: the lit bar (+0x10 image, +0x20 width) */
    /* 0x150 */ char capSprite[0x18];   /* id 0x50009: the bar end */
    /* 0x168 */ char label1Sprite[0x18]; /* id 0x5001C */
    /* 0x180 */ char label2Sprite[0x18]; /* id 0x5000B */
} CoolGadget;

typedef char CoolGadget_size_check[sizeof(CoolGadget) == 0x198 ? 1 : -1];
typedef char CoolGadget_colour_check[(int)&((CoolGadget *)0)->colour == 0xD8 ? 1 : -1];
typedef char CoolGadget_phase_check[(int)&((CoolGadget *)0)->phase == 0x128 ? 1 : -1];
typedef char CoolGadget_sprite_check[(int)&((CoolGadget *)0)->label2Sprite == 0x180 ? 1 : -1];

#define COOLG(p) ((CoolGadget *)(p))

#endif
