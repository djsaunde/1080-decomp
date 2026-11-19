#!/usr/bin/env python3
"""port/gen/decls_cpp.h -> decls_cpp_wasm.h for the wasm32 build (plan section 30; see port/wasm/wasm_cxx.h).

    gen-decls-wasm.py <decls_cpp.h> <out.h>

Each prototype's int-class and pointer parameters become PortW<T>; a `...` tail becomes PORT_WASM_TAIL extra
defaulted parameters, except for the callees that are really variadic (their `...` stays: a wasm varargs call
passes the extra arguments in memory, which is what their bodies read)."""
import re
import sys

# the variadic C definitions the donors can name (the 21.5 list)
VARIADIC = {"ScreenWindow_ctor", "TextWin_printf", "dbg_printf", "gl_osSyncPrintf", "gl_rmonPrintf", "gl_sprintf",
            "osSyncPrintf", "snd_SetPlayer0ChannelParams_2F9D4", "sys_fatal", "sys_printStub_00067AE8",
            "sys_sprintf_00061B30", "uso_nullPrintf"}
WRAP = {"int", "unsigned int", "short", "unsigned short", "signed char", "unsigned char", "char", "void *",
        "const void *"}

# port-gen-decls.py's BYVALUE_CALLERS: a donor passes a one-word key struct where the C definition takes an int;
# a PortWA copies the struct's word (a non-POD class cannot go through `...` in clang)
BYVALUE_INT = {"ColourItemType_ctor", "ItemMgr_findType_0003EA98", "ItemMgr_queryType"}

PROTO = re.compile(r"^(?P<ret>.*?\W)(?P<name>\w+)\((?P<params>[^()]*)\);\s*$")


def param(p):
    p = p.strip()
    default = ""
    if p.endswith("= 0"):
        p, default = p[:-3].strip(), " = 0"
    if p in WRAP:
        t = "void *" if p.endswith("*") else p
        return "PortW<%s>%s" % (t, default and " = PortW<%s>()" % t)
    return p + default


def convert(line):
    m = PROTO.match(line)
    if not m or line.startswith("#") or line.startswith("/*"):
        return line
    name, params = m.group("name"), m.group("params").strip()
    ps = [x for x in params.split(",")] if params else []
    ps = [x.strip() for x in ps]
    tail = bool(ps) and ps[-1] == "..."
    if tail:
        ps = ps[:-1]
    if ps == ["void"] or ((not ps or ps == ["..."]) and name not in BYVALUE_INT):
        # `f(...)`: kept variadic. A donor passes some of these a struct by value (nodelist_walk_0005534C, the
        # BYVALUE callers); wasm-fixtypes.py turns every variadic call of a fixed definition into a direct call
        # with the arguments as given (a struct `byval`, as the definition takes it)
        return line
    out = [param(x) for x in ps]
    if tail:
        if name in BYVALUE_INT:
            out.append("PORT_WASM_TAIL_DEFAULTS")
        else:
            # kept: the extra arguments (a struct by value too: CollShape_query's V3) go as written, and
            # wasm-fixtypes.py turns the variadic call of a fixed definition into a direct one
            out.append("...")
    ret = m.group("ret")
    if re.fullmatch(r"\s*(int|int /\*void\*/|void \*|unsigned int)\s*", ret):
        ret = "PortWR "
    return "%s%s(%s);\n" % (ret, name, ", ".join(out))


def main():
    src, dst = sys.argv[1], sys.argv[2]
    lines = open(src).readlines()
    out = ['#include "wasm_cxx.h"\n'] + [convert(x) for x in lines]
    tmp = "%s.%d.tmp" % (dst, __import__("os").getpid())
    with open(tmp, "w") as fh:
        fh.writelines(out)
    import os
    os.replace(tmp, dst)


if __name__ == "__main__":
    main()
