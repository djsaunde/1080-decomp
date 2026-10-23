/* Native original-compiler source port from src/h2hproc_uso/h2hproc_uso.c. Target bytes unchanged. */
#include "context_h2hproc_uso.h"
void h2hproc_uso_func_0000002C(int *a0, int a1) {
    H2HLinkNode *s0;
    int t1;
    int flag = 0;
    int resource = 0x01450000;
    do {
        switch (a1) {
            case 0:
                h2hproc_2C_call_0000020C(a0);
                ((H2HStatePrefix *)&D_00000000)->field_40 = 1;
                break;
            case 1:
                h2hproc_2C_import_9(a0, 2, 7, 1);
                flag = 1;
                ((H2HStatePrefix *)&D_00000000)->field_44 = 5;
                ((H2HStatePrefix *)&D_00000000)->field_48 = 6;
                break;
            case 2:
                h2hproc_2C_import_9(a0, 2, 7, 1);
                flag = 1;
                ((H2HStatePrefix *)&D_00000000)->field_44 = 5;
                ((H2HStatePrefix *)&D_00000000)->field_48 = 6;
                break;
            case 3:
                h2hproc_2C_import_9(a0, 2, 7, 2);
                flag = 1;
                ((H2HStatePrefix *)&D_00000000)->field_44 = 5;
                ((H2HStatePrefix *)&D_00000000)->field_48 = 6;
                break;
            case 4:
                h2hproc_2C_import_9(a0, 2, 7, 4);
                flag = 1;
                ((H2HStatePrefix *)&D_00000000)->field_44 = 5;
                ((H2HStatePrefix *)&D_00000000)->field_48 = 6;
                break;
            case 5:
                h2hproc_2C_import_10(&D_00000000, 4,
                                 ((H2HStatePrefix *)&D_00000000)->field_64, 4);
                flag = 1;
                t1 = h2hproc_2C_import_11(a0, a0[0], 2);
                t1 = (int)h2hproc_2C_call_00000620(0, resource, t1, a0[2]);
                h2hproc_2C_import_13(a0, 0, t1);
                break;
            case 6:
                h2hproc_2C_call_00000274(a0);
                flag = 1;
                s0 = (H2HLinkNode *)h2hproc_2C_import_15(0, 1, 0);
                h2hproc_2C_import_16(((H2HStatePrefix *)&D_00000000)->field_10, s0);
                if (s0->field_14 != 0) {
                    s0->field_04 = 1;
                }
                s0->field_14 = &D_00000000;
                break;
        }
        a1 = ((H2HStatePrefix *)&D_00000000)->field_40;
    } while (flag == 0);
}
