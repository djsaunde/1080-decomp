/* CourseList_createItem (0x690 / 420 words) -- timing-screen panel
 * constructor: panel + widget allocs, the gc child with the RoData 0x3BC scale,
 * the EE6C flag-word chain, a 14-case mode switch wiring the 05D0E0 children,
 * the saved 0x80000 bit restore, then the arg3..arg8 placement.
 *
 * EXACT under IDO 7.1 cfe -O2 (agent-c run 217). Spliced over the unit's empty
 * placeholder by REPLACE_FUNC_BODY (Makefile TIMB5_D884_DONOR) so the switch's
 * donor-local .rodata jumptable reloc is renamed
 * CourseList_createItem_rodata and pinned to the table the USO ships at
 * RoData +0x3C0 (the target reads it through D_807FF3E0 + 0x3C0).
 *
 * The flag chain (run 217): the target re-reads the flag word after every
 * store with the destructive `lui tN; lw tN,4(tN)` form and shares one `lui at`
 * between two stores. That is as1 output for ONE symbol whose address uopt never
 * held in a register. Per-pair aliases give it: each store and the load that
 * follows it use the same alias (as1 then keeps the load behind the store and
 * reuses $at for a second store to the same alias), and each alias is also used
 * after the switch calls, so its address web crosses calls and uopt leaves it
 * uncolored (direct symbol form everywhere). Stores are u32 and loads s32:
 * with matching types uopt forwards the stored value into the next load. */
#include "common.h"
#include "game/select.h"
#include "gdl/node.h"
#include "gdl/light.h" /* LightTarget (the model's inner node) */
#include "uso_call.h"
#define LIST ((SelItemList *)arg0)
#define ITEM(p) ((SelCourseItem *)(p))
#define MODEL(p) ((SelCourseModel *)(p))

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern int gen_imp_mem_new();
extern int gen_imp_Node_ctor();
extern int gen_imp_list_append();
char *SelItemParms_ctor(char *arg0);
char *SelItemInfo_ctor(char *arg0);
extern int SelItem_reset_00003890();
extern int gen_imp_QNode_ctor(char *, char *, f32, f32, f32);
extern int gen_imp_PolyShape_ctor();
extern int gen_imp_MNode_addChild();
extern char timproc_uso_b5_D_807FEE90;
extern char timproc_uso_b5_D_807FDB64;
extern char timproc_uso_b5_D_807FEE98;
extern char timproc_uso_b5_D_807FEEA4;
extern char timproc_uso_b5_D_807FEEAC;
extern char timproc_uso_b5_D_807FEEB8;
extern char timproc_uso_b5_D_807FEBB4;
extern char timproc_uso_b5_D_807FA914;
extern char timproc_uso_b5_D_807FEEE8;
extern char timproc_uso_b5_D_807FEEF4;
extern char timproc_uso_b5_D_807FEF00;
extern char timproc_uso_b5_D_807FEF0C;
extern char timproc_uso_b5_D_807FEF18;
extern char timproc_uso_b5_D_807FEF24;
extern char timproc_uso_b5_D_807FEF30;
extern char timproc_uso_b5_D_807FEF3C;
extern char import_800200CC;
extern char import_80807FB8;
extern char import_8077EBB4;
extern char import_8077A914;
extern char import_80806674;
extern char import_80776674;
extern char import_00000000;
/* gPolyGeomMode_3EEC8_8005EE6C aliases (all 0): l1/l2 single reads, s1/s2/s4 store+reload pairs, s7 else-arm store */
extern char d884_ee6c_l1, d884_ee6c_l2;
extern char d884_ee6c_s1, d884_ee6c_s2, d884_ee6c_s4, d884_ee6c_s7;
extern char d884_db64_gc;
#define EE6C_L(s) (*(s32 *)((char *)&s + 4))
#define EE6C_S(s) (*(u32 *)((char *)&s + 4))
#define MODE (*(s32 *)((char *)&import_800200CC + 0x34))

