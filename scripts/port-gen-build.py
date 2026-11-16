#!/usr/bin/env python3
"""PC port M1 build skeleton generator (docs/1080_PC_PORT_PLAN.md section 13).

Reads port/sourcemap.tsv + port/sourcemap-placeholders.tsv and writes:

  port/gen/sources.cmake   the C / C++ files the port compiles, with the
                           per-file include dir and forced-include header
  port/gen/tu/<tu>.h       one header per translation unit that holds a body
                           the port must NOT use (placeholder, helper copy,
                           excluded libultra / libc copy) or a shipped name
                           that the source does not spell (C++ renames).
                           It is passed with `-include` and contains:
                             #pragma weak SYM        for every dropped body, so
                                                     gcc never inlines / IPA-
                                                     propagates the placeholder
                             /* port-drop: SYM */    read by port/tools/cc-select.py
                             /* port-alias: NEW TARGET */  shipped name -> gcc
                                                     assembler name
                             /* port-noinit: TARGET */  drop TARGET from .init_array
                                                     (static initialisers run
                                                     when the module "loads")
                             /* port-local: SYM */   make SYM file-local (the
                                                     asm-processor stubs of the
                                                     match_imports context headers)
  port/gen/excluded.tsv    mapped functions the port does not compile
                           (asm, kernel, game_libs libultra / libc copies:
                           plan decision 6; the shim provides them)

Nothing in src/ is touched.  The selection itself happens at the assembly
level in port/tools/cc-select.py (the CMake compiler launcher): each dropped
definition is renamed to a local `__port_dead_SYM`, so every reference in the
unit binds to the one shipped body, wherever it is compiled.

Usage: scripts/port-gen-build.py [--check]
  --check  also report how the 30 cfront-mangled C++ renames resolved
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
GEN = ROOT / "port" / "gen"

# game_libs carries a statically linked libultra + libc (READABILITY_PLAN
# 0x69E04 / 0x6BAD4-0x75264).  Plan decision 6: the shim replaces libultra; the
# libc copies (_Printf uses IDO va_arg) are replaced by host libc wrappers.
# gu (guMtxF2L, guOrtho, guPosition, fsin / fcos, sqrtf) is plain math and is
# KEPT: "the game's own guMtxF2L-style conversions run unchanged" (plan 4.3).
LIBULTRA_RE = re.compile(
    r"^(gl_)?("
    r"_*os[A-Z_]|__os|osVi|osAi|osPi|osSp|osCont|osPfs|osMotor|"
    r"__ll_|__ull_|_VirtualToPhysicalTask|_MakeMotorData|"
    r"corrupted|viMgrMain|rmonPrintf|"
    r"__osCont|__osPack|__osSum|__osCheck|__osGet|__osId|__osRepair|__osPfs"
    r")")
LIBC_RE = re.compile(
    r"^(gl_)?(_?Printf|_?Putfld|_?Litob|_?Genld|_?Ldtob|xldtob_stub_\w+|proutSprintf|sprintf|"
    r"strchr|strchr_tail_\w+|strlen|memcpy|bcopy|bzero|ldiv|lldiv)$")


def read_tsv(path):
    rows = []
    for line in path.read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("function\t"):
            continue
        rows.append(line.split("\t"))
    return rows


def exclusion(func, seg, kind):
    if kind == "asm":
        return "asm (GLOBAL_ASM; libultra .s / handwritten)"
    if seg == "kernel":
        return "kernel (libultra core, USO loader, ROM fs, rmon: port/ultra, port/uso, port/fs)"
    if seg == "game_libs" and LIBULTRA_RE.match(func):
        return "game_libs libultra copy (port/ultra shim)"
    if seg == "game_libs" and LIBC_RE.match(func):
        return "game_libs libc copy (host libc wrappers)"
    return None


def tu_name(src):
    return src.replace("/", "__")


CFRONT_RE = re.compile(r"^(?P<meth>__ct|__dt|__sti|[A-Za-z_]\w*?)__(?P<len>\d+)(?P<rest>.*)$")


def cfront_split(old):
    """cfront `meth__<n>ClassF...` -> (class, method); ctor -> (class, class)."""
    if old.startswith("__sti__"):
        return None, "__sti__"
    m = CFRONT_RE.match(old)
    if not m:
        return None, None
    n = int(m.group("len"))
    cls = m.group("rest")[:n]
    meth = m.group("meth")
    if meth == "__ct":
        meth = cls
    elif meth == "__dt":
        meth = "~" + cls
    return cls, meth


def load_nodecl():
    """port/gen/decls_skip.tsv: C++ unit -> CALL callees it declares itself (no generated decl)."""
    nodecl = {}
    skip_tsv = GEN / "decls_skip.tsv"
    if skip_tsv.exists():
        for line in open(skip_tsv):
            if line.strip() and not line.startswith("#"):
                unit, names = line.rstrip("\n").split("\t")
                nodecl[unit] = names.split()
    return nodecl


def gcc_symbols(src, flags):
    """Defined text symbols of src compiled with g++: [(asmname, demangled)]."""
    with tempfile.TemporaryDirectory(dir="/tmp/claude-1000") as td:
        obj = os.path.join(td, "x.o")
        decls = []
        if (GEN / "decls_cpp.h").exists():
            decls = ["-DPORT_NODECL_%s" % n for n in load_nodecl().get(src, ())]
            decls += ["-include", str(GEN / "decls_cpp.h")]
        cmd = ["g++"] + flags + decls + ["-I", os.path.dirname(src), "-c", "-o", obj, src]
        p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=100)
        if p.returncode:
            raise SystemExit("g++ failed on %s:\n%s" % (src, p.stderr[-2000:]))
        out = subprocess.run(["nm", "--defined-only", obj], capture_output=True, text=True).stdout
    syms = [l.split()[2] for l in out.splitlines() if len(l.split()) == 3 and l.split()[1] in "Tt"]
    dem = subprocess.run(["c++filt"], input="\n".join(syms), capture_output=True, text=True).stdout.splitlines()
    return list(zip(syms, dem))


def resolve_rename(src, old, flags):
    """gcc assembler name of the body cfront called `old` in src."""
    cls, meth = cfront_split(old)
    syms = gcc_symbols(src, flags)
    if meth == "__sti__":
        cands = [s for s, d in syms if s.startswith("_GLOBAL__sub_I_")]
        return cands, True
    if meth is None:
        # a plain extern "C" name (e.g. intern)
        return [s for s, d in syms if s == old], False
    want = "%s::%s(" % (cls, meth)
    cands = [s for s, d in syms if d.startswith(want)]
    if cls == meth and len(cands) > 1:
        # complete-object (C1) and base-object (C2) ctors: C1 is what `new` /
        # a declaration calls; gcc makes it an alias of C2.
        c1 = [s for s in cands if re.search(r"C1E", s)]
        cands = c1 or cands
    return cands, False


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--map", default="port/sourcemap.tsv")
    ap.add_argument("--placeholders", default="port/sourcemap-placeholders.tsv")
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()

    rows = read_tsv(ROOT / args.map)
    phs = read_tsv(ROOT / args.placeholders)

    excluded = {}
    keep = collections.defaultdict(set)
    for r in rows:
        func, seg, src, kind, how, host = r[:6]
        why = exclusion(func, seg, kind)
        if why:
            excluded[func] = (seg, src, why)
        else:
            keep[src].add(func)
    compiled = sorted(keep)

    drop = collections.defaultdict(dict)  # src -> {sym: reason}
    for r in rows:
        func, seg, src = r[:3]
        if func in excluded and src in keep:
            drop[src][func] = "excluded: " + excluded[func][2]
    for func, host, body, shipped in phs:
        if host in keep and body in ("c", "extra"):
            if func in keep[host]:
                raise SystemExit("conflict: %s both kept and a placeholder in %s" % (func, host))
            drop[host].setdefault(func, "%s copy; shipped body in %s" % (
                "placeholder" if body == "c" else "helper", shipped))

    # C++ renames (cfront-mangled / plain) and static initialisers.
    cxx_flags = ["-m32", "-std=gnu++98", "-fpermissive", "-fno-exceptions", "-fno-rtti",
                 "-malign-double", "-DPORT", "-D", "GLOBAL_ASM(x)=", "-w",
                 "-I", "port/include", "-I", "include", "-I", "src"]
    aliases = collections.defaultdict(list)  # src -> [(new, target)]
    noinit = collections.defaultdict(list)
    report = []
    for r in rows:
        func, seg, src, kind, how, host = r[:6]
        note = r[6] if len(r) > 6 else ""
        m = re.match(r"rename (?:mangled )?(\S+)->(\S+)$", note)
        if not m or func in excluded:
            continue
        old = m.group(1)
        if not src.endswith(".cpp"):
            raise SystemExit("unexpected C rename kept: %s %s" % (func, note))
        cands, sti = resolve_rename(src, old, cxx_flags)
        if len(cands) != 1:
            raise SystemExit("rename %s -> %s in %s: %d candidates %s" % (old, func, src, len(cands), cands))
        aliases[src].append((func, cands[0]))
        if sti:
            noinit[src].append(cands[0])
        report.append((src, old, cands[0], func))

    # match_imports context headers carry asm-processor `_asmpp_large_func*`
    # stubs: drop them everywhere they are compiled.
    asmpp = collections.defaultdict(set)
    for src in compiled:
        if src.startswith("src/match_imports/"):
            p = subprocess.run(["cpp", "-P", "-DPORT", "-D", "GLOBAL_ASM(x)=", "-Iport/include", "-Iinclude",
                                "-Isrc", "-I" + os.path.dirname(src), src],
                               cwd=ROOT, capture_output=True, text=True, timeout=100)
            for name in set(re.findall(r"\b(_asmpp_\w+)\s*\(", p.stdout)):
                asmpp[src].add(name)

    # C units: CALL macros call f with the placeholder token's type (include/uso_call.h under
    # PORT), so the token must be declared at the call.  IDO-era code often relies on an
    # implicit or block-scope `int tok()` that is out of scope at a later call: give such units
    # a file-scope K&R `int tok();` -- only when every declaration / definition of tok the unit
    # sees is exactly that (anything else would conflict; those units declare it themselves).
    kr_decls = collections.defaultdict(list)
    call_tok = re.compile(r"\b(?:(CALL)|(FCALL)|(GCALL))\(\s*\w+\s*\)|\bXCALL\(\s*(\w+)\s*,")
    for src in compiled:
        if not src.endswith(".c"):
            continue
        text = (ROOT / src).read_text(errors="replace")
        toks = set()
        for m in call_tok.finditer(text):
            toks.add("gl_func_00000000" if m.group(1) else "func_00000000" if m.group(2)
                     else "game_uso_func_00000000" if m.group(3) else m.group(4))
        if not toks:
            continue
        p = subprocess.run(["cpp", "-P", "-DPORT_GEN_SCAN", "-D", "GLOBAL_ASM(x)=", "-Iport/include", "-Iinclude",
                            "-Isrc", "-I" + os.path.dirname(src), src],
                           cwd=ROOT, capture_output=True, text=True, timeout=100)
        pre = p.stdout
        for tok in sorted(toks):
            ok = True
            for m in re.finditer(r"(?:^|[;{}])\s*([A-Za-z_][\w\s\*]*?)\b%s\s*(\(\s*\)\s*;)?"
                                 % re.escape(tok), pre, re.M):
                ret = " ".join(m.group(1).split())
                if ret in ("return", "else", "do") or re.match(r"^(if|while|for|switch|case|goto)\b", ret):
                    continue
                if ret not in ("int", "extern int") or m.group(2) is None:
                    ok = False
                    break
            if ok:
                kr_decls[src].append(tok)

    # placeholder calls without a CALL name, bound per function from the ROM relocations
    # (scripts/port-gen-binds.py -> port/gen/binds.tsv; plan section 14)
    binds = collections.defaultdict(list)
    if (GEN / "binds.tsv").exists():
        for line in open(GEN / "binds.tsv"):
            if line.strip() and not line.startswith("#"):
                unit, fn, tok, tgt, _ = line.rstrip("\n").split("\t")
                if unit in compiled:
                    binds[unit].append((fn, tok, tgt))

    # ---- write ----
    tudir = GEN / "tu"
    tudir.mkdir(parents=True, exist_ok=True)
    for old in tudir.glob("*.h"):
        old.unlink()
    headers = {}
    for src in compiled:
        if not (drop[src] or aliases[src] or asmpp[src] or kr_decls[src] or binds[src]):
            continue
        name = tu_name(src) + ".h"
        lines = ["/* generated by scripts/port-gen-build.py from port/sourcemap.tsv -- do not edit */",
                 "/* %s: %d shipped bodies kept, %d bodies dropped, %d aliases */" % (
                     src, len(keep[src]), len(drop[src]), len(aliases[src]))]
        for sym in sorted(drop[src]):
            lines.append("#pragma weak %s" % sym)
            lines.append("/* port-drop: %s */ /* %s */" % (sym, drop[src][sym]))
        for new, target in aliases[src]:
            lines.append("/* port-alias: %s %s */" % (new, target))
        for target in noinit[src]:
            lines.append("/* port-noinit: %s */" % target)
        for sym in sorted(asmpp[src]):
            lines.append("/* port-local: %s */ /* asm-processor stub from the context header */" % sym)
        for fn, tok, tgt in binds[src]:
            lines.append("/* port-bind: %s %s %s */" % (fn, tok, tgt))
        for tok in kr_decls[src]:
            lines.append("int %s(); /* CALL placeholder token: file-scope K&R declaration */" % tok)
        (tudir / name).write_text("\n".join(lines) + "\n")
        headers[src] = name

    with open(GEN / "sources.cmake", "w") as fh:
        fh.write("# generated by scripts/port-gen-build.py from port/sourcemap.tsv -- do not edit\n")
        fh.write("# %d translation units (%d C, %d C++), %d with a port/gen/tu header\n" % (
            len(compiled), sum(1 for s in compiled if s.endswith(".c")),
            sum(1 for s in compiled if s.endswith(".cpp")), len(headers)))
        fh.write("set(PORT_GAME_SOURCES\n")
        for src in compiled:
            fh.write("  ${GAME_ROOT}/%s\n" % src)
        fh.write(")\n")
        # C++ units: the C-linkage prototypes of their CALL callees (scripts/port-gen-decls.py,
        # section 14), minus the ones the unit declares itself.
        nodecl = load_nodecl()
        for src in compiled:
            opts = "-I${GAME_ROOT}/%s" % os.path.dirname(src)
            if src in headers:
                opts += ";-include;${PORT_GEN}/tu/%s" % headers[src]
            if src.endswith(".cpp") and (GEN / "decls_cpp.h").exists():
                opts += "".join(";-DPORT_NODECL_%s" % n for n in nodecl.get(src, ()))
                opts += ";-include;${PORT_GEN}/decls_cpp.h"
            fh.write('set_source_files_properties(${GAME_ROOT}/%s PROPERTIES COMPILE_OPTIONS "%s")\n' % (src, opts))

    with open(GEN / "excluded.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-build.py -- mapped functions the port does not compile\n")
        fh.write("function\tsegment\tsource\treason\n")
        for func in sorted(excluded, key=lambda f: (excluded[f][0], excluded[f][1], f)):
            seg, src, why = excluded[func]
            fh.write("%s\t%s\t%s\t%s\n" % (func, seg, src, why))

    kept = sum(len(v) for v in keep.values())
    print("mapped functions: %d; kept %d in %d files (C %d, C++ %d); excluded %d" % (
        len(rows), kept, len(compiled), sum(1 for s in compiled if s.endswith(".c")),
        sum(1 for s in compiled if s.endswith(".cpp")), len(excluded)))
    for why, c in collections.Counter(v[2] for v in excluded.values()).most_common():
        print("  excluded %4d  %s" % (c, why))
    print("tu headers: %d; local asm-processor stubs %d; drops %d (%s); aliases %d (static initialisers %d)" % (
        len(headers), sum(len(v) for v in asmpp.values()),
        sum(len(v) for v in drop.values()),
        ", ".join("%s=%d" % kv for kv in collections.Counter(
            v.split(":")[0].split(" ")[0] for d in drop.values() for v in d.values()).most_common()),
        sum(len(v) for v in aliases.values()), sum(len(v) for v in noinit.values())))
    if args.check:
        for src, old, target, new in report:
            print("  rename %-40s %-34s -> %s" % (old, target, new))
    return 0


if __name__ == "__main__":
    sys.exit(main())
