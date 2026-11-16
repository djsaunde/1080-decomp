#include "common.h"
#include "uso_call.h"
#include "gdl/dlctx.h" /* DLContext / DLBuf: the gui's current display-list context */
#include "gdl/race.h"  /* RaceSprite: the gui sprite (handle +0x10, file id +0x14) */

#define GUISPRITE(p) ((RaceSprite *)(p))

extern char D_00000000;

/* GfxCtx ring: a0->0xC points to {Gfx *buf @0; int idx @4}. Used by the
 * gui_uso texture-load DL-builder family (00003F18/0000413C/00004354). Each
 * packet reloads a0->0xC twice (once for idx read/bump, once for buf) — the
 * store to ->idx forces IDO to reload the ctx pointer for the base. */
struct GfxRing_413C { int *buf; int idx; };

/* SOURCE=4 audit 2026-06-01: this cluster is not a missed accessor-template
 * candidate. The 0x0..0x148 char mapper is an over-split early-return chain;
 * the fragments look like leaf-branch-past-end/caller-set-register caps only
 * because predecessor tests branch into later entries while carrying $v0/$a0.
 * find-misplit-pairs.py does not flag it because each split entry has its own
 * jr ra. Proper recovery is a deliberate full-cluster merge, not per-fragment
 * C bodies. */


/* gui_fontCreate: single coherent function, 0x148-0x55C (0x414 / 261
 * insns), terminated by `jr ra; addiu sp,sp,0x80`. The earlier "BUNDLED
 * 0x7D0/500-insn / 3 sub-fns + orphan" note was STALE: expected/gui_uso.c.o
 * shows the next symbol gui_setupRenderMode_0000055C starting at 0x55C, and the
 * function has zero inter-function relocs in range. The two `jal 0` are
 * baked-zero placeholders -> gl_func_00000000 (find-or-create allocator).
 *
 * GLYPH-GRID-LAYOUT constructor. Args
 *   (char *obj, int a1, int a2, int a3, int rows, int cols)   [rows=sp+0x90,
 *   cols=sp+0x94]. Find-or-create obj (alloc 40 if null). Fields:
 *     o->4  = a1 (pixel/atlas base ptr)
 *     o->C  = a2/rows (gw, glyph cell width) -- later overwritten by maxw
 *     o->0  = rows*cols (cell count)
 *     o->10 = o->14 = a3/cols (gh, glyph cell height)
 *     o->18 = a2,  o->1C = a3
 *     o->20 = gl_func_00000000(rows*cols*20)  (per-cell record array, 20B ea)
 *   Then per cell (col in [0,cols), row in [0,rows), gi = col*rows+row):
 *   scans the gh x gw pixel block at base + col*gh*a2 + row*gw, finding
 *   min/max non-zero column (minx/maxx). Writes record g = o->0x20 + gi*20:
 *     g[0]=row*gw+minx, g[1]=col*gh, g[2]=(maxx+1)-minx, g[4]=minx.
 *   Tracks maxw = max(maxx+1), sum = sum(maxx+1). Finally o->C = maxw,
 *   o->8 = (int)(sum*0.5f) / o->0, and back-fills g[3]=o->C for every cell.
 *
 * STATUS: 99.98% fuzzy (was 99.92), 255/261 raw target words. Still NM.
 * 2026-08-22 fixes: (1) maxx+=1
 * in place, reused for maxw/g[2]/sum (target addiu a1,a1,1 in place); (2)
 * per-store reload of *(o+0x20) -- no cached g pointer (aliasing store
 * between field writes forces the 4 reloads); (3) tail is INTEGER divide
 * (int)(sum*0.5f) / o->0 (break 0x7/0x6 checks), not float divide; (4)
 * idx=gi row-copy for the record address web. 2026-09-09: initialize gi
 * INSIDE the rows>0 guard, with a guarded do/while row loop. This restores
 * the target's per-column multu and per-row argument-home reload; merely
 * moving gi's declaration never fixed the strength reduction. Initialize
 * maxx before minx to match the scan setup order. Follow-up: reuse a0 as
 * the allocated object and row in the final fill loop, inline rows*cols,
 * and store both gh fields before the original atlas dimensions. Keep
 * the pixel scan on one line for the unrolled slt/addiu schedule.
 * Follow-up: hoist the working locals and order col/gw/gh declarations to
 * recover the target spill homes (sp+0x78, +0x58, +0x54). Stage the scan-line
 * byte offset in x before resetting it for the scan; this fixes the final
 * addu operand order without changing the scan's instruction schedule.
 * Remaining: 136-byte frame vs 128 and the four incoming-argument reloads
 * shifted by that frame delta (six raw words total). Dropping the line
 * local shrinks the frame but shifts the other spill homes; parameter
 * reuse, address escapes and volatile bounds regress. Register hints inert.
 * Diagnostic follow-up: omitting line and reordering col/gw/gh reaches
 * frame 128 with all named homes exact, but seven anonymous-spill offsets
 * four bytes low (254/261 words). Also reusing a1 for maxw reaches 257/261:
 * frame and all stack slots exact, incoming a1 saved in s6 instead of s5.
 * Its four register-word differences score below this version; keep this C.
 * Both the 128- and 136-byte layouts have numlr=37/finalnumlr=45 (eight
 * splits) in a completed uopt trace: split count alone does not explain
 * the frame delta. See docs/IDO_CODEGEN.md#glyph-grid-frame-trace-gui148.
 * 2026-09-12: recover the 40-byte grid and 20-byte cell types below, with
 * pointer-valued arguments and sizeof-based allocations. Both build modes
 * retain their prior instruction bytes; this is not a new exact match.
 * The smaller-frame variant's incoming pixel pointer and max-width value
 * share one P+4 live range (s6), while the separate sum local gets s5.
 * See docs/IDO_CODEGEN.md#glyph-grid-types-and-parameter-web-gui148.
 * See docs/IDO_CODEGEN.md#glyph-grid-pointer-reuse-gui148. Still NON_MATCHING. */
/* EXACT 2026-10-22 (agent-g run 125) via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_0148.cpp (REPLACE_FUNC_BODY, both trees): the cfe
 * frame residue above was the declaration set (12 NCC scalars, sum in the
 * dead a1 parameter, dead `if (maxw) {}`). This is the splice host. */
void gui_fontCreate(void) {
}

/* gui_setupRenderMode_0000055C: display-list emitter (sibling of gui_beginFrame_000016A4),
 * six fixed GBI commands into the context cached at a0->0x24. EXACT
 * 2026-10-22 (agent-g run 126) via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_055C.cpp (REPLACE_FUNC_BODY, both trees). The
 * old "slot rotation" residual came from where uopt's basic blocks
 * ended. This is the splice host. */
void gui_setupRenderMode_0000055C(void) {
}

