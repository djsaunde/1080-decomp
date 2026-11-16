#!/usr/bin/env python3
"""PC port source map (docs/1080_PC_PORT_PLAN.md section 4.1, milestone M0).

For each of the functions in report.json, decide which source body is the
SHIPPED code, i.e. the body whose matching-build bytes end up in the ROM:

  plain   the C definition in the unit's own .c file
  donor   a REPLACE_FUNC_BODY donor (.cpp NCC donor, or an -O0 / -O1 / IDO 5.3 /
          -O2 split .c donor) spliced into the host object by
          scripts/replace-function-body.py
  import  a src/match_imports/ native port (bound .elf spliced the same way)
  asm     hand-written assembly assembled into the host via GLOBAL_ASM
          (src/libultra/*.s, src/handwritten/*.s, asm/nonmatchings/*_pad.s)

The Makefile is parsed statically (global `VAR := value` definitions plus the
target-specific REPLACE_FUNC_BODY / POST_COMPILE assignments on the matching
build/src/... targets).  Each source file is preprocessed with the host cpp in
the matching configuration (no NON_MATCHING, no PORT, IDO-like predefines) and
scanned for top-level function definitions and GLOBAL_ASM includes.  Donor
chains are followed (a donor can itself receive REPLACE_FUNC_BODY splices).

Outputs (under port/):
  sourcemap.tsv               one row per function:
                              function, segment, source, kind, how, host, note
                              (note: "rename OLD->NEW" / "rename mangled OLD->NEW"
                              when the shipped name is an objcopy / POST_COMPILE
                              rename, "POST_COMPILE sub-symbol ...", "qualified ...")
  sourcemap-placeholders.tsv  host bodies the port must NOT compile, because
                              the shipped body comes from a donor / import:
                              function, host, host_body, shipped_source
                              host_body = c   (a C definition in the matching view)
                                        | nm  (only an #ifdef NON_MATCHING reference)
                                        | none (no body in the host at all)
                                        | extra (another selected file, e.g. a donor,
                                          also defines it: a helper copy)

Self-check: every report function appears exactly once and none is
unresolved; exits non-zero otherwise.

Usage: scripts/port-sourcemap.py [--out-dir port] [--quiet]
"""
import argparse
import collections
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# IDO-like predefines for the matching view of the source.
CPP_MATCHING = ["-undef", "-D__sgi", "-Dsgi", "-D_MIPSEB", "-DMIPSEB", "-D__mips=2",
                "-D_MIPS_SZINT=32", "-D_MIPS_SZLONG=32", "-D_MIPS_SZPTR=32",
                "-D_MIPS_SIM=1", "-D_ABIO32=1", "-D__STDC__=1"]


# --------------------------------------------------------------------------
# Makefile
# --------------------------------------------------------------------------

def read_makefile(path):
    lines, buf = [], ""
    for raw in path.read_text().splitlines():
        if raw.endswith("\\"):
            buf += raw[:-1] + " "
            continue
        lines.append(buf + raw)
        buf = ""
    if buf:
        lines.append(buf)
    return lines


VAR_RE = re.compile(r"^([A-Za-z_][A-Za-z0-9_]*)\s*(:=|::=|\?=|\+=|=)\s*(.*)$")
TSV_RE = re.compile(r"^([^\t:=#][^:=]*?):\s*(?:private\s+)?(?:override\s+)?"
                    r"([A-Z_]+)\s*(:=|\+=|=)\s*(.*)$")


def expand(s, gvars, depth=0):
    if depth > 20:
        return s

    def sub(m):
        name = m.group(1) or m.group(2)
        return expand(gvars.get(name, ""), gvars, depth + 1)
    return re.sub(r"\$\(([A-Za-z_][A-Za-z0-9_]*)\)|\$\{([A-Za-z_][A-Za-z0-9_]*)\}", sub, s)


