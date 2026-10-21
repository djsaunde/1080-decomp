/* IDO 7.1 NCC (C++) donor for gl_func_000372D4 (0x74 / 29 words): copy a
 * Vec3 into the attached object's sub-node position, reset this->rot to
 * (-90, 180, 0) and fire virtual slot 3.
 *
 * The cfe wrap capped on two shapes, both C++:
 * - the dead `sw t6,28(sp)` is the attached-object handle: a one-pointer
 *   smart-pointer struct (`Ptr` with operator->) is an aggregate local, so it
 *   lives in its frame slot and the load is stored there once;
 * - the x store at p+0x60 before `addiu v1,v1,48` and y/z at 52/56 is the
 *   inline Sub::setPos(const V3 &) (this = p + 0x30) forwarding to
 *   V3::operator=.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
struct V3 { float x, y, z;
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    V3 &operator=(const V3 &v) { set(v); return *this; } };
struct Sub372 { char pad[0x30]; V3 pos; void setPos(const V3 &v) { pos = v; } };
struct Node372 { char pad[0x30]; Sub372 sub; };
struct Obj372 { char pad[0xF4]; Node372 *node; };
struct Ptr372 { Obj372 *o; Obj372 *operator->() { return o; } };
struct Base372 {
    char pad[0x14]; Obj372 *att; char pad18[0x10];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Self372 : Base372 {
    int pad2c;
    V3 rot;
};
extern "C" void gl_func_000372D4(Self372 *s, const V3 &v) {
    Ptr372 o; o.o = s->att;
    o->node->sub.setPos(v);
    s->rot.set(-90.0f, 180.0f, 0.0f);
    s->v3();
}
