/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0004F2DC (0x428 / 266 words), agent-g run 194.
 * Screen-point -> world raycast picker as C++ (kit CXLVI lead: the target re-reads the ray direction from memory
 * for the K* products = by-reference inline operators). The ray direction is built member by member, rotated
 * and normalised in place by two callees, then `d = dir` through a by-value operator= (the target's word copy
 * 200 -> 180 then float stores 212..220), `start + d * far` through the by-value operator* / operator+ return
 * and parameter temporaries (160 -> 180 -> 412, 112 -> 132 -> 84), `start *= kvec` in place and the scaled end
 * point through the ctor (no temporary). `if (1) {}` after `start *= kvec` (lever-sweep bbsplit) keeps the
 * kvec loads after the in-place products. cfe body 90.36 -> 94.78 standalone, length = target (1064).
 * agent-c run 307: EXACT (named operator* result via set(), pads in operator+ and before p, no code-free test before
 * the getPos call). Spliced over a placeholder on both build paths. Blank-USO data refs carry in-place addends:
 * D_4F2DC_b / D_4F2DC_e = 0, D_00000000F = 0x128 (kvec), the donor .rodata (-1e5 / 1e5) renamed
 * game_libs_func_0004F2DC_rodata by the splice and pinned to the USO RoData at 0x1BA8. */
typedef float f32;
extern "C" {
int gl_func_00000000(...);
extern int D_4F2DC_b;
extern int D_4F2DC_e;
extern f32 D_00000000F[3];
}
template <int K> struct PadK { int a[K]; PadK() {} }; /* agent-c run 289: unreferenced frame pad (a class with a ctor keeps its home) */
template <class T> inline T idv(T v) { return v; } /* agent-g run 303: zero-code by-value identity (kit CCCLXXX) */
struct V3 { f32 x, y, z;
    V3() {}
    V3(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; } /* agent-c run 290: the ctor's body for the early-declared e */
    void operator=(V3 v) { x = v.x; y = v.y; z = v.z; }
    V3 operator*(f32 s) const { V3 r; r.set(x * s, y * s, z * s); return r; } /* agent-c run 307: named result
        filled through set() (the literal then loads into f14 like the target; frame -24 restored by the block pad
        before p below), 97.489 -> 97.752 */
    V3 operator+(const V3 &o) const { PadK<2> pr; return V3(x + o.x, y + o.y, z + o.z); } /* agent-c run 307: an
        unreferenced 8-byte pad in the inline (the end temporary drops to the target's sp+84; the block pad below
        shrinks to 4 words), 97.752 -> 98.534. agent-g run 303: all three sums this-first
        (the target's add.s operand order once start.x stays in f18). agent-c run 289: by const reference -- the
        * operand temporary bound to it is a full-expression temporary, homed at the frame top (the target's 412);
        * then the x / y sums operand-first (8-way order grid, 95.733 -> 95.771) */
    void operator*=(const V3 &k) { x *= k.x; y *= k.y; z *= k.z; } /* agent-g run 303: x, y, z (with e *= KV below; earlier
        order sweeps: agent-g run 245, agent-c run 276) */
    void operator/=(const V3 &k) { x /= k.x; y /= k.y; z /= k.z; }
};
#define KV (*(V3 *)D_00000000F) /* collision kvec (&D + 0x128) */
/* agent-g run 303: the camera position is the translation row of an M4 at cam + 0x70 (game_uso 3558's Body::mtx.getPos
 * shape: the target computes this = cam + 0x70, `addiu v1,v1,112`); the `if (1) {}` between x and y keeps the y / z
 * reads off the folded cam-relative addresses */
