/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_00007C1C
 * (1071 words). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM.
 *
 * Steering target in the XZ plane: returns (by value, hidden a0) the offset
 * from *pos to node b (a when b == NULL), widened by +-250 * b->width / 2
 * along the segment normal when the opponent sits ahead (mode 1 / 2), then
 * walks b->next while the next node's offset stays inside the hitA / hitB
 * wedge (3FAC computes the wedge edges).
 *
 * Run 186 (agent-c): rewritten from the cfe emulation into C++ value
 * classes (6A30 recipe): every `bnez &stack; jal 055750(12)` guard is the
 * edg ctor protocol of a derived XZ temporary (XZ(const Vec3 &) /
 * XZ(float, float), base-initialiser forwarding to Vec3(a, b, c)), every
 * word-copy chain through sp+852 / 540 / 508 is an inline by-value return
 * through edg's pooled return slots, and `result = v` is a by-value
 * operator= (copy into the pooled slot, then float stores).
 *  - The loop's direction is ONE full-expression,
 *    `XZ ldir = perp(XZ(n->pos) - b->pos);`: operator-'s return slot (852)
 *    stays busy until the end of the full-expression, so perp returns
 *    through the second pooled slot (540, the target's s2) and the
 *    operator- result is copied into a binding temporary (380). With a
 *    named lseg perp used 852 and s2 never appeared (89.37 -> 94.48, length
 *    1068 -> 1071 = target).
 *  - `x = (const Vec3 &)xzValue` binds operator=(Vec3) to the base
 *    directly; plain `x = xzValue` adds a derived-to-base copy (+9 words).
 *  - The then-arm binds the difference to a reference (94.48 -> 94.51).
 *  - No named notFirst / firstFlag in the loop: `if (!hit || ...)`, then
 *    `if (hit)` before `hit = 1` (same semantics: 3FAC cannot touch the
 *    local). uopt CSEs `hit != 0` / its negation at the loop top and spills
 *    them to the bottom spill area (target sp+84 / 80); the named ints took
 *    an edg home instead (94.51 -> 95.10).
 *  - lever-sweep: the loop threshold written twice instead of a named t2
 *    (95.10 -> 95.12); the first-hit copies in hitB, hitA order (95.12).
 * LEFT: edg's long-lifetime (deferred) temporaries are listed in reverse
 * creation order (then-arm d / nA, else-arm stg / mA / mC2), so several
 * homes are 4-16 bytes off; `a` (target s2) and `out` (target s0) are not
 * kept in callee-saved registers.
 * History (cfe emulation, runs 143-185): 88.2 -> 92.59. */
extern "C" void *game_uso_func_055750(unsigned);
struct Vec3 {
    void *operator new(unsigned n) { return game_uso_func_055750(n); }
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
    Vec3 &operator=(Vec3 v) { x = v.x; y = v.y; z = v.z; return *this; }
    /* agent-c run 201 operator-spelling grid: compound += / -= and a set()-filled operator* result, 95.20 -> 95.24 standalone */
    Vec3 &operator+=(const Vec3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3 &operator-=(const Vec3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vec3 operator*(float s) const { Vec3 r; r.set(x * s, y * s, z * s); return r; }
};
struct XZ : Vec3 {
    XZ(const Vec3 &v) : Vec3(v.x, 0.0f, v.z) {}
    XZ(float a, float c) : Vec3(a, 0.0f, c) {}
};
inline XZ operator-(const XZ &a, const Vec3 &b) { return XZ(a.x - b.x, a.z - b.z); }
inline XZ perp(const Vec3 &v) { return XZ(v.z, -v.x); }
struct Node { char pad0[0x2C]; Node *next; Vec3 pos; char pad3C[0x54 - 0x3C]; float width; };
struct W { char pad0[0xB4]; Vec3 pos; char padC0[0x348 - 0xC0]; float speed; char pad34C[0x908 - 0x34C]; W *other; };
struct Self { char pad0[0x30]; W *w; char pad34[0xA0 - 0x34]; float lim; int active; char padA8[0xCC - 0xA8]; float range; };
extern "C" {
extern char D_00000000;
void game_uso_func_00003FAC(float *, float *, float *, float, float);
float game_uso_func_082880(float);
void game_uso_func_071028(Vec3 *v);
float game_uso_func_00007A98(Self *);
}
extern "C" Vec3 game_uso_func_00007C1C(Self *s, Node *a, Node *b, Vec3 *pos, float *out) {
    Vec3 result;
    if (out != 0) {
        *out = 0.0f;
    }
    if (b == 0) {
        const Vec3 &d = XZ(a->pos) - *pos;
        result.y = 0.0f;
        result.z = d.z;
        result.x = d.x;
    } else {
        int mode = 0;
        Node *a_al = a; /* agent-c run 195: alias copy (s-reg tie) */
        Vec3 seg = XZ(b->pos) - a_al->pos;
        if (s->w->other != 0) {
            float dot = game_uso_func_00007A98(s);
            int active = s->active;
            if (active == 0) {
                if (s->lim <= dot) {
                    active = 1;
                    s->active = 1;
                }
            } else if (dot <= -s->lim) {
                s->active = 0;
                active = 0;
            }
            if (active == 1 && dot <= *(float *)((char *)&D_00000000 + 0xFC)) {
                XZ e = XZ(s->w->other->pos) - a->pos;
                mode = (seg.x * e.z <= seg.z * e.x) ? 2 : 1;
            }
        }
        XZ dir = perp(seg);
        game_uso_func_071028(&dir);
        XZ rd = XZ(b->pos) - *pos;
        result = (const Vec3 &)rd;
        if (mode == 1) {
            result += dir * 250.0f * b->width * 0.5f;
        } else if (mode == 2) {
            result -= dir * 250.0f * b->width * 0.5f;
        }
        float metric = game_uso_func_082880(rd.x * rd.x + rd.z * rd.z);
        float thresh = b->width * 250.0f;
        Vec3 hitA, hitB;
        int hit = 0;
        if (thresh < metric) {
            game_uso_func_00003FAC(&hitA.x, &hitB.x, &rd.x, metric, thresh);
            hit = 1;
        }
        Vec3 hitC;
        Node *n = b->next;
        while (n != 0) {
            XZ ldir = perp(XZ(n->pos) - b->pos);
            game_uso_func_071028(&ldir);
            XZ lrd = XZ(n->pos) - *pos;
            hitC = (const Vec3 &)lrd;
            if (mode == 1) {
                hitC += ldir * 250.0f * n->width * 0.5f;
            } else if (mode == 2) {
                hitC -= ldir * 250.0f * n->width * 0.5f;
            }
            if (!hit ||
                ((hitC.x * hitA.z <= hitC.z * hitA.x) &&
                 (hitC.z * hitB.x <= hitC.x * hitB.z))) {
                float m2 = game_uso_func_082880(lrd.x * lrd.x + lrd.z * lrd.z);
                if (n->width * 250.0f < m2) {
                    Vec3 fA, fB;
                    game_uso_func_00003FAC(&fA.x, &fB.x, &lrd.x, m2, n->width * 250.0f);
                    if (hit) {
                        if (hitA.z * fA.x < hitA.x * fA.z) {
                            hitA = fA;
                        }
                        if (hitB.x * fB.z < hitB.z * fB.x) {
                            hitB = fB;
                        }
                    } else {
                        hitA = fA;
                        hitB = fB;
                    }
                    hit = 1;
                }
                result = hitC;
                b = n;
                n = n->next;
                continue;
            }
            break;
        }
        if (n != 0 &&
            game_uso_func_082880(result.x * result.x + result.z * result.z) <
                s->w->speed * s->range) {
            Vec3 n0 = hitC;
            game_uso_func_071028(&n0);
            Vec3 n1 = hitA;
            game_uso_func_071028(&n1);
            Vec3 n2 = hitB;
            game_uso_func_071028(&n2);
            if ((n0.x * n2.x + n0.z * n2.z) < (n0.x * n1.x + n0.z * n1.z)) {
                result = hitA;
            } else {
                result = hitB;
            }
        }
    }
    return result;
}
