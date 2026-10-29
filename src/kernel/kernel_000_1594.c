/* IDO 7.1 cfe -O2 donor for func_80001594 (0x220 / 136 words): USO loader
 * init (see the placeholder in kernel_000.c). Kept out of C_FILES (Makefile).
 *
 * Installs the USO loader callback table at D_80012BC0 (cb, held in a3 via the
 * int-cast base), clears the 135 0x9C-byte D_800130A0 slots' name byte (+0x72)
 * and the D_80012D60 words, then copies up to 16 0x14-byte segment descriptors
 * into D_80012C70 (0xC stride), failing with cb->unk8 = -0x17 when a segment
 * end passes arg1 + 0x80000000.
 *
 * agent-c run 229: the slot-clear loop is an INDEX loop on i (`for (i = 0;
 * i < 135; i++) { ... }` on ONE line). Its i web starts before the
 * func_80000480 + 0x30 address CSE dies, so that address colours a2 (a1 is
 * i's) like the target; the pointer walk (p / q) left it in a1. The one-line
 * body keeps the target's sltu / sw / sb / bnez / addiu order (as1 line ties).
 * The rest is agent-g's / agent-c's run 163-228 NM body. */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; u32 unk10; } UsoSegDesc;
typedef struct { s32 unk0; s32 unk4; s32 unk8; } UsoSegEnt;
extern s32 D_80012BC0;
extern s32 D_80012D60[];
extern char D_800130A0;
extern char D_80012C70[];
extern void *D_8001309C;
extern void func_80001584(void *, u32);
extern void func_80000874(void *);
extern void func_800018A8(void);
extern char func_80001994[], func_80000DEC[], D_80000CF0[], func_80001088[];
extern char D_80001258[], D_8000128C[], D_80000BD4[], D_800016BC[];
extern char D_80001814[], D_80001894[], D_80001870[], D_80001748[];
/* the target's .text-relative (0x80000C5C) and .L800007A8 local-label references, pinned in undefined_syms_auto.txt */
extern char func_80001594_L80000C5C[], func_80001594_L800007A8[];
extern char D_800016AC[], D_80001860[], D_8000189C[];
extern char func_800014A8[], func_800010E8[], func_80000480[];

void func_80001594(UsoSegDesc *arg0, u32 arg1) {
    s32 cb;
    s32 i;
    UsoSegEnt *e;
    u32 lim;

    func_80001584(arg0, arg1);
    D_8001309C = &D_800130A0;
    func_80000874(arg0);
    cb = (s32)&D_80012BC0;
    *(s32 *)(cb + 0x3C) = (s32)func_80001994;
    *(s32 *)(cb + 0x40) = (s32)func_80000DEC;
    *(s32 *)(cb + 0x8) = 0;
    *(s32 *)(cb + 0x44) = (s32)D_80000CF0;
    *(s32 *)(cb + 0x64) = (s32)func_80001088;
    *(s32 *)(cb + 0x68) = (s32)(func_800010E8 + 0x44);
    *(s32 *)(cb + 0x84) = (s32)(func_800014A8 + 0x24);
    *(s32 *)(cb + 0x88) = (s32)(func_800014A8 + 0x24);
    *(s32 *)(cb + 0x34) = (s32)(func_80000480 + 0x30);
    *(s32 *)(cb + 0x6C) = (s32)D_80001258;
    *(s32 *)(cb + 0x30) = (s32)(func_80000480 + 0x30);
    *(s32 *)(cb + 0x70) = (s32)D_8000128C;
    *(s32 *)(cb + 0x74) = (s32)func_80001584;
    *(s32 *)(cb + 0x78) = (s32)(func_800014A8 + 0x40);
    *(s32 *)(cb + 0x7C) = (s32)D_80000BD4;
    *(s32 *)(cb + 0x38) = 0;
    *(s32 *)(cb + 0x4C) = (s32)D_800016BC;
    *(s32 *)(cb + 0x50) = (s32)D_80001814;
    *(s32 *)(cb + 0x58) = (s32)D_80001894;
    *(s32 *)(cb + 0x54) = (s32)D_80001870;
    *(s32 *)(cb + 0x48) = (s32)D_80001748;
    *(s32 *)(cb + 0xA0) = (s32)func_80001594_L80000C5C;
    *(s32 *)(cb + 0x8C) = (s32)(func_800014A8 + 0x24);
    *(s32 *)(cb + 0x90) = (s32)(func_800014A8 + 0x18);
    *(s32 *)(cb + 0x80) = (s32)func_80001594_L800007A8;
    *(s32 *)(cb + 0x5C) = (s32)D_800016AC;
    *(s32 *)(cb + 0x60) = (s32)D_80001860;
    *(s32 *)(cb + 0x94) = (s32)D_8000189C;
    for (i = 0; i < 135; i++) { D_80012D60[i] = 0; (&D_800130A0)[i * 0x9C + 0x72] = 0; }
    for (i = 0; i < 16; i++) {
        if (arg0[i].unk0 == 0) break;
        e = (UsoSegEnt *)D_80012C70 + i;
        e->unk0 = arg0[i].unkC; if (1) {}
        if (e->unk0 != 0) {
            e->unk4 = arg0[i].unk10;
            e->unk8 = e->unk0;
            if (arg1 + 0x80000000 < (u32)e->unk4) {
                lim = e->unk4;
                *(s32 *)(cb + 0x8) = -0x17;
                *(s32 *)(cb + 0x10) = lim;
                break;
            }
        }
    }
    func_800018A8();
}
