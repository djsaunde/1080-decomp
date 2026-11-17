#!/usr/bin/env python3
"""PC port: check libgame.a against the source map and list what it needs
from outside (docs/1080_PC_PORT_PLAN.md section 13).

    scripts/port-undefined.py <libgame.a> [-o port/undefined.txt]

1. Selection check: every kept function of port/sourcemap.tsv (minus
   port/gen/excluded.tsv) is defined exactly once (global; or local / inlined
   when the source declares it static), no excluded function is defined, and
   no global symbol is defined twice.
2. Undefined symbols: the archive is relocatably linked as a whole with
   --gc-sections --gc-keep-exported (every exported symbol is a root, the
   renamed-dead placeholder copies fall away), and the remaining undefined
   symbols are grouped for the shim:
     libultra os*, gu*, libc (+ compiler runtime), USO / CALL plumbing,
     data aliases, C++ cross-donor references, other.
   Each line: symbol, how it is referenced (call / data / both), references.
Needs only binutils with the elf_i386 emulation (no 32-bit libc).
"""
import argparse
import collections
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "scripts"))

OS_RE = re.compile(r"^(gl_)?(_*os[A-Z]|__os|__ll_|__ull_|_VirtualToPhysicalTask|_MakeMotorData|"
                   r"corrupted|viMgrMain|rmon|__rmon|osSyncPrintf)")
GU_RE = re.compile(r"^(gl_)?(gu[A-Z]\w*|fsin|fcos|sinf|cosf|sqrtf)$")
LIBC_RE = re.compile(r"^(gl_)?(_?Printf|_?Putfld|_?Litob|_?Genld|_?Ldtob|proutSprintf|sprintf|printf|"
                     r"strchr\w*|strlen|strcpy|strncpy|strcmp|strncmp|strcat|memcpy|memmove|memset|memcmp|"
                     r"bcopy|bzero|ldiv|lldiv|abs|labs|atoi|qsort|rand|srand)$")
RT_RE = re.compile(r"^__(u?(div|mod)di3|ashldi3|ashrdi3|lshrdi3|muldi3|fixu?\w+|floatu?\w+|"
                   r"x86\.get_pc_thunk\.\w+|stack_chk_fail\w*)$|^_GLOBAL_OFFSET_TABLE_$")
PLUMB_RE = re.compile(r"(^|_)import_|^func_0{8}|^(gl_)?func_[0-9A-Fa-f]{8}|^\w+_func_[0-9A-Fa-f]{8}|"
                      r"^uso_|^gl_uso|^CALL|^XCALL|^GL_DATA|^INPUT_MGR")


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, timeout=100, **kw)


def read_tsv(path):
    return [l.rstrip("\n").split("\t") for l in open(path)
            if l.strip() and not l.startswith(("#", "function\t"))]


def selection_check(lib):
    rows = read_tsv(ROOT / "port/sourcemap.tsv")
    excluded = {r[0] for r in read_tsv(ROOT / "port/gen/excluded.tsv")}
    gdefs = collections.defaultdict(list)
    ldefs = set()
    member = None
    for line in run(["nm", "-A", "--defined-only", lib]).stdout.splitlines():
        m = re.match(r"^[^:]+:([^:]+):\s*(\S+)?\s+(\w)\s+(\S+)$", line)
        if not m:
            continue
        obj, typ, sym = m.group(1), m.group(3), m.group(4)
        if typ.isupper() and typ not in "CVW":
            gdefs[sym].append(obj)
        elif typ.islower():
            ldefs.add(sym)
    missing, static_ok, dup_kept = [], 0, []
    src_cache = {}
    for r in rows:
        func, src = r[0], r[2]
        if func in excluded:
            continue
        n = len(gdefs.get(func, []))
        if n == 1:
            continue
        if n > 1:
            dup_kept.append((func, gdefs[func]))
            continue
        if src not in src_cache:
            src_cache[src] = (ROOT / src).read_text(errors="replace")
        if func in ldefs or re.search(r"\bstatic\b[^;{]*\b%s\s*\(" % re.escape(func), src_cache[src]):
            static_ok += 1
            continue
        missing.append((func, src))
    excl_defined = sorted(f for f in excluded if f in gdefs)
    dups = {s: v for s, v in gdefs.items() if len(v) > 1}
    return {"kept": sum(1 for r in rows if r[0] not in excluded), "missing": missing,
            "static": static_ok, "dup_kept": dup_kept, "excluded_defined": excl_defined,
            "dups": dups, "nglobal": len(gdefs)}


