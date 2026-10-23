/* Native original-compiler source port from src/titproc_uso/titproc_uso.c. Target bytes unchanged. */
#include "context_titproc_uso.h"
void titproc_uso_func_00001710(TitprocBlendObject *object) {
    float ones[4];
    char *tgt;
    char local_gap[4];
    float green, blue;
    float t;
    float red;
    float *src;
    float vec[4];
    float out[4];
    src = ones;
    red = 1.0f;
    ones[0] = red; ones[1] = red; ones[2] = red; ones[3] = red;
    if (*(int *)((char *)&import_00020098 + 0x18C) != 0) {
        red = ones[0];
        t = *(float *)((char *)&import_00263E00 + 0x40);
        vec[0] = red; vec[3] = red; vec[1] = t; vec[2] = t;
        t = (float)object->field_2C / 255.0f;
        out[3] = red;
        red = (vec[0] - src[0]) * t + src[0];
        green = (vec[1] - src[1]) * t + src[1];
        blue = (vec[2] - src[2]) * t + src[2];
        out[0] = red; out[1] = green; out[2] = blue;
        import_0024E608(&import_0024CAF8, object->field_2C, out);
        tgt = (char *)&import_00263D30 + 0x18;
    } else {
        import_0024E608(&import_0024CAF8, object->field_2C, ones);
        tgt = (char *)&import_00263D30;
    }
    import_0024F2C8(tgt);
    import_0024F34C(tgt, (int)object->field_34, (int)object->field_38, 3);
}
