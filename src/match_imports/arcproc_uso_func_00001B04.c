/* Native original-compiler source port from src/arcproc_uso/arcproc_uso_tail1.c. Target bytes unchanged. */
#include "context_arcproc_uso_tail1.h"
void arcproc_uso_func_00001B04(Arc1B04Object *arg) {
    int result;
    result = arcproc_uso_import_165_1B04(&D_arc1B04_a, 0x40100);
    if (result == 0) return;
    result = arcproc_uso_import_116_1B04(arg->unk50);
    if (result != 0) {
        arg->unk60 = 1;
        ((int *)&D_arc1B04_a)[0x40 / 4] = 8;
        ((int *)&D_arc1B04_a)[0x44 / 4] = 6;
    } else {
        ((int *)&D_arc1B04_a)[0x40 / 4] = 6;
        arcproc_uso_import_117_1B04(arg, 7, 0);
    }
}
