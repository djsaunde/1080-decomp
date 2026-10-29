/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0004F2DC (0x428 / 266 words), agent-g run 194.
 * Screen-point -> world raycast picker as C++ (kit CXLVI lead: the target re-reads the ray direction from memory
 * for the K* products = by-reference inline operators). The ray direction is built member by member, rotated
 * and normalised in place by two callees, then `d = dir` through a by-value operator= (the target's word copy
 * 200 -> 180 then float stores 212..220), `start + d * far` through the by-value operator* / operator+ return
 * and parameter temporaries (160 -> 180 -> 412, 112 -> 132 -> 84), `start *= kvec` in place and the scaled end
 * point through the ctor (no temporary). `if (1) {}` after `start *= kvec` (lever-sweep bbsplit) keeps the
 * kvec loads after the in-place products. cfe body 90.36 -> 94.78 standalone, length = target (1064).
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32;
extern "C" {
int gl_func_00000000(...);
extern int D_00000000B;
extern int D_00000000E;
extern f32 D_00000000F[3];
}
struct V3 { f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void operator=(V3 v) { x = v.x; y = v.y; z = v.z; }
    V3 operator*(f32 s) const { return V3(x * s, y * s, z * s); }
    V3 operator+(V3 o) const { return V3(x + o.x, y + o.y, z + o.z); }
    void operator*=(const V3 &k) { x *= k.x; y *= k.y; z *= k.z; }
    void operator/=(const V3 &k) { x /= k.x; y /= k.y; z /= k.z; }
};
#define KV (*(V3 *)D_00000000F) /* collision kvec (&D + 0x128) */
struct Cam { char p[0xA0]; V3 pos; char pac[0xB4 - 0xAC]; char mtx[1]; };
extern "C" int game_libs_func_0004F2DC(char *arg0, int x, int y, V3 *out) {
    f32 dists[8];
    int surfs[8];
    f32 hits[8][3];
    int count; /* agent-g run 195 (declmove sweep): below dists / surfs; agent-g run 224: below hits too */
    count = 0;
    V3 dir;
    dir.x = -((f32)x - 160.0f); dir.y = (f32)y - 120.0f; dir.z = 8.0f / *(f32 *)((char *)&D_00000000B + 0x128);
    gl_func_00000000(&dir, (*(Cam **)(arg0 + 0x70))->mtx);
    gl_func_00000000(&dir);
    V3 d;
    d = dir;
    Cam *cam = *(Cam **)(arg0 + 0x70);
    V3 start; start.x = cam->pos.x; start.y = cam->pos.y; start.z = cam->pos.z;
    V3 end = start + d * -100000.0f; /* agent-c run 218: bootup.uso RoData 0x1BA8 / 0x1BAC are the literals -1e5 / 1e5 */
    start *= KV; if (1) {}
    V3 e(end.x * KV.x, end.y * KV.y, end.z * KV.z);
    char *p = *(char **)&D_00000000E + 0 * (int)(cam); /* agent-c run 209: folded counted use of cam */
    if (p != 0) {
        count = gl_func_00000000(p, 8, dists, surfs, &start, &e, 0, hits);
    }
    int besti = -1;
    if (count != 0) {
        f32 best = 100000.0f;
        for (int i = 0; i < count; i++) {
            if (0.0f < dists[i] && dists[i] < best) {
                best = dists[i];
                besti = i;
            }
        }
        if (besti >= 0) {
            *(V3 *)hits[besti] /= KV;
            *out = *(V3 *)hits[besti];
            return 1;
        }
    }
    return 0;
}
