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
  port-alias: NEW TARGET [ALSO..]  NEW is defined as a global alias of TARGET (the
                          shipped name of a cfront-mangled C++ donor body, which
                          the matching build gets from objcopy --redefine-sym).
                          A mangled `_Z` TARGET is made local, since every donor
                          has its own `Obj` class (Obj::run in two donors).
  port-noinit: TARGET     TARGET's .init_array entry is removed: the static
                          initialiser runs when its module "loads" (plan
                          decision 4), under its shipped name.
  port-local: SYM         SYM's `.globl` is removed (file-local junk such as the
                          asm-processor `_asmpp_large_func*` stubs).
  port-dropdata: SYM      the unit's definition of the data object SYM is renamed to
                          the local `__port_dead_SYM`; references keep SYM (aliased
                          to the module blob by port/gen/aliases.ld, or bound).
  port-bind: FUNC TOK+N TGT  (data) only the references `TOK+N` (N the addend, signed
                          decimal; `TOK` alone is N = 0) become `TGT+N`: a token that
                          reaches several blob objects in one function, told apart by
                          the ROM site's addend.
  port-bind: FUNC TOK TGT inside FUNC's body (and its gcc clones FUNC.part.N / .isra.N
                          ...), every reference to the placeholder token TOK becomes TGT:
                          the ROM callee of all of FUNC's unnamed TOK calls
                          (scripts/port-gen-binds.py, plan section 14).

Units without such a header are compiled unchanged.
"""
import os
import re
import subprocess
import sys

DIRECTIVE_RE = re.compile(r"/\*\s*port-(dropdata|drop|alias|noinit|local|bind):\s*([^*]*?)\s*\*/")


BINDS = {}


DROPDATA = set()


def parse_header(path):
    drop, alias, noinit, local = set(), [], set(), set()
    with open(path) as fh:
        for line in fh:
            for kind, arg in DIRECTIVE_RE.findall(line):
                parts = arg.split()
                if kind == "drop":
                    drop.add(parts[0])
                elif kind == "dropdata":
                    DROPDATA.add(parts[0])
                elif kind == "local":
                    local.add(parts[0])
                elif kind == "bind":
                    m = re.fullmatch(r"(\w+)([+-]\d+)", parts[1])
                    if m:
                        BINDS.setdefault(parts[0], {}).setdefault(m.group(1), {})[int(m.group(2))] = parts[2]
                    else:
                        BINDS.setdefault(parts[0], {})[parts[1]] = parts[2]
                elif kind == "alias":
                    alias.append((parts[0], parts[1]))
                    for also in parts[2:]:
                        # `port-alias: NEW TARGET ALSO..`: TARGET is an entry (a cfront ctor's
                        # port_ctor_NEW, plan 18.11) and the body NEW's binds name is ALSO
                        alias.append((parts[0], also))
                else:
                    noinit.add(parts[0])
    # a bound function compiled under another label (a C++ donor's `port-alias: NEW TARGET`:
    # the asm has only TARGET, e.g. RcpMgr_service = _ZN3Obj3runEv) binds inside TARGET
    for new, target in alias:
        if new in BINDS:
            BINDS.setdefault(target, {}).update(BINDS[new])
    return drop, alias, noinit, local


def bind_calls(text, binds):
    """rename placeholder tokens to their ROM callee inside the bound functions' bodies"""
    if not binds:
        return text
    out, cur = [], None
    for line in text.splitlines():
        m = re.match(r"^([\w.$]+):", line)
        if m:
            base = m.group(1).split(".")[0]
            if m.group(1) in binds or (base in binds and not m.group(1).startswith(".")):
                cur = binds.get(m.group(1)) or binds[base]
            elif not m.group(1).startswith("."):
                cur = None
        elif cur and re.match(r"\s*\.size\s", line):
            out.append(line)
            cur = None
            continue
        if cur and not line.lstrip().startswith("."):
            for tok, tgt in cur.items():
                if isinstance(tgt, dict):       # per addend: {N: target}
                    def sub(m, tgt=tgt):
                        n = int(m.group(1) or "0")
                        if n in tgt:
                            return tgt[n] + (m.group(1) or "")
                        # g++ folds a member offset into the addend (`D+0x128` then `->y`:
                        # D_00000000+300): the object is the nearest bound addend below
                        # (within 0x100); every bound target is the blob base where TOK = 0
                        lower = [k for k in tgt if k <= n and n - k < 0x100]
                        return tgt[max(lower)] + (m.group(1) or "") if lower else m.group(0)
                    line = re.sub(r"(?<![\w.])%s([+-]\d+)?(?![\w$])" % re.escape(tok), sub, line)
                else:
                    line = re.sub(r"(?<![\w.])%s(?![\w$])" % re.escape(tok), tgt, line)  # `$tok`: an immediate
        out.append(line)
    return "\n".join(out) + "\n"


