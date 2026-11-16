/* IDO 5.3 -O1 donor for __rmonWriteMem — rmon memory-write handler (147
 * insns, frame 0x48). PROVEN EXACT 147/147 (2026-07-10 standalone probe vs
 * asm/nonmatchings/kernel/__rmonWriteMem.s; only R_MIPS_26 jal placeholders
 * differ, resolved at link). 5th 5.3-vs-7.1 temp-rematerialization crack
 * (7564/8264/87B4/70A0 family). The documented "coloring + frame-layout
 * cap" (deferred `move s0,t6` prologue + slot-offset cascade) decomposes
 * into FOUR levers, all C-reachable at 5.3 -O1:
 *   (1) STRUCTURE FIX: the byte-merge is an if/else-if/else CHAIN (the ==1
 *       and ==2 merges `b` past the low-byte merge), not three independent
 *       ifs — the old NM body's shape was wrong.
 *   (2) PROLOGUE: first byte-read through the RAW PARAM (`*(u8*)((char*)
 *       sp48 + 9)`) with `arg0 = sp48` as a separate statement — the reload
 *       CSEs to the param temp (lbu t7,9(t6)) and the s0 commit stays a
 *       distinct `or s0,t6,zero` (build otherwise folds to `lw s0,home`).
 *   (3) FRAME MAP: IDO 5.3 -O1 gives EVERY `register` var a dead 4-byte
 *       home slot allocated in strict decl order (first decl = highest),
 *       exactly like homed locals. Target layout = `register arg0`
 *       declared FIRST (dead home 0x44), then struct (0x34) / sp30 / sp2C /
 *       sp28 / sp24 (0x24), then `register var_s1` LAST (dead home 0x20)
 *       -> frame 0x48. With both register decls adjacent the two dead homes
 *       fuse at one end and every slot shifts +4/+8.
 *   (4) ORDER TIES: word-loop init `sp28 = len>>2;` BEFORE `sp24 = ptr;`
 *       (lw-first ugen numbering t4/t5/t6); SP-range sum spelled
 *       `FW(0x14) + FW(0x10)` (len-first load pair); reply tail store
 *       order tag -> zero -> id (8264 lever).
 * Spliced into kernel_018_b.c.o via REPLACE_FUNC_BODY (real 5.3 -O1
 * output; donor relocs imported by the splice). NOTE: target contains a
 * mid-function jal to 0x80006AEC (the merge-store join inside THIS
 * function) — from the compiler's view it is a plain extern call
 * (R_MIPS_26 func_80006AEC), resolved by undefined_syms_auto.txt. */
#include "common.h"
#include "libultra/os_thread.h"
#include "libultra/rmon.h"
extern s32 func_80004B30(u32);
extern s32 func_80006A98(s32);
extern void func_80006A50(s32, s32);
extern void func_80006AEC(s32, void *, s32);
extern void func_800073F8(void *, s32, s32);

s32 __rmonWriteMem(KKWriteRequest *sp48) {
    register KKWriteRequest *arg0;
    KKObjectEvent sp34;
    s32 sp30;
    s32 sp2C;
    u32 sp28;
    s32 *sp24;
    register s32 var_s1;

    arg0 = sp48;
    if ((sp48->writeHeader.header.method == RMON_CPU) && (func_80004B30(arg0->writeHeader.addr) == -1)) {
        return TV_ERROR_INVALID_ADDRESS;
    }
    if (arg0->writeHeader.nbytes >= 0x401U) {
        return TV_ERROR_INVALID_CAPABILITY;
    }
    /* (*arg0).f in the range test: the -> spelling reorders its addu / sltu temps */
    if (((*arg0).writeHeader.addr < 0x04000000U) || (((*arg0).writeHeader.nbytes + (*arg0).writeHeader.addr) >= 0x05000000U)) {
        var_s1 = 0;
    } else {
        var_s1 = 1;
    }
    if (var_s1 != 0) {
        sp30 = arg0->writeHeader.addr & 3;
        if (sp30 != 0) {
            if (arg0->writeHeader.nbytes != 1) {
                return TV_ERROR_INVALID_ADDRESS;
            }
            sp2C = func_80006A98(arg0->writeHeader.addr & ~3);
            if (sp30 == 1) {
                sp2C = (arg0->buffer[0] << 0x10) | (sp2C & 0xFF00FFFF);
            } else if (sp30 == 2) {
                sp2C = (arg0->buffer[0] << 8) | (sp2C & 0xFFFF00FF);
            } else {
                sp2C = arg0->buffer[0] | (sp2C & ~0xFF);
            }
            func_80006A50(arg0->writeHeader.addr & ~3, sp2C);
            goto block_26;
        }
        sp28 = arg0->writeHeader.nbytes >> 2;
        sp24 = (s32 *)arg0->buffer;
        if (arg0->writeHeader.nbytes & 3) {
            return TV_ERROR_INVALID_ADDRESS;
        }
        var_s1 = sp28;
        sp28 -= 1;
        if (var_s1 != 0) {
            do {
                func_80006A50(arg0->writeHeader.addr, *sp24++);
                arg0->writeHeader.addr = arg0->writeHeader.addr + 4;
                var_s1 = sp28;
                sp28 -= 1;
            } while (var_s1 != 0);
        }
        goto block_26;
    }
    func_80006AEC(arg0->writeHeader.addr, arg0->buffer, arg0->writeHeader.nbytes);
block_26:
    sp34.header.code = arg0->writeHeader.header.code;
    sp34.header.error = TV_ERROR_NO_ERROR;
    sp34.object = arg0->writeHeader.object;
    func_800073F8(&sp34, sizeof(sp34), KK_TYPE_REPLY);
    return 0;
}
