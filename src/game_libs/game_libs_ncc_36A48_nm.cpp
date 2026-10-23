/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00036A48 (0x154 / 85
 * words): index of the array entry whose callback-transformed vector has the
 * largest dot product with dir (best starts at -1.0f). NCC 97.1 vs cfe 87.9
 * (82 -> 85 words = target; agent-c run 133). Load-bearing: the callback
 * returns V3 BY VALUE (edg return temp + the r copy through s4); `v = r`
 * goes through a member operator= (float-wise lwc1 / swc1 copy to v at
 * sp+164); v declared first and a 5-word assigned pad give frame 176.
 * Still NON_MATCHING (9 words): r sits at sp+120 (target 92) and the return
 * temp at 100 (target 116). Declaring r at function scope, `V3 r; r = ...`,
 * and pads of 0-8 words do not swap them.
 * Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
struct V3 { float x, y, z; V3 &operator=(const V3 &o) { x = o.x; y = o.y; z = o.z; return *this; } };
extern "C" V3 gl_36A48_cb(void *, V3, V3);
extern "C" int gl_func_00036A48(void *o, V3 dir, V3 *arr, int n) {
    V3 v;
    int pz[5]; pz[0] = 0;
    float best = -1.0f; int bi = 0;
    for (int i = 0; i < n; i++) { V3 r = gl_36A48_cb(o, dir, arr[i]); v = r; float d = v.x * dir.x + v.y * dir.y + v.z * dir.z; if (best < d) { best = d; bi = i; } }
    return bi;
}
