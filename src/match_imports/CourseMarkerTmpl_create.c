/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
#include "gdl/node.h"
#include "game/course.h"
#include "game/game_uso_data.h" /* kStr_ / kTag_ / kVtbl_ (CourseMarker group 0x6B0..0x6CC) */

/* CourseMarkerTmpl_create (course.h CourseMarker): new CourseMarker (0x8C) = ColourItem base, vptr
 * Data 0x578, then its "CourseMarker::Parms" (wid float 1.0, ext int, flg int); the owner type's
 * +0x40 node, when set, becomes the marker's child. */
#define CM(p) ((CourseMarker *)(p))
#define MPARMS(p) ((CourseMarkerParms *)(p))
#define PARM(p) ((CourseParm *)(p))
#define FPARM(p) ((CourseFParm *)(p))
char *CourseMarkerTmpl_create(char *owner) {
    char *result;
    union { char *allocated; char *nodes[1]; } carrier;
    char *embedded;
    One1 slot0, slot1, slot2;
    int pad_mid[1];
    char *header;
    char *allocation;
    char *child;
    One1 marsh;
    char *node;
    allocation = (char *)game_uso_func_055750(0x8C);
    carrier.allocated = (char *)allocation;
    if (carrier.allocated != 0) {
        node = 0;
        uso_base_init_051C28((SnwBase3D5BC *)((int)allocation | (int)node));
        ((SnwBase3D5BC *)carrier.nodes[0])->field_28 = &game_uso_D_807FEB68;
        embedded = (char *)&CM(carrier.allocated)->parms;
        if (embedded != 0 || (embedded = (char *)game_uso_func_055750(0x50)) != 0) {
            header = embedded;
            if (header != 0 || (header = (char *)game_uso_func_055750(8)) != 0) {
                MPARMS(header)->name = (int)(&game_uso_D_807FECA0 + kStr_CourseMarker_Parms_6B0);
                MPARMS(header)->list = 0;
            }
            slot0.a = *(int *)(&game_uso_D_807FECB4 + kTag_wid_6C4);
            marsh = slot0;
            child = (char *)&MPARMS(embedded)->wid;
            if (child != 0 || (child = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(child, embedded, marsh, 1);
                PARM(child)->vptr = &game_uso_D_807FE9B8 + kVtbl_FloatParm_3C8;
                FPARM(child)->value2 = 0;
                FPARM(child)->value = 1.0f;
            }
            slot1.a = *(int *)(&game_uso_D_807FECB8 + kTag_ext_6C8);
            marsh = slot1;
            child = (char *)&MPARMS(embedded)->ext;
            if (child != 0 || (child = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(child, embedded, marsh, 1);
                PARM(child)->vptr = &game_uso_D_807FEA00 + kVtbl_IntParm_410;
                PARM(child)->value = 0;
                PARM(child)->value2 = 0;
            }
            slot2.a = *(int *)(&game_uso_D_807FECBC + kTag_flg_6CC);
            marsh = slot2;
            child = (char *)&MPARMS(embedded)->flg;
            if (child != 0 || (child = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(child, embedded, marsh, 0);
                PARM(child)->vptr = &game_uso_D_41C0_last + kVtbl_IntParm_410;
                PARM(child)->value2 = 0;
                PARM(child)->value = 0;
            }
        }
    }
    result = carrier.allocated;
    /* RAW: unknown-object: owner + 0x40 node link on an untyped owner */
    node = *(char **)(owner + 0x40);
    if (node != 0) {
        result = carrier.allocated;
        carrier.nodes[0] = node;
        game_uso_func_07ACE0(&((Node *)result)->children, carrier.nodes[0]); /* list_append */
        if (((Node *)carrier.nodes[0])->parent != 0) ((Node *)carrier.nodes[0])->unk04 = 1;
        ((Node *)carrier.nodes[0])->parent = (Node *)result;
    }
    return result;
}