/* gui_setPrimColor_000006B8: one G_SETPRIMCOLOR append (col[0..2] * 255 above
 * the alpha byte) into a0->0x24's gfx context. EXACT 2026-10-22 (agent-g run
 * 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_06B8.cpp
 * (REPLACE_FUNC_BODY, both trees; the 1A5C recipe). This is the splice host. */
void gui_setPrimColor_000006B8(void) {
}

/* gui_setPrimColorClamped_000008C0 [0x8C0..0xB58): GUI primitive-color DL builder --
 * FULL-CLUSTER MERGE 2026-06-09 of the former 22-insn fall-through head +
 * gui_func_00000918 (144-insn body; splat split at a mid-function
 * scheduling artifact). Merge-safety proven via the gui.uso module tables:
 * 0x8C0 appears as a referenced entry, 0x918 appears NOWHERE (not an
 * independent runtime entry; the old "implicit $v0-input convention" was
 * just the fall-through from this head). Structure: emits
 * G_SETOTHERMODE(0xB900031D, 0x00404240-ish) + G_SETCOMBINE(0xFC309661...)
 * pairs into the DL ring at ctx->dl (ctx = a0->0x24, cursor at
 * ctx->0xC->4, 8-byte slots), then two G_SETPRIMCOLOR-class commands whose
 * color word is packed from three floats (R/G/B x 1.0f scale) via the
 * cvt.w.s + FCSR-overflow-clamp idiom (cfc1/ctc1, andi 0x78), each channel
 * clamped to 0..0xFF and OR'd into byte lanes 16/8/0.
 *
 * FULL BODY 2026-07-10 (agent-g, stub-rewrite vein): the float->u8 clamp
 * ladder IS just `(u32)(chan * 255.0f)` — IDO's float->UNSIGNED cast
 * expansion (2^31 bias 0x4F000000 + FCSR invalid-op retry) emits the exact
 * cfc1/ctc1/cvt.w.s/andi 0x78 sequence at -mips2 (no per-file flag needed;
 * the -mips1 note in MATCHING_WORKFLOW is for the SIGNED (int) cast). The
 * three DL appends re-deref a0->0x24 (ctx) and ctx->0xC (ring) each time.
 * All 166 insns + the whole clamp ladder now match; residual is a
 * register-coloring cap (target colors the ring ptr $v1, IDO gives $a1) +
 * FP-const/stack-slot scheduling — objdiff fuzzy 9%->~85% (unit 76.2->79.3).
 * INCLUDE_ASM stays the build path (not byte-exact). */
/* EXACT 2026-10-22 (agent-g run 126) via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_08C0.cpp (REPLACE_FUNC_BODY, both trees). The
 * old "ring pointer colour" residual came from where uopt's 20-variable-load
 * rule ended the basic block (the 17DC recipe moves it). This is the splice
 * host. */
void gui_setPrimColorClamped_000008C0(void) {
}

extern int gl_func_00000000();
/* gui_drawString: glyph-string draw (0x1AC). For each char of the string
 * (strlen re-called every iteration -> `sltu` vs the unsigned count, a3 homed
 * and reloaded for it while p walks): a space advances x by o->8; any other
 * char looks up its glyph index, reads the 20-byte glyph record at
 * o->0x20 + idx*stride (stride a 0x14 LOCAL -> the loop-held s8 + multu, not
 * shifts), issues the texture-rect setup and draw calls, then advances x by
 * the record's +8 width re-indexed with the u8-narrowed idx (andi 0xff).
 * Load-bearing: the non-space arm written FIRST (target beql to the space
 * arm); g0/g1/w read into locals before the call (t0,t1,s0 load order);
 * `p++, i++` in the for-increment; (h << 10) / h written INLINE as the last
 * call arg -- IDO still evaluates it at the top of the arm, but a named
 * local for it swaps the s4/s5 colouring with the u8 idx. */
extern int gl_func_00000000();
void gui_drawString(int *a0, int a1, int a2, unsigned char *a3) {
    int h = a0[0x14 / 4];
    unsigned char *p = a3;
    unsigned int i;
    int stride;
    int idx;
    unsigned char gi;
    int *g;
    int g0, g1, w;

    stride = 0x14;
    for (i = 0; i < (unsigned int)CALL(str_len)(a3); p++, i++) {
        if (*p != ' ') {
            idx = gl_func_00000000(*p);
            gi = idx;
            g = (int *)(a0[0x20 / 4] + idx * stride);
            g0 = g[0]; g1 = g[1]; w = g[2];
            CALL(gui_loadTileIA8)(a0[0x24 / 4], a0[4 / 4], a0[0x18 / 4], a0[0x1C / 4], g0, g1, w, h, 0);
            CALL(gui_texRect_00004568)(a0[0x24 / 4], a1, a2, w, h, (w << 10) / w, (h << 10) / h);
            /* RAW: data-table: font glyph table (a0[8], stride bytes per glyph, advance at +8) */
            a1 += *(int *)(a0[0x20 / 4] + gi * stride + 8);
        } else {
            a1 += a0[8 / 4];
        }
    }
}

/* gui_drawString_00000D04: 0x200 (128 insns). Text-rendering function that builds
 * an RDP command list. Partial structural decode 2026-05-03:
 *
 * PROLOGUE (insns 1-15):
 *   addiu sp,-0x58              ; large frame
 *   sw ra/s0..s8 to sp+0x30..0x54  ; saves all 9 callee-saved (heavy state)
 *   sw a2,0x60(sp); sw a3,0x64(sp) ; spill last 2 args
 *   v0 = a0->0x24               ; gp/cmd-list-state ptr
 *   s4 = a0->0x14               ; some scaler
 *   t2 = 0xBB000001             ; built via lui+ori - RDP opcode
 *                                 (G_SETCOMBINE or similar 1-byte cmd)
 *   t3 = 0x80008000             ; coord constant (signed-min XY)
 *   s1 = a0; s3 = a1; s7 = a3; a0 = a3
 *
 * BODY HEAD (insns ~15-25):
 *   v1 = v0->0xC                ; cmd-list pointer (head)
 *   t0 = v1->4                  ; current count
 *   v1->4 = t0 + 1              ; bump count
 *   slot = v0->0xC->[0] + t0*8  ; slot[0..7] = next 8-byte cmd
 *   slot[0] = 0xBB000001        ; emit RDP cmd
 *   slot[4] = 0x80008000        ; emit cmd-args
 *   sw a3,0x64(sp)              ; re-spill a3
 *   jal gl_func(a3)             ; string length helper
 *   move s8,zero                ; total = 0 (delay slot)
 *   if (v0 == 0) goto end       ; empty string, branch to epilogue
 *
 * BODY MIDDLE (insns ~25-100): per-character loop
 *   c = *s7                     ; lbu char
 *   if (c == ' ') skip          ; space-handling branch
 *   width_scaled = s4 << 10     ; scaled glyph width?
 *   div(width_scaled, s4)       ; division - possibly per-glyph stride
 *   bnez s4 (guard against /0)
 *   ... (further glyph-table lookups, FPU coord math, more RDP cmd emits)
 *
 * EPILOGUE: restore all 9 saved regs, addiu sp, jr ra.
 *
 * Speculation: this is the entry point for "draw text string with kerning"
 * - emits a SETCOMBINE-class RDP cmd, then loops over chars accumulating
 * positions and submitting per-glyph display list. ~80 insns of body math
 * remain TBD; multi-tick decomp expected. */
