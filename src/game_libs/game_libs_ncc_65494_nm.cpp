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
 * accumulator zero), h / w / dt as function-scope named locals (target homes:
 * n 0x134, d 0x128, hit 0xF8), and `psum += v * w` before `wsum += w` (lever-sweep
 * order, 80.88 -> 86.37). Then operator- and the impulse scale build their named
 * result through the inline set(): set's parameters are evaluated before any
 * store into r, so all the field loads / subs / muls come first as in the target
 * (86.37 -> 89.91; a float-temp spelling also reorders them but drops two words).
 * Left: the target leaves self + 0x2FC uncoloured (spill home 0x6C, no s8; ours
 * colours it s8: +1 save word) and keeps psum's product loads ahead of its stores
 * (the set() form on that operator* does not move them). */
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float a, float b, float c) { x = a; y = b; z = c; }
    void operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3 operator-(const Vec3 &v) const { Vec3 r; r.set(x - v.x, y - v.y, z - v.z); return r; }
    Vec3 operator*(float s) const { Vec3 r; r.x = x * s; r.y = y * s; r.z = z * s; return r; }
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
struct VEnt { short off; short pad; void (*fn)(Node *, float, Vec3 *, int); };
struct Node {
    char pad0[0x28]; VEnt *vt;
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
    float h;
    float w;
    float dt;

    count = 0;
    wsum = 0.0f;
    minv = 0.0f;
    nsum.z = 0.0f; nsum.y = 0.0f; nsum.x = 0.0f;
    psum.z = 0.0f; psum.y = 0.0f; psum.x = 0.0f;
    while ((node = it.next()) != 0) {
        Vec3 n;
        Vec3 d;
        Hit hit;
        n.set((float)iz(), 1.0f, (float)iz());
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
        node->vt[0xC].fn((Node *)(node->vt[0xC].off + (char *)node), h, &n, 0x3F800000);
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
            gl_func_00062F64_d(self, &node->v120, n.sc(self->f290 * w + self->f28C * -dt));
        }
    }
    if (count != 0) {
        float f = (float)count; /* block-local again after the set() step (89.91 -> 90.02) */
        nsum.y /= f; nsum.x /= f; nsum.z /= f;
        psum /= wsum;
        nsum *= -minv * self->f288;
        gl_func_00062F64_e(self, &psum, &nsum);
    }
}
