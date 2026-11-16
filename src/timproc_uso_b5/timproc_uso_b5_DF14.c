/* LevelList_createItem (0x698 / 422 words) -- D884's sibling panel
 * constructor (the 0x2AC slot index, a 14-case switch on arg2 + 10 wiring the
 * 05D0E0 children, the saved 0x80000 bit restore, the arg3..arg8 placement).
 *
 * EXACT under IDO 7.1 cfe -O2 (agent-c run 217). Spliced over the unit's empty
 * placeholder by REPLACE_FUNC_BODY (Makefile TIMB5_DF14_DONOR) so the switch's
 * donor-local .rodata jumptable reloc is renamed
 * LevelList_createItem_rodata and pinned to the table the USO ships at
 * RoData +0x400 (the target reads it through D_807FF420 + 0x400), right after
 * D884's table at +0x3C0.
 *
 * Run 217 fixes over the NM body: (1) the D884 flag-chain recipe (per-pair EE6C
 * aliases: each store and the reload after it share an alias, each alias is used
 * again after the switch calls so uopt leaves its address uncoloured; u32 stores,
 * s32 loads); (2) the 0x140 / 0x13C stores on one line; (3) `if (1) {}` right
 * after the head block's `rr = r` (the r reload is issued before the arg2 reload,
 * target order); (4) the tail block writes through gsub instead of a block-local
 * rr: the r reload then colours v1 and FW(arg0, 0x38) v0, as the target. */
#include "common.h"
#include "game/select.h"
#include "gdl/node.h"
#include "gdl/light.h" /* LightTarget (the model's inner node) */
#include "game/input.h"
#define LIST ((SelItemList *)arg0)
#define ITEM(p) ((SelLevelItem *)(p))
#define MODEL(p) ((SelLevelModel *)(p))

#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern int gen_imp_mem_new();
extern int gen_imp_Node_ctor();
extern int gen_imp_list_append();
extern char D_00000000;
char *SelItemParms_ctor(char *arg0);
char *SelItemInfo_ctor(char *arg0);
extern int SelItem_reset_00003890();
/* Prototyped 5-arg decl: real signature passes f32 singles (mfc1 a2/a3 +
 * swc1 16(sp) 5th arg in the jal delay). */
extern int gen_imp_QNode_ctor(char *, char *, f32, f32, f32);
extern int gen_imp_PolyShape_ctor();
extern int gen_imp_MNode_addChild();
extern char timproc_uso_b5_D_807FDB64;
extern char timproc_uso_b5_D_807FEBB4;
extern char timproc_uso_b5_D_807FA914;
extern char import_800200CC;
extern char import_80807FB8;
extern char import_8077EBB4;
extern char import_8077A914;
extern char import_80806674;
extern char import_80776674;
/* Data symbols subscripted as byte arrays so &SYM[N] forms HI16/LO16 against the named global. */
extern char timproc_uso_b5_D_807FEF48[];
extern char timproc_uso_b5_D_807FEF50[];
extern char timproc_uso_b5_D_807FEF5C[];
extern char timproc_uso_b5_D_807FEF64[];
extern char timproc_uso_b5_D_807FEF70[];
extern char timproc_uso_b5_D_807FEFA0[];
extern char timproc_uso_b5_D_807FEFAC[];
extern char timproc_uso_b5_D_807FEFB8[];
extern char timproc_uso_b5_D_807FEFC4[];
extern char timproc_uso_b5_D_807FEFD0[];
extern char timproc_uso_b5_D_807FEFDC[];
extern char timproc_uso_b5_D_807FEFE8[];
extern char timproc_uso_b5_D_807FEFF4[];
extern char import_80087FB8[];
/* gPolyGeomMode_3EEC8_8005EE6C aliases (all 0): l1/l2 single reads, s1/s2/s4 store+reload pairs, s7 else-arm store */
extern char df14_ee6c_l1, df14_ee6c_l2;
extern char df14_ee6c_s1, df14_ee6c_s2, df14_ee6c_s4, df14_ee6c_s7;
#define EE6C_L(s) (*(s32 *)((char *)&s + 4))
#define EE6C_S(s) (*(u32 *)((char *)&s + 4))

