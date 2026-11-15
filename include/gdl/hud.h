#ifndef GDL_HUD_H
#define GDL_HUD_H

/* The race HUD object that owns the "Gadget" widgets (game_libs 0xE2A4-0xEAAC,
 * src/game_libs/game_libs_tail.c). Each hud_add<X>Gadget lazily builds the
 * sprite sets it needs (hud_initSprites_*), constructs the gadget with
 * <X>Gadget_ctor(0, ...), stores it in its slot, then links it as a child
 * through the node child list at +0x10 (list_append) and sets the
 * gadget's parent back-link (gadget +0x14). hud_releaseGadgets calls each
 * slot's <X>Gadget_release.
 *
 * Only the fields the hud_* functions touch are named. Slots and value
 * pointers are kept as int where the matched bodies use int arithmetic;
 * a field's width and signedness must not change (READABILITY_PLAN 9.1). */
typedef struct Hud {
    /* 0x00 */ char unk00[0x10];
    /* 0x10 */ char children[0x28];      /* node child list (address taken only) */
    /* 0x38 */ s32 unk38[3];             /* read by hud_addCourseGadget as floats and ints */
    /* 0x44 */ char unk44[0x10];
    /* 0x54 */ s32 pointsListArg;        /* 3rd ctor arg of PointsGadget_ctor_00003138 */
    /* 0x58 */ s32 digitsTall;           /* hud_initSprites_0000E4A8: 16x320 grid, 1x10 cells */
    /* 0x5C */ s32 digitsWide;           /* hud_initSprites_0000E4A8: 160x16 grid, 10x1 cells */
    /* 0x60 */ s32 sprites60;            /* hud_initSprites_0000E5D0 (Speed / Time / Points font) */
    /* 0x64 */ s32 sprites64;            /* hud_initSprites_0000E53C (CountDown) */
    /* 0x68 */ s32 sprites68;            /* hud_initSprites_0000E53C (CountDown) */
    /* 0x6C */ s32 pointsList;           /* PointsGadget_ctor_00003138 */
    /* 0x70 */ s32 speed;                /* SpeedGadget */
    /* 0x74 */ s32 speedValue;           /* f32 * the speed is read from */
    /* 0x78 */ s32 time;                 /* TimeGadget */
    /* 0x7C */ s32 timeValue;            /* s32 * the time (ms) is read from */
    /* 0x80 */ s32 course;               /* CourseGadget */
    /* 0x84 */ s32 countDown;            /* CountDownGadget */
    /* 0x88 */ s32 countDownValue;       /* s32 * the count is read from */
    /* 0x8C */ s32 points;               /* PointsGadget */
    /* 0x90 */ s32 pointsValue;
    /* 0x94 */ s32 points94;             /* PointsGadget_ctor_00000ED0 class (empty release) */
    /* 0x98 */ s32 cool;                 /* CoolGadget */
    /* 0x9C */ s32 coolValue;            /* s32 * the cool value is read from */
} Hud;

typedef char Hud_size_check[sizeof(Hud) == 0xA0 ? 1 : -1];
typedef char Hud_children_check[(int)&((Hud *)0)->children == 0x10 ? 1 : -1];
typedef char Hud_unk38_check[(int)&((Hud *)0)->unk38 == 0x38 ? 1 : -1];
typedef char Hud_pointsListArg_check[(int)&((Hud *)0)->pointsListArg == 0x54 ? 1 : -1];
typedef char Hud_sprites60_check[(int)&((Hud *)0)->sprites60 == 0x60 ? 1 : -1];
typedef char Hud_pointsList_check[(int)&((Hud *)0)->pointsList == 0x6C ? 1 : -1];
typedef char Hud_course_check[(int)&((Hud *)0)->course == 0x80 ? 1 : -1];
typedef char Hud_points_check[(int)&((Hud *)0)->points == 0x8C ? 1 : -1];
typedef char Hud_coolValue_check[(int)&((Hud *)0)->coolValue == 0x9C ? 1 : -1];

#endif /* GDL_HUD_H */
