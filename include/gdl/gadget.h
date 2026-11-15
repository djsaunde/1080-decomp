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

#endif
