/* IDO 5.3 -O1 donor for func_8000858C (0x228 / 138 words) = libultra rmon's
 * __rmonGetBranchTarget(method, thread, addr): decode the MIPS instruction at
 * addr (method 1 reads it through __rmonReadWordAt = func_80006A98) and return
 * the branch / jump destination, or -1. func_80009B60 = __rmonGetRegisterContents
 * (jr / jalr targets).
 *
 * The old "dangling absolute jump tables / audio data" verdict was the +0x400
 * kernel numbering offset: the 24-entry opcode table and the 20-entry REGIMM
 * table sit in the ROM at our 0xA690 / 0xA6F0 (runtime 0x8000AA90 / 0x8000AAF0,
 * entries = runtime case addresses). The donor's two .rodata references (+0 and
 * +0x60) are renamed func_8000858C_rodata by the splice and pinned to 0x8000AA90.
 * JALR test is `(inst & 0x7FF) == 9`; IDO 5.3 (not 7.1) gives the andi / or
 * temp copies. Kept out of C_FILES (Makefile). */
extern unsigned int func_80006A98(unsigned int *);
extern int func_80009B60(int, int, int);
int func_8000858C(int method, int thread, char *addr) {
    int inst;

    if (method == 1) {
        inst = func_80006A98((unsigned int *)addr);
    } else {
        inst = *(int *)addr;
    }

    switch ((inst >> 26) & 0x3F) {
    case 0:
        if (((inst >> 5) & 0x7FFF) == 0 && (inst & 0x3F) == 8) {
            return func_80009B60(method, thread, (inst >> 21) & 0x1F);
        }
        if (((inst >> 16) & 0x1F) == 0 && (inst & 0x7FF) == 9) {
            return func_80009B60(method, thread, (inst >> 21) & 0x1F);
        }
        break;
    case 1:
        switch ((inst >> 16) & 0x1F) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 16:
        case 17:
        case 18:
        case 19:
            return ((inst << 16) >> 14) + (int)addr + 4;
        }
        break;
    case 2:
    case 3:
        return (((unsigned int)inst << 6) >> 4) + (((int)((unsigned int)addr + 4) >> 28) << 28);
    case 4:
    case 5:
    case 20:
    case 21:
        return ((inst << 16) >> 14) + (int)addr + 4;
    case 6:
    case 7:
    case 22:
    case 23:
        if (((inst >> 16) & 0x1F) == 0) {
            return ((inst << 16) >> 14) + (int)addr + 4;
        }
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        if (((inst >> 21) & 0x1F) == 8) {
            switch ((inst >> 16) & 0x1F) {
            case 0:
            case 1:
            case 2:
            case 3:
                return ((inst << 16) >> 14) + (int)addr + 4;
            }
        }
        break;
    }
    return -1;
}
