#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/node.h"
#include "game/input.h"

/* bootup_uso 0x10C8C..0x116C7 remainder of the former tail3a mid-section,
 * moved out of bootup_uso_tail3a.c on 2026-07-10 when gui_addRowSub_00010B6C
 * (0x10B6C, -O0) was carved into bootup_uso_o0_10B6C.c. Same OPT_FLAGS as
 * tail3a was -O2 -g3 but both residents are -O0 (flipped 2026-07-10):
 * GameFlow_create is MATCHED (real C in build path); GameFlow_startMode is an
 * honest 94.5% NM wrap (INCLUDE_ASM in build path). TRUNCATE_TEXT 0xA3C. */

extern int func_00000000();
extern int D_00000000;
extern char *gui_getCurrentRow_00010324();

/* GameFlow_create: -O0 object constructor (216 insns). Inits arg0's struct
 * fields, builds a temp Vec4-ish stack record (sp6C) passed by value to a
 * registrar call, allocates two 0x80 sub-objects (->0xC0/->0xC4), then a
 * sub-record (->0x154) whose byte/halfword fields drive 4 callbacks + a
 * global flag. sp6C.w loads from the shared FP literal pool (reloc to
 * .text func_00000C10 @ module-offset 0xC10, +0xC; same recipe as
 * bootup_uso_o0_100F0.c). MATCHED 2026-07-10 (216/216). */
extern int func_000000F0;
extern struct { float f0, f1, f2, f3; } func_00000C10; /* FP literal pool */
extern char sStr_GameFlow_C5D0;
extern char sStr_Flow_C5DC;
extern char sStr_rom_gui_uso_C5E4;
extern char sStr_rom_flowimgs_bin_C5F0;

typedef struct {
    float x, y, z, w;
} Vec4_10C8C;

void GameFlow_create(int *arg0, int arg1) {
    int sp84;
    register char *p;   /* s0, home 0x80 */
    register char *q;   /* s1, home 0x7C */
    Vec4_10C8C rec;     /* sp6C */
    int sp68;
    register float x;   /* f20, home 0x64 */
    register float y;   /* f22, home 0x60 */
    register float z;   /* f24, home 0x5C */
    register float w;   /* f26, home 0x58 */

    p = (char *)arg0;
    q = (char *)&sStr_GameFlow_C5D0;
    NODE(p)->name = (int)q;
    GAMEFLOW(arg0)->app = (void *)arg1;
    sp68 = func_00000000(0);
    p = (char *)&rec;
    if (p != 0 || (p = (char *)func_00000000(0x10)) != 0) {
        q = p;
        x = 0;
        y = 0;
        z = 0;
        w = func_00000C10.f3;
        *(float *)q = x;
        *(float *)(q + 4) = y;
        *(float *)(q + 8) = z;
        *(float *)(q + 0xC) = w;
    }
    func_000000F0 = func_00000000(0, &sStr_Flow_C5DC, 0x10, 0x12C, 0x10, 0x14, rec);
    func_00000000(sp68);
    GAMEFLOW(arg0)->usoList = 0;
    GAMEFLOW(arg0)->procUsoPath = 0;
    GAMEFLOW(arg0)->flowMode = 9;
    GAMEFLOW(arg0)->demoIndex = -1;
    GAMEFLOW(arg0)->unk70 = 0;
    GAMEFLOW(arg0)->seq = 0;
    sp84 = func_00000000(0);
    func_00000000(&gSysLib, &sStr_rom_gui_uso_C5E4);
    p = 0;
    if (p != 0 || (p = (char *)func_00000000(0x80)) != 0) {
        func_00000000(p, 1);
    }
    GAMEFLOW(arg0)->ctrls[0] = p;
    p = 0;
    if (p != 0 || (p = (char *)func_00000000(0x80)) != 0) {
        func_00000000(p, 2);
    }
    GAMEFLOW(arg0)->ctrls[1] = p;
    func_00000000(arg0);
    GAMEFLOW(arg0)->unk84 = 0;
    GAMEFLOW(arg0)->unk88 = 1;
    GAMEFLOW(arg0)->locked = 0;
    GAMEFLOW(arg0)->game = (char *)func_00000000(0);
    GAMEFLOW(arg0)->fader = (void *)func_00000000(0, 0);
    GAMEFLOW(arg0)->backup = (void *)func_00000000(0);
    GAMEFLOW(arg0)->unlocks = (char *)func_00000000(GAMEFLOW(arg0)->backup);
    func_00000000(*(unsigned char *)(GAMEFLOW(arg0)->unlocks + 0x12));
    func_00000000(*(unsigned char *)(GAMEFLOW(arg0)->unlocks + 0x13));
    func_00000000(*(unsigned char *)(GAMEFLOW(arg0)->unlocks + 0x14));
    func_00000000(*(unsigned short *)(GAMEFLOW(arg0)->unlocks + 4) & 3);
    D_00000000 = (*(unsigned short *)(GAMEFLOW(arg0)->unlocks + 4) & 4) == 4;
    GAMEFLOW(arg0)->recordCap = *(int *)GAMEFLOW(arg0)->backup;
    GAMEFLOW(arg0)->recordBuf = func_00000000(GAMEFLOW(arg0)->recordCap << 2);
    GAMEFLOW(arg0)->flowImgs = (void *)func_00000000(0, &sStr_rom_flowimgs_bin_C5F0);
    func_00000000(sp84);
    func_00000000(arg0);
}