def parse_makefile(path):
    gvars = {}
    tvars = collections.defaultdict(lambda: collections.defaultdict(list))  # target -> var -> [specs]
    pending = []  # (targets, var, op, value) expanded after globals are known
    recipes = []  # (targets, recipe line) for explicit rules
    cur_targets = []
    for line in read_makefile(path):
        if line.startswith("\t"):
            if cur_targets:
                recipes.append((cur_targets, line))
            continue
        if not line or line.lstrip().startswith("#"):
            continue
        cur_targets = []
        m = VAR_RE.match(line)
        if m and ":" not in m.group(1):
            name, op, val = m.groups()
            val = val.split(" #")[0].strip()
            if op == "+=":
                gvars[name] = (gvars.get(name, "") + " " + val).strip()
            elif op == "?=":
                gvars.setdefault(name, val)
            else:
                gvars[name] = val
            continue
        m = TSV_RE.match(line)
        if m:
            targets, var, op, val = m.groups()
            pending.append((targets.split(), var, op, val))
            continue
        m = re.match(r"^([^:=]+?):(?!=)", line)
        if m and "%" not in m.group(1):
            cur_targets = m.group(1).split()
    for targets, var, op, val in pending:
        for t in targets:
            t = expand(t, gvars)
            v = expand(val, gvars).split()
            if op == "+=":
                tvars[t][var].extend(v)
            else:
                tvars[t][var] = list(v)
    # objcopy --redefine-sym OLD=NEW in explicit recipes (C++ donors rename
    # their mangled cfront symbols to the shipped names).
    for targets, line in recipes:
        found = re.findall(r"--redefine-sym[ =]([^\s$]+?)=(\S+)", line)
        # `--redefine-sym $$(nm $@ | awk '$$3 ~ /^__sti__/ ...')=NEW`
        found += re.findall(r"--redefine-sym \$\$\(.*?[\^ ](__\w+?)/.*?\)=(\S+)", line)
        for old, new in found:
            for t in targets:
                tvars[expand(t, gvars)]["REDEFINE_SYM"].append(old + "=" + new)
    return gvars, tvars


def obj_to_source(obj):
    """build/src/X.c.o -> src/X.c ; *.bound.elf -> its src/match_imports/X.c."""
    o = obj
    for suf in (".linked.elf.bound.elf", ".linked.elf"):
        if o.endswith(suf):
            o = o[: -len(suf)]
    if o.startswith("build/non_matching/"):
        o = o[len("build/non_matching/"):]
    elif o.startswith("build/"):
        o = o[len("build/"):]
    if o.endswith(".o"):
        o = o[:-2]
    return o


def source_to_obj(src):
    return "build/" + src + ".o"


# --------------------------------------------------------------------------
# Source scanning
# --------------------------------------------------------------------------

TOK_RE = re.compile(r'''
    (?P<ws>\s+)
  | (?P<str>"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')
  | (?P<id>[A-Za-z_][A-Za-z0-9_]*)
  | (?P<num>\.?[0-9][A-Za-z0-9_.]*)
  | (?P<p>::|->|\+\+|--|<<|>>|[{}()\[\];,=*&<>.:~!?+\-/%^|#])
  | (?P<other>.)
''', re.X)

AGG = {"struct", "union", "enum", "class", "namespace"}


def cpp(src, extra=()):
    lang = ["-x", "c++"] if src.endswith(".cpp") else ["-x", "c"]
    cmd = ["gcc", "-E", "-P"] + lang + CPP_MATCHING + list(extra) + \
        ["-I", "include", "-I", "src", "-I", os.path.dirname(src), src]
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    return p.stdout, p.returncode, p.stderr


