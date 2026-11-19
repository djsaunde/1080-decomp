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
#define MEMCHECK(mem, a, t) WASM_RT_CHECK_BASE(mem); \\
  if (UNLIKELY((a) < 0x1000000u)) port_wasm_null_access(a);
#else
#define MEMCHECK(mem, a, t) WASM_RT_CHECK_BASE(mem);
#endif"""


def main():
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
