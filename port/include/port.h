/* Forced into every port compile (port/CMakeLists.txt).  PORT-only definitions
 * that cannot be passed as -D (CMake drops function-like macros). */
#ifndef PORT_PORT_H
#define PORT_PORT_H

/* asm-processor's bare GLOBAL_ASM("x.s") lines: the shim / port code replaces
 * the hand-written assembly (plan decision 6). */
#define GLOBAL_ASM(x)

#endif