def load_aliases(path):
    """[(name, expression, symbol the expression names)] of an implicit linker script of
    `name = expr;` lines (port/gen/aliases.ld)"""
    out = []
    if os.path.exists(path):
        for line in open(path):
            m = re.match(r"\s*(\w+)\s*=\s*([^;]+);", line)
            if m:
                out.append((m.group(1), m.group(2).strip(), re.match(r"\w+", m.group(2).strip()).group(0)))
    return out


def undefined(lib, tmp, extra=(), aliases=()):
    """extra: more archives (the port/uso library ...); aliases: load_aliases().  An alias whose
    target the inputs do not define resolves its name; the target is then counted instead."""
    stripped = os.path.join(tmp, "g.a")
    rel = os.path.join(tmp, "r.o")
    p = run(["objcopy", "--strip-debug", lib, stripped])
    if p.returncode:
        raise SystemExit(p.stderr)
    inputs = [stripped]
    for k, e in enumerate(extra):
        inputs.append(os.path.join(tmp, "x%d.a" % k))
        p = run(["objcopy", "--strip-debug", os.path.abspath(e), inputs[-1]])
        if p.returncode:
            raise SystemExit(p.stderr)
    alias_und = collections.Counter()
    if aliases:
        defs = set()
        for i in inputs:
            defs |= {l.split()[-1] for l in run(["nm", "--defined-only", i]).stdout.splitlines()
                     if len(l.split()) == 3 and l.split()[1].isupper()}
        script = os.path.join(tmp, "aliases.ld")
        twice = sorted(name for name, _, _ in aliases if name in defs)
        if twice:
            raise SystemExit("aliases.ld names %d symbols the inputs also define: %s" % (
                len(twice), " ".join(twice[:20])))
        with open(script, "w") as fh:
            for name, expr, sym in aliases:
                if sym in defs:
                    fh.write("%s = %s;\n" % (name, expr))
                else:
                    alias_und[sym] += 1
        inputs.append(script)
    p = run(["ld", "-m", "elf_i386", "-r", "--gc-sections", "--gc-keep-exported",
             "--whole-archive"] + inputs + ["-o", rel])
    if p.returncode:
        raise SystemExit(p.stderr)
    aliased = {a[0] for a in aliases}
    weak = set()
    und = []
    for l in run(["nm", "-u", rel]).stdout.splitlines():
        if l.strip():
            und.append(l.split()[-1])
            if l.split()[0] in "wv":
                weak.add(l.split()[-1])
    und = [u for u in und if u not in aliased]
    und += sorted(set(alias_und) - set(und))
    refs = collections.defaultdict(collections.Counter)
    out = subprocess.run(["objdump", "-r", rel], capture_output=True, text=True, timeout=300).stdout
    sec = ""
    for line in out.splitlines():
        if line.startswith("RELOCATION RECORDS FOR ["):
            sec = line[len("RELOCATION RECORDS FOR ["):-2]
            continue
        parts = line.split()
        if len(parts) == 3 and parts[1].startswith("R_386_"):
            sym = re.sub(r"[+-]0x[0-9a-f]+$", "", parts[2])
            kind = "call" if parts[1] in ("R_386_PC32", "R_386_PLT32") and sec.startswith(".text") else "data"
            refs[sym][kind] += 1
    for sym, n in alias_und.items():
        refs[sym]["call"] += n      # named by an alias (a call target, or a shim entry point)
    return und, refs, weak


