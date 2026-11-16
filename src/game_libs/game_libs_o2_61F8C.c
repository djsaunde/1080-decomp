/* IDO 7.1 -O2 donor for AnimData_load -- (cmd, val) record loader
 * (0x208 / 130 words; 130/130 word-exact apart from reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 52CD4 / 3AC5C recipe, docs/MATCHING_WORKFLOW.md#post0b-jumptable-donor-52cd4).
 * switch (cmd) cases 1..5: the 5-entry table sits at donor .rodata +0, renamed
 * AnimData_load_rodata and pinned to bootup.uso RoData +0x2080 (the
 * extractor dump at roff 0x2084 lists exactly the five arms in address order).
 *
 * Load-bearing shapes:
 *  - the stream reader (jal 0x0C01D913) and the debug print (jal 0x0C01D900)
 *    are NONZERO baked jals -> gl_ref_0007644C / gl_ref_00076400 pins; the
 *    default arm and the exit check are plain jal 0 (gl_func_00000000).
 *  - the default arm passes cmd as a1, which is why cmd is loaded into a1.
 *  - case 1 prints the value it just stored (`*(int *)(self + 0xC)`): IDO
 *    forwards the stored t7 (`or a1,t7`); passing `val` reloads it from the
 *    stack (&val escaped), and a named copy adds 8 frame bytes.
 *  - `base[1]` with base = &D_00000000 constant-folds to a fresh lui t0; a
 *    volatile read through the s3 base keeps the target's `lw t0,4(s3)`. */
#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/anim.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int D_00000000;
extern int gl_ref_0007644C(); /* baked jal 0x0C01D913: stream int reader */
extern int gl_ref_00076400(); /* baked jal 0x0C01D900: debug print */
extern char sFmt_numkeys_2205C[], sFmt_numnodes_22068[], sFmt_frame_data_22078[];
extern char sFmt_numframes_22088[], sFmt_unknown_anim_cmd_22098[], sStr_AMgr_bad_num_Nodes_220AC[];

/* Strings: 1 "numkeys %d", 2 "numnodes %d", 3 "frame data %d", 5 "numframes %d",
 * default "unknown anim cmd %d", exit "AMgr: bad num Nodes". `base` is gSysLib, the stream
 * the anim file is read through (base[1] != 0: leave frame data in the file, keep its offset). */
void AnimData_load(AnimData *self) {
    int *base;
    int cmd;
    int val;
    int n;
    int buf;

    base = &gSysLib;
    self->numNodes = 0;
    for (;;) {
        gl_ref_0007644C(&cmd);
        gl_ref_0007644C(&val);
        if (cmd == 0) {
            break;
        }
        switch (cmd) {
        case 1: /* keys: val AnimKey records */
            n = val * 8;
            if (self->keepKeys == 0) {
                self->numKeys = val;
                gl_ref_00076400(sFmt_numkeys_2205C, self->numKeys);
                self->keys = (AnimKey *)CALL(mem_new)(self->numKeys * 8);   /* mem_new */
                CALL(fs_gdlReadFile)(base, self->keys, n);              /* fs_gdlReadFile */
            } else {
                buf = CALL(mem_new)(n);
                CALL(fs_gdlReadFile)(base, buf, n);
                CALL(mem_delete)(buf);                              /* mem_delete */
            }
            break;
        case 2:
            self->numNodes = val;
            gl_ref_00076400(sFmt_numnodes_22068, self->numNodes);
            break;
        case 3: /* frame data: val bytes = numFrames * numNodes * 14 */
            n = val;
            gl_ref_00076400(sFmt_frame_data_22078, n);
            if (((volatile int *)base)[1] == 0) {
                self->frameData = CALL(mem_newAligned)(n, 8);         /* mem_newAligned */
                CALL(fs_gdlReadFile)(base, self->frameData, n);
            } else {
                self->frameData = 0;
                self->fileOffset = CALL(fs_gdlSeekFile)(base, 1, 0);  /* fs_gdlSeekFile: current position */
                CALL(fs_gdlSeekFile)(base, 1, n);                     /* skip */
            }
            break;
        case 4: /* skipped block */
            CALL(fs_gdlSeekFile)(base, 1, val);
            break;
        case 5:
            self->numFrames = val;
            gl_ref_00076400(sFmt_numframes_22088, self->numFrames);
            break;
        default:
            CALL(sys_fatal)(sFmt_unknown_anim_cmd_22098, cmd);
            break;
        }
    }
    /* the second numNodes read stays a raw `*(s16 *)` deref: spelled self->numNodes,
     * the beql operands swap (t3, v0 -> v0, t3) */
    /* RAW: byte-lock: spelled self->numNodes the beql operands swap (t3, v0 -> v0, t3); see the comment above */
    if (self->numNodes != 0 && ((AnimDataMgr *)self->mgr)->numNodes != *(s16 *)((char *)self + 0)) {
        CALL(sys_fatal)(sStr_AMgr_bad_num_Nodes_220AC);
    }
    self->state = 7;
}
