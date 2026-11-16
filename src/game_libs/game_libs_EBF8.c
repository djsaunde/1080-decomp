#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"    /* NODE(p): vptr (+0x28), parent (+0x14) */
#include "gdl/gl_data.h" /* ColourItem_vtbl_75A0 */
#include "game/course.h" /* ColourSubItem (0x40: a ColourItem + unk3C) */

/* game_libs 0xEBF8..0xEC88 (bootup.uso Text+0x23264). Until 2026-11-16 these 36 words
 * were the head of the game_libs_text2_pre bin, so no unit held them and report.json did
 * not count them; scripts/rom-coverage.py found the function shape (docs/ROM_COVERAGE.md).
 * The rest of that bin is RSP IMEM.
 *
 * It is a word-for-word twin of ColourItem_create_0000C5B0 (game_libs_tail.c) and of
 * edd_ItemType_create_000003BC: the TextReloc table resolves its jals to mem_new,
 * ColourItem_ctor and list_append, and its vptr store to Data+0x75A0 (Sym 143,
 * ColourItem_vtbl_75A0), the same targets as C5B0. Ported verbatim from C5B0, whose
 * flat int* form (decl order p2,head,p1; p1 reused for arg0; the arg0[0x40] re-load)
 * is load-bearing for the register / spill-slot allocation. Here the body is exactly
 * 0x90 (jr-delay nop included): no pad word follows it. */
extern int gl_func_00000000();
extern char D_00000000;

int *ColourItem_create_0000EBF8(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&ColourItem_vtbl_75A0;
        ((ColourSubItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if (NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}
