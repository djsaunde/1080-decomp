#include "common.h"

/* Forward declarations */
extern s32 __osPiAccessQueue;
extern s32 D_8000A480;
extern s32 D_80013004;
extern void func_80005DC0(s32*, s32, s32);
extern void func_80005C00(void);
extern void func_80004FE0(s32*, s32*, s32);
extern void func_800053D0(s32*, s32*, s32);
extern s32 D_8000A3E0;
extern s32 func_800066B0(void);
extern void func_800066D0(s32);
extern void func_80003D0C(s32*);
extern void* D_8000A420;
extern s32 D_8000A418;
extern s32 siacs_bss_0000;
extern s32 func_800009D8(void*, s32, s32, void*);
extern void func_80003FF0(s32, void*);
extern void* func_80003E54(void*);
extern void func_8000A110(void*);
extern void func_8000857C(void);
extern void func_8000785C(s32);
extern void func_80008ED0(void);
extern void func_80007760(void);
typedef void (*FuncPtr2)(void*, void*);
extern FuncPtr2 D_80012C44;
extern s32 D_8000A32C;
extern s32 D_8000A340;
extern s32 D_80012BC0;
extern s32 D_80012BE4;
extern s32 D_80012C64;
extern s32 D_80012C68;
extern s32 D_80012D60[];
extern s32 D_80013000;
extern s32 D_8000A2E0;
extern s32 D_8000A41C;
extern s32 func_80002890(s32);
extern void func_8000A0E0(void);
extern void func_80005350(s32, s32);
extern void func_80005400(s32, s32);
extern s32 func_80008430(s32);
extern void func_80009148(s32);
extern void func_80009030(s32, s32);
extern void func_80008498(void);
extern s32 func_80006A98(s32);
extern void func_800091F0(s32);
extern s32 func_80009EA0(void);
typedef struct { s32 pad[3]; s32 position; } FileState;

typedef struct {
    void* mtqueue;
    void* fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    s32** msg;
} OSMesgQueue;

typedef struct Thread {
    s32 field0;
    s32 pri;
    s32 queue;
    s32 pad0C;
    u16 state;
} Thread;

typedef struct { s32 queue; s32 msg; } OSEventState;
extern OSEventState __osEventStateTab[];

typedef struct {
    u8 pad_00[0x98];
    u8 in_use;
    u8 pad_99[3];
} UsoSlot;

typedef struct {
    UsoSlot slots[4];
} UsoSlotGroup;

extern u8 D_80013138;
extern UsoSlot D_800130A0;
extern u8 D_800131D4;
extern UsoSlot D_8001313C;
extern u8 D_80013270;
extern UsoSlot D_800131D8;
extern UsoSlotGroup D_80013274;
extern UsoSlotGroup D_800182E4;


/* func_80000000 (entry stub) + func_80000060 (boot main) live in kernel_000_entry.c */

extern u32 D_8000A2D8;
extern u32 D_8000A2DC;
extern s32 D_80012D5C;

/* Bump allocator with alignment + bookkeeping. NATURAL CEILING: 91.15% NM.
 * Was previously documented (2026-05-14) as "Promoted from 91.15% NM ->
 * byte-exact via 17-entry INSN_PATCH + 1-word SUFFIX_BYTES_FORCE (4 bytes)"
 * — INSN_PATCH + grow-by-1-insn SUFFIX_BYTES_FORCE REMOVED 2026-05-23 as
 * match-faking (per feedback_no_instruction_forcing_matches_policy).
 * Cap class: "preemptive set + nop delay" structural shape — target emits
 * `or v0, v1, 0` before the bounds-check branch (separating the move from
 * the branch delay), making the function 1 insn larger than IDO's natural
 * emit. 11+ negative C-shape attempts confirmed the cap is unreachable
 * from naturally-emitted C; default build is INCLUDE_ASM.
 *   D_8000A2D8 = current heap top (bumps on each call)
 *   D_8000A2DC = heap end limit (returns NULL when exceeded)
 *   D_80012D5C = running total of bytes allocated
 *
 * Current match: 18/25 insns (~72%), 7 mnemonic diffs (was 15/26 ~58%).
 * 2026-06-23 NM-body rewrite (agent-i) cut the residual from 11 diffs to 7:
 *   - INLINE the alignment mask `(alignment - 1)` (drop the named `v` mask
 *     var) so IDO frees $a1/$a2 for the two global-pointer bases and hoists
 *     `lui %hi(D_80012D5C)` into the early beqz delay slot — the entire
 *     address-materialization region (a1/a2 coloring) now MATCHES the target.
 *   - explicit `r = v; if (oom) r = 0; return r;` reserves $v0 for the result
 *     so the load/store/bookkeeping region matches too.
 * Residual 7 diffs are the genuine documented structural cap:
 *   (1) `and t6,a0,v0` vs `and t6,v0,a0` — commutative & operand order; IDO
 *       won't swap it from any C source orientation (verified flip-and = 7).
 *   (2-7) the target preemptively emits `move v0,v1` BEFORE the sltu/bnez and
 *       uses TWO `jr ra` (success falls to a bare return; OOM zeroes v0 in the
 *       second jr's delay slot), making the target 25 insns vs IDO's natural
 *       24. No tested C shape forces IDO to emit the +1-insn double-jr-ra form
 *       (goto-out, do/while-break, nested if/else, explicit two-return all
 *       regress; permuter-immune per cap_analysis_2026-05-28, 153k iters).
 *
 * Tried: condition-flip with goto-out (9/26, regressed), explicit
 * `result = orig; if (oom) result = 0; return result;` early-set
 * pattern (5/26, regressed harder — IDO still lets new_top win $v0).
 * Both alternatives shorten the function further by combining the
 * preemptive set into the branch delay slot. No tested C shape forces
 * IDO to emit the +1-insn longer "preemptive set + nop delay" pattern.
 *
 * 2026-05-08 — additional negative attempts:
 *   (a) `if (new_top < D_8000A2DC) return result; return 0;` (flipped
 *       condition with early-return success): 91.15% → 88.65% (-2.5pp).
 *   (b) `ret = result; if (new_top >= D_8000A2DC) ret = 0; return ret;`
 *       (explicit ret local to force preemptive set): 91.15% → 85.38%
 *       (-5.8pp). IDO collapses `ret` into `result` and emits the same
 *       shortened branch-delay pattern.
 * Both retests confirm the shape cap. The 91.15% wrap is the local maximum
 * for naturally-emitted IDO -O2 with current REG_ALLOC_ORDER constraints.
 *
 * 2026-05-08 retest #3 — `register u32 mask;` IDO register hint to coerce
 *   $v0 allocation: emit unchanged ($v1 still selected for mask). IDO's
 *   `register` honors strong reg-class hints but doesn't override the
 *   priority-based $v0/$v1 split when $v0 is reserved for a longer-live
 *   pseudo. Confirms cap is structural, not register-hint-fixable.
 *
 * 2026-05-27 retest #4 — split-jr-ra-via-goto-out lever (the same lever
 *   that landed gl_func_0001FD98 / improved gl_func_00036E74 + 0003D16C):
 *   `if (new_top < D_8000A2DC) goto succ; return 0; succ: return result;`.
 *   Standalone-cc shows mine still places `or v0, v1, $0` in the bnez
 *   delay slot rather than target's BEFORE-sltu position. The goto-out
 *   lever splits the jr-ra into two but doesn't move the preemptive set
 *   out of the delay slot — same root structural-cap as variants (a)/(b).
 *   The cap is the position of the v0=v1 copy, not the number of jr-ra blocks.
 *
 * 2026-06-01 retest #5 — explicit `ret = v; __asm__(""); if (...) return`
 *   barrier is invalid for this IDO/asm-processor path: empty `__asm__` emits
 *   a real jal to `__asm__`, adds a frame/spills, and drops this function to
 *   29.04%. Do not use the asm-barrier lever here.
 *
 * 2026-06-01 retest #6 — GCC-style explicit register locals for the target
 *   `$t9/$t1/$t3` allocation, e.g. `register u32 new_top asm("$25");`, are not
 *   accepted by IDO cfe (`Syntax Error` at `asm`). This lever is unavailable. */
