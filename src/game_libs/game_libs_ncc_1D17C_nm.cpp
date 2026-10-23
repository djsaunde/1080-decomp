/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0001D17C
 * (0x84 / 33 words): three display-list packets for record a1 (stride 344),
 * the two middle words OR'ing the record's +0x28 / +0x2A halfwords into
 * 0x0C1A0000; returns the advanced pointer. NCC 96.0 vs cfe 89.0 (agent-c
 * run 133), 33 words = target (cfe folds the cursor into offsets, 31). The
 * gbi-style `{ Gfx *_g = p++; ... }` copies keep the chained cursors
 * (a2 = a0 + 8, t0 = a2 + 8); the first packet is stored through p directly.
 * Still NON_MATCHING (16 words): colours -- record base v1 (target a3), the
 * shared 0x0C1A0000 a3 (target t1), the third cursor a0 (target t0).
 * Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
extern "C" int D_00000000;
struct Gfx { unsigned int w0, w1; };
extern "C" Gfx *game_libs_func_0001D17C(Gfx *p, int a1) {
    char *rec = (char *)&D_00000000 + a1 * 344;
    p->w0 = 0x0A000C80; p->w1 = 0x03E001A0; p++;
    { Gfx *_g = p++; _g->w0 = *(unsigned short *)(rec + 0x28) | 0x0C1A0000; _g->w1 = 0x0E200C80; }
    { Gfx *_g = p++; _g->w0 = *(unsigned short *)(rec + 0x2A) | 0x0C1A0000; _g->w1 = 0x03E00E20; }
    return p;
}
