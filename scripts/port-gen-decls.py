#!/usr/bin/env python3
"""PC port: C-linkage prototypes for the C++ donors (docs/1080_PC_PORT_PLAN.md section 14).

    scripts/port-gen-decls.py <libgame.a>     # a built port library (any state with DWARF)

Under PORT, include/uso_call.h expands CALL(f) / FCALL / GCALL / XCALL(tok, f) to the real callee
f (plan decision 4).  The NCC .cpp donors build without -I include, so f is undeclared there.
This writes port/gen/decls_cpp.h: one `extern "C"` prototype per callee that a C++ unit names
in a CALL macro, taken from the DWARF of the library itself:

  * the callee's DEFINITION (a DW_TAG_subprogram with code) when the library has it; otherwise a
    DECLARATION seen in some C unit (the real headers: libultra shim functions etc.);
  * the ABI type of each parameter and of the return value: every pointer is `void *` (so a
    donor's mirror struct pointer converts implicitly), integers keep their width and signedness,
    float / double / long long are kept exactly (they change the i386 calling convention);
  * a K&R (unprototyped) C definition gets its promoted parameter types (float -> double,
    char / short -> int), which is what its body expects;
  * a variadic definition keeps its `...`.

A unit that declares or defines one of the names itself (any mention outside a CALL macro)
must not see the generated prototype (C++ rejects two different C-linkage declarations):
each such prototype is guarded by `#ifndef PORT_NODECL_<name>`, and port/gen/decls_skip.tsv
lists the units that get `-DPORT_NODECL_<name>` (scripts/port-gen-build.py adds it).
Names with no DWARF anywhere fall back to `int f(...)` and are listed as such.
"""
import argparse
import collections
import io
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GEN = ROOT / "port/gen"

try:
    from elftools.elf.elffile import ELFFile
except ImportError:  # re-exec under the repo venv
    import os
    venv = ROOT.parents[1] / ".venv/bin/python"
    os.execv(str(venv), [str(venv)] + sys.argv)

CALL_RE = re.compile(r"\b(?:CALL|FCALL|GCALL)\(\s*(\w+)\s*\)|\bXCALL\(\s*\w+\s*,\s*(\w+)\s*\)")


def ar_members(path):
    data = open(path, "rb").read()
    assert data[:8] == b"!<arch>\n"
    pos, longnames = 8, b""
    while pos < len(data):
        hdr = data[pos:pos + 60]
        name, size = hdr[:16].decode().strip(), int(hdr[48:58])
        body = data[pos + 60:pos + 60 + size]
        pos += 60 + size + (size & 1)
        if name == "//":
            longnames = body
            continue
        if name in ("/", "/SYM64/"):
            continue
        if name.startswith("/"):
            off = int(name[1:])
            name = longnames[off:longnames.index(b"/\n", off)].decode()
        yield name.rstrip("/"), body


def strip_cv(die):
    while die is not None and die.tag in ("DW_TAG_typedef", "DW_TAG_const_type", "DW_TAG_volatile_type",
                                          "DW_TAG_restrict_type"):
        die = die.get_DIE_from_attribute("DW_AT_type") if "DW_AT_type" in die.attributes else None
    return die


def type_of(die):
    """C type text for a DIE's DW_AT_type (None = void); ('struct', size) for a by-value record."""
    if "DW_AT_type" not in die.attributes:
        return None
    t = strip_cv(die.get_DIE_from_attribute("DW_AT_type"))
    if t is None:
        return None
    if t.tag in ("DW_TAG_pointer_type", "DW_TAG_reference_type", "DW_TAG_array_type",
                 "DW_TAG_ptr_to_member_type", "DW_TAG_subroutine_type"):
        return "void *"
    if t.tag == "DW_TAG_enumeration_type":
        return "int"
    if t.tag in ("DW_TAG_structure_type", "DW_TAG_union_type", "DW_TAG_class_type"):
        return ("struct", t.attributes["DW_AT_byte_size"].value if "DW_AT_byte_size" in t.attributes else 0)
    if t.tag == "DW_TAG_base_type":
        enc = t.attributes["DW_AT_encoding"].value
        size = t.attributes["DW_AT_byte_size"].value
        if enc == 4:  # float
            return {4: "float", 8: "double"}.get(size, "long double")
        unsigned = enc in (2, 7, 8)  # boolean, unsigned, unsigned_char
        base = {1: "char", 2: "short", 4: "int", 8: "long long"}[size]
        if size == 1 and not unsigned:
            base = "signed char"
        return ("unsigned " + base) if unsigned else base
    return "int"