#ifdef NON_MATCHING
u32 func_800000B0(u32 size, u32 alignment) {
    u32 v;   /* old heap top -> $v1, the returned ptr on success */
    u32 r;   /* result: r=v then forced to 0 on OOM, lives in return reg */

    /* inline mask (alignment-1) instead of a named var: lets IDO put the mask
     * in $v0/$v1 and free $a1/$a2 for the two global-pointer bases, matching
     * the target's a1/a2 pointer coloring and the lui-%hi-in-beqz-delay hoist. */
    if (size & (alignment - 1)) {
        size = (size + alignment) & ~(alignment - 1);
    }
    v = D_8000A2D8;
    D_8000A2D8 = v + size;
    D_80012D5C += size;
    r = v;
    if (D_8000A2D8 >= D_8000A2DC) {
        r = 0U;
    }
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/kernel", func_800000B0);
#endif

/* Boot init */
void func_80000118(s32 a0, s32 a1) {
    s32 saved = func_80002890(1);
    func_8000A0E0();
    func_80005350(a0, a1);
    func_80005400(a0, a1);
    func_80002890(saved);
}

/* func_80000168: string-difference predicate (0 if equal, 1 if different).
 * EXACT via twin port (2026-10-20, twin-finder.py): structurally identical
 * (1.000) to game_libs_func_00067B04 (post1b, exact 29/29), whose C carries
 * the two levers the old 18-diff "loop-body CSE cap" lacked: the dead in-guard
 * `q = arg1;` reassignment (q coalesces with c0 in $v0, p takes $v1, plain
 * beq with `move v1,a0` delay instead of beqzl+sltu) and the dead `int r = 0;`
 * initializer (r colors $v1). */
s32 func_80000168(unsigned char *arg0, unsigned char *arg1) {
    int c0 = *arg0;
    unsigned char *q;
    unsigned char *p;
    int r = 0;

    if (c0 != 0) {
        q = arg1;
        if (*arg1 != 0) {
            do {
                p = arg0;
                q = arg1;
                arg1++;
                arg0++;
                if (*q != *p) {
                    return 1;
                }
                c0 = *arg0;
            } while (!((c0 == 0) || (*arg1 == 0)));
        }
    }
    r = c0 != 0;
    if (r == 0) {
        r = *arg1 != 0;
    }
    return r;
}

void func_800001DC(char* dst, char* src) {
    char* s = src;
    unsigned char c = *src;
    char* d = dst;

    *dst = c;
    if (c == 0) {
        return;
    }

    do {
        c = *(s + 1);
        s = s + 1;
        d = d + 1;
        *d = c;
    } while (c != 0);
}

/* uso_find_slash — scan a string for '/' and write its index to *arg1.
 *
 * Returns 1 if found (writing the slash's index to *arg1), 0 if end of
 * string reached. v0 trails v1 by one statement; the duplicate `v0 = v1;`
 * at the top of the loop body is the value-numbering nudge that lets IDO
 * fill both the entry-beqz and loop-back-bnel delay slots with `or v0,v1`. */
s32 func_8000020C(char *arg0, s32 *arg1) {
    s32 v0;
    s32 v1;

    v1 = 0;
    v0 = v1;
    if ((u8)arg0[v0] != 0) {
        do {
            v0 = v1;
            v1++;
            if ((u8)arg0[v0] == '/') {
                v1 -= 1;
                *arg1 = v1;
                return 1;
            }
            v0 = v1;
        } while ((u8)arg0[v1] != 0);
    }
    return 0;
}

extern s32 func_800005DC();
extern s32 func_8000060C();
extern void func_80000660();

/* func_80000260: split "dir/file" at the first '/' (func_8000020C) into dir
 * (strcpy func_800001DC from a 0x40 stack copy) and file; no slash -> dir =
 * whole path, file = "". IDO 7.1 -O2. Kernel jal targets sit 0x400 above our
 * numbering: `jal func_8000060C` is func_8000020C and `jal func_800005DC` is
 * func_800001DC, so the calls go through the absolute names (the 5DC one via
 * the func_800005DC_d2c alias: func_800005DC is also a C definition in this
 * TU). The byte loop is IDO's own 4x unroll of a plain for loop.
 * EXACT 2026-10-20 (agent-g run 112; the old NM wrote the unroll by hand and
 * sat at 0%): the file copy is a while loop (IDO peels its first test, so
 * file[0] gets a constant offset) with explicit post-increment copies
 * m = j++ then k = i++ (that order picks m=a0/k=a1; without the copies IDO
 * strength-reduces the indices into pointers), and `if (1) {}` after j = 0
 * (BB lever: j in v1, the loaded char in v0). */
extern s32 func_800005DC_d2c(void*, void*);
void func_80000260(char *path, char *dir, char *file) {
    char buf[0x40];
    s32 len;
    s32 i;
    s32 j;
    s32 k;
    s32 m;

    if (func_8000060C(path, &len) != 0) {
        for (i = 0; i < len; i++) {
            buf[i] = path[i];
        }
        buf[i] = 0;
        i = len + 1;
        func_800005DC_d2c(dir, buf);
        j = 0;
        if (1) {
        }
        while (m = j++, k = i++, (file[m] = path[k]) != 0) {
        }
    } else {
        func_800005DC_d2c(dir, path);
        file[0] = 0;
    }
}

void func_800003A8(s32 arg0, s32 arg1, s32 arg2) {
    s32 savedMask;
    s32 status;
    u16 first16;
    u16 loop16;

    D_80013000 = 0;
    func_8000A0E0();
    while (func_80006640() & 3) {}
    func_80004A50(0, arg1, arg0, arg2);
    status = func_80006640();
    first16 = status;
    D_80013000 = status;
    if (first16 & 3) {
        do {
            status = func_80006640();
            loop16 = status;
            D_80013000 = status;
        } while (loop16 & 3);
    }
    savedMask = func_80002890(1);
    func_80005350(arg0, arg2);
    func_80005400(arg0, arg2);
    func_80002890(savedMask);
}

/* uso_set_alloc */
extern s32 D_8000A2E0;
void func_80000474(s32 val) {
    D_8000A2E0 = val;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct00480Src;

typedef struct {
    s32 unk0;
    s32 unk4;
    Struct00480Src blk; /* 0x8..0x1C: copied wholesale from src */
    s32 unk1C;
} Struct00480Dst;

/* 2026-06-10 kernel-relayout: previous body (field-by-field copy with
 * unk18/unk1C swapped) was a FALSE match - it emitted 13 insns with
 * CSE'd forwarding vs ROM's 14 (verified pre-relayout against a shifted
 * baseline). ROM truth is a 5-word struct assignment (t7/t6-alternating
 * block-copy idiom) + reload of dst->blk.unk8 + zero of unk4. */
void func_80000480(Struct00480Dst* dst, Struct00480Src* src) {
    dst->blk = *src;
    dst->unk1C = dst->blk.unk8;
    dst->unk4 = 0;
}

/* func_800004B8: named-handler lookup, IDO 7.1 -O2, EXACT. Walks the
 * 0x14-byte entry table at *D_8000A2E0 until a NULL name; on the first
 * entry whose name matches (func_80000568 == 0) it calls
 * func_80000880(a0, entry) and returns &D_80012C70[i] (0xC stride).
 * The old split (this .s without an epilogue + func_80000568.s "shared
 * epilogue stub" that four callers supposedly jal'd) was one function: the
 * kernel's jal targets are 0x400 above our symbol numbering, so `jal
 * func_80000568` is really a call to func_80000168 (the string compare,
 * exact twin of game_libs_func_00067B04). func_80000568 stays an absolute
 * undefined_syms entry for those jals.
 * Levers: `s32 i = 0` initialized at the declaration with a while loop
 * (the pre-loop i*20 and the separate first-name load come from that), and
 * the entry address as byte arithmetic `(char *)tab + i * 20` (tab first
 * in the addu; &tab[i] puts the scaled index first). a2 is unused but
 * homed, as in the target. */
typedef struct { char *name; s32 pad[4]; } HandlerEntry;   /* 0x14 */
typedef struct { s32 w[3]; } HandlerSlot;                   /* 0xC */
extern HandlerSlot D_80012C70[];
extern void func_80000880();
extern s32 func_80000568();

HandlerSlot *func_800004B8(void *a0, char *name, s32 a2) {
    s32 i = 0;

    if (*(HandlerEntry **)&D_8000A2E0 == NULL) {
        return NULL;
    }
    while ((*(HandlerEntry **)&D_8000A2E0)[i].name != NULL) {
        if (func_80000568((*(HandlerEntry **)&D_8000A2E0)[i].name, name) == 0) {
            func_80000880(a0, (HandlerEntry *)((char *)*(HandlerEntry **)&D_8000A2E0 + i * 20));
            return &D_80012C70[i];
        }
        i++;
    }
    return NULL;
}

/* func_80000568 (old "shared epilogue stub") = the tail of func_800004B8,
 * merged into it; the address stays an absolute undefined_syms entry. */

/* uso_stub_ret0 */
s32 func_8000058C(s32 arg0) {
    return 0;
}

/* Byte-copy loop, 16 insns / 0x40 in target. Tightened wrap (14 insns / 0x38,
 * verified standalone IDO -O2) — was previously 30 insns / 0x78 (+52 bytes
 * bloat). Body now matches target's structure (post-decrement do-while with
 * `bnez count, loop; addiu count, -1` epilogue) — still 2 insns short of
 * target's 16:
 *   - missing `or $a3, $a2, $zero` snapshot at top (count snapshot, dead —
 *     a3 is overwritten in loop). Standard `register s32 cp = count;` doesn't
 *     emit it because IDO sees no use post-loop.
 *   - missing `or $a0, $v0, $zero; lbu t, 0(a0)` 2-insn snapshot+load (target
 *     pattern). My emit folds to `lbu a0, 0(v0)` (1-insn direct).
 * Also target uses (a3=dst-snap, a1=count-snap dead); my emit picks
 * (a1=dst-snap, a3=rem-snap). Register-pick diff inside the snapshots.
 *
 * 2026-05-06 retry: tried explicit `register` qualifiers on every local
 * (sp/dp/cp/p/q/rem/c) + `do { ... rem = count--; } while (rem != 0)`
 * post-decrement form to shape the loop epilogue. Got correct loop tail
 * (bnez+addiu in delay slot) but couldn't restore the dead snapshots —
 * IDO's DCE removes any local that isn't read post-assignment, even with
 * `register` and `(void)` casts.
 *
 * Cap class likely INSN_PATCH (target +2 dead ors over my body) or shaped
 * by a different inlining/calling convention not reproducible from std C.
 * The 2 dead `or` insns suggest the original was a hand-written byte-copy
 * (CRT/libc-style) where the dead ARGV-passthrough ors are remnants of
 * an inline-tail-call ABI dance. Stays NM.
 *
 * 2026-05-17: was previously promoted to exact via PREFIX_BYTES +
 * INSN_PATCH + SUFFIX_BYTES_FORCE — all three instruction-injecting
 * mechanisms REMOVED 2026-05-23 as match-faking (per
 * feedback_no_instruction_forcing_matches_policy; only genuine all-zero
 * SUFFIX_BYTES, USO-header PREFIX_BYTES, and TRUNCATE_TEXT remain).
 * Reverted to "stays NM" — default build is INCLUDE_ASM.
 *
 * 2026-05-31 PERMUTER TRIED (was un-tried; now ruled out): the residual is
 * a SIZE mismatch — target keeps 2 redundant `or` ABI-dance moves (16 insns,
 * no frame) that -O2 coalesces (mine=14) while -O1 ADDS a frame (18 insns).
 * decomp-permuter -j2 ~200s FLOORED at score ~230 (base 310), NEVER reached
 * 0 — finds closer variants but no C structure reproduces the exact 2
 * redundant moves at -O2-no-frame. Genuine redundant-move/size cap; not
 * permuter-crackable. (Kernel = non-asm-processor so the permuter runs
 * cleanly here, unlike the game_libs asm-processor floor.) */
/* 2026-10-23 EXACT (agent-g run 144) via the IDO 7.1 NCC donor
 * src/kernel/kernel_ncc_598.cpp: the plain `while (n--) *d++ = *s++;` byte
 * copy over char-pointer locals compiled as C++ keeps every post-increment /
 * post-decrement temp (the "dead ABI-dance ors" above, 16/16 words); cfe
 * folds them (12-15 words). This definition is only the splice placeholder. */
void func_80000598(u8 *src, u8 *dst, s32 count) {
}

/* func_800005DC (really starts at 0x5D8: the .s glabel sits on the
 * prologue word) = bounded file read, IDO 7.1 -O2: buf must be 8-aligned
 * (else errno D_80013004 = -0x1C), size*n clamped to capacity - pos, read
 * via the D_80012C40 hook, pos advanced. The alabels func_8000060C /
 * func_80000660 inside the .s are not entries: kernel jal targets sit 0x400
 * above our numbering, so `jal func_8000060C` / `jal func_80000660` are
 * calls to 0x20C / func_80000260 (absolute undefined_syms entries).
 * EXACT 2026-10-20 (agent-g run 112): `return D_80013004 = -0x1C;` is what
 * puts the errno address in v1 and the value in v0. */
extern s32 D_80013004;
typedef s32 (*ReadFunc)(void *, u32, void *);
extern ReadFunc D_80012C40;
typedef struct {
    s32 pad0;
    s32 pos;     /* +0x4 */
    s32 pad8;
    char *buf;   /* +0xC */
    s32 padN[3];
    u32 capacity; /* +0x1C */
} FileStateRdr;
s32 func_800005DC(s32 buf, u32 size, u32 n, FileStateRdr *fp) {
    s32 total;

    if (buf & 7) {
        return D_80013004 = -0x1C;
    }
    total = size * n;
    if (fp->capacity < total + fp->pos) {
        total = fp->capacity - fp->pos;
    }
    if (total < 0) {
        return 0;
    }
    if (total != 0) {
        D_80012C40((void *)buf, (u32)(fp->buf + fp->pos), (void *)total);
    }
    fp->pos += total;
    return total;
}

/* uso_advance_position */
void func_80000688(s32* file, s32 offset) {
    file[1] += offset;
}

s32 uso_file_open(FileState* file, u32* arg1) {
    struct {
        u32 type;
        u32 size;
        u32 unk8;
        s32 pad;
    } header;

    if (func_800009D8(&header, 0xC, 1, file) < 0) {
        return D_80013004;
    }
    if (header.type == 0xA) {
        return 0;
    }
    if (func_800009D8(arg1, header.size, 1, file) < 0) {
        return D_80013004;
    }
    if (*arg1 != 0x12345678) {
        return -0x15;
    }
    return 1;
}

/* uso_skip_to_end: reads USO section headers until End (type 11).
 *
 * CRACKED 2026-06-11 (rules sweep wave 2, 100.0): the bnel operand-order
 * "cap" was ucode shape, not emit normalization. uoptinput.c swaps ==/!=
 * operands when the LEFT operand is not an isvar at READ time. A local
 * ARRAY element (header[0]) reads as lda+ilod (isop) -> always swapped
 * to const-first (bnel s3,t0). A local STRUCT FIELD (header.type) reads
 * as a direct var ref (isvar) -> no swap -> var-first (bnel t0,s3).
 * So the original buffer is a 3-word STRUCT, not u32[3]. Same emitted
 * bytes everywhere else (escaped local -> conservative temp reloads). */
s32 uso_skip_to_end(FileState *file)
{
  s32 pad;
  struct { u32 type; u32 size; u32 x; } header;
  s32 new_var;
  do
  {
    new_var = func_800009D8(&header, 12, 1, file);
    pad = header.type;
    if (new_var < 0)
    {
      return D_80013004;
    }
    if (8 != pad)
    {
      ((s32 *) file)[1] += header.size;
    }
  }
  while (header.type != 11);
  return 0;
}


/* uso_file_check — always returns 1 (success) */
s32 func_800007C4(s32 a0, s32 a1) {
    return 1;
}

typedef struct {
    char pad_00[0x14];
    s32 field_14;
    char pad_18[0x34];
    s32 field_4C;
    char pad_50[0x22];
} UsoEntry74;

typedef struct {
    char pad_00[0x84];
    void (*field_84)(void*, void*, s32, s32);
} UsoCallbacks84;

typedef struct {
    char pad_00[0x84];
    void (*field_84)();
} UsoLooseCb;

extern s32 D_8000A2E4;
extern s32 D_80012F7C;

typedef struct { u8 pad_00[0x72]; char name[0x22]; s32 unk94; u8 pad_98[4]; } UsoLoadSlotF;
/* func_800007D4: for every live module slot (D_80012D60[0..0x86]) call the
 * loader hook D_80012BC0.field_84(&D_8000A2E4, name, base, base + size).
 * IDO 7.1 -O2. EXACT 2026-10-20 (agent-g run 112; was NM 83.0, written as a
 * pointer walk with four `register` invariants and called a coloring cap):
 * a plain index loop lets uopt build the p/end walk and hold &D_80012BC0 in
 * s2 itself. */
void func_800007D4(void) {
    s32 i;
    UsoEntry74 *e;

    for (i = 0; i < 0x87; i++) {
        e = ((UsoEntry74 **)&D_80012D60)[i];
        if (e != 0) {
            ((UsoCallbacks84 *)&D_80012BC0)->field_84(&D_8000A2E4, (char *)e + 0x72, e->field_4C, e->field_4C + e->field_14);
        }
    }
}

typedef struct {
    char pad_00[0x94];
    s32 field_94;
} UsoTable94;

extern s32 D_8000A2F4;

/* func_8000085C: call the loader hook D_80012BC0.field_84(&D_8000A2F4, i,
 * slot->unk94, slot->name) for all 135 module slots. IDO 7.1 -O2. The old
 * NM (a hand pointer walk) was filed as a coloring cap. */
/* EXACT 2026-10-20 (agent-g run 112): a plain index loop over the typed
 * 0x9C-byte slot array (the two "no-reloc absolute bases" are &slots[0] and
 * &slots[0].name; the .s now spells them %hi/%lo(D_800130A0[+0x72])). */
void func_8000085C(void) {
    s32 i;

    for (i = 0; i < 0x87; i++) {
        ((UsoCallbacks84 *)&D_80012BC0)->field_84(&D_8000A2F4, i, ((UsoLoadSlotF *)&D_800130A0)[i].unk94, ((UsoLoadSlotF *)&D_800130A0)[i].name);
    }
}



/* func_800008F0: unload a USO module by path. Debug-print via the
 * D_80012C44 hook, split the path (func_80000660 = func_80000260: dir ->
 * D_80012FC0, file -> D_80012F80), find the D_80012D60 slot whose name (+0x72)
 * equals the file (func_80000568 = func_80000168 strcmp, 0 = equal), clear it
 * and notify every other live slot (func_800021D0 / func_8000275C, i.e. our
 * 0x1DD0 / 0x235C). IDO 7.1 -O2.
 * EXACT 2026-10-20 (agent-g run 112): the old NM called 568/660 through
 * function-pointer casts (jalr) believing they were shared-epilogue stubs;
 * kernel jal targets sit 0x400 above our numbering, so they are plain
 * direct calls to the absolute names. The inner loop's end pointer is
 * D_80012D60+0x21C (the target .s names it D_80012F7C; same address). */
extern s32 D_8000A300;
extern s32 D_80012F80;
extern s32 D_80012FC0;
extern s32 func_800021D0();
extern s32 func_8000275C();
extern s32 D_8000A314;
extern s32 D_8000A328;
extern s32* func_800008B8(void*, void*, void*);
extern s32 func_80000A98();

s32 func_800008F0(char *path) {
    s32 i;
    s32 j;
    char *e;

    D_80012C44((void *)&D_8000A300, path);
    func_80000660(path, &D_80012FC0, &D_80012F80);
    for (i = 0; i < 0x87; i++) {
        if (D_80012D60[i] != 0 &&
            func_80000568(&D_80012F80, (char *)D_80012D60[i] + 0x72) == 0) {
            *((char *)D_80012D60[i] + 0x98) = 0;
            D_80012D60[i] = 0;
            for (j = 0; j < 0x87; j++) {
                e = (char *)D_80012D60[j];
                if (e != 0) {
                    func_800021D0(e, i);
                    func_8000275C(e, i);
                }
            }
            return 0;
        }
    }
    return 0;
}

/* func_800009D8 (5-insn shared-tail epilogue: lw s1/s2/s3, jr ra, addiu sp,+0x28)
 * merged into predecessor func_8000098C (size 0x4C → 0x60). Address re-exported
 * via undefined_syms_auto.txt for the many cross-jal callers (kernel_000.c
 * line 383, 389, 409, etc.). */
/* func_800009EC: USO module loader, IDO 7.1 -O2. ONE function [0x9EC,0xC88)
 * split by splat into func_800009EC (no epilogue) + func_80000A88 + func_80000A98
 * (the jal'd 0x80000A88/0A98/0B34 are other functions: kernel jal targets
 * sit 0x400 above our symbol numbering). Callees by real address: 660 ->
 * 0x260, 8B8 -> func_800004B8 (handler lookup), 568 -> func_80000168
 * (strcmp), 22C8 -> 0x1EC8, 25A4/2650/20F4 -> 0x21A4/0x2250/0x1CF4.
 * 2026-10-20 (agent-g, run 111): the three .s merged into func_800009EC.s
 * (0x29C; the module-base lui/addiu re-symbolized as %hi/%lo(D_800130A0),
 * the bnez into 0xB34 made a local label) and this redecode, 2 words off.
 * Levers found:
 * - the module base 0x800130A0 is a %hi/%lo symbol (D_800130A0; splat left
 *   it a literal, which compiles to lui/ori);
 * - one `if (1) {}` after `err = 0` makes &D_8000A2D8 rematerialize
 *   (agent-c's BB-count lever) instead of holding it in s3;
 * - len/off named temps for the func_8000098C args;
 * - the three cleanup loops index D_80012D60[j] (IDO strength-reduces to the
 *   target's s0/s1 pointer+end walk, incl. bnel on the third);
 * - header is s32[8] with off/len/new_var/desc declared BEFORE it (their
 *   homes fill 0x70..0x80), found at 0x48, i spills at 0x44;
 * - `new_var = D_80012D60` read in loop 1's test (permuter find) gives the
 *   target's mod-in-s1 / i-in-v1 coloring.
 * EXACT 2026-10-22 (agent-g run 118): the last 2 words (lui s0/s1 order)
 * were the loop-1 induction-variable shape. desc is NOT a separate stepped
 * pointer: the call takes `&D_80013112 + i * 0x9C` (uopt strength-reduces it
 * into its own IV, created after mod's -> mod's lui schedules first), and
 * the for-init is the comma `mod = base, i = 0` with the step `mod++, i++`
 * (mod init before the loop as its own statement, or `i++, mod++`, leaves
 * the two addiu or the bne-delay increment swapped). desc stays declared
 * (unused) only for the frame layout: dropping it shrinks the frame by 8. */
typedef struct UsoModule {
    char unk0[0x70];
    s16 unk70;          /* slot id */
    char unk72[0x94 - 0x72];
    s32 unk94;
    char pad98[4];
} UsoModule;

extern char D_80013112;                 /* = &D_800130A0[0].unk72 */
extern s32 D_8000A314;
extern s32 D_8000A328;
extern s32 D_80012FC0;
extern s32 D_80012F80;
extern void func_80001CB0();
extern s32 func_800022C8();
extern s32 func_800025A4();
extern s32 func_80002650();
extern s32 func_800020F4();

s32 func_800009EC(s32 file) {
    s32 off;
    s32 len;
    s32 *new_var;
    char *desc;
    s32 header[8];
    s32 *range;
    UsoModule *found;
    s32 i;
    s32 j;
    UsoModule *mod;
    s32 err;

    D_80012C64 = -1;
    D_80012C68 = -1;
    err = 0;
    if (1) {
    }
    D_80012C44(&D_8000A314, (void *)file);
    ((s32 *)&D_80012BC0)[6] = 0;
    ((s16 *)&D_80012BC0)[0x16] = 0;
    func_80000660(file, &D_80012FC0, &D_80012F80);
    range = func_800008B8(header, &D_80012FC0, &D_8000A328);
    if (range == NULL) {
        return -0x11;
    }
    D_8000A2D8 = range[0];
    D_8000A2DC = range[1];
    found = NULL;
    for (mod = (UsoModule *)&D_800130A0, i = 0; i != 0x87; mod++, i++) {
        if (func_80000568(&D_80013112 + i * 0x9C, &D_80012F80) == 0) {
            found = mod;
            func_80001CB0(mod);
            mod->unk70 = i;
            if (D_80012D60[i] != 0) {
                return -0x13;
            }
            D_80012D60[i] = (s32)mod;
            header[1] = mod->unk94;
            err = func_800022C8(mod, header);
            if (err != 0) {
                return err;
            }
            break;
        }
    }
    new_var = D_80012D60;
    if (found == NULL) {
        return -0x14;
    }
    len = range[1] - range[0];
    off = D_8000A2D8 - range[0];
    ((s32 *)&D_80012BC0)[1] = len - off;
    ((s32 *)&D_80012BC0)[0] = D_8000A2D8;
    func_8000098C(header, len, off, &D_80012BC0);
    for (j = 0; j < 0x87; j++) {
        if (new_var[j] != 0) {
            err |= func_800025A4(D_80012D60[j]);
        }
    }
    if (err != 0) {
        return -0xA;
    }
    for (j = 0; j < 0x87; j++) {
        if (D_80012D60[j] != 0) {
            err |= func_80002650(D_80012D60[j]);
        }
    }
    if (err != 0) {
        return -0xC;
    }
    for (j = 0; j < 0x87; j++) {
        if (D_80012D60[j] != 0) {
            err |= func_800020F4(D_80012D60[j]);
        }
    }
    if (err != 0) {
        return err;
    }
    range[0] = D_8000A2D8;
    return found->unk70;
}

/* uso_call_init — dispatches a function from the USO module vtable */
typedef s32 (*UsoFunc)(void*);

s32 func_80000C88(void) {
    s32 modIndex = D_80012C68;
    s32* mod;
    s32 funcIndex;
    s32 funcTable;

    if (modIndex < 0) {
        return -1;
    }

    mod = (s32*)D_80012D60[modIndex];
    if (mod == 0) {
        return -1;
    }

    funcIndex = D_80012C64;
    if (funcIndex < 0) {
        return -1;
    }

    funcTable = *(s32*)((char*)mod + 0x3C);
    if (funcTable == 0) {
        return -1;
    }

    {
        /* (t = funcTable) assignment-expr pins funcTable as the addu rs
         * (ROM: addu t8,a2,t7) - docs/IDO_CODEGEN integer commutative
         * operand-order lever; plain source-order swap does not flip it.
         * (2026-06-10 kernel relayout audit; was the last kernel_000
         * non-reloc word diff.) */
        s32 t;
        UsoFunc fn = (UsoFunc)*(s32*)((t = funcTable) + funcIndex * 12 + 4);
        D_80012BE4 = fn(&D_80012BC0);
    }
    return 0;
}

/* func_80000D2C: name-lookup. If D_80012D60[id] is set, strcpy its name (at
 * +0x72) into out and return 2. Else iterate the D_8000A2E0 list (0x14 stride):
 * for each entry, enumerate via func_80000880/func_80000A98; on an entry whose
 * +0x4 field == id, copy its name (func_800005DC, src=entry+0xC) and return 1;
 * func_80000B34<0 aborts with D_80013004. (Old 83% NM history: the frame
 * excess was the byte-array locals, not an IDO cap -- see the note above the
 * definition below; the bne operand order and t3 flag resolved with it.) */
extern s32 func_8000098C(void*);
extern s32 func_80000B34(void*);
/* func_800005DC is defined 4-arg earlier in this merged TU; call it 2-arg here
 * via an alias symbol (undefined_syms_auto.txt: func_800005DC_d2c = 0x800005DC)
 * so the redecl is legal and emits a direct jal (not the cast's lui+addiu+jalr).
 * See docs/MATCHING_WORKFLOW.md#feedback-alias-extern-via-undefined-syms. */
extern s32 func_800005DC_d2c(void*, void*);
#define DSC func_800005DC_d2c
typedef struct { u8 pad_00[0x72]; u8 name[0x22]; s32 unk94; u8 pad_98[4]; } UsoLoadSlot;
typedef struct { s32 unk0; s32 idx; s32 unk8; } UsoLoadRec;
typedef struct { s32 unk0; s32 unk4; s32 pad[8]; } UsoLoadIter;
typedef struct { s32 head; s32 pad[4]; } UsoLoadReq;
/* USO directory entry yielded by func_80000A98 (record + name). */
typedef struct { s32 unk0; s32 idx; s32 unk8; u8 name[0x18]; } UsoLoadEnt;

/* func_80000D2C: EXACT 2026-10-16 (agent-g). Typed stack aggregates fix the
 * 8-byte frame excess the byte-array body had (0x98 vs 0x90): iterator 0x28
 * (same UsoLoadIter as func_800014A8) + entry 0x24, declared req, it, slot,
 * i, ent, c -- IDO maps the frame top-down in declaration order and the
 * scalar homes fill the holes, so both the aggregate sizes and the decl order
 * matter (found by a small size x order sweep; 3 of ~150 combos are exact). */
s32 func_80000D2C(u8 *out, s32 id) {
    UsoLoadReq *req;
    UsoLoadIter it;
    u8 *slot;
    s32 i;
    UsoLoadEnt ent;
    u8 c;

    slot = (u8 *)D_80012D60[id];
    req = (UsoLoadReq *)D_8000A2E0;
    if (slot != NULL) {
        c = slot[0x72];
        i = 0;
        out[0] = c;
        if (c != 0) {
            do {
                i++;
                c = ((u8 *)D_80012D60[id])[i + 0x72];
                out[i] = c;
            } while (c != 0);
        }
        return 2;
    }
    for (; req->head != 0; req++) {
        func_80000880(&it, req);
        while (func_80000A98(&it, &ent) != 0) {
            if (ent.idx == id) {
                func_800005DC_d2c(out, ent.name);
                return 1;
            }
            if (func_80000B34(&it) < 0) {
                return D_80013004;
            }
        }
        func_8000098C(&it);
    }
    return 0;
}

void func_80000E58(void) {
    D_80012C44(&D_8000A32C, &D_8000A340);
}

extern s32 D_8000A344;
/* Address -> loaded-USO resolver: scan the USO slot-pointer table
 * (&D_80012D60 .. &D_80012F7C); for the first loaded slot whose
 * [field_4C, field_4C+field_14) range contains arg0, return its code base
 * (slot+0x72) and the in-USO offset (arg0 - field_4C). Not found -> &D_8000A344
 * / 0. IDO -O2 unrolls the simple slot loop x4 with a 3-iteration prologue.
 * The `tbl` decl inside the loop body is load-bearing: it makes IDO recompute
 * the table base each iteration (matching the target's coloring exactly). */
void func_80000E8C(u32 arg0, void **arg1, s32 *arg2) {
    s32 i;

    for (i = 0; i < 135; i++) {
        UsoEntry74 **tbl = (UsoEntry74 **) &D_80012D60;
        UsoEntry74 *e = tbl[i];
        if (e != NULL) {
            if ((arg0 >= (u32) e->field_4C) && (arg0 < (u32) (e->field_4C + e->field_14))) {
                *arg1 = (char *) e + 0x72;
                *arg2 = arg0 - e->field_4C;
                return;
            }
        }
    }
    *arg1 = &D_8000A344;
    *arg2 = 0;
}

/* uso_get_vtable — returns pointer to USO vtable */
extern s32 D_80012BC0;
void* func_800010B4(void) {
    return &D_80012BC0;
}

/* uso_set_callbacks (no-op) */
void func_800010C0(s32 a0, s32 a1) {
}

void func_800010CC(s32 a0, ...) {
}

extern s32 D_8000A34C;
extern s32 D_8000A35C;
extern s32 D_8000A2A0;
extern s32 D_80012D30;

/* func_800010E8: dump the 11 loader counters through the D_80012BC0.field_84
 * logger: one header line with D_80012D5C, then (&D_8000A35C, name[i],
 * D_80012D30[i]) per entry. IDO 7.1 -O2.
 * EXACT 2026-10-20 (agent-g run 112; was NM 58.5, a pointer walk with five
 * `register` locals filed as a 5th-callee-save coloring cap): a plain
 * index loop. The .s loop bound is now spelled %hi/%lo(D_80012D30 + 0x2C)
 * (same address as D_80012D5C, which the header line reads as a value). */
void func_800010E8(void) {
    s32 i;

    ((UsoLooseCb *)&D_80012BC0)->field_84(&D_8000A34C, D_80012D5C);
    for (i = 0; i < 11; i++) {
        ((UsoLooseCb *)&D_80012BC0)->field_84(&D_8000A35C, (&D_8000A2A0)[i], (&D_80012D30)[i]);
    }
}

extern s32 D_80012D30;
extern s32 D_80012D34;
extern s32 D_80012D38;
extern s32 D_80012D3C[];
extern s32 D_80012D5C;
extern s32 D_80012D5C_alias; /* distinct extern @ 0x80012D5C: breaks store<->end CSE */

/* func_80001184 -- EXACT (agent-g run 144) via the IDO 7.1 NCC donor
 * src/kernel/kernel_ncc_1184.cpp; this definition is only the splice
 * placeholder. The old "two IDO caps" verdict (one `lui at` shared by the
 * 0x80012D30 / 0x80012D34 stores; loop pointer in v1) was module-local data:
 * with the 11-word table as a `static int sTab[11]`, as1 shares the `lui at`
 * and the plain zeroing loop compiles to the target (see the donor). */
void func_80001184(void) {
}

/* uso_find_file(path, file): split the path (func_80000660 = func_80000260:
 * dir -> D_80012FC0, name -> D_80012F80), look the directory up
 * (func_800008B8 = func_800004B8, -0x11 if unknown), then scan the 135
 * 0x9C-byte module slots at D_800130A0 for that name (func_80000568 =
 * func_80000168 strcmp); on a hit copy slot->unk94 into file[1]. IDO 7.1 -O2.
 * EXACT 2026-10-20 (agent-g run 112; was NM 92.3): the old pointer-walk
 * spelling left the loop-preamble addiu order and the found-block schedule
 * off; an index loop over a TYPED slot array (`slots[i].name` /
 * `slots[i].unk94`) gives both. The 0x30-byte hole at the top of the frame
 * is an unused local (any 0x29..0x2C-byte array; s32 pad[11] here). */
extern char D_8000A368, D_80013112, D_80018356;
s32 uso_find_file(void *arg0, void *arg1) {
    s32 pad[11];
    s32 i;
    s32 found;
    func_80000660(arg0, &D_80012FC0, &D_80012F80);
    if (func_800008B8(arg1, &D_80012FC0, &D_8000A368) == 0) {
        return -0x11;
    }
    found = 0;
    for (i = 0; i < 135; i++) {
        if (func_80000568(((UsoLoadSlotF *)&D_800130A0)[i].name, &D_80012F80) == 0) {
            found = 1;
            ((s32 *)arg1)[1] = ((UsoLoadSlotF *)&D_800130A0)[i].unk94;
            break;
        }
    }
    if (found == 0) {
        return -0x11;
    }
    return 0;
}

/* uso_get_error — returns last error code */
s32 func_800012AC(s32 arg0) {
    return D_80013004;
}

extern void* (*D_80012BF4)(s32, s32);
extern s32 func_800015D0(void*, void*);

/* uso_open_file: alloc 0x28-byte file struct, init via func_800015D0, read
 * 12-byte header. Byte-exact: the `char pad[8]` declared BEFORE `header`
 * makes IDO place header[3] at sp+0x18 (and the 8-byte pad in the 0x24-0x2B
 * gap) to match the target frame layout. */
void* func_800012BC(void* arg0) {
    void* file;
    char pad[8];  /* frame padding to push header to sp+0x18 */
    s32 header[3];

    file = D_80012BF4(0x28, 8);
    if (func_800015D0(arg0, file) < 0) {
        return 0;
    }
    if (func_800009D8(header, 0xC, 1, file) < 0) {
        return 0;
    }
    ((s32*)file)[8] = ((s32*)file)[1];
    ((s32*)file)[7] = header[1] + ((s32*)file)[1];
    return file;
    (void)pad;
}

extern void func_80000A88(void*, s32);

/* USO record-walk loop: open via func_800015D0, read 12-byte records with
 * func_800009D8 until type==6 (end), dispatching others to func_80000A88.
 * Byte-exact: `char pad_hi[4]` (above header) + `file` declared AFTER `header`
 * reproduce the target frame (file@sp+0x30, header@sp+0x58, frame 0x68); the
 * `result = header[0]` reuse pins the record-type into $v0 as the beql rs. */
s32 func_80001348(void* arg0, s32* arg1) {
    char pad_hi[4];
    s32 header[3];
    s32 file[10];
    s32 result;

    result = func_800015D0(arg0, file);
    if (result < 0) {
        return result;
    }

    while (1) {
        result = func_800009D8(header, 0xC, 1, file);
        if (result <= 0) {
            if (result == 0) {
                result = -1000;
            }
            return result;
        }
        result = header[0];
        if (result == 6) {
            break;
        }
        if (result == 8) {
            continue;
        }
        func_80000A88(file, header[1]);
    }
    *arg1 = file[1] + file[3];
    (void)pad_hi;
    return 0;
}

/* uso_read */
s32 func_80001414(void* file, void* buf, s32 size) {
    extern s32 D_80013004;
    extern s32 func_800009D8(void*, s32, s32, void*);
    s32 result;
    result = func_800009D8(buf, size, 1, file);
    if (result < 0) {
        return D_80013004;
    }
    return result;
}

/* uso_file_end — returns file position + size */
s32 func_80001460(s32* file) {
    return file[1] + file[3];
}

/* uso_seek — set or adjust file position */
s32 func_80001470(s32* file, s32 whence, s32 offset) {
    if (whence) {
        file[1] += offset;
    } else {
        file[1] = offset;
    }
    return file[1];
}

/* uso_file_close (no-op) */
void func_80001494(s32 arg0) {
}

/* uso_ret_zero — returns 0.0f */
float func_8000149C(void) {
    return 0.0f;
}

/* func_800014A8: USO loader main loop. Walk the pending-request list
 * (D_8000A2E0, 0x14-byte entries, NULL-head terminated); for each, open a
 * record iterator (func_80000880), and for every record yielded by
 * func_80000A98 whose slot index is valid, read the USO body into
 * D_800130A0[idx].name (+0x72, 0x9C-stride slot) and stamp slot +0x94 from
 * the iterator. EXACT 2026-10-16 after splitting func_80001594 out of the
 * bundled .s: the old 30% NM body used raw byte arrays + an explicit
 * do/while; typed stack structs + for/while give the 0x98 frame and the
 * s3=base / s4=0x9C coloring. */

void func_800014A8(void) {
    UsoLoadIter it;
    u8 buf[0x24];
    UsoLoadRec rec;
    UsoLoadReq *req;
    UsoLoadSlot *slot;

    for (req = (UsoLoadReq *)D_8000A2E0; req->head != 0; req++) {
        func_80000880(&it, req);
        while (func_80000A98(&it, &rec) != 0) {
            if (rec.idx >= 0) {
                slot = &((UsoLoadSlot *)&D_800130A0)[rec.idx];
                func_800005DC_d2c(slot->name, buf);
                slot->unk94 = it.unk4;
            }
            func_80000B34(&it);
        }
    }
}

/* func_80001594: split out of func_800014A8.s (2026-10-16) -- the .s bundled
 * two functions (14A8 ends jr ra at 0x158C; 1594 opens its own sp -0x20
 * prologue). USO loader init: fills the D_80012BC0 dispatch table, clears
 * the D_80012D60/D_800130A0 slot arrays, copies 16 segment records. */
#ifdef NON_MATCHING
/* NM 95.79% 2026-10-16 (agent-g, was bare 0%): size-exact (136/136 insns).
 * Body is complete:
 * install the USO loader callback table at D_80012BC0 (cb, held in a3 via the
 * int-cast base -- a plain `s32 *cb` gets folded to $at-macro stores and loses
 * a3), clear the 135 0x9C-stride D_800130A0 slots' name byte + D_80012D60
 * words, then copy up to 16 0x14-byte segment descriptors into D_80012C70
 * (0xC stride), failing with cb->unk8 = -0x17 when a segment end passes
 * arg1 + 0x80000000. The store ORDER matters for the a0/a2 CSE'd addends and
 * the as1 schedule (the target's emission order is NOT the source order).
 * Residual: loop-2 colouring (target ptr=v1 i=a1 e=v0 c=t6 lim=t7; this gives
 * ptr=v0 i=v1 e=a0 c=a1 lim=a2 under every spelling tried: decl order (all
 * 5040 perms), index vs pointer walk, while/goto/&&, u32 i, assignment-in-
 * condition), one sltu-vs-sw swap in loop 1, and three no-reloc words in the
 * target: 0x3C = lui 0x8000/addiu 0x1994 with NO reloc (a literal gives
 * ori; func_80001994 gives the addiu but adds a reloc), and %hi/%lo of the
 * local labels .L80000C5C / .L800007A8 (spelled D_80000C5C / D_800007A8),
 * plus the _a alias names. A function-typed (char *)func_80000A98 + 0x1C4
 * for .L80000C5C drops it to 82.8 (base CSE again).
 * Run 163 (agent-g, 95.98 -> 96.62): loop 2's c and lim are written ON THE
 * FIELD (`e->unk0 = ...; if (e->unk0 != 0)`, `e->unk4 = ...; e->unk8 =
 * e->unk0;`, compare through `(u32)e->unk4`, then `lim = e->unk4` read BEFORE
 * the cb + 8 store): uopt forwards the stored values, so c / lim become the
 * target's ugen t6 / t7 and ptr / e take v1 / v0 (the old empty tests on arg1
 * and c are gone); the cb + 0x34 store placed before cb + 0x30 gives the
 * target's 52-then-48 order. Left: i = a0 (target a1 -- the forwarded c LR,
 * adjsave 15, is coloured after i at 20.5), the 480 constant a1 (target a2),
 * the loop-1 lui order, one sltu / sw swap and the no-reloc words. */
typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; u32 unk10; } UsoSegDesc;
typedef struct { s32 unk0; s32 unk4; s32 unk8; } UsoSegEnt;
extern void *D_8001309C;
extern void func_80001584(void *, u32);
extern void func_80000874(void *);
extern void func_800018A8(void);
extern char func_80001994[], func_80000DEC[], D_80000CF0[], func_80001088[];
extern char D_80001258[], D_8000128C[], D_80000BD4[], D_800016BC[];
extern char D_80001814[], D_80001894[], D_80001870[], D_80001748[];
extern char D_80000C5C[], D_800007A8[], D_800016AC[], D_80001860[], D_8000189C[];
/* char-typed aliases of TU-defined functions: a function-typed base gets one
 * CSE'd lui/addiu plus per-use `addiu rX,base,K`, while the target folds each
 * addend into its own %hi/%lo pair (array-typed extern behaviour). */
extern char func_800014A8_a[], func_800010E8_a[], func_80000480_a[];

void func_80001594(UsoSegDesc *arg0, u32 arg1) {
    s32 cb;
    s32 *p;
    u8 *q;
    s32 i;
    UsoSegEnt *e;
    u32 lim;

    func_80001584(arg0, arg1);
    D_8001309C = &D_800130A0;
    func_80000874(arg0);
    cb = (s32)&D_80012BC0;
    *(s32 *)(cb + 0x3C) = (s32)func_80001994;
    *(s32 *)(cb + 0x84) = (s32)(func_800014A8_a + 0x24);
    *(s32 *)(cb + 0x40) = (s32)func_80000DEC;
    *(s32 *)(cb + 0x8) = 0;
    *(s32 *)(cb + 0x88) = (s32)(func_800014A8_a + 0x24);
    *(s32 *)(cb + 0x44) = (s32)D_80000CF0;
    *(s32 *)(cb + 0x64) = (s32)func_80001088;
    *(s32 *)(cb + 0x68) = (s32)(func_800010E8_a + 0x44);
    *(s32 *)(cb + 0x34) = (s32)(func_80000480_a + 0x30);
    *(s32 *)(cb + 0x6C) = (s32)D_80001258;
    *(s32 *)(cb + 0x30) = (s32)(func_80000480_a + 0x30);
    *(s32 *)(cb + 0x70) = (s32)D_8000128C;
    *(s32 *)(cb + 0x74) = (s32)func_80001584;
    *(s32 *)(cb + 0x78) = (s32)(func_800014A8_a + 0x40);
    *(s32 *)(cb + 0x7C) = (s32)D_80000BD4;
    *(s32 *)(cb + 0x38) = 0;
    *(s32 *)(cb + 0x4C) = (s32)D_800016BC;
    *(s32 *)(cb + 0x50) = (s32)D_80001814;
    *(s32 *)(cb + 0x58) = (s32)D_80001894;
    *(s32 *)(cb + 0x54) = (s32)D_80001870;
    *(s32 *)(cb + 0x48) = (s32)D_80001748;
    *(s32 *)(cb + 0xA0) = (s32)D_80000C5C;
    *(s32 *)(cb + 0x8C) = (s32)(func_800014A8_a + 0x24);
    *(s32 *)(cb + 0x90) = (s32)(func_800014A8_a + 0x18);
    *(s32 *)(cb + 0x80) = (s32)D_800007A8;
    *(s32 *)(cb + 0x5C) = (s32)D_800016AC;
    *(s32 *)(cb + 0x60) = (s32)D_80001860;
    *(s32 *)(cb + 0x94) = (s32)D_8000189C;
    q = (u8 *)&D_800130A0;
    p = D_80012D60;
    do {
        q += 0x9C;
        *p = 0;
        q[-0x2A] = 0;
        p++;
    } while (q < (u8 *)&D_800182E4);
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
#else
INCLUDE_ASM("asm/nonmatchings/kernel", func_80001594);
#endif




void func_800017B4(void) {
}

/* func_800017BC: allocate the first free module slot (in_use byte at +0x98)
 * of the 135 0x9C-byte slots at D_800130A0, or NULL. IDO 7.1 -O2.
 * EXACT 2026-10-20 (agent-g run 112; was NM 67.0): it is one plain loop;
 * the three "singleton" checks are IDO peeling 135 % 4 = 3 iterations
 * before its x4 unroll. Char arithmetic for both the flag and the return
 * keeps a single strength-reduced pointer (a typed slots[i] spelling
 * builds a second offset IV for the return). The old func_800018A8
 * ("empty function", jr ra; nop) was this function's return, split off by
 * splat; its address stays an absolute undefined_syms entry. The .s now
 * spells the slot addresses D_800130A0 + K (ROM bytes unchanged). */
void* func_800017BC(void) {
    s32 i;

    for (i = 0; i < 135; i++) {
        if (((u8 *)&D_800130A0)[i * 0x9C + 0x98] == 0) {
            ((u8 *)&D_800130A0)[i * 0x9C + 0x98] = 1;
            return (u8 *)&D_800130A0 + i * 0x9C;
        }
    }
    return 0;
}


void func_800018B0(void* arg0) {
    s32 i;
    char* ptr;

    *(u8*)((char*)arg0 + 0x99) = 1;
    *(s32*)((char*)arg0 + 0x38) = 0;
    *(s32*)((char*)arg0 + 0x3C) = 0;
    /* 2026-06-10 kernel-relayout: previous do-while form was a FALSE match
     * (v0/v1 swapped + bound/counter pre-header order; verified against a
     * shifted baseline). for-comma-init pins ptr->$v1, bound-before-counter
     * (docs/IDO_CODEGEN.md for-loop comma-init lever). Now byte-exact. */
    for (i = 2, ptr = (char*)arg0 + 8; i != 0xE; ) {
        i += 4;
        *(s32*)(ptr + 0x38) = 0;
        *(s32*)(ptr + 0x3C) = 0;
        *(s32*)(ptr + 0x40) = 0;
        *(s32*)(ptr + 0x44) = 0;
        ptr += 0x10;
    }
}

/* func_800018F0 - splat over-split repaired: merged former
 * func_80001994 (0x148) back in (was split at the internal early-return
 * `bnez a2; jr ra; v0=0` ~0x80001900, NOT a function boundary). True
 * size 0x1EC (123 insns, ends 0x80001AD8); func_80001ADC is the next
 * genuine function. func_80001994 = 0x80001994 added to
 * undefined_syms_auto.txt for the in-function `bnel ...,0x80001994`.
 * Per docs/MATCHING_WORKFLOW.md#feedback-split-fragments-over-splits-
 * on-internal-early-return.
 * Structural decode: s32 func_800018F0(tbl a0, a1, a2, s32 a3, ...)
 *   table-scan/insert helper. Early-outs: if (!a1) ret on a0->0x3C
 *   path; if (a2) continue else return 0; if (a3 <= 0) goto tail
 *   (return 0). Walks a3 entries of a 12-byte (0xC-stride) record
 *   array based at a0->0x3C (idx = (rec>>4)*12); per entry checks
 *   (h&7)==1, (h>>4)==arg@sp+0x10, (h&8)!=0 to find a match; on hit
 *   links via *a2 chain (t1=a2, rec->0x4/0x8 copied), sets flags
 *   (&~8 etc.), returns 1; no match returns 0. a0->0x3C = record
 *   table base; record: +0 s16 header (bit0-2 kind, bit3 flag, >>4
 *   id), +0x4 / +0x8 link words. Caps <80: beql/bnel branch-likely
 *   x3 + multu/mflo index + cross-block b-to-shared-tail. Full body
 *   INCLUDE_ASM-preserved. INCLUDE_ASM (no episode; tautology-trap). */
#ifdef NON_MATCHING
#ifndef FW
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
#endif
/* USO relocation/fixup applier: walk arg2's 0xC-byte fixup records (count arg3,
 * processed two-per-iteration after an odd-count peel). For each record whose
 * flag (unk0/unkC) bit3 is set and whose target table slot (arg0->unk3C indexed
 * by flag>>4) is live (low-3==1), matches the module arg4 (slot>>4) and is
 * loaded (bit3), apply the patch (*(arg1 + rec->unk4) = rec->unk8) and clear the
 * flag. (func_80001994 is the loop-body alt-entry via undefined_syms.) */
/* 2026-10-20 (agent-g run 112): 65.5 -> 89.2. The old body was a hand
 * transcription of IDO's own odd-peel + x2 unroll; this is the plain loop
 * (with `continue` for both skips). Left: the store block sits after the
 * skip (target `bnezl found` + duplicated first load), the ~8 mask is a
 * hoisted t5 constant even in the peel (ours uses at), and the found/one/
 * i register picks. */
typedef struct { s16 info; s16 pad2; s32 a; s32 b; } RelSym18F0;
typedef struct { s32 info; s32 off; s32 val; } RelEnt18F0;
s32 func_800018F0(void *arg0, s32 arg1, void *arg2, s32 arg3, s32 arg4) {
    RelSym18F0 *syms = *(RelSym18F0 **)((char *)arg0 + 0x3C);
    RelEnt18F0 *rel = arg2;
    RelSym18F0 *s;
    s32 i;
    s32 found;

    if (arg1 == 0 || rel == 0) {
        return 0;
    }
    for (i = 0; i < arg3; i++) {
        if (!(rel[i].info & 8)) {
            continue;
        }
        {
            found = 0;
            s = &syms[rel[i].info >> 4];
            if ((s->info & 7) == 1 && (s->info >> 4) == arg4 && (s->info & 8)) {
                found = 1;
            }
            if (found == 0) {
                continue;
            }
            *(s32 *)(arg1 + rel[i].off) = rel[i].val;
            rel[i].info &= ~8;
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/kernel", func_800018F0);
#endif

#ifdef NON_MATCHING
/* func_80001ADC: USO/overlay relocation applier (NM decode, 2026-10-17 agent-g).
 * rel[n] records of 12 bytes: info (bits 4.. = symbol index into the 12-byte
 * symtab at obj->0x3C, bit 3 = already applied, bits 0..2 = type), the target
 * offset from base, and a slot that receives the original word. A symbol whose
 * flags lack bit 3 (unresolved) is skipped. Types: R_26 (jal target field),
 * HI16 (remember the half and its address), LO16 (fold hi<<16 + lo + value,
 * patch the remembered HI16 with the carry-adjusted upper half), R_32 (add).
 * Bad type: D_80012C44(D_8000A36C, type) and return -2. Case numbers from the
 * jumptable (2026-10-20, agent-g run 112): jtbl_8000A63C IS in the ROM, 0x400
 * lower like every kernel address (our 0xA23C): type 1 = R_26, 2 = LO16,
 * 3 = HI16, 4 = R_32. Open: the target dispatches through that 4-entry jumptable;
 * IDO 7.1/5.3 at -O1/-O2/-O3 (+-g3) lower a 4-label switch to a compare chain
 * (5 labels are needed for a table; an extra label always widens the table:
 * case 0 / 5 / 7 / 8 / -1 tested), so the switch shape is still unknown; hiptr/hival
 * are uninitialised locals homed at 0x34/0x28 in the target.
 * 2026-10-27 agent-g run 186 (52.46 -> ~81.8): a `case 5:` label on the
 * default arm (same behaviour: type 5 already errored) makes cfe's cluster
 * 5 labels long, so the 1..5 table + type-1 index appear (sltiu 5 vs the
 * target's 4); indexed rel[i] with every info read through memory (no
 * local) and the declaration order above give the 0x50 frame with hival /
 * hiptr at 0x28 / 0x34. Then (lever-sweep de-naming, 81.88 -> 93.21) the
 * symbol pointer is not a local: both reads index symtab afresh, which gives
 * the target's base / symtab s3 / s2 and the per-arm `lw a0` delay fills.
 * The frame: cfe gives unreferenced locals a home only by declaration
 * position -- `info` / `sym` declared (unused) before hiptr restore the
 * target's 0x50 frame with hiptr / hival at 0x34 / 0x28. Left: sltiu 5,
 * val / p loads scheduled after the bound test, temp register picks. */
extern char D_8000A36C[];
typedef struct { s32 info; s32 offset; s32 orig; } UsoRel_1ADC;
typedef struct { s16 flags; s16 pad; s32 value; s32 pad2; } UsoUsoRelSym_1ADC_1ADC;

#define SYM_1ADC (&symtab[rel[i].info >> 4]) /* run 186: de-named (lever-sweep 81.81 -> 93.13) */
s32 func_80001ADC(char *obj, char *base, UsoRel_1ADC *rel, s32 n) {
    UsoUsoRelSym_1ADC_1ADC *symtab;
    s32 type, val, lo;
    s32 info; UsoUsoRelSym_1ADC_1ADC *sym; /* run 186: unreferenced; their cfe homes give the target's 0x50 frame */
    s16 *hiptr;
    s32 v; /* run 186: declared after hiptr (5040-order grid): hiptr home 0x34 as in the target */
    s32 i;
    s32 hival;
    u32 *p;

    symtab = *(UsoUsoRelSym_1ADC_1ADC **)(obj + 0x3C);
    if (base == 0 || rel == 0) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (rel[i].info & 8) {
            continue;
        }
        type = rel[i].info & 7;
        if (SYM_1ADC->flags & 8) {} else { continue; }
        val = SYM_1ADC->value;
        p = (u32 *)(base + rel[i].offset);
        rel[i].orig = *p;
        switch (type) {
        case 1:
            *p = (*p & 0xFC000000) | ((((u32)val >> 2) & 0x3FFFFFF) + (*p & 0x3FFFFFF));
            break;
        case 3:
            hiptr = (s16 *)p + 1;
            hival = ((s16 *)p)[1]; if (1) {} /* run 186 (lever-sweep bbsplit) */
            break;
        case 2:
            lo = ((s16 *)p)[1];
            if (hiptr != 0) {
                v = (hival << 16) + (s16)lo + val; if (1) {} /* run 186 (lever-sweep bbsplit) */
                *hiptr = (u32)(v - (s16)v) >> 16; if (1) {}
                hiptr = 0;
            }
            ((s16 *)p)[1] = (hival << 16) + lo + val;
            break;
        case 4:
            *p += val;
            break;
        case 5:
        default:
            D_80012C44(D_8000A36C, (void *)type);
            return -2;
        }
        rel[i].info |= 8;
    }
    return 0;
}
#undef SYM_1ADC
#else
INCLUDE_ASM("asm/nonmatchings/kernel", func_80001ADC);
#endif

/* func_80001CF4 - verified structural decode (kernel, 56 insns incl.
 * a leaked predecessor word).
 * BOUNDARY ARTIFACT: the .s text begins at 0x80001CF0 (addiu sp,sp,
 * 0x50) but the symbol func_80001CF4 = 0x80001CF4. That leading
 * +0x50 is the PREDECESSOR's epilogue delay-slot (its jr ra +
 * addiu sp,sp,0x50), disassembled one word early by splat. The
 * real function starts at 0x80001CF4 = `addiu sp,sp,-0x20`. Harmless
 * for the INCLUDE_ASM build (bytes are bytes), but a future C match
 * must target the -0x20 prologue and the predecessor must own the
 * +0x50 word (split-fragments / undefined_syms boundary fix needed
 * before this can be byte-matched).
 * Struct-typing reference: s = a 3-slot resource/handle holder.
 * Parallel field groups, slot i in {0,1,2}: handle s->{0x54,0x50,
 * 0x4C} (84/80/76), paramA s->{0x48,0x44,0x40} (72/68/64), paramB
 * s->{0x10,0xC,0x8} (16/12/8), paramC s->{0x1C,0x18,0x14} (28/24/20).
 * Per non-null slot: func_80001EDC(handle, paramA, paramB) (init/
 * validate; nonzero = failure -> abort with -0xE/-0xF/-0x10 for
 * slots 0/1/2), then func_80000518(handle, paramC) (commit/apply).
 * Caps <80: leaked predecessor prologue word (boundary) + beql
 * branch-likely chain + 2 callees. INCLUDE_ASM remains build path
 * (no episode; tautology-trap rule + unresolved head boundary). */
#ifndef FW
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
#endif
/* func_80001CF4: MATCHED 2026-06-21 (dead-#else promotion). Three-stage
 * sub-object finalize/validate: for each non-NULL sub-object (unk54/unk50/
 * unk4C) run func_80001EDC(self, sub, paramA, paramB) + the func_80000518(
 * sub, paramC) cleanup (commit runs in the beql delay slot, BEFORE the
 * failure test); bail with -0xE/-0xF/-0x10 if the stage fails. The matching
 * body sat in a dead #else/#ifdef branch (never compiled) while the build
 * path was INCLUDE_ASM. ROM byte-identical to baserom. */
s32 func_80001CF4(void *arg0) {
    extern s32 func_80001EDC();
    s32 temp_s1;

    if (FW(arg0, 0x54) != 0) {
        temp_s1 = func_80001EDC(arg0, FW(arg0, 0x54), FW(arg0, 0x48), FW(arg0, 0x10));
        func_80000518(FW(arg0, 0x54), FW(arg0, 0x1C));
        if (temp_s1 != 0) {
            return -0xE;
        }
    }
    if (FW(arg0, 0x50) != 0) {
        temp_s1 = func_80001EDC(arg0, FW(arg0, 0x50), FW(arg0, 0x44), FW(arg0, 0xC));
        func_80000518(FW(arg0, 0x50), FW(arg0, 0x18));
        if (temp_s1 != 0) {
            return -0xF;
        }
    }
    if (FW(arg0, 0x4C) != 0) {
        temp_s1 = func_80001EDC(arg0, FW(arg0, 0x4C), FW(arg0, 0x40), FW(arg0, 0x8));
        func_80000518(FW(arg0, 0x4C), FW(arg0, 0x14));
        if (temp_s1 != 0) {
            return -0x10;
        }
    }
    return 0;
}

/* func_80001DD0: MATCHED 2026-06-23 (sibling of func_80001CF4). Three-stage
 * sub-object finalize/validate with an extra threaded arg. For each non-NULL
 * handle slot (0x54/0x50/0x4C) it calls the 5-arg helper func_80001CF0(self,
 * handle, paramA, paramB, arg1) (self=a0 in s0; arg1 saved at sp+0x2c and
 * passed via the sp+0x10 stack-arg slot), then runs func_80000518(handle,
 * paramC) ALWAYS (commit emitted in the `beql s1,0,next` delay slot, BEFORE
 * the failure test), then bails -0xE/-0xF/-0x10 on failure. The 5-arg form
 * (self as the leading arg, like func_80001CF4 passes arg0 to func_80001EDC)
 * is correct with this beql-commit-before-check shape — the old "5-arg
 * regresses" note was measured against the wrong (return-before-commit)
 * structure. ROM byte-identical to baserom. */
s32 func_80001DD0(void *arg0, s32 arg1) {
    extern s32 func_80001CF0();
    s32 temp_s1;

    if (FW(arg0, 0x54) != 0) {
        temp_s1 = func_80001CF0(arg0, FW(arg0, 0x54), FW(arg0, 0x48), FW(arg0, 0x10), arg1);
        func_80000518(FW(arg0, 0x54), FW(arg0, 0x1C));
        if (temp_s1 != 0) {
            return -0xE;
        }
    }
    if (FW(arg0, 0x50) != 0) {
        temp_s1 = func_80001CF0(arg0, FW(arg0, 0x50), FW(arg0, 0x44), FW(arg0, 0xC), arg1);
        func_80000518(FW(arg0, 0x50), FW(arg0, 0x18));
        if (temp_s1 != 0) {
            return -0xF;
        }
    }
    if (FW(arg0, 0x4C) != 0) {
        temp_s1 = func_80001CF0(arg0, FW(arg0, 0x4C), FW(arg0, 0x40), FW(arg0, 0x8), arg1);
        func_80000518(FW(arg0, 0x4C), FW(arg0, 0x14));
        if (temp_s1 != 0) {
            return -0x10;
        }
    }
    return 0;
}

/* func_80001EC8 -- EXACT (agent-g run 145) via the IDO 7.1 cfe donor
 * src/kernel/kernel_000_1EC8.c; this definition is only the splice
 * placeholder. USO section-record loader: read 12-byte records until type 11,
 * allocate (flag bit 0: alloc98, else lookup34) or clear (type 8) each
 * section, run the reloc callbacks, stash the buffer in mod[type + 14], add the
 * size to the 0x80012D30 table and dispatch on the type through the 12-entry
 * jump table the ROM already carries at jtbl_8000A64C (runtime addresses,
 * +0x400). The old "runtime jump table" blocker is solved like
 * func_80005C50's: the donor's .rodata reloc is renamed func_80001EC8_rodata
 * and pinned to 0x8000A64C. */
s32 func_80001EC8(s32* arg0, void* file) {
    return 0;
}


/* func_800021A4: 43-insn entry-list walker (absorbed func_800021D0 fragment
 * via merge-fragments). objdiff 94.53% (2026-06-23, agent-e) — up from the
 * old ~85% ceiling after a structural rewrite: insn COUNT now exact (43),
 * the comparison constants 2/4/3 are correctly hoisted before the loop, the
 * case-2/case-4 (beq/beql) dispatch and store-block (addu a0+(type>>4)*4;
 * lw 0x38; add value; sh|8; sw) match shape exactly, and entry/exit/loop
 * back-edge are byte-identical.
 *
 * RESIDUAL = 2 mnemonic diffs + register-coloring (genuine cap class):
 *   1. case-3 (a3==3) scheduling: target emits `beq a3,t2,.L2230` with the
 *      `ori` speculatively in the delay slot and a separate `sh t8` block
 *      falling into the shared `slt`; my emit inverts to `bne` and inlines
 *      `sh` in the b-delay (slt vs sh swap). IDO branch-sense + delay-slot
 *      scheduling artifact — no C lever flips it without duplicating the
 *      loop tail (tried: !=3/goto-skip splits → +slt/+addiu dup, regresses).
 *   2. t-register renumber: constants land in t1/t2/t3 not t0/t1/t2 because
 *      the store-block value-load (lw 0x4(a2)) grabs t0; cascades into the
 *      whole +0x44..+0x68 store block (t9/a3/t0/t4 vs t3/t4/t7/t6) and the
 *      post-shift branch offsets. Live-range coloring, decl-order-invariant
 *      (verified). Permuter would be the tool but import is blocked here by
 *      the asm-processor two-phase build (cc1 chokes on INCLUDE_ASM; see
 *      docs/MATCHING_WORKFLOW.md "PERMUTER=1 DOESN'T work").
 * Default build is INCLUDE_ASM (byte-exact via raw bytes). */
#ifdef NON_MATCHING
/* 2026-10-20 (agent-g run 112): 94.5 -> 94.9 rewritten as the natural
 * symbol-resolve loop (2/4: add the segment base, 3: mark). The compare
 * chain order 2,4,3 rules out a switch (cfe sorts case values); goto-style
 * ifs give it. Left: i/p/count land in a1/v1/a2 instead of v1/a2/a1 (decl
 * order inert, 120 perms), and the t==3 test inverts (permuter 8 min: 96.3
 * only with a junk `s++; s--;`). */
int func_800021A4(int *a0) {
    RelSym18F0 *syms;
    RelSym18F0 *s;
    s32 i;
    s32 info;
    s32 t;

    syms = *(RelSym18F0 **)((char *)a0 + 0x3C);
    if (syms == 0) {
        return 0;
    }
    s = syms;
    for (i = 0; i < a0[1]; i++, s++) {
        info = s->info;
        if (info & 8) {
            continue;
        }
        t = info & 7;
        if (2 == t) goto add;
        if (t == 4) goto add;
        if (t == 3) goto mark;
        continue;
    add:
        s->a += a0[(info >> 4) + 14];
        s->info = info | 8;
        continue;
    mark:
        s->info = info | 8;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/kernel", func_800021A4);
#endif

/* func_800021D0 is now an alabel inside func_800021A4 (merged via splat
 * fragment-merge — the original split-at-0x800021D0 was a splat boundary
 * artifact since 0x800021D0 has no prologue and uses registers set up
 * by the 0x800021A4 prologue). */

/* 2026-10-23 EXACT (agent-g run 144): the "arg0 vs counter saved-register
 * contest" above was a decode shape (sibling of func_8000235C):
 * - the error callback takes THREE arguments (msg, u, l): passing the counter
 *   as a fourth made it live into the call block and gave IT the saved
 *   register; with three, o takes s0 and the counter stays in a3;
 * - u->list cached in a local `l` (one load, reused for the link);
 * - one index local `k` for BOTH the module-table index (type >> 4) and the
 *   entry's link index: that web takes a0 and pushes the count into a1;
 * - plain `i < o->count` loop test: cfe's PRE puts the count reload only into
 *   the two store arms (the miss-arm delay slot and before the latch). */
extern s32 D_8000A3A0;
typedef struct { short type; short pad; int val; short idx; short pad2; } Ent80002250;
typedef struct {
    char p0[4];
    int count;
    char p8[0x34];
    Ent80002250 *list;
    char p40[0x59];
    unsigned char dirty;
} Obj80002250;
typedef struct { char p0[0x18]; int miss; char p1c[0x84 - 0x1C]; void (*field_84)(); } Cb80002250;
int func_80002250(Obj80002250 *o) {
    Ent80002250 *e;
    Obj80002250 *u;
    Ent80002250 *l;
    int k;
    int i;

    o->dirty = 0;
    if (o->list == 0) {
        return 0;
    }
    i = 0;
    e = o->list;
    for (; i < o->count; i++) {
        if (!(e->type & 8) && (e->type & 7) == 1) {
            k = e->type >> 4;
            u = ((Obj80002250 **)D_80012D60)[k];
            if (u == 0) {
                o->dirty = 1;
                ((Cb80002250 *)&D_80012BC0)->miss++;
            } else {
                l = u->list;
                if (l == 0) {
                    ((Cb80002250 *)&D_80012BC0)->field_84(&D_8000A3A0, u, l);
                    return -2;
                }
                e->type |= 8;
                k = e->idx;
                e->val = l[k].val;
            }
        }
        e++;
    }
    return 0;
}

/* func_8000235C: 33-insn entry-list clear-by-type helper.
 *   if no entry list at a0+0x3C, return 0.
 *   For each 0xC-byte entry, if low type bits == 1, high bits match arg1,
 *   and bit 3 is set, clear bit 3 and mark a0+0x99 dirty.
 *
 * C structure is correct, but natural IDO -O2 rotates the saved original arg,
 * loop counter, entry pointer, and loaded type word through v0/v1/a2/a3
 * instead of the target's a3/v1/a2/v0 layout. The emitted body is same-size;
 * Makefile INSN_PATCH previously closed the register-allocation diff. INSN_PATCH
 * REMOVED 2026-05-23 (see lower note).
 *
 * 2026-05-17: tested `register int saved_a1 = a1;` + renamed locals (p, v,
 * i) — fuzzy unchanged at 91.06%. IDO's register-priority formula is driven
 * by ref-count + loop-depth, not local-name or `register` hint for this case.
 * Permuter-class register-allocation cap.
 *
 * 2026-05-20: direct-pointer/count-reload variants also emitted the same
 * rotated-register body. The INSN_PATCH promotion was REMOVED 2026-05-23
 * as match-faking; the function is now at honest 91.06% NM (permuter-class
 * register-allocation cap).
 *
 * 2026-06-20 (agent-b): statement-reorder narrowed the coloring. Loading the
 * list (`v0 = a0[0x3C/4]`) BEFORE saving the arg, and initializing the entry
 * pointer (`a2 = ...`) BEFORE the counter (`v1 = 0`), pins list->$v0, entry
 * ptr->$a2 and the loop type-word->$v0 EXACTLY as target (was list->$v1,
 * ptr->$a3 fully-rotated before). RESIDUAL is now a single 2-register swap:
 * saved-arg lands in $v1 / counter in $a3 (target: arg->$a3, counter->$v1).
 * This last swap is coupled to the as1 scheduler: emitting the counter-init
 * before the ptr-init (to flip the coloring) reorders the prologue insns and
 * sends the entry ptr to $a3 instead. The two can't be satisfied together via
 * statement order (scheduler-tie + coloring). Genuine ugen coloring cap. */
/* 2026-10-23 EXACT (agent-g run 144): the "ugen coloring cap" above was a
 * decode shape. Fresh decode: a for loop over a cached count `n` that is
 * RE-READ inside the hit arm after the dirty-flag store (that store-then-load
 * order is source order), the entry pointer stepped at the end of the body,
 * and the three inits in the order i, n, e after the null test (a 72-variant
 * decl x init-order sweep: only init order i, n, e gives the target's
 * i = v1 / n = a1 / e = a2 / id = a3 colouring; declaration order is free). */
typedef struct { short type; short pad; int a; int b; } Ent8000235C;
typedef struct {
    char p0[4];
    int count;
    char p8[0x34];
    Ent8000235C *list;
    char p40[0x59];
    unsigned char dirty;
} Obj8000235C;
int func_8000235C(Obj8000235C *o, int id) {
    Ent8000235C *e;
    int i;
    int n;

    if (o->list == 0) {
        return 0;
    }
    i = 0;
    n = o->count;
    e = o->list;
    for (; i < n; i++) {
        if ((e->type & 7) == 1 && (e->type >> 4) == id && (e->type & 8)) {
            e->type &= ~8;
            o->dirty = 1;
            n = o->count;
        }
        e++;
    }
    return 0;
}

/* func_800023E0 = Yay0 decoder + osSetIntMask (two handwritten routines in one splat symbol), real source in src/handwritten/kernel/decode_yay0_setintmask.s (docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources). */
GLOBAL_ASM("src/handwritten/kernel/decode_yay0_setintmask.s")

/* func_80002530 = libultra osInitialize (KMC kernel variant). EXACT via
 * IDO 5.3 -O1 donor splice (REPLACE_FUNC_BODY, see
 * src/kernel/kernel_000_ido53_2530.c — proven 284/284 standalone; this unit
 * is 7.1 -O2 which caps at ~53%, so the body below compiles at the wrong
 * opt and the donor bytes replace it in the .o). Key decode facts: enables
 * FPU (SR |= CU1), sets FpcCsr, polls PIF status 0x1FC007FC via
 * __osSiRawReadIo/__osSiRawWriteIo (func_80004EC0/func_8000A040), installs
 * the 16-byte exception preamble D_80003270 at the four vectors
 * 0x80000000/80/100/180 + writeback/inval caches, osMapTLBRdb
 * (func_80009DF0), reads the clock via osPiRawReadIo(4,..)
 * (func_80002DB0); D_8000A3D0 = osClockRate is a u64 (OSTime):
 * `osClockRate = osClockRate * 3 / 4` calls the 64-bit helpers
 * func_80002F78 (__ll_mul) / func_80002E78 (__ll_div); D_8000A3D8 =
 * osViClock by D_80000300 = osTvType; then the KMC debugger hook: if
 * *(u32*)0xBFF00000 == 'KMC\0', copy the 9-word stub at D_80003280 to the
 * general-exception vector, and if the KMC flags word lacks bit 4, copy the
 * 0x800-word KMC ROM image high and tail-call its entry(+8). */
typedef struct {
    u32 inst1, inst2, inst3, inst4;
} __osExceptionVector;
extern __osExceptionVector D_80003270;          /* __osExceptionPreamble */
extern u64 D_8000A3D0;                          /* osClockRate (OSTime) */
extern s32 D_8000A3D8;                          /* osViClock */
extern s32 D_8000030C;                          /* osResetType */
extern s32 D_80000300;                          /* osTvType */
extern s32 D_8000031C;                          /* osAppNMIBuffer */
extern s32 __kmc_pt_mode;
extern s32 D_80003280[];                        /* KMC E_VEC stub words */

void func_80002530(void) {
    extern s32 func_800031E0(void);             /* __osGetSR */
    extern void func_800031D0(s32);             /* __osSetSR */
    extern void func_80009C40(s32);             /* __osSetFpcCsr */
    extern s32 func_80004EC0(void *, u32 *);    /* __osSiRawReadIo */
    extern s32 func_8000A040(void *, u32);      /* __osSiRawWriteIo */
    extern void func_800031F0(void *, s32);     /* osWritebackDCache */
    /* func_80005350 (osInvalICache) already declared (s32,s32) at file top */
    extern void func_80009DF0(void);            /* osMapTLBRdb */
    extern void func_80002DB0(s32, u32 *);      /* osPiRawReadIo */
    extern void func_800030D0(void *, s32);     /* bzero */
    u32 pifdata;
    u32 clock = 0;
    void (*entry)();
    s32 pad50;
    s32 pad4C;
    s32 *src;
    s32 *dst;
    s32 sp40;
    void *sp3C;
    s32 *sp38;
    s32 *sp34;
    s32 *sp30;
    s32 sp2C;

    func_800031D0(func_800031E0() | 0x20000000);
    func_80009C40(0x01000800);
    while (func_80004EC0((void *)0x1FC007FC, &pifdata)) {
        ;
    }
    while (func_8000A040((void *)0x1FC007FC, pifdata | 8)) {
        ;
    }
    *(__osExceptionVector *)0x80000000 = D_80003270;
    *(__osExceptionVector *)0x80000080 = D_80003270;
    *(__osExceptionVector *)0x80000100 = D_80003270;
    *(__osExceptionVector *)0x80000180 = D_80003270;
    func_800031F0((void *)0x80000000, 0x190);
    func_80005350(0x80000000, 0x190);
    func_80009DF0();
    func_80002DB0(4, &clock);
    clock &= ~0xF;
    if (clock != 0) {
        D_8000A3D0 = clock;
    }
    D_8000A3D0 = D_8000A3D0 * 3 / 4;
    if (D_8000030C == 0) {
        func_800030D0(&D_8000031C, 0x40);
    }
    if (D_80000300 == 0) {
        D_8000A3D8 = 0x02F5B2D2;
    } else if (D_80000300 == 2) {
        D_8000A3D8 = 0x02E6025C;
    } else {
        D_8000A3D8 = 0x02E6D354;
    }
    if (__kmc_pt_mode == 0) {
        sp38 = (s32 *)0xBFF08004;
        sp3C = (void *)0xBFF00000;
        if (*(s32 *)0xBFF00000 == 0x4B4D4300) {
            src = D_80003280;
            dst = (s32 *)0x80000180;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            src += 2;
            dst += 2;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            func_800031F0((void *)0x80000180, 0x24);
            func_80005350(0x80000180, 0x24);
            __kmc_pt_mode = 1;
            if (!(*sp38 & 0x10)) {
                sp40 = *(s32 *)((char *) sp3C + 4);
                if (sp40 != 0xBFF00000) {
                    sp30 = (s32 *)(sp40 | 0x20000000);
                    sp2C = 0x800;
                    sp34 = (s32 *)0xBFF00000;
                    do {
                        *sp30 = *sp34;
                        sp30 += 1; sp34 += 1;
                        sp2C -= 1;
                    } while (sp2C != 0);
                }
                entry = (void (*)())(sp40 + 8);
                entry(0x4B4D4300, 0);
            }
        }
    }
}






void func_800029A0(void) {
}
#pragma GLOBAL_ASM("asm/nonmatchings/kernel/func_800029A0_pad.s")
