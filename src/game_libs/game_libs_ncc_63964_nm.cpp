/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_00063964 (280 words; this body 278, cfe NM
 * body 275). NM object only; the ROM build keeps INCLUDE_ASM.
 * agent-g run 165 (docs/IDO_CODEGEN.md kit CV): the cfe body's struct-copy staging (`t = d; n = t;`,
 * `t = s1; c1 = t; ... u = r1; w = u; out.x = w.x ...`) is plain C++ Vec3 arithmetic: inline by-value
 * operators (each result goes through the one shared return-copy temp at sp+220) and a BY-VALUE
 * `operator=(V3)` that copies its parameter element-wise (the u -> w -> out tail); `V3 n = va - vb;`
 * declared at its use is copy-constructed (two struct copies, no element-wise tail); fin / va / vb / out /
 * count declared first in that order give the target's homes. No pads, no empty tests. 84.94 standalone
 * (cfe NM 77.49). Then `V3 out(0.0f, 0.0f, 0.0f);` CONSTRUCTED at the zeroing point (not declared up top
 * and zeroed): probably the folded this==0 test of the ctor ends the basic block; the subtraction reloads va / vb
 * from their homes exactly like the target (what the cfe `if (1) {}` imitated): 88.74, length 1112.
 * LEFT: frame 312 vs 328 (low temps), the 0 + va.y accumulate.
 * Run 170 (kit CXV): four unused volatile ints in a block AFTER the last call are homed below every temp:
 * frame 328 = target (the named homes were already right relative to the frame top).
 * Run 182 (agent-g, kit CXXXI): `int count;` declared after `out` (target homes out 280..288 above count 276),
 * the V3 ctor forwarding to an inline set() (the 5062C rule), and three empty `if (1) {}` BB breaks (after the
 * out construction, after `acc = 0.0f`, before `acc += va.y`): the then-arm now rematerialises its zero and
 * adds it (target `mtc1 zero,f6; add.s f16,f6,f8`) instead of folding 0 + va.y: 88.72 -> 96.19, body 1112 -> 1116
 * (target 1120). LEFT: the out zeros in their own f0 (ours share acc's f16), a 12-byte temp block order. */
typedef float f32;
extern "C" void gl_func_00000000(...);
struct V3 {
    f32 x, y, z;
    V3() {}
    void zero() { x = y = z = 0.0f; }
    void operator=(V3 o) { x = o.x; y = o.y; z = o.z; }
    void operator+=(const V3 &b) { x += b.x; y += b.y; z += b.z; }
    V3(f32 a, f32 b, f32 c) { set(a, b, c); }
    void set(f32 a, f32 b, f32 c) { x = a; y = b; z = c; }
};
inline V3 operator-(const V3 &a, const V3 &b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline V3 operator+(const V3 &a, const V3 &b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline V3 operator*(const V3 &a, f32 s) { return V3(a.x * s, a.y * s, a.z * s); }
struct O { char p[0xDC]; V3 pos; char pE8[0xF4 - 0xE8]; char probe[0x108 - 0xF4]; int *tgt; V3 pa; V3 pb; };
extern "C" void gl_func_00063964(O *o, f32 acc, int a2, int a3) {
    V3 fin;
    V3 va;
    V3 vb;

    gl_func_00000000(o->probe, &o->pa, &va, o);
    gl_func_00000000(o->probe, &o->pb, &vb, o);
    va += o->pos;
    vb += o->pos;
    V3 out(0.0f, 0.0f, 0.0f); if (1) {}
    int count;
    V3 n = va - vb;
    count = 0;
    acc = 0.0f; if (1) {}
    gl_func_00000000(&n);
    if (va.y < (f32)0.0) {
        out = va + n * va.y;
        count = 1;
        if (1) {} acc += va.y;
    }
    if (vb.y < 0.0f) {
        out = vb + n * vb.y;
        count += 1;
        acc += vb.y;
    }
    if (count == 2) {
        out = (va + vb) * 0.5f;
        acc = acc * 0.5;
    }
    fin.x = 0.0f; fin.y = -acc; fin.z = 0.0f;
    gl_func_00000000(o->tgt, &out, &fin);
    { volatile int q0, q1; }
}
