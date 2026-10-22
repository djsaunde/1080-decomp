/* IDO 7.1 NCC (C++) NON_MATCHING donor for game_uso_func_0000B274 (0x1AC / 107 words).
 *
 * Easing update: gate on the global's progress, project the position
 * (063734), store the screen coords, ease the scale down past a threshold,
 * then copy the position into the global display object and push the scale
 * to the child before its third virtual (cfront vtable entry at +0x18).
 * C++ shapes that the cfe body could not give:
 * - the global object's address held in $s1 across the calls: a named int
 *   copy of the address (`int gi = (int)&sym; Glob *g = (Glob *)gi;`) keeps
 *   g a variable under edg (a plain or int-cast pointer is folded back into
 *   lui/%lo per use);
 * - the display object is reached through an inline member (setPos) whose
 *   `this` is a named int address plus 0xA0, giving the target's
 *   `lui/addiu v0,+160; swc1 48(v0)`;
 * - the child's vptr sits at +0x28 because the virtuals live in a base class
 *   with only padding before them (cfront places the vptr after the first
 *   virtual class's data);
 * - the scale product is written field-first (edg keeps source order).
 * RESIDUAL (20 words, 11 with stack offsets masked): frame 88 vs 64. The
 * relative layout of v / gi / di / r / the two pool slots is the target's,
 * but edg adds three 8-byte temporaries for the float->double arithmetic on
 * cast-pointer reads (g->prog, the threshold doubles); double literals drop
 * one of them but give the wrong %lo in the unlinked game_uso block.
 * Spliced into the NON_MATCHING object only; the ROM keeps INCLUDE_ASM.
 * docs/IDO_CODEGEN.md#kit-xxiii-agent-g-run-120. */
extern "C" {
extern char import_8005C108;
extern char import_8005C1A8;
extern char D_B274_t0, D_B274_t1, D_B274_t2, D_B274_t3;
}
#define DBL(s, o) (*(double *)(&(s) + (o)))
struct V3 { float x, y, z; };
struct Glob { char pad[0x254]; int handle; char pad258[0x2A0 - 0x258]; float prog; };
struct Dst { char pad[0x30]; V3 pos; void setPos(const V3 &p) { pos.x = p.x; pos.y = p.y; pos.z = p.z; } };
struct CB { char pad[0x28]; virtual void f0(); virtual void f1(); virtual void f2(); };
struct Child : CB { char pad2c[0xB0 - 0x2C]; float sx, sy; char padb8[0xC0 - 0xB8]; short id; };
struct Self { char pad[0xA0]; V3 pos; char padac[0xC4 - 0xAC]; Child *child; float r; float r0; int ix, iy, h; char sub[0x134 - 0xDC]; float k; char pad138[0x14C - 0x138]; int id; char pad150[0x164 - 0x150]; char sub2[4]; };
extern "C" {
int game_uso_func_063734(int, V3 *, int, int);
void import_000F8678(Glob *, void *);
void game_uso_func_04D700(Self *);
void game_uso_func_05B750(Glob *, void *);
}
extern "C" void game_uso_func_0000B274(Self *self) {
    V3 v;
    int gi = (int)&import_8005C108; Glob *g = (Glob *)gi;
    int di = (int)&import_8005C1A8 + 0xA0;
    float r;
    if (DBL(D_B274_t0, 0x120) < g->prog) return;
    v.x = self->pos.x; v.y = self->pos.y; v.z = self->pos.z;
    if (game_uso_func_063734(g->handle, &v, 1, 0) == 0) return;
    self->ix = (int)v.x;
    self->iy = (int)v.y;
    import_000F8678(g, self->sub);
    self->h = g->handle;
    self->r = self->r0;
    if (DBL(D_B274_t1, 0x128) < g->prog)
        self->r = self->r * (1.0 - (g->prog - DBL(D_B274_t1, 0x128)) / DBL(D_B274_t2, 0x130));
    if (!(DBL(D_B274_t3, 0x138) < self->r)) return;
    game_uso_func_04D700(self);
    ((Dst *)di)->setPos(self->pos);
    r = self->k * self->r;
    game_uso_func_05B750(g, self->sub2);
    self->child->id = self->id;
    self->child->sx = r;
    self->child->sy = r;
    self->child->f2();
}
