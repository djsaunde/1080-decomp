#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;

/* Shared input layout: 673FC initializes it, 679BC/67A10 configure frames,
 * and 675A4 passes the same instance to 34CC8. Meanings remain provisional. */
typedef struct GlInputFrame675A4 {
    s8 field_00;
    s8 field_01;
    s16 field_02;
} GlInputFrame675A4;

typedef struct GlInput675A4 {
    f32 field_00, field_04;
    s8 field_08, field_09;
    u8 unknown_0A[6];
    s32 field_10, field_14, field_18, field_1C;
    s32 field_20, field_24;
    s32 field_28, field_2C, field_30;
    s32 field_34, field_38, field_3C, field_40;
    f32 field_44, field_48;
    s32 field_4C, field_50, field_54;
    s32 *field_58;
    GlInputFrame675A4 *field_5C;
    u8 unknown_60[8];
    s32 field_68, field_6C, field_70;
    s16 field_74, field_76;
    u8 *field_78;
    s16 field_7C;
} GlInput675A4;

/* Shared display-slot prefix evidenced by game_libs_func_00003298 (reset),
 * gl_func_00003430 (update), gl_func_000038F4 (dispatch), and allocators
 * gl_func_00003B1C/00003C0C/00003DB8. func_000144B4 initializes +44/+5C
 * and +88/+8C. This prefix does not establish the full allocation size. */
typedef struct {
    char pad_00[0x28];
    void *field_28;
    char pad_2C[0x18];
    float field_44;
    char pad_48[0x14];
    float field_5C;
    char pad_60[4];
    float field_64;
    float field_68;
    float field_6C;
    float field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    float field_88;
    float field_8C;
    char pad_90[4];
    int field_94;
    int field_98;
    int field_9C;
    void *field_A0;
    float field_A4;
    float field_A8;
    float field_AC;
    int field_B0;
    char field_B4[1];
} GlDisplaySlot;
/* field_B4[1] anchors trailing text; its capacity remains unknown. */

/* Shared partial base prefix: relocation-linked constructor 3800C and
 * bootup 1E08/27E8/2F90/3B78/7C74/84A0 use the same instances. This is
 * an observed prefix, not a recovered original class or complete derived size.
 * The constructor stores an opaque O32 word at +0xC; 1E08 later stores a
 * descriptor pointer there. Keep both views without asserting all words are
 * pointers. The +0x18/+0x24 embedded initialization storage stays opaque.
 */
typedef union BootupObjectCarrier {
    s32 word;
    void *pointer;
} BootupObjectCarrier;
typedef struct BootupObjectBase {
    u32 field_00;
    s32 field_04;
    u32 field_08;
    BootupObjectCarrier field_0c;
    void *field_10;
    struct BootupObjectBase *field_14;
    u8 unk_18[4];
    s32 field_1c;
    s32 field_20;
    u8 unk_24[4];
    void *field_28;
} BootupObjectBase;



/* Command prefix shared by 0836C/0CDDC/38830/38964/39624/39B58/
 * 3D620/52994. Original relocation-backed fallback calls preserve the same
 * packet pointer; observed 38964 dispatch also forwards it. This is an
 * eight-byte prefix, not a claim about every command's complete extent.
 * The carrier at +4 is tag-dependent: a word/flag or a payload pointer.
 * Only the lookup requests (tags 13/14) establish an object reply at +8;
 * 38BB8 constructs tag 14 and consumes that reply. Object classes and
 * ignored versus forwarded callback return declarations remain separate.
 */
typedef union GLCommandCarrier {
    s32 word;
    void *pointer;
} GLCommandCarrier;
typedef struct GLCommandHeader {
    s32 field_00;
    GLCommandCarrier field_04;
} GLCommandHeader;
typedef struct GLCommandLookup {
    GLCommandHeader field_00;
    void *field_08;
} GLCommandLookup;
/* These are distinct tag-dependent payload views, not one interchangeable
 * word/halfword contract. Tag 14 loads a signed halfword at byte offset 4
 * (the first halfword of that word on this big-endian target).
 */
typedef struct GLCommandPayload12 {
    u8 unknown_00[4];
    s32 field_04;
} GLCommandPayload12;
typedef struct GLCommandPayload13 {
    u8 *field_00;
} GLCommandPayload13;
typedef struct GLCommandPayload14 {
    u8 unknown_00[4];
    s16 field_04;
} GLCommandPayload14;

