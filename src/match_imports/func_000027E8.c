/* Native original-compiler source port from src/bootup_uso/bootup_uso.c. Target bytes unchanged. */
#include "context_bootup_uso.h"
BootupObjectBase *func_000027E8(BootupSceneSetup *scene, int a1, int a2, int a3) {
    BootupObjectBase *s4C, *s48, *s44, *s40, *s3C;
    int sv;
    BootupObjectBase *g;
    char *p;
    int m;
    f32 fd;
    sv = (int)bootup_27E8_func_0497D0(&bootup_27E8_timer_object);
    scene->field_3c = a2;
    bootup_27E8_func_074710(&bootup_27E8_ref_7328, 0);
    bootup_27E8_func_0118E4(&bootup_27E8_registration_context, &bootup_27E8_ref_7334);
    bootup_27E8_func_002C94(scene->field_134, a2);
    bootup_27E8_func_074710(&bootup_27E8_ref_7344, 0);
    bootup_27E8_func_0118E4(&bootup_27E8_registration_context, (char *)&D_00000000 + 0x7350);
    bootup_27E8_func_0118E4(&bootup_27E8_registration_context, &bootup_27E8_ref_7360);
    bootup_27E8_func_074840();
    bootup_27E8_func_074710(&bootup_27E8_ref_7374, 0);
    s3C = bootup_27E8_func_04C678(0, (s32)(&bootup_27E8_ref_7380));
    s4C = bootup_27E8_func_04C678(0, (s32)(&bootup_27E8_ref_738C));
    s48 = bootup_27E8_func_04C678(0, (s32)(&bootup_27E8_ref_7390));
    s44 = bootup_27E8_func_04C678(0, (s32)(&bootup_27E8_ref_7394));
    s40 = bootup_27E8_func_04C678(0, (s32)((char *)&D_00000000 + 0x7398));
    g = bootup_27E8_func_050E80(0);
    g = *(BootupObjectBase **)&bootup_27E8_singleton_storage = g;
    bootup_27E8_func_07ACE0((char *)s4C + 0x10, g);
    if (g->field_14) g->field_04 = 1;
    g->field_14 = s4C;
    bootup_27E8_func_074840();
    bootup_27E8_func_002060(scene->field_134, s48, s4C, s3C, s44, s40);
    bootup_27E8_func_0020A4(scene->field_134);
    bootup_27E8_func_002080(scene->field_134);
    bootup_27E8_func_002F90(scene->field_134);
    m = (int)bootup_27E8_func_0497D0(&bootup_27E8_timer_object);
    fd = (f32)bootup_27E8_scale1.field_10;
    bootup_27E8_func_048E7C(&bootup_27E8_ref_739C,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    sv = (int)bootup_27E8_func_0497D0(&bootup_27E8_timer_object);
    scene->field_134->field_98 = bootup_27E8_func_006808(scene->field_134, bootup_27E8_func_002014(scene->field_134, a1), a2);
    m = (int)bootup_27E8_func_0497D0(&bootup_27E8_timer_object);
    fd = (f32)bootup_27E8_scale2.field_10;
    bootup_27E8_func_048E7C(&bootup_27E8_ref_73B4,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    sv = (int)bootup_27E8_func_0497D0(&bootup_27E8_timer_object);
    bootup_27E8_func_002DA4(scene->field_134, a2, a3);
    m = (int)bootup_27E8_func_0497D0(&bootup_27E8_timer_object);
    fd = (f32)bootup_27E8_scale3.field_10;
    bootup_27E8_func_048E7C(&bootup_27E8_ref_73CC,
                  ((f32)(u32)m - (f32)(u32)sv) / fd);
    p = (char *)s48 + 0x10;
    bootup_27E8_func_07ACE0(p, s44);
    if (s44->field_14) s44->field_04 = 1;
    s44->field_14 = s48;
    bootup_27E8_func_07ACE0(p, s40);
    if (s40->field_14) s40->field_04 = 1;
    s40->field_14 = s48;
    bootup_27E8_func_074840();
    p = (char *)s3C + 0x10;
    bootup_27E8_func_07ACE0(p, s4C);
    if (s4C->field_14) s4C->field_04 = 1;
    s4C->field_14 = s3C;
    bootup_27E8_func_07ACE0(p, s48);
    if (s48->field_14) s48->field_04 = 1;
    s48->field_14 = s3C;
    return s3C;
}
