/* IDO 7.1 NCC (C++) donor for gl_func_0003DD28: cfront create + virtual call + list attach.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
/* (agent-c run 133 / 134) 3DD28 = create an object through its cfront allocating
 * constructor (called with this == 0; `new Obj` compiles to the same __ct(0) call),
 * set +0x70 = 10.0f, call its virtual f(&kind) with kind = 19 through the
 * this-adjusted vtable entry, link it into the owner's node list (node = a0->0x40),
 * then node->4 = 1 if the node already held an object, node->0x14 = o.
 * cfe 67.3 -> NCC 81.4 (run 133) -> 81.7 (frame pads) -> EXACT: `if (a0) {}` right
 * after the create call keeps a0 live across it, which moves o into v1 (the target's
 * `move v1,v0` copy) and fills the jalr delay as the target does. pad[2] / pad2[2]
 * (one dead store each) give the frame 56 and the kind / o / node homes 44 / 40 / 36:
 * NCC gives every referenced local a slot. */
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
    o = (Obj *)gl_func_00000000(0); if (a0) {}
    o->f70 = 10.0f;
    kind = 19;
    o->f(&kind);
    node = a0->n40;
    gl_func_00000000(o->link, node);
    if (node->o14) node->x4 = 1;
    node->o14 = o;
    return o;
}
