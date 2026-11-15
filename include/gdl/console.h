#ifndef GDL_CONSOLE_H
#define GDL_CONSOLE_H

#include "gdl/node.h"

/* libgdl text console (game_libs post0b 0x60584-0x61A0C; see docs/1080_ENGINE_CORE.md).
 *
 * TextWin (0x58, TextWin_ctor 0x60584 (self, title, maxCols, nRows, cols, visRows, colour))
 * is a scrolling text window: nRows line records over one (maxCols + 1) * nRows character
 * buffer, of which visRows rows x cols columns are drawn at (x, y) in cw x ch cells by
 * TextWin_draw (its exact NCC donor, src/game_libs/game_libs_ncc_608A4.cpp, spells the
 * same layout as C++ members: rows, buf, hs, vis, top, cols, cw, ch, x, y, font, color).
 * TextWin_write appends to rows[cur] until maxCols, TextWin_newLine advances cur (rotating
 * the line records once the buffer is full) and keeps top = cur - visRows + 1; the
 * ConsoleMgr_scroll* helpers move top / hScroll. The windows of a ConsoleMgr form a
 * circular doubly-linked list through next / prev (ConsoleMgr_linkWin). */
typedef struct TextWinRow {
    /* 0x0 */ char *text;
    /* 0x4 */ s32 len;
} TextWinRow;

typedef struct TextWin {
    /* 0x00 */ TextWinRow *rows;  /* nRows records, ctor alloc(nRows * 8) */
    /* 0x04 */ char *buf;          /* ctor alloc((maxCols + 1) * nRows); rows[i].text = buf + i * (maxCols + 1) */
    /* 0x08 */ s32 maxCols;        /* chars per line (TextWin_write wraps at it) */
    /* 0x0C */ s32 nRows;          /* line records */
    /* 0x10 */ s32 hScroll;        /* first drawn column (ConsoleMgr_scrollLeft / Right) */
    /* 0x14 */ s32 visRows;        /* rows drawn */
    /* 0x18 */ s32 top;            /* first drawn row (ConsoleMgr_scrollUp / Down, TextWin_newLine) */
    /* 0x1C */ s32 cols;           /* columns drawn */
    /* 0x20 */ s32 cur;            /* line being written */
    /* 0x24 */ s32 cw;             /* cell width, 6 (8 with the big font) */
    /* 0x28 */ s32 ch;             /* cell height, 8 */
    /* 0x2C */ s32 x;              /* 0x14 */
    /* 0x30 */ s32 y;              /* 0x28 */
    /* 0x34 */ s32 muted;          /* TextWin_printf prints only when 0; TextWin_toggleMute flips bit 0 */
    /* 0x38 */ struct TextWin *next; /* ConsoleMgr window ring */
    /* 0x3C */ struct TextWin *prev;
    /* 0x40 */ char *title;        /* ctor arg1, drawn in the title bar */
    /* 0x44 */ s32 owner;          /* ctor: result of a blank-import call; ConsoleMgr_closeOwnerWins matches it */
    /* 0x48 */ u8 colour[16];      /* 16-byte by-value ctor arg (S_00060584); its first 4 bytes are the RGBA
                                    * passed to the colour call */
} TextWin;

/* ConsoleMgr (0x3C, ConsoleMgr_ctor 0x60D40, name sConsoleMgrName): a Node that owns the
 * TextWin ring. Its Node DoDisplay flag (doFlags bit 0x4) is the console's visibility
 * (ConsoleMgr_toggleVisible / ConsoleMgr_setFlag_00060FFC). */
typedef struct ConsoleMgr {
    /* 0x00 */ Node node;
    /* 0x2C */ TextWin *curWin;    /* ConsoleMgr_nextWin / prevWin step it through next / prev */
    /* 0x30 */ TextWin *winList;   /* ring head (0 = empty) */
    /* 0x34 */ s32 input;          /* pad-state object: ConsoleMgr_display tests its +0x10 / +0x18 button bits */
    /* 0x38 */ s32 unk38;          /* passed to the final draw call in ConsoleMgr_display */
} ConsoleMgr;

typedef char TextWinRow_size_check[sizeof(TextWinRow) == 0x8 ? 1 : -1];
typedef char TextWin_size_check[sizeof(TextWin) == 0x58 ? 1 : -1];
typedef char TextWin_cur_check[(int)&((TextWin *)0)->cur == 0x20 ? 1 : -1];
typedef char TextWin_next_check[(int)&((TextWin *)0)->next == 0x38 ? 1 : -1];
typedef char TextWin_colour_check[(int)&((TextWin *)0)->colour == 0x48 ? 1 : -1];
typedef char ConsoleMgr_size_check[sizeof(ConsoleMgr) == 0x3C ? 1 : -1];
typedef char ConsoleMgr_unk38_check[(int)&((ConsoleMgr *)0)->unk38 == 0x38 ? 1 : -1];

#define TEXTWIN(p) ((TextWin *)(p))
#define CONSOLEMGR(p) ((ConsoleMgr *)(p))

#endif /* GDL_CONSOLE_H */
