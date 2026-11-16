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
#ifdef PORT
/* PC port (docs/1080_PC_PORT_PLAN.md decision 4, section 14): the USO system is a static
 * link, so a placeholder call names its real callee f.
 *  - C++ (the NCC donors): f itself; port/gen/decls_cpp.h (forced include) declares it.
 *  - C: f called WITH THE PLACEHOLDER'S TYPE at this site (the in-scope declaration of
 *    the token: usually K&R `int gl_func_00000000()`, or a per-site typed alias).  That is
 *    the type the matched call was compiled against, so the arguments are passed exactly as
 *    in the ROM, and no prototype of f (which may not be in scope, or may disagree with the
 *    IDO-era call) is needed.  A block-scope extern with an asm label names the symbol f;
 *    __COUNTER__ keeps each site's declaration distinct. */
#ifdef __cplusplus
#define CALL(f) f
#define FCALL(f) f
#define GCALL(f) f
#define XCALL(tok, f) f
#else
#define PORT_CALL_AS(tok, f) PORT_CALL_AS_(tok, f, __COUNTER__)
#define PORT_CALL_AS_(tok, f, n) PORT_CALL_AS__(tok, f, n)
#define PORT_CALL_AS__(tok, f, n) \
    (*({ extern __typeof__(tok) __port_call_##n __asm__(#f); &__port_call_##n; }))
#define CALL(f) PORT_CALL_AS(gl_func_00000000, f)
#define FCALL(f) PORT_CALL_AS(func_00000000, f)
#define GCALL(f) PORT_CALL_AS(game_uso_func_00000000, f)
#define XCALL(tok, f) PORT_CALL_AS(tok, f)
#endif
#else
#define CALL(f) gl_func_00000000
#define FCALL(f) func_00000000
#define GCALL(f) game_uso_func_00000000
#define XCALL(tok, f) tok
#endif

#endif
