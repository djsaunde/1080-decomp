#include "common.h"

/* gl___osSetGlobalIntMask = libultra __osSetGlobalIntMask (os/setglobalintmask.c),
 * statically linked into the game_libs USO. Byte-exact masked-twin of matched
 * kernel __osSetGlobalIntMask (kernel_011_b): disable-int, OR mask into the global
 * int-mask word, restore-int. IDO 5.3 -O1, spliced into game_libs_post1b2c.c.o
 * via REPLACE_FUNC_BODY. Both OS-API callees collapse to the address-0
 * placeholder gl_func_00000000; the global int-mask word (kernel __OSGlobalIntMask)
 * maps to the zero-resolving global D_00000000. */
extern int gl_func_00000000();
extern int D_00000000;

void gl___osSetGlobalIntMask(int mask) {
    register int sr = gl_func_00000000();
    D_00000000 |= mask;
    gl_func_00000000(sr);
}