def promote(t):
    if t == "float":
        return "double"
    if t in ("char", "signed char", "short", "unsigned char", "unsigned short"):
        return "int"
    return t


def origin(die):
    for a in ("DW_AT_abstract_origin", "DW_AT_specification"):
        if a in die.attributes:
            return die.get_DIE_from_attribute(a)
    return None


def attr(die, name):
    d = die
    while d is not None:
        if name in d.attributes:
            return d.attributes[name].value
        d = origin(d)
    return None


def signature(die):
    """(ret, [params], variadic, prototyped) of a subprogram DIE."""
    base = die
    while origin(base) is not None and "DW_AT_type" not in base.attributes and \
            not any(c.tag == "DW_TAG_formal_parameter" for c in base.iter_children()):
        base = origin(base)
    ret_die = die
    while "DW_AT_type" not in ret_die.attributes and origin(ret_die) is not None:
        ret_die = origin(ret_die)
    ret = type_of(ret_die)
    params, variadic = [], False
    src = die if any(c.tag in ("DW_TAG_formal_parameter", "DW_TAG_unspecified_parameters")
                     for c in die.iter_children()) else base
    for c in src.iter_children():
        if c.tag == "DW_TAG_formal_parameter":
            p = c
            while "DW_AT_type" not in p.attributes and origin(p) is not None:
                p = origin(p)
            if attr(c, "DW_AT_artificial"):
                continue
            params.append(type_of(p))
        elif c.tag == "DW_TAG_unspecified_parameters":
            variadic = True
    proto = bool(attr(die, "DW_AT_prototyped"))
    return ret, params, variadic, proto


def shipped_objects():
    """function -> archive member name of the unit that holds its shipped body."""
    m = {}
    for line in open(ROOT / "port/sourcemap.tsv"):
        r = line.rstrip("\n").split("\t")
        if len(r) > 3 and not line.startswith("#"):
            m[r[0]] = Path(r[2]).name + ".o"
    return m


def collect(lib, shipped=None):
    shipped = shipped or {}
    defs, decls = {}, {}
    for name, body in ar_members(lib):
        elf = ELFFile(io.BytesIO(body))
        if not elf.has_dwarf_info():
            continue
        cpp_unit = name.endswith(".cpp.o")
        dw = elf.get_dwarf_info()
        for cu in dw.iter_CUs():
            for die in cu.iter_DIEs():
                if die.tag != "DW_TAG_subprogram":
                    continue
                nm = attr(die, "DW_AT_name")
                if not nm:
                    continue
                nm = nm.decode() if isinstance(nm, bytes) else nm
                ln = attr(die, "DW_AT_linkage_name")
                if ln and (ln.decode() if isinstance(ln, bytes) else ln).startswith("_Z"):
                    continue
                try:
                    sig = signature(die)
                except Exception:  # noqa: BLE001 -- odd DWARF: skip this DIE
                    continue
                ext = bool(attr(die, "DW_AT_external"))
                if attr(die, "DW_AT_declaration"):
                    if not cpp_unit:
                        decls.setdefault(nm, (sig, name))
                    continue
                # a definition: concrete (code), or the abstract instance of an inlined one;
                # external ones win over a static of the same name
                # the shipped body's own unit wins over host placeholders (port-dropped copies)
                rank = (4 if shipped.get(nm) == name else 0) + (2 if ext else 0) + (1 if ("DW_AT_low_pc" in die.attributes or
                                                  "DW_AT_ranges" in die.attributes) else 0)
                if nm not in defs or rank > defs[nm][2]:
                    defs[nm] = (sig, name, rank)
    return defs, decls


