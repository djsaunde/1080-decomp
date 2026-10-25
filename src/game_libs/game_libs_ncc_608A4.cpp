/* IDO 7.1 NCC (C++) EXACT donor for gl_func_000608A4 (0x1D0 / 116 words), agent-c run 175, spliced over an empty
 * placeholder in game_libs_post0b.c in both build trees (Makefile rename of draw__7TextWinFi).
 * `void TextWin::draw(int big)`: the cfe body (83.91, m2c temps) as a C++ text-window member -- an optional big-font
 * switch, a by-address Pt(0, 0x408) passed to the frame call, the clipped right edge (290), the title through the
 * blank callee, then one call per visible row whose length exceeds the horizontal scroll. The global draw context
 * (&D_00000000, held in s4) is set through inline members `size(w, h)` / `pos(px, py)`: their parameters are the
 * target's s0 / v0 values around the stores. Calls go to gl_func_00000000 (the target's baked jal 0). */
extern "C" {
void gl_func_00000000(...);
}
struct Ctx { char pad[0x21C]; int x, y, cw, ch;
    void size(int w, int h) { cw = w; ch = h; }
    void pos(int px, int py) { x = px; y = py; }
    void posr(int px, int py) { y = py; x = px; }
};
extern "C" Ctx D_00000000;
struct Pt { int a, b; Pt(int a_, int b_) { a = a_; b = b_; } };
struct Row { char *s; int len; };
struct TextWin {
    Row *rows;
    char *buf;
    int pad8, padC;
    int hs;
    int vis;
    int top;
    int cols;
    int pad20;
    int cw;
    int ch;
    int x;
    int y;
    int pad34[3];
    char *font;
    int pad44;
    char color[4];
    void draw(int big);
};
void TextWin::draw(int big) {
    if (big) gl_func_00000000(&D_00000000, 1);
    Pt p(0, 0x408);
    if (big) {
        cw = 8;
        ch = 8;
    } else {
        gl_func_00000000(&D_00000000, color);
    }
    int w = cols * cw;
    int right = x + w;
    if (right > 290) right = 290;
    gl_func_00000000(&D_00000000, x, y, w + 8, vis * ch + 16, &p);
    D_00000000.size(8, 8);
    D_00000000.pos(x + 4, y + 1);
    gl_func_00000000(&D_00000000, font, right);
    D_00000000.size(cw, ch);
    int yy = y + 12;
    for (int i = top; i < top + vis; i++, yy += ch) {
        if (hs < rows[i].len) {
            D_00000000.pos(x + 4, yy);
            gl_func_00000000(&D_00000000, rows[i].s + hs, right);
        }
    }
    gl_func_00000000(&D_00000000, 0);
}
