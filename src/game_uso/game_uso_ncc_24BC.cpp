/* IDO 7.1 NCC (C++) donor for SnowChaseActorCam_applyMode_000024BC (0x258 / 150 words).
 *
 * Camera-homing update, sibling of 28C0 / 2CC8 / 1DDC (same V3 class kit):
 * key 3 snaps out->goal and out->pos to the source body's matrix
 * translation; otherwise q = obj->pos + 023D4(this) (by-value return),
 * n = normalize(XZ of the input vector) * spd, q -= n, q.y += height (key 2
 * selects 0xF4, else 0xDC), q -= in / (sx, sy, sx), and goal = pos = q.
 *
 * agent-g run 119 rewrite (cfe 72%), finished in run 129 (118 -> 0 raw):
 *  - every out access is `Out *o = c.getOut(); ...; o = c.getOut(); ...`
 *    through an inline Ctx accessor (the accessor result is an edg
 *    temporary: the target's v0 reloads; getPos's this stays v1). The tail
 *    writes through Out::setG / setP members, inside a nested block so its
 *    `o` is allocated below the 023D4 return temp (sp+60 as the target).
 *  - no named `key`: a named int takes a slot and pushes m 4 bytes down.
 *  - the key-3 arm's dead `int zz` between its two store groups and n / s at
 *    the top of the else arm give frame 152 and every target slot; s is
 *    filled by three separate component stores (sx is loaded twice).
 *  - one empty if after `q += r` ends a block, so a2 is reloaded after the
 *    first call and re-saved before the second, as in the target.
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe).
 * agent-g run 475: member names follow include/game/camera.h (names only, same bytes): Self = SnowChaseActorCam; the Out / Node /
 * Body views are the CamMNode bodies (local matrix 0x30, localPos = local[3] at 0x60, world matrix 0x70, worldPos 0xA0). */
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    V3 &operator+=(const V3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    V3 &operator-=(const V3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    void operator*=(float s) { x *= s; y *= s; z *= s; }
};
struct M4 {
    float m[4][4];
    void getPos(V3 &v) const { v.x = m[3][0]; v.y = m[3][1]; v.z = m[3][2]; }
};
struct Out { char pad[0x60]; V3 localPos; char pad6c[0x34]; V3 worldPos;
    void setG(const V3 &v) { localPos.x = v.x; localPos.y = v.y; localPos.z = v.z; }
    void setP(const V3 &v) { worldPos.x = v.x; worldPos.y = v.y; worldPos.z = v.z; } };
struct Body { char pad[0x70]; M4 world; };
struct Self { char pad[0x14]; Out *body; char pad18[0x38 - 0x18]; Body *target; Body *source; int mode;
              char pad44[0x94 - 0x44]; float txz; char pad98[0xAC - 0x98]; float cxz;
              char padb0[0xDC - 0xB0]; float toy; char pade0[0xF4 - 0xE0]; float ty2;
              char padf8[0x10C - 0xF8]; float chy; };
extern "C" {
V3 cam_toTargetLocal_000023D4(Self *a1);
void game_uso_func_071028(V3 *);
}
struct Ctx { Out *out; Ctx(Self *s) { out = s->body; }
    Out *getOut() const { return out; } };
extern "C" void SnowChaseActorCam_applyMode_000024BC(Self *a0, V3 *in) {
    Ctx c(a0);
    if (a0->mode == 0) return;
    if (a0->mode == 3) {
        V3 m;
        a0->source->world.getPos(m);
        Out *o = c.getOut(); o->localPos.x = m.x; o->localPos.y = m.y; o->localPos.z = m.z;
        int zz; zz = 0;
        o = c.getOut(); o->worldPos.x = m.x; o->worldPos.y = m.y; o->worldPos.z = m.z;
    } else {
        V3 r;
        V3 q;
        V3 n;
        V3 s;
        a0->target->world.getPos(q);
        r = cam_toTargetLocal_000023D4(a0);
        q += r;
        if (1) {}
        n.set(in->x, 0.0f, in->z);
        game_uso_func_071028(&n);
        n *= a0->txz;
        q -= n;
        q.y += a0->mode == 2 ? a0->ty2 : a0->toy;
        s.x = in->x / a0->cxz; s.y = in->y / a0->chy; s.z = in->z / a0->cxz;
        q -= s;
        { Out *o = c.getOut(); o->setG(q);
        o = c.getOut(); o->setP(q); }
    }
}
