/* IDO 7.1 NCC (C++) donor for CheckpointCyl_isBetweenPosts (0x130), spliced into
 * checkpoint_uso.c.o via REPLACE_FUNC_BODY (agent-b run 640). EXACT.
 *
 * Cylinder checkpoint "between the posts" test: u = pos - postA, w = pos - postB, both
 * flattened to the XZ plane (y = 0); returns 1 when u . w < 0, i.e. pos lies between the two
 * gate posts that CheckpointCyl_handleMsg places at +0xA4 / +0xE4.
 *
 * Load-bearing (each found by a grid over the shape, run 640):
 *  - V3 value class: `operator-` builds a ctor temp that is returned by value, and the
 *    by-value `operator=` forwards to `set(const V3 &)`. That gives the target's copy chain
 *    (ctor temp sp+0x44 / 0x8 -> return slot 0x54 -> by-value parameter 0x1C -> float copies).
 *    `operator=` must return `V3 &`: with `void` the first ctor temp lands 4 bytes low (0x40).
 *  - `const V3 &pos = *p` and `pos - *a`: the same expression on the pointer, `*p - *a`, gives
 *    a different edg temp layout (frame and first temp 4 bytes off).
 *  - The post pointers are kept named with `if (0) { a = 0; }` (game_libs_ncc_3B01C.cpp): no
 *    offset folding, so the target's `addiu v0, a0, 0xA4` / `0xE4` base.
 *  - `w.y = u.y = 0.0f` then the plain 3-term dot: the y stores are dead and go away, but the
 *    `0 * 0` term stays (an mtc1 zero that is not folded).
 *  - The dot product in a named `d`: uopt colours it (f16) alongside the zero (f18), which takes
 *    both out of the FP temp ring, so the z loads reuse f4 / f6 as in the target.
 */
typedef float f32;
struct V3 {
    f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    V3 operator-(const V3 &o) const { return V3(x - o.x, y - o.y, z - o.z); }
    V3 &operator=(V3 v) { set(v); return *this; }
    f32 operator*(const V3 &o) const { return x * o.x + y * o.y + z * o.z; }
};
/* CheckpointCyl (0x10C): SNode, then two CollObj gate posts; the post positions are kept at
 * +0xA4 / +0xE4 (CheckpointCyl_handleMsg). */
struct CheckpointCyl { char pad[0xA4]; V3 postA; char padb[0xE4 - 0xB0]; V3 postB; };

extern "C" int CheckpointCyl_isBetweenPosts(CheckpointCyl *self, V3 *p) {
    V3 u, w;
    const V3 &pos = *p;
    V3 *a = &self->postA;
    if (0) { a = 0; }
    u = pos - *a;
    V3 *b = &self->postB;
    if (0) { b = 0; }
    w = pos - *b;
    w.y = u.y = 0.0f;
    f32 d = u * w;
    if (d < 0.0f) return 1;
    return 0;
}