extern int gl_func_00000000();
extern char D_00000000;
/* Full decode (2026-05-24): kerned text-string renderer. Emits a SETCOMBINE-class
 * RDP cmd (0xBB000001/0x80008000), then for each of strlen(a3) chars: space ->
 * advance x by the space glyph's width; else look up glyph = a0->0x20[idx*0x14]
 * (idx = gl_func(c)), make two per-glyph render calls, advance x by glyph width.
 * The two `1024` packet metrics are REDUNDANT signed divisions in the target:
 * (scale<<10)/scale and (glyph[2]<<10)/glyph[2] (each ==1024 but IDO emits the
 * div + the div-by-0 / -1<<31-overflow break-traps).
 *
 * Decode 2026-06-23: 71 -> 95.74% (128/128 insns, all-correct C) via:
 *  (1) loop bound is the per-char callback's RETURN re-evaluated each iter, NOT a
 *      hoisted strlen: peeled do/while `if(cond){do{...}while(cond);}` with
 *      `(unsigned)i < (unsigned)gl_func(a3)` (unsigned -> sltu/beqzl like target);
 *  (2) the SECOND glyph lookup re-derives its index from `(unsigned char)g`
 *      (target's `andi s5,v0,0xff`) - first lookup uses raw g, second the byte;
 *  (3) compute the masked glyph OFFSET *after* the first render call so the masked
 *      byte stays live in a saved reg across the call (target s5) - this single
 *      reorder evicts the loop-invariant ' '(=32) from its hoisted saved reg
 *      (li s8,32), shifting i->s8 / p->s7 to match (82 -> 94%);
 *  (4) hold glyph[2] in a named local `gw` (target s0) reused by both calls + div;
 *  (5) keep only the masked OFFSET (goff) and re-derive the base `a0->0x20` at
 *      each glyph2 field access (target re-reads s1->0x20), not a cached pointer.
 * 2026-10-15 (agent-g) 95.74 -> EXACT, closing every listed residual:
 *  (6) test the raw `*p != ' '` and pass `*p` to the lookup (no `c` local): the
 *      char loads straight into a0 and the test becomes `beql a0,at` (the old
 *      `lbu v0` + `move a0,v0` delay copy = the tested value had been named
 *      first; IDO_CODEGEN#extra-call-arg-and-test-member-before-naming-3d8a8);
 *  (7) glyph[0]/[1] read into named locals next to gw: both load before the arg
 *      setup and store together, like the target;
 *  (8) the RDP-const pair on ONE line (`slot[0]=..; slot[1]=..;`) fixes the
 *      prologue ori/lw order; `(i = 0)` inside the first loop test moves the
 *      `move s8,zero` after the a3 arg copy;
 *  (9) glyph2 x arg as `px = x; px += glyph16;` gives `addu a1,s3,t9` (x first);
 *      `x + glyph16` in either operand order emits `addu a1,t9,s3`. */
void gui_drawString_00000D04(int *a0, int *a1, int a2, char *a3) {
    int *cmd_state = (int *)a0[0x24 / 4];
    int *cmd_list = (int *)cmd_state[0xC / 4];
    int scale = a0[0x14 / 4];
    int idx = cmd_list[1];
    int *slot;
    int i;
    int x = (int)a1;
    char *p = a3;
    cmd_list[1] = idx + 1;
    slot = (int *)((char *)((int *)cmd_state[0xC / 4])[0] + idx * 8);
    slot[0] = 0xBB000001; slot[1] = 0x80008000;
    if ((unsigned int)(i = 0) < (unsigned int)CALL(str_len)(a3)) {
        do {
            if (*p != ' ') {
                int g = gl_func_00000000(*p);
                unsigned int gi = (unsigned char)g;
                int sd = (scale * 1024) / scale;
                int *glyph = (int *)((char *)a0[0x20 / 4] + g * 0x14);
                int goff;
                int px;
                int gu = glyph[0];
                int gv = glyph[1];
                int gw = glyph[2];
                CALL(gui_loadTileIA8)(a0[0x24 / 4], a0[0x4 / 4], a0[0x18 / 4], a0[0x1C / 4],
                                 gu, gv, gw, scale, 0);
                goff = gi * 0x14;
                px = x;
                /* RAW: data-table: font glyph table (a0[8], 20-byte glyph records) */
                px += *(int *)((char *)a0[0x20 / 4] + goff + 16);
                CALL(gui_texRect_00004568)(a0[0x24 / 4], px,
                                 a2, gw, scale, (gw * 1024) / gw, sd);
                x += *(int *)((char *)a0[0x20 / 4] + goff + 12); /* RAW: data-table (as above) */
            } else {
                x += ((int *)a0[0x20 / 4])[3];
            }
            p++;
            i++;
        } while ((unsigned int)i < (unsigned int)CALL(str_len)(a3));
    }
}