# shim functions with no compilable C source (asm, or a source that itself has a CALL conflict):
# libultra / libc prototypes
ASM_PROTOS = {"sqrtf": "float sqrtf(float);",
              "gl_sprintf": "int gl_sprintf(void *, const void *, ...);",
              "gl_osSpTaskYield": "void gl_osSpTaskYield(void);"}
PORT_CFLAGS = ["-m32", "-malign-double", "-g", "-O0", "-std=gnu89", "-fcommon", "-w", "-DPORT",
               "-include", "port/include/port.h", "-Iport/include", "-Iinclude", "-Isrc"]


def excluded_defs(names):
    """DWARF definitions of excluded functions (the shim's surface), from their own sources
    compiled once with the port's C flags (the port does not link these objects)."""
    import subprocess
    import tempfile
    srcs = {}
    for line in open(ROOT / "port/sourcemap.tsv"):
        r = line.rstrip("\n").split("\t")
        if len(r) > 3 and r[0] in names and r[2].endswith(".c"):
            srcs.setdefault(r[2], set()).add(r[0])
    found = {}
    with tempfile.TemporaryDirectory() as td:
        objs = []
        for i, src in enumerate(sorted(srcs)):
            obj = "%s/%d.o" % (td, i)
            p = subprocess.run(["gcc"] + PORT_CFLAGS + ["-I" + str(Path(src).parent), "-c", "-o", obj, src],
                               cwd=ROOT, capture_output=True, text=True, timeout=100)
            if p.returncode == 0:
                objs.append(obj)
        if objs:
            ar = td + "/x.a"
            subprocess.run(["ar", "rc", ar] + objs, check=True, timeout=100)
            d, _ = collect(ar)
            found = {n: v for n, v in d.items() if n in names}
    return found


# callees whose C++ callers pass a 4-byte mirror struct by value where the C definition takes an
# int (same i386 stack slot): declared `(...)`, which passes the struct as is.  Only for callees
# without float / double / long long parameters (render() checks).
BYVALUE_CALLERS = {"ColourItemType_ctor"}

# callees whose trailing fixed parameters a C++ caller omits: the C definition declares a
# parameter only as a matching lever and overwrites it before any read, so the missing argument
# is never read.  Declared with a default value (0) for that many trailing parameters.
# gui_fontAdjustAdvance(int *, int, int): `a2 = a1` first (gui_uso.c); viewrec's 2-argument call.
OPTIONAL_TAIL = {"gui_fontAdjustAdvance": 1}


def cpp_units():
    """the C++ units the port compiles: the .cpp sources of port/sourcemap.tsv (not
    port/gen/sources.cmake, which port-gen-build.py writes only after this has run)"""
    srcs = set()
    for line in open(ROOT / "port/sourcemap.tsv"):
        f = line.rstrip("\n").split("\t")
        if len(f) > 3 and f[2].endswith(".cpp") and not line.startswith("#"):
            srcs.add(f[2])
    return sorted(srcs)


