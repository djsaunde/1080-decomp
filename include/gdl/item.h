#ifndef GDL_ITEM_H
#define GDL_ITEM_H

#include "node.h" /* same directory: NCC donors include this header by relative path (C++-safe: no offset asserts) */

/* libgdl item types (game_libs post0b: ItemType_ctor 0x3D16C, ColourItem_ctor 0x3D5BC,
 * ColourItemType_ctor 0x3D7F8, ItemMgr_ctor 0x3E238, ItemMgr_addType 0x3E904,
 * ItemMgr_findType_0003EA98; docs/1080_SYSTEM_LAYER.md section 9).
 *
 * An ItemType is a Node whose children are its items (ItemType_addItem attaches through
 * Node.children / parent). Types are keyed by a 4-byte tag (ItemMgr_findType compares it as
 * one word; ItemType_load prints it with "Loading ItemType '%s' (%d)") and chained through
 * next from ItemMgr.types (ItemMgr_addType pushes at the head). The vtable slots are in
 * docs/1080_SYSTEM_LAYER.md: 10 newItem, 11 buildItems, 14 getItem. */
typedef struct ItemType {
    /* 0x00 */ Node node;          /* node.children = the items */
    /* 0x2C */ s32 tag;            /* 4-char key, by-value ctor arg */
    /* 0x30 */ struct ItemType *next; /* ItemMgr type list */
    /* 0x34 */ s32 builtHead;      /* buildItems (Colour / SNode): the first newItem(0) result; each
                                    * later one is chained into the previous item's next word */
    /* 0x38 */ s32 searchDown;     /* 0 at ctor; SNodeItemType_vslot15 (nearest item by key +0x64):
                                    * set = the largest key below ref, clear = the smallest above */
    /* 0x3C */ s32 unk3C;          /* 0 at ctor; ItemMgr_removeItems clears it with the item list */
    /* 0x40 */ s32 unk40;          /* ctor arg 2; SNodeItemType_newItem attaches this Node under each
                                    * new SNode item (game_uso.c reads it too, so the name stays) */
    /* 0x44 */ s32 typeId;         /* -1 at ctor; ItemMgr_addType arg 2: the type's class bit
                                    * (item_registerTemplates passes 4, ...); ItemMgr_Load loads
                                    * a type only when it shares a bit with ItemMgr.classMask */
} ItemType;

/* ColourItemType (0x58, vtable D+0x1F228): an RGBA quad, default (1, 0, 0, 1); its display
 * calls gfx_setPrimColour on it */
typedef struct ColourItemType {
    /* 0x00 */ ItemType type;
    /* 0x48 */ f32 colour[4];
} ColourItemType;

/* ItemMgr (0x38) */
typedef struct ItemMgr {
    /* 0x00 */ Node node;
    /* 0x2C */ ItemType *types;    /* list head through ItemType.next */
    /* 0x30 */ s32 unk30;          /* 0 at ctor; ItemMgr_Load looks an unknown tag up in the 12-byte
                                    * records it points at ({tag, ?, class bits}) */
    /* 0x34 */ s32 classMask;      /* ctor arg 2 (-1 = every class); item_registerTemplates stores the
                                    * mode's class bits (0x1 / 0x2 / 0x4 / 0x8 / 0x20, 0xFFFF for arg 1).
                                    * ItemMgr_Load only builds a template whose class bits & classMask
                                    * and only loads a type whose typeId & classMask */
} ItemMgr;

typedef char ItemType_size_check[sizeof(ItemType) == 0x48 ? 1 : -1];
typedef char ColourItemType_size_check[sizeof(ColourItemType) == 0x58 ? 1 : -1];
typedef char ItemMgr_size_check[sizeof(ItemMgr) == 0x38 ? 1 : -1];

#define ITEMTYPE(p) ((ItemType *)(p))
#define COLOURITEMTYPE(p) ((ColourItemType *)(p))
#define ITEMMGR(p) ((ItemMgr *)(p))

#endif /* GDL_ITEM_H */
