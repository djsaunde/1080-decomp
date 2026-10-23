/* IDO 7.1 NCC (C++) donor for func_00004948 (camera transition tick, 0x6A4 + an
 * all-zero pad word). EXACT (agent-c run 125).
 * - Vec3 has no user copy ctor (a user copy ctor adds cfront's this==0 new
 *   protocol to every return); operators build `Vec3 r(x, y, z); return r;`
 *   = r -> return temp (sp+260, shared) -> destination copy ladder;
 * - the float ctor forwards to an inline set(a, b, c): edg allocates its
 *   ctor `this`/parameter temps in the order that puts the divisor-call temp of
 *   section 3 at sp+232 and colours the shared x/y/z temps f14/f2/f12 (run 124's
 *   dead `if (0) { x = a; }` + a declared `E16 *ep;` gave the same colours but
 *   left the divisor at sp+236);
 * - Node::get(Vec3 &) member copies = the interleaved lwc1/swc1 C/E fills;
 * - case-order `switch` = the edg compare chain (no jumptable);
 * - frame: B before sv in the snap macro; the recpos() reference temp in
 *   section A places the slots.
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. */
struct Vec3 {
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
};
inline Vec3 operator*(const Vec3 &a, float k) { Vec3 r(a.x * k, a.y * k, a.z * k); return r; }
inline Vec3 operator-(const Vec3 &a, const Vec3 &b) { Vec3 r(a.x - b.x, a.y - b.y, a.z - b.z); return r; }
struct Node { char pad[0x30]; Vec3 pos; void get(Vec3 &v) { v.x = pos.x; v.y = pos.y; v.z = pos.z; } };
struct Ob { char pad[0x70]; Node node; };
inline Vec3 &recpos(char *o) { return *(Vec3 *)(*(char **)(o + 0x154) + 0x318); }
struct E16 { float lo, hi, v, w; };
struct E8 { float a, b; };
extern "C" {
void func_00000000(...);
float bu4948_ff(float);
extern char func_000003F8[];
extern E16 bu_4948_t16[];
extern E8 bu_4948_t8[];
extern float bu_4948_k0, bu_4948_k1, bu_4948_klo, bu_4948_khi;
}
#define F(p,o) (*(float *)((char *)(p) + (o)))
#define W(p,o) (*(unsigned *)((char *)(p) + (o)))
#define SNAP() { Vec3 *sv = (Vec3 *)(*(char **)(obj + 0x154) + 0x318); Vec3 B; B = *sv * bu_4948_k0; B.y *= bu_4948_k1; func_00000000(obj + 0x30, &B); }
extern "C" void func_00004948(char *obj) {
#define st W(obj, 0x158)
    if (st == 2) {
        Vec3 B;
        B = recpos(obj) * bu_4948_k0;
        B.y *= bu_4948_k1;
        func_00000000(obj + 0x30, &B);
        F(obj, 0x14C) = F(obj, 0x150);
    } else if ((st >= 0x13 && st < 0x18) || (st >= 0x18 && st < 0x1D)) {
        Vec3 d, C, E;
        float dist;
        int idx;
        (*(Ob **)(obj + 0xF4))->node.get(C); ((Ob *)obj)->node.get(E);
        d = C - E;
        dist = bu4948_ff(d.x * d.x + d.y * d.y + d.z * d.z);
        idx = 0;
        switch (W(obj, 0x158)) {
        case 0x13: case 0x18: idx = 0; break;
        case 0x14: case 0x19: idx = 1; break;
        case 0x15: case 0x1A: idx = 2; break;
        case 0x16: case 0x1B: idx = 3; break;
        case 0x17: case 0x1C: idx = 4; break;
        }
#define e (&bu_4948_t16[idx])
        if (e->lo < dist) F(obj, 0x14C) = e->w;
        else if (dist < e->hi) F(obj, 0x14C) = e->v;
        else F(obj, 0x14C) = e->w + (e->v - e->w) / (e->lo - e->hi) * (e->lo - dist);
        if (st >= 0x18 && st < 0x1D) SNAP();
    } else if ((st >= 0x1D && st < 0x22) || (st >= 0x22 && st < 0x27)) {
        Vec3 d, C, E;
        float dist;
        int idx;
        float ang, r, t;
        (*(Ob **)(obj + 0xF4))->node.get(C); ((Ob *)obj)->node.get(E);
        d = C - E;
        dist = bu4948_ff(d.x * d.x + d.y * d.y + d.z * d.z);
        idx = 0;
        switch (W(obj, 0x158)) {
        case 0x1D: case 0x22: idx = 0; break;
        case 0x1E: case 0x23: idx = 1; break;
        case 0x1F: case 0x24: idx = 2; break;
        case 0x20: case 0x25: idx = 3; break;
        case 0x21: case 0x26: idx = 4; break;
        }
#undef e
#define e (&bu_4948_t8[idx])
        ang = e->b / 2 * F(func_000003F8, 0x15C) / 180.0f;
        r = bu4948_ff(ang) * e->a / bu4948_ff(ang);
        t = bu4948_ff(dist * dist + r * r);
        t = bu4948_ff(dist / t) * 2 / F(func_000003F8, 0x160) * 180.0f;
        if (t < bu_4948_klo) F(obj, 0x14C) = bu_4948_klo; else if (bu_4948_khi < t) F(obj, 0x14C) = bu_4948_khi; else F(obj, 0x14C) = t;
        if (st >= 0x22 && st < 0x27) SNAP();
    } else {
        F(obj, 0x14C) = F(obj, 0x150);
    }
    func_00000000(obj + 0x30);
    func_00000000(obj);
}
