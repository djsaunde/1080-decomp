#!/usr/bin/env python3
"""PC port M0 compile census (docs/1080_PC_PORT_PLAN.md sections 4.1 / 5 M0).

Compiles every C / C++ source file that port/sourcemap.tsv selects with the
host compiler, compile-only, for a 32-bit host:

  C    gcc -m32 -c -std=gnu89 -DPORT -Iport/include -Iinclude -Isrc -I<dir>
  C++  g++ -m32 -c -std=gnu++98 -fpermissive -fno-exceptions -fno-rtti -DPORT ...

into a scratch directory (never into build/), then sorts the diagnostics into
categories and, for the objects that do compile, counts global symbols that
are defined in more than one object (the "multiple definitions" a link would
hit: placeholder bodies, shared statics in headers, copies of the same helper).

Nothing in src/ is modified.  Hand-written assembly rows (kind=asm) are not
compiled; the port replaces them (libultra shim) or re-implements them.

Usage: scripts/port-compile-census.py [--scratch DIR] [-j N] [--std gnu89|gnu99]
                                      [--json OUT] [--examples N]
"""
import argparse
import collections
import concurrent.futures
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

DIAG_RE = re.compile(r"^(?P<file>[^:\n]+):(?P<line>\d+):(?:(?P<col>\d+):)?\s*"
                     r"(?P<sev>fatal error|error|warning):\s*(?P<msg>.*)$")

# Ordered: the first matching category wins.
CATEGORIES = [
    ("asm-processor / GLOBAL_ASM / pragma",
     lambda m, src: re.search(r"^\s*(?:#\s*pragma\s+)?(?:GLOBAL_ASM|INCLUDE_ASM|INCLUDE_RODATA)\b|\basm\s*\(|__asm", src, re.M)
     or re.search(r"\basm\b|pragma|GLOBAL_ASM", m)),
    ("struct layout asserts (i386 4-byte u64/f64 alignment)",
     lambda m, src: re.search(r"size of array '[^']*assert[^']*' is negative", m.replace("\u2018", "'").replace("\u2019", "'"))),
    ("missing headers",
     lambda m, src: "No such file or directory" in m),
    ("int / pointer",
     lambda m, src: re.search(r"pointer from integer|integer from pointer|invalid conversion from '[^']*' to "
                              r"'[^']*\*'|invalid conversion from '[^']*\*' to '(?:int|unsigned|long|short|char|u?s(?:8|16|32))|"
                              r"cast from '[^']*\*' to '[^']*' loses precision|int-conversion|"
                              r"incompatible types when (?:assigning|initializing|returning)|"
                              r"invalid operands to binary|lvalue required|cannot convert", m)),
    ("IDO-specific syntax",
     lambda m, src: re.search(r"\bva_arg\b|__builtin_classof", src)
     or re.search(r"too (?:few|many) arguments to function|__unaligned|\b__builtin_alignof\b|#\s*ident|\bpragma\b|"
                              r"expected (?:identifier|declaration specifiers|'[^']*') before|"
                              r"unknown type name|storage class specified|"
                              r"(?:conflicting|incompatible) types for|redefinition of|"
                              r"redeclared as different kind|static declaration of .* follows non-static|"
                              r"variable-sized object|initializer element is not constant|"
                              r"array type has incomplete element type|has incomplete type", m)),
]
CPP_CATEGORY = "C++ issues"
OTHER = "other"


def categorise(msg, srcline, is_cpp):
    for name, pred in CATEGORIES:
        if pred(msg, srcline or ""):
            return name
    return CPP_CATEGORY if is_cpp else OTHER


def read_map(path):
    rows = []
    for line in path.read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("function\t"):
            continue
        rows.append(line.split("\t"))
    return rows