/* gui_drawStringScaled_00000F04: 313-insn / 0x4E4 F3DEX2 glyph-STRING renderer.
 * 2026-06-05 FULL DECODE (was INCLUDE_ASM-only + prose). Disassembled the
 * raw .word USO body to mnemonics, fed m2c, then translated to compilable C.
 *
 * Iterates the string arg5 char-by-char (skip 0x20 space), and for each glyph
 * appends F3DEX2 commands to the DL buffer at ctx->0x24 (->0xC = {base=0x0,
 * count=0x4}; each cmd = 8 bytes at base + count*8, count post-incremented):
 *   - one G_RDPHALF-ish header pair (0xBB000001 / 0x80008000) before the loop
 *   - per glyph: a callee (gl_func placeholder) draws the glyph quad, then
 *     TEXRECT (0xE4......), G_RDPHALF_1 (0xB4......) with scaled S/T deltas,
 *     and TEXRECT_FLIP/half (0xB3......) with the DSDX/DTDY frac (sp7C).
 *   - var_f22 (pen X advance) accumulates each glyph's width (glyph->0xC),
 *     scaled by arg3; arg4 scales the Y/row, spD4 = -ctx->0x10/2 centers it.
 *   glyph table: ctx->0x20 base, entry = base + glyphIdx*0x14
 *   (glyphIdx = gl_func(ch)); entry->0x0/0x4 = tex coords, ->0x8 = width.
 *
 * NON_MATCHING: first-pass structural decode (313 FPU insns, multi-run).
 * All callees are the gl_func_00000000 cross-USO placeholder (raw objdump
 * lost the real reloc symbols + the exact arg signature / f12-f14 float-arg
 * order), and the s0-s7/f20-f30 regalloc + glyph-struct typing are not yet
 * pinned, so this is reference C, not a byte match.
 *
 * 2026-06-05 RESIDUAL (full target<->build disasm diff): LOGIC confirmed
 * correct (per-char loop, 4 DL appends, FP scale+clamp chains, both var_f22
 * arms line up). Gap is FRAME size + cascading regalloc renumber.
 *   - FIXED this pass: the m2c artifact `if (f10 & 0x20000000)` was really
 *     `if (s4 < 0)` (target gate is `bgez s4`); `f10=(int)f2` was held across
 *     the big glyph-draw call ONLY for that test, so correcting the gate let
 *     `f10` be inlined into `s4` → dropped one cross-call spill, frame
 *     0x130->0x128, 323->318 insns.
 *   - REMAINING: frame 0x128 vs target 0x108 (4 more spill words). Target
 *     fits its 9 cross-call-live values in $s0-$s8 (s2=ctx, s6=ctx->0x14,
 *     s7=cursor, s8=idx, s4=(int)f2*4, s3=(short)s4, s1=clamp, s5=f10b,
 *     s0=glyph-width) + spills only sp7C/spD3; build's expr structure makes
 *     IDO spill 4 extra. Cascading: build ctx=$s3 vs target $s2.
 *   - FIXED 2026-06-05 (pass 2): first call is gl_func(arg0,arg5) — target
 *     leaves a0=ctx from entry and loads a1=arg5 in the delay slot; m2c had
 *     gl_func(arg5,a3) (wrong). 523->519 diffs.
 *   - FIXED 2026-06-05 (pass 3): named-local `float two=2.0f` divisor forces
 *     `div.s` (not `mul *0.5`) and the target shares that one 2.0 across both
 *     divisions AND the `-2.0` sub — div.s now 5/5, 519->503 diffs, 316 insns.
 *     (IDO_CODEGEN#feedback-ido-div-2-mul-fold-and-mtc1-load-delay-nops.)
 *     COST: frame 0x128->0x130 — `two` is declared/used with the gl_func call
 *     INSIDE the divide expr, so 2.0 is live ACROSS that call (target
 *     materializes 2.0 AFTER the call, in $f0, dead before the loop). Net diffs
 *     still improved. Next: get `two`/2.0 to materialize post-call (splitting
 *     the call to an explicit `rw` temp REGRESSED earlier — needs care).
 *   - 2026-06-05 (pass 4): moved idx/s7 init AFTER the first call to match
 *     target order (523->501 diffs cumulative). Tried `rw` temp to push 2.0
 *     materialization post-call: REGRESSED to 519 — don't retry.
 *   - REGALLOC CAP (the last frame word): build caches arg5 in an in-frame
 *     slot (sp+0x118, written twice) instead of reloading from the stack-arg
 *     HOME slot each time like the target (frame 0x130 vs 0x108 = this 1 slot
 *     + the s2/s3 ctx renumber it cascades). IDO chooses to cache the
 *     stack-passed arg5 rather than re-`lw` from its home; no C lever found to
 *     force home-reload. Structure + FP (div.s 5/5) + arg/init order all now
 *     match; the residual is this IDO spill-vs-reload choice (permuter-class).
 *   - 2026-06-22 (agent-b): 69.97% -> 75.45% fuzzy via three correct-C levers:
 *     (1) CURSOR SPILL ELIMINATED: the two TEXRECT clamps used `wa3=0; if(..)`
 *         init-then-override; switching to symmetric `if(..)x;else x=0;` (like
 *         the target's other clamps) freed a saved reg so the cursor lives in
 *         $s7 instead of spilling to sp+0x118 — frame dropped one extra slot
 *         and insn count is now exact (313). (2) CURSOR/IDX REGISTER MATCH:
 *         swapping the `s7=arg5;`/`idx=0;` decl-init order flipped IDO to
 *         cursor=$s7, idx=$s8 (was idx=$s7, cursor=$s8) matching the target.
 *         (3) DL-WRITE RE-READ: the header append's base pointer is re-loaded
 *         from `a3->0xC` (write `**(char***)(a3+0xC)`), not the cached `dl`;
 *         this matched the target's exact pre-loop DL-build schedule (insns
 *         0-57 now identical bar reg-renumber). NB: applying the same re-read
 *         to the 3 LOOP appends REGRESSED (the loop already caches dl for
 *         cnt+write, so re-reading double-loads) — header only.
 *   - REMAINING (permuter/coloring-class, no C lever found): frame still 0x130
 *     vs 0x108 (+40 = IDO places the 3 spills sp7C/spD3/spD4 high w/ small top
 *     reservation; target places them low w/ a 0x34 top hole — same spill COUNT,
 *     different layout); one duplicated `multu` in the cmd3[1] f10b clamp (IDO
 *     tail-duplicates it into both inner branches, target keeps it single — a
 *     short-cast temp did NOT defeat it); `beql` vs `beq` on the space-skip
 *     test; and the resulting recompute(sll/sra)-vs-reload(lw/move) opcode
 *     skew. Opcode multiset differs by these few; logic is byte-correct.
 *   - 2026-10-23 agent-g (run 139): 82.17 -> ~84.7 fuzzy, 309 -> 310 words. The
 *     frame is DECLARED-LOCAL COUNT: cfe reserved one word per declared local
 *     (register-allocated or not), so dropping 8 words of locals (cnt2-4 /
 *     cmd2-4 reuse cnt / cmd, one t / v pair for the nested blocks, ch / f2 /
 *     gidx inlined) gives frame 0x108 = target, and declaring 12 words of
 *     locals before spD4 puts spD4 / spD3 at 212 / 211 (target). xa3 / xa0b are
 *     int (no re-sign-extension), and the three loop appends re-read the write
 *     base from a3->0xC (with cnt / cmd shared this no longer double-loads).
 *     LEFT: the first packet colours a3 v1 / dl a3 (target a3 / v0), sp7C
 *     spills at 120 (target 124), and the two `(t < 0) ? t : 0` arms (target
 *     `b join; move v0,a0; move a0,v0`) -- writing them as ternaries is 309 /
 *     81.8.
 *     Then the B4 (RDPHALF_1) word as the stock gbi.h gSPScisTextureRectangle
 *     expression (as 2DE0 / 329C: nested (s16) ternaries with MAX / MIN, dsdx
 *     re-evaluated inside, also in the B3 word): 84.7 -> 86.2, 310 -> 315 words
 *     (of 313; the B4 arms now have the target's `b join` shapes). The cfe ?:
 *     temps each reserve a frame word, so a2v / a1v / a3v / v were dropped
 *     (frame 0x118, target 0x108). The macro form of the TEXRECT words lowers
 *     fuzzy (85.3 / 85.4): kept as the int clamps. Four parameter empty tests from
 *     a fuzzy-scored PARAMS_ONLY psweep (after the sp7C store, the a3 reload, the
 *     ca1 store and the B4 word): 86.2 -> 91.2.
 *   - 2026-10-24 agent-g (run 165): superseded for the NM object by the C++ donor
 *     src/gui_uso/gui_uso_ncc_0F04_nm.cpp (inline Ctx::next() appends, 97.15).
 *   - 2026-11-05 agent-g (run 294): EXACT via the IDO 7.1 NCC donor
 *     src/gui_uso/gui_uso_ncc_0F04.cpp (REPLACE_FUNC_BODY, both trees). This
 *     empty function is the splice host; the cfe reconstruction below is kept
 *     for reference only (not compiled). */
