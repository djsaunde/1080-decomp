/* snoweffects.uso 0x9328: the SnowTarget Cfront constructor, EXACT through the MIPSpro 7.2.1 front end
 * (EDG 2.29, tools/ncc721.sh; agent-b run 711). Spliced over the placeholder in snoweffects_uso.c in both
 * build trees; the objcopy step renames snow_target_ctor to snoweffects_uso_func_00009328 and the three
 * callees to the 0-pinned blank-jal aliases snowe9_* (undefined_syms_auto.txt).
 *
 * Layout: SNode base (out-of-line ctor, then the SnowTarget vptr Data+0x0 at +0x28), a Parms member at +0x240
 * (0x8C bytes: an 8-byte list head {Data+0xA34, 0}, then five Parms: float 40 (name Data+0xA44, vtbl
 * Data+0x838), float 10 (0xA48), Colour value / min / max (0xA4C, vtbl 0x850; only the value is stored),
 * int 0 flag 0 (0xA50, vtbl 0x868), int 3 (0xA54)), then name (+0xC) = Data+0xA58, +0x22C..+0x23C = 0, 0, 1, 3, 0,
 * colour +0x88 = 1 x4 (stored in reverse), +0x98..+0xA8 = 0. Each Parm goes through the GdlParmBase ctor
 * (= Parms_addParm(this, owner, name by value, flag)) before its vptr at +0xC.
 *
 * Why EDG 2.29 and a free function (MATCHING_SUMMARY section 5, kit CDXLV): the target's temporaries are homed
 * in EDG 2.29 pool order (name temp n1 at sp+216, n2 212, the Colour temps 196 / 180 / 164, n3..n5 160 / 156 /
 * 152), which the IDO 7.1 NCC front end (EDG 2.19) reverses. But EDG 2.29 keeps an out-of-line constructor's
 * `this` in memory (`sw a0,184(sp); lw t6,184(sp)`), while the target keeps it in s0. So the constructor is
 * written as what Cfront lowers it to: an extern "C" function taking the object pointer, with the
 * `this == 0 -> operator new` checks spelled out, and every inline member constructor as an inline function
 * of the same shape. Levers, each read off the ucode pool words (`ugen -O0 -d` of the .B):
 *   - PName / Colour temporaries are DERIVED types (PNameT, ColourT) sliced into the POD by-value parameters.
 *     The slice copies member-wise, so uopt forwards the name word (`sw t8,216(sp); sw t8,132(sp)`); a plain
 *     PName copy goes through an address (`addiu v0,sp,184; lw t1,0(v0)`).
 *   - The Parm inlines take the owner BEFORE the object pointer (o, t, ...): the Parm<float> object pointer is
 *     then the 4th address word (sp+136) as in the target.
 *   - ParmC's value member has the parameter's own type (Colour), so `v = val` is a direct struct move with no
 *     extra address word below the Colour parameters (frame 224, not 232).
 *   - ColourT's ctor routes r / g through `me()->set2(r_, g_)`: the materialised float parameters and the
 *     me() word put ParmC's object pointer at sp+84 and its Colour parameters at sp+64 / 48 / 32.
 */
extern "C" void *mem_new(unsigned int);
extern "C" char D_00000000[];

struct PName { char *s; };
struct PNameT : PName { PNameT(char *p) { s = p; } };
struct ColourP { float r, g, b, a; };
struct Colour : ColourP {
    Colour *me() { return this; }
    void set2(float r_, float g_) { r = r_; g = g_; }
};
struct ColourT : Colour {
    ColourT(float r_, float g_, float b_, float a_) { me()->set2(r_, g_); b = b_; a = a_; }
};

extern "C" void *GdlParmBase_ctor(void *self, void *owner, PName name, int flag); /* Parms_addParm */
extern "C" void *SNode_ctor(void *self);

