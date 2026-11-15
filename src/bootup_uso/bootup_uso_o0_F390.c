#include "common.h"
#include "gdl/gl_data.h"
#include "uso_call.h"

/* -O0 cluster at offsets 0xF390..0xF6C3. -O0 wrappers split out of
 * bootup_uso.c so they build at -O0 (matching the originals' frame +
 * spill+reload + b-+1 patterns). See feedback_ido_o0_register_and_inline.md,
 * project_1080_bootup_uso_o0_runs.md, and
 * feedback_o0_cluster_split_with_layout_shim.md. */

extern int func_00000000();
extern char D_00000000;
extern char sStr_Node_C574;

void input_ctor_0000F390(void) {
    register char *p = &gInputMgr;
    FCALL(Node_ctor)(p, &sStr_Node_C574);
}

void SnowBoard_restart(int a0) {
    FCALL(App_restart)(a0);
}

void SnowBoard_softReset(int a0) {
    FCALL(App_softReset)(a0);
}

/* Two-block bit set/clear — set=1, masks 4 then 8.
 * MATCH KEY (re-matched 2026-06-10; previous body was a silent false match —
 * same length, registers renumbered): FIVE register vars in declaration order
 * p1->$a1, p2->$a2, set->$a3, t->$t0, u->$t1; the final clear-branch uses the
 * DISTINCT var `u` (target allocates $t1 there, while `t` stays $t0 in the
 * other three branches; a single shared `t` renumbers everything). */
void Process_start(int *a0) {
    register int *p1, *p2;
    register int set;
    register int *t;
    register int *u;
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 1;
    if (set) { t = p2; *t = *t | 4; }
    else     { t = p2; *t = *t & ~4; }
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 1;
    if (set) { t = p2; *t = *t | 8; }
    else     { u = p2; *u = *u & ~8; }
}

/* Two-block bit set/clear — set=0, masks 4 then 8.
 * MATCH KEY (re-matched 2026-06-10; previous body was a silent false match —
 * same length, registers renumbered): FIVE register vars in declaration order
 * p1->$a1, p2->$a2, set->$a3, t->$t0, u->$t1; the final clear-branch uses the
 * DISTINCT var `u` (target allocates $t1 there, while `t` stays $t0 in the
 * other three branches; a single shared `t` renumbers everything). */
void Process_stop(int *a0) {
    register int *p1, *p2;
    register int set;
    register int *t;
    register int *u;
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 0;
    if (set) { t = p2; *t = *t | 4; }
    else     { t = p2; *t = *t & ~4; }
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 0;
    if (set) { t = p2; *t = *t | 8; }
    else     { u = p2; *u = *u & ~8; }
}

/* Single-block bit set/clear — set=0, mask=8. */
void Node_clearFlag8_0000F564(int *a0) {
    register int *p1, *p2;
    register int set;
    register int *t;
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 0;
    if (set) { t = p2; *t = *t | 8; }
    else     { t = p2; *t = *t & ~8; }
}

/* Single-block bit set/clear — set=1, mask=8. */
void Node_setFlag8_0000F5BC(int *a0) {
    register int *p1, *p2;
    register int set;
    register int *t;
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 1;
    if (set) { t = p2; *t = *t | 8; }
    else     { t = p2; *t = *t & ~8; }
}

/* Single-block bit set/clear — set=0, mask=4. */
void Node_clearFlag4_0000F614(int *a0) {
    register int *p1, *p2;
    register int set;
    register int *t;
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 0;
    if (set) { t = p2; *t = *t | 4; }
    else     { t = p2; *t = *t & ~4; }
}

/* Single-block bit set/clear — set=1, mask=4. */
void Node_setFlag4_0000F66C(int *a0) {
    register int *p1, *p2;
    register int set;
    register int *t;
    p1 = (int*)((char*)a0 + 0x18);
    p2 = p1;
    set = 1;
    if (set) { t = p2; *t = *t | 4; }
    else     { t = p2; *t = *t & ~4; }
}
