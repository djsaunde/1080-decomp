/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
#include "game/timproc_uso_b5_data.h"
#include "gdl/light.h" /* LightTarget (the model's light-target child) */
#define LIST(p) ((SelItemList *)(p))
#define MODEL(p) ((SelBoardModel *)(p))
/* the d550 externs are aliases of bootup gPolyGeomMode (one import, separate %hi / %lo pairs) */
#define GEOMMODE(sym) (((SelPolyGeomMode *)&(sym))->geomMode)
#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_bss[];
#define d550_ee6c_l1 (*(__typeof__(d550_ee6c_l1) *)(port_bootup_bss + 0x3EEC8))
#define d550_ee6c_l2 (*(__typeof__(d550_ee6c_l2) *)(port_bootup_bss + 0x3EEC8))
#define d550_ee6c_l3 (*(__typeof__(d550_ee6c_l3) *)(port_bootup_bss + 0x3EEC8))
#define d550_ee6c_l4 (*(__typeof__(d550_ee6c_l4) *)(port_bootup_bss + 0x3EEC8))
#define d550_ee6c_s1 (*(__typeof__(d550_ee6c_s1) *)(port_bootup_bss + 0x3EEC8))
#define d550_ee6c_s2 (*(__typeof__(d550_ee6c_s2) *)(port_bootup_bss + 0x3EEC8))
#define d550_ee6c_s3 (*(__typeof__(d550_ee6c_s3) *)(port_bootup_bss + 0x3EEC8))
#endif
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
                NODE(o2)->vptr = &timproc_uso_b5_D_807FDB64 + 0;
                SelItemParms_ctor(o2 + 0x2C);
            }
            SelItemInfo_ctor(obj + 0x194);
        }
        NODE(child)->vptr = &timproc_uso_b5_D_807FEE68 + 0x6D4; NODE(child)->name = &timproc_uso_b5_D_807FEE68 + 0x15F8; SelItem_reset_00003890(child); /* BoardItem; one line: IDO's schedule follows the line numbers */
        ((SelBoardItem *)child)->blink = 0;
        SELITEM(child)->flags = 0;
    }
    r = child;
    gen_imp_list_append(arg0 + 0x10, child);
    if (NODE(child)->parent != 0) {
        NODE(child)->unk04 = 1;
    }
    {
        char *rr = r;
        {
        s32 a2v = arg2;
        NODE(child)->parent = (Node *)arg0;
        ((TimB5EntryFields *)rr)->enabled = (f32) arg1;
        ((TimB5EntryFields *)rr)->id = a2v;
        obj = (char *)(a2v + 1);
    }}
    child = (char *)gen_imp_mem_new((char *)0x138);
    if (child != 0) {
        gen_imp_QNode_ctor(child, &timproc_uso_b5_D_807FEE74 + 0x1604, 0.0f, 0.0f, 0.0f);
        NODE(child)->vptr = &timproc_uso_b5_D_807FEE7C + 0x444; NODE(child)->name = &timproc_uso_b5_D_807FEE7C + 0x160C; gc = (char *)gen_imp_mem_new((char *)0x16C); /* SelBoard */
        if (gc != 0) {
            f32 cf;
            gen_imp_QNode_ctor(gc, &timproc_uso_b5_D_807FEE84 + 0x1614, (f32) 0, (f32) 0, (f32) 0);
            g2 = gc;
            if (1) {}
            cf = *(f32 *)((char *)&import_80807FB8 + kRoF_gen_1e7_3B4); /* genproc RoData */
            NODE(g2)->vptr = &import_80087FB8[0]; ((LightTarget *)g2)->mask = 1; /* vptr: bootup LightTarget_vtbl (gl_data.h) */
            ((LightTarget *)g2)->unk108[0] = cf;
            ((LightTarget *)g2)->unk108[1] = cf;
            ((LightTarget *)g2)->unk108[2] = cf;
            ((LightTarget *)g2)->scale = 1.0f;
        }
        MODEL(child)->head.inner = gc;
        gen_imp_MNode_addChild(child, gc);
        if (1) {}
        p = MODEL(child)->head.inner;
        p += 0xB4;
        ((Vec3 *)p)->z = (f32)0;
        ((Vec3 *)p)->y = (f32)0;
        ((Vec3 *)p)->x = (f32)0;
        savedbit = GEOMMODE(d550_ee6c_l1) & 0x80000; p = (char *)((GEOMMODE(d550_ee6c_l2) & 0xFFF7FFFF) | 0x2000); GEOMMODE(d550_ee6c_s1) = (s32) p; gen_imp_MNode_addChild(child, (char *)import_000A12EC(obj));
        if (savedbit != 0) {
            p = (char *)(GEOMMODE(d550_ee6c_l3) | 0x80000);
            GEOMMODE(d550_ee6c_s2) = (s32) p;
        } else {
            p = (char *)(GEOMMODE(d550_ee6c_l4) & 0xFFF7FFFF);
            GEOMMODE(d550_ee6c_s3) = (s32) p;
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
        ((TimB5EntryFields *)g2)->cfv = *(f32 *)((char *)&import_80807FB8 + kRoF_gen_m28p48_3B8);
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
    if (NODE(child)->parent != 0) {
        NODE(child)->unk04 = 1;
    }
    NODE(child)->parent = (Node *)obj;
    child = (char *)LIST(arg0)->count;
    LIST(arg0)->count = (s32) child + 1;
    LIST(arg0)->items[(s32) child] = (SelItem *) r;
    return r;
}
#ifdef PORT
#undef d550_ee6c_l1
#undef d550_ee6c_l2
#undef d550_ee6c_l3
#undef d550_ee6c_l4
#undef d550_ee6c_s1
#undef d550_ee6c_s2
#undef d550_ee6c_s3
#endif
