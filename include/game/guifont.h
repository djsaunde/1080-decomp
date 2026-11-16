#ifndef GAME_GUIFONT_H
#define GAME_GUIFONT_H

/* gui.uso bitmap font (agent-a run 590). gui_fontCreate (0x148; its exact NCC donor
 * src/gui_uso/gui_uso_ncc_0148.cpp spells the same layout as GuiGlyphGrid148 / GuiGlyphCell148)
 * builds it: a 40-byte font over an IA8 glyph atlas cut into rows * cols cells, one 20-byte
 * GuiGlyph per cell. gui_drawString / gui_drawString_00000D04 / gui_stringWidth index glyphs[] by
 * the character's cell (gl_func_00000000(c)) and step the pen by width / advance; a space steps it
 * by spaceWidth (drawString, stringWidth) or glyphs[0].advance (drawString_00000D04).
 * Plain C types only, so the NCC donors can include it. */

typedef struct GuiGlyph {
    /* 0x00 */ int x;               /* first ink column in the atlas */
    /* 0x04 */ int y;               /* the cell's atlas row */
    /* 0x08 */ int width;           /* ink width (right edge minus leftBearing) */
    /* 0x0C */ int advance;         /* the font's maximum right edge (fixed pitch) */
    /* 0x10 */ int leftBearing;     /* first ink column within the cell */
} GuiGlyph; /* size 0x14 */

typedef struct GuiFont {
    /* 0x00 */ int cellCount;       /* rows * cols */
    /* 0x04 */ char *pixels;        /* the IA8 atlas (gui_loadTileIA8 arg 2) */
    /* 0x08 */ int spaceWidth;      /* half the mean cell width */
    /* 0x0C */ int cellWidth;       /* initial width, then the maximum right edge */
    /* 0x10 */ int cellHeight;
    /* 0x14 */ int height;          /* the drawn glyph height */
    /* 0x18 */ int atlasWidth;
    /* 0x1C */ int atlasHeight;
    /* 0x20 */ GuiGlyph *glyphs;    /* cellCount records */
    /* 0x24 */ void *drawCtx;       /* the display-list context the draw calls append to */
} GuiFont; /* size 0x28 */

typedef char GuiGlyph_size_check[sizeof(GuiGlyph) == 0x14 ? 1 : -1];
typedef char GuiFont_size_check[sizeof(GuiFont) == 0x28 ? 1 : -1];

#endif /* GAME_GUIFONT_H */
