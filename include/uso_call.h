#ifndef USO_CALL_H
#define USO_CALL_H
/*
 * Named USO placeholder calls (scripts/name-placeholder-calls.py, agent-a run 508).
 *
 * The decomp links every USO at VRAM 0, so a call through an import or a
 * self-relocation is spelled with a zero placeholder (`gl_func_00000000(...)`):
 * the real callee is only in the module's TextReloc R_MIPS_26 entry for that jal.
 * These macros put the callee's name at the call site:
 *
 *     CALL(AnimPlayer_start)(anim, 0)   ==  gl_func_00000000(anim, 0)
 *
 * Each macro expands to the EXACT original placeholder token and drops its name
 * argument, so the preprocessed source, and therefore the bytes, are unchanged by
 * construction. The name is documentation, and it is checked:
 * `scripts/name-placeholder-calls.py check` re-verifies every CALL(X) against the
 * ROM relocations of its function (the readability gate runs it), and rename-symbol.py
 * rewrites X along with the callee's definition, so names cannot rot.
 *
 *   CALL(f)       gl_func_00000000         (bootup.uso / game_libs imports and the
 *                                           placeholder most modules use)
 *   FCALL(f)      func_00000000
 *   GCALL(f)      game_uso_func_00000000
 *   XCALL(tok, f) tok                      (any other placeholder token, e.g. a
 *                                           per-site alias `gl_func_00000000_send`)
 *
 * NCC .cpp donors build without -I include: include this file by relative path.
 */
#define CALL(f) gl_func_00000000
#define FCALL(f) func_00000000
#define GCALL(f) game_uso_func_00000000
#define XCALL(tok, f) tok

#endif