def compile_one(src, scratch, std, extra=()):
    is_cpp = src.endswith(".cpp")
    out = scratch / (src.replace("/", "__") + ".o")
    if is_cpp:
        cmd = ["g++", "-m32", "-c", "-std=gnu++98", "-fpermissive", "-fno-exceptions", "-fno-rtti"]
    else:
        cmd = ["gcc", "-m32", "-c", "-std=" + std]
    cmd += list(extra) + ["-DPORT", "-fdiagnostics-plain-output",
            "-fmax-errors=0", "-I", "port/include", "-I", "include", "-I", "src",
            "-I", os.path.dirname(src), "-o", str(out), src]
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=100)
    return src, p.returncode, p.stderr, (str(out) if p.returncode == 0 else None)


def line_of(path, n, cache={}):
    if path not in cache:
        try:
            cache[path] = (ROOT / path).read_text(errors="replace").splitlines()
        except OSError:
            cache[path] = []
    lines = cache[path]
    return lines[n - 1] if 0 < n <= len(lines) else ""


def defined_globals(obj):
    p = subprocess.run(["nm", "-g", "--defined-only", obj], capture_output=True, text=True)
    out = []
    for line in p.stdout.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "TDBRCVW" and parts[1] not in "CVW":
            out.append((parts[2], parts[1]))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--map", default="port/sourcemap.tsv")
    ap.add_argument("--placeholders", default="port/sourcemap-placeholders.tsv")
    ap.add_argument("--scratch", default="/tmp/claude-1000/port-census")
    ap.add_argument("-j", type=int, default=8)
    ap.add_argument("--std", default="gnu89")
    ap.add_argument("--json")
    ap.add_argument("--examples", type=int, default=3)
    ap.add_argument("--extra", default="", help="extra compiler flags, e.g. -malign-double")
    args = ap.parse_args()

    rows = read_map(ROOT / args.map)
    files = sorted({r[2] for r in rows if r[3] in ("c", "cpp")})
    asm_files = sorted({r[2] for r in rows if r[3] == "asm"})
    scratch = Path(args.scratch)
    scratch.mkdir(parents=True, exist_ok=True)

    results = {}
    with concurrent.futures.ThreadPoolExecutor(args.j) as ex:
        for src, rc, err, obj in ex.map(lambda f: compile_one(f, scratch, args.std, args.extra.split()), files):
            results[src] = (rc, err, obj)

    cat_counts = collections.Counter()
    cat_files = collections.defaultdict(set)
    cat_examples = collections.defaultdict(list)
    msg_counts = collections.Counter()
    warn_counts = collections.Counter()
    file_err = collections.Counter()
    ok_files, bad_files = [], []
    for src in files:
        rc, err, obj = results[src]
        (ok_files if rc == 0 else bad_files).append(src)
        for line in err.splitlines():
            m = DIAG_RE.match(line)
            if not m:
                continue
            msg = m.group("msg")
            if m.group("sev") == "warning":
                wm = re.search(r"\[-W([\w=-]+)\]$", msg)
                warn_counts[wm.group(1) if wm else re.sub(r"\u2018[^\u2019]*\u2019|'[^']*'", "X", msg)[:60]] += 1
                continue
            ln = int(m.group("line"))
            srcline = "\n".join(line_of(m.group("file"), k) for k in range(max(1, ln - 8), ln + 1))
            cat = categorise(msg, srcline, src.endswith(".cpp"))
            if cat == OTHER and src in cat_files[CATEGORIES[0][0]]:
                cat = CATEGORIES[0][0]  # cascade of an earlier bare GLOBAL_ASM(...) in the same file
            cat_counts[cat] += 1
            cat_files[cat].add(src)
            file_err[src] += 1
            key = re.sub(r"'[^']*'", "'X'", msg)
            msg_counts[(cat, key)] += 1
            if len(cat_examples[cat]) < args.examples:
                cat_examples[cat].append("%s:%s: %s" % (m.group("file"), m.group("line"), msg[:160]))

    # Multiple definitions across the objects that compiled.
    defs = collections.defaultdict(list)
    for src in ok_files:
        for sym, t in defined_globals(results[src][2]):
            if not sym.startswith("__x86.get_pc_thunk"):  # comdat, deduplicated by the linker
                defs[sym].append(src)
    multi = {s: v for s, v in defs.items() if len(v) > 1}
    shipped = {r[0] for r in rows}
    placeholder_hosts = collections.defaultdict(set)
    ph = ROOT / args.placeholders
    if ph.exists():
        for r in read_map(ph):
            placeholder_hosts[r[0]].add(r[1])
    multi_placeholder = sum(1 for s in multi if s in placeholder_hosts)
    multi_shipped = sum(1 for s in multi if s in shipped)
    cat_counts_multi = len(multi)

    n_c = sum(1 for f in files if f.endswith(".c"))
    n_cpp = len(files) - n_c
    print("M0 compile census (%s, C as -std=%s, C++ as gnu++98 -fpermissive, extra flags: %s)" % (
        subprocess.run(["gcc", "-dumpfullversion"], capture_output=True, text=True).stdout.strip() or "gcc",
        args.std, args.extra or "none"))
    print("files: %d (C %d, C++ %d); asm files not compiled: %d" % (len(files), n_c, n_cpp, len(asm_files)))
    print("  OK      %4d (C %d, C++ %d)" % (len(ok_files), sum(1 for f in ok_files if f.endswith(".c")),
                                           sum(1 for f in ok_files if f.endswith(".cpp"))))
    print("  failing %4d (C %d, C++ %d)" % (len(bad_files), sum(1 for f in bad_files if f.endswith(".c")),
                                           sum(1 for f in bad_files if f.endswith(".cpp"))))
    print()
    print("| category | errors | files | example |")
    print("|---|---:|---:|---|")
    for cat in [c[0] for c in CATEGORIES] + [CPP_CATEGORY, OTHER]:
        ex = cat_examples[cat][0] if cat_examples[cat] else ""
        print("| %s | %d | %d | %s |" % (cat, cat_counts[cat], len(cat_files[cat]), ex.replace("|", "\\|")))
    print("| multiple definitions across files (compiled objects) | %d symbols | %d files | %s |" % (
        cat_counts_multi, len({f for v in multi.values() for f in v}),
        (lambda kv: "%s in %s" % (kv[0], ", ".join(kv[1][:3])) if kv else "")(
            next(iter(sorted(multi.items(), key=lambda kv: -len(kv[1]))), None))))
    print("  of which shipped functions: %d (listed in sourcemap-placeholders.tsv: %d)" % (multi_shipped, multi_placeholder))
    kinds = collections.Counter()
    for sym, v in multi.items():
        kinds["asm-processor stub (_asmpp_*) in match_imports context headers" if sym.startswith("_asmpp_")
              else "shipped function; extra copy listed in sourcemap-placeholders.tsv" if sym in placeholder_hosts
              else "shipped function, other copy" if sym in shipped else
              "data / non-report symbol"] += 1
    for k, c in kinds.most_common():
        print("    %-50s %d" % (k, c))
    print("  examples: " + "; ".join("%s (%s)" % (sym, ", ".join(v)) for sym, v in
                                     sorted(multi.items(), key=lambda kv: (kv[0].startswith("_asmpp_"), kv[0] in shipped, kv[0]))[:4]))
    print()
    print("top error messages:")
    for (cat, key), c in msg_counts.most_common(25):
        print("  %5d  [%s] %s" % (c, cat, key[:150]))
    print()
    print("examples:")
    for cat in cat_examples:
        for e in cat_examples[cat]:
            print("  [%s] %s" % (cat, e))
    print()
    print("files with most errors:")
    for f, c in file_err.most_common(12):
        print("  %5d  %s" % (c, f))
    print()
    print("top warnings (not errors; int/pointer ones matter for 64-bit):")
    for w, c in warn_counts.most_common(12):
        print("  %6d  %s" % (c, w))

    if args.json:
        Path(args.json).write_text(json.dumps({
            "ok": ok_files, "failing": bad_files,
            "categories": {c: {"errors": cat_counts[c], "files": sorted(cat_files[c]),
                               "examples": cat_examples[c]} for c in cat_counts},
            "multiple_definitions": multi,
            "warnings": warn_counts,
        }, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