#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_bss[];
#define d884_ee6c_l1 (*(__typeof__(d884_ee6c_l1) *)(port_bootup_bss + 0x3EEC8))
#define d884_ee6c_l2 (*(__typeof__(d884_ee6c_l2) *)(port_bootup_bss + 0x3EEC8))
#define d884_ee6c_s1 (*(__typeof__(d884_ee6c_s1) *)(port_bootup_bss + 0x3EEC8))
#define d884_ee6c_s2 (*(__typeof__(d884_ee6c_s2) *)(port_bootup_bss + 0x3EEC8))
#define d884_ee6c_s4 (*(__typeof__(d884_ee6c_s4) *)(port_bootup_bss + 0x3EEC8))
#define d884_ee6c_s7 (*(__typeof__(d884_ee6c_s7) *)(port_bootup_bss + 0x3EEC8))
#endif
char *CourseList_createItem(char *arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    char *volatile r;
    volatile int pad0;
    char *obj;
    char *child;
    volatile int pad[7];
    s32 savedbit;
    char *p;
    char *gc;

    if (0) { gen_imp_MNode_addChild(&arg1, &arg2); }
    child = gen_imp_mem_new((char *)0x2BC);
    if (child != 0) {
        obj = child;
        if (obj != 0 || (obj = gen_imp_mem_new((char *)0x2B8)) != 0) {
            gen_imp_Node_ctor(obj, &timproc_uso_b5_D_807FEE90 + 0x1620);
            NODE(obj)->vptr = (char *)&timproc_uso_b5_D_807FDB64;
            SelItemParms_ctor(&((SelItem *)obj)->parms);
            SelItemInfo_ctor(&((SelItem *)obj)->info);
        }
        NODE(child)->vptr = &timproc_uso_b5_D_807FEE98 + 0x7C4; NODE(child)->name = &timproc_uso_b5_D_807FEE98 + 0x1628;
        SelItem_reset_00003890(child);
    }
    r = child;
    gen_imp_list_append(&NODE(arg0)->children, child);
    if (NODE(child)->parent != 0) {
        NODE(child)->unk04 = 1;
    }
    {
        char *rr = r;
        NODE(child)->parent = (Node *)arg0;
        ITEM(rr)->base.enabled = (f32) arg1;
        ITEM(rr)->base.id = arg2;
    }
    child = gen_imp_mem_new((char *)0x144);
    if (child != 0) {
        gen_imp_QNode_ctor(child, &timproc_uso_b5_D_807FEEA4 + 0x1634, 0.0f, 0.0f, 0.0f);
        NODE(child)->vptr = &timproc_uso_b5_D_807FEEAC + 0x514; NODE(child)->name = &timproc_uso_b5_D_807FEEAC + 0x163C;
        gc = gen_imp_mem_new((char *)0x16C);
        if (gc != 0) {
            f32 one;
            char *g2;
            gen_imp_QNode_ctor(gc, &timproc_uso_b5_D_807FEEB8 + 0x1648, 0.0f, 0.0f, 0.0f);
            g2 = gc;
            one = *(f32 *)((char *)&import_80807FB8 + kRoF_gen_1e7_3BC);
            NODE(g2)->vptr = (char *)&d884_db64_gc; ((LightTarget *)g2)->mask = 1;
            ((LightTarget *)g2)->unk108[0] = one; /* the RoData 0x3BC constant, not 1.0f */
            ((LightTarget *)g2)->unk108[1] = one;
            ((LightTarget *)g2)->unk108[2] = one;
            ((LightTarget *)g2)->scale = 1.0f;
        }
        MODEL(child)->head.inner = gc;
        gen_imp_MNode_addChild(child, gc);
        if (1) {}
        p = (char *)&((QNode *)MODEL(child)->head.inner)->vqs;
        ((f32 *)p)[2] = 0; /* int 0: one mtc1 zero (0.0f gives three) */
        ((f32 *)p)[1] = 0;
        ((f32 *)p)[0] = 0;
        MODEL(child)->highlight = 1.0f;
        /* the flag chain as one source line (one .loc group, run 216); per-pair aliases (run 217, see top) */
        savedbit = EE6C_L(d884_ee6c_l1) & 0x80000; EE6C_S(d884_ee6c_s1) = EE6C_L(d884_ee6c_l2) & 0xFFF7FFFF; EE6C_S(d884_ee6c_s2) = EE6C_L(d884_ee6c_s1) & ~1; EE6C_S(d884_ee6c_s2) = EE6C_L(d884_ee6c_s2) | 2; EE6C_S(d884_ee6c_s4) = EE6C_L(d884_ee6c_s2) & 0xFFFDFFFF; EE6C_S(d884_ee6c_s4) = EE6C_L(d884_ee6c_s4) | 0x2000;
        /* both stores on one line: as1 then puts the 0x13C constant in the lw delay like the target */
        MODEL(child)->unk140 = 1; MODEL(child)->unk13C = 1;
        switch (arg2) {
        case 11:
            if (MODE == 3) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEBB4 + 0x1654, &import_8077EBB4 + 0, 0));
            }
            MODEL(child)->unk13C = 0;
            break;
        case 12:
            if (MODE == 3) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FA914 + 0x1660, &import_8077A914 + 0, 0));
            }
            MODEL(child)->unk13C = 0;
            break;
        case 13:
            if (MODE == 3) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &import_80806674 + 0x166C, &import_80776674 + 0, 0));
            }
            MODEL(child)->unk13C = 0;
            break;
        case 0:
            if (MODE != 2) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEEE8 + 0x1678, &XDATA(import_00000000, port_charmap_data, 0x1B4) + 0, 0));
            }
            break;
        case 1:
            if (MODE != 2) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEEF4 + 0x1684, &XDATA(import_00000000, port_charmap_data, 0x33C4) + 0, 0));
            }
            break;
        case 2:
            if (MODE != 3 && MODE != 5) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEF00 + 0x1690, &XDATA(import_00000000, port_charmap_data, 0x65D4) + 0, 0));
            }
            break;
        case 3:
            if (MODE != 3 && MODE != 5) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEF0C + 0x169C, &XDATA(import_00000000, port_charmap_data, 0x97E4) + 0, 0));
            }
            break;
        case 4:
            if (MODE != 3 && MODE != 5) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEF18 + 0x16A8, &XDATA(import_00000000, port_charmap_data, 0xC9F4) + 0, 0));
            }
            break;
        case 5:
            if (MODE != 3 && MODE != 5) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEF24 + 0x16B4, &XDATA(import_00000000, port_charmap_data, 0xFC04) + 0, 0));
            }
            break;
        case 6:
            if (MODE != 3 && MODE != 5) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEF30 + 0x16C0, &XDATA(import_00000000, port_charmap_data, 0x12E14) + 0, 0));
            }
            break;
        case 7:
            if (MODE != 3 && MODE != 5) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEF3C + 0x16CC, &XDATA(import_00000000, port_charmap_data, 0x16024) + 0, 0));
            }
            break;
        }
        /* one source line = one .loc group: as1 hoists the else arm's lui first (target t2 then t0) */
        if (savedbit) EE6C_S(d884_ee6c_s2) = EE6C_L(d884_ee6c_s1) | 0x80000; else EE6C_S(d884_ee6c_s7) = EE6C_L(d884_ee6c_s4) & 0xFFF7FFFF;
        {
            MODEL(child)->head.qnode.cvqs.trans[0] = arg3;
            MODEL(child)->head.qnode.cvqs.trans[1] = arg4;
            MODEL(child)->head.qnode.cvqs.trans[2] = arg5;
            MODEL(child)->view[0] = arg6;
            MODEL(child)->view[1] = arg7;
            MODEL(child)->view[2] = arg8; /* arg8 right after arg7, 0x120 before 0x124: as1 then issues the stack-arg loads like the target (run 161) */
            MODEL(child)->lit = 0;
            MODEL(child)->focused = 0;
            MODEL(child)->mapFade = 0.0f;
            MODEL(child)->alpha = 0.0f;
            MODEL(child)->unk120 = *(f32 *)((char *)&import_80807FB8 + kRoF_gen_0p28_3F8);
            MODEL(child)->highlight = 0.0f;
        }
    }
    {
        char *rr = r;
        ITEM(rr)->model = MODEL(child);
        ITEM(rr)->base.node = child;
        ITEM(rr)->base.parms.cfv.value = -32.0f;
    }
    obj = LIST->dispItems;
    gen_imp_list_append(&NODE(obj)->children, child);
    if (NODE(child)->parent != 0) {
        NODE(child)->unk04 = 1;
    }
    NODE(child)->parent = (Node *)obj;
    child = (char *)LIST->count;
    LIST->count = (s32) child + 1;
    LIST->items[(s32) child] = (SelItem *)r;
    return r;
}
#ifdef PORT
#undef d884_ee6c_l1
#undef d884_ee6c_l2
#undef d884_ee6c_s1
#undef d884_ee6c_s2
#undef d884_ee6c_s4
#undef d884_ee6c_s7
#endif
