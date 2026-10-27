/* IDO 7.1 NCC (C++) donor for gl_func_00039E24 (0xC0 / 48 words), spliced into game_libs_post0b.c.o via
 * REPLACE_FUNC_BODY. (agent-c run 180) Query the root through a 7-arg prototyped call; on success hand the
 * global block and self+0x2C to a callee, then scale the camera vector (block+0xA0, Vec3 at +0x30) by the
 * block's scale triple (+0x128) and add self->0x6C to its y. The camera is a MEMBER of the same global block
 * (D+0xA0), scaled through an inline member Cam::mul(G &) called on a local reference `Cam &c = g.cam`:
 * - the reference parameter keeps each scale load after the previous vector store (may-alias), and
 * - `this` (block+0xA0, lui v0) is materialised before the re-materialised block base (lui a2) -- the order
 *   the cfe decode (99.79, gl_39e24_vf function-typed alias) could not reach;
 * - the y add through the same reference keeps the 0x34(v0) form. EXACT. */
extern "C" {
int gl_proto_39E24(int, int, float, float, float, int, float);
void gl_func_00000000_39E24v(...);
int gl_func_00000000(...);
}
struct V3 { float x, y, z; };
struct G;
struct Cam { char pad[0x30]; V3 v; void mul(G &g); };
struct G { char pad[0xA0]; Cam cam; char padDC[0x128 - 0xDC]; V3 scale; char pad134[0x254 - 0x134]; int root; };
inline void Cam::mul(G &g) { v.x *= g.scale.x; v.y *= g.scale.y; v.z *= g.scale.z; }
extern "C" G D_39E24_g;
extern "C" void gl_func_00039E24(char *a0) {
    if (gl_proto_39E24(D_39E24_g.root, 3, *(float *)(a0 + 0x5C), *(float *)(a0 + 0x60),
                    *(float *)(a0 + 0x64), 0, 0.0f) != 0) {
        gl_func_00000000_39E24v(&D_39E24_g, a0 + 0x2C);
        {
            Cam &c = D_39E24_g.cam;
            c.mul(D_39E24_g);
            c.v.y += *(float *)(a0 + 0x6C);
        }
        gl_func_00000000(a0);
    }
}