void gui_drawStringScaled_00000F04(void) {
}


/* Text-width accumulator with glyph-table lookup.
 *   a0 = font-info struct: +0x08 = space-width, +0x20 = glyph-table ptr
 *   a1 = string
 * For each char up to gl_func_00000000(a1) length:
 *   if ' ': total += a0[2] (space-width at +0x8)
 *   else:   key = gl_func_00000000(c);
 *           total += *(int*)(glyph_table + key*20 + 8)  (stride 20). */
int gui_stringWidth(char *a0, unsigned char *a1) {
    int total = 0;
    unsigned char *p = a1;
    unsigned int i = 0;
    unsigned char c;
    int key;

    if (CALL(str_len)(a1) != 0) {
        do {
            c = *p;
            if (c != 0x20) {
                key = gl_func_00000000(c);
                /* RAW: data-table: font glyph table (a0 + 0x20, 20-byte glyph records, advance at +8); a0 + 0x8 is the untyped font's spacing */
                total += *(int*)(*(char**)(a0 + 0x20) + key * 20 + 8);
            } else {
                total += *(int*)(a0 + 0x8); /* RAW: data-table (as above) */
            }
            p++;
            i++;
        } while (i < CALL(str_len)(a1));
    }
    return total;
}

extern int gui_func_00000000();

int gui_centerStringX(int a0, int a1, int a2) {
    return a1 - (XCALL(gui_func_00000000, gui_stringWidth)(a0, a2) / 2);
}

extern int gl_func_00000000();

int gui_rightAlignStringX(int a0, int a1, int a2) {
    return a1 - CALL(gui_stringWidth)(a0, a2);
}

extern int gl_func_00000000();
int gui_stringWidth_00001514(int *a0, unsigned char *a1) {
    int total = 0;
    unsigned char *p = a1;
    unsigned int i = 0;

    if (CALL(str_len)(a1) != 0) {
        do {
            if (*p != 0x20) {
                total += a0[3];
            } else {
                total += a0[2];
            }
            p++;
            i++;
        } while (i < (unsigned int)CALL(str_len)(a1));
    }
    return total;
}

extern int gui_func_00000000();

int gui_centerStringX_000015BC(int a0, int a1, int a2) {
    return a1 - (XCALL(gui_func_00000000, gui_stringWidth_00001514)(a0, a2) / 2);
}

int gui_rightAlignStringX_000015F4(int a0, int a1, int a2) {
    return a1 - CALL(gui_stringWidth_00001514)(a0, a2);
}

/* gui_fontAdjustAdvance: shift-all-table-entries-by-delta (advance/kerning
 * adjust): for each of *a0 entries (0x14 stride, base at a0->0x20), add the
 * delta to entry->0xC; then a0->0xC += delta. Split off from the original
 * 4.1 KB splat bundle in a prior tick (the dispatch body lives in the
 * following symbols). MATCHED 2026-06-09 via three levers: (1) declare a
 * third parameter and `a2 = a1` -- IDO homes the copy in $a2 exactly like
 * the target's `or a2,a1` (the fn is 2-arg at call sites; K&R-compatible);
 * (2) reuse a1 as the byte-offset IV, i declared first (wins $v0);
 * (3) write the update as bump-then-RMW (`v1 = v1+0xC; *v1 = *v1 + a2;`)
 * -- the load becomes an expression temp ($t8 pool, not a named-local $a3)
 * and IDO schedules it BEFORE the addiu with the 0xC offset folded. */
void gui_fontAdjustAdvance(int *a0, int a1, int a2) {
    int i;
    a2 = a1;
    a1 = 0;
    i = 0;
    if (*a0 > 0) {
        do {
            int *v1 = (int *)((char *)a0[0x20 / 4] + a1);
            v1 = (int *)((char *)v1 + 0xC);
            a1 += 0x14;
            i++;
            *v1 = *v1 + a2;
        } while (i < *a0);
    }
    a0[0xC / 4] = a0[0xC / 4] + a2;
}

/* 0x1670 is a single alignment nop between gui_fontAdjustAdvance and the real
 * function at 0x1674 — splat mis-placed the gui_uso_func_00001670 symbol on the
 * pad (and over-split the loop tail into 0000168C). The nop is emitted via
 * all-zero SUFFIX_BYTES_FORCE on gui_fontAdjustAdvance in the Makefile — a 1-word
 * GLOBAL_ASM pad block emits 2 words (+4 segment length drift, the documented
 * trap). The true function is gui_floorPow2 below. */

/* gui_floorPow2: highest-set-bit / mask-down scan. Returns the largest
 * power-of-two <= a0 (walking 0x8000 down), or 1 if a0 < 1. Merged from the
 * mis-split 00001670 body + 0000168C loop tail. */
int gui_floorPow2(int a0) {
    int v1 = 0x8000;
    do {
        if (!(a0 < v1)) {
            return v1;
        }
        v1 >>= 1;
    } while (v1 != 0);
    return 1;
}

/* gui_beginFrame_000016A4: display-list emitter, four fixed GBI commands into
 * the gfx builder (*(D+0x254))->0x158 cached in the module global
 * D_gui16a4_cache (pinned = 0). EXACT 2026-10-22 (agent-g run 126) via the
 * IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_16A4.cpp (REPLACE_FUNC_BODY,
 * both trees; the 055C recipe). This is the splice host. */
void gui_beginFrame_000016A4(void) {
}

/* MATCHED 2026-05-31 via decomp-permuter (the prior "permanent NM constant-
 * materialization scheduling-order cap" was WRONG — it was cracked, not by
 * manual C swaps, but by the permuter: putting the two arr[] stores on a SINGLE
 * source line shifts IDO's lui/ori scheduling to interleave t1's ori before
 * t0's, matching the target. Byte-exact. Lesson: 2-diff load/schedule-ORDER
 * residuals are permuter-crackable even when manual statement-reorder fails. */
