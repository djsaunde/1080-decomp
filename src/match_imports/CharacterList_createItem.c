/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
#include "gdl/light.h" /* LightTarget (the model's light-target child) */
#define ITEM(p) ((SelCharacterItem *)(p))
#define MODEL(p) ((SelCharModel *)(p))
SelCharacterItem *CharacterList_createItem(SelItemList *a0, int arg1, int arg2, u32 arg3,
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
    if (0) { gen_imp_MNode_addChild(&arg1, &arg2); }
    child.ptr = (char *)timproc_uso_b5_d14c_alloc(0x2C4);
    if (child.ptr != 0) {
        obj = child.ptr;
        if (obj != 0 ||
            (obj = (char *)timproc_uso_b5_d14c_alloc(0x2B8)) != 0) {
            gen_imp_Node_ctor(obj, &timproc_uso_b5_D_807FEE30 + 0x15C0);
            ((TimB5UiObject *)obj)->field_28 = (void *)(&timproc_uso_b5_D_807FDB64 + 0);
            SelItemParms_ctor(obj + 0x2C);
            SelItemInfo_ctor(obj + 0x194);
        }
        { char *descriptor = &timproc_uso_b5_D_807FEE38 + 0x15C8; ((TimB5UiObject *)child.ptr)->field_28 = (void *)(&timproc_uso_b5_D_807FEE38 + 0x5E4); ((TimB5UiObject *)child.ptr)->field_0C = descriptor; }
        SelItem_reset_00003890(child.ptr);
    }
    r = child.ptr;
    gen_imp_list_append((char *)a0 + 0x10, child.ptr);
    if (((TimB5UiObject *)child.ptr)->field_14 != 0) {
        ((TimB5UiObject *)child.ptr)->field_04 = 1;
    }
    {
        char *rr = r;
        ((TimB5UiObject *)child.ptr)->field_14 = (TimB5UiObject *)a0;
        ITEM(rr)->base.enabled = (float)arg1;
        ITEM(rr)->base.id = arg2 - 1;
    }
    child.ptr = (char *)timproc_uso_b5_d14c_alloc(0x148);
    if (child.ptr != 0) {
        gen_imp_QNode_ctor(child.ptr, &timproc_uso_b5_D_807FEE48 + 0x15D8,
                                   0.0f, 0.0f, 0.0f);
        ((TimB5UiObject *)child.ptr)->field_28 = (void *)(&timproc_uso_b5_D_807FDDEC + 0x57C);
        gc = (char *)timproc_uso_b5_d14c_alloc(0x16C);
        if (gc != 0) {
            float cf;
            gen_imp_QNode_ctor(gc, &timproc_uso_b5_D_807FEE54 + 0x15E4,
                                       0.0f, 0.0f, 0.0f);
            cf = *(float *)((char *)&import_80807FB8 + 0x3A0); /* RAW: data-table: an import-base data read (&import_80807FB8 + K, a RoData float) */
            ((TimB5UiObject *)gc)->field_28 = (void *)(&import_80087FB8[0]); ((LightTarget *)gc)->mask = 0xFFFF; /* gc is a LightTarget (vptr: bootup LightTarget_vtbl) */
            ((LightTarget *)gc)->unk108[0] = cf;
            ((LightTarget *)gc)->unk108[1] = cf;
            ((LightTarget *)gc)->unk108[2] = cf;
            ((LightTarget *)gc)->scale = 1.0f;
        }
        MODEL(child.ptr)->head.inner = gc;
        gen_imp_MNode_addChild(child.ptr, gc);
        if (1) {}
        p = MODEL(child.ptr)->head.inner;
        p += 0xB4;
        do {
            ((struct TimB5V3 *)p)->x = 0.0f;
            ((struct TimB5V3 *)p)->z = 0.0f;
            ((struct TimB5V3 *)p)->y = 100.0f;
        } while (0);
        MODEL(child.ptr)->highlight = *(float *)((char *)&import_80800078 + 0x3A4); /* RAW: data-table (as above) */
        n = *(int *)&import_80020078;
        if (1) {}
        p = (char *)timproc_uso_b5_d14c_alloc(0xB4);
        if (p != 0) {
            import_0010D33C(p, n);
            ((TimB5UiObject *)p)->field_28 = (void *)(&timproc_uso_b5_D_807FDAB8 + 0x248);
            ((SelCharAnim *)p)->keyHit = 0;
            gen_imp_AnimPlayer_reset(p);
        }
        MODEL(child.ptr)->anim = (SelCharAnim *)p;
        gen_imp_MNode_addChild(child.ptr, p);
        savedbit = d14c_ee6c_l1.flags & 0x80000; if (1) {} { int x = (d14c_ee6c_l2.flags & 0xFFF7FFFF) | 0x20000 | 0x2000 | 2 | 1; d14c_ee6c_s1.flags = x; } d14c_ee64_st = d14c_ee64_ld & ~8; p = (char *)MODEL(child.ptr)->anim; gen_imp_SnowBoarder_loadBoardModel_0000B1B4(arg2, 0, 0x201, p, child.ptr);
        MODEL(child.ptr)->animSet = ((s32 *)&timproc_uso_b5_D_807FDB4C)[arg2];
        gen_imp_AnimPlayer_start((char *)MODEL(child.ptr)->anim, MODEL(child.ptr)->animSet, 0, 2, 2, 1);
        if (1) {}
        {
            char *p2 = (char *)MODEL(child.ptr)->anim;
            struct TimB5UiMethod24 *vt;
            char *adjusted;
            p = ((Node *)p2)->vptr; /* the AnimPlayer's vtable */
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
        MODEL(child.ptr)->view[0] = arg6;
        MODEL(child.ptr)->view[1] = arg7;
        MODEL(child.ptr)->view[2] = arg8;
        MODEL(child.ptr)->alpha = 0.0f;
        MODEL(child.ptr)->unk124 = d14c_fb8_a[0x3A8 / sizeof(f32)];
        MODEL(child.ptr)->unk128 = d14c_fb8_b[0x3AC / sizeof(f32)];
        MODEL(child.ptr)->animTimer = 1;
        MODEL(child.ptr)->focused = 0;
        MODEL(child.ptr)->visible = 0;
    }
    {
        char *rr = r;
        ITEM(rr)->model = MODEL(child.ptr);
        ITEM(rr)->base.parms.cfv.value = d14c_fb8_c[0x3B0 / sizeof(f32)];
        ITEM(rr)->base.node = child.ptr;
    }
    obj = a0->dispItems;
    gen_imp_list_append(obj + 0x10, child.ptr);
    if (((TimB5UiObject *)child.ptr)->field_14 != 0) {
        ((TimB5UiObject *)child.ptr)->field_04 = 1;
    }
    ((TimB5UiObject *)child.ptr)->field_14 = (TimB5UiObject *)obj;
    child.index = a0->count;
    a0->count = child.index + 1;
    a0->items[child.index] = (SelItem *)r;
    return ITEM(r);
}
