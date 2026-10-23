/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
TimB5UiPanel *timproc_uso_b5_func_0000D14C(TimB5UiOwner *a0, int arg1, int arg2, u32 arg3,
                                 u32 arg4, u32 arg5, float arg6,
                                 float arg7, float arg8) {
    char *volatile r;
    int n2;
    char *obj;
    union { char *ptr; s32 index; } child;
    char *gc;
    volatile int pad[6];
    int savedbit;
    char *p;
    int n;
    if (0) { timproc_uso_b5_func_04DFFC(&arg1, &arg2); }
    child.ptr = (char *)timproc_uso_b5_d14c_alloc(0x2C4);
    if (child.ptr != 0) {
        obj = child.ptr;
        if (obj != 0 ||
            (obj = (char *)timproc_uso_b5_d14c_alloc(0x2B8)) != 0) {
            timproc_uso_b5_func_04C678(obj, &timproc_uso_b5_D_807FEE30 + 0x15C0);
            ((TimB5UiObject *)obj)->field_28 = (void *)(&timproc_uso_b5_D_807FDB64 + 0);
            timproc_uso_b5_func_00002B74(obj + 0x2C);
            timproc_uso_b5_func_000032C8(obj + 0x194);
        }
        { char *descriptor = &timproc_uso_b5_D_807FEE38 + 0x15C8; ((TimB5UiObject *)child.ptr)->field_28 = (void *)(&timproc_uso_b5_D_807FEE38 + 0x5E4); ((TimB5UiObject *)child.ptr)->field_0C = descriptor; }
        timproc_uso_b5_func_00003890(child.ptr);
    }
    r = child.ptr;
    timproc_uso_b5_func_07ACE0((char *)a0 + 0x10, child.ptr);
    if (((TimB5UiObject *)child.ptr)->field_14 != 0) {
        ((TimB5UiObject *)child.ptr)->field_04 = 1;
    }
    {
        char *rr = r;
        ((TimB5UiObject *)child.ptr)->field_14 = (TimB5UiObject *)a0;
        ((TimB5UiPanel *)rr)->field_2A4 = (float)arg1;
        ((TimB5UiPanel *)rr)->field_2B0 = arg2 - 1;
    }
    child.ptr = (char *)timproc_uso_b5_d14c_alloc(0x148);
    if (child.ptr != 0) {
        timproc_uso_b5_func_0546DC(child.ptr, &timproc_uso_b5_D_807FEE48 + 0x15D8,
                                   0.0f, 0.0f, 0.0f);
        ((TimB5UiObject *)child.ptr)->field_28 = (void *)(&timproc_uso_b5_D_807FDDEC + 0x57C);
        gc = (char *)timproc_uso_b5_d14c_alloc(0x16C);
        if (gc != 0) {
            float cf;
            timproc_uso_b5_func_0546DC(gc, &timproc_uso_b5_D_807FEE54 + 0x15E4,
                                       0.0f, 0.0f, 0.0f);
            cf = *(float *)((char *)&import_80807FB8 + 0x3A0);
            ((TimB5UiObject *)gc)->field_28 = (void *)(&import_80087FB8[0]); (*(int *)((char *)(gc) + (0x120))) = 0xFFFF;
            *(float *)(gc + 0x108) = cf;
            *(float *)(gc + 0x10C) = cf;
            *(float *)(gc + 0x110) = cf;
            *(float *)(gc + 0x124) = 1.0f;
        }
        *(char **)(child.ptr + 0x108) = gc;
        timproc_uso_b5_func_04DFFC(child.ptr, gc);
        if (1) {}
        p = *(char **)(child.ptr + 0x108);
        p += 0xB4;
        do {
            ((struct TimB5V3 *)p)->x = 0.0f;
            ((struct TimB5V3 *)p)->z = 0.0f;
            ((struct TimB5V3 *)p)->y = 100.0f;
        } while (0);
        *(float *)(child.ptr + 0x130) = *(float *)((char *)&import_80800078 + 0x3A4);
        n = *(int *)&import_80020078;
        if (1) {}
        p = (char *)timproc_uso_b5_d14c_alloc(0xB4);
        if (p != 0) {
            import_0010D33C(p, n);
            ((TimB5UiObject *)p)->field_28 = (void *)(&timproc_uso_b5_D_807FDAB8 + 0x248);
            (*(int *)((char *)(p) + (0xB0))) = 0;
            timproc_uso_b5_func_077574(p);
        }
        *(char **)(child.ptr + 0x10C) = p;
        timproc_uso_b5_func_04DFFC(child.ptr, p);
        savedbit = d14c_ee6c_l1.flags & 0x80000; if (1) {} { int x = (d14c_ee6c_l2.flags & 0xFFF7FFFF) | 0x20000 | 0x2000 | 2 | 1; d14c_ee6c_s1.flags = x; } d14c_ee64_st = d14c_ee64_ld & ~8; p = *(char **)(child.ptr + 0x10C); timproc_uso_b5_func_00B1B4(arg2, 0, 0x201, p, child.ptr);
        (*(int *)((char *)(child.ptr) + (0x140))) = ((s32 *)&timproc_uso_b5_D_807FDB4C)[arg2];
        timproc_uso_b5_func_077C44(*(char **)(child.ptr + 0x10C), (*(int *)((char *)(child.ptr) + (0x140))), 0, 2, 2, 1);
        if (1) {}
        {
            char *p2 = *(char **)(child.ptr + 0x10C);
            struct TimB5UiMethod24 *vt;
            char *adjusted;
            p = *(char **)(p2 + 0x28);
            vt = (struct TimB5UiMethod24 *)p;
            adjusted = p2 + vt->adjustment;
            vt->method(adjusted);
        }
        if (savedbit != 0) {
            int x = d14c_ee6c_l3.flags | 0x80000;
            d14c_ee6c_s2.flags = x;
        } else {
            int x = d14c_ee6c_l4.flags & 0xFFF7FFFF;
            d14c_ee6c_s3.flags = x;
        }
        *(float *)(child.ptr + 0x110) = arg6;
        *(float *)(child.ptr + 0x114) = arg7;
        *(float *)(child.ptr + 0x118) = arg8;
        *(float *)(child.ptr + 0x120) = 0.0f;
        *(float *)(child.ptr + 0x124) = d14c_fb8_a[0x3A8 / sizeof(f32)];
        *(float *)(child.ptr + 0x128) = d14c_fb8_b[0x3AC / sizeof(f32)];
        (*(int *)((char *)(child.ptr) + (0x144))) = 1;
        (*(int *)((char *)(child.ptr) + (0x138))) = 0;
        (*(int *)((char *)(child.ptr) + (0x134))) = 0;
    }
    {
        char *rr = r;
        ((TimB5UiPanel *)rr)->field_2B8 = (TimB5UiObject *)child.ptr;
        ((TimB5UiPanel *)rr)->field_134 = d14c_fb8_c[0x3B0 / sizeof(f32)];
        ((TimB5UiPanel *)rr)->field_29C = (TimB5UiObject *)child.ptr;
    }
    obj = (char *)a0->field_38;
    timproc_uso_b5_func_07ACE0(obj + 0x10, child.ptr);
    if (((TimB5UiObject *)child.ptr)->field_14 != 0) {
        ((TimB5UiObject *)child.ptr)->field_04 = 1;
    }
    ((TimB5UiObject *)child.ptr)->field_14 = (TimB5UiObject *)obj;
    child.index = a0->field_6C;
    a0->field_6C = child.index + 1;
    a0->field_3C[child.index] = (TimB5UiObject *)r;
    return (TimB5UiPanel *)r;
}
