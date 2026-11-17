/* IDO 7.1 NCC (C++) NON_MATCHING donor for snoweffects.uso 0x9328, the SnowTarget Cfront constructor
 * (agent-e run 692; draft history in ncc_drafts/snoweffects_nm_e.cpp, runs 653 / 659 / 680). Spliced over the
 * INCLUDE_ASM in snoweffects_uso.c on the non_matching build only (Makefile SNOW_NM_E9_DONOR): the objcopy step
 * renames the ctor to snoweffects_uso_func_00009328, the SNode / GdlParmBase (Parms_addParm) ctors and
 * operator new to 0-pinned blank-jal aliases, and the four vtables to symbols pinned at their Data offsets
 * (SnowTarget 0x0, ParmF 0x838, ParmC 0x850, ParmI 0x868). 246 / 247 words; frame 192 vs 224.
 *
 * SNode base (out of line, then vptr +0x28), a Parms member at +0x240 (0x8C bytes, an 8-byte list head
 * {Data+0xA34, 0} with its own new(8) fallback), then five inline Parm ctors over GdlParmBase
 * (= Parms_addParm(this, owner, name by value, flag), vptr at +0xC): float 40 (name Data+0xA44, vtbl
 * Data+0x838), float 10 (0xA48), Colour value / min / max (1,1,1,1) / (0,0,0,0) / (1,1,1,1) passed by
 * value (0xA4C, vtbl 0x850; only the value is stored), int 0 flag 0 (0xA50, vtbl 0x868), int 3 (0xA54).
 * Body: name (+0xC) = Data+0xA58, +0x22C..+0x23C = 0, 0, 1, 3, 0, colour +0x88 = 1 x4, +0x98..+0xA8 = 0.
 * Levers (run 680): each name is passed as an rvalue PName (one materialised temp per inline Parm ctor);
 * ParmC's Colour member is a POD base (a user ctor adds a this + 16 == 0 -> new(16) check); col[3] .. col[0]
 * are stored in reverse.
 * Run 697 (agent-b): ParmC takes its name AFTER the three Colours, (o, val, lo, hi, name, fl), so the name temp is
 * created after the Colour temps as in the target (and is reloaded from its word for the GdlParmBase call);
 * ParmC stores f20 before v, ParmI f14 before v (objdiff 95.66 -> 99.63 %).
 * Run 705 (agent-b): the five int stores each on their own source line (as1 line-group tie-break) put +0x234 before
 * +0x238 as in the target; the code shape is now exact, only frame offsets differ. Frame 192 vs 224: our edg temps are homed
 * bottom-up in creation order (n1 lowest), the target's top-down (n1 at 216, a1 spill 220), and the target's
 * pool area below is 32 bytes larger.
 */
#include "snoweffects_ncc_e.h"
extern "C" void *mem_new(unsigned int);
struct PName { char *s; PName(char *p) { s = p; } };
struct ColourP { float r, g, b, a; };
struct Colour : ColourP { Colour(float r_, float g_, float b_, float a_) { r = r_; g = g_; b = b_; a = a_; } };
struct Parms9;
struct GdlParmBase9 {
    char hdr[0xC];
    GdlParmBase9(Parms9 *o, PName n, int fl);   /* Parms_addParm */
    virtual void v1();
    void *operator new(unsigned n) { return mem_new(n); }
};
struct ParmF9 : GdlParmBase9 {
    float v; int f14;
    ParmF9(Parms9 *o, PName n, float val, int fl) : GdlParmBase9(o, n, fl) { v = val; f14 = 0; }
    virtual void v1();
};
struct ParmI9 : GdlParmBase9 {
    int v; int f14;
    ParmI9(Parms9 *o, PName n, int val, int fl) : GdlParmBase9(o, n, fl) { f14 = 0; v = val; }
    virtual void v1();
};
struct ParmC9 : GdlParmBase9 {
    ColourP v; int f20;
    ParmC9(Parms9 *o, Colour val, Colour lo, Colour hi, PName n, int fl) : GdlParmBase9(o, n, fl) { f20 = 0; v = val; }
    virtual void v1();
};
struct ParmsHead9 {
    char *vt;
    int count;
    ParmsHead9() { vt = D_00000000 + 0xA34; count = 0; }
    void *operator new(unsigned n) { return mem_new(n); }
};
#define NM9(off) PName(*(char **)(D_00000000 + (off)))
struct Parms9 : ParmsHead9 {
    ParmF9 p08;
    ParmF9 p20;
    ParmC9 p38;
    ParmI9 p5C;
    ParmI9 p74;
    Parms9() : p08(this, NM9(0xA44), 40.0f, 1), p20(this, NM9(0xA48), 10.0f, 1),
               p38(this, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 0.0f),
                   Colour(1.0f, 1.0f, 1.0f, 1.0f), NM9(0xA4C), 1),
               p5C(this, NM9(0xA50), 0, 0), p74(this, NM9(0xA54), 3, 1) {}
    void *operator new(unsigned n) { return mem_new(n); }
};
struct SNode9 {
    char pad00[0xC];
    char *name;
    char pad10[0x28 - 0x10];
    SNode9();                                     /* SNode_ctor */
    virtual void v1();
    void *operator new(unsigned n) { return mem_new(n); }
};
struct SnowTarget9 : SNode9 {
    char pad2c[0x88 - 0x2C];
    /* 0x88 */ float col[4];
    /* 0x98 */ float f98, f9C, fA0, fA4, fA8;
    char padAC[0x22C - 0xAC];
    int f22C, f230, f234, f238, f23C;
    /* 0x240 */ Parms9 parms;
    SnowTarget9();
    virtual void v1();
};
SnowTarget9::SnowTarget9() {
    name = D_00000000 + 0xA58;
    f22C = 0;
    f230 = 0;
    f234 = 1;
    f238 = 3;
    f23C = 0;
    col[3] = 1.0f; col[2] = 1.0f; col[1] = 1.0f; col[0] = 1.0f;
    f98 = 0.0f; f9C = 0.0f; fA0 = 0.0f; fA4 = 0.0f; fA8 = 0.0f;
}
