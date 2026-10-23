/* Native original-compiler source port from src/h2hproc_uso/h2hproc_uso.c. Target bytes unchanged. */
#include "context_h2hproc_uso.h"
extern char snw_h2h_1360_text_data;
extern char snw_h2h_1360_import149;
void h2hproc_uso_func_00001360(int *self) {
    char scratch[64];
    int *vtable;
    int *v_call;
    int retval;
    int gate;
    char c;
    *(int*)((char*)self + 0x98) += 1;
    gate = (int)*(int*)((char*)*(int*)((char*)self + 0x34) + 0x28);
    ((void(*)(int))((int*)gate)[0x1C/4])(*(int*)((char*)self + 0x34) + *(short*)((char*)gate + 0x18));
    h2hproc_1360_import_157((*(H2HParentState **)((char *)self + 0x2C))->field_568);
    h2hproc_1360_import_158((*(H2HParentState **)((char *)self + 0x2C))->field_568);
    gate = ((*(H2HParentState **)((char *)self + 0x2C))->field_6B8 == 0)
         ? *(int*)((char*)self + 0x98) : 0x10;
    if (gate & 0x10) {
        h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x3FC);
        retval = h2hproc_1360_import_160((*(H2HParentState **)((char *)self + 0x2C))->field_568, 80, scratch);
        h2hproc_1360_import_161((*(H2HParentState **)((char *)self + 0x2C))->field_568, retval, 30, scratch);
        {
            retval = (*(H2HParentState **)((char *)self + 0x2C))->field_6A8[0]; if (retval != 1) c = 115; else c = 0; h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x400, retval, c);
        }
        retval = h2hproc_1360_import_160((*(H2HParentState **)((char *)self + 0x2C))->field_568, 80, scratch);
        h2hproc_1360_import_161((*(H2HParentState **)((char *)self + 0x2C))->field_568, retval, 80, scratch);
    }
    gate = ((*(H2HParentState **)((char *)self + 0x2C))->field_6B8 == 1)
         ? *(int*)((char*)self + 0x98) : 0x10;
    if (gate & 0x10) {
        h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x40C);
        retval = h2hproc_1360_import_160((*(H2HParentState **)((char *)self + 0x2C))->field_568, 240, scratch);
        h2hproc_1360_import_161((*(H2HParentState **)((char *)self + 0x2C))->field_568, retval, 30, scratch);
        {
            retval = (*(H2HParentState **)((char *)self + 0x2C))->field_6A8[1]; if (retval != 1) c = 115; else c = 0; h2hproc_1360_import_159(scratch, &snw_h2h_1360_text_data + 0x410, retval, c);
        }
        retval = h2hproc_1360_import_160((*(H2HParentState **)((char *)self + 0x2C))->field_568, 240, scratch);
        h2hproc_1360_import_161((*(H2HParentState **)((char *)self + 0x2C))->field_568, retval, 80, scratch);
    }
    h2hproc_1360_import_162(&snw_h2h_1360_import149);
    v_call = (int*)((char*)self + 0x38);
    h2hproc_1360_import_163(v_call);
    h2hproc_1360_import_164(v_call, 60, 64, 3);
    v_call = (int*)((char*)self + 0x50);
    h2hproc_1360_import_163(v_call);
    vtable = (int*)*(int*)((char*)self + 0x48);
    if (1) {}
    retval = *(short*)((char*)vtable + 0x20);
    h2hproc_1360_import_164(v_call, retval + 68, 64, 3);
    v_call = (int*)((char*)self + 0x68);
    h2hproc_1360_import_163(v_call);
    h2hproc_1360_import_164(v_call, 220, 64, 3);
    v_call = (int*)((char*)self + 0x80);
    h2hproc_1360_import_163(v_call);
    vtable = (int*)*(int*)((char*)self + 0x78);
    if (1) {}
    retval = *(short*)((char*)vtable + 0x20);
    h2hproc_1360_import_164(v_call, retval + 228, 64, 3);
    h2hproc_1360_import_165(self);
}