/* Relocation-linked root/view family: original symbol154 is global storage;
 * its +0x254 slot supplies the root used by 063B4/07EC8/082F8/CFA0,
 * 4F704 and 48AEC. It is not CFA0's BootupState self. 48AEC's original
 * slot-load/root+0x158 pairs establish the existing rendering-object type.
 * These are partial views, not original class names or complete object sizes.
 * field_98 anchors indexed float storage; its full extent is unproved.
 * The child matrix accesses in 063B4 establish the 0x10 row stride, while
 * 4F704 reads the +0xBC/+0xCC/+0xDC lane. Row padding stays opaque.
 */
typedef struct { u32 command, color; } GL470E4Packet;
typedef struct { GL470E4Packet *packets; s32 count; } GL470E4List;
/* The second stream advances by0x48; its first four RGBA-shaped groups
 * expose RGB bytes only. The remaining bytes and alpha meanings are unknown. */
typedef struct { u8 lanes[4][4]; u8 unknown_10[0x38]; } GLRenderRecord48;
typedef struct { GLRenderRecord48 *packets; s32 count; } GLRenderRecord48List;
typedef struct { u8 pad[0xC]; GL470E4List *list; u8 pad_10[0x1C]; GLRenderRecord48List *field_2C; } GL470E4Object;
typedef struct GLRootChild {
    u8 unknown_00[0xA0];
    f32 field_A0, field_A4, field_A8;
    u8 unknown_AC[8];
    f32 field_B4, field_B8, field_BC;
    u8 unknown_C0[4];
    f32 field_C4, field_C8, field_CC;
    u8 unknown_D0[4];
    f32 field_D4, field_D8, field_DC;
} GLRootChild;
/* Four opaque words copied at root+DC; arithmetic meaning is unknown. */
typedef struct GLRootWords4 { u32 word_00, word_04, word_08, word_0C; } GLRootWords4;
typedef struct GL470E4Context {
    u8 unknown_00[0x70];
    GLRootChild *field_70;
    u8 unknown_74[4];
    u32 field_78;
    u8 unknown_7C[4];
    f32 field_80;
    u8 unknown_84[0x14];
    f32 field_98;
    u8 unknown_9C[0x10];
    f32 field_AC, field_B0;
    u8 unknown_B4[0x28];
    GLRootWords4 field_DC;
    u8 unknown_EC[0x1C];
    s32 field_108, field_10C, field_110;
    u8 unknown_114[0x44];
    GL470E4Object *object; /* +0x158; retain existing render-family name */
} GL470E4Context;

/* Shared object prefix evidenced by 38DC0/39094/393B8/39EE4/39F7C.
 * 04948 and game.uso B420 pass the same instance after changing its +0x30
 * transform. Fields beyond this prefix stay opaque in those callers.
 * Dispatch adjustment/function pairs are +10/+14, +18/+1C, +20/+24;
 * 39094 additionally consumes the result of +58/+5C. Unused callback
 * results do not establish their original return types; void models the
 * observed calls without promising a result. No table ownership is changed.
 */
typedef struct GlObjectPrefix GlObjectPrefix;
typedef struct GlObjectLink GlObjectLink;
typedef struct GlObjectDispatch GlObjectDispatch;
struct GlObjectLink {
    GlObjectPrefix *field_00;
    GlObjectLink *field_04;
};
struct GlObjectDispatch {
    u8 pad_00[0x10];
    s16 field_10;
    u8 pad_12[2];
    void (*field_14)();
    s16 field_18;
    u8 pad_1A[2];
    void (*field_1C)();
    s16 field_20;
    u8 pad_22[2];
    void (*field_24)();
    u8 pad_28[0x30];
    s16 field_58;
    u8 pad_5A[2];
    s32 (*field_5C)(s32);
};
struct GlObjectPrefix {
    u8 pad_00[4];
    void *field_04;
    u32 field_08;
    s32 field_0C;
    GlObjectLink *field_10;
    GlObjectPrefix *field_14;
    u32 field_18;
    u8 pad_1C[4];
    u32 field_20;
    u8 pad_24[4];
    GlObjectDispatch *field_28;
    u32 field_2C;
    f32 field_30[16];
    f32 field_70[16];
    s32 field_B0;
};

/* Shared 0x3C prefix: game.uso 041C0/03A28/AC78/11A94 pass the same
 * allocated instance to game_libs gl_func_0003D5BC (bootup Text+0x51C28).
 * The initializer stores an address at +0x28, a zero word at +0x2C,
 * and float zeros at +0x30/+0x34/+0x38. Other field meanings and the
 * original class name are unknown; no unrelated equal-offset class is implied.
 */
