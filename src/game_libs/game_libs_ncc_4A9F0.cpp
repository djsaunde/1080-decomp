/* IDO 7.1 NCC (C++) EXACT donor for game_libs_func_0004A9F0 (0xA0 / 40 words),
 * spliced over the empty placeholder in game_libs_post0b.c (both trees): copy
 * 3-byte source records (a1 + idx1 * 4) into grid cells (+0xC) of D+0x214's
 * grid at self->0xD4 + idx2, driven by a -1 terminated halfword stream.
 * NCC (agent-c run 133): frameless with self in a3 (the cfe body promotes self
 * to s0 with a frame). Load-bearing: the cell store is an inline member
 * Self::put(i, src) (self is then `this`, a copy in a3), the source pointer is
 * passed as an expression (loop-invariant temp t0), and the grid root is a named
 * local in put (v0).
 * agent-c run 217: EXACT. (1) An `if (1) {}` after the put call ends the inner
 * body's block before the cursor re-read: the cursor copy then colours v0 (the
 * dead grid root's register, target) instead of t1, and the -1 / &D registers
 * drop back to t1 / t2. (2) The cell address as ONE expression,
 * `&cells[base] + i`: the row sum goes to a ugen temp (t4) and the cell to v1,
 * which also puts the byte loads on the target's t6 / t7 / t9 (the run-191
 * two-step `cell += i` kept both sums in v1). */
extern "C" int D_00000000;
struct Cell { char pad[0xC]; char c[3]; char pad2; };
struct Grid { Cell *cells; };
struct G214 { char pad[0x1C]; Grid *grid; };
struct Self { char pad[0xD4]; short base; void put(int i, char *src); };
inline void Self::put(int i, char *src) { G214 *g = *(G214 **)((char *)&D_00000000 + 0x214); Cell *cell = &g->grid->cells[base] + i; cell->c[0] = *src++; cell->c[1] = *src++; cell->c[2] = *src; }
extern "C" void game_libs_func_0004A9F0(Self *self, char *a1, short *cursor) {
    short idx1, idx2;
    while ((idx1 = *cursor++) != -1) {
        while ((idx2 = *cursor++) != -1) { self->put(idx2, a1 + idx1 * 4); if (1) {} }
    }
}
