/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_00001DDC (0x5F8 / 382 words; this body 382 = target).
 * NM object only; the ROM build keeps INCLUDE_ASM.
 *
 * agent-g run 168 (docs/IDO_CODEGEN.md kit CXI): the 24BC / 28C0 sibling written as real C++ --
 * the same Self layout, the inline Ctx accessor (a stack object holding a0->out, re-read for every
 * use: the target's `sw t7,300(sp)` / `lw v0,300(sp)` home), M4::getPos for the two transform
 * positions (the `addiu v1,v1,112` + 52 / 56 loads), and V3 operator* returning by value (ctor
 * temp -> pooled return slot -> by-value operator= parameter -> float copies into acc: the target's
 * three-hop copy chains). operator= forwarding to set(const V3 &) gives the target frame (384).
 * The two homes above ref are NAMED V3s declared first in the arm (r, e) assigned through the
 * IMPLICIT memberwise operator= (word copies from the return slot: `r = f23D4(a0); self += r;`,
 * `e = diff * sel; a0->vel -= e;`); acc / vel take the products through a by-value put() that
 * forwards to set(const V3 &) (the parameter-temp hop + the frame-384 reference slots).
 * LEFT (99.77): the key-3 source / non-3 object pointers take v0 where the target has v1, and the
 * f23D4 return slot sits 8 bytes lower (the ref read's pointer temps are allocated before it).
 * The cfe body in game_uso.c stays as the C reference (99.10). */
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void put(V3 v) { set(v); }
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    V3 &operator-=(const V3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    V3 operator*(float s) const { return V3(x * s, y * s, z * s); }
};
struct M4 {
    float m[4][4];
    void getPos(V3 &v) const { v.x = m[3][0]; v.y = m[3][1]; v.z = m[3][2]; }
};
struct Out { char pad[0x30]; char acc[0x30]; V3 goal; char pad6c[0x34]; V3 pos; };
struct Body { char pad[0x70]; M4 mtx; };
struct Self { char pad[0x14]; Out *out; char pad18[0x2C - 0x18]; V3 vel; Body *obj; Body *src; int key;
              char pad44[0x7C - 0x44]; float lim; char pad80[0x94 - 0x80]; float spd; char pad98[0xAC - 0x98]; float sx;
              char padb0[0xC4 - 0xB0]; float vlim; char padc8[0xDC - 0xC8]; float hA; char pade0[0xF4 - 0xE0]; float hB;
              char padf8[0x10C - 0xF8]; float sy; };
extern "C" {
V3 game_uso_func_000023D4(Self *a1);
float game_uso_func_070238(float);
void game_uso_func_071028(V3 *);
void game_uso_func_072EE8(void *, V3 *);
}
struct Ctx { Out *out; Ctx(Self *s) { out = s->out; }
    Out *getOut() const { return out; } };
extern "C" void game_uso_func_00001DDC(Self *a0) {
    if (a0->key == 0) return;
    if (a0->key == 3) {
        V3 m;
        Ctx c(a0);
        /* run 179 (spelling grid): the matrix read through a const M4 & block
         * local and the pos copy as set(m) give every register (99.77 -> 99.92);
         * left: the two reference homes put the frame 8 bytes over the target */
        { const M4 &mm = a0->src->mtx; mm.getPos(m); }
        { Out *o = c.getOut(); o->goal.x = m.x; o->goal.y = m.y; o->goal.z = m.z; }
        c.getOut()->pos.set(m);
    } else {
        V3 r;
        int e = 0; /* run 179: unused 4-byte home in place of V3 e (frame 0x188 -> 0x180 = target) */
        V3 ref;
        V3 self;
        Ctx c(a0);
        V3 acc;
        ref.set(c.getOut()->pos);
        { const M4 &mm = a0->obj->mtx; mm.getPos(self); }
        r = game_uso_func_000023D4(a0);
        self.x += r.x; self.y += r.y; self.z += r.z; /* run 179: spelled out (grid) */
        float speed = a0->spd;
        V3 diff(self.x - ref.x, 0.0f, self.z - ref.z);
        float mag = game_uso_func_070238(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        game_uso_func_071028(&diff);
        if (mag < a0->lim) {
            acc.put(diff * (mag - a0->lim));
        } else {
            acc.put(diff * (mag - speed) * a0->sx);
        }
        game_uso_func_072EE8(c.getOut()->acc, &acc);
        a0->vel.put(diff * speed);
        float sel = (a0->key == 2) ? a0->hB : a0->hA;
        diff.set(0.0f, 1.0f, 0.0f);
        mag = ref.y - self.y;
        if (mag < a0->vlim) {
            acc.put(diff * (a0->vlim - mag));
        } else {
            acc.put(diff * (sel - mag) * a0->sy);
        }
        game_uso_func_072EE8(c.getOut()->acc, &acc);
        acc = diff * sel; /* the last product reuses acc (was V3 e) */
        a0->vel -= acc;
    }
}
