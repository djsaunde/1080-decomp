/* IDO 7.1 NCC (C++) EXACT donor for SnowChaseActorCam_update (0x5F8 / 382 words), spliced over a placeholder in
 * both build trees (agent-c run 240).
 *
 * agent-c run 240 (99.969 -> 100): the frame comes from the named V3 e (last product), no named references in
 * the key-3 arm, a dead `if (0) { r = f23D4(a0); }` before the matrix read (drops the 8-byte hole above the
 * pooled return slot) and a two-int block `{ int p1 = 0, p2 = 0; }` (puts the 8 bytes below it: slot sp+252).
 * Without the named `const M4 &mm` the two matrix pointers joined edg's first pool temp (v0); reading through
 * an inline `mtx.me()->getPos(v)` gives each its own LR, which takes v1 as in the target.
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
 * The cfe body in game_uso.c stays as the C reference (99.10).
 * agent-g run 475: member names follow include/game/camera.h (names only, same bytes): Self = SnowChaseActorCam; the Out / Node /
 * Body views are the CamMNode bodies (local matrix 0x30, localPos = local[3] at 0x60, world matrix 0x70, worldPos 0xA0). */
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
    const M4 *me() const { return this; }
    void getPos(V3 &v) const { v.x = m[3][0]; v.y = m[3][1]; v.z = m[3][2]; }
};
struct Out { char pad[0x30]; char local[0x30]; V3 localPos; char pad6c[0x34]; V3 worldPos; };
struct Body { char pad[0x70]; M4 world; };
struct Self { char pad[0x14]; Out *body; char pad18[0x2C - 0x18]; V3 eyeOffset; Body *target; Body *source; int mode;
              char pad44[0x7C - 0x44]; float mxz; char pad80[0x94 - 0x80]; float txz; char pad98[0xAC - 0x98]; float cxz;
              char padb0[0xC4 - 0xB0]; float miy; char padc8[0xDC - 0xC8]; float toy; char pade0[0xF4 - 0xE0]; float ty2;
              char padf8[0x10C - 0xF8]; float chy; };
extern "C" {
V3 cam_toTargetLocal_000023D4(Self *a1);
float game_uso_func_070238(float);
void game_uso_func_071028(V3 *);
void game_uso_func_072EE8(void *, V3 *);
}
struct Ctx { Out *out; Ctx(Self *s) { out = s->body; }
    Out *getOut() const { return out; } };
extern "C" void SnowChaseActorCam_update(Self *a0) {
    if (a0->mode == 0) return;
    if (a0->mode == 3) {
        V3 m;
        Ctx c(a0);
        a0->source->world.me()->getPos(m);
        c.getOut()->localPos.set(m);
        c.getOut()->worldPos.set(m);
    } else {
        V3 r;
        V3 e;
        V3 ref;
        V3 self;
        Ctx c(a0);
        V3 acc;
        ref.set(c.getOut()->worldPos);
        if (0) { r = cam_toTargetLocal_000023D4(a0); }
        { int p1 = 0, p2 = 0; }
        a0->target->world.me()->getPos(self);
        r = cam_toTargetLocal_000023D4(a0);
        self.x += r.x; self.y += r.y; self.z += r.z; /* run 179: spelled out (grid) */
        float speed = a0->txz;
        V3 diff(self.x - ref.x, 0.0f, self.z - ref.z);
        float mag = game_uso_func_070238(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
        game_uso_func_071028(&diff);
        if (mag < a0->mxz) {
            acc.put(diff * (mag - a0->mxz));
        } else {
            acc.put(diff * (mag - speed) * a0->cxz);
        }
        game_uso_func_072EE8(c.getOut()->local, &acc);
        a0->eyeOffset.put(diff * speed);
        float sel = (a0->mode == 2) ? a0->ty2 : a0->toy;
        diff.set(0.0f, 1.0f, 0.0f);
        mag = ref.y - self.y;
        if (mag < a0->miy) {
            acc.put(diff * (a0->miy - mag));
        } else {
            acc.put(diff * (sel - mag) * a0->chy);
        }
        game_uso_func_072EE8(c.getOut()->local, &acc);
        e = diff * sel;
        a0->eyeOffset -= e;
    }
}
