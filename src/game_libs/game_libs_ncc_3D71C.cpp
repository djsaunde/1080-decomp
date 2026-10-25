/* IDO 7.1 NCC (C++) EXACT donor for gl_func_0003D71C (0xDC / 55 words), agent-c run 162.
 * Guarded camera-matrix reset: if test(G+0x254, 3, x, y, z, 0, 0.0f) copy the 0x40-byte block D+0xE8 to the
 * D+0xA0 matrix and scale its translation row by the D+0x128.. factors. The cfe body (96.00) could not order
 * the two base addius (as1 put the source first); as C++ the destination is a KEPT named pointer
 * (`if (0) { d = 0; }`), which puts its addiu (v0) ahead of the source's (t6) like the target, and each
 * product is written `o->x * factor` (edg keeps the written operand order; the factor loads first).
 * Spliced over an empty placeholder in both build trees. */
typedef float f32;
struct Blk { int w[16]; };
struct Obj { char pad[0x30]; f32 x, y, z; };
extern "C" {
int gl_3D71C_test(int, int, f32, f32, f32, int, f32);
void gl_func_00000000(...);
void gl_3d71c_dA0();
extern char D_00000000[];
}
extern "C" void gl_func_0003D71C(Obj *o) {
    if (gl_3D71C_test(*(int *)(D_00000000 + 0x254), 3, o->x, o->y, o->z, 0, 0.0f) != 0) {
        Blk *d = (Blk *)gl_3d71c_dA0;
        if (0) { d = 0; }
        *d = *(Blk *)(D_00000000 + 0xE8);
        *(f32 *)((char *)d + 0x30) = o->x * *(f32 *)(D_00000000 + 0x128);
        *(f32 *)((char *)d + 0x34) = o->y * *(f32 *)(D_00000000 + 0x12C);
        *(f32 *)((char *)d + 0x38) = o->z * *(f32 *)(D_00000000 + 0x130);
        gl_func_00000000(o);
    }
}
