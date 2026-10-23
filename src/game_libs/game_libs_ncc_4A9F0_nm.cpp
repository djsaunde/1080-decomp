/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0004A9F0
 * (0xA0 / 40 words): copy 3-byte source records (a1 + idx1 * 4) into grid
 * cells (+0xC) of D+0x214's grid at self->0xD4 + idx2, driven by a -1
 * terminated halfword stream. NCC 89.2 vs cfe 76.5 (agent-c run 133), and
 * the length is the target's 40 words, FRAMELESS with self in a3. The cfe
 * body promotes self to s0 with a frame. Load-bearing: the cell store is an
 * inline member Self::put(i, src) (self is then `this`, a copy in a3), the
 * source pointer is passed as an expression (loop-invariant temp t0), and the
 * grid root is a named local in put (v0).
 * Still NON_MATCHING: the inner cursor copy colours t1 (target v0), which
 * shifts the -1 / &D registers by one, and the target loads src[0] before
 * the grid. Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
extern "C" int D_00000000;
struct Cell { char pad[0xC]; char c[3]; char pad2; };
struct Grid { Cell *cells; };
struct G214 { char pad[0x1C]; Grid *grid; };
struct Self { char pad[0xD4]; short base; void put(int i, char *src); };
inline void Self::put(int i, char *src) { G214 *g = *(G214 **)((char *)&D_00000000 + 0x214); Cell *cell = &g->grid->cells[base + i]; cell->c[0] = *src++; cell->c[1] = *src++; cell->c[2] = *src; }
extern "C" void game_libs_func_0004A9F0(Self *self, char *a1, short *cursor) {
    short idx1, idx2;
    while ((idx1 = *cursor++) != -1) {
        while ((idx2 = *cursor++) != -1) self->put(idx2, a1 + idx1 * 4);
    }
}
