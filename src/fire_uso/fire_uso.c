#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;

/* fire.uso: the fire course object (an SNode subclass with a Sprite and two
 * gfx colour parms) and its ItemType template. Compiler: IDO 7.1 cfe -O2 matches
 * the members below; the module is libgdl C++ (vtables at Data+0x228C / 0x231C,
 * cfront-style this==0 allocating ctors), written here as C.
 *
 * Relocs (scripts/uso-imports.py sites fire): every `&D_00000000` is a zero
 * placeholder; the target is in the TextReloc. Imports from bootup.uso:
 * Bss+0x3C168 = GfxLib, Bss+0x3C768 = the hostio object. */

/* fire_uso_func_00000000: FireTmpl ctor (0x68 bytes). not matched yet (an NM body scored 78%):
 * the whole control flow, the inlined 0x48-byte ItemType base ctor (name passed
 * by value) and the store order match; two residuals: the target keeps the
 * name copy at sp+0x1C (two more frame words, likely the inlined Colour::set's
 * materialised this/params) and reloads 255.0f for the alpha dividend instead
 * of reusing the divisor register (cfe, NCC and value-variants all CSE it).
 * The divides are not folded, so the colours come from an inline
 * set(r, g, b, a) dividing by 255.0f (C needs locals for that, which cost frame words). */
INCLUDE_ASM("asm/nonmatchings/fire_uso/fire_uso", fire_uso_func_00000000);

int *fire_uso_func_00000270(char *self, int sprite);

/* fire_uso_func_00000148: FireTmpl create: new Fire(tmpl->sprite), scale 1,
 * virtual handleMsg (vtable slot at +0x28) with message 19, then link into the
 * sprite's list. not matched yet (an NM body scored 74%): the target keeps the new object in v1
 * (v0 reused for the vtable), homes it at sp+0x28 with the message at sp+0x2C
 * (frame 0x38), reads tmpl->sprite from a0 before the home store and leaves the
 * jalr delay slot empty; cfe and NCC (real class + new) both give frame 0x30. */
INCLUDE_ASM("asm/nonmatchings/fire_uso/fire_uso", fire_uso_func_00000148);

/* Fire_declColourParms: hostio colour parms for the env (0x48) and prim (0x58) colours. */
void fire_uso_func_000001E4(char *self) {
    CALL(gfx_declColourParm)(self + 0x48, &D_00000000 + 0x237C);
    CALL(gfx_declColourParm)(self + 0x58, &D_00000000 + 0x2380);
}

void fire_uso_func_00000224(char *self) {
    CALL(gfx_setPrimColour)(&D_00000000, self + 0x58);
    CALL(gfx_setEnvColour)(&D_00000000, self + 0x48);
    CALL(Node_display)(self);
}

int *fire_uso_func_00000270(char *self, int sprite) {
    if (self != 0 || (self = (char *)CALL(mem_new)(0x94)) != 0) {
        CALL(SNode_ctor)(self);
        *(char **)(self + 0x28) = &D_00000000;
        *(short *)(self + 0x8C) = 0;
        *(float *)(self + 0x90) = 0.0f;
        *(int *)(self + 0x88) = sprite;
        *(float *)(self + 0x6C) = 6.0f;
        CALL(Node_setUnk24)(self, 2);
    }
    return (int *)self;
}

void fire_uso_func_000002E8(char *self) {
    *(short *)(*(char **)(self + 0x88) + 0xC0) = *(short *)(self + 0x8C);
    *(float *)(*(char **)(self + 0x88) + 0xB0) = *(float *)(self + 0x70);
    *(float *)(*(char **)(self + 0x88) + 0xB4) = *(float *)(self + 0x70);
    *(short *)(self + 0x8C) = (*(short *)(self + 0x8C) + 1) & 7;
    CALL(SNode_display)(self);
}

extern void gl_func_00000000_ff(void *, char *, void *, float, float, int);
void fire_uso_func_00000338(char *self) {
    CALL(hostio_beginGroup)(&D_00000000, &D_00000000 + 0x2388, 0);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(&D_00000000, &D_00000000 + 0x2390, self + 0x90, 0.0f, 1.0f, 0);
    CALL(hostio_endGroup)(&D_00000000);
    CALL(SNode_hostioParms)(self);
}
