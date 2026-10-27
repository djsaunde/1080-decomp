/* IDO 7.1 NCC (C++) donor for func_000076F4 (0xDC / 55 words) -- the bootup twin of gl_func_00039E24
 * (docs/IDO_CODEGEN.md kit CXXVIII / CXXIX): query the root (G+0x18) through a 7-arg prototyped call; on
 * success hand the global block and s0+0x2C to a callee, scale the camera vector by the block's scale
 * triple (+0x128) through an inline member Cam::mul(G &) called on a local reference `Cam &c = g.cam`, add
 * s0->0x6C to its y, call the second helper on s0 and bump the emit counter.
 * - the camera is a MEMBER of the same block (block+0xA0, vector at +0x30 of it; root at +0x254 -- the ROM
 *   immediates, the expected object's relocation addends read 0x20 / 0x18): `this` (lui v0) is
 *   materialised before the re-materialised block base (lui a2) and each scale load stays after the
 *   previous vector store (may-alias through the reference) -- the two things the cfe body (99.82, a
 *   function-typed vector alias) could not get together;
 * - the stack-argument product is written 0x80 * 0x70 (NCC load order; cfe wanted 0x70 * 0x80).
 * Spliced into bootup_uso.c.o (both trees) via REPLACE_FUNC_BODY over an empty placeholder;
 * bu_76f4_g / bu_76f4_cnt / func_00000000_76f4_q / func_00000000_76f4_v are blank (0) aliases. */
extern "C" {
int func_00000000_76f4_q(void *, int, int, int, float, int, float);
void func_00000000_76f4_v(...);
extern int bu_76f4_cnt;
}
struct V3 { float x, y, z; };
struct G;
struct Cam { char pad[0x30]; V3 v; void mul(G &g); };
struct G { char pad[0xA0]; Cam cam; char padDC[0x128 - 0xDC]; V3 scale; char pad134[0x254 - 0x134]; void *root; };
inline void Cam::mul(G &g) { v.x *= g.scale.x; v.y *= g.scale.y; v.z *= g.scale.z; }
extern "C" G bu_76f4_g;
extern "C" void func_000076F4(char *s0) {
    if (func_00000000_76f4_q(bu_76f4_g.root, 3, *(int *)(s0 + 0x5C), *(int *)(s0 + 0x60),
                    *(float *)(s0 + 0x64), 0, *(float *)(s0 + 0x80) * *(float *)(s0 + 0x70)) != 0) {
        func_00000000_76f4_v(&bu_76f4_g, s0 + 0x2C);
        {
            Cam &c = bu_76f4_g.cam;
            c.mul(bu_76f4_g);
            c.v.y += *(float *)(s0 + 0x6C);
        }
        func_00000000_76f4_v(s0);
        bu_76f4_cnt += 1;
    }
}
