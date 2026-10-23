/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
char *timproc_uso_b5_func_0000D550(char *arg0, volatile s32 arg1, volatile s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    char *volatile r;
    char *g2;
    char *volatile obj;
    char *child;
    volatile int pad[7];
    s32 savedbit;
    char *p;
    char *gc;
    if (0) { timproc_uso_b5_func_04DFFC(&gc); }
    child = (char *)timproc_uso_b5_func_055750((char *)0x2C8);
    if (child != 0) {
        obj = child;
        if (child != 0 || (p = (char *)timproc_uso_b5_func_055750((char *)0x2B8), obj = p, p != 0)) {
            timproc_uso_b5_func_04C678(obj, &timproc_uso_b5_D_807FEE60 + 0x15F0);
            {
                char *o2 = obj;
                (*(s32 *)((char *)(o2) + 0x28)) = (s32) (&timproc_uso_b5_D_807FDB64 + 0);
                timproc_uso_b5_func_00002B74(o2 + 0x2C);
            }
            timproc_uso_b5_func_000032C8(obj + 0x194);
        }
        (*(s32 *)((char *)(child) + 0x28)) = (s32) (&timproc_uso_b5_D_807FEE68 + 0x6D4); (*(s32 *)((char *)(child) + 0xC)) = (s32) (&timproc_uso_b5_D_807FEE68 + 0x15F8); timproc_uso_b5_func_00003890(child);
        (*(s32 *)((char *)(child) + 0x2C4)) = 0;
        (*(s32 *)((char *)(child) + 0x2B4)) = 0;
    }
    r = child;
    timproc_uso_b5_func_07ACE0(arg0 + 0x10, child);
    if (((TimB5ObjectPrefix *)child)->field_14 != 0) {
        ((TimB5ObjectPrefix *)child)->field_04 = 1;
    }
    {
        char *rr = r;
        {
        s32 a2v = arg2;
        ((TimB5ObjectPrefix *)child)->field_14 = arg0;
        ((TimB5EntryFields *)rr)->field_2A4 = (f32) arg1;
        ((TimB5EntryFields *)rr)->field_2B0 = a2v;
        obj = (char *)(a2v + 1);
    }}
    child = (char *)timproc_uso_b5_func_055750((char *)0x138);
    if (child != 0) {
        timproc_uso_b5_func_0546DC(child, &timproc_uso_b5_D_807FEE74 + 0x1604, 0.0f, 0.0f, 0.0f);
        (*(s32 *)((char *)(child) + 0x28)) = (s32) (&timproc_uso_b5_D_807FEE7C + 0x444); (*(s32 *)((char *)(child) + 0xC)) = (s32) (&timproc_uso_b5_D_807FEE7C + 0x160C); gc = (char *)timproc_uso_b5_func_055750((char *)0x16C);
        if (gc != 0) {
            f32 cf;
            timproc_uso_b5_func_0546DC(gc, &timproc_uso_b5_D_807FEE84 + 0x1614, (f32) 0, (f32) 0, (f32) 0);
            g2 = gc;
            if (1) {}
            cf = *(f32 *)((char *)&import_80807FB8 + 0x3B4);
            (*(s32 *)((char *)(g2) + 0x28)) = (s32) (&import_80087FB8[0]); (*(s32 *)((char *)(g2) + 0x120)) = 1;
            *(f32 *)(g2 + 0x108) = cf;
            *(f32 *)(g2 + 0x10C) = cf;
            *(f32 *)(g2 + 0x110) = cf;
            *(f32 *)(g2 + 0x124) = 1.0f;
        }
        (*(s32 *)((char *)(child) + 0x108)) = (s32) gc;
        timproc_uso_b5_func_04DFFC(child, gc);
        if (1) {}
        p = (char *)(*(s32 *)((char *)(child) + 0x108));
        p += 0xB4;
        ((Vec3 *)p)->z = (f32)0;
        ((Vec3 *)p)->y = (f32)0;
        ((Vec3 *)p)->x = (f32)0;
        savedbit = (*(s32 *)((char *)&d550_ee6c_l1 + 4)) & 0x80000; p = (char *)(((*(s32 *)((char *)&d550_ee6c_l2 + 4)) & 0xFFF7FFFF) | 0x2000); (*(s32 *)((char *)&d550_ee6c_s1 + 4)) = (s32) p; timproc_uso_b5_func_04DFFC(child, (char *)import_000A12EC(obj));
        if (savedbit != 0) {
            p = (char *)((*(s32 *)((char *)&d550_ee6c_l3 + 4)) | 0x80000);
            (*(s32 *)((char *)&d550_ee6c_s2 + 4)) = (s32) p;
        } else {
            p = (char *)((*(s32 *)((char *)&d550_ee6c_l4 + 4)) & 0xFFF7FFFF);
            (*(s32 *)((char *)&d550_ee6c_s3 + 4)) = (s32) p;
        }
        *(f32 *)(child + 0xDC) = arg3;
        *(f32 *)(child + 0xE0) = arg4;
        *(f32 *)(child + 0xE4) = arg5;
        *(f32 *)(child + 0x10C) = (f32) 0;
        *(f32 *)(child + 0x110) = arg6;
        *(f32 *)(child + 0x114) = arg7;
        *(f32 *)(child + 0x118) = arg8;
        (*(s32 *)((char *)(child) + 0x12C)) = 0;
        (*(s32 *)((char *)(child) + 0x130)) = 0;
        *(f32 *)(child + 0x120) = 1.0f;
        *(f32 *)(child + 0x124) = (f32) 0;
    }
    {
        g2 = r;
        do {
        ((TimB5EntryFields *)g2)->field_2B8 = child;
        ((TimB5EntryFields *)g2)->field_134 = *(f32 *)((char *)&import_80807FB8 + 0x3B8);
        ((TimB5EntryFields *)g2)->field_29C = child;
        } while (0);
    }
    p = (char *)(*(s32 *)((char *)(arg0) + 0x38));
    {
        register char *list = p + 0x10;
        if (list) {}
        obj = p;
        timproc_uso_b5_func_07ACE0(list, child);
    }
    if (((TimB5ObjectPrefix *)child)->field_14 != 0) {
        ((TimB5ObjectPrefix *)child)->field_04 = 1;
    }
    ((TimB5ObjectPrefix *)child)->field_14 = obj;
    child = (char *)(*(s32 *)((char *)(arg0) + 0x6C));
    (*(s32 *)((char *)(arg0) + 0x6C)) = (s32) child + 1;
    (*(s32 *)((char *)(arg0 + (s32) child * 4) + 0x3C)) = (s32) r;
    return r;
}
