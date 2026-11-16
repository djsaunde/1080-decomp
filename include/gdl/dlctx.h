#ifndef GDL_DLCTX_H
#define GDL_DLCTX_H

/* libgdl display-list context (0x7C bytes, built by gfx_newDLContext at
 * game_libs 0x56580 in src/game_libs/game_libs_post0b.c).
 *
 * It holds six growable command/data buffers. gfx_printDLUsage prints them
 * in this order with
 *   "%s : Gfx %d/%d, Vtx %d/%d, Light %d/%d, Mtx %d/%d, Vp %d/%d, Hilite %d/%d"
 * (count/max of each buffer's current chunk, then the context name at +0x70).
 * gfx_newDLContext allocates max+1 elements of 8 (Gfx), 0x10 (Vtx),
 * 0x48 (light), 0x40 (Mtx) and 0x10 (Vp) bytes; Hilite is created empty.
 *
 * Every buffer is reached through its `cur` pointer (the ctor points it at
 * the buffer itself), so an emitter appends a Gfx command as
 *   rec = ctx->gfx.cur; idx = rec->count; rec->count = idx + 1;
 *   arr = (int *)(ctx->gfx.cur->base + idx * 8);  arr[0] = w0; arr[1] = w1;
 * gfx_endDL closes the list (G_RDPPIPESYNC, G_ENDDL) and calls the overflow
 * report for every buffer whose count went past max.
 *
 * base is kept as an integer address: the matched emitters do the slot
 * arithmetic in int, and pointer arithmetic changes IDO's temp order. */
typedef struct DLBuf {
    /* 0x0 */ s32 base;            /* element array */
    /* 0x4 */ s32 count;           /* elements used */
    /* 0x8 */ s32 max;             /* capacity given to gfx_newDLContext */
    /* 0xC */ struct DLBuf *cur;   /* current chunk (initially this buffer) */
} DLBuf;

typedef struct DLContext {
    /* 0x00 */ DLBuf gfx;          /* Gfx commands, 8 bytes each */
    /* 0x10 */ DLBuf vtx;          /* Vtx, 0x10 */
    /* 0x20 */ DLBuf light;        /* 0x48-byte light blocks */
    /* 0x30 */ DLBuf mtx;          /* Mtx, 0x40 */
    /* 0x40 */ DLBuf vp;           /* Vp, 0x10 */
    /* 0x50 */ DLBuf hilite;       /* Hilite (no storage allocated) */
    /* 0x60 */ s32 frameStart;     /* gfx cursor when gfx_resetDLContext ran */
    /* 0x64 */ s32 frameStartCount;/* gfx count at that point */
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ char *name;         /* printed by gfx_printDLUsage / overflow */
    /* 0x74 */ s32 unk74;
    /* 0x78 */ u32 flags;          /* bit 0 cleared by gfx_resetDLContext */
} DLContext;

typedef char DLBuf_size_check[sizeof(DLBuf) == 0x10 ? 1 : -1];
typedef char DLContext_size_check[sizeof(DLContext) == 0x7C ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char DLContext_name_check[(int)&((DLContext *)0)->name == 0x70 ? 1 : -1];
typedef char DLContext_flags_check[(int)&((DLContext *)0)->flags == 0x78 ? 1 : -1];
#endif

/* field access through untyped `char *` cursors (the hand-inlined gSP/gDP appends:
 * `buf = DLCTX(ctx)->gfx.cur; n = DLBUF(buf)->count; DLBUF(buf)->count = n + 1;
 * pkt = (char *)DLCTX(ctx)->gfx.cur->base + n * 8`) */
#define DLCTX(p) ((DLContext *)(p))
/* one 8-byte Gfx command at an untyped packet pointer: DLPKT(pkt)->w0 = cmd; ->w1 = arg */
typedef struct DLGfx {
    /* 0x0 */ s32 w0;
    /* 0x4 */ s32 w1;
} DLGfx;
#define DLPKT(p) ((DLGfx *)(p))
#define DLBUF(p) ((DLBuf *)(p))

/* GdlRenderState: the render-state record the gfx_emit* state emitters take as `int *` (agent-b run 583).
 * Only its flags word is read: gfx_emitRenderState_00047644 emits the cycle type always, the combine
 * mode when flags & 0xF00, the geometry mode when flags >> 12 and the render mode when flags & 0xFF;
 * gfx_renderModeWord / gfx_renderMode2Word map bits 18 / 19 / 23 and the low byte to an other-mode
 * word; bits 19 / 23 select two-cycle (gfx_emitCycleType, gfx_emitCombineMode). gfx_initDdm builds
 * one in place after a DL count (flags = its argument). Word 0 is not read by these emitters. */
typedef struct GdlRenderState {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 flags;
} GdlRenderState;

#endif /* GDL_DLCTX_H */
