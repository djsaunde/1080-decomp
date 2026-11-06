/* IDO 7.1 -O2 donor for gl_func_00051AD8 -- draw-state command decoder
 * (0x3C0 / 240 words; 240/240 word-exact standalone apart from reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 29CCC / 52CD4 recipe, docs/MATCHING_WORKFLOW.md#donor-splice-switch-
 * jumptable-rodata-rename). The C switch over op 200..211 puts its 12-entry
 * jumptable at donor .rodata +0; the splice renames that reloc to
 * gl_func_00051AD8_rodata, pinned to 0x1BB4 in undefined_syms_auto.txt, so the
 * `lw t6,0x1BB4(at)` table load links exactly as the USO ships it.
 *
 * EXACT 2026-11-06 (agent-c run 326): the last word was the bound check's
 * delay slot (ours stole the default head `lui a0,2`, the target keeps the
 * fall-through `sll t6,t6,2`). A folded counted use of pp in case 211
 * (`out->w54 = src->w54 + 0 * (int)(pp)`, pp set earlier in that case; zero
 * code) found by scripts/zero-use-sweep.py flips as1's choice. Everything else
 * is agent-g's run-112 decode (see the post0b placeholder comment):
 *  - the default arg op = s->op assigned INSIDE the call;
 *  - ONE pointer var q is both the cursor and the read value; p is the cursor
 *    of 204/208/206/205; the 202/211 index is a separate v;
 *  - 208 copies a Pair from r + 1; 0x58 is a 1-word struct; the high byte of
 *    0x30 is a u8[1] array; the unused x and the decl order set the 72-byte
 *    frame and the 0x1C / 0x28 / 0x2C slots.
 */
#include "common.h"

extern int gl_func_00000000();
extern int D_00000000;

typedef struct { s32 w[4]; } Quad_51AD8;
typedef struct { s32 a, b; } Pair_51AD8;

typedef struct {
    Quad_51AD8 q;      /* 0x00 */
    f32 f10;           /* 0x10 */
    s32 w14;           /* 0x14 */
    s32 w18;           /* 0x18 */
    s32 w1C;           /* 0x1C */
    s16 h20;           /* 0x20 */
    s16 h22;           /* 0x22 */
    s32 w24;           /* 0x24 */
    s32 w28;           /* 0x28 */
    s16 h2C;           /* 0x2C */
    s16 h2E;           /* 0x2E */
    s16 h30;           /* 0x30 */
    u8 b32;            /* 0x32 */
    Pair_51AD8 m;      /* 0x34 */
    s32 w3C;           /* 0x3C */
    s32 w40;           /* 0x40 */
    s32 w44;           /* 0x44 */
    s32 *w48;          /* 0x48 */
    s32 w4C;           /* 0x4C */
    s32 w50;           /* 0x50 */
    s32 w54;           /* 0x54 */
    struct { s32 v; } w58;           /* 0x58 */
} State_51AD8;

extern State_51AD8 *gl_ref_00000148_51AD8[];
extern State_51AD8 *gl_ref_00000160_51AD8[];

typedef struct {
    s32 op;
    s32 *cur;
} Strm_51AD8;

void gl_func_00051AD8(State_51AD8 *out, Strm_51AD8 *s) {
    s32 *r;
    s32 op;
    State_51AD8 **pp;
    s32 *x;
    State_51AD8 *src;
    s32 v;
    u8 *b;
    u8 hi[1];
    s32 *p;
    s32 *q;

    switch (s->op) {
    case 200:
        q = s->cur; s->cur = q + 1;
        break;
    case 209:
        q = s->cur; s->cur = q + 1; out->w44 = *q;
        break;
    case 204:
        p = s->cur; s->cur = p + 1; q = (s32 *)*p;
        out->m.a = 0;
        out->m.b = (s32)q;
        break;
    case 210:
        q = s->cur; s->cur = q + 1; b = (u8 *)*q;
        out->w58.v = *(s32 *)b;
        out->b32 = b[0];
        hi[0] = gl_func_00000000(b[1]);
        out->h30 = (hi[0] << 8) | gl_func_00000000(b[2]);
        break;
    case 208:
        p = s->cur; s->cur = p + 1; q = (s32 *)*p;
        r = (s32 *)gl_func_00000000(&D_00000000, q);
        out->w48 = r;
        out->m = *(Pair_51AD8 *)(r + 1);
        break;
    case 207:
        q = s->cur; s->cur = q + 1; out->m.a = *q;
        break;
    case 206:
        p = s->cur; s->cur = p + 1; q = (s32 *)*p;
        out->m.b &= ~(s32)q;
        break;
    case 205:
        p = s->cur; s->cur = p + 1; q = (s32 *)*p;
        out->m.b |= (s32)q;
        break;
    case 202:
        q = s->cur; s->cur = q + 1; v = *q;
        pp = &gl_ref_00000160_51AD8[v];
        if (*pp != 0) {
            s->cur += 4;
            out->q = (*pp)->q;
        } else {
            gl_func_00000000(s, out);
        }
        break;
    case 201:
        gl_func_00000000(s, out);
        break;
    case 203:
        q = s->cur; s->cur = q + 1; out->w24 = *q;
        q = s->cur; s->cur = q + 1; out->w1C = *q;
        q = s->cur; s->cur = q + 1; out->h20 = *q;
        q = s->cur; s->cur = q + 1; out->h22 = *q;
        q = s->cur; s->cur = q + 1; out->w28 = (s16)*q;
        q = s->cur; s->cur = q + 1; out->h2C = *q;
        q = s->cur; s->cur = q + 1; out->h2E = *q;
        break;
    case 211:
        q = s->cur; s->cur = q + 1; v = *q;
        pp = &gl_ref_00000148_51AD8[v];
        if (*pp == 0) {
            gl_func_00000000((char *)&D_00000000 + 0x20F14);
        }
        src = *pp;
        out->q = src->q;
        out->f10 = src->f10;
        out->w14 = src->w14;
        out->w18 = src->w18;
        out->w1C = src->w1C;
        out->h20 = src->h20;
        out->h22 = src->h22;
        out->w24 = src->w24;
        out->w28 = src->w28;
        out->h2C = src->h2C;
        out->h2E = src->h2E;
        out->h30 = src->h30;
        out->b32 = src->b32;
        out->m = src->m;
        out->w3C = src->w3C;
        out->w40 = src->w40;
        out->w44 = src->w44;
        out->w48 = src->w48;
        out->w4C = src->w4C;
        out->w50 = src->w50;
        out->w54 = src->w54 + 0 * (int)(pp); /* folded counted use of pp (set above in this case; zero code) */
        out->w58 = src->w58;
        break;
    default:
        gl_func_00000000((char *)&D_00000000 + 0x20F20, op = s->op);
        break;
    }
}
