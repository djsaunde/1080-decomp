#ifndef GAME_SUN_H
#define GAME_SUN_H

#ifndef __cplusplus
#include "game/camera.h"
#include "game/rideranim.h" /* Pair2 */
#include "gdl/sprite.h"
#else
/* The edg (NCC) donors include this header by relative path, without -I include (same-directory includes
 * below), and edg rejects gdl/sprite.h's &((T *)0)->f asserts. They get this mirror of include/gdl/sprite.h's
 * Sprite: the same layout and member names, the Node base opaque. Keep the two in step. */
#include "camera.h"
#include "rideranim.h" /* Pair2 */
typedef struct Sprite {
    char node[0x2C];        /* 0x00: Node; vptr at +0x28 */
    s32 unk2C[0x16];
    s32 children;           /* 0x84 */
    s32 unk88[10];
    f32 scaleX;             /* 0xB0 */
    f32 scaleY;             /* 0xB4 */
    f32 offsetX;
    f32 offsetY;
    s16 unkC0;              /* 0xC0: image / material */
    s16 padC2;
    s32 perspective;
    f32 depthBias;
    struct DLContext *startDL; /* 0xCC */
    struct DLContext *endDL;   /* 0xD0 */
    s32 unkD4;
} Sprite;
#endif

/*
 * Sun and LensFlare: the course sky objects of game.uso 0xAE1C-0xC0E8 (agent-g runs 465 / 497;
 * docs/1080_GAME_CLASSES.md section 12).
 *
 * Both are MNodes (the 0xB4-byte matrix node of camera.h CamMNode: local[3] = position relative to the
 * parent, world[3] (0xA0) = world position) with a Sprite child drawn as a billboard.
 *
 * Sun (0x178, vtbl Data 0xC5C, node "sun"): Sun_update places it on the sky sphere from the rot / ele parms
 * (game_uso_func_070338 = sin / cos pairs); Sun_display projects world[3] to the screen, registers the
 * measureVisibility callback with the renderer and draws the sprite at scl * fade. Sun_measureVisibility reads
 * 9 depth-buffer texels around the projected position (0xFFFC = sky) and eases `visibility` toward the
 * fraction that is clear (drops at once, rises slowly).
 *
 * LensFlare (0x278, vtbl Data 0xCEC): display draws numElems LensFlareElems along the line from the sun's
 * screen position through the screen centre, fading them by how directly the camera looks at the sun.
 */

/* one 0x18-byte parm: game_libs parm base + a float (Data 0x860) or int (Data 0x878) subclass */
typedef struct SunParm {
    char pad00[0xC];        /* 0x00: owner list link and the 4-char tag (game_libs parm ctor 0x4A188) */
    void *vptr;             /* 0x0C */
    union {
        f32 f;
        s32 i;
    } value;                /* 0x10: the ctor default, overwritten by the parms file */
    s32 unk14;              /* 0x14: cleared by the ctor */
} SunParm;

/* one 0x24-byte colour parm (vtable Data 0x890): four floats */
typedef struct SunColorParm {
    char pad00[0xC];
    void *vptr;             /* 0x0C */
    f32 value[4];           /* 0x10: r, g, b, a (ctor default 1, 1, 1, 1) */
    s32 unk20;              /* 0x20: cleared by the ctor */
} SunColorParm;

/* "Parms" of Sun (0x8C; tags at game.uso Data 0xD5C..0xD6C, in this order) */
typedef struct SunParms {
    const char *name;       /* 0x00: "Parms" (Data 0xD54); Sun_ctor stores name and head itself (the inlined
                             * 8-byte Parms base) before adding the parms */
    s32 head;               /* 0x04: parm list head (0 from the ctor) */
    SunParm rot;            /* 0x08 (sun value 0x104): azimuth, the second sin / cos of Sun_update */
    SunParm ele;            /* 0x20 (0x11C): elevation, the first sin / cos of Sun_update */
    SunParm scl;            /* 0x38 (0x134): sprite size (default 2.0); display draws scl * fade */
    SunParm mat;            /* 0x50 (0x14C): int: the sprite's material / image (copied to sprite->unkC0) */
    SunColorParm col;       /* 0x68 (0x164): sprite colour (game_uso_func_05B750) */
} SunParms;

/* the 16-byte member-function callback object (vptr Data 0xC44): the renderer calls obj->*pmf. Sun keeps one
 * for Sun_measureVisibility (pmf = {0, -1, Sun_measureVisibility}, Data 0x840) */
typedef struct MemberCallback {
    void *vptr;             /* 0x00 */
    void *obj;              /* 0x04 */
    Pair2 pmf;              /* 0x08: Cfront pointer to member function */
} MemberCallback;