def classify(sym, kref, autosyms, kernel_funcs, bindnames=frozenset()):
    if RT_RE.match(sym):
        return "libc: compiler runtime (libgcc)"
    if OS_RE.match(sym):
        return "libultra os* (port/ultra)"
    if GU_RE.match(sym):
        return "gu* (libultra gu, not compiled with the game_libs copy excluded)"
    if LIBC_RE.match(sym):
        return "libc (host libc wrappers)"
    if sym in kernel_funcs:
        return "USO / CALL plumbing: kernel services (USO loader, ROM fs, boot; port/uso, port/fs)"
    if PLUMB_RE.search(sym):
        return "USO / CALL plumbing: import / placeholder call targets (bound by USO relocations)"
    if sym in bindnames:
        return ("USO / CALL plumbing: match_imports binding names (scripts/match-bindings-*.json; "
                "the shipped relocation names the target)")
    if sym.startswith("_Z"):
        return "other: C++ cross-donor references (mangled; local mirror classes)"
    if sym in autosyms or re.match(r"^D[FS]?_", sym) or "vtbl" in sym:
        if kref["call"] and not kref["data"]:
            return "USO / CALL plumbing: import / placeholder call targets (bound by USO relocations)"
        return "data aliases (undefined_syms_auto.txt = 0, bound by USO relocations; extractor / port/uso)"
    return "other"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("lib")
    ap.add_argument("-o", "--out", default=str(ROOT / "port/undefined.txt"))
    ap.add_argument("--extra", action="append", default=[],
                    help="another archive linked with the library (the port/uso library)")
    ap.add_argument("--extra-objects", nargs="*", default=[],
                    help="object files linked with the library (port/main.c, the host layer)")
    ap.add_argument("--aliases", default=str(ROOT / "port/gen/aliases.ld"),
                    help="implicit linker script of token aliases ('' for none)")
    args = ap.parse_args()
    lib = os.path.abspath(args.lib)

    chk = selection_check(lib)
    print("selection: %d kept functions; defined once %d, static/inlined %d, missing %d, "
          "defined twice %d; excluded but defined %d; global symbols %d, duplicated %d" % (
              chk["kept"], chk["kept"] - chk["static"] - len(chk["missing"]) - len(chk["dup_kept"]),
              chk["static"], len(chk["missing"]), len(chk["dup_kept"]), len(chk["excluded_defined"]),
              chk["nglobal"], len(chk["dups"])))
    for f, s in chk["missing"][:20]:
        print("  MISSING %s (%s)" % (f, s))
    for f, v in chk["dup_kept"][:20]:
        print("  TWICE %s %s" % (f, v))
    for f in chk["excluded_defined"][:20]:
        print("  EXCLUDED-DEFINED %s" % f)
    for s, v in list(chk["dups"].items())[:20]:
        print("  DUP %s %s" % (s, v))

    autosyms = set()
    for line in open(ROOT / "undefined_syms_auto.txt"):
        m = re.match(r"\s*(\w+)\s*=", line)
        if m:
            autosyms.add(m.group(1))
    import glob
    import json
    bindnames = set()
    for jf in glob.glob(str(ROOT / "scripts/match-bindings-*.json")):
        bindnames.update(json.load(open(jf)).get("renames", {}))
    # the host libc the port's own code uses (port/port_libc.h declares it for builds without
    # 32-bit libc headers), plus what gcc emits calls to
    host_libc = set(re.findall(r"\b(\w+)\s*\(", open(ROOT / "port/port_libc.h").read().split(
        "PORT_LIBC_DECLS_ONLY", 2)[-1])) | {"stdout", "stderr", "memcpy", "memmove", "memset",
                                             "strcmp", "sqrtf", "sqrt", "fabsf"}
    kernel_funcs = {r[0] for r in read_tsv(ROOT / "port/gen/excluded.tsv") if r[1] == "kernel"}
    undecomp = {r[0] for r in read_tsv(ROOT / "port/gen/excluded.tsv") if "include_asm" in r[-1]}

    with tempfile.TemporaryDirectory(dir="/tmp/claude-1000") as tmp:
        und, refs, weak = undefined(lib, tmp, args.extra + args.extra_objects, load_aliases(args.aliases) if args.aliases else ())
    groups = collections.defaultdict(list)
    for s in und:
        if s in weak:
            g = "weak: optional hooks (resolve to 0 when absent; game-side PORT work defines them)"
        elif not refs[s]["call"] + refs[s]["data"]:
            g = "unreferenced: named only by sections --gc-sections drops (no link error)"
        elif s in undecomp:
            g = "modules not decompiled yet (INCLUDE_ASM; defined once their C lands)"
        elif s in host_libc:
            g = "host libc / libm"
        else:
            g = classify(s, refs[s], autosyms, kernel_funcs, bindnames)
        groups[g].append(s)

    def how(s):
        k = refs[s]
        return ("both" if k["call"] and k["data"] else "call" if k["call"] else "data"), k["call"] + k["data"]

    order = sorted(groups, key=lambda g: (g.startswith("other"), g))
    with open(args.out, "w") as fh:
        fh.write("# generated by scripts/port-undefined.py -- symbols libgame.a needs from outside\n")
        fh.write("# (the port/ultra, port/uso, port/fs shims, host libc, and the extracted data).\n")
        fh.write("# %d symbols.  Columns: symbol, referenced as call / data / both, relocations.\n" % len(und))
        for g in order:
            fh.write("\n## %s: %d\n" % (g, len(groups[g])))
            for s in sorted(groups[g]):
                h, n = how(s)
                fh.write("%s\t%s\t%d\n" % (s, h, n))
    print("undefined: %d symbols -> %s" % (len(und), os.path.relpath(args.out, ROOT)))
    for g in order:
        c = collections.Counter(how(s)[0] for s in groups[g])
        print("  %5d  %s  (%s)" % (len(groups[g]), g, ", ".join("%s %d" % kv for kv in sorted(c.items()))))
    bad = chk["missing"] or chk["dup_kept"] or chk["excluded_defined"] or chk["dups"]
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
