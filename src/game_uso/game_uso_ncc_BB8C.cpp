/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_0000BB8C (0x3F0 / 252 words).
 *
 * Contact-response sweep (sibling of the 1DDC / 28C0 camera family): probe
 * ray q = {dir (0, 0, -1), pos = sub matrix translation}, rotated by
 * 070F38; world = dir * 1000 (+ body translation * scale, component-wise),
 * norm = world - q.pos normalised; d = -dir.norm.  When d passes obj->lo the
 * two clamped ramps f24 / f20 feed the 05B750 / 05B614 effect calls and the
 * per-child 0000B750 loop.
 *
 * agent-g run 128 rewrite from scratch (the cfe NM body was 287 raw words):
 * the 192 -> 204 -> 164 word-copy chains of the target are edg's operator
 * temporaries: V3 operator* / operator- returning V3(...) (result temp ->
 * return temp) and a BY-VALUE void operator=(V3 v) (return temp -> param
 * temp -> float stores).  q / Q stay POD (a V3 member with a user default
 * ctor brings the this == 0 -> new protocol); k is ONE double variable
 * loaded with the 0xFA90 constant in the compare and the 0xFA98 constant
 * before the loop (target f22); the dot product is an inline member (moves
 * d to f2/f26 as the target); d / k / i / c are function-scope scalars
 * (frame 296); two empty if (1) {} inside getPos.
 * RESIDUAL (72 raw): the getPos this-adjust (target lwc1 160(a2); addiu
 * a2,a2,112; 52/56(a2)), the float zero hoisted above the K88 return, the
 * f20 / k colours swapped (f22 / f20), the operator= param temp 8 bytes low
 * and the P2 record 4 bytes low.
 * Spliced into the NON_MATCHING object only; the ROM keeps INCLUDE_ASM. */
struct P3 { float x, y, z; };
struct V3 {
    float x, y, z;
    V3() {}
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    float dot(const V3 &v) const { return x * v.x + y * v.y + z * v.z; }
    void operator=(V3 v) { x = v.x; y = v.y; z = v.z; }
    V3 operator*(float s) const { return V3(x * s, y * s, z * s); }
    V3 operator-(const V3 &v) const { return V3(x - v.x, y - v.y, z - v.z); }
};
struct M4 {
    float m[4][4];
    void getPos(P3 &v) const { v.x = m[3][0]; if (1) {} v.y = m[3][1]; v.z = m[3][2]; if (1) {} }
};
struct Body { char pad[0x70]; M4 mtx; };
struct Sub { char pad[0x70]; M4 mtx; char padb0[0xC8 - 0xB0]; float k; };
struct Glob { char pad[0x70]; Body *body; char pad74[0xB8 - 0x74]; int a, b, c, d; };
struct Ray { P3 dir; P3 pos; };
struct Child { char pad[0xB8]; char rec[0x1C]; int flags; float lim; };
struct Obj { char pad[0xB4]; int b4; char padb8[0x220 - 0xB8]; Sub *sub; char pad224[0x23C - 0x224]; float lo; char pad240[0x254 - 0x240]; float k2; char pad258[0x26C - 0x258]; float lo2; char pad270[0x274 - 0x270]; unsigned int n; };
struct Q { P3 v; float s; };
struct P2 { int a, b; };
extern "C" {
extern Glob *import_80087DA8;
extern char import_8005C108[];
extern double game_uso_D_807FFA88[], game_uso_D_807FFA90[], game_uso_D_807FFA98[];
void game_uso_func_070F38(Ray *, void *);
void game_uso_func_071028(V3 *);
void game_uso_func_05B750(void *, Q *);
void game_uso_func_05B614(void *, int, int, int, int, P2 *);
void game_uso_func_0000B750(void *, int, V3 *, V3 *, float);
}
#define SC(i) (*(float *)(import_8005C108 + 0x128 + (i) * 4))
extern "C" void game_uso_func_0000BB8C(Obj *obj) {
    Ray q;
    V3 world;
    V3 norm;
    float d;
    double k;
    unsigned int i;
    Child *c;
    obj->sub->mtx.getPos(q.pos);
    q.dir.x = 0.0f; q.dir.y = 0.0f; q.dir.z = -1.0f;
    game_uso_func_070F38(&q, (char *)import_80087DA8->body + 0xB4);
    world = ((V3 &)q.dir) * 1000.0f;
    world.x += import_80087DA8->body->mtx.m[3][0] * SC(0);
    world.y += import_80087DA8->body->mtx.m[3][1] * SC(1);
    world.z += import_80087DA8->body->mtx.m[3][2] * SC(2);
    norm = world - (V3 &)q.pos;
    game_uso_func_071028(&norm);
    d = -((V3 &)q.dir).dot(norm);
    if (obj->lo < d) {
        float f24 = (d - obj->lo) / (1.0f - obj->lo);
        if (f24 > 1.0) f24 = 1.0f;
        f24 *= obj->sub->k;
        if (f24 < game_uso_D_807FFA88[45]) return;
        P2 r;
        r.b = 1033;
        r.a = 0;
        float f20 = 0.0f;
        if (obj->lo2 < f24) {
            f20 = (f24 - obj->lo2) / (1.0f - obj->lo2);
            f20 *= f20;
            if (f20 > 1.0) f20 = 1.0f;
            if ((k = game_uso_D_807FFA90[46]) < f20) {
                Q w;
                w.v.x = 0.0f; w.v.y = 0.0f; w.v.z = 0.0f;
                w.s = f20 * obj->k2;
                game_uso_func_05B750(import_8005C108, &w);
                game_uso_func_05B614(import_8005C108, import_80087DA8->c, import_80087DA8->d, import_80087DA8->a, import_80087DA8->b, &r);
            }
        }
        k = game_uso_D_807FFA98[47];
        for (i = 0, c = (Child *)obj; i < obj->n; i++, c = (Child *)((char *)c + 36)) {
            if (c->lim < d) {
                if (c->flags & 1) {
                    if (k < f20) game_uso_func_0000B750(c->rec, obj->b4, &world, &norm, f20);
                } else {
                    game_uso_func_0000B750(c->rec, obj->b4, &world, &norm, f24);
                }
            }
        }
    }
}
