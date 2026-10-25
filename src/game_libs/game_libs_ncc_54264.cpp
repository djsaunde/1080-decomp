/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00054264 (0x358 / 214 words), agent-c run 175, spliced over an empty
 * placeholder in game_libs_post0b.c in both build trees (Makefile rename of sample__4MeshFR2V3T1i).
 * The cfe body (85.11: volatile Vec3 locals, a `mult` variable for the multu, an int-copy temp) is plain C++:
 * `void Mesh::sample(V3 &p, V3 &n, int t)` -- a uniform random point in triangle t (u, v from two calls through the
 * blank callee = the target's baked jal 0, folded when u + v > 1), the three s16 positions converted by an inline
 * `V3(const S3 &)` ctor (the `* 6` index stays a multu), the barycentric blend written to p, then the three float
 * normals copied in through a BY-VALUE `set(V3 v)` (the target's int-word copy into the one temp at sp+0x24, then
 * the float copy-out) and blended into n. Two function-scope volatiles + the named cs before `V3 a` = frame 0x78
 * and every home (98.15, length = target). c's S3 pointer as the named `cs` kept past the p stores by a trailing
 * `if (cs) {}` takes p's reload off v0 (as1 then hoists p's load and the &tmp addiu like the target, 99.02); a
 * second dead test `if (t) {}` restores the colour priorities the first one's extra blocks had flipped (t * 8 in
 * v1, the 6 / 12 constants and p in a1, u / v in fv1 / fa0) -> EXACT. */
extern "C" float gl_func_00000000(...);
struct S3 { short x, y, z; };
struct V3 {
    float x, y, z;
    V3() {}
    V3(const S3 &s) { x = s.x; y = s.y; z = s.z; }
    void set(V3 v) { x = v.x; y = v.y; z = v.z; }
};
struct Tri { short f0; unsigned short i[3]; };
struct Mesh {
    char pad[0x58];
    V3 *nrm;
    int pad5c;
    S3 *verts;
    int pad64;
    Tri *tris;
    void sample(V3 &p, V3 &n, int t);
};
void Mesh::sample(V3 &p, V3 &n, int t) {
    float u = gl_func_00000000();
    float v = gl_func_00000000();
    if (u + v > 1.0f) {
        u = 1.0f - u;
        v = 1.0f - v;
    }
    volatile int q0, q1;
    S3 *cs;
    V3 a(verts[tris[t].i[0]]);
    V3 b(verts[tris[t].i[1]]);
    cs = &verts[tris[t].i[2]];
    V3 c(*cs);
    float w = 1.0f - u - v;
    p.x = a.x * w + b.x * u + c.x * v;
    p.y = a.y * w + b.y * u + c.y * v;
    p.z = a.z * w + b.z * u + c.z * v;
    if (cs) {}
    if (t) {}
    a.set(nrm[tris[t].i[0]]);
    b.set(nrm[tris[t].i[1]]);
    c.set(nrm[tris[t].i[2]]);
    n.x = a.x * w + b.x * u + c.x * v;
    n.y = a.y * w + b.y * u + c.y * v;
    n.z = a.z * w + b.z * u + c.z * v;
}
