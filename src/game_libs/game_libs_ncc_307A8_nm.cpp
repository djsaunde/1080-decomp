/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_000307A8 (0x104 / 65 words), agent-c run 177.
 * Fade timer tick over the 8-word state block that the exact sibling 308AC (game_libs_ncc_308AC.cpp) reads as a
 * FILE-LOCAL static: as1 shares one `lui at` between two accesses only when both hit the same LOCAL symbol and
 * uopt has not CSE'd the address. So the two target pairs -- the zeroing stores `lui at; sw zero,12(at);
 * sw zero,8(at)` and the down-arm loads `lui at; lwc1 $f0,16(at); lwc1 $f2,20(at)` -- are two references to
 * fields of the static A308 in ONE block, and every other access is a per-site extern alias (D_307A8_x, the cfe
 * body's 349E0 alias-budget rule) so no address is CSE'd into v0. This removes the two extra `lui at` words the
 * cfe body could not lose (68 -> 65 words = target). The `while (0) { f = c + c; }` / `if (c) {}` keep-alives
 * are the cfe body's (load order of the up arm; c live into the else arm).
 * Residual (5 words): the hold count is a candidate (v1, decrement t8) where the target keeps it in the ugen ring
 * (lui/lw t8 hoisted above the beqz, decrement t9) -- no single-load spelling found that is not a uopt candidate.
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
typedef struct { int mode; int f4; int f8; int fC; f32 f10; f32 f14; f32 f18; f32 f1C; } St308;
static St308 A308;
extern "C" St308 D_307A8_a, D_307A8_d, D_307A8_e, D_307A8_f, D_307A8_g, D_307A8_h, D_307A8_i, D_307A8_j,
    D_307A8_m, D_307A8_n, D_307A8_o, D_307A8_p;
extern "C" int D_00000000_1c0a8;
extern "C" void gl_func_00000000_f(int, f32);
extern "C" void game_libs_func_000307A8(void) {
    int c;
    f32 f;
    f32 fl;
    if (D_307A8_a.mode == 0) {
        if (D_00000000_1c0a8 != 5) {
            return;
        }
        { int &r = A308.fC; int &q = A308.f8; r = 0; q = 0; }
    }
    c = D_307A8_d.f8;
    if (c != 0) {
        f = D_307A8_e.f10; while (0) { f = c + c; }
        f += D_307A8_f.f18;
        c -= 1;
        D_307A8_g.f8 = c;
        if (1.0f < f) {
            f = 1.0f;
        }
        D_307A8_h.f10 = f;
    } else if (D_307A8_i.fC != 0) {
        D_307A8_j.fC = D_307A8_i.fC - 1;
    } else {
        { f32 &r = A308.f10; f32 &q = A308.f14; f = r; fl = q; } if (c) {}
        f -= D_307A8_m.f1C;
        D_307A8_n.f10 = f;
        if (f < fl) {
            D_307A8_o.f10 = fl;
        }
    }
    f = D_307A8_p.f10;
    gl_func_00000000_f(0x01000800, f);
}
