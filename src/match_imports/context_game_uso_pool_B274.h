/* Exact local type/declaration context; function bodies omitted. */
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
typedef struct { u32 command, color; } GL470E4Packet;
typedef struct { GL470E4Packet *packets; s32 count; } GL470E4List;
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
    GL470E4Object *object;
} GL470E4Context;
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
typedef struct SnwBase3D5BC {
    u8 unknown_00[0x28];
    void *field_28;
    s32 field_2C;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} SnwBase3D5BC;
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
    s32 field_100[1];
    u8 unknown_104[0x1C];
    s32 field_120;
    s32 field_124;
    s32 field_128;
    u8 unknown_12C[0x5C];
    s32 field_188;
    s32 field_18C;
} BootupSlotOwnerView;
const unsigned char game_uso_B274_original_rodata_prefix[0x128] = {
    0x3e, 0x2a, 0xaa, 0xab, 0x00, 0x00, 0x09, 0x74, 0x00, 0x00, 0x0a, 0x18, 0x00, 0x00, 0x0a, 0x94,
    0x00, 0x00, 0x0a, 0xc8, 0x00, 0x00, 0x0a, 0xc8, 0x00, 0x00, 0x0a, 0x9c, 0x00, 0x00, 0x0a, 0xa4,
    0x00, 0x00, 0x09, 0xe0, 0x00, 0x00, 0x09, 0xe8, 0x00, 0x00, 0x09, 0xf0, 0x00, 0x00, 0x09, 0xf8,
    0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x08, 0x00, 0x00, 0x0a, 0x5c, 0x00, 0x00, 0x0a, 0x64,
    0x00, 0x00, 0x0a, 0x6c, 0x00, 0x00, 0x0a, 0x74, 0x00, 0x00, 0x0a, 0x7c, 0x00, 0x00, 0x0a, 0x84,
    0x00, 0x00, 0x0d, 0xc0, 0x00, 0x00, 0x0e, 0x10, 0x00, 0x00, 0x0e, 0x60, 0x00, 0x00, 0x0e, 0xac,
    0x6d, 0x72, 0x6b, 0x00, 0x6d, 0x6b, 0x31, 0x00, 0x6d, 0x6b, 0x32, 0x00, 0x6d, 0x6b, 0x33, 0x00,
    0x6d, 0x6b, 0x34, 0x00, 0x6d, 0x6b, 0x35, 0x00, 0x6d, 0x6b, 0x36, 0x00, 0x6d, 0x6b, 0x37, 0x00,
    0x6d, 0x6b, 0x38, 0x00, 0x6d, 0x6b, 0x39, 0x00, 0x6d, 0x6b, 0x6c, 0x00, 0x6d, 0x6b, 0x72, 0x00,
    0x40, 0x49, 0x0f, 0xdb, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xc6, 0x1c, 0x40, 0x00,
    0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0xc5, 0x3b, 0x80, 0x00, 0xc6, 0x6a, 0x60, 0x00,
    0xc6, 0x6a, 0x60, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00,
    0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00,
    0xc6, 0x1c, 0x40, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0x3f, 0x8c, 0xcc, 0xcd, 0xc5, 0xbb, 0x80, 0x00,
    0xc5, 0xbb, 0x80, 0x00, 0xc5, 0xbb, 0x80, 0x00, 0x40, 0x19, 0x99, 0x9a, 0x3e, 0xcc, 0xcc, 0xcd,
    0x3e, 0x99, 0x99, 0x9a, 0x3f, 0x4c, 0xcc, 0xcd, 0xc7, 0xc3, 0x50, 0x00, 0x45, 0x3b, 0x80, 0x00,
    0x00, 0x00, 0x96, 0xa8, 0x00, 0x00, 0x97, 0x0c, 0x00, 0x00, 0x96, 0xa8, 0x00, 0x00, 0x96, 0xe0,
    0x7f, 0x7f, 0xff, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0xef, 0x5c, 0x28, 0xf5, 0xc2, 0x8f, 0x5c,
};
extern char import_8005C108;
extern GL470E4Self game_uso_B274_context[];
extern char import_8005C1A8;
extern char game_uso_D_807FFA40;
extern char game_uso_D_807FFA48;
extern char game_uso_D_807FFA50;
extern char game_uso_D_807FFA58;
typedef struct {
    void *field_00;
    void *field_04;
    s16 field_08;
    s16 field_0A;
    u32 field_0C;
} GameUsoB274FieldDC;
extern int game_uso_func_063734(GL470E4Context *camera, float *position, int viewport_offset, float *reciprocal_w);
extern int import_000F8678(GL470E4Self *context, GameUsoB274FieldDC *record);
extern int game_uso_func_04D700(void *object);
extern void game_uso_B274_set_prim_color(GL470E4Self *context, const float *rgba);
typedef struct {
    u8 pad_00[0x18];
    s16 field_18;
    u16 pad_1A;
    int (*field_1C)(void *);
} GameUsoB274Dispatch;
typedef struct {
    u8 pad_00[0x28];
    GameUsoB274Dispatch *field_28;
    u8 pad_2C[0x84];
    float field_B0;
    float field_B4;
    u8 pad_B8[8];
    s16 field_C0;
} GameUsoB274Child;
typedef struct {
    u8 pad_00[0xA0];
    float field_A0[3];
    u8 pad_AC[0x18];
    union { u32 word; GameUsoB274Child *child; } field_C4;
    float field_C8;
    float field_CC;
    int field_D0;
    int field_D4;
    GL470E4Context *field_D8;
    GameUsoB274FieldDC field_DC;
    u8 pad_EC[0x48];
    float field_134;
    u8 pad_138[0x14];
    int field_14C;
    u8 pad_150[0x14];
    float field_164[4];
} GameUsoB274Object;
void game_uso_func_0000B274(GameUsoB274Object *s0);