struct GdlParm { char hdr[0xC]; char *vt; };
struct ParmF : GdlParm { float v; int f14; };
struct ParmI : GdlParm { int v; int f14; };
struct ParmC : GdlParm { Colour v; int f20; };
struct ParmsHead { char *vt; int count; };
struct Parms : ParmsHead { ParmF p08; ParmF p20; ParmC p38; ParmI p5C; ParmI p74; };
struct SnowTarget {
    char pad00[0xC];
    /* 0x0C */ char *name;
    char pad10[0x28 - 0x10];
    /* 0x28 */ char *vt;
    char pad2C[0x88 - 0x2C];
    /* 0x88 */ float col[4];
    /* 0x98 */ float f98, f9C, fA0, fA4, fA8;
    char padAC[0x22C - 0xAC];
    /* 0x22C */ int f22C, f230, f234, f238, f23C;
    /* 0x240 */ Parms parms;
};

#define PARM_NAME(off) PNameT(*(char **)(D_00000000 + (off)))

inline ParmF *ParmF_ctor(Parms *o, ParmF *t, PName n, float val, int fl) {
    if (t != 0 || (t = (ParmF *)mem_new(sizeof(ParmF))) != 0) {
        GdlParmBase_ctor(t, o, n, fl);
        t->vt = D_00000000 + 0x838;
        t->v = val;
        t->f14 = 0;
    }
    return t;
}
inline ParmI *ParmI_ctor(Parms *o, ParmI *t, PName n, int val, int fl) {
    if (t != 0 || (t = (ParmI *)mem_new(sizeof(ParmI))) != 0) {
        GdlParmBase_ctor(t, o, n, fl);
        t->vt = D_00000000 + 0x868;
        t->f14 = 0;
        t->v = val;
    }
    return t;
}
inline ParmC *ParmC_ctor(Parms *o, ParmC *t, Colour val, Colour lo, Colour hi, PName n, int fl) {
    if (t != 0 || (t = (ParmC *)mem_new(sizeof(ParmC))) != 0) {
        GdlParmBase_ctor(t, o, n, fl);
        t->vt = D_00000000 + 0x850;
        t->f20 = 0;
        t->v = val;
    }
    return t;
}
inline ParmsHead *ParmsHead_ctor(ParmsHead *t) {
    if (t != 0 || (t = (ParmsHead *)mem_new(sizeof(ParmsHead))) != 0) {
        t->vt = D_00000000 + 0xA34;
        t->count = 0;
    }
    return t;
}
inline Parms *Parms_ctor(Parms *t) {
    if (t != 0 || (t = (Parms *)mem_new(sizeof(Parms))) != 0) {
        ParmsHead_ctor(t);
        ParmF_ctor(t, &t->p08, PARM_NAME(0xA44), 40.0f, 1);
        ParmF_ctor(t, &t->p20, PARM_NAME(0xA48), 10.0f, 1);
        ParmC_ctor(t, &t->p38, ColourT(1.0f, 1.0f, 1.0f, 1.0f), ColourT(0.0f, 0.0f, 0.0f, 0.0f),
                   ColourT(1.0f, 1.0f, 1.0f, 1.0f), PARM_NAME(0xA4C), 1);
        ParmI_ctor(t, &t->p5C, PARM_NAME(0xA50), 0, 0);
        ParmI_ctor(t, &t->p74, PARM_NAME(0xA54), 3, 1);
    }
    return t;
}

extern "C" SnowTarget *snow_target_ctor(SnowTarget *self) {
    if (self != 0 || (self = (SnowTarget *)mem_new(sizeof(SnowTarget))) != 0) {
        SNode_ctor(self);
        self->vt = D_00000000;
        Parms_ctor(&self->parms);
        self->name = D_00000000 + 0xA58;
        /* one store per line: as1 keeps a .loc group's stores in source order only when split */
        self->f22C = 0;
        self->f230 = 0;
        self->f234 = 1;
        self->f238 = 3;
        self->f23C = 0;
        self->col[3] = 1.0f; self->col[2] = 1.0f; self->col[1] = 1.0f; self->col[0] = 1.0f;
        self->f98 = 0.0f; self->f9C = 0.0f; self->fA0 = 0.0f; self->fA4 = 0.0f; self->fA8 = 0.0f;
    }
    return self;
}
