#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;
typedef struct { int a, b, c, d; } Quad4;

/* USO entry-0: leading `beq zero,zero,+0x736F` trampoline (loader-patched
 * at runtime) followed by the standard -O2 int-reader template body. The
 * trampoline word is injected post-cc via PREFIX_BYTES (see Makefile +
 * scripts/inject-prefix-bytes.py). */
void boarder5_uso_func_00000000(int *dst) {
    int buf[2];
    /* UNNAMED: sentinel this is the sentinel-named Text+0 function (PREFIX_BYTES trampoline at word 0), which the call resolver cannot place; the ROM jal at Text+0x18 is fs_gdlReadFile */
#ifdef PORT /* plan 18.8: the gate tools cannot attribute this PREFIX_BYTES Text+0 function, so it is bound by
             * hand from its ROM TextRelocs: the jal (Text+0x18) is fs_gdlReadFile, the HI16 / LO16 at
             * +0xC / +0x10 bootup Bss+0x3C740 (gSysLib), as in the module's other fs_gdlRead* readers */
    { extern int fs_gdlReadFile(); extern char port_bootup_bss[]; fs_gdlReadFile(port_bootup_bss + 0x3C740, buf, 4); }
#else
    gl_func_00000000(&D_00000000, buf, 4);
#endif
    *dst = buf[0];
}

void fs_gdlReadQuad_boarder5_00000040(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

/* Original jals target fn-4 (the leading-nop true entries) — base ROM never
 * calls the splat-named entries; undefined_syms absolutes per the h2h_4DC
 * precedent. */
extern int boarder5_uso_func_b5_3C();
extern int boarder5_uso_func_b5_D0();
extern int boarder5_uso_func_b5_10C();
void boarder5_readQuadParm_00000098(char *dst) {
    int tmp;
    /* UNNAMED: sentinel callee is boarder5 Text+0 = boarder5_uso_func_00000000 (a *_00000000 name, kept) */
    SCALL(gl_func_00000000, boarder5_uso_func_00000000)(&tmp);
    boarder5_uso_func_b5_3C((Quad4*)(dst + 0x10));
}
#pragma GLOBAL_ASM("asm/nonmatchings/boarder5_uso/boarder5_uso/boarder5_uso_func_00000098_pad.s")

void fs_gdlReadInt_boarder5_000000D4(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
#ifdef PORT
    PORT_BE_SWAP32S(buf, 4);
#endif
    *dst = buf[0];
}

void fs_gdlReadQuad_boarder5_00000110(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
#ifdef PORT
    PORT_BE_SWAP32S(&buf, 16);
#endif
    *dst = buf;
}

void boarder5_readQuadParm_00000168(char *dst) {
    int tmp;
    boarder5_uso_func_b5_D0(&tmp);
    boarder5_uso_func_b5_10C((Quad4*)(dst + 0x10));
}