typedef struct Sun {
    CamMNode mnode;         /* 0x000: local[3] = sky direction * distance (Sun_update), world[3] = world position */
    char padB4[0xC4 - 0xB4];
    Sprite *sprite;         /* 0x0C4: billboard (import_000F5ADC(0, "...")): scaleX / scaleY = scl * fade, unkC0 = mat */
    f32 fade;               /* 0x0C8: visibility, scaled down past a renderer distance (fog) limit; 1.0 from the ctor */
    f32 visibility;         /* 0x0CC: 0..1 fraction of the 9 sampled depth texels that are clear (measureVisibility) */
    s32 screenX;            /* 0x0D0: projected position (display) */
    s32 screenY;            /* 0x0D4 */
    void *depthImage;       /* 0x0D8: the renderer's depth buffer image at display time: width +0xB8, height +0xBC */
    MemberCallback visCallback; /* 0x0DC */
    SunParms parms;         /* 0x0EC */
} Sun;

/* one flare ring / disc (0x24; LensFlareElem_init_0000B710 defaults, LensFlareElem_read_0000B884 file order
 * pos, size, image, color, flags, minDot) */
typedef struct LensFlareElem {
    f32 pos;                /* 0x00: position along the sun-to-centre line (0 = at the sun) [0] */
    f32 color[4];           /* 0x04: r, g, b, a [1, 1, 1, 1] */
    f32 size;               /* 0x14: [RoData 0x150] */
    s32 image;              /* 0x18: [0] */
    s32 flags;              /* 0x1C: [8]; LFELEM_FLAG_* */
    f32 minDot;             /* 0x20: drawn only while the camera-to-sun alignment exceeds it [0] */
} LensFlareElem;

/* LensFlareElem.flags. `fade` = LensFlare_display's alignment fade passed to LensFlareElem_draw_0000B750 */
#define LFELEM_FLAG_SQFADE  0x01 /* drawn with the squared (dlv) fade instead of the linear one */
#define LFELEM_FLAG_SIZE    0x02 /* draw: size *= fade */
#define LFELEM_FLAG_SLIDE   0x04 /* draw: pos += fade * 400 */
#define LFELEM_FLAG_FADE    0x08 /* draw: size / alpha use fade (else 1.0) */

#define LENSFLARE_MAX_ELEMS 10

/* "LensFlare::Parms" (0x50; tags fin / dlv / dfm at Data 0xD8C..0xD94) */
typedef struct LensFlareParms {
    const char *name;       /* 0x00: "LensFlare::Parms" (Data 0xD78); stored by LensFlare_create with head */
    s32 head;               /* 0x04: parm list head */
    SunParm fin;            /* 0x08 (flare value 0x23C): the alignment below which nothing is drawn */
    SunParm dlv;            /* 0x20 (0x254): alpha scale of the dimming disc drawn over the screen */
    SunParm dfm;            /* 0x38 (0x26C): the alignment where the dimming starts */
} LensFlareParms;

typedef struct LensFlare {
    CamMNode mnode;         /* 0x000 */
    Sprite *sprite;         /* 0x0B4: the sprite the elements are drawn with */
    LensFlareElem elems[LENSFLARE_MAX_ELEMS]; /* 0x0B8 */
    Sun *sun;               /* 0x220: LensFlare_create arg 2: the sun it follows (its world[3] and fade) */
    LensFlareParms parms;   /* 0x224 */
    u32 numElems;           /* 0x274: elems in use (10 from the ctor; LensFlare_readParms reads the count) */
} LensFlare;

#ifndef __cplusplus
#define SUN_ASSERT(T, f, off) typedef char sun_assert_##T##_##off[(int)&((T *)0)->f == (off) ? 1 : -1]
SUN_ASSERT(Sun, sprite, 0xC4);
SUN_ASSERT(Sun, visibility, 0xCC);
SUN_ASSERT(Sun, depthImage, 0xD8);
SUN_ASSERT(Sun, visCallback, 0xDC);
SUN_ASSERT(Sun, parms, 0xEC);
SUN_ASSERT(Sun, parms.rot.value, 0x104);
SUN_ASSERT(Sun, parms.ele.value, 0x11C);
SUN_ASSERT(Sun, parms.scl.value, 0x134);
SUN_ASSERT(Sun, parms.mat.value, 0x14C);
SUN_ASSERT(Sun, parms.col.value, 0x164);
SUN_ASSERT(LensFlareElem, size, 0x14);
SUN_ASSERT(LensFlareElem, minDot, 0x20);
SUN_ASSERT(LensFlare, elems, 0xB8);
SUN_ASSERT(LensFlare, sun, 0x220);
SUN_ASSERT(LensFlare, parms.fin.value, 0x23C);
SUN_ASSERT(LensFlare, parms.dlv.value, 0x254);
SUN_ASSERT(LensFlare, parms.dfm.value, 0x26C);
SUN_ASSERT(LensFlare, numElems, 0x274);
typedef char sun_assert_parms_size[sizeof(SunParms) == 0x8C ? 1 : -1];
typedef char sun_assert_size[sizeof(Sun) == 0x178 ? 1 : -1];
typedef char sun_assert_elem_size[sizeof(LensFlareElem) == 0x24 ? 1 : -1];
typedef char sun_assert_flare_size[sizeof(LensFlare) == 0x278 ? 1 : -1];
#endif

#endif /* GAME_SUN_H */