void gui_setCombine_00001794(int a0) {
    int *rec = *(int **)&D_00000000;
    int *sub = (int *)rec[3];
    int count = sub[1];
    int *arr;
    sub[1] = count + 1;
    arr = (int *)(((int *)rec[3])[0] + count * 8);
    arr[0] = 0xFCFFFFFF; arr[1] = 0xFFFCF279;
}

/* gui_setColorEnvPrim_000017DC: DL colour setup (SETCOMBINE FC309A61/FF37FFFF,
 * SETENVCOLOR with the a1 alpha byte, SETPRIMCOLOR from col * 255 + alpha).
 * EXACT 2026-10-22 (agent-g run 126) via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_17DC.cpp (REPLACE_FUNC_BODY, both trees). The old
 * "third slot pointer colour" residual came from where uopt's 20-variable-load
 * block limit ended the block. This is the splice host. */
void gui_setColorEnvPrim_000017DC(void) {
}

/* gui_setColor_00001A5C: DL colour emitter (SETCOMBINE FC309661/FF2FFFFF, then G_SETPRIMCOLOR
 * with col[0..2] * 255 above the alpha byte). EXACT 2026-10-22 (agent-g run
 * 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_1A5C.cpp
 * (REPLACE_FUNC_BODY, both trees): an inline next() member on the gfx
 * context, and a redundant & 0xFF under the red channel's << 24 that burns
 * the t-ring slot the old cfe body was missing. This is the splice host. */
void gui_setColor_00001A5C(void) {
}

/* gui_setFillColor_00001CA8: DL colour emitter (SETCOMBINE FCFFFFFF/FFFDF6FB, then G_SETPRIMCOLOR
 * with col[0..2] * 255 above the alpha byte). EXACT 2026-10-22 (agent-g run
 * 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_1CA8.cpp
 * (REPLACE_FUNC_BODY, both trees): an inline next() member on the gfx
 * context, and a redundant & 0xFF under the red channel's << 24 that burns
 * the t-ring slot the old cfe body was missing. This is the splice host. */
void gui_setFillColor_00001CA8(void) {
}

/* gui_setEnvPrimColor_00001EF4: DL colour setup (SETCOMBINE, SETENVCOLOR from env * 255
 * + alpha, SETPRIMCOLOR from prim * 255 + 0xFF). EXACT 2026-10-22 (agent-g
 * run 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_1EF4.cpp
 * (REPLACE_FUNC_BODY, both trees; the 1A5C recipe). This is the splice host. */
void gui_setEnvPrimColor_00001EF4(void) {
}

/* gui_setColorEnvPrim_00002334: 17DC with the combiner FC11EA23/FFD7FFFF. EXACT
 * 2026-10-22 (agent-g run 126) via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_2334.cpp (REPLACE_FUNC_BODY, both trees; the 17DC
 * recipe). This is the splice host. */
void gui_setColorEnvPrim_00002334(void) {
}

/* SOURCE=4 decode (2026-06-01): display-list rectangle command writer.
 * Uses the global GUI context at D[0] -> ctx, appends one 2-word command at
 * ctx->dl[ctx->cursor++] and packs x/y coordinates as 12-bit fixed-point
 * values after multiplying by 4. The target stores a0 into its caller arg slot
 * and reads the fifth arg from sp+0x10; the address-taken K&R arg keeps that
 * shape in the NM body. */
void gui_setScissor(a0, x0, y0, x1, y1)
    int a0;
    int x0;
    int y0;
    int x1;
    int y1;
{
    DLContext *global = *(DLContext **)&D_00000000;
    DLBuf *ctx = global->gfx.cur;
    int cursor = ctx->count;
    int *dst;
    int sx0;
    int sy0;
    int sx1;
    int sy1;

    (void)&a0;
    ctx->count = cursor + 1;
    dst = (int *)(global->gfx.cur->base + (cursor << 3));

    sx0 = (int)((float)x0 * 4.0f) & 0xFFF;
    sy0 = (int)((float)y0 * 4.0f) & 0xFFF;
    sx1 = (int)((float)x1 * 4.0f) & 0xFFF;
    sy1 = (int)((float)y1 * 4.0f) & 0xFFF;

    dst[0] = 0xED000000 | (sx0 << 12) | sy0;
    dst[1] = (sx1 << 12) | sy1;
}

extern int gl_func_00000000();
extern char D_00000000;

void gui_spriteLoad(char *a0, int a1) {
    int saved;
    GUISPRITE(a0)->fileId = a1;
    saved = CALL(app_setHeapMode)(7);
    /* RAW: data-table: D_00000000 + 0x150 is a USO-local pool word (the BinFileMgr) */
    *(int *)&GUISPRITE(a0)->image = CALL(BinFileMgr_getAcquire)((*(int *)((char *)&D_00000000 + 0x150)), GUISPRITE(a0)->fileId);
    CALL(app_setHeapMode)(saved);
}

void gui_spriteInit(int *a0, int a1) {
    a0[5] = a1;
    a0[4] = 0;
}

extern int gui_func_00000000();
extern char D_00000000;

void gui_spriteRelease(char *a0) {
    if (GUISPRITE(a0)->image != 0) {
        /* RAW: data-table: D_00000000 + 0x150 is a USO-local pool word (the BinFileMgr) */
        XCALL(gui_func_00000000, BinFileMgr_getRelease)((*(int *)((char *)&D_00000000 + 0x150)), GUISPRITE(a0)->fileId, a0);
    }
    GUISPRITE(a0)->image = 0;
}

extern int gl_func_00000000();
void gui_spriteRefresh(char *a0) {
    int saved;
    int v0;
    int v1;
    if (GUISPRITE(a0)->fileId == -1) {
        GUISPRITE(a0)->fileId = 0;
    }
    saved = CALL(app_setHeapMode)(7);
    /* RAW: data-table: D_00000000 + 0x150 is a USO-local pool word (the BinFileMgr) */
    v0 = CALL(BinFileMgr_get_00068730)((*(int *)((char *)&D_00000000 + 0x150)), GUISPRITE(a0)->fileId);
    v1 = (int)GUISPRITE(a0)->image;
    if (v1 == 0 || v0 != v1) {
        /* RAW: data-table (as above) */
        *(int *)&GUISPRITE(a0)->image = CALL(BinFileMgr_getAcquire)((*(int *)((char *)&D_00000000 + 0x150)), GUISPRITE(a0)->fileId);
    }
    CALL(app_setHeapMode)(saved);
}

/* gui_drawSprite: tiled sprite blitter. EXACT 2026-10-22 (agent-g run 125)
 * via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_27A0.cpp (REPLACE_FUNC_BODY,
 * both trees). The old cfe reconstruction (~96.5 %) is superseded. This is the
 * splice host. */
