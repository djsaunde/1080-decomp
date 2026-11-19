#!/usr/bin/env python3
"""Post-process wasm2c's output for the 1080 port (plan section 30).

    w2c-post.py <dir with game-impl.h>

call_indirect: wasm traps when the slot's function type differs from the call's. The game calls through
vtable / SysApi / callback slots with K&R and placeholder types (an `int (*)()` slot holding a void
function, one more or one fewer argument), which the i386 and x86-64 builds tolerate by their calling
conventions; the native build (21.12, icall-scan) reviewed the cases where it matters. So the type test is
dropped: the call goes through the C pointer as the x86-64 / arm64 native code would (wasm2c functions are
plain C functions of u32 / u64 / f32 / f64 arguments). A null or out-of-range slot still traps, through
port_wasm_bad_icall (port/wasm/host/wasm_main.c), which names the index.
"""
import os
import re
import sys

OLD = """#define CHECK_CALL_INDIRECT(table, ft, x)                \\
  (LIKELY((x) < table.size && table.data[x].func &&      \\
          func_types_eq(ft, table.data[x].func_type)) || \\
   TRAP(CALL_INDIRECT))"""
NEW = """void port_wasm_bad_icall(uint32_t x, uint32_t size);
#define CHECK_CALL_INDIRECT(table, ft, x)                \\
  (LIKELY((x) < table.size && table.data[x].func) ||     \\
   (port_wasm_bad_icall((x), table.size), 0))"""


# PORT_WASM_NULLCHECK (a debug build): the module is linked with --global-base=16 MB, so nothing lives below
# 16 MB; a load or store there is a null (or null + offset) pointer, which native code would fault on and wasm
# would quietly do. port_wasm_null_access reports it (and traps).
OLD_MC = "#define MEMCHECK(mem, a, t) WASM_RT_CHECK_BASE(mem);"
NEW_MC = """#ifdef PORT_WASM_NULLCHECK
void port_wasm_null_access(uint64_t a);
extern uint64_t port_wasm_watch_lo, port_wasm_watch_hi;
void port_wasm_watch_hit(uint64_t a, unsigned n);
#define MEMCHECK(mem, a, t) WASM_RT_CHECK_BASE(mem); \\
  if (UNLIKELY((a) < 0x1000000u)) port_wasm_null_access(a); \\
  if (UNLIKELY((a) + sizeof(t) > port_wasm_watch_lo && (a) < port_wasm_watch_hi)) port_wasm_watch_hit(a, sizeof(t));
#else
#define MEMCHECK(mem, a, t) WASM_RT_CHECK_BASE(mem);
#endif"""


# Indirect calls with fewer arguments than the slot's function takes (a K&R slot type, a cfront virtual called with
# `this` only): the callee would read whatever its argument registers hold, and in the translated code those can
# be host addresses (different every run: the audio went nondeterministic). Each call through the table also
# passes PAD_INT zero words and PAD_FLT zero floats after its own arguments: on x86-64 and arm64 they land in the
# next integer / vector argument registers, so a missing u32 or f32 parameter reads 0. A callee that takes them
# ignores extra arguments.
PAD_INT, PAD_FLT = 4, 4
CI = re.compile(r"CALL_INDIRECT\(")


def pad_calls(text):
    out, pos, n = [], 0, 0
    for m in CI.finditer(text):
        i = m.end()
        depth, j, args, start = 1, i, [], i
        while depth:
            c = text[j]
            if c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
                if depth == 0:
                    args.append(text[start:j])
            elif c == "," and depth == 1:
                args.append(text[start:j])
                start = j + 1
            j += 1
        if len(args) < 5 or "(*)(" not in args[1] or args[1].rstrip().endswith("void*)") is None:
            continue
        ty = args[1].strip()
        k = ty.rindex(")")
        ty = ty[:k] + "".join(", u32" for _ in range(PAD_INT)) + "".join(", f32" for _ in range(PAD_FLT)) + ty[k:]
        args = [a.strip() for a in args]
        args[1] = ty
        args += ["0u"] * PAD_INT + ["0.0f"] * PAD_FLT
        out.append(text[pos:m.start()])
        out.append("CALL_INDIRECT(" + ", ".join(args) + ")")
        pos = j
        n += 1
    out.append(text[pos:])
    return "".join(out), n


def main():
    import glob
    total = 0
    for c in sorted(glob.glob(os.path.join(sys.argv[1], "game_*.c"))):
        text = open(c).read()
        if "/* w2c-post: padded */" in text:
            continue
        text, n = pad_calls(text)
        total += n
        with open(c, "w") as fh:
            fh.write("/* w2c-post: padded */\n" + text)
    path = os.path.join(sys.argv[1], "game-impl.h")
    text = open(path).read()
    if NEW in text:
        return 0
    if OLD not in text or OLD_MC not in text:
        sys.stderr.write("w2c-post: %s: CHECK_CALL_INDIRECT / MEMCHECK not in the expected form (a new wasm2c?)\n"
                         % path)
        return 1
    with open(path, "w") as fh:
        fh.write(text.replace(OLD, NEW).replace(OLD_MC, NEW_MC, 1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
