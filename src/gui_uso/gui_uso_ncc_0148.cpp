/* IDO 7.1 NCC (C++) donor for gui_func_00000148 (0x414 / 261 words).
 *
 * Glyph-grid layout constructor (find-or-create a 40-byte grid, scan each
 * cell's ink columns, fill the 20-byte cell records). The cfe NM was 255/261
 * (frame 136 vs 128). Under NCC the frame is one word per declared scalar,
 * so the fix is the declaration set, not the order of homes:
 *  - no line local: the scan indexes (px + y * a2)[x];
 *  - the width sum reuses the a1 parameter (dead after the pixels store),
 *    which with 12 locals puts the anonymous gw/gh spills at sp+72/68;
 *  - a dead "if (maxw) {}" after the fill loop extends maxw's live range so
 *    uopt ranks the a1/sum web first (s5) and maxw second (s6); found by a
 *    10-minute permuter run on this NCC source.
 * docs/IDO_CODEGEN.md kit XXXIII. Spliced via REPLACE_FUNC_BODY into both
 * gui_uso trees. */
extern "C" { void *gl_func_00000000(...); }
typedef struct GuiGlyphCell148 {
    int x;                      /* 0x00: first ink column in the atlas */
    int y;                      /* 0x04: cell's atlas row */
    int ink_width;              /* 0x08: right edge minus left bearing */
    int advance;                /* 0x0C: shared maximum right edge */
    int left_bearing;           /* 0x10: first ink column within the cell */
} GuiGlyphCell148;

typedef struct GuiGlyphGrid148 {
    int cell_count;             /* 0x00 */
    char *pixels;               /* 0x04 */
    int half_mean_width;        /* 0x08: integer-truncated half-sum / count */
    int cell_width;             /* 0x0C: initial width, then maximum right edge */
    int cell_height;            /* 0x10 */
    int height_14;              /* 0x14: initialized to the same height */
    int atlas_width;            /* 0x18 */
    int atlas_height;           /* 0x1C */
    GuiGlyphCell148 *cells;     /* 0x20 */
    void *draw_state;           /* 0x24: not initialized by this function */
} GuiGlyphGrid148;

extern "C" GuiGlyphGrid148 *gui_func_00000148(GuiGlyphGrid148 *a0, int a1, int a2, int a3, int rows, int cols) {
    /* NCC frame: the 12 declared scalars (sum lives in the dead a1 parameter). */
    int idx;
    int col;
    int maxw;
    int maxx;
    char *px;
    int x;
    int y;
    int row;
    int gi;
    int gw;
    int gh;
    int minx;

    if (a0 == 0) {
        a0 = (GuiGlyphGrid148 *)gl_func_00000000(sizeof(GuiGlyphGrid148));
        if (a0 == 0) {
            goto end;
        }
    }
    gw = a2 / rows;
    gh = a3 / cols;
    a0->pixels = (char *)a1;
    a0->cell_width = gw;
    a0->cell_count = rows * cols;
    a0->cell_height = gh;
    a0->height_14 = gh;
    a0->atlas_width = a2;
    a0->atlas_height = a3;
    a0->cells = (GuiGlyphCell148 *)gl_func_00000000(rows * cols * sizeof(GuiGlyphCell148));

    a1 = 0;
    maxw = 0;
    for (col = 0; col < cols; col++) {
        row = 0;
        if (rows > 0) {
            gi = col * rows;
            do {
                px = a0->pixels + (col * gh) * a2 + row * gw;
                idx = gi;
                maxx = 0; minx = gw;
                for (y = 0; y < gh; y++) {
                    x = y * a2; /* dead; kept from the original scan-line staging */
                    /* Same-line scan preserves IDO's unrolled compare schedule. */
                    for (x = 0; x < gw; x++) { if ((px + y * a2)[x] != 0) { if (x < minx) minx = x; if (maxx < x) maxx = x; } }
                }
                maxx += 1;
                if (maxw < maxx) maxw = maxx;
                a0->cells[idx].x = row * gw + minx;
                a0->cells[idx].y = col * gh;
                a0->cells[idx].ink_width = maxx - minx;
                a1 += maxx;
                a0->cells[idx].left_bearing = minx;
                gi++;
                row++;
            } while (row != rows);
        }
    }
    a0->cell_width = maxw;
    a0->half_mean_width = (int)((float)a1 * 0.5f) / a0->cell_count;
    {
        for (row = 0; row < a0->cell_count; row++) {
            a0->cells[row].advance = a0->cell_width;
        }
        if (maxw) {} /* keeps maxw live: s6 for maxw, s5 for the a1/sum web */
    }
end:
    return a0;
}
