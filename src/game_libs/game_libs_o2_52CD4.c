/* IDO 7.1 -O2 donor for BasePolyShape_parse -- command-stream decoder
 * (0x468 / 282 words; 282/282 word-exact standalone apart from reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 29CCC / 44B78 / 31334 recipe, docs/MATCHING_WORKFLOW.md#donor-splice-
 * switch-jumptable-rodata-rename). The C switch over op 0..36 puts its
 * 37-entry jumptable at donor .rodata +0 and the 10000.0f literal right
 * behind it at +0x94; the splice renames that reloc to
 * BasePolyShape_parse_rodata, pinned to 0x1BE4 in undefined_syms_auto.txt, so
 * the `lw t6,0x1BE4(at)` table load and the `lwc1 f8,0x1C78(at)` literal both
 * link exactly as the USO ships them (bootup.uso RoData file offset = imm + 4:
 * table at 0x1BE8..0x1C78, 0x461C4000 at 0x1C7C). scripts/extract-uso-
 * jumptable.py --module 0xD9FE28 --shim 0x1466C dumps the 37 entries minus
 * the last (case 36 = raw 0x6755C read by hand); arm offset = raw - 0x67340.
 *
 * The "external USO jumptable = structural cap" note on the old NM wrap was
 * the in-unit verdict; with the donor splice it is landable.
 *
 * Load-bearing shapes:
 *  - op is a plain s32 local `= *p & 0xFFFF` (lw + andi; a u16 local or a
 *    u16 param gives lhu / homes a2-a3). It lands in a3 on its own.
 *  - every stream read is the post-inc temp idiom `q = cur; cur += 1; x = *q;`
 *    (docs/IDO_CODEGEN.md#relocfree-external-jumptable-switch-3ac5c): it
 *    keeps the per-read `or v1,s0,zero`; `*cur++` folds to lw 0(s0) and
 *    merges the increments into +8 / -4 offsets.
 *  - case arms in SOURCE order = target address order (35, 6, 27, 28, 29,
 *    31, 4, 7, 8, 9, 26, 0, 11, 36, 12, 18, 19, 20, 21, 10, 25, 30, 34, 32,
 *    default, 1). Cases 2/3/5/13-17/22-24/33 share the default arm; case 1
 *    is the empty arm that lands on the next-op read.
 *  - flag word at +0x38 is reached through a +0x34 base pointer (`addiu
 *    v0,s1,52; lw 4(v0)`): in case 8 it needs an `if (1)` block around the
 *    RMW, otherwise uopt folds it to 0x38(s1). Cases 11/21 keep the base
 *    because both arms use it; their tested word is the named local w (a0).
 *  - case 29: the second word is stored straight from *q right after the
 *    read (a named v2 colors a0 instead of t3); the alloc result goes
 *    through a pointer local (s2 = arg0+0x84 computed in the jal delay).
 *  - frame: lflag/dflag/rflag are stack-homed at 52/48/40; the decl list
 *    (with the otherwise unused v2 and the register-only w) is what puts
 *    them there -- dropping v2 shifts every slot by 4.
 *  - finalize: n = count; alloc(n * 0xC) (named n gives lw v0 + sll/subu);
 *    the 0x5C != 0 arm calls with arg0 only, the else arm (arg0, lflag, 1);
 *    IDO hoists the shared a0/a1 setup above the branch.
 *  - the three message addresses 0x20FD4/0x20FDC/0x20FF4 are pinned data
 *    refs (gl_ref_000xxxxx_52CD4) so the lui 2 / addiu pairs bake. */
#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/polyshape.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int D_00000000;
extern char sStr_Mats_20FD4[], sStr_parsed_materials_broken_20FDC[], sStr_shape_scale_dont_work_mate_20FF4[];