def scan_defs(text):
    """Return (set of defined function names, list of GLOBAL_ASM paths)."""
    defs = set()
    gasm = []
    for line in text.splitlines():
        m = re.match(r'\s*(?:#\s*pragma\s+)?GLOBAL_ASM\s*\(\s*"([^"]+)"', line)
        if m:
            gasm.append(m.group(1))
    # Drop pragma lines before tokenising.
    text = "\n".join(l for l in text.splitlines()
                     if not l.lstrip().startswith("#") and not re.match(r"\s*GLOBAL_ASM\s*\(", l))
    toks = [m.group(0) for m in TOK_RE.finditer(text) if m.lastgroup not in ("ws",)]
    depth = 0
    file_scope = 0        # depth that counts as file scope (raised by extern "C" {)
    file_scopes = []
    stmts = [[]]
    for t in toks:
        if depth == file_scope:
            if t == ";":
                stmts.append([])
                continue
            if t == "{":
                cur = stmts[-1]
                if len(cur) >= 2 and cur[-2] == "extern" and cur[-1].startswith('"'):
                    file_scopes.append(file_scope)
                    depth += 1
                    file_scope = depth
                    stmts = [[]]
                    continue
                name = decl_name(stmts)
                if name:
                    defs.add(name)
                depth += 1
                continue
            if t == "}":
                # closing an extern "C" block
                if file_scopes:
                    depth -= 1
                    file_scope = file_scopes.pop()
                stmts = [[]]
                continue
            stmts[-1].append(t)
            continue
        if t == "{":
            depth += 1
        elif t == "}":
            depth -= 1
            if depth == file_scope:
                stmts = [[]]
    return defs, gasm


def decl_name(stmts):
    cur = stmts[-1]
    if not cur or ")" not in cur:
        # K&R parameter declarations (`f(a, b) int a; int b; {`): walk back to
        # the declarator statement.
        for back in range(len(stmts) - 1, max(-1, len(stmts) - 12), -1):
            s = stmts[back]
            if ")" in s:
                cur = s
                break
        else:
            return None
    # aggregate definitions / initialisers
    pd = 0
    for k, t in enumerate(cur):
        if t == "(":
            pd += 1
        elif t == ")":
            pd -= 1
        elif pd == 0 and t == "=":
            return None
    if cur[0] in AGG or (len(cur) >= 2 and cur[-2] in AGG) or (cur[-1] in AGG):
        if not any(t == "(" for t in cur):
            return None
    cand = cur
    # strip trailing const/throw()/attributes after the parameter list
    while cand and cand[-1] not in (")",):
        if cand[-1] in ("const", "volatile"):
            cand = cand[:-1]
            continue
        if cand[-1] == ":" or cand[-1] == ",":
            return None
        # C++ ctor initialiser list etc. -- give up gracefully
        break
    if not cand or cand[-1] != ")":
        # ctor init lists: Foo::Foo(...) : a(1) -- find the last top-level ')'
        pass
    # first top-level '('
    pd = 0
    for k, t in enumerate(cand):
        if t == "(":
            if pd == 0:
                if k > 0 and re.match(r"[A-Za-z_]\w*$", cand[k - 1]) and cand[k - 1] not in (
                        "__attribute__", "__declspec", "sizeof"):
                    nm = cand[k - 1]
                    # C++ qualified names: A::f -> A::f
                    if k >= 3 and cand[k - 2] == "::":
                        nm = cand[k - 3] + "::" + nm
                    return nm
                if k + 2 < len(cand) and cand[k + 1] == "*" and re.match(r"[A-Za-z_]\w*$", cand[k + 2]):
                    return cand[k + 2]
            pd += 1
        elif t == ")":
            pd -= 1
    return None


ASM_SYM_RE = re.compile(r"^\s*(?:glabel|\.globl|\.global|\.ent|LEAF|XLEAF|NESTED|jlabel)\s*\(?\s*([A-Za-z_$][\w$.]*)")


def asm_syms(path):
    out = []
    p = ROOT / path
    if not p.exists():
        return out
    for line in p.read_text(errors="replace").splitlines():
        m = ASM_SYM_RE.match(line)
        if m:
            out.append(m.group(1))
    return out


def nm_reference_has(src, func):
    """Does the raw source contain a C body for func (e.g. only under NON_MATCHING)?"""
    p = ROOT / src
    if not p.exists():
        return False
    return re.search(r"^[^\n;#]*\b" + re.escape(func) + r"\s*\([^;{]*\)\s*\{", p.read_text(errors="replace"),
                     re.M) is not None


