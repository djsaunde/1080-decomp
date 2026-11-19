#!/usr/bin/env python3
"""Compiler launcher of the wasm32 build (plan section 30): cc-select.py's job on LLVM IR.

    cc-wasm.py <clang> <args...> -c <src> -o <obj>

The i386 / x32 / mingw builds apply the per-unit directives of port/gen/tu/<tu>.h (port-drop, port-dropdata,
port-alias, port-noinit, port-local, port-bind) to the compiler's assembly, and port/gen/aliases.ld at link time.
wasm-ld has no linker scripts and the game is linked with full LTO (so that the WebAssembly backend's
FixFunctionBitcasts pass adapts the K&R / placeholder-typed direct calls, which wasm otherwise traps on), so here
the unit is compiled to unoptimised IR (-O0 without optnone: nothing inlined yet, every body is still the function
the directive names), the directives and the aliases are applied to the IR text, and the IR is compiled to an LTO
bitcode object with the real optimisation flags.

Addends (`port-bind: FUNC TOK+N TGT`, `TOK = BASE+0xK;`): the byte offset of a reference is computed from its
constant expression (getelementptr over the wasm32 data layout, `add (ptrtoint @TOK, N)`), as the relocation
addend gcc would have written. A reference with no bound addend falls back to the nearest bound one below it
(within 0x100), as cc-select does; anything else is left alone and listed when CC_WASM_VERBOSE is set.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ALIASES_LD = os.path.join(HERE, "..", "gen", "aliases.ld")
DIRECTIVE_RE = re.compile(r"/\*\s*port-(dropdata|drop|alias|noinit|local|bind):\s*([^*]*?)\s*\*/")
NAME = r"[-a-zA-Z$._][-a-zA-Z$._0-9]*"
VERBOSE = os.environ.get("CC_WASM_VERBOSE")


def log(msg):
    if VERBOSE:
        sys.stderr.write("cc-wasm: %s\n" % msg)


# ---------------------------------------------------------------- directives (as cc-select.py)

def parse_header(path):
    drop, alias, noinit, local, dropdata, binds = set(), [], set(), set(), set(), {}
    with open(path) as fh:
        for line in fh:
            for kind, arg in DIRECTIVE_RE.findall(line):
                parts = arg.split()
                if kind == "drop":
                    drop.add(parts[0])
                elif kind == "dropdata":
                    dropdata.add(parts[0])
                elif kind == "local":
                    local.add(parts[0])
                elif kind == "bind":
                    m = re.fullmatch(r"(\w+)([+-]\d+)", parts[1])
                    if m:
                        binds.setdefault(parts[0], {}).setdefault(m.group(1), {})[int(m.group(2))] = parts[2]
                    else:
                        binds.setdefault(parts[0], {})[parts[1]] = parts[2]
                elif kind == "alias":
                    alias.append((parts[0], parts[1]))
                    for also in parts[2:]:
                        alias.append((parts[0], also))
                else:
                    noinit.add(parts[0])
    for new, target in alias:
        if new in binds:
            binds.setdefault(target, {}).update(binds[new])
    return drop, alias, noinit, local, dropdata, binds


def parse_aliases(path):
    out = {}
    if not os.path.exists(path):
        return out
    text = re.sub(r"/\*.*?\*/", "", open(path).read(), flags=re.S)
    for a, b in re.findall(r"([\w.$]+)\s*=\s*([^;]+);", text):
        out[a] = b.strip()
    return out


def split_target(t):
    """'port_x_data+0x1C' -> ('port_x_data', 0x1C)"""
    m = re.fullmatch(r"\s*([\w.$]+)\s*(?:([+-])\s*(0x[0-9a-fA-F]+|\d+))?\s*", t)
    if not m:
        raise SystemExit("cc-wasm: cannot parse target %r" % t)
    off = int(m.group(3), 0) if m.group(3) else 0
    return m.group(1), (-off if m.group(2) == "-" else off)


# ---------------------------------------------------------------- IR types and the wasm32 layout

class Types:
    def __init__(self, text):
        self.named = {}
        for m in re.finditer(r"^(%" + NAME + r"|%\"[^\"]*\") = type (.*)$", text, re.M):
            self.named[m.group(1)] = m.group(2).strip()
        self.cache = {}

    def parse(self, s, i=0):
        """parse one type at s[i:], return (type tree, next index)"""
        while s[i] == " ":
            i += 1
        if s.startswith("ptr", i) and not re.match(r"[\w]", s[i + 3:i + 4] or " "):
            i += 3
            if s.startswith(" addrspace(", i):
                i = s.index(")", i) + 1
            return ("ptr",), i
        m = re.match(r"i(\d+)", s[i:])
        if m:
            return ("int", int(m.group(1))), i + len(m.group(0))
        for f, n in (("half", 2), ("bfloat", 2), ("float", 4), ("double", 8), ("fp128", 16), ("x86_fp80", 16)):
            if s.startswith(f, i):
                return ("fp", n), i + len(f)
        if s[i] == "[" or s[i] == "<" and s[i + 1] != "{":
            close = "]" if s[i] == "[" else ">"
            m = re.match(r"[\[<]\s*(\d+) x ", s[i:])
            n = int(m.group(1))
            et, j = self.parse(s, i + len(m.group(0)))
            while s[j] == " ":
                j += 1
            assert s[j] == close, s[i:j + 5]
            return ("arr" if close == "]" else "vec", n, et), j + 1
        if s[i] == "{" or s.startswith("<{", i):
            packed = s[i] == "<"
            j = i + (2 if packed else 1)
            fields = []
            while True:
                while s[j] == " ":
                    j += 1
                if s[j] == "}":
                    j += 1
                    break
                t, j = self.parse(s, j)
                fields.append(t)
                while s[j] == " ":
                    j += 1
                if s[j] == ",":
                    j += 1
            if packed:
                assert s[j] == ">"
                j += 1
            return ("struct", packed, fields), j
        if s[i] == "%":
            m = re.match(r"%(" + NAME + r"|\"[^\"]*\")", s[i:])
            return ("named", "%" + m.group(1)), i + len(m.group(0))
        raise SystemExit("cc-wasm: cannot parse type at %r" % s[i:i + 40])

    def resolve(self, t):
        if t[0] == "named":
            body = self.named[t[1]]
            if body == "opaque":
                return ("struct", False, [])
            return self.resolve(self.parse(body)[0])
        return t

    def layout(self, t):
        """(size, align, field offsets) of a type"""
        t = self.resolve(t)
        key = repr(t)
        if key in self.cache:
            return self.cache[key]
        k = t[0]
        if k == "ptr":
            r = (4, 4, None)
        elif k == "int":
            n = max(1, (t[1] + 7) // 8)
            n = 1 << (n - 1).bit_length()
            r = (n, min(n, 8) if n <= 8 else 16, None)
        elif k == "fp":
            r = (t[1], t[1], None)
        elif k in ("arr", "vec"):
            es, ea, _ = self.layout(t[2])
            r = (es * t[1], ea, None)
        else:
            off, al, offs = 0, 1, []
            for f in t[2]:
                fs, fa, _ = self.layout(f)
                if t[1]:
                    fa = 1
                off = (off + fa - 1) // fa * fa
                offs.append(off)
                off += fs
                al = max(al, fa)
            off = (off + al - 1) // al * al
            r = (off, al, offs)
        self.cache[key] = r
        return r

    def gep_offset(self, src, idxs):
        size = self.layout(src)[0]
        off = idxs[0] * size
        t = self.resolve(src)
        for ix in idxs[1:]:
            if t[0] in ("arr", "vec"):
                off += ix * self.layout(t[2])[0]
                t = self.resolve(t[2])
            elif t[0] == "struct":
                off += self.layout(t)[2][ix]
                t = self.resolve(t[2][ix])
            else:
                raise SystemExit("cc-wasm: gep into %r" % (t,))
        return off


def matching_paren(s, i):
    depth = 0
    for j in range(i, len(s)):
        c = s[j]
        if c in "([{<" and not (c == "<" and s[j + 1:j + 2] not in "{"):
            depth += 1
        elif c in ")]}" or (c == ">" and j > 0 and s[j - 1] == "}"):
            depth -= 1
            if depth == 0:
                return j
    raise SystemExit("cc-wasm: unbalanced expression %r" % s[i:i + 80])


def enclosing_open(s, i):
    """index of the '(' that encloses position i (constant-expression nesting), or -1"""
    depth = 0
    j = i - 1
    while j >= 0:
        c = s[j]
        if c == ")":
            depth += 1
        elif c == "(":
            if depth == 0:
                return j
            depth -= 1
        j -= 1
    return -1


GEP_KW = re.compile(r"getelementptr(?: (?:inbounds|nuw|nusw|inrange\([^)]*\)))*\s*$")


def ref_offset(types, line, i):
    """the byte addend of the reference to a global at line[i] (its '@'): the constant expression around it"""
    o = enclosing_open(line, i)
    if o < 0:
        return 0
    head = line[:o]
    if GEP_KW.search(head):
        c = matching_paren(line, o)
        body = line[o + 1:c]
        src, j = types.parse(body, 0)
        rest = body[j:]
        # ", ptr @TOK, i32 a, i32 b ..."
        m = re.match(r"\s*,\s*ptr\s+(@" + NAME + r"|@\"[^\"]*\")((?:\s*,\s*(?:inrange\s+)?i\d+\s+-?\d+)*)\s*$", rest)
        if not m:
            return None
        idxs = [int(x) for x in re.findall(r"i\d+\s+(-?\d+)", m.group(2))]
        off = types.gep_offset(src, idxs) if idxs else 0
        # nested: this expression may itself be the base of an outer constant GEP (gcc's addend is the sum)
        start = GEP_KW.search(head).start()
        while True:
            o2 = enclosing_open(line, start)
            if o2 < 0 or not GEP_KW.search(line[:o2]):
                return off
            c2 = matching_paren(line, o2)
            body2 = line[o2 + 1:c2]
            src2, j2 = types.parse(body2, 0)
            inner = line[start:c + 1]
            m2 = re.match(r"\s*,\s*ptr\s+", body2[j2:])
            if not m2 or not body2[j2 + m2.end():].startswith(inner):
                return off
            tail2 = body2[j2 + m2.end() + len(inner):]
            if not re.fullmatch(r"((?:\s*,\s*(?:inrange\s+)?i\d+\s+-?\d+)*)\s*", tail2):
                return off
            idx2 = [int(x) for x in re.findall(r"i\d+\s+(-?\d+)", tail2)]
            off += types.gep_offset(src2, idx2) if idx2 else 0
            start, c = GEP_KW.search(line[:o2]).start(), c2
    if re.search(r"ptrtoint\s*$", head):
        o2 = enclosing_open(line, o)
        if o2 >= 0 and re.search(r"\b(add)(?: nuw| nsw)*\s*$", line[:o2]):
            c2 = matching_paren(line, o2)
            m = re.search(r",\s*i\d+\s+(-?\d+)\s*$", line[o2 + 1:c2])
            if m:
                return int(m.group(1))
        return 0
    return 0


# ---------------------------------------------------------------- the module text

def sym_re(name):
    if re.fullmatch(NAME, name):
        return re.compile(r"@" + re.escape(name) + r"(?![-a-zA-Z$._0-9])")
    return re.compile(r"@\"" + re.escape(name) + r"\"")


def irname(name):
    return "@" + name if re.fullmatch(NAME, name) else '@"%s"' % name


LINKAGE = r"(?:(?:private|internal|available_externally|linkonce|weak|common|appending|extern_weak|linkonce_odr|" \
          r"weak_odr|external|dso_local|dso_preemptable|hidden|protected|default|local_unnamed_addr|unnamed_addr)\s+)*"


class Module:
    def __init__(self, text):
        self.lines = text.split("\n")
        self.types = Types(text)
        self.defined_fn = {}   # name -> line index of `define`
        self.declared_fn = set()
        self.globals = {}      # name -> line index (definition or external)
        self.aliases = {}      # name -> aliasee name
        for i, ln in enumerate(self.lines):
            if ln.startswith("define "):
                m = re.search(r"@(" + NAME + r"|\"[^\"]*\")\(", ln)
                self.defined_fn[m.group(1).strip('"')] = i
            elif ln.startswith("declare "):
                m = re.search(r"@(" + NAME + r"|\"[^\"]*\")\(", ln)
                self.declared_fn.add(m.group(1).strip('"'))
            elif ln.startswith("@"):
                m = re.match(r"@(" + NAME + r"|\"[^\"]*\") = (.*)$", ln)
                if m:
                    name = m.group(1).strip('"')
                    am = re.search(r"\balias\b.*,\s*ptr\s+@(" + NAME + r"|\"[^\"]*\")\s*$", m.group(2))
                    if am and re.match(LINKAGE + r"alias\b", m.group(2)):
                        self.aliases[name] = am.group(1).strip('"')
                    else:
                        self.globals[name] = i
        self.extra = []

    def is_function(self, name):
        return name in self.defined_fn or name in self.declared_fn

    def known(self, name):
        return name in self.defined_fn or name in self.declared_fn or name in self.globals or name in self.aliases

    def body_range(self, name):
        i = self.defined_fn.get(name)
        if i is None:
            return None
        j = i
        while self.lines[j] != "}":
            j += 1
        return i, j

    def declare_like(self, target, as_function):
        if self.known(target) or any(e[0] == target for e in self.extra):
            return
        if as_function:
            self.extra.append((target, "declare void %s()" % irname(target)))
        else:
            self.extra.append((target, "%s = external global [0 x i8], align 1" % irname(target)))

    def make_internal(self, name):
        if name in self.defined_fn:
            i = self.defined_fn[name]
            self.lines[i] = re.sub(r"^define\s+" + LINKAGE, "define internal ", self.lines[i])
        elif name in self.globals:
            i = self.globals[name]
            ln = self.lines[i]
            m = re.match(r"(@(?:" + NAME + r"|\"[^\"]*\") = )" + LINKAGE + r"(.*)$", ln)
            if m and not m.group(2).startswith("external "):
                rest = m.group(2)
                rest = re.sub(r"^(thread_local(?:\([^)]*\))?\s+)?", lambda x: x.group(0), rest)
                self.lines[i] = m.group(1) + "internal " + rest

    def rename_def(self, name, new):
        """rename the definition of name to new (internal); references keep name (now external)"""
        if name in self.defined_fn:
            i = self.defined_fn[name]
            self.lines[i] = sym_re(name).sub(irname(new), self.lines[i], count=1)
            self.lines[i] = re.sub(r"^define\s+" + LINKAGE, "define internal ", self.lines[i])
            self.defined_fn[new] = self.defined_fn.pop(name)
            self.extra.append((name, "declare void %s(...)" % irname(name)))
            return True
        if name in self.globals:
            i = self.globals[name]
            ln = self.lines[i]
            m = re.match(r"(@(?:" + NAME + r"|\"[^\"]*\")) = " + LINKAGE + r"(.*)$", ln)
            if m.group(2).startswith("external ") or m.group(2).startswith("global ") and " external " in ln[:40]:
                return False
            if re.match(r"(@\S+) = (?:dso_local |hidden )*external ", ln):
                return False
            self.lines[i] = irname(new) + " = internal " + m.group(2)
            self.globals[new] = self.globals.pop(name)
            self.extra.append((name, "%s = external global [0 x i8], align 1" % irname(name)))
            return True
        return False

    def text(self):
        out = "\n".join(self.lines)
        if self.extra:
            out += "\n" + "\n".join(e[1] for e in self.extra) + "\n"
        return out


def substitute(mod, lo, hi, tok, resolve, as_function):
    """in lines[lo:hi], replace references to tok: resolve(addend) -> (base, off) or None"""
    rx = sym_re(tok)
    changed = 0
    for k in range(lo, hi):
        ln = mod.lines[k]
        if tok not in ln:
            continue
        # not the definition / declaration line of tok itself
        if re.match(r"(define|declare)\b", ln) and re.search(r"@\"?" + re.escape(tok) + r"\"?\(", ln):
            continue
        if re.match(r"@\"?" + re.escape(tok) + r"\"? = ", ln):
            continue
        pos = 0
        out = []
        for m in rx.finditer(ln):
            n = ref_offset(mod.types, ln, m.start())
            r = resolve(n)
            if r is None:
                log("%s: no target for %s%+d" % (lo, tok, n if n is not None else 0))
                continue
            base, off = r
            mod.declare_like(base, as_function)
            rep = irname(base) if off == 0 else "getelementptr (i8, ptr %s, i32 %d)" % (irname(base), off)
            out.append(ln[pos:m.start()])
            out.append(rep)
            pos = m.end()
            changed += 1
        if out:
            out.append(ln[pos:])
            mod.lines[k] = "".join(out)
    return changed


def by_addend(tgt):
    def resolve(n):
        if n is None:
            return None
        if n in tgt:
            return split_target(tgt[n])
        lower = [k for k in tgt if k <= n and n - k < 0x100]
        if lower:
            log("addend %d: nearest bound %d" % (n, max(lower)))
            return split_target(tgt[max(lower)])
        return None
    return resolve


def apply_directives(mod, drop, alias, noinit, local, dropdata, binds, phase):
    """phase 'pre' (before the function-local -O1): drops, locals, noinit, aliases; 'post': the binds"""
    # a bound label that is an alias (C1 -> C2 ctors) binds inside its aliasee
    for a, b in mod.aliases.items():
        if a in binds:
            binds.setdefault(b, {}).update(binds[a])
    # gcc names a unit's static initialiser _GLOBAL__sub_I_<first global>; clang _GLOBAL__sub_I_<file>
    sub_i = [n for n in mod.defined_fn if n.startswith("_GLOBAL__sub_I_")]

    def gname(n):
        if n.startswith("_GLOBAL__sub_I_") and n not in mod.defined_fn and len(sub_i) == 1:
            return sub_i[0]
        return n
    alias = [(a, gname(t)) for a, t in alias]
    for k in list(binds):
        if gname(k) != k:
            binds.setdefault(gname(k), {}).update(binds[k])
    noinit = {gname(n) for n in noinit}
    if phase == "pre":
        return apply_structure(mod, drop, alias, noinit, local, dropdata)
    # binds: inside each bound function's body
    for fn, tb in binds.items():
        rng = mod.body_range(fn)
        if rng is None:
            continue
        for tok, tgt in tb.items():
            as_fn = mod.is_function(tok)
            if isinstance(tgt, dict):
                substitute(mod, rng[0] + 1, rng[1], tok, by_addend(tgt), as_fn)
            else:
                base, off = split_target(tgt)
                substitute(mod, rng[0] + 1, rng[1], tok, lambda n, b=base, o=off: (b, o), as_fn)
    return None


def apply_structure(mod, drop, alias, noinit, local, dropdata):
    found = set()
    for sym in drop | dropdata:
        if mod.rename_def(sym, "__port_dead_" + sym):
            found.add(sym)
    local_targets = {t for _, t in alias if t.startswith("_Z")} | set(local)
    for sym in local_targets:
        mod.make_internal(sym)
    if noinit:
        for k, ln in enumerate(mod.lines):
            if ln.startswith("@llvm.global_ctors = "):
                ents = re.findall(r"\{ i32, ptr, ptr \} \{ i32 \d+, ptr @(" + NAME + r"|\"[^\"]*\"), ptr [^}]*\}", ln)
                keep = re.findall(r"(\{ i32, ptr, ptr \} \{ i32 \d+, ptr @(?:" + NAME + r"|\"[^\"]*\"), ptr [^}]*\})", ln)
                kept = [e for e, n in zip(keep, ents) if n.strip('"') not in noinit]
                if not kept:
                    mod.lines[k] = ""
                else:
                    mod.lines[k] = "@llvm.global_ctors = appending global [%d x { i32, ptr, ptr }] [%s]" % (
                        len(kept), ", ".join(kept))
    done = set()
    for new, target in alias:
        if new in done:
            continue
        done.add(new)
        if not mod.known(target):
            raise SystemExit("cc-wasm: alias %s -> %s: no %s in the unit" % (new, target, target))
        if new in mod.declared_fn:
            mod.lines = [ln for ln in mod.lines if not (ln.startswith("declare ") and
                                                        re.search(r"@\"?" + re.escape(new) + r"\"?\(", ln))]
        mod.extra.append((new, "%s = alias i8, ptr %s" % (irname(new), irname(target))))
    return found


def apply_aliases(mod, aliases):
    """port/gen/aliases.ld: every remaining reference to an aliased token, anywhere in the unit"""
    if not aliases:
        return
    text = "\n".join(mod.lines)
    present = set(re.findall(r"@(" + NAME + r")", text))
    for tok in present & set(aliases):
        if tok in mod.defined_fn or (tok in mod.globals and not re.search(
                r"= (?:dso_local |hidden )*external ", mod.lines[mod.globals[tok]])):
            continue
        base, off = split_target(aliases[tok])
        substitute(mod, 0, len(mod.lines), tok, lambda n, b=base, o=off: (b, o), mod.is_function(tok))


def no_common(text):
    # -fcommon tentative definitions (several match_imports units define gl_storage_66B64_threadRegion): wasm has no
    # common symbols, so they become weak definitions (the copies are the same size)
    return re.sub(r"^(@(?:" + NAME + r"|\"[^\"]*\") = )common ", r"\1weak ", text, flags=re.M)


def strip_noinline(text):
    # -O0 marks every function noinline; the binds are applied, so LTO may inline now
    return re.sub(r"^(attributes #\d+ = \{.*?)\bnoinline\b ?", r"\1", text, flags=re.M)


NEW_RE = re.compile(r"\boperator\s+new\s*(\[\s*\])?\s*\(\s*unsigned(?:\s+int)?\b")


def cxx_inputs(cmd, obj):
    """a C++ donor: decls_cpp.h -> decls_cpp_wasm.h, and `operator new(unsigned)` -> size_t (wasm32's
    size_t is unsigned long; the donors spell i386's unsigned int)"""
    if "-include" in cmd:
        for i, a in enumerate(cmd):
            if a.endswith("/port/gen/decls_cpp.h") and cmd[i - 1] == "-include":
                gen = os.path.join(os.path.dirname(os.path.abspath(obj)), "decls_cpp_wasm.h")
                gen = os.environ.get("CC_WASM_DECLS", gen)
                if not os.path.exists(gen) or os.path.getmtime(gen) < os.path.getmtime(a):
                    subprocess.run([sys.executable, os.path.join(HERE, "gen-decls-wasm.py"), a, gen], check=True)
                cmd[i] = gen
                cmd[i:i] = []
                cmd.insert(i - 1, "-I" + HERE)
                break
    srcs = [k for k, a in enumerate(cmd) if re.search(r"\.(cpp|cc|cxx)$", a) and not a.startswith("-")]
    for k in srcs:
        text = open(cmd[k]).read()
        if NEW_RE.search(text):
            tmp = obj + ".src.cpp"
            with open(tmp, "w") as fh:
                fh.write('#line 1 "%s"\n' % cmd[k] + NEW_RE.sub(lambda m: "operator new%s(__SIZE_TYPE__" % (m.group(1) or ""), text))
            cmd[k:k + 1] = ["-iquote", os.path.dirname(os.path.abspath(cmd[k])), tmp]
            break
    return cmd


def main():
    argv = sys.argv[1:]
    hdr = None
    for i, a in enumerate(argv):
        if a == "-include" and i + 1 < len(argv) and "/port/gen/tu/" in argv[i + 1]:
            hdr = argv[i + 1]
    if "-c" not in argv or "-o" not in argv or "-flto" not in " ".join(argv):
        os.execvp(argv[0], argv)
    oi = argv.index("-o")
    obj = argv[oi + 1]
    ll = obj + ".ll"
    cmd = []
    skip = False
    for k, a in enumerate(argv):
        if skip:
            skip = False
            continue
        if re.fullmatch(r"-O[0-3sz]|-flto(=.*)?", a):
            continue
        if a in ("-MD", "-MMD"):
            continue
        if a in ("-MT", "-MF"):
            skip = True
            continue
        cmd.append(a)
    cmd[cmd.index("-c")] = "-S"
    cmd[cmd.index("-o") + 1] = ll
    cmd += ["-emit-llvm", "-O0", "-Xclang", "-disable-O0-optnone"]
    # dependency file for the build system: from the first compile
    if "-MF" in argv:
        cmd += ["-MD", "-MT", argv[argv.index("-MT") + 1] if "-MT" in argv else obj, "-MF",
                argv[argv.index("-MF") + 1]]
    cmd = cxx_inputs(cmd, obj)
    p = subprocess.run(cmd)
    if p.returncode:
        return p.returncode
    tgt = [a for a in argv if a.startswith("--target=") or a.startswith("-target")]
    if any(re.search(r"\.(cpp|cc|cxx)$", a) for a in argv if not a.startswith("-")):
        # a C++ donor: g++ -O2 inlined the donor's inline members (and the static-init helpers) into the ROM
        # function whose binds name their tokens (NCC inlined them too), so inline them here, before the binds
        with open(ll) as fh:
            text = strip_noinline(fh.read())
        text = re.sub(r"^(define (?:linkonce_odr|internal)\b[^\n]*\)[^\n{]*?)(#\d+ )?((?:comdat )?\{)$",
                      lambda m: m.group(1) + " alwaysinline " + (m.group(2) or "") + m.group(3), text, flags=re.M)
        with open(ll, "w") as fh:
            fh.write(text)
        p = subprocess.run([argv[0]] + tgt + ["-O0", "-Xclang", "-disable-O0-optnone", "-Wno-override-module", "-S",
                            "-emit-llvm", ll, "-o", ll])
        if p.returncode:
            return p.returncode
    # The structural directives first (drops, locals, the module initialisers, the aliases): the -O1 below must
    # see a dropped placeholder copy as the external declaration it becomes, or it retypes the unit's calls to
    # it after the placeholder's own (a `void f(void)` copy: the result of `x = CALL(f)(..)` becomes poison).
    # The initialisers a module runs when it loads (port-noinit, plan decision 4) leave llvm.global_ctors here,
    # so GlobalOpt cannot evaluate them into the data.
    with open(ll) as fh:
        mod = Module(fh.read())
    if hdr:
        apply_directives(mod, *parse_header(hdr), phase="pre")
    with open(ll, "w") as fh:
        fh.write(mod.text())
    # gcc writes each data reference as one relocation, symbol + the whole constant offset (`&ctx->initPool` is
    # D_00000000+8584), and the per-addend binds are keyed on that; unoptimised IR keeps the base in a local
    # and adds the field later. A function-local -O1 (no inlining: the binds are per function) folds every
    # such address back into one constant expression, as gcc did.
    p = subprocess.run([argv[0]] + tgt + ["-O1", "-fno-inline", "-Wno-override-module", "-S", "-emit-llvm", ll, "-o",
                        ll])
    if p.returncode:
        return p.returncode
    with open(ll) as fh:
        mod = Module(fh.read())
    if hdr:
        apply_directives(mod, *parse_header(hdr), phase="post")
    apply_aliases(mod, parse_aliases(ALIASES_LD))
    with open(ll, "w") as fh:
        fh.write(no_common(strip_noinline(mod.text())))
    opt = [os.environ["WASM_OPT_LEVEL"]] if os.environ.get("WASM_OPT_LEVEL") else ([a for a in argv if re.fullmatch(r"-O[0-3sz]", a)] or ["-O2"])
    lto = [a for a in argv if re.fullmatch(r"-flto(=.*)?", a)]
    p = subprocess.run([argv[0]] + tgt + opt + lto + ["-Wno-override-module", "-c", ll, "-o", obj])
    return p.returncode


if __name__ == "__main__":
    sys.exit(main())