char *LevelList_createItem(char *arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    char *volatile r;
    volatile int pad0;
    char *sub;
    char *child;
    volatile int pad[7];
    s32 flag;
    f32 *p;
    char *gsub;
    s32 mode;
    f32 t;

    child = (char *) gen_imp_mem_new((char *)0x2BC);
    if (child != 0) {
        sub = child;
        if ((sub != 0) || ((sub = (char *) gen_imp_mem_new((char *)0x2B8)) != 0)) {
            gen_imp_Node_ctor(sub, &timproc_uso_b5_D_807FEF48[5848]);
            NODE(sub)->vptr = (s32) (&timproc_uso_b5_D_807FDB64 + 0);
            SelItemParms_ctor(&((SelItem *)sub)->parms);
            SelItemInfo_ctor(&((SelItem *)sub)->info);
        }
        NODE(child)->vptr = (s32) &timproc_uso_b5_D_807FEF50[2468]; NODE(child)->name = (s32) &timproc_uso_b5_D_807FEF50[5856];
        SelItem_reset_00003890(child);
    }
    r = child;
    ITEM(child)->base.listIndex = LIST->count;
    gen_imp_list_append(&NODE(arg0)->children, child);
    if (NODE(child)->parent != 0) {
        NODE(child)->unk04 = 1;
    }
    {
        char *rr = r;
        if (1) {} /* run 217: the r reload issues before the arg2 reload (target) */
        NODE(child)->parent = (Node *)arg0;
        ITEM(rr)->base.enabled = (f32) arg1; /* run 216: cvt store before the id store */
        ITEM(rr)->base.id = arg2;
        sub = (char *)(arg2 + 10);
    }
    child = (char *) gen_imp_mem_new((char *)0x144);
    if (child != 0) {
        gen_imp_QNode_ctor(child, &timproc_uso_b5_D_807FEF5C[5868], 0.0f, 0.0f, 0.0f);
        NODE(child)->vptr = (s32) &timproc_uso_b5_D_807FEF64[1300]; NODE(child)->name = (s32) &timproc_uso_b5_D_807FEF64[5876];
        gsub = (char *) gen_imp_mem_new((char *)0x16C);
        if (gsub != 0) {
            gen_imp_QNode_ctor(gsub, &timproc_uso_b5_D_807FEF70[5888], 0.0f, 0.0f, 0.0f);
            t = *(f32 *)(&import_80807FB8 + 1020);
            NODE(gsub)->vptr = (s32) &import_80087FB8[0]; ((LightTarget *)gsub)->mask = 1; /* run 216: one line = D884's store order (0x120 first) */ /* vptr: bootup LightTarget_vtbl (gl_data.h) */
            ((LightTarget *)gsub)->unk108[0] = t;
            ((LightTarget *)gsub)->unk108[1] = t;
            ((LightTarget *)gsub)->unk108[2] = t;
            ((LightTarget *)gsub)->scale = 1.0f;
        }
        MODEL(child)->head.inner = gsub;
        gen_imp_MNode_addChild(child, gsub);
        if (1) {}
        p = (f32 *)&((QNode *)MODEL(child)->head.inner)->vqs;
        p[2] = 0; /* int 0: one mtc1 zero for the three stores (0.0f gives three) */
        p[1] = 0;
        p[0] = 0;
        MODEL(child)->highlight = 1.0f;
        /* flag chain on one line (run 216); per-pair aliases (run 217, see top) */
        flag = EE6C_L(df14_ee6c_l1) & 0x80000; EE6C_S(df14_ee6c_s1) = EE6C_L(df14_ee6c_l2) & 0xFFF7FFFF; EE6C_S(df14_ee6c_s2) = EE6C_L(df14_ee6c_s1) & ~1; EE6C_S(df14_ee6c_s2) = EE6C_L(df14_ee6c_s2) | 2; EE6C_S(df14_ee6c_s4) = EE6C_L(df14_ee6c_s2) & 0xFFFDFFFF; EE6C_S(df14_ee6c_s4) = EE6C_L(df14_ee6c_s4) | 0x2000;
        MODEL(child)->unk140 = 1; MODEL(child)->unk13C = 1;
        switch ((s32) sub) {
        case 0:
            if (INPUT_MGR(import_800200CC).nextFlowMode == 3) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, (&timproc_uso_b5_D_807FEBB4 + 5900), (&import_8077EBB4 + 0), 0));
            }
            MODEL(child)->unk13C = 0;
            break;
        case 1:
            if (INPUT_MGR(import_800200CC).nextFlowMode == 3) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, (&timproc_uso_b5_D_807FA914 + 5912), (&import_8077A914 + 0), 0));
            }
            MODEL(child)->unk13C = 0;
            break;
        case 2:
            if (INPUT_MGR(import_800200CC).nextFlowMode == 3) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, (&import_80806674 + 5924), (&import_80776674 + 0), 0));
            }
            MODEL(child)->unk13C = 0;
            break;
        case 3:
            if (INPUT_MGR(import_800200CC).nextFlowMode != 2) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFA0[5936], (&D_00000000 + 0), 0));
            }
            break;
        case 4:
            if (INPUT_MGR(import_800200CC).nextFlowMode != 2) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFAC[5948], (&D_00000000 + 0), 0));
            }
            break;
        case 5:
            mode = INPUT_MGR(import_800200CC).nextFlowMode;
            if ((mode != 3) && (mode != 5)) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFB8[5960], (&D_00000000 + 0), 0));
            }
            break;
        case 6:
            mode = INPUT_MGR(import_800200CC).nextFlowMode;
            if ((mode != 3) && (mode != 5)) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFC4[5972], (&D_00000000 + 0), 0));
            }
            break;
        case 7:
            mode = INPUT_MGR(import_800200CC).nextFlowMode;
            if ((mode != 3) && (mode != 5)) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFD0[5984], (&D_00000000 + 0), 0));
            }
            break;
        case 8:
            mode = INPUT_MGR(import_800200CC).nextFlowMode;
            if ((mode != 3) && (mode != 5)) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFDC[5996], (&D_00000000 + 0), 0));
            }
            break;
        case 9:
            mode = INPUT_MGR(import_800200CC).nextFlowMode;
            if ((mode != 3) && (mode != 5)) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFE8[6008], (&D_00000000 + 0), 0));
            }
            break;
        case 10:
            mode = INPUT_MGR(import_800200CC).nextFlowMode;
            if ((mode != 3) && (mode != 5)) {
                gen_imp_MNode_addChild(child, gen_imp_PolyShape_ctor(0, &timproc_uso_b5_D_807FEFF4[6020], (&D_00000000 + 0), 0));
            }
            break;
        case 11:
        case 12:
        case 13:
            break;
        }
        /* one line (D884's lui order fix, run 216) */
        if (flag) EE6C_S(df14_ee6c_s2) = EE6C_L(df14_ee6c_s1) | 0x80000; else EE6C_S(df14_ee6c_s7) = EE6C_L(df14_ee6c_s4) & 0xFFF7FFFF;
        MODEL(child)->head.qnode.cvqs.trans[0] = arg3;
        MODEL(child)->head.qnode.cvqs.trans[1] = arg4;
        MODEL(child)->head.qnode.cvqs.trans[2] = arg5;
        MODEL(child)->view[0] = arg6;
        MODEL(child)->view[1] = arg7;
        MODEL(child)->view[2] = arg8; /* arg8 right after arg7, 0x120 before 0x124: as1 then issues the stack-arg loads like the target (run 161) */
        MODEL(child)->unk134 = 0;
        MODEL(child)->focused = 0;
        MODEL(child)->unk128 = 0.0f;
        MODEL(child)->alpha = 0.0f;
        MODEL(child)->unk120 = *(f32 *)&import_80087FB8[1080];
        MODEL(child)->highlight = 0.0f;
    }
    {
        gsub = r; /* run 217: through gsub (a block-local rr took v0 from FW(arg0, 0x38)) */
        ITEM(gsub)->model = MODEL(child);
        ITEM(gsub)->base.node = child;
        ITEM(gsub)->base.parms.cfv.value = -32.0f; if (1) {}
    }
    sub = LIST->dispItems;
    gen_imp_list_append(&NODE(sub)->children, child);
    if (NODE(child)->parent != 0) {
        NODE(child)->unk04 = 1;
    }
    NODE(child)->parent = (Node *)sub;
    child = (char *)LIST->count;
    LIST->count = (s32) child + 1;
    LIST->items[(s32) child] = (SelItem *)r;
    return r;
}