void gui_drawSprite(void) {
}

#ifdef NON_MATCHING
/* 2026-08-22 agent-f rewrite from target disasm: 70.56 -> 93.91. Levers:
 * ctx rides the s2 reused-temp web (ctx -> info -> tex, one variable);
 * DIRECT-SYMBOL *(int **)&D_00000000 at every call site (held-pointer
 * s7 local was folding to per-use lui/lw instead of the $s7 LICM
 * promotion); tile width 32 held in VARIABLE s3 so the then-arm
 * (s3<<10)/s3 emits the real div + break7/6 guards (literal 32
 * const-folded them away, -20 insns); assignment ORDER s4=a3 before
 * s5=a1 is what maps the copies to target regs (decl order of the two
 * is a no-op; target emits or s5,a1 first — 2-insn schedule tie);
 * then-arm `int *dead = a0[4]` re-deref (target has dead or v0,v1) —
 * uopt DCEs the copy but the probe still moved loop-top load shape +3pp.
 * NEGATIVES banked: if(1){s1=s6;} pin = 90.76 (no copy, perturbs);
 * s1=s6 vs s1=a4 both coalesce the counter into one web (target keeps
 * lw s6,128 + or s1,s6 uncoalesced — the missing 2 insns).
 * RESIDUAL: +8 frame (120 vs 112); s-permutation from the coalesced
 * a4-web (counter s0 vs s1, height s2 vs s0, ctx-web s1 vs s2); a0
 * spill-store placement; prologue ori/or schedule ties. Or-copy/
 * coloring class (same as 4568). */
/* 2026-10-22 agent-g run 128 (57 -> 34 raw): field58 is the SIXTH declared
 * local (cfe frame word per declared referenced local: frame 112, field58 at
 * sp+88 as the target) and two empty if (1) {} cuts. LEFT: the target keeps
 * lw s6,128 + or s1,s6 (w promoted, rem a copy); here rem loads the param. */