def render(name, sig):
    ret, params, variadic, proto = sig
    if not proto:
        params = [promote(p) for p in params]
    if isinstance(ret, tuple):
        return None
    if any(isinstance(p, tuple) for p in params) or name in BYVALUE_CALLERS:
        # a by-value struct: the donor passes its own mirror type.  i386 passes a struct through
        # `...` exactly as by value (copied to the stack), so the variadic form is ABI-exact as
        # long as no float parameter would be promoted.
        if any(p in ("float", "double", "long long", "unsigned long long") for p in params):
            return None
        return "%s %s(...);" % (ret or "int /*void*/", name)
    # every prototype ends in `...`: IDO-era callers sometimes pass more arguments than the
    # callee takes (a K&R placeholder call; extra MIPS argument registers are ignored).  On i386
    # cdecl the fixed parameters are laid out identically and the caller pops the extras, so the
    # tail is ABI-exact for the real parameters and absorbs the surplus.
    params = list(params)
    for i in range(len(params) - OPTIONAL_TAIL.get(name, 0), len(params)):
        params[i] += " = 0"
    ps = ", ".join(params)
    ps = (ps + ", ...") if ps else "..."
    # a void callee is declared int: IDO donors read the v0 a void call leaves behind
    # (`int r = CALL(list_append)(...)`), and on i386 that is just a read of eax.
    return "%s %s(%s);" % (ret or "int /*void*/", name, ps)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("lib")
    a = ap.parse_args()
    defs, decls = collect(a.lib, shipped_objects())

    want = collections.defaultdict(set)  # name -> units
    skip = collections.defaultdict(set)  # unit -> names
    rests = {}
    for src in cpp_units():
        text = (ROOT / src).read_text(errors="replace")
        for m in CALL_RE.finditer(text):
            want[m.group(1) or m.group(2)].add(src)
        # PORT branches call the real names directly (they are not declarations)
        nop = re.sub(r"^#ifdef PORT\n.*?^#(else|endif)", " ", text, flags=re.S | re.M)
        rest = CALL_RE.sub(" ", re.sub(r"/\*.*?\*/|//[^\n]*", " ", nop, flags=re.S))
        rests[src] = set(re.findall(r"\b[A-Za-z_]\w*\b", rest))
    for src, idents in rests.items():
        skip[src] = idents & set(want)
        if not skip[src]:
            del skip[src]

    guarded = {n for v in skip.values() for n in v}
    # an unprototyped declaration (K&R `int f();` in some unit) loses to the excluded body's DWARF
    defs.update(excluded_defs({n for n in want if n not in defs
                               and (n not in decls or not decls[n][0][3])}))
    out, stats, fallback, byval = [], collections.Counter(), [], []
    for n in sorted(want):
        if n in defs:
            sig, how = defs[n][0], "def"
        elif n in decls:
            sig, how = decls[n][0], "decl"
        else:
            sig, how = None, "none"
        line = render(n, sig) if sig else ASM_PROTOS.get(n)
        if n in ASM_PROTOS:
            how, line = "manual", ASM_PROTOS[n]
        if sig and line is None:
            byval.append(n)
            how = "byval"
        if line is None:
            fallback.append(n)
            line = "int %s(...);" % n
        stats[how] += 1
        if n in guarded:
            out.append("#ifndef PORT_NODECL_%s\n%s\n#endif" % (n, line))
        else:
            out.append(line)

    with open(GEN / "decls_cpp.h", "w") as fh:
        fh.write("/* generated by scripts/port-gen-decls.py from the port library's DWARF -- do not edit.\n"
                 " * C-linkage prototypes of the callees the C++ donors name in CALL macros\n"
                 " * (include/uso_call.h under PORT).  Pointers are void *; see the script. */\n"
                 "#ifndef PORT_GEN_DECLS_CPP_H\n#define PORT_GEN_DECLS_CPP_H\n"
                 "#ifdef __cplusplus\nextern \"C\" {\n#endif\n")
        fh.write("/* %d callees: %d from a definition, %d from a header declaration, %d by hand, %d fallback int f(...) */\n" % (
            len(want), stats["def"], stats["decl"], stats["manual"], len(fallback)))
        if fallback:
            fh.write("/* fallback (no DWARF%s): %s */\n" % (
                "; by-value struct: " + " ".join(byval) if byval else "", " ".join(fallback)))
        fh.write("\n".join(out) + "\n")
        fh.write("#ifdef __cplusplus\n}\n#endif\n#endif\n")
    with open(GEN / "decls_skip.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-decls.py: units that declare a CALL callee themselves\n")
        for src in sorted(skip):
            fh.write("%s\t%s\n" % (src, " ".join(sorted(skip[src]))))
    print("callees %d (def %d, decl %d, fallback %d: %s); units with own declarations %d" % (
        len(want), stats["def"], stats["decl"], len(fallback), " ".join(fallback), len(skip)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