def filter_asm(text, drop, alias, noinit, local=()):
    # a bound label that gcc emitted as a `.set` alias of another body binds inside that body:
    # a C++ ctor / dtor's complete-object symbol (C1 / D1) is `.set` to the base-object one
    # (C2 / D2), which holds the code (Shadow_ctor_0004B0A8 = _ZN3ObjC1EP3Src, plan 18.8)
    for a, b in re.findall(r"^\s*\.set\s+([\w.$]+)\s*,\s*([\w.$]+)\s*$", text, re.M):
        if a in BINDS:
            BINDS.setdefault(b, {}).update(BINDS[a])
    text = bind_calls(text, BINDS)
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
        if len(toks) >= 2 and toks[0] in (".globl", ".weak") and toks[1] in drop | DROPDATA:
            continue
        if section.startswith(".debug") and len(toks) == 2 and toks[1] in DROPDATA:
            # the dropped object's DWARF location (DW_OP_addr) names the local dead copy: the
            # global SYM is a placeholder the code binds per function, and is defined nowhere
            # (bootup_uso_ncc_6204's `Bu6204Ob D_00000000`, plan 18.8)
            out.append(line.replace(toks[1], "__port_dead_" + toks[1], 1))
            continue
        if len(toks) >= 2 and toks[0] in (".type", ".size") and toks[1] in DROPDATA:
            found.add(toks[1])
            out.append(line.replace(toks[1], "__port_dead_" + toks[1], 1))
            continue
        if s.endswith(":") and s[:-1] in DROPDATA:
            out.append("__port_dead_" + s)
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
    done = set()
    for new, target in alias:
        if new in done:
            continue    # the extra (bind-only) targets of a `port-alias: NEW TARGET ALSO..`
        done.add(new)
        out += ["\t.globl\t%s" % new, "\t.type\t%s, @function" % new, "\t.set\t%s, %s" % (new, target)]
    return "\n".join(out) + "\n", found


def x32_to_elf64(text):
    """the x32 unit's tables that the ELF64 loader reads with 8-byte entries: the constructor /
    destructor arrays (.init_array / .fini_array / .ctors / .dtors) hold `.long fn` in x32"""
    out = []
    section = ""
    for line in text.splitlines():
        toks = re.split(r"[\s,]+", line.strip())
        if toks and toks[0] in (".section", ".text", ".data", ".bss", ".previous", ".popsection"):
            section = toks[1] if toks[0] == ".section" and len(toks) > 1 else toks[0]
        if section.split(".")[1:2] and section.split(".")[1] in ("init_array", "fini_array", "ctors", "dtors") \
                and toks and toks[0] == ".long":
            line = line.replace(".long", ".quad", 1)
        out.append(line)
    return "\n".join(out) + "\n"


def main():
    argv = sys.argv[1:]
    hdr = None
    for i, a in enumerate(argv):
        if a == "-include" and i + 1 < len(argv) and "/port/gen/tu/" in argv[i + 1]:
            hdr = argv[i + 1]
    # PORT_64 (plan 21): -mx32 units (the ILP32 data model in x86-64 code) are compiled to
    # assembly and assembled as ELF64 objects (-m64), so they link into the x86-64 executable
    x32 = "-mx32" in argv
    if (hdr is None and not x32) or "-c" not in argv or "-o" not in argv:
        os.execvp(argv[0], argv)
    drop, alias, noinit, local = parse_header(hdr) if hdr else (set(), [], set(), [])
    oi = argv.index("-o")
    obj = argv[oi + 1]
    asm = obj + ".s"
    cmd = list(argv)
    cmd[cmd.index("-c")] = "-S"
    cmd[oi + 1] = asm
    p = subprocess.run(cmd)
    if p.returncode:
        return p.returncode
    if hdr:
        with open(asm) as fh:
            text, found = filter_asm(fh.read(), drop, alias, noinit, local)
        with open(asm, "w") as fh:
            fh.write(text)
    if x32:
        with open(asm) as fh:
            text = x32_to_elf64(fh.read())
        with open(asm, "w") as fh:
            fh.write(text)
    m32 = ["-m64"] if x32 else [a for a in argv if a in ("-m32", "-m64")]
    p = subprocess.run([argv[0]] + m32 + ["-c", "-o", obj, asm])
    return p.returncode


if __name__ == "__main__":
    sys.exit(main())
