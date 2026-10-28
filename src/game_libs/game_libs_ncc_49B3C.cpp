/* IDO 7.1 NCC (C++) donor for gl_func_00049B3C (0x280 / 160 words), EXACT (agent-g run 190), spliced over
 * the post0b.c placeholder in both trees.
 * agent-c run 174 built the shape (E1 / E2 virtual-call element types, kept-parameter iter(), for-init list walk).
 * Last three levers (agent-g run 190): AIter::cur() reads the array through a local reference
 * (`Arr &r = *a; return r[idx];`) -- the extra edg temp puts the body's base reload in v1 and the vtable in a1;
 * LI::next() as an if-statement (`if ((cur = nxt) != 0) { nxt = nxt->next; return cur->data; } return 0;`)
 * gives the bottom copy's result v0; with that next(), the list walk is a plain `LI li(g->head); while (li.next())`
 * (the for-init form now costs the entry copy). */
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
    E1 *cur() { Arr &r = *a; return r[idx]; }
};
inline AIter iter(Arr *a) {  if (0) { a = 0; }  return AIter(a); }
struct L { E2 *data; L *next; };
struct LI { L *cur; L *nxt;
  LI(L *h) {   nxt = h; }
  E2 *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->data; } return 0; } };
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
        LI li(g->head); while (li.next()) {
             callu(li.cur->data); 
        }
    }
    gl_func_00000000(D_49B3C_s6);
     
    gl_func_00000000(self);
}
