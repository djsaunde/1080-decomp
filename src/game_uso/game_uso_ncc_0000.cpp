/* IDO 7.1 NCC (C++) donor for game_uso_func_00000000 (0xA0 / 40 words).
 *
 * Uniform cubic B-spline basis: the four weights for parameter t into
 * out[0..3]. The 1/6 is a float LITERAL (the unit's .rodata item 0), not a
 * local loaded from a data symbol: as a constant it becomes a uopt
 * reg-alloc-prep candidate, coloured after omt/t2/t3 ($f16 here, as in the
 * target), while its lui/lwc1 still schedules first. A `float D = ...` local
 * colours $f0 first; a global re-read after every out[] store.
 * edg keeps the source operand order, so every product is written D-first.
 * The donor's .rodata reloc is renamed by replace-function-body.py to
 * game_uso_func_00000000_rodata (pinned to 0 in undefined_syms_auto.txt);
 * the host placeholder re-emits the 1/6 word so game_uso.c.o's .rodata
 * keeps its layout. */
extern "C" void game_uso_func_00000000(float *out, float t) {
    float omt = 1.0f - t;
    float t2 = t * t;
    float t3 = t2 * t;
    out[0] = (1.0f / 6.0f) * omt * omt * omt;
    out[1] = (1.0f / 6.0f) * (3.0f * t3 - 6.0f * t2 + 4.0f);
    out[2] = (1.0f / 6.0f) * (3.0f * (t2 - t3 + t) + 1.0f);
    out[3] = (1.0f / 6.0f) * t3;
}