typedef struct SnwBase3D5BC {
    u8 unknown_00[0x28];
    void *field_28;
    s32 field_2C;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} SnwBase3D5BC;

/* Partial views shared by the 63DC4/64124/63F34 and 8B44/8BD4 family.
 * Original field meanings and bytes inside unknown spans are not established.
 * The two context slots at 0x16C and 0x170 remain distinct fields. */
typedef struct SnwRecordVec3 {
    f32 field_00, field_04, field_08;
} SnwRecordVec3;

typedef struct SnwRecordContext {
    u8 unknown_000[0xB4];
    SnwRecordVec3 field_0B4;
    u8 unknown_0C0[0x2A0];
    f32 field_360;
    u8 unknown_364[0x5D4];
    s32 field_938;
    SnwRecordVec3 field_93C;
    u8 unknown_948[0x20];
    f32 field_968;
    u8 unknown_96C[4];
    f32 field_970;
    u8 unknown_974[0x30];
    s16 field_9A4;
    u8 unknown_9A6[0x6A];
    u16 field_A10;
} SnwRecordContext;

typedef struct SnwBaseRecord {
    u8 unknown_000[0x28];
    void *field_028;
    u8 unknown_02C[0xDC];
    f32 field_108, field_10C, field_110;
    SnwRecordVec3 field_114;
    SnwRecordVec3 field_120;
    f32 field_12C, field_130, field_134;
    u8 unknown_138[0xC];
    f32 field_144, field_148;
    s32 field_14C;
    f32 field_150, field_154;
    u8 unknown_158[8];
    s32 field_160;
    f32 field_164;
    s32 field_168;
    SnwRecordContext *field_16C;
} SnwBaseRecord;

typedef struct SnwExtendedRecord {
    SnwBaseRecord base;
    SnwRecordContext *field_170;
    u16 field_174;
    u8 unknown_176[2];
} SnwExtendedRecord;

/* Shared PI request/queue layout: 35440 -> 6EF64, also 356FC,
 * 6D964 and 6DD14. Names retain observed offsets. */
struct OSPiHandle_s;
typedef struct {
    void *field_00;
    void *field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    void **field_14;
} GlMesgQueue;

typedef struct {
    u16 field_00;
    u8 field_02;
    u8 field_03;
    GlMesgQueue *field_04;
    void *field_08;
    u32 field_0C;
    u32 field_10;
    struct OSPiHandle_s *field_14;
} GlIoMesg;

/* Shared rendering instance: existing display family plus B274/4E1BC evidence. */
typedef struct {
    u8 pad_00[0x198];
    void *field_198[10];
    s32 field_1C0;
    u8 pad_1C4[0x58];
    s32 field_21C;
    s32 field_220;
    s32 field_224;
    s32 field_228;
    u8 pad_22C[0x28];
    GL470E4Context *context;
    u8 pad_258[0x34];
    s32 field_28C;
    u8 pad_290[0x10];
    f32 field_2A0;
} GL470E4Self;

/* Partial views shared by 11AB4, 11C70/11CA4 and 11E00/11FA8.
 * Original relocation/argument flow carries the symbol-272 owner through
 * 10FEC -> 11AB4 -> 11C70; registration uses 11CA4. Indexed fields below
 * are one-element offset anchors, NOT recovered sequence capacities.
 * Unknown byte ranges preserve offsets, not the full objects' sizes.
 * The mask words use u32 for their bit patterns; original signedness is
 * unknown. Counts are signed, and field_128 uses the -1 scan sentinel.
 */
typedef struct BootupSlotView {
    u8 unknown_00[0x18];
    u32 field_18;
    u32 field_1C;
} BootupSlotView;

typedef struct BootupSlotOwnerView {
    u8 unknown_00[0xC0];
    BootupSlotView *field_C0[1];
    u8 unknown_C4[0x1C];
    BootupSlotView *field_E0[1];
    u8 unknown_E4[0x1C];
    s32 field_100[1]; /* O32 word carrier; original element type unknown. */
    u8 unknown_104[0x1C];
    s32 field_120;
    s32 field_124;
    s32 field_128;
    u8 unknown_12C[0x5C];
    s32 field_188;
    s32 field_18C;
} BootupSlotOwnerView;


#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* COMMON_H */
