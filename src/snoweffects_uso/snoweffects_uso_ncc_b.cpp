#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donors for the snow effect's child-node creators (agent-b run 640), spliced into
 * snoweffects_uso.c.o via REPLACE_FUNC_BODY. All three EXACT:
 *   0x5FD4  SnowEffect::init (name Data+0x63C, both groups, SnowRing_init stride 4, 0x6B08, child0 +0x9C)
 *   0x6144  SnowEffect::createA (0x6420 defaults, childA +0x98, virtual slot 11 arg 8)
 *   0x6248  SnowEffect::createB (0x70C0 defaults, childB +0xA0, virtual slot 11 arg 16)
 * Each `new`s a 0x48-byte child: the inline SnowNode ctor (edg's this == 0 protocol, so a second
 * mem_new(0x48) fallback) calls the out-of-line Node ctor with the child's name (Data+0x648 / 0x650 /
 * 0x658), stores the SnowNode vptr (Data+0x60), then the child's own vptr (Data+0x1C8 / 0x158 / 0x238).
 * The child is attached to the groups at +0x238 then +0x234 (list_append on the group's child list,
 * flag = 1 if it already had a parent, parent = group), then gets virtual slot 11.
 * Load-bearing:
 *  - the `new` is an argument of the first attach (`lnk(childA = new SnowChildA, grpA)`): in one
 *    full-expression the inline ctor's `this` and the attach's q get separate edg pool words (0x24 /
 *    0x2C, a0 / a2). As two statements they share a word and the ctor's `this` is coloured a2.
 *  - list_append through a void-typed alias (gl_func_00000000_snowb_v): v0 is forbidden through a
 *    value-returning call's block, and the target keeps the group in v0 across the first attach.
 * Symbols are renamed after the compile (Makefile): the member functions to the C names, the Node
 * ctor and the four vtables to 0-pinned names in undefined_syms_auto.txt (blank lui/addiu in the ROM). */
extern "C" {
int gl_func_00000000(...);
void gl_func_00000000_snowb_v(...); /* list_append, typed void */
void gl_func_00000000_snow_3ee4(void *self, int a1, int a2, int a3, int a4, float f5, float f6);
extern char D_00000000[];
}
inline void *operator new(unsigned n) { return (void *)CALL(mem_new)(n); }

/* bootup Node (0x28 data bytes, vptr at +0x28; virtual slot 11 takes an int) */
struct Node {
    char pad00[4];
    /* 0x04 */ int flag;
    char pad08[0xC - 8];
    /* 0x0C */ char *name;
    /* 0x10 */ int list;      /* child list */
    /* 0x14 */ Node *parent;
    char pad18[0x28 - 0x18];
    Node(char *name);
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(int);
};

/* attach q under group s */
inline void lnk(Node *q, Node *s) {
    XCALL(gl_func_00000000_snowb_v, list_append)(&s->list, q);
    if (q->parent) q->flag = 1;
    q->parent = s;
}

/* the 0x48-byte child nodes: a SnowNode base (vtable Data+0x60) and one class per child */
struct SnowNode : Node { char pad2c[0x48 - 0x2C]; SnowNode(char *name) : Node(name) {} virtual void v1(); };
struct SnowChild0 : SnowNode { SnowChild0() : SnowNode(D_00000000 + 0x648) {} virtual void v1(); };
struct SnowChildA : SnowNode { SnowChildA() : SnowNode(D_00000000 + 0x650) {} virtual void v1(); };
struct SnowChildB : SnowNode { SnowChildB() : SnowNode(D_00000000 + 0x658) {} virtual void v1(); };

struct SnowEffect : Node {
    char pad2c[0x98 - 0x2C];
    /* 0x098 */ SnowChildA *childA;
    /* 0x09C */ SnowChild0 *child0;
    /* 0x0A0 */ SnowChildB *childB;
    char padA4[0x234 - 0xA4];
    /* 0x234 */ Node *grpB;
    /* 0x238 */ Node *grpA;
    void init(Node *gB, Node *gA, int a3, int a4, int a5, float f6, float f7);
    void createA();
    void createB();
};

void SnowEffect::init(Node *gB, Node *gA, int a3, int a4, int a5, float f6, float f7) {
    name = D_00000000 + 0x63C;
    grpA = gA;
    grpB = gB;
    XCALL(gl_func_00000000_snow_3ee4, SnowRing_init_00003EE4)(this, a3, 4, a4, a5, f6, f7);
    CALL(snoweffects_uso_func_00006B08)(this);
    lnk(child0 = new SnowChild0, gA);
    lnk(child0, gB);
    child0->v11(8);
}

void SnowEffect::createA() {
    CALL(snoweffects_uso_func_00006420)(this);
    lnk(childA = new SnowChildA, grpA);
    lnk(childA, grpB);
    childA->v11(8);
}

void SnowEffect::createB() {
    CALL(snoweffects_uso_func_000070C0)(this);
    lnk(childB = new SnowChildB, grpA);
    lnk(childB, grpB);
    childB->v11(16);
}
