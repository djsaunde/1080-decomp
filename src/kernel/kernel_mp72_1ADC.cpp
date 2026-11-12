/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) donor for func_80001ADC (134 words, EXACT, agent-h run 363),
 * compiled by tools/ncc721.sh and spliced into kernel_000.c.o on both build paths over the placeholder.
 * USO relocation applier: see the decode comment at func_80001ADC in kernel_000.c.
 *
 * The target dispatches the 4-label `switch (type)` (1..4) through a native 4-entry jump table, which only
 * EDG 2.29's switch rule produces (cfe needs a phantom `case 5:` and gets sltiu 5). The donor's .rodata
 * reloc is renamed func_80001ADC_rodata by the splice and pinned to the ROM table jtbl_8000A63C (table at
 * donor .rodata +0). Levers (kits CDXLVII / CDXLVIII):
 *   - the four head statements sit INSIDE the flags test's then-arm (`if (flags & 8) { ... } else { continue; }`):
 *     written after an empty then-arm they land in a join block that uopt folds into the test's fall-through
 *     with every statement LOC dropped, so the whole head carries the test's line and as1 breaks the
 *     sltiu / val-load tie by ugen order (sltiu first); inside the then-arm each statement keeps its line and
 *     the lower-line val load wins the tie, as in the target (`lw t8; addiu; lw a3; addu; lw t9; sltiu`);
 *   - `i = i;` on its own line after the R_26 store: the uopt-inserted loop tail of that arm (end-pointer
 *     recompute, rel[i].info reload) carries that later line, so the `sw` is scheduled before the end-pointer
 *     `sll` and the info reload fills the branch delay slot;
 *   - `(u16)(lo & 0xFFFF)` in the LO16 tail store: same low half; the two folded masks burn two ugen ring
 *     temps (as1 deletes both), so the tail and every later temp take the target's registers;
 *   - info / sym are unreferenced: their homes give the target's 0x50 frame with hiptr / hival at 0x34 / 0x28. */
typedef int s32;
typedef short s16;
typedef unsigned int u32;
typedef unsigned short u16;
typedef void (*FuncPtr2)(...);
extern "C" FuncPtr2 D_80012C44;
extern "C" char D_8000A36C[];
typedef struct { s32 info; s32 offset; s32 orig; } UsoRel_1ADC;
typedef struct { s16 flags; s16 pad; s32 value; s32 pad2; } UsoRelSym_1ADC;

extern "C" s32 func_80001ADC(char *obj, char *base, UsoRel_1ADC *rel, s32 n) {
    s32 info; UsoRelSym_1ADC *sym;
    UsoRelSym_1ADC *symtab;
    s32 type, val, lo;
    s16 *hiptr;
    s32 v;
    s32 i;
    s32 hival;
    u32 *p;

    symtab = *(UsoRelSym_1ADC **)(obj + 0x3C);
    if (0) { info = 0; sym = 0; }
    if (base == 0 || rel == 0) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (rel[i].info & 8) {
            continue;
        }
        if ((&symtab[rel[i].info >> 4])->flags & 8) {
            type = rel[i].info & 7;
            p = (u32 *)(base + rel[i].offset);
            val = (&symtab[rel[i].info >> 4])->value;
            rel[i].orig = *p;
        } else { continue; }
        switch (type) {
        case 1:
            *p = ((((u32)val >> 2) & 0x3FFFFFF) + (*p & 0x3FFFFFF)) | (*p & 0xFC000000);
            i = i;
            break;
        case 3:
            hiptr = (s16 *)p + 1;
            hival = ((s16 *)p)[1];
            break;
        case 2:
            lo = ((s16 *)p)[1];
            /* faithful uninitialised read (MATCHING_WORKFLOW extension 2026-11-12): hiptr / hival are unset on a
             * LO16-before-HI16 path; the target reads both homes before any write (lw t4,0x28(sp) at 0x80001B30,
             * lw t2,0x34(sp) at 0x80001B34; stores only at the loop exit 0x80001CCC / 0x80001CD0) */
            if (hiptr) {
                v = (hival << 16) + (s16)lo + val;
                *hiptr = (u32)(v - (s16)v) >> 16;
                hiptr = 0;
            }
            ((s16 *)p)[1] = (hival << 16) + (u16)(lo & 0xFFFF) + val;
            break;
        case 4:
            *p = *p + val;
            break;
        default:
            D_80012C44(D_8000A36C, (void *)type);
            return -2;
        }
        rel[i].info = rel[i].info | 8;
    }
    return 0;
}