struct M4 { f32 m[4][4]; void getPos(V3 &v) const { v.x = m[3][0]; if (1) {} v.y = m[3][1]; v.z = m[3][2]; } };
struct Cam { char p[0x70]; M4 m; char pac[0xB4 - 0xB0]; char mtx[1]; };
extern "C" int game_libs_func_0004F2DC(char *arg0, int x, int y, V3 *out) {
    /* agent-c run 280 (declaration-order grid, 120): count and padt FIRST -- dists / surfs / hits then sit at the
     * target's 376 / 344 / 248 (were 392 / 360 / 264); `count = 0;` must stay right after count's declaration (after
     * the V3 padt ctor it costs 4.7 points), 95.692 -> 95.714 */
    int count; /* agent-g run 195 / 224 placed it below the arrays (pre run-280 order) */
    count = 0;
    /* agent-c run 278 / 280: an empty-ctor `V3();` temporary here stood in for the frame-top word block; agent-c run 289:
     * that block is operator+'s const-reference operand temporary (see V3), so the stand-in is gone */
    f32 dists[8];
    int surfs[8];
    f32 hits[8][3];
    V3 start; /* agent-c run 290 (kit CCCLII home order, -g3 names): start / e declared right after hits -- start M-188,
               * e M-200, d M-212, dir M-224 = the target's homes; the early declarations need a code-free statement at
               * start's old position (the pz pad below, or `if (1) {}`) or `d * k` reads the copied registers (1040 / 1052
               * bytes): 95.7707 -> 95.7932 (start) -> 95.8421 (e through set()) */
    V3 e;
    V3 d; /* agent-g run 224 (decl-move sweep): declared before dir, 95.46 -> 95.67 */
    V3 dir;
    dir.x = -((f32)x - 160.0f); dir.y = (f32)y - 120.0f; dir.z = 8.0f / *(f32 *)((char *)&D_4F2DC_b + 0x128);
    gl_func_00000000(&dir, (*(Cam **)(arg0 + 0x70))->mtx);
    gl_func_00000000(&dir);
    d = dir;
    /* agent-g run 297: the product reads d through a const reference (zero code): uopt no longer forwards the
     * operator= parameter temp's words to the `d * k` reads, which now load d's own home (sp+212..220) like the
     * target (operator= return / pointer / reference forms, 18): 95.8421 -> 95.8534 */
    /* agent-g run 303: the run-297 `const V3 &dr = d` view and the named cam dropped (stale after the e = ... form); the
     * getPos receiver is the rvalue *(Cam **)(arg0 + 0x70) (kit CCCLXXXV): the target's dead `addiu v1,v1,112` this-temp
     * appears after the three cam-relative loads; besti declared in the count block. 97.365 -> 97.474 */
    /* agent-g run 303: was agent-c run 289's PadK<4> pz frame pad (no longer needed: frame 424 = target); agent-c run 307:
     * the run-303 `if (1) {}` here dropped -- with the operator+ pad the cam pointer then takes v1 and the dead this-temp
     * `addiu v1,v1,112` follows the three loads like the target, 98.534 -> 100 */
    (*(Cam **)(arg0 + 0x70))->m.getPos(start);
    /* agent-g run 303: the target's `end` (sp+84, below the operator temporaries) is e's by-value operator= parameter --
     * e = start + dr * -1e5 (bootup.uso RoData 0x1BA8 / 0x1BAC are the literals -1e5 / 1e5), start *= KV, then e *= KV
     * (same values as e.set(KV.x * end.x, ...)): start.x stays in f18 across the add and the scale like the target,
     * 95.853 -> 97.365, length 1064 = target */
    e = start + d * idv(-100000.0f); /* agent-g run 303: zero-code identity on the literal (expression-form sweep), 97.474 -> 97.489 */
    start *= KV;
    e *= KV;
    { PadK<4> pq; } /* agent-c run 307: unreferenced block-scope frame pad (frame 424 = target with the named operator* result) */
    char *p = *(char **)&D_4F2DC_e; /* agent-c run 287: run 209's folded cam use dropped (stale: instruction stream identical, 95.72) */
    if (p != 0) {
        count = gl_func_00000000(p, 8, dists, surfs, &start, &e, 0, hits);
    }
    if (count != 0) {
        int besti = -1;
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