s32 *BasePolyShape_parse(char *arg0, s32 *arg1) {
    s32 op;
    s32 *cur;
    s32 v;
    s32 v2; /* unused: holds a frame slot */
    s32 lflag;
    s32 dflag;
    s32 w;
    s32 rflag;
    char *p;
    s32 *q;
    s32 *pp;
    s32 n;

    rflag = 0;
    dflag = 0;
    lflag = 0;
    op = *arg1 & 0xFFFF;
    cur = arg1 + 1;
    if (op != 2) {
        do {
            switch (op) {
            case 35:
                q = cur; cur += 1; BASEPOLY(arg0)->collGrid = (struct CollGrid *)*q;
                break;
            case 6:
                q = cur; cur += 1; v = *q;
                CALL(BasePolyShape_parse)(arg0, v);
                break;
            case 27:
                q = cur; cur += 1; BASEPOLY(arg0)->nVerts = *q;
                q = cur; cur += 1; BASEPOLY(arg0)->verts = (s16 (*)[3])(*q);
                break;
            case 28:
                q = cur; cur += 1; BASEPOLY(arg0)->nTris = *q;
                q = cur; cur += 1; BASEPOLY(arg0)->tris = (PolyTri *)(*q);
                dflag = 1;
                break;
            case 29:
                CALL(memtrack_begin)(sStr_Mats_20FD4, 0);
                q = cur; cur += 1; v = *q;
                q = cur; cur += 1; BASEPOLY(arg0)->unk80 = *q;
                BASEPOLY(arg0)->nMats = 0;
                BASEPOLY(arg0)->matCap = v;
                if (v != 0) {
                    pp = (s32 *)(arg0 + 0x84);
                    *pp = CALL(mem_new)(v * 4);
                }
                CALL(BasePolyShape_parseMats)(arg0, BASEPOLY(arg0)->unk80);
                CALL(memtrack_end)();
                break;
            case 31:
                CALL(sys_fatal)(sStr_parsed_materials_broken_20FDC);
                break;
            case 4:
                q = cur; cur += 1; BASEPOLY(arg0)->unk64 = *q;
                break;
            case 7:
                q = cur; cur += 1; BASEPOLY(arg0)->unk78 = *q;
                break;
            case 8:
                q = cur; cur += 1; BASEPOLY(arg0)->alphas = (u8 *)*q;
                p = arg0 + 0x34;
                if (1) { *(s32 *)(p + 4) |= 8; }
                break;
            case 9:
                q = cur; cur += 1; BASEPOLY(arg0)->vtxColours = (u32 *)*q;
                break;
            case 26:
                q = cur; cur += 1; BASEPOLY(arg0)->nTris = *q;
                q = cur; cur += 1; BASEPOLY(arg0)->triList = (s16 *)*q;
                break;
            case 0:
                lflag = 1;
                break;
            case 11:
                q = cur; cur += 1; w = *q;
                p = arg0 + 0x34;
                if (w != 0) {
                    *(s32 *)(p + 4) |= 0x2000;
                } else {
                    *(s32 *)(p + 4) &= ~0x2000;
                }
                break;
            case 36:
                cur += 3;
                CALL(sys_fatal)(sStr_shape_scale_dont_work_mate_20FF4);
                break;
            case 12:
                q = cur; cur += 1; BASEPOLY(arg0)->unk98 = *q;
                break;
            case 18:
                q = cur; cur += 1;
                BASEPOLY(arg0)->unk9C = (f32) *q / 10000.0f;
                break;
            case 19:
                cur++;
                break;
            case 20:
                q = cur; cur += 1; rflag = *q;
                break;
            case 21:
                q = cur; cur += 1; w = *q;
                p = arg0 + 0x34;
                if (w != 0) {
                    *(s32 *)(p + 4) |= 0x4000;
                } else {
                    *(s32 *)(p + 4) &= ~0x4000;
                }
                break;
            case 10:
                cur++;
                break;
            case 25:
                q = cur; cur += 1; BASEPOLY(arg0)->palette = (u8 *)(*q);
                break;
            case 30:
                q = cur; cur += 1; BASEPOLY(arg0)->unk48 = *q;
                q = cur; cur += 1; BASEPOLY(arg0)->unk6C = *q;
                break;
            case 34:
                q = cur; cur += 1; BASEPOLY(arg0)->vtxNormals = (f32 *)(*q);
                q = cur; cur += 1; BASEPOLY(arg0)->faceNormals = (f32 *)(*q);
                break;
            case 32:
                q = cur; cur += 1; BASEPOLY(arg0)->unk74 = *q;
                q = cur; cur += 1; BASEPOLY(arg0)->remap = (u16 *)(*q);
                break;
            default:
                cur = (s32 *)CALL(BasePolyShape_parseCmd)(arg0, op, cur);
                break;
            case 1:
                break;
            }
            q = cur; cur += 1; op = *q & 0xFFFF;
        } while (op != 2);
    }
    if (dflag != 0) {
        if ((int)BASEPOLY(arg0)->palette == 0 && (int)BASEPOLY(arg0)->vtxNormals == 0) {
            n = BASEPOLY(arg0)->nVerts;
            BASEPOLY(arg0)->vtxNormals = (f32 *)(CALL(mem_new)(n * 0xC));
        }
        if ((int)BASEPOLY(arg0)->faceNormals == 0) {
            n = (s32)BASEPOLY(arg0)->nTris;
            BASEPOLY(arg0)->faceNormals = (f32 *)(CALL(mem_new)(n * 0xC));
        }
        if ((int)BASEPOLY(arg0)->palette != 0) {
            CALL(BasePolyShape_genFaceNormals)(arg0);
        } else {
            CALL(BasePolyShape_genNormals)(arg0, lflag, 1);
        }
    }
    if (rflag != 0) {
        CALL(ZoneShape_build)(arg0);
    }
    if (!(gPolyFlags_3EEC4 & 0x100)) {
        if ((int)BASEPOLY(arg0)->faceNormals != 0) {
            CALL(mem_delete)((int)BASEPOLY(arg0)->faceNormals);
            BASEPOLY(arg0)->faceNormals = (f32 *)0;
        }
    }
    return cur;
}
