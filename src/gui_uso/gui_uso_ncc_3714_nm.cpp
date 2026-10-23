/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_00003714: the existing cfe NM body
 * recompiled unchanged under NCC (82.96 -> 85.32, same length). NM object only; ROM build keeps INCLUDE_ASM.
 * Run 138: four parameter empty tests from a fuzzy-scored sweep -> 88.72 (247 -> 250 of 256 words). */
typedef float f32; typedef double f64; typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef signed char s8; typedef unsigned char u8;
extern "C" {
extern char D_00000000;
extern int gui_func_00000000(...);
}
extern "C" void gui_func_00003714(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6) {
    char * volatile *dp = (char * volatile *)&D_00000000;
    char *t;    /* $a1: gctx / x-clamp accumulator */
    s32 dl;     /* $v0: dlring / (s16)dsdx */
    s32 idx;    /* $v1: idx / by / (s16)dtdy */
    s32 slot1;  /* $t0: packet1 slot / (s16)(by<<2) */
    s32 slot2;  /* $t1: mode struct ptr / packet2 slot / dsdx */
    s32 slot3;  /* $t2: B4 slot */
    s32 w;      /* $a0: x test / clamp lo / mult tmp / final slot */
    s32 lrx;    /* $a2: (s16)(bx<<2) / y result */
    s32 u;      /* $a3: bx / by<<2 raw / dtdy */
    s32 sp30;
    f32 fv;     /* $f0: (f32)arg4 then (f32)sp30 */
    f32 ws;     /* $f12 */
    f32 hs;     /* $f14 */
    f32 two;    /* $f16 */

    t = *dp;
    dl = *(s32 *)(t + 0xC);
    idx = *(s32 *)(dl + 4);
    *(s32 *)(dl + 4) = idx + 1;
    slot1 = *(s32 *)(*(s32 *)(t + 0xC)) + idx * 8;
    *(s32 *)slot1 = 0xBB000001;
    *(s32 *)(slot1 + 4) = 0x80008000;

    slot2 = *(s32 *)(arg0 + 0x10);
    sp30 = *(s16 *)(slot2 + 0x22);
    gui_func_00000000(*dp, *(s32 *)(slot2 + 8), *(s16 *)(slot2 + 0x20), sp30, arg3, 0, arg4, sp30, 0); if (arg5) {}

    two = (f32)2;
    fv = (f32)arg4;
    t = *dp;
    dl = *(s32 *)(t + 0xC);
    idx = *(s32 *)(dl + 4);
    *(s32 *)(dl + 4) = idx + 1;
    slot2 = *(s32 *)(*(s32 *)(t + 0xC)) + idx * 8;

    t = 0;
    u = (s32)((f32)arg1 + (-(fv / two) * arg5));
    ws = fv * arg5;
    if (1) { lrx = u; lrx = lrx << 0x12; }
    w = (s16)((u + (s32)ws) << 2);
    if (w > 0) {
        t = (char *)w;
    }
    lrx = lrx >> 0x10;
    fv = (f32)sp30;
    idx = (s32)((f32)arg2 + (-(fv / two) * arg6));
    hs = fv * arg6;
    w = 0;
    dl = (s16)((idx + (s32)hs) << 2);
    u = idx << 2;
    if (dl > 0) {
        w = dl;
    }
    *(s32 *)slot2 = ((((s32)t & 0xFFF) << 0xC) | 0xE4000000) | (w & 0xFFF);
    t = 0;
    if (lrx > 0) {
        t = (char *)lrx;
    }
    slot1 = (s16)u;
    if (slot1 > 0) {
        w = slot1;
    } else {
        w = 0;
    }
    *(s32 *)(slot2 + 4) = (((s32)t & 0xFFF) << 0xC) | (w & 0xFFF);

    t = *dp;
    dl = *(s32 *)(t + 0xC);
    idx = *(s32 *)(dl + 4);
    *(s32 *)(dl + 4) = idx + 1;
    slot3 = *(s32 *)(*(s32 *)(t + 0xC)) + idx * 8;
    *(s32 *)slot3 = 0xB4000000;
    if (lrx < 0) {
        slot2 = (s32)(((f32)(arg4 - 1) * 1024.0f) / ws);
        dl = (s16)slot2;
        if (dl < 0) {
            w = ((s32)((u32)lrx * (u32)dl)) >> 7;
            idx = 0;
            if (w > 0) {
                t = (char *)w;
            } else {
                t = (char *)idx;
            }
        } else {
            w = ((s32)((u32)lrx * (u32)dl)) >> 7;
            dl = 0;
            if (w < 0) {
                dl = w;
            }
            w = dl;
            t = (char *)w;
        }
    } else {
        t = 0;
        slot2 = (s32)(((f32)(arg4 - 1) * 1024.0f) / ws);
    }
    if (u < 0) {
        u = (s32)(((f32)(sp30 - 1) * 1024.0f) / hs);
        idx = (s16)u;
        if (idx < 0) {
            dl = ((s32)((u32)slot1 * (u32)idx)) >> 7;
            idx = 0;
            if (dl > 0) {
                lrx = dl;
            } else {
                lrx = idx;
            }
        } else {
            dl = ((s32)((u32)slot1 * (u32)idx)) >> 7; if (arg3) {}
            idx = 0;
            if (dl < 0) {
                idx = dl;
            }
            dl = idx;
            lrx = dl; if (arg0) {}
        }
    } else {
        lrx = 0;
        u = (s32)(((f32)(sp30 - 1) * 1024.0f) / hs);
    }
    *(s32 *)(slot3 + 4) = ((-(s32)t) << 0x10) | ((-lrx) & 0xFFFF);

    t = *dp;
    dl = *(s32 *)(t + 0xC);
    idx = *(s32 *)(dl + 4);
    *(s32 *)(dl + 4) = idx + 1;
    w = *(s32 *)(*(s32 *)(t + 0xC)) + idx * 8;
    *(s32 *)w = 0xB3000000;
    *(s32 *)(w + 4) = (slot2 << 0x10) | (u & 0xFFFF);
    if (arg4) {}
}
