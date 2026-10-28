/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00049B3C (0x280 / 160 words).
 * agent-c run 174: rebuilt from the run 173 draft (E1 / E2 virtual-call element types, `it.cur()->tick()` body,
 * kept-parameter iter()) with the list walk as `for (LI li(g->head); li.next(); )`: EDG keeps the for-init's
 * pooled temps for the whole statement, so the entry copy of li.next() colours its result a1 as the target.
 * 99.01 -> 99.33 (objdiff diff, run 162 donor -> this). Residual: the array body's base / vtable loads swap
 * v1 / a1, and the list loop's bottom-copy result is v1 (target v0). The same for-init form on the array loop
 * (41D40's lever) costs 16 words here. Still NON_MATCHING. */
extern "C" {
int gl_func_00000000(...);
void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern char D_00000000[];
extern char D_49B3C_s1[], D_49B3C_s2[], D_49B3C_s3[], D_49B3C_s4[], D_49B3C_s5[], D_49B3C_s6[];
}
inline void *operator new(unsigned n) { return (void *)gl_func_00000000(n); }
struct E1 { char pad[0x5C]; virtual void a(); virtual void b(); virtual void tick(); };
struct E2 { char pad[0x28]; virtual void upd(); };
struct Arr { E1 **data; int pad4, pad8; int count; E1 *&operator[](int i) { return data[i]; } };
struct IBase { Arr *a; int idx; int last; IBase(Arr *arr) {   a = arr; idx = -1; last = arr->count - 1; } };
struct AIter : IBase {
    AIter(Arr *arr) : IBase(arr) {  if (0) { arr = 0; }  }
    E1 **next() {  if (idx < last) return &(*a)[++idx]; return 0;  }
    E1 *cur() { return (*a)[idx]; }
};
inline AIter iter(Arr *a) {  if (0) { a = 0; }  return AIter(a); }
struct L { E2 *data; L *next; };
struct LI { L *cur; L *nxt;
  LI(L *h) {   nxt = h; }
  E2 *next() {  return (cur = nxt) ? (nxt = nxt->next, cur->data) : 0;  } };
inline void callu(E2 *e) { if (0) { e = 0; } e->upd(); }
inline void callt(E1 *e) { if (0) { e = 0; } e->tick(); }
inline void regp(char *a, char *b, char *p) { if (0) { p = 0; } gl_func_00000000(a, b, p, 0); }
struct Grp { char pad[0x10]; L *head; };
extern "C" void gl_func_00049B3C(char *self) {
     
    gl_func_00000000(D_49B3C_s1, D_00000000 + 0x1FFD4, 0);
     regp(D_49B3C_s2, D_00000000 + 0x1FFE0, self + 64); 
     regp(D_49B3C_s3, D_00000000 + 0x1FFE8, self + 68); 
    gl_49B3C_regf(D_49B3C_s4, D_00000000 + 0x1FFF4, self + 156, 0.0f, 10.0f, 0);
     
     AIter it = iter((Arr *)(self + 132)); 
    while (it.next()) {
          it.cur()->tick(); 
    }
    gl_func_00000000(D_49B3C_s5, D_00000000 + 0x20004, self + 164, -1, *(int *)(self + 140), 0); if (1) {}
     if (*(Grp **)(self + 44)) { Grp *g = *(Grp **)(self + 44); 
        for (LI li(g->head); li.next(); ) {
             callu(li.cur->data); 
        }
    }
    gl_func_00000000(D_49B3C_s6);
     
    gl_func_00000000(self);
}