/* GameFlow_startMode: mode-switch dispatcher (439 insns), -O0. Tears down /
 * re-registers three sub-objects (0x134 / self / 0x150) against the module
 * event list, bumps the global sequence counter, then switch(arg1) builds
 * the mode object (->0x30): per case, alloc(8|0xC) + alloc(8) vtable pair.
 * Case source order 0,1,3,2,4,5,6,7,8 recovered from the RoData jumptable
 * at module 0xC20 (scripts/extract-uso-jumptable.py; targets bias +8 =
 * Text leading marker word). The C switch emits a local .rodata jumptable:
 * tenshoe.ld pins this unit's .rodata at VMA 0xC20 via a NOLOAD section so
 * the lui/lw dispatch fields bake %hi/%lo(0xC20) while the table bytes
 * (present in the baked USO RoData asset) stay out of the ROM image.
 *
 * EXACT 2026-09-05 (439/439, reloc-resolved; was an "honest NM" at 415/439
 * with a single root diff: the `arg1 != arg0->0x38` guard, where the target
 * evaluates the SCALAR arg1 first -- lw t0,0x44(sp); lw t1,0x40; lw
 * t2,0x38(t1); beq t0,t2 -- while IDO -O0 cfe always emits the deref side
 * first for `scalar != deref` / `deref != scalar` alike; ~25 plain
 * spellings could not flip it). LEVER: a comma expression on the scalar
 * side, `deref != (0, arg1)`, makes cfe evaluate arg1 first into a plain
 * t-reg with no frame/insn change (the mirrored `(0, arg1) != deref`
 * promotes the comma value to $s0 instead -- wrong). The 20 downstream
 * t0/t1 FIFO knock-ons collapsed with it. See docs/IDO_CODEGEN.md
 * "-O0 scalar-vs-deref equality eval order" (comma-lever addendum). */
typedef int (*FN10FEC)();

extern char D_0000C550;
extern char sFmt_num_C604;
extern char sStr_FLOW_free_blocks_1_C60C;
extern int D_0000C4EC[];

void GameFlow_startMode(char *arg0, int arg1, int arg2) {
    int sp3C;
    register char *p;   /* s0 */
    register char *q;   /* s1 */
    register char *r;   /* s2 */
    register char *s;   /* s3 */
    int sp28;

    q = arg0;
    p = q + 0x10;
    *(int *)p = 0;

    p = GAMEFLOW(arg0)->game;
    q = (char *)&D_00000000;
    r = q + 0x10;
    func_00000000(r, p);
    if (NODE(p)->parent != 0) {
        NODE(p)->unk04 = 1;
    }
    r = q;
    NODE(p)->parent = (int)r;

    p = arg0;
    q = (char *)&D_00000000;
    r = q + 0x10;
    func_00000000(r, p);
    if (NODE(p)->parent != 0) {
        NODE(p)->unk04 = 1;
    }
    s = q;
    NODE(p)->parent = (int)s;

    p = (char *)GAMEFLOW(arg0)->flowImgs;
    q = arg0;
    r = q + 0x10;
    func_00000000(r, p);
    if (NODE(p)->parent != 0) {
        NODE(p)->unk04 = 1;
    }
    s = q;
    NODE(p)->parent = (int)s;

    func_00000000(arg0);
    GAMEFLOW(arg0)->unk74 = 0;
    GAMEFLOW(arg0)->unk78 = 0;

    p = (char *)D_00000000;
    D_00000000 = (int)p + 1;
    func_00000000(func_000000F0, &sFmt_num_C604, p, (&D_00000000)[arg1]);
    sp3C = func_00000000(func_000000F0);
    if (sp3C != 1) {
        func_00000000(&sStr_FLOW_free_blocks_1_C60C);
    }

    if (GAMEFLOW(arg0)->flowMode != (0, arg1)) {
        GAMEFLOW(arg0)->nextFlowMode = arg1;
        func_00000000(arg0);
        func_00000000(arg0, D_0000C4EC[(&D_00000000)[arg1]]);
        sp28 = func_00000000(3);
        func_00000000(&D_00000000, 1);
        switch (arg1) {
        case 0:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(8)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 1:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(8)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 3:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(0xC)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 2:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(0xC)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 4:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(8)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 5:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(0xC)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 6:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(8)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 7:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(8)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        case 8:
            q = 0;
            if (q != 0 || (q = (char *)func_00000000(8)) != 0) {
                r = q;
                if (r != 0 || (r = (char *)func_00000000(8)) != 0) {
                    *(int *)(r + 4) = (int)&D_0000C550;
                }
                *(int *)(q + 4) = (int)&D_00000000;
            }
            p = q;
            GAMEFLOW(arg0)->flowProc = p;
            break;
        }
        func_00000000(&D_00000000, 0);
        func_00000000(sp28);
    }

    (*(FN10FEC)*(int *)(p + 0xC))(
        (int)GAMEFLOW(arg0)->flowProc +
            *(short *)((p = *(char **)(GAMEFLOW(arg0)->flowProc + 4)) + 8),
        arg2);
    GAMEFLOW(arg0)->flowMode = arg1;
    func_00000000();
    p = (char *)D_00000000;
    func_00000000(*(int *)(GAMEFLOW(arg0)->game + 0x114), p);
}
