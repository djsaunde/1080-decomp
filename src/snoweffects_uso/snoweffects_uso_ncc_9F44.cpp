/* IDO 7.1 NCC (C++) EXACT donor for SnowLink_update_00009F44 (0xA4 / 41 words), agent-h run 621.
 * REPLACE_FUNC_BODY-spliced over the INCLUDE_ASM placeholder in snoweffects_uso.c on both build paths.
 * The tell that this is C++: every Parm value read keeps the parm's base pointer (addiu v1, v0, 0x114 then
 * lwc1 0x10(v1)), the shape an inline `Parm<T>::get()` returning a reference leaves; cfe folds the offsets.
 * The Vec3 copy at the end is a user operator= (all three loads before the stores, src a2 / dst a1).
 * Copies the source's position parm into this+0x40, then the target's colour / two float parms and
 * position into its draw fields, and sets a global dirty flag (blank lui / sw off the D_00000000 pin). */
extern "C" char D_00000000[];

struct Vec3 {
    float x, y, z;
    Vec3 &operator=(const Vec3 &o) { float a = o.x, b = o.y, c = o.z; x = a; y = b; z = c; return *this; }
};
struct Quad { int a, b, c, d; };

/* a libgdl HostIO parm (gdl/parm.h GdlParmBase header, value from +0x10) */
template <class T> struct Parm { char hdr[0x10]; T v; const T &get() const { return v; } };

struct SnowTarget {
    char pad00[0x5C];
    /* 0x5C */ Vec3 pos;
    char pad68[0x88 - 0x68];
    /* 0x88 */ Quad colour;
    /* 0x98 */ float f98;
    /* 0x9C */ float f9C;
    /* 0xA0 */ Vec3 drawPos;
    char padAC[0x248 - 0xAC];
    /* 0x248 */ Parm<float> parm248;
    char pad25C[0x260 - 0x25C];
    /* 0x260 */ Parm<float> parm260;
    char pad274[0x278 - 0x274];
    /* 0x278 */ Parm<Quad> colourParm;
};
struct SnowSource { char pad[0x114]; /* 0x114 */ Parm<Vec3> posParm; };
struct SnowLink {
    char pad00[0x30];
    /* 0x30 */ SnowSource *src;
    char pad34[4];
    /* 0x38 */ SnowTarget *tgt;
    char pad3C[4];
    /* 0x40 */ Vec3 pos;
};

extern "C" void SnowLink_update_00009F44(SnowLink *self) {
    if (self->src) {
        const Vec3 &v = self->src->posParm.get();
        self->pos.x = v.x;
        self->pos.y = v.y;
        self->pos.z = v.z;
    }
    if (self->tgt) {
        self->tgt->colour = self->tgt->colourParm.get();
        self->tgt->f98 = self->tgt->parm248.get();
        self->tgt->f9C = self->tgt->parm260.get();
        self->tgt->drawPos = self->tgt->pos;
    }
    *(int *)D_00000000 = 1;
}
