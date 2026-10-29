/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_00065494 (0x538 / 334 words):
 * the per-frame contact solve over the self->0x39C child list. NM object only
 * (build/non_matching); the ROM keeps INCLUDE_ASM. agent-g run 186: whole-function
 * C++ port of the cfe NM body (80.62): the {cur, nxt} list iterator with an if/return
 * next(), Vec3 value class with the by-value operator= (word copy into the pooled
 * parameter temp, then float stores), named-result operator- / operator* / dot, the
 * impulse passed as a const Vec3 & (bound temp at sp+0x9C), the two float-bit call
 * arguments as int (target `lui a2,0x43C8` / `lui a3,0x3F80`: a float parameter
 * reuses the hoisted 1.0 / 400.0 registers), the normal's zeros as `(float)iz()`
 * (a distinct uopt constant: the target keeps that zero in fs1 apart from the fs2
 * accumulator zero), h / w / dt / f as function-scope named locals (target homes:
 * n 0x134, d 0x128, hit 0xF8; after the set() step below they went back to their
 * blocks, 89.91 -> 90.09), and `psum += v * w` before `wsum += w` (lever-sweep
 * order, 80.88 -> 86.37). Then operator- and the impulse scale build their named
 * result through the inline set(): set's parameters are evaluated before any
 * store into r, so all the field loads / subs / muls come first as in the target
 * (86.37 -> 89.91; a float-temp spelling also reorders them but drops two words).
 * agent-c run 213: four trailing `if (1) {}` blocks leave self + 0x2FC uncoloured
 * (spill home 0x6C, no s8) as in the target; registers then line up (91.68 -> 95.29),
 * and operator* returning `Vec3(x * s, y * s, z * s)` evaluates all three products
 * before the stores into the by-value temp, as the target (95.29 -> 98.54, 1336 =
 * target length).
 * agent-c run 214: frame 392 = target with every named home exact: operator= returns
 * Vec3 & (frame 384 -> 392 with a named w), loop-block scalars h / w / dt declared
 * first then n / d / hit (n at 0x134, d 0x128, hit 0xF8), f at function scope (one
 * more outer home); the 3-float ctor forwarding to set() fixes the s1 / s2 / s3
 * permutation. Left: edg temp homes below hit, FP colours of w / dt. */
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float a, float b, float c) { set(a, b, c); }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); } /* run 213: ctor form, products before stores */
    Vec3 sc(float s) const { Vec3 r; r.set(x * s, y * s, z * s); return r; }
    void operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; }
    void operator-=(const Vec3 &v) { x -= v.x; y -= v.y; z -= v.z; }
    void operator*=(float s) { x *= s; y *= s; z *= s; }
    void operator/=(float s) { x /= s; y /= s; z /= s; }
    float dot(const Vec3 &v) const {  float r = x * v.x + y * v.y + z * v.z; return r;  }
};
struct V3P { float x, y, z; };
inline int iz() { return 0; }
struct Hit { V3P v; float d; short idx; short pad0; int pad1[7]; };
struct Node;
struct NB { /* run 214: a real virtual call (vptr 0x28, slot 12 = vtbl +0x60) gives addu a0,t2,s0 */
    char pad0[0x28];
    virtual void f1();
    virtual void f2();
    virtual void f3();
    virtual void f4();
    virtual void f5();
    virtual void f6();
    virtual void f7();
    virtual void f8();
    virtual void f9();
    virtual void f10();
    virtual void f11();
    virtual void contact(float, Vec3 *, int);
};
struct Node : NB {
    char pad2C[0xDC - 0x2C]; Vec3 pos;
    char padE8[0x114 - 0xE8]; Vec3 v114; Vec3 v120;
    char pad12C[0x144 - 0x12C]; float f144; int pad148; int idx;
};
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Node *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->obj; } return 0; }
};
struct Self {
    char pad0[0xDC]; Vec3 pos;
    char padE8[0x218 - 0xE8]; float f218;
    char pad21C[0x288 - 0x21C]; float f288; float f28C; float f290;
    char pad294[0x2FC - 0x294]; char f2FC[0x318 - 0x2FC]; Vec3 v318;
    char pad324[0x39C - 0x324]; Link *list;
};
extern "C" {
extern int D_00000000[];
void gl_func_00062F64(void *, Vec3 *, Vec3 *);
int gl_func_00062F64_b(int, Vec3 *, int, Hit *);
void gl_func_00062F64_d(Self *, Vec3 *, const Vec3 &);
void gl_func_00062F64_e(Self *, Vec3 *, Vec3 *);
}
extern "C" void gl_func_00065494(Self *self) {
    Iter it(self->list);
    Vec3 psum;
    Vec3 nsum;
    int count;
    float wsum;
    float minv;
    Node *node;
    float f;

    count = 0;
    nsum.x = 0.0f; nsum.y = 0.0f; nsum.z = 0.0f;
    psum.x = 0.0f; psum.y = 0.0f; psum.z = 0.0f;
    wsum = 0.0f; /* run 186: accumulator zeros after the vector zeros (120-order grid, 90.02 -> 91.61) */
    minv = 0.0f;
    while ((node = it.next()) != 0) {
        float h;
        float w;
        float dt;
        Vec3 n;
        Vec3 d;
        Hit hit;
        n.x = (float)iz(); n.y = 1.0f; n.z = (float)iz();
        node->v120 = node->pos;
        d = node->pos - self->pos;
        gl_func_00062F64(self->f2FC, &d, &node->v114);
        node->v114 -= d;
        node->v114 += self->v318;
        if (gl_func_00062F64_b(D_00000000[0], &node->v120, 0x43C80000, &hit) != 0) {
            node->idx = hit.idx;
            n = *(Vec3 *)&hit.v;
            h = hit.d;
        } else {
            h = node->v120.y;
            n.set((float)iz(), 1.0f, (float)iz());
        }
        h -= self->f218;
        node->f144 = h;
        node->contact(h, &n, 0x3F800000);
        if (h < 0.0f) {
            if (h < minv) {
                minv = h;
            }
            nsum += n;
            count++;
            w = -h;
            psum += node->v120 * w;
            wsum += w;
            dt = node->v114.dot(n);
            if (dt > 0.0f) {
                dt = 0.0f;
            }
            { gl_func_00062F64_d(self, &node->v120, n.sc(w * self->f290 + self->f28C * -dt)); } /* run 214: own block homes its bound temp below the += one */
        }
    }
    if (count != 0) {
        f = (float)count;
        nsum /= f; /* run 214: operator/= gives the target's y / x / z division schedule */
        psum /= wsum;
        nsum *= -minv * self->f288;
        gl_func_00062F64_e(self, &psum, &nsum);
    }
    if (1) {} if (1) {} if (1) {} if (1) {} /* agent-c run 213: four empty blocks raise the BB count so the
                                                * self + 0x2FC address stays uncoloured (spill home 0x6C, no s8),
                                                * as in the target (91.68 -> 95.29 standalone) */
}
