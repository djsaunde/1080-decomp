/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
#define LIST(p) ((SelItemList *)(p))
#define MODEL(p) ((SelBoardModel *)(p))
char *BoardList_createItem(char *arg0, volatile s32 arg1, volatile s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    char *volatile r;
    char *g2;
    char *volatile obj;
    char *child;
    volatile int pad[7];
    s32 savedbit;
    char *p;
    char *gc;
    if (0) { gen_imp_MNode_addChild(&gc); }
    child = (char *)gen_imp_mem_new((char *)0x2C8);
    if (child != 0) {
        obj = child;
        if (child != 0 || (p = (char *)gen_imp_mem_new((char *)0x2B8), obj = p, p != 0)) {
            gen_imp_Node_ctor(obj, &timproc_uso_b5_D_807FEE60 + 0x15F0);
            {
                char *o2 = obj;
                (*(s32 *)((char *)(o2) + 0x28)) = (s32) (&timproc_uso_b5_D_807FDB64 + 0);
                SelItemParms_ctor(o2 + 0x2C);
            }
            SelItemInfo_ctor(obj + 0x194);
        }
        (*(s32 *)((char *)(child) + 0x28)) = (s32) (&timproc_uso_b5_D_807FEE68 + 0x6D4); (*(s32 *)((char *)(child) + 0xC)) = (s32) (&timproc_uso_b5_D_807FEE68 + 0x15F8); SelItem_reset_00003890(child);
        (*(s32 *)((char *)(child) + 0x2C4)) = 0;
        SELITEM(child)->flags = 0;
    }
    r = child;
    gen_imp_list_append(arg0 + 0x10, child);
    if (((TimB5ObjectPrefix *)child)->field_14 != 0) {
        ((TimB5ObjectPrefix *)child)->field_04 = 1;
    }
    {
        char *rr = r;
        {
        s32 a2v = arg2;
        ((TimB5ObjectPrefix *)child)->field_14 = arg0;
        ((TimB5EntryFields *)rr)->enabled = (f32) arg1;
        ((TimB5EntryFields *)rr)->id = a2v;
        obj = (char *)(a2v + 1);
    }}
    child = (char *)gen_imp_mem_new((char *)0x138);
    if (child != 0) {
        gen_imp_QNode_ctor(child, &timproc_uso_b5_D_807FEE74 + 0x1604, 0.0f, 0.0f, 0.0f);
        (*(s32 *)((char *)(child) + 0x28)) = (s32) (&timproc_uso_b5_D_807FEE7C + 0x444); (*(s32 *)((char *)(child) + 0xC)) = (s32) (&timproc_uso_b5_D_807FEE7C + 0x160C); gc = (char *)gen_imp_mem_new((char *)0x16C);
        if (gc != 0) {
            f32 cf;
            gen_imp_QNode_ctor(gc, &timproc_uso_b5_D_807FEE84 + 0x1614, (f32) 0, (f32) 0, (f32) 0);
            g2 = gc;
            if (1) {}
            cf = *(f32 *)((char *)&import_80807FB8 + 0x3B4);
            (*(s32 *)((char *)(g2) + 0x28)) = (s32) (&import_80087FB8[0]); (*(s32 *)((char *)(g2) + 0x120)) = 1;
            *(f32 *)(g2 + 0x108) = cf;
            *(f32 *)(g2 + 0x10C) = cf;
            *(f32 *)(g2 + 0x110) = cf;
            *(f32 *)(g2 + 0x124) = 1.0f;
        }
        MODEL(child)->head.inner = gc;
        gen_imp_MNode_addChild(child, gc);
        if (1) {}
        p = MODEL(child)->head.inner;
        p += 0xB4;
        ((Vec3 *)p)->z = (f32)0;
        ((Vec3 *)p)->y = (f32)0;
        ((Vec3 *)p)->x = (f32)0;
        savedbit = (*(s32 *)((char *)&d550_ee6c_l1 + 4)) & 0x80000; p = (char *)(((*(s32 *)((char *)&d550_ee6c_l2 + 4)) & 0xFFF7FFFF) | 0x2000); (*(s32 *)((char *)&d550_ee6c_s1 + 4)) = (s32) p; gen_imp_MNode_addChild(child, (char *)import_000A12EC(obj));
        if (savedbit != 0) {
            p = (char *)((*(s32 *)((char *)&d550_ee6c_l3 + 4)) | 0x80000);
            (*(s32 *)((char *)&d550_ee6c_s2 + 4)) = (s32) p;
        } else {
            p = (char *)((*(s32 *)((char *)&d550_ee6c_l4 + 4)) & 0xFFF7FFFF);
            (*(s32 *)((char *)&d550_ee6c_s3 + 4)) = (s32) p;
        }
        MODEL(child)->head.qnode.cvqs.trans[0] = arg3;
        MODEL(child)->head.qnode.cvqs.trans[1] = arg4;
        MODEL(child)->head.qnode.cvqs.trans[2] = arg5;
        MODEL(child)->spin = (f32) 0;
        MODEL(child)->view[0] = arg6;
        MODEL(child)->view[1] = arg7;
        MODEL(child)->view[2] = arg8;
        MODEL(child)->focused = 0;
        MODEL(child)->lit = 0;
        MODEL(child)->alpha = 1.0f;
        MODEL(child)->highlight = (f32) 0;
    }
    {
        g2 = r;
        do {
        ((TimB5EntryFields *)g2)->model = child;
        ((TimB5EntryFields *)g2)->cfv = *(f32 *)((char *)&import_80807FB8 + 0x3B8);
        ((TimB5EntryFields *)g2)->node = child;
        } while (0);
    }
    p = LIST(arg0)->dispItems;
    {
        register char *list = p + 0x10;
        if (list) {}
        obj = p;
        gen_imp_list_append(list, child);
    }
    if (((TimB5ObjectPrefix *)child)->field_14 != 0) {
        ((TimB5ObjectPrefix *)child)->field_04 = 1;
    }
    ((TimB5ObjectPrefix *)child)->field_14 = obj;
    child = (char *)LIST(arg0)->count;
    LIST(arg0)->count = (s32) child + 1;
    LIST(arg0)->items[(s32) child] = (SelItem *) r;
    return r;
}
