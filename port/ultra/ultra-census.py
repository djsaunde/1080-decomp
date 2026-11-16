#!/usr/bin/env python3
"""libultra coverage census for the port's shim (port/ultra).

Decision 6 of docs/1080_PC_PORT_PLAN.md: the kernel and the game_libs copies of
libultra are not compiled for the port; port/ultra replaces them. This script
says, for every libultra row of port/sourcemap.tsv, what the port does with it,
and checks that every libultra function the GAME calls is provided.

  libultra row  = a game_libs function in the libultra part of the segment
                  (source offset >= 0x69E04 = osSendMesg, src/libultra/*.s, or
                  the mixed libultra hosts post1b2*, post1c, post2b*), or one
                  whose name is an os / __os / gu name. Every kernel row is out
                  of the port (the kernel is replaced as a whole).
  game caller   = a selected body (sourcemap) outside those rows that names the
                  function. Calls are found after preprocessing in the matching
                  configuration with CALL(f) / FCALL(f) / GCALL(f) / XCALL(t, f)
                  redefined to f, so placeholder calls count by their real callee.

Writes port/ultra/provided.tsv:
  function  source  action  provider  game_callers
  action: shim   - port/ultra defines it (same name, or a PORT_ALIAS of the libultra name)
          decomp - the port compiles the decomp body (portable C: gu*, fsin / fcos)
          host   - the host C library provides it (memcpy, strlen, sqrtf)
          drop   - libultra internal; no game body calls it
          MISSING- a game body calls it and nothing provides it (listed on stdout)

Usage: port/ultra/ultra-census.py   (from the project root; needs gcc for cpp)
"""
import collections
import importlib.util
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.chdir(ROOT)
spec = importlib.util.spec_from_file_location("sm", "scripts/port-sourcemap.py")
sm = importlib.util.module_from_spec(spec)
spec.loader.exec_module(sm)

LIB_START = 0x69E04
MIXED_HOSTS = re.compile(r"game_libs_(post1b2c?|post1c|post2b(_\w)?)\.c$")
NAME_RE = re.compile(r"^(gl_)?_*(os|gu)[A-Z]|^gl___?os|^gl_gu|^gl___u?ll|^gl__[A-Z]")
DECOMP = {"guOrtho", "guOrthoF", "guMtxF2L", "guMtxL2F", "guMtxIdentF", "gl_guMtxIdent",
          "guPosition", "guPositionF", "fsin", "fcos"}
HOST = {"memcpy", "strlen", "sqrtf", "bzero"}
CPP_NAMES = ["-DUSO_CALL_H", "-DCALL(f)=f", "-DFCALL(f)=f", "-DGCALL(f)=f", "-DXCALL(t,f)=f"]


def is_libultra(fn, seg, src):
    if seg != "game_libs":
        return False
    if src.startswith("src/libultra/") or MIXED_HOSTS.search(src):
        return True
    m = re.search(r"_([0-9A-F]{5})\.c$", src)
    if m and int(m.group(1), 16) >= LIB_START and "_o2_" not in src:
        return True
    return bool(NAME_RE.search(fn))


def shim_symbols():
    defs = set()
    d = os.path.join("port", "ultra")
    for f in sorted(os.listdir(d)):
        if not f.endswith(".c") or f.startswith("test_"):
            continue
        text = open(os.path.join(d, f)).read()
        defs |= set(re.findall(r"PORT_ALIAS\(\s*\w+\s*,\s*(\w+)\s*\)", text))
        for m in re.finditer(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{", text, re.M):
            defs.add(m.group(1))
        defs |= set(re.findall(r"^(?:u8|s32|u32|u64)\s+(\w+)\s*(?:\[[^\]]*\])?\s*=", text, re.M))
    return defs


def game_uses(rows, ultra):
    names = set(ultra)
    game = collections.defaultdict(set)
    for fn, seg, src, kind, how, host in rows:
        if seg != "kernel" and fn not in ultra:
            game[src].add(fn)
    uses = collections.defaultdict(set)
    for src, fns in sorted(game.items()):
        if src.endswith(".s"):
            continue
        text, rc, err = sm.cpp(src, CPP_NAMES)
        text = "\n".join(l for l in text.splitlines()
                         if not l.lstrip().startswith("#") and not re.match(r"\s*GLOBAL_ASM\s*\(", l))
        toks = [m.group(0) for m in sm.TOK_RE.finditer(text) if m.lastgroup != "ws"]
        depth = fs = 0
        fss, stmts, cur = [], [[]], None
        for t in toks:
            if depth == fs:
                if t == ";":
                    stmts.append([])
                    continue
                if t == "{":
                    c = stmts[-1]
                    if len(c) >= 2 and c[-2] == "extern" and c[-1].startswith('"'):
                        fss.append(fs)
                        depth += 1
                        fs = depth
                        stmts = [[]]
                        continue
                    cur = sm.decl_name(stmts)
                    depth += 1
                    continue
                if t == "}":
                    if fss:
                        depth -= 1
                        fs = fss.pop()
                    stmts = [[]]
                    continue
                stmts[-1].append(t)
                continue
            if t == "{":
                depth += 1
            elif t == "}":
                depth -= 1
                if depth == fs:
                    stmts, cur = [[]], None
            elif cur in fns and t in names and t != cur:
                uses[t].add(cur)
    return uses


def main():
    rows = [l.rstrip("\n").split("\t") for l in open("port/sourcemap.tsv") if not l.startswith("#")][1:]
    ultra = {fn: src for fn, seg, src, kind, how, host in rows if is_libultra(fn, seg, src)}
    shim = shim_symbols()
    uses = game_uses(rows, ultra)
    out = ["# generated by port/ultra/ultra-census.py -- do not edit",
           "function\tsource\taction\tprovider\tgame_callers"]
    counts = collections.Counter()
    missing = []
    for fn in sorted(ultra, key=lambda f: (ultra[f], f)):
        callers = sorted(uses.get(fn, ()))
        if fn in shim:
            action, prov = "shim", "port/ultra"
        elif fn in DECOMP:
            action, prov = "decomp", ultra[fn]
        elif fn in HOST:
            action, prov = "host", "libc"
        elif callers:
            action, prov = "MISSING", "-"
            missing.append((fn, callers))
        else:
            action, prov = "drop", "-"
        counts[action] += 1
        out.append("\t".join([fn, ultra[fn], action, prov, ",".join(callers) or "-"]))
    with open("port/ultra/provided.tsv", "w") as f:
        f.write("\n".join(out) + "\n")
    print("libultra rows: %d  (%s)" % (len(ultra), ", ".join("%s %d" % kv for kv in sorted(counts.items()))))
    called = [fn for fn in ultra if uses.get(fn)]
    print("called by game bodies: %d, provided: %d" % (len(called), len(called) - len(missing)))
    for fn, callers in missing:
        print("MISSING %s <- %s" % (fn, " ".join(callers)))
    return 1 if missing else 0


if __name__ == "__main__":
    sys.exit(main())
