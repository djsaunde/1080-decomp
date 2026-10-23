/* Native original-compiler source port from src/arcproc_uso/arcproc_uso_tail1.c. Target bytes unchanged. */
#include "context_arcproc_uso_tail1.h"
void arcproc_uso_func_00001F54(ArcPanel *arg0) {
    ArcContext *bc;
    ArcPanelData *p44;
    unsigned int word;
    int rem;
    int v0;
    int a;
    bc = arg0->fieldBC;
    if ((bc->field4F0 & 0x10000) && (bc->field4DC == 1)) {
        arg0->field30 = arg0->field30 + 0x21;
        arcproc_import_179(arg0);
        arcproc_import_180(arg0);
        arg0->fieldA0 -= 0x21;
        if (arg0->fieldA0 < 0) {
            arg0->fieldA0 = 0;
            arcproc_import_181(arg0->field84, 0, 0);
            arcproc_import_182(arg0->fieldBC);
            arg0->field2C = 0;
        }
    }
    arcproc_import_183(arg0);
    bc = arg0->fieldBC;
    if (bc->field4F0 & 0x10000) {
        if ((bc->field4DC == 1) && (arg0->field44->field34 != 0)) {
            arcproc_import_184(arg0);
            arcproc_import_185(arg0, arg0->fieldA0);
            arcproc_import_181(arg0->field84, arg0->fieldA0, 0);
            arcproc_import_186(arg0->fieldBC);
        }
        rem = arg0->field44->field18 - arg0->fieldA8;
        if (rem >= 0xFA) {
            arg0->fieldA8 += 0xFA;
        } else if (rem >= 0x32) {
            arg0->fieldA8 += 0x32;
        } else if (rem >= 0x19) {
            arg0->fieldA8 += 0x19;
        } else if (rem > 0) {
            arg0->fieldA8 += 1;
        }
        arcproc_import_174(arg0->field8C, arg0->fieldA8);
        v0 = arg0->fieldBC->field4DC;
        if ((v0 == 1) || (v0 == 0)) {
            arcproc_import_181(arg0->field84, arg0->fieldA0, 1);
            arcproc_import_187(arg0->field80, arg0->field44->field30, 0.0f);
            p44 = arg0->field44;
            arcproc_import_188(arg0->field80, p44->field08, p44->field0C, 0.0f, 0.0f);
        }
        if (arg0->field44->field1C != 0) {
            arcproc_import_177(arg0->field94, 0xA0, 0x46, 1.0f, 1.0f);
            arg0->field94->field78 = 0xFF;
            word = (unsigned int)&arg0->field94->field18;
            *(int *)word = *(int *)word | 4;
            p44 = arg0->field44;
            a = *(int *)&p44->field20;
            word = p44->field1C - a;
            if ((int)word >= 0x32) {
                word = (unsigned int)p44;
                if (1) { word += 0x20; }
                *(unsigned int *)word += 0x32;
                arcproc_import_189(0x20, 0x32);
                a = arg0->field44->field20;
            } else if ((int)word >= 5) {
                word = (unsigned int)p44;
                if (1) { word += 0x20; }
                *(unsigned int *)word += 5;
                arcproc_import_189(0x20, 5);
                a = arg0->field44->field20;
            } else if ((int)word > 0) {
                p44->field20 = p44->field20 + 1;
                arcproc_import_189(0x20, 1);
                a = arg0->field44->field20;
            }
            arcproc_import_176(arg0->field94, a);
        } else {
            word = (unsigned int)&arg0->field94->field18;
            *(int *)word = *(int *)word & ~4;
        }
        bc = arg0->fieldBC;
        if ((bc->field4DC == 2) && (bc->field4F8 == 0)) {
            arg0->fieldAC += 1;
            if (arg0->fieldAC & 8) {
                word = (unsigned int)&arg0->field8C->field18;
                *(int *)word = *(int *)word & ~4;
            } else {
                word = (unsigned int)&arg0->field8C->field18;
                *(int *)word = *(int *)word | 4;
            }
        }
    }
    arcproc_import_40(arg0);
}
