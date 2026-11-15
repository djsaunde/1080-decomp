#include "common.h"

/* 64-bit libgcc-style helpers, statically linked into the game_libs USO.
 * Byte-identical masked-twins (0 relocs, self-contained) of the matched kernel
 * kernel_056.c family. Compile -O2 -mips3 so IDO inlines the d-arithmetic
 * (ddivu/dmultu/dsrav/...) instead of emitting jal back to themselves. The
 * "should not be used for ucode 32-bit compiles" warning is expected. Spliced
 * into game_libs_post1b2c.c.o (default -O2 -mips2) via REPLACE_FUNC_BODY, same
 * recipe as the landed gl___ull_divremi divmod. */

unsigned long long gl___ull_rem(unsigned long long a, unsigned long long b) { return a % b; }  /* __ull_rem  <- k___ull_rem */
unsigned long long gl___ll_rem(unsigned long long a, unsigned long long b) { return a % b; }  /* __ull_rem (dup) <- k___ll_rem */
unsigned long long gl___ull_div(unsigned long long a, unsigned long long b) { return a / b; }  /* __ull_div  <- k___ull_div */
unsigned long long gl___ll_lshift(unsigned long long a, unsigned long long b) { return a << b; } /* __ll_lshift <- k___ll_lshift */
long long gl___ll_div(long long a, long long b) { return a / b; }  /* __ll_div <- k___ll_div */
long long gl___ll_mul(long long a, long long b) { return a * b; }  /* __ll_mul <- k___ll_mul */
long long gl___ll_rshift(long long a, long long b) { return a >> b; } /* __ll_rshift <- k___ll_rshift */
unsigned long long gl___ull_rshift(unsigned long long a, unsigned long long b) { return a >> b; } /* __ull_rshift (entry 0x6C714; the old "6C710" symbol carried 6C484's inter-object pad word, agent-g 2026-09-09) */

/* gl___ll_mod (Euclidean mod <- k___ll_mod) LANDED 2026-06-21 in the
 * -O1 -mips3 donor game_libs_o1_6C8AC.c, NOT here: the divergence was OPT_FLAGS
 * (this unit is -O2 → leaf-optimizes to 33 insns), not TU-context. At -O1 -mips3
 * IDO keeps arg-homing → byte-exact 39-insn (0x9c) form. */
