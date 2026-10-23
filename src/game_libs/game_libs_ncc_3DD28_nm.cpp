/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003DD28 (0x98 / 38
 * words): factory -- o = create(0); o->0x70 = 10.0f; o->virtual slot 5
 * (vptr +0x28: lh +0x28 / lw +0x2C) with &kind (19); attach o + 0x10 to
 * a0->0x40's list; if the list already had an owner (node->0x14 != 0, the cfe
 * NM body tests == 0 -- wrong sense) set node->4 = 1; node->0x14 = o.
 * NCC 81.4 vs cfe 67.3 (36 vs 35 words, target 38; agent-c run 133). Still
 * NON_MATCHING: the target copies the create() result into v1 (o spilled at
 * sp+40, kind sp+44, node sp+36, frame 56 vs 40) and keeps a nop after the
 * jalr. Run 134: pad[2] above kind and pad2[2] below node (one dead store
 * each; NCC gives every referenced local a slot) give the target frame 56 and
 * the kind / o / node homes 44 / 40 / 36 (81.45 -> 81.71); new Obj compiles to
 * the same __ct(0) call. Left: the target copies o into v1 after the call.
 * Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
extern "C" void *gl_func_00000000(...);
struct Base { char pad[0x10]; char link[0x18];
  virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void f(int *); };
struct Obj : Base { char pad2c[0x44]; float f70; };
struct Node { int x0; int x4; char pad8[0xC]; Obj *o14; };
struct Owner { char pad[0x40]; Node *n40; };
extern "C" Obj *gl_func_0003DD28(Owner *a0) {
    int pad[2];
    int kind;
    Obj *o;
    Node *node;
    int pad2[2];
    pad[0] = 0; pad2[0] = 0;
    o = (Obj *)gl_func_00000000(0);
    o->f70 = 10.0f;
    kind = 19;
    o->f(&kind);
    node = a0->n40;
    gl_func_00000000(o->link, node);
    if (node->o14) node->x4 = 1;
    node->o14 = o;
    return o;
}
