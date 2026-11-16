/* IDO 7.1 NCC (C++) EXACT donor for snoweffects.uso 0xA10C (snow light update; agent-a runs 661 / 665 /
 * 669 / 677), REPLACE_FUNC_BODY-spliced over the placeholder in snoweffects_uso.c on both build paths
 * (Makefile SNOW_A_FUNCS). The 1/127 literal is RoData+0xF4 (snoweffects_uso_func_0000A10C_rodata pin). */
#include "snoweffects_ncc.h"
/* discarded-result callees through a void-typed blank (0-pinned) alias: an int return leaves a dead v0 def */
extern "C" void gl_func_00000000_snowa_v(...);

/* ---- 0xA10C snow light update (0x654). Levers (runs 661 / 665 / 669 / 677):
 * - run 677, the last frame word: the first walk's item copy is held in the function-scope `type` (dead
 *   inside the walk), passed as `type = ...` so the inline helper substitutes it. A named item local in
 *   the loop body adds a word (with the helper's pooled copy of `t`), which put the second walk's
 *   iterator at sp+84 instead of sp+88.
 * - frame (run 661, -O2 -g3 .mdebug): the second walk's ItemType at function scope, c / i at function
 *   scope, SKey(int) ctor for the `sw a1, K(sp)` arg-temp store.
 * - s1 is ONE uopt variable: edg's pooled pointer temporary. It holds the colour Parm `this`, the ambient
 *   Parm `this` of the copy, and each walk's item (the test and the calls). Pool temps are reused after each
 *   full expression, so a value live across a call in one walk makes the whole pool variable callee-saved.
 *   The walks pass `it.get()` to inline helpers whose item parameter is used more than once (a pool temp),
 *   and the named item is assigned from a second, differently spelled read (`it.cur->data`) inside the same
 *   call: the pool temp keeps the load, the item becomes its copy (s0) and is not coalesced.
 * - the ambient compare as two full expressions (x || y, then z): two pool words above the key temp, the
 *   compare on the CSE'd base (v1), the copy through the pool temp (s1).
 * - the discarded-result callees declared void: an int return leaves a dead v0 def that pushed next()'s
 *   result, the loop-body Parm base and the col cursor into v1.
 * - j / f declared first in the vertex loop body and one more block-scope word put n8 at sp+112.
 */
extern "C" int snowDirtyFlag_A30;
extern char *snow_itemMgr;
struct SVec { float x, y, z; SVec &operator+=(const SVec &o) { x += o.x; y += o.y; z += o.z; return *this; } void set(float a, float b, float c) { x = a; y = b; z = c; } };
template <class T> struct SParm { char hdr[0x10]; T v; const T &get() const { return v; } };
struct SCol { float r, g, b; int a; };
struct SMtx { float m[4][4]; };
struct SSrc { char pad[0x70]; SMtx mtx; char padB0[0x114 - 0xB0]; SParm<SCol> colour; };
struct SAmb { char pad[0x114]; SParm<SVec> v; };
struct STgt { char pad[0x2C]; SMtx mtx; char pad6C[0x278 - 0x6C]; SParm<SCol> colourParm; };
struct SShape { char pad[0x40]; unsigned int nVtx; char pad44[0x50 - 0x44]; unsigned char *cols; char pad54[0xC4 - 0x54]; unsigned char *drawCols; };
struct SKey { int v; SKey(int a) { v = a; } };
struct SIt { char *data; SIt *next; };
struct SType { char pad[0x10]; SIt *items; };
struct SItem { char pad[0x230]; int vtxCount; int flags; int f238; int f23C; };
struct SLights {
    char pad[0x2C];
    SShape *shape;   /* 0x2C */
    SAmb *ambient;   /* 0x30 */
    SSrc *src;       /* 0x34 */
    STgt *tgt;       /* 0x38 */
    char pad3C[0x40 - 0x3C];
    SVec amb;        /* 0x40 */
};
#define ITER_NEXT(it) (((it)[0] = (it)[1], (it)[1] != 0) ? ((it)[1] = (it)[1]->next, (it)[0]->data) : 0)
struct SIter {
    SIt *cur, *nxt;
    SIter(SIt *l) { nxt = l; }
    char *next() { cur = nxt; if (nxt != 0) { nxt = nxt->next; return cur->data; } return 0; }
    SItem *get() { return (SItem *)cur->data; }
};
struct SIterB {
    SIt *it[2];
};
/* p is a pooled edg temporary (used three times); q is substituted where it is used, so the caller's
 * `type = ...` copy is evaluated after p is loaded (p keeps the load, the copy is the item for the store).
 * q must be the assigned variable itself: a cast of the assignment is not substituted and costs a word. */
