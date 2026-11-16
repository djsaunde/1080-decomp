#include "common.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern char D_00000000;
typedef struct { int a, b, c, d; } Quad4;

void boarder4_uso_func_00000000(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadQuad_boarder4_0000003C(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void boarder4_readQuadParm_00000094(char *dst) {
    int tmp;
    /* UNNAMED: sentinel callee is boarder4_uso_func_00000000, this module's own Text+0 function (a *_00000000 name, kept) */
    boarder4_uso_func_00000000(&tmp);
    fs_gdlReadQuad_boarder4_0000003C((Quad4*)(dst + 0x10));
}
#pragma GLOBAL_ASM("asm/nonmatchings/boarder4_uso/boarder4_uso/boarder4_uso_func_00000094_pad.s")

void fs_gdlReadInt_boarder4_000000D0(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void fs_gdlReadQuad_boarder4_0000010C(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void boarder4_readQuadParm_00000164(char *dst) {
    int tmp;
    fs_gdlReadInt_boarder4_000000D0(&tmp);
    fs_gdlReadQuad_boarder4_0000010C((Quad4*)(dst + 0x10));
}
#pragma GLOBAL_ASM("asm/nonmatchings/boarder4_uso/boarder4_uso/boarder4_uso_func_00000164_pad.s")
