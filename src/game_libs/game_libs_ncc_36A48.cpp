/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00036A48 (0x154 / 85 words), agent-c run 163.
 * Best-aligned candidate: for each arr[i], r = cb(o, dir, arr[i]) (V3 returned through a hidden
 * pointer, both V3 arguments word-copied into the outgoing area); d = r . dir; keep the index of the
 * largest d (best starts at -1.0f). Load-bearing: V3's `V3 &operator=(V3 v)` taking its argument BY
 * VALUE gives the target's return-temp -> parameter word copy -> float copy into r (the float loads
 * are reused by the dot product); returning `V3 &` (not void) costs the extra 0xC temp between the
 * parameter and the return slot; r declared first is homed at the top of the frame; one block-scoped
 * unused volatile at the end fills the 0xB0 frame. The cfe body reached 97.20. Spliced over an empty
 * placeholder in both build trees. */
typedef float f32;
struct V3 {
    f32 x, y, z;
    V3 &operator=(V3 v) { x = v.x; y = v.y; z = v.z; return *this; }
};
extern "C" {
V3 gl_36A48_cb(void *, V3, V3);
}
extern "C" int gl_func_00036A48(void *o, V3 dir, V3 *arr, int n) {
    V3 r;
    f32 best;
    int bi;
    int i;
    f32 d;
    best = -1.0f;
    bi = 0;
    for (i = 0; i < n; i++) {
        r = gl_36A48_cb(o, dir, arr[i]);
        d = r.x * dir.x + r.y * dir.y + r.z * dir.z;
        if (best < d) {
            best = d;
            bi = i;
        }
    }
    { volatile int z0; }
    return bi;
}