inline void buildItem(SLights *t, SItem *p, SType *q) {
    q;
    if (p->vtxCount == 0) XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009884)(p, t->shape);
    XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009BE8)(p, t->shape);
}
inline int itemNormalsDirty(SItem *p, SItem *q) {
    q;
    return p->flags & 2;
}
extern "C" void snoweffects_uso_func_0000A10C(SLights *t) {
    SType *type;
    unsigned char *c;
    unsigned int i;
    t->tgt->mtx = t->src->mtx;
    t->tgt->colourParm.v = t->src->colour.get();
    XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009768)(t->tgt);
    if (t->amb.x != t->ambient->v.get().x || t->amb.y != t->ambient->v.get().y) goto changed;
    if (t->amb.z == t->ambient->v.get().z) goto same;
changed:
    t->amb.x = t->ambient->v.get().x;
    t->amb.y = t->ambient->v.get().y;
    t->amb.z = t->ambient->v.get().z;
    snowDirtyFlag_A30 = 1;
same:
    if (snowDirtyFlag_A30) {
        c = t->shape->cols;
        for (i = 0; i < t->shape->nVtx; i++) {
            SVec col;
            int j;
            float *f;
            short *v = (short *)CALL(PolyShape_getVertex)(t->shape, i);
            SVec *p = (SVec *)(D_00000000 + 0x40);
            p->x = v[0];
            p->y = v[1];
            p->z = v[2];
            int unused = 0; /* one more block-scope word: puts n8 at sp+112 */
            {
                signed char n8[4];
                XCALL(gl_func_00000000_snowa_v, PolyShape_getNormal8)(t->shape, i, n8);
                SVec *n = (SVec *)(D_00000000 + 0x50);
                n->x = n8[0] * (1.0f / 127.0f);
                n->y = n8[1] * (1.0f / 127.0f);
                n->z = n8[2] * (1.0f / 127.0f);
            }
            col.set(0.0f, 0.0f, 0.0f);
            col.x += t->ambient->v.get().x;
            col.y += t->ambient->v.get().y;
            col.z += t->ambient->v.get().z;
            XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009FE8)(t, t->tgt, &col);
            for (j = 0; j < 3; j++) {
                f = &col.x + j;
                *f *= 255.0f;
                if (*f < 0.0f) *f = 0.0f;
                if (255.0f < *f) *f = 255.0f;
            }
            c[0] = col.x;
            c[1] = col.y;
            c[2] = col.z;
            c[3] = 255;
            c += 4;
        }
        type = (SType *)CALL(ItemMgr_findType_0003EA98)(snow_itemMgr, SKey(*(int *)(D_00000000 + 0xA78)));
        if (type != 0) {
            SIter it(type->items);
            while (it.next() != 0) {
                /* the item is held in `type` (dead here): a named item local is a frame word too many */
                buildItem(t, it.get(), type = (SType *)it.cur->data);
                ((SItem *)type)->flags = 4;
            }
        }
        t->shape->drawCols = t->shape->cols;
        snowDirtyFlag_A30 = 0;
    }
    type = (SType *)CALL(ItemMgr_findType_0003EA98)(snow_itemMgr, SKey(*(int *)(D_00000000 + 0xA7C)));
    {
        if (type != 0) {
            SIter it(type->items);
            while (it.next() != 0) {
                SItem *item;
                if (itemNormalsDirty(it.get(), item = (SItem *)it.cur->data)) {
                    XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009884)(item, t->shape);
                    item->flags &= ~2;
                }
                if (item->flags & 1) {
                    XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009BE8)(item, t->shape);
                    item->f238 = 0;
                    item->f23C = 0;
                    item->flags &= ~1;
                }
                if (item->flags & 4) {
                    XCALL(gl_func_00000000_snowa_v, snoweffects_uso_func_00009C48)(item, t->shape);
                    item->flags &= ~4;
                }
            }
        }
    }
}

