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
extern char D_00000000;
struct GfxRing_413C { int *buf; int idx; };
typedef struct GuiTextureInfo {
    char unk00[8];
    void *field_08;
    char unk0C[20];
    short field_20;
    short field_22;
    int field_24;
} GuiTextureInfo;
typedef struct GuiTextureObject {
    char unk00[16];
    GuiTextureInfo *field_10;
} GuiTextureObject;
typedef struct GuiDLRing {
    u32 *buf;
    s32 idx;
} GuiDLRing;
typedef struct GuiDLCtx {
    u8 pad0[0xC];
    GuiDLRing *ring;
} GuiDLCtx;
typedef struct GuiGlyphCell148 {
    int x;
    int y;
    int ink_width;
    int advance;
    int left_bearing;
} GuiGlyphCell148;
typedef struct GuiGlyphGrid148 {
    int cell_count;
    char *pixels;
    int half_mean_width;
    int cell_width;
    int cell_height;
    int height_14;
    int atlas_width;
    int atlas_height;
    GuiGlyphCell148 *cells;
    void *draw_state;
} GuiGlyphGrid148;
GuiGlyphGrid148 *gui_func_00000148(GuiGlyphGrid148 *a0, char *a1, int a2, int a3, int rows, int cols);
void gui_uso_func_0000055C(char *a0);
struct GuiDrawState6B8 { char unk00[0xC]; struct GfxRing_413C *ring_0C; };
struct GuiColorObject6B8 { char unk00[0x24]; struct GuiDrawState6B8 *draw_state_24; };
struct GuiColorPacket6B8 { unsigned command; unsigned rgba; };
void gui_uso_func_000006B8(struct GuiColorObject6B8 *a0, int a1, float *a2, int a3);
typedef struct GuiDLPkt8C0 {
    u32 w0;
    u32 w1;
} GuiDLPkt8C0;
void gui_uso_func_000008C0(GuiGlyphGrid148 *a0);
extern int gl_func_00000000();
typedef struct GuiB58Glyph {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
} GuiB58Glyph;
typedef struct GuiB58Context {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    GuiB58Glyph *field_20;
    void *field_24;
} GuiB58Context;
extern u32 gui_b58_import_ba0(const u8 *);
extern u32 gui_b58_import_bf0(u32);
extern void gui_b58_import_c38(void *, s32, s32, s32, s32, s32, s32, s32, s32);
extern void gui_b58_import_c8c(void *, s32, s32, s32, s32, s32, s32);
extern u32 gui_b58_import_cc0(const u8 *);
void gui_func_00000B58(GuiB58Context *arg0, s32 arg1, s32 arg2, const u8 *arg3);
typedef struct GuiDrawStateD04 {
    char unk_00[0xC];
    struct GfxRing_413C *ring;
} GuiDrawStateD04;
extern unsigned int gui_D04_bound_D88(char *);
extern unsigned int gui_D04_bound_EC0(char *);
extern int gui_D04_glyph(unsigned char);
extern void gui_D04_load(void *, char *, int, int, int, int, int, int, int);
extern void gui_D04_draw(void *, int, int, int, int, int, int);
void gui_func_00000D04(GuiGlyphGrid148 *a0, int a1, int a2, char *a3);
extern int gl_func_00000000();
void gui_func_00000F04(void *arg0, int arg1, int arg2, float arg3, float arg4, unsigned char *arg5);
int gui_func_000013E8(char *a0, unsigned char *a1);
extern int gui_func_00000000();
int gui_func_000014B4(int a0, int a1, int a2);
extern int gl_func_00000000();
int gui_func_000014EC(int a0, int a1, int a2);
extern int gl_func_00000000();
int gui_func_00001514(int *a0, unsigned char *a1);
extern int gui_func_00000000();
int gui_func_000015BC(int a0, int a1, int a2);
int gui_func_000015F4(int a0, int a1, int a2);
void gui_func_0000161C(int *a0, int a1, int a2);
int gui_uso_func_00001674(int a0);
struct GuiBuilder_16A4 { int field_0[3]; struct GfxRing_413C *field_C; };
extern struct GuiBuilder_16A4 *D_gui16a4_cache;
void gui_uso_func_000016A4(int a0);
void gui_uso_func_00001794(int a0);
void gui_uso_func_000017DC(int unused, int blend_alpha, const float rgb[3], int alpha);
typedef struct GuiColorCommand1A5C {
    unsigned int w0;
    unsigned int w1;
} GuiColorCommand1A5C;
typedef struct GuiColorRing1A5C {
    GuiColorCommand1A5C *buffer;
    int count;
} GuiColorRing1A5C;
typedef struct GuiColorContext1A5C {
    int unk0[3];
    GuiColorRing1A5C *ring;
} GuiColorContext1A5C;
void gui_uso_func_00001A5C(void *a0, int a1, float *a2);
void gui_uso_func_00001CA8(void *a0, int a1, float *a2);
void gui_uso_func_00001EF4(void *a0, int a1, float *a2, float *a3);
void gui_uso_func_00002334(int unused, int environment_alpha,
                         const float *rgb, int primitive_alpha);
void gui_uso_func_000025B4();
extern int gl_func_00000000();
extern char D_00000000;
void gui_func_0000267C(char *a0, int a1);
void gui_func_000026CC(int *a0, int a1);
extern int gui_func_00000000();
extern char D_00000000;
void gui_func_000026D8(char *a0);
extern int gl_func_00000000();
void gui_func_0000271C(char *a0);
extern char gui_27A0_context;
typedef int (*Gui27A0Render)(void *, void *, int, int, int, int, int, int, int);
extern int gui_27A0_tile_width(int, GuiTextureInfo *);
extern int gui_27A0_draw(void *, int, int, int, int, int, int);
extern int gui_27A0_render1032(void *, void *, int, int, int, int, int, int, int);
extern int gui_27A0_render272(void *, void *, int, int, int, int, int, int, int);
extern int gui_27A0_render288(void *, void *, int, int, int, int, int, int, int);
extern int gui_27A0_render_default(void *, void *, int, int, int, int, int, int, int);
void gui_func_000027A0(GuiTextureObject *a0, int a1, int a2, int a3);
extern int gui_import_2CEC(int *, void *, int, int, int, int, int, int, int);
extern int gui_import_2D0C(int *, int, int, int, int, int, int);
extern int gui_import_2D4C(int *, void *, int, int, int, int, int, int, int);
extern int gui_import_2DA0(int *, int, int, int, int, int, int);
void gui_func_00002BB0(GuiTextureObject *object, int x, int y, int source_x, int count);
void gui_func_00002DE0(char *arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4);
void gui_func_0000329C(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void gui_func_00003714(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6);
extern int gl_func_00000000();
extern char D_00000000;
void gui_uso_func_00003B14(int a0, int a1, int a2, int a3, int arg5);
extern int D_3B80_f0, D_3B80_f1, D_3B80_f2;
void gui_func_00003B80(int *a0, int a1, int a2, int a3);
void gui_uso_func_00003F18(char *a0, int a1, int a2, int a3, int arg5,
                           int arg6, int arg7, int arg8, int arg9);
void gui_uso_func_0000413C(char *a0, int a1, int a2, int a3, int arg4,
                           int arg5, int arg6, int arg7, int arg8);
void gui_uso_func_00004354(char *a0, int a1, int a2, int a3, int arg5,
                           int arg6, int arg7, int arg8, int arg9);
extern int gl_func_00000000();
void gui_func_00004568(int *a0, int a1, int a2, int a3, int a4, int a5, int a6);