void gui_drawSpriteIA8Clip_00002BB0(int *a0, int a1, int a2, int a3, int a4) {
    int s5; int s4; int s3; int s1; int s6; int field58; int s0; int s8; int *s2; int *v1;

    /* first DL emit: G_SETSCISSOR (BB000001 / 80008000); ctx rides the s2 web */
    s2 = *(int **)&D_00000000;
    {
        int *v0 = (int *)s2[0xC / 4];
        int idx = v0[1];
        v0[1] = idx + 1;
        {
            int *slot = (int *)(((int *)s2[0xC / 4])[0]) + idx * 2;
            slot[0] = 0xBB000001;
            slot[1] = (int)0x80008000;
        }
    }
    s4 = a3;
    s5 = a1;
    s2 = (int *)a0[0x10 / 4];
    s3 = 32;
    s6 = a4;
    s1 = a4; if (1) {}
    field58 = *(short *)((char *)s2 + 0x20); if (1) {}
    if (s6 == 0) return;
    s0 = *(short *)((char *)s2 + 0x22);
    s8 = (s0 << 10) / s0;
    do {
        v1 = (int *)a0[0x10 / 4];
        if (s1 >= 33) {
            s6 = (s3 << 10) / s3;
            s2 = (int *)v1[8 / 4];
            gl_func_00000000(*(int **)&D_00000000, s2, field58, s0, s4, 0, s3, s0, 0);
            gl_func_00000000(*(int **)&D_00000000, s5, a2, s3, s0, s6, s8);
            s5 += 32;
            s4 += 32;
            s1 -= 32;
        } else {
            s2 = (int *)v1[8 / 4];
            s3 = s1;
            gl_func_00000000(*(int **)&D_00000000, s2, field58, s0, s4, 0, s1, s0, 0);
            gl_func_00000000(*(int **)&D_00000000, s5, a2, s1, s0, (s1 << 10) / s1, s8);
            s1 = 0;
        }
    } while (s1 != 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/gui_uso/gui_uso", gui_drawSpriteIA8Clip_00002BB0);
#endif

/* 2026-11-11 agent-c (run 337): EXACT via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_2DE0.cpp (REPLACE_FUNC_BODY, both trees). This empty
 * function is the splice host; the cfe reconstruction below is kept for
 * reference only (not compiled). */
void gui_drawSpriteScaledF_00002DE0(void) {
}


/* 2026-11-06 agent-g (run 309): EXACT via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_329C.cpp (REPLACE_FUNC_BODY, both trees). This empty
 * function is the splice host; the cfe reconstruction below is kept for
 * reference only (not compiled). */
void gui_drawSpriteScaled_0000329C(void) {
}


/* 2026-10-23 agent-g (run 137): the NM comparison object uses an IDO 7.1 NCC donor,
 * src/gui_uso/gui_uso_ncc_3714_nm.cpp (this body recompiled under NCC: 83.0 -> 85.4).
 * 2026-11-05 agent-g (run 300): EXACT via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_3714.cpp (REPLACE_FUNC_BODY, both trees). This empty
 * function is the splice host; the cfe reconstruction below is kept for
 * reference only (not compiled). */
void gui_drawSpriteScroll_00003714(void) {
}


/* gui_fillRect: 27-insn / 0x6C RDP G_FILLRECT display-list builder.
 * 0xF6 << 24 = G_FILLRECT command word (per F3DEX2 gbi.h).
 *
 * MATCHED 2026-05-31 via decomp-permuter. The prior "2-insn order-swap CAP"
 * (target emits entry[1]'s `or` before entry[0]'s) was cracked: putting the two
 * entry[] stores on a SINGLE source line shifts IDO's `or` scheduling to match.
 * Byte-exact. (Same crack as gui_setCombine_00001794.) */
extern int gl_func_00000000();
extern char D_00000000;
void gui_fillRect(int a0, int a1, int a2, int a3, int arg5) {
    typedef struct DLState { int *base; int idx; } DLState;
    typedef struct GameState { char pad[0xC]; DLState *dlp; } GameState;
    GameState *gs = *(GameState**)&D_00000000;
    DLState *dlp = gs->dlp;
    int idx = dlp->idx;
    int *entry;
    (void)a0;
    dlp->idx = idx + 1;
    entry = gs->dlp->base + idx * 2;
    entry[0] = 0xF6000000 | ((a3 & 0x3FF) << 14) | ((arg5 & 0x3FF) << 2); entry[1] = ((a1 & 0x3FF) << 14) | ((a2 & 0x3FF) << 2);
}

/* gui_drawImage_00003B80: image blitter (27A0's sibling). EXACT 2026-10-22 (agent-g
 * run 125) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_3B80.cpp
 * (REPLACE_FUNC_BODY, both trees). The old cfe reconstruction (~96.2 %) is
 * superseded. Its three trailing zero words stay the SUFFIX_BYTES_FORCE pad.
 * This is the splice host. */
void gui_drawImage_00003B80(void) {
}

/* gui_loadTileRGBA32: RGBA32 texture-tile loader (SETTIMG, SETTILE, LOADSYNC,
 * LOADTILE, PIPESYNC, SETTILE, SETTILESIZE). EXACT 2026-10-22 (agent-g run
 * 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_3F18.cpp
 * (REPLACE_FUNC_BODY, both trees; the 413C gbi.h recipe). Its three leading
 * zero words are gui_drawImage_00003B80's pad (SUFFIX_BYTES_FORCE). The notes
 * above describe the old cfe reconstruction. This is the splice host. */
void gui_loadTileRGBA32(void) {
}

/* gui_loadTileRGBA16 — verified structural decode (no-call/no-branch RDP
 * display-list builder, 134 insns, 1% first pass; GfxCtx-idiom regalloc
 * cascade needs multi-run → INCLUDE_ASM build path; N64-forensics ref).
 * Args: f(GfxCtx **a0, a1, a2, a3, arg4@sp16, arg5@sp20, arg6@sp24,
 *         arg7@sp28, arg8@sp32). a0->0xC = ptr to {Gfx *buf @0; int idx @4}.
 * Per-packet idiom (×7): g=(ctx*)a0->0xC; i=g->idx; g->idx=i+1;
 *   slot=(int*)g->buf + i*2; slot[0]=w0; slot[1]=w1.
 * DOUBLE-RELOAD LEVER (agent-g 2026-07): the target reloads a0->0xC TWICE per
 * packet — once for ->idx read/bump, once for ->buf base — because the store
 * to ->idx forces IDO to re-fetch the (possibly-aliased) ctx pointer for the
 * base. Reproduce by dereferencing a0->0xC a SECOND time for the base AFTER
 * the ->idx store: `r=*(GfxRing**)(a0+0xC); i=r->idx; r->idx=i+1;
 *  slot=(*(GfxRing**)(a0+0xC))->buf + i*2;`. This took 0413C 51->68 fuzzy;
 * packet 1 now matches byte-for-byte. Applied identically to sibling 003F18
 * and 04354 (see struct GfxRing_413C at file top).
 * Forensics: texture-load DL fragment (SETTIMG→SETTILE→LOADSYNC→LOADTILE→
 * PIPESYNC→SETTILE→SETTILESIZE). t1 is CSE'd across packets 2&6; arg6=texW,
 * arg7=texH-ish, arg8=tile fmt/palette. Residual <80: cross-packet CSE + the
 * slot-pointer coloring (a3 vs t2) drive a regalloc/scheduling cascade no C
 * reproduces exactly. INCLUDE_ASM remains build path (NM-wrap). */
/* gui_loadTileRGBA16: RGBA16 texture-tile loader (SETTIMG, SETTILE, LOADSYNC,
 * LOADTILE, PIPESYNC, SETTILE, SETTILESIZE). EXACT 2026-10-22 (agent-g run
 * 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_413C.cpp
 * (REPLACE_FUNC_BODY, both trees): the real gbi.h macro bodies with
 * `uls + w - 1` corner arguments and the context's inline next() append; the
 * notes above describe the old cfe reconstruction. This is the splice host. */
void gui_loadTileRGBA16(void) {
}

/* gui_loadTileIA8: 133-insn (0x214) Gfx-display-list builder.
 * Leaf function (no stack frame, only caller-slot a3 spill at sp+0xC)
 * that writes ~6 RDP commands into a display list buffer at a0->[0xC].
 *
 * Args (inferred from asm structure):
 *   a0 — gui-state struct; a0->[0xC] is a Gfx-buffer descriptor:
 *        - +0: dlist pointer (Gfx **) into the actual command buffer
 *        - +4: command counter (advanced by 1 per command emitted)
 *   a1 — image segment-address (used as second word of G_SETTIMG)
 *   a2 — width minus 1 (asm pre-decrements: addiu t4, a2, -1)
 *   a3 — saved at sp+0xC at function entry, never used in body
 *   sp+0x10..0x20 — additional stack args (4..5 more ints)
 *
 * RDP command sequence (each emits a 64-bit Gfx command pair):
 *   0xFD68 hi-word — G_SETTIMG (set texture image)
 *   0xE600 hi-word — G_RDPLOADSYNC
 *   0xF400 hi-word — G_LOADTILE
 *   0x0700 hi-word — appears in tile-size cmd
 *   0xE700 hi-word — G_RDPPIPESYNC
 *   0xF200 hi-word — G_SETTILESIZE
 *   0xF568 hi-word — variant of SETTIMG (8-bit format?)
 *
 * Pattern: classic "render textured quad" setup — set texture image,
 * load the tile, set tile params, set tile size, draw a texture
 * rectangle. Common in N64 GUI/HUD code.
 *
 * Structural cap: complex F3DEX2 / RDP gfx code with heavy bit-shifting
 * (sll t9, v1, 3 = stride*8 for 64-bit Gfx) and packing
 * (multiple `andi rN, 0xFFF` for 12-bit width/height fields). Multi-
 * tick decomp; first pass documents the structure, future passes can
 * decode each command into gsDPSetTextureImage / gsDPLoadTile /
 * gsDPSetTile / gsDPSetTileSize / gsSPTextureRectangle macros (or
 * their open-coded equivalents).
 *
 * 2026-05-08: structural NM-only first pass. The 0xC-byte addressing
 * pattern (a0->[0xC] = Gfx-buffer descriptor with .dlist + .count
 * fields) recurs across gui_uso_func_*413C, *4354, *4568, *4774 —
 * worth typing as `GfxBuf { Gfx **dlist; int count; }` in a future
 * pass once decoded for one of them. */
/* gui_loadTileIA8: IA8 texture-tile loader (SETTIMG, SETTILE, LOADSYNC,
 * LOADTILE, PIPESYNC, SETTILE, SETTILESIZE). EXACT 2026-10-22 (agent-g run
 * 121) via the IDO 7.1 NCC donor src/gui_uso/gui_uso_ncc_4354.cpp
 * (REPLACE_FUNC_BODY, both trees): the real gbi.h macro bodies with
 * `uls + w - 1` corner arguments and the context's inline next() append; the
 * notes above describe the old cfe reconstruction. This is the splice host. */
void gui_loadTileIA8(void) {
}

/* 2026-11-05 agent-c (run 308): EXACT via the IDO 7.1 NCC donor
 * src/gui_uso/gui_uso_ncc_4568.cpp (REPLACE_FUNC_BODY, both trees): the real
 * gbi.h gSPTextureRectangle / gSPScisTextureRectangle macro bodies over an
 * inline Ctx::next(DL *) append. This empty function is the splice host; the
 * cfe reconstruction below is kept for reference only (not compiled). */
void gui_texRect_00004568(void) {
}

