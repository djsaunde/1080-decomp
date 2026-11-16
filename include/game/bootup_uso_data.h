#ifndef GAME_BOOTUP_USO_DATA_H
#define GAME_BOOTUP_USO_DATA_H

/* bootup.uso (bootup_uso + game_libs) data: named SECTION OFFSETS (agent-g run 593).
 *
 * gl_data.h's GL_DATA(K) / GL_DATA_STR(K, "text") spell a section-base site directly off the
 * zero placeholder. Some matched bodies read the section through a local base pointer
 * (`table = (char *)&D_00000000; ... table + K`), where respelling the site as GL_DATA(K)
 * changes the codegen (the local is the matching lever). Those K are named here instead, the
 * game_uso_data.h way: plain integers, so the site preprocesses to the same expression.
 * `scripts/uso-data-sites.py check` (gate step 5) verifies each value claim ("text") against
 * the bootup.uso section bytes and each function that uses a constant (src/game_libs,
 * src/bootup_uso and their src/match_imports ports) against a section-base LO16 reloc of that
 * section with that K. Macros only: C and C++ safe. */

/* ---- Data: RaceTracker gate-kind parm tags (3-letter strings read as a word; the D0D0 lookup
 * key that RaceTracker_findGates passes by value) ---- */
#define kTag_chk_D680 0xD680 /* Data+0xD680: "chk" (the next checkpoint gate: RaceEntry.gate) */
#define kTag_gtl_D684 0xD684 /* Data+0xD684: "gtl" (fork gate, left: a RaceEntry.gateA / gateB candidate) */
#define kTag_gtr_D688 0xD688 /* Data+0xD688: "gtr" (fork gate, right) */

#endif
