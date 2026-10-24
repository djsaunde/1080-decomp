/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_00044CE8
 * (post0b, 43 words; 43 = target). NM object only; the ROM build keeps
 * INCLUDE_ASM. agent-c run 148 (docs/IDO_CODEGEN.md kit LXXXVII): the pool
 * alloc `&pool->e[pool->n++]` (50 twelve-byte records at +0x378, count at
 * +0x5D0) under NCC gives the target's base shape -- the first count test
 * folds to `lw 1488(a3)`, the pool base `addiu v0,a3,888` is rematerialised
 * in the bnezl likely slot and again after the overflow call (cfe spills a
 * named base or folds every access to a3 offsets, 91 %). Frame 32 = two
 * slots (r 28, p 24): every edg temp costs a slot (`n++` +8, a separate
 * `int k` +8), so the old count rides in r itself (role pair).
 * RESIDUAL (1 colour): the old count colours a0 (r's register, it becomes
 * the call argument) where the target has v1; an expression-only capture
 * (r = &p->e[p->n]; p->n += 1) colours v1 but numbers the address temp
 * before the increment (t8 / t9 swapped). */
extern "C" {
extern int gl_func_00000000(...);
extern char D_00000000[];
}
struct Rec { int a, b, c; };
struct Pool {
    Rec e[50];
    int n;
};
struct Obj {
    char pad0[0x2AC];
    Rec *slot[51];
    Pool pool;
};
extern "C" Rec *gl_func_00044CE8(Obj *o, unsigned int i, int a2) {
    if (i >= 50) gl_func_00000000(D_00000000 + 0x1FE94, D_00000000 + 0x1FEA8, 0x102D);
    Rec *r;
    Pool *p = &o->pool;
    if (p->n >= 50) gl_func_00000000();
    r = (Rec *)p->n; p->n = (int)r + 1;
    r = &p->e[(int)r];
    o->slot[i] = r;
    r->c = 0;
    gl_func_00000000(r, a2);
    return r;
}
