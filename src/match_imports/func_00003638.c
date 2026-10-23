/* Native original-compiler source port from src/bootup_uso/bootup_uso.c. Target bytes unchanged. */
#include "context_bootup_uso.h"
Bootup3638Node *func_00003638(void *arg0, void *arg1, int arg2, int arg3,
                            int arg4, char *arg5, float arg6, float arg7,
                            int arg8, int arg9) {
    Bootup3638Node *node;
    Bootup3638Node *p;
    func_00000000_3638_v(&D_000074DC);
    arg8 |= 2;
    node = func_3638_node(0, arg1, ++arg2, ++arg3, arg4, arg6, arg7, arg8, arg9);
    p = node;
    func_3638_attach(arg5 + 0x10, node);
    if (p->parent != 0) {
        p->linked = 1;
    }
    p->parent = arg5;
    if (arg8 & 0xC) {
        func_3638_notify(arg0, node, 0);
    } else {
        func_3638_notify(arg0, node, 1);
    }
    func_3638_finalize();
    return node;
}
