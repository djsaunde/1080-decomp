#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for BasePolyShape_parseMats: token-stream interpreter; the switch is edg's case-order compare chain (100, 101, 0), which cfe could not lay out (95.4 cap).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
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
            /* RAW: donor-mirror: C++ donor (NCC Cfront); its local mirror structs cannot include the C headers */
            if (idx >= *(int *)(arg0 + 0x8C)) {
                XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F38, cmd);
            }
            node = (char *)XCALL(gl_func_00034458_51F5C, Mat_clone_0004FBA4)(arg0, &arg1);
            n = node;
            if (*(int *)(arg0 + 0x90) >= *(int *)(arg0 + 0x8C)) { /* RAW: donor-mirror (as above) */
                XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F4C);
            }
            k = vec[3];
            vec[3] = k + 1;
            ((char **)vec[0])[k] = n;
            *(int *)(node + 0x18) = idx; /* RAW: donor-mirror (as above) */
            idx++;
            break;
        case 101:
            if (prev == 0) {
                XCALL(gl_func_00034458_51F5C, sys_fatal)((char *)&D_00000000 + 0x20F68, cmd);
            }
            *(char **)(prev + 0x40) = node; /* RAW: donor-mirror (as above) */
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