class Scanner:
    def __init__(self):
        self.cache = {}
        self.cpp_fail = {}

    def get(self, src):
        if src not in self.cache:
            if not (ROOT / src).exists():
                self.cache[src] = (set(), {}, False)
                return self.cache[src]
            text, rc, err = cpp(src)
            if rc != 0:
                self.cpp_fail[src] = err.strip().splitlines()[:3]
            defs, gasm = scan_defs(text)
            asmmap = {}
            for g in gasm:
                for s in asm_syms(g):
                    asmmap.setdefault(s, g)
            self.cache[src] = (defs, asmmap, True)
        return self.cache[src]


# --------------------------------------------------------------------------
# Resolution
# --------------------------------------------------------------------------

def post_compile_maps(tvars, target):
    """Symbol renames / added sub-symbols applied by POST_COMPILE on target."""
    renames, subsyms = {}, {}
    for spec in " ".join(tvars.get(target, {}).get("POST_COMPILE", [])).split(";"):
        toks = spec.split()
        if "scripts/rename-elf-symbol.py" in toks:
            for a in toks[toks.index("scripts/rename-elf-symbol.py") + 2:]:
                if "=" in a:
                    old, new = a.split("=", 1)
                    renames[new] = old
        if "scripts/add-elf-func-symbol.py" in toks:
            for a in toks[toks.index("scripts/add-elf-func-symbol.py") + 2:]:
                if "=" in a:
                    nm, rng = a.split("=", 1)
                    subsyms[nm] = rng
    for spec in tvars.get(target, {}).get("REDEFINE_SYM", []):
        old, new = spec.split("=", 1)
        renames[new] = old
    return renames, subsyms


def resolve(func, src, tvars, scan, chain, depth=0):
    """Return (final_source, kind, chain, note) for func as shipped from src."""
    target = source_to_obj(src)
    specs = tvars.get(target, {}).get("REPLACE_FUNC_BODY", [])
    donor = None
    for s in specs:
        fn, _, d = s.partition("=")
        if fn == func:
            donor = d  # the last spec for func wins (each splice overwrites)
    if donor and depth < 8:
        dsrc = obj_to_source(donor)
        return resolve(func, dsrc, tvars, scan, chain + [(src, dsrc)], depth + 1)
    defs, asmmap, ok = scan.get(src)
    kind_c = "cpp" if src.endswith(".cpp") else "c"
    if func in defs:
        return src, kind_c, chain, ""
    if func in asmmap:
        return asmmap[func], "asm", chain, "GLOBAL_ASM in " + src
    renames, subsyms = post_compile_maps(tvars, target)
    if func in renames:
        old = renames[func]
        if old in defs:
            return src, kind_c, chain, "rename %s->%s" % (old, func)
        if src.endswith(".cpp") and "__" in old:
            return src, kind_c, chain, "rename mangled %s->%s" % (old, func)
    if func in subsyms:
        return src, kind_c, chain, "POST_COMPILE sub-symbol %s of the unit body" % subsyms[func]
    # C++ qualified match (A::f exported extern "C" as f is the norm; keep lookup cheap)
    for d in defs:
        if d.endswith("::" + func):
            return src, kind_c, chain, "qualified " + d
    return src, "?", chain, "unresolved" + ("" if ok else " (file missing)")


