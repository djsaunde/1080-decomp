#!/usr/bin/env python3
"""Compiler launcher for the port build (CMAKE_<LANG>_COMPILER_LAUNCHER).

    cc-select.py <compiler> <args...>

When the compile has `-include .../port/gen/tu/<tu>.h` (written by
scripts/port-gen-build.py), the unit is compiled to assembly, filtered, and
assembled:

  port-drop: SYM          the unit's own definition of SYM is renamed to the
                          local `__port_dead_SYM` (its `.globl` / `.weak` is
                          removed).  Every reference in the unit keeps the name
                          SYM and binds, at link time, to the one shipped body
                          (port/sourcemap.tsv).  The header's `#pragma weak SYM`
                          has already stopped gcc from inlining the placeholder.
                          The dead copy is unreferenced; --gc-sections drops it.
  port-alias: NEW TARGET  NEW is defined as a global alias of TARGET (the
                          shipped name of a cfront-mangled C++ donor body, which
                          the matching build gets from objcopy --redefine-sym).
                          A mangled `_Z` TARGET is made local, since every donor
                          has its own `Obj` class (Obj::run in two donors).
  port-noinit: TARGET     TARGET's .init_array entry is removed: the static
                          initialiser runs when its module "loads" (plan
                          decision 4), under its shipped name.
  port-local: SYM         SYM's `.globl` is removed (file-local junk such as the
                          asm-processor `_asmpp_large_func*` stubs).

Units without such a header are compiled unchanged.
"""
import os
import re
import subprocess
import sys

DIRECTIVE_RE = re.compile(r"/\*\s*port-(drop|alias|noinit|local):\s*([^*]*?)\s*\*/")


def parse_header(path):
    drop, alias, noinit, local = set(), [], set(), set()
    with open(path) as fh:
        for line in fh:
            for kind, arg in DIRECTIVE_RE.findall(line):
                parts = arg.split()
                if kind == "drop":
                    drop.add(parts[0])
                elif kind == "local":
                    local.add(parts[0])
                elif kind == "alias":
                    alias.append((parts[0], parts[1]))
                else:
                    noinit.add(parts[0])
    return drop, alias, noinit, local


def filter_asm(text, drop, alias, noinit, local=()):
    out = []
    dead = None
    found = set()
    local_targets = {t for _, t in alias if t.startswith("_Z")} | set(local)
    section = ""
    for line in text.splitlines():
        s = line.strip()
        toks = re.split(r"[\s,]+", s)
        if toks and toks[0] in (".section", ".text", ".data", ".bss"):
            section = toks[1] if toks[0] == ".section" and len(toks) > 1 else toks[0]
        if len(toks) >= 2 and toks[0] in (".globl", ".weak") and toks[1] in drop:
            continue  # the reference stays a plain global undefined symbol
        if len(toks) >= 2 and toks[0] == ".globl" and toks[1] in local_targets:
            continue
        if len(toks) >= 2 and toks[0] == ".type" and toks[1] in drop:
            dead = toks[1]
            found.add(dead)
            out.append(line.replace(dead, "__port_dead_" + dead, 1))
            continue
        if dead is not None:
            if s == dead + ":":
                out.append("__port_dead_%s:" % dead)
                continue
            if toks[0] == ".size" and len(toks) > 1 and toks[1] == dead:
                out.append("\t.size\t__port_dead_%s, .-__port_dead_%s" % (dead, dead))
                dead = None
                continue
        if section == ".init_array" and len(toks) == 2 and toks[0] == ".long" and toks[1] in noinit:
            continue
        out.append(line)
    if dead is not None:
        raise SystemExit("cc-select: unterminated dropped body %s" % dead)
    # `.text` before the aliases: gas `.set` needs no section, but keep it tidy.
    for new, target in alias:
        out += ["\t.globl\t%s" % new, "\t.type\t%s, @function" % new, "\t.set\t%s, %s" % (new, target)]
    return "\n".join(out) + "\n", found


def main():
    argv = sys.argv[1:]
    hdr = None
    for i, a in enumerate(argv):
        if a == "-include" and i + 1 < len(argv) and "/port/gen/tu/" in argv[i + 1]:
            hdr = argv[i + 1]
    if hdr is None or "-c" not in argv or "-o" not in argv:
        os.execvp(argv[0], argv)
    drop, alias, noinit, local = parse_header(hdr)
    oi = argv.index("-o")
    obj = argv[oi + 1]
    asm = obj + ".s"
    cmd = list(argv)
    cmd[cmd.index("-c")] = "-S"
    cmd[oi + 1] = asm
    p = subprocess.run(cmd)
    if p.returncode:
        return p.returncode
    with open(asm) as fh:
        text, found = filter_asm(fh.read(), drop, alias, noinit, local)
    with open(asm, "w") as fh:
        fh.write(text)
    m32 = [a for a in argv if a in ("-m32", "-m64")]
    p = subprocess.run([argv[0]] + m32 + ["-c", "-o", obj, asm])
    return p.returncode


if __name__ == "__main__":
    sys.exit(main())
