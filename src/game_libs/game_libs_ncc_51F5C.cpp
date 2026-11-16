#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for BasePolyShape_parseMats: token-stream interpreter; the switch is edg's case-order compare chain (100, 101, 0), which cfe could not lay out (95.4 cap).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
#include "../../include/gdl/material.h" /* BaseMat: id, next */
#include "../../include/gdl/polyshape.h" /* BasePolyShape (arg0): mats, matCap, nMats */
extern "C" {
extern int D_00000000;
int gl_func_00034458_51F5C(...);
}
extern "C" void BasePolyShape_parseMats(char *arg0, int *arg1) {
    char *node;
    char *prev;
    int idx;
    int done;
    int cmd;
    int *vec;
    int k;
    char *n;

    node = 0;
    prev = 0;
    *(char **)&D_00000000 = arg0;
    idx = 0;
    done = 0;
    do {
        int *c = arg1;
        arg1 = c + 1;
        cmd = *c;
        switch (cmd) {
        case 100:
            prev = node;
            vec = (int *)(arg0 + 0x84);
            if (idx >= BASEPOLY(arg0)->matCap) {
                XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F38, cmd);
            }
            node = (char *)XCALL(gl_func_00034458_51F5C, Mat_clone_0004FBA4)(arg0, &arg1);
            n = node;
            if (BASEPOLY(arg0)->nMats >= BASEPOLY(arg0)->matCap) {
                XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F4C);
            }
            k = vec[3];
            vec[3] = k + 1;
            ((char **)vec[0])[k] = n;
            ((BaseMat *)node)->id = idx;
            idx++;
            break;
        case 101:
            if (prev == 0) {
                XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F68, cmd);
            }
            ((BaseMat *)prev)->next = (BaseMat *)node; /* multi-texture chain */
            break;
        case 0:
            done = 1;
            break;
        default:
            XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F78, cmd);
            break;
        }
    } while (done == 0);
}
