#include "common.h"

extern int func_00000000();
extern char D_00000000;
typedef struct { int a, b, c, d; } Quad4;

/* USO-base absolute placeholders (undefined_syms_auto.txt): the segment links
 * at VRAM 0, so `extern D_00000030` produces the same lui/addiu words as the
 * target's %hi/%lo(func_00000008 + 0x28) — the F7D0 trick, addend baked in
 * the addiu (form-a LO16-fold, unreachable via struct-member address-of). */
extern float D_00000030[4];    /* == func_00000008 + 0x28: Vec4f zeroed slot */
extern char D_0000002C;        /* == func_00000008 + 0x24 */
typedef struct {
    char pad44[0x44];
    char *ptr44;                /* +0x44: object holder, result stored at +0x114 */
} F0Struct;
extern F0Struct func_000000F0; /* USO-base reloc */
extern int func_00000188;      /* USO-base reloc */
extern char D_0000C57C;
extern char D_0000C58C;
extern char D_0000C594;

/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_F81C.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
void *func_0000F81C(int a0, int a1) {
    return 0;
}

/* func_0000F954 + func_0000F9E8 split out to bootup_uso_o0_F954.c on 2026-06-23
 * (cluster -O0 split; F954 is a byte-exact -O0 match, F9E8 stays NM at 1w off).
 * Moved together so tail1.c keeps only the -O2 func_0000F81C and the linker layout
 * stays contiguous. */

/* func_0000FAE8 split out to bootup_uso_o0_FAE8.c on 2026-06-23 — it's an -O0
 * function (reloads self each use, register-var bit-manip in s0-s3, if(1)
 * dead-branch); now a byte-exact -O0 C match there (same recipe as 100F0/118E4). */

/* func_000100F0 + func_0001016C split out to bootup_uso_o0_100F0.c on
 * 2026-05-14 for -O0 build (same recipe as func_000118E4 ->
 * bootup_uso_o0_118E4.c earlier this date). */
