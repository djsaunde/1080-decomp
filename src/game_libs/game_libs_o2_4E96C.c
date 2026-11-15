/* IDO 7.1 -O2 donor for Window_doCommand -- 9-way parameter-stream decoder
 * (0x1B8 / 110 words; word-exact apart from the jumptable reloc field).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 52CD4 / 3AC5C / 61F8C recipe, docs/MATCHING_WORKFLOW.md#post0b-jumptable-donor-52cd4).
 * switch (op) cases 109..117: the 9-entry table sits at donor .rodata +0,
 * renamed Window_doCommand_rodata and pinned to bootup.uso RoData +0x1B84
 * (the extractor dump at roff 0x1B88 lists all nine arms in case order,
 * arm = target - shim - vaddr with no -8; case 114 = the default arm).
 *
 * Load-bearing shape: in cases 109 and 112 the stream cursor lives in the
 * same int local (v) that holds the trunc.w.s result in cases 110/111, and
 * the loaded word lives in the pointer local (p) the other arms use as the
 * cursor. That puts the cursor in v1 and the word in v0, as in the target;
 * separate locals give v0/v1 the other way round. */
#include "common.h"
#include "gdl/window.h"

extern int gl_func_00000000();

typedef struct { int op; int *args; } C4E96C;
#define A4E96C(c) (p = (c)->args, (c)->args = p + 1, *p)

void Window_doCommand(char *a0, C4E96C *a1) {
    int *p;
    int v;
    switch (a1->op) {
    case 109:
        v = (int)a1->args; a1->args = (int *)v + 1; p = *(int **)v;
        ((Window *)a0)->unk100 = 0;
        ((Window *)a0)->clearFlags = (int)p;
        break;
    case 117:
        ((Window *)a0)->displayMask = A4E96C(a1);
        break;
    case 110:
        v = (int)(float)A4E96C(a1);
        ((Window *)a0)->originX = v;
        ((Window *)a0)->vpX = v;
        v = (int)(float)A4E96C(a1);
        ((Window *)a0)->originY = v;
        ((Window *)a0)->vpY = v;
        break;
    case 111:
        v = (int)(float)A4E96C(a1);
        ((Window *)a0)->width = v;
        ((Window *)a0)->vpW = v;
        v = (int)(float)A4E96C(a1);
        ((Window *)a0)->height = v;
        ((Window *)a0)->vpH = v;
        break;
    case 112:
        v = (int)a1->args; a1->args = (int *)v + 1; p = *(int **)v;
        ((Window *)a0)->flags = (int)p;
        break;
    case 113:
        ((Window *)a0)->projType = A4E96C(a1);
        break;
    case 115:
        ((Window *)a0)->near = A4E96C(a1);
        break;
    case 116:
        ((Window *)a0)->far = A4E96C(a1);
        break;
    default:
        gl_func_00000000(a0, a1);
        break;
    }
}