def elf_func_bytes(path):
    """{symbol: bytes} for the defined symbols in .text of a big-endian ELF32 .o."""
    import struct
    data = (ROOT / path).read_bytes()
    if data[:4] != b"\x7fELF" or data[5] != 2:
        return {}
    shoff, = struct.unpack(">I", data[0x20:0x24])
    shentsize, shnum, shstrndx = struct.unpack(">HHH", data[0x2E:0x34])
    secs = [struct.unpack(">IIIIIIIIII", data[shoff + i * shentsize: shoff + i * shentsize + 40])
            for i in range(shnum)]
    out = {}
    for sh in secs:
        if sh[1] != 2:  # SHT_SYMTAB
            continue
        strtab = secs[sh[6]]
        for k in range(sh[5] // 16):
            nm, val, size, info, other, shndx = struct.unpack(">IIIBBH", data[sh[4] + k * 16: sh[4] + k * 16 + 16])
            if shndx == 0 or shndx >= len(secs):
                continue
            ts = secs[shndx]
            if ts[1] != 1 or not (ts[2] & 4):  # PROGBITS + EXECINSTR
                continue
            end = data.index(b"\0", strtab[4] + nm)
            name = data[strtab[4] + nm:end].decode("ascii", "replace")
            off = ts[4] + val - ts[3]  # sh_offset + (st_value - sh_addr); sh_addr is 0 in a .o
            if size == 0:
                # linked carriers can drop st_size: keep the rest of the section,
                # the comparison trims to the expected length.
                size = ts[3] + ts[5] - val
                name = name + "\0nosize"
            out.setdefault(name, bytes(data[off: off + size]))
    return out


def reloc_insensitive_equal(a, b):
    """Equal up to relocatable immediates: jal targets and 16-bit immediates
    (the expected/ USO objects hold link-time values the .o leaves 0)."""
    if len(a) != len(b):
        return False
    for k in range(0, len(a), 4):
        x, y = a[k:k + 4], b[k:k + 4]
        if x == y:
            continue
        if x[0] >> 2 == y[0] >> 2 == 3:      # jal
            continue
        if x[:2] == y[:2]:                     # same opcode/rs/rt, imm16 differs
            continue
        return False
    return True


def verify(rows):
    """Compare each function's bytes in the object built from its mapped source
    against expected/ (the ROM-derived reference objects)."""
    cache = {}

    def get(p):
        if p not in cache:
            cache[p] = elf_func_bytes(p) if (ROOT / p).exists() else None
        return cache[p]
    res = collections.Counter()
    bad = []
    for func, seg, src, kind, how, host, note in rows:
        exp = get("expected/" + host + ".o")
        if how == "import":
            obj = "build/" + src + ".o.linked.elf.bound.elf"
        elif kind == "asm":
            obj = "build/" + host + ".o"
        else:
            obj = "build/" + src + ".o"
        got = get(obj)
        if exp is None or got is None:
            res["missing-object"] += 1
            bad.append((func, obj, "object missing"))
            continue
        if func not in exp:
            res["not-in-expected"] += 1
            bad.append((func, "expected/" + host + ".o", "no such symbol in expected/ (not checked)"))
            continue
        if func not in got and func + "\0nosize" in got:
            got = dict(got)
            got[func] = got[func + "\0nosize"][:len(exp[func])]
        if func not in got:
            if "sub-symbol" in note:
                res["sub-symbol (not checked)"] += 1
            else:
                res["symbol-missing"] += 1
                bad.append((func, obj, "symbol missing"))
            continue
        if got[func] == exp[func]:
            res["identical"] += 1
        elif reloc_insensitive_equal(got[func], exp[func]):
            res["identical up to relocated immediates"] += 1
        elif (lambda a, b: reloc_insensitive_equal(a[:len(b)], b[:len(a)])
              and not (a[len(b):] + b[len(a):]).strip(b"\0"))(got[func], exp[func]):
            # symbol sizes differ only by zero padding words (nop tails,
            # SUFFIX_BYTES pads, alignment folded into st_size)
            res["identical up to zero padding"] += 1
        else:
            res["DIFFERENT"] += 1
            bad.append((func, obj, "bytes differ (%d vs %d)" % (len(got[func]), len(exp[func]))))
    return res, bad


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out-dir", default="port")
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument("--verify", action="store_true",
                    help="compare each mapped source's built object against expected/ (needs a full make)")
    args = ap.parse_args()

    report = json.loads((ROOT / "report.json").read_text())
    funcs = []
    for u in report["units"]:
        src = u["metadata"]["source_path"]
        seg = src.split("/")[1]
        for f in u.get("functions", []):
            funcs.append((f["name"], seg, src))

    _, tvars = parse_makefile(ROOT / "Makefile")
    scan = Scanner()
    rows, placeholders, problems = [], [], []
    for func, seg, host in funcs:
        final, kind, chain, note = resolve(func, host, tvars, scan, [])
        if kind == "?":
            problems.append((func, host, final, note))
        hops = [d for _, d in chain]
        if kind == "asm":
            how = "asm"
        elif any(h.startswith("src/match_imports/") for h in hops):
            how = "import"
        elif hops:
            how = "donor"
        else:
            how = "plain"
        rows.append((func, seg, final, kind, how, host, note))
        # every file in the chain except the last holds a placeholder body
        for frm, to in chain:
            defs, _, _ = scan.get(frm)
            body = "c" if func in defs else ("nm" if nm_reference_has(frm, func) else "none")
            placeholders.append((func, frm, body, final))

    # Extra bodies: a selected file (typically a donor) that also defines a
    # report function whose shipped body lives elsewhere (helpers the donor
    # carries for codegen context, libultra copies under their C names).
    mapped = {r[0]: r[2] for r in rows}
    listed = {(p[0], p[1]) for p in placeholders}
    for f in sorted({r[2] for r in rows if r[3] in ("c", "cpp")}):
        defs, _, _ = scan.get(f)
        for d in sorted(defs):
            if d in mapped and mapped[d] != f and (d, f) not in listed:
                placeholders.append((d, f, "extra", mapped[d]))
                listed.add((d, f))

    out = ROOT / args.out_dir
    out.mkdir(parents=True, exist_ok=True)
    with open(out / "sourcemap.tsv", "w") as fh:
        fh.write("# generated by scripts/port-sourcemap.py -- do not edit\n")
        fh.write("function\tsegment\tsource\tkind\thow\thost\tnote\n")
        for r in sorted(rows, key=lambda r: (r[5], r[0])):
            fh.write("\t".join(r[:7]) + "\n")
    with open(out / "sourcemap-placeholders.tsv", "w") as fh:
        fh.write("# generated by scripts/port-sourcemap.py -- host bodies the port must NOT compile\n")
        fh.write("function\thost\thost_body\tshipped_source\n")
        for r in sorted(placeholders, key=lambda r: (r[1], r[0])):
            fh.write("\t".join(r) + "\n")

    names = collections.Counter(r[0] for r in rows)
    dups = [n for n, c in names.items() if c > 1]
    total = len(rows)
    by = collections.Counter((r[3], r[4]) for r in rows)
    print("functions: %d (unique %d, duplicates %d, unresolved %d)" % (total, len(names), len(dups), len(problems)))
    for (k, h), c in sorted(by.items()):
        print("  kind=%-4s how=%-6s %5d" % (k, h, c))
    print("source files selected: %d" % len({r[2] for r in rows}))
    pb = collections.Counter(p[2] for p in placeholders)
    print("placeholders: %d (%s)" % (len(placeholders), ", ".join("%s=%d" % kv for kv in sorted(pb.items()))))
    notes = collections.Counter(r[6].split(" ")[0] + " " + (r[6].split(" ")[1] if " " in r[6] else "")
                                for r in rows if r[6] and r[3] != "?")
    if not args.quiet:
        for k, c in notes.most_common():
            print("  note: %-40s %d" % (k, c))
        for f, err in sorted(scan.cpp_fail.items()):
            print("  cpp warning: %s: %s" % (f, " | ".join(err)[:200]))
        for p in problems:
            print("  UNRESOLVED: %s host=%s last=%s %s" % p)
    ok = total == len(funcs) and not dups and not problems
    if args.verify:
        res, bad = verify(rows)
        print("verify vs expected/:", ", ".join("%s=%d" % kv for kv in sorted(res.items())))
        for b in bad[:40]:
            print("  VERIFY: %s %s: %s" % b)
    print("self-check:", "OK" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
