#!/usr/bin/env python3
"""The wasm32 build's link fixup (plan section 30): one symbol, one kind.

    wasm-fixtypes.py <clang> <stamp> <obj.o>...

An ELF symbol has no kind, and the decomp often names a function through a data declaration (a callback
address in a table: `extern char Fn[]; ... &Fn`), or calls a data token. A wasm object keeps functions
(table indices when their address is taken) and data (memory addresses) apart, and wasm-ld rejects a
symbol used both ways ("symbol type mismatch"). cc-wasm.py leaves each object's IR next to it (<obj>.ll);
this reads them all, finds every name that some unit defines as a function (or as data), and rewrites the
other units' declarations of it to the same kind, then recompiles those objects. With opaque pointers a
use (`ptr @Fn`) is the same text either way, so only the declaration line changes.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

NAME = r"(?:[-a-zA-Z$._][-a-zA-Z$._0-9]*|\"[^\"]*\")"
DEFINE = re.compile(r"^define\s+(?!internal\b|private\b)[^@]*@(" + NAME + r")\(", re.M)
DECLARE = re.compile(r"^declare\s+[^@]*@(" + NAME + r")\(.*$", re.M)
GLOBAL = re.compile(r"^@(" + NAME + r") = (?!.*\b(?:internal|private)\b)(.*)$", re.M)
EXTERN = re.compile(r"^@(" + NAME + r") = (?:(?:dso_local|hidden|extern_weak|external)\s+)*(?:external|extern_weak)\s+"
                    r"(?:(?:dso_local|hidden|unnamed_addr|local_unnamed_addr|thread_local)\s+)*global\b.*$", re.M)


# ---------------------------------------------------------------- call-site adaptation
# FixFunctionBitcasts (the LTO pass that adapts a direct call whose type differs from its callee's) drops and
# pads arguments and bitcasts same-size types, but traps (`*_bitcast_invalid`) on an i32 argument for an i8 /
# i16 parameter: a K&R call (promoted arguments) of a `char` / `short` parameter, which every MIPS / x86 ABI
# passes as the same register / slot. So each such call is adapted here, where every definition is known: the
# argument is truncated / extended / converted (double <-> float) to the parameter's type, the result is
# extended to what the caller reads, and a non-variadic call of a variadic definition gets the variadic type
# (its extra arguments then go in the wasm varargs block the body walks).

ATTR = re.compile(r"(?:noundef|signext|zeroext|inreg|nonnull|noalias|nocapture|readonly|writeonly|readnone|returned|"
                  r"nofree|immarg|align \d+|dereferenceable(?:_or_null)?\(\d+\)|range\([^)]*\)|"
                  r"(?:byval|sret|elementtype)\([^)]*\))\s+")
SMALL = {"i1": 1, "i8": 8, "i16": 16, "i32": 32, "i64": 64}


def split_top(s):
    out, depth, cur = [], 0, []
    for c in s:
        if c in "([{<":
            depth += 1
        elif c in ")]}>":
            depth -= 1
        if c == "," and depth == 0:
            out.append("".join(cur))
            cur = []
        else:
            cur.append(c)
    if "".join(cur).strip():
        out.append("".join(cur))
    return [x.strip() for x in out]


def type_prefix(a):
    """'i32 noundef %3' -> ('i32', 'noundef ', '%3'); only scalar types are adapted"""
    m = re.match(r"(i\d+|float|double|ptr)\s+((?:" + ATTR.pattern + r")*)(.*)$", a)
    if not m:
        return None
    return m.group(1), m.group(2), m.group(3)


def parse_def(line):
    """(name, ret, retext, [(type, ext)], variadic, cc) of a `define` line, or None"""
    m = re.match(r"define\s+(.*?)\s*@(" + NAME + r")\((.*)\)[^()]*\{\s*$", line)
    if not m:
        return None
    pre, name, params = m.group(1), m.group(2).strip('"'), m.group(3)
    pre = re.sub(r"range\([^)]*\)|dereferenceable(?:_or_null)?\(\d+\)|align \d+", " ", pre)
    toks = pre.split()
    if not toks or not re.fullmatch(r"i\d+|float|double|ptr|void", toks[-1]):
        return None
    ret = toks[-1]
    retext = "signext" if "signext" in toks else "zeroext"
    cc = "fastcc" if "fastcc" in toks else ""
    ps = split_top(params)
    variadic = bool(ps) and ps[-1] == "..."
    if variadic:
        ps = ps[:-1]
    types = []
    for p in ps:
        t = type_prefix(p)
        if t is None:
            return None
        ext = "signext" if "signext" in t[1] else "zeroext"
        types.append((t[0], ext))
    return name, ret, retext, types, variadic, cc


CALL = re.compile(r"^(\s*)(%[-\w.$]+ = )?((?:tail |musttail |notail )?call )((?:(?:fastcc|ccc|noundef|signext|zeroext|nonnull|inreg|noalias|nnan|ninf|nsz|arcp|contract|afn|reassoc|fast) )*)(\S+) (\([^()]*\) )?@(" +
                  NAME + r")\((.*)\)(.*)$")


def conv(fr, to, ext, val, tmp):
    """the instruction converting val (type fr) to type to, or None"""
    if fr == to:
        return None
    if fr in SMALL and to in SMALL:
        if SMALL[fr] > SMALL[to]:
            return "%s = trunc %s %s to %s" % (tmp, fr, val, to)
        return "%s = %s %s %s to %s" % (tmp, "sext" if ext == "signext" else "zext", fr, val, to)
    if fr == "ptr" and to in SMALL:
        return "%s = ptrtoint ptr %s to %s" % (tmp, val, to)
    if fr in SMALL and to == "ptr":
        return "%s = inttoptr %s %s to ptr" % (tmp, fr, val)
    if fr == "double" and to == "float":
        return "%s = fptrunc double %s to float" % (tmp, val)
    if fr == "float" and to == "double":
        return "%s = fpext float %s to double" % (tmp, val)
    return None


def widen_defs(text):
    """every external definition with an i1 / i8 / i16 parameter or result gets an i32 entry under its name: the
    body becomes the internal F.narrow and F truncates / extends around it. Address-taken functions (the SysApi
    and vtable slots) are called through K&R and int-typed pointers, and the optimiser turns some of those into
    direct calls that FixFunctionBitcasts could not adapt; with i32 entries every such call agrees with wasm's
    own (i32) view of a small integer."""
    out = []
    n = 0
    for line in text.split("\n"):
        if line.startswith("define ") and ".narrow" not in line:
            d = parse_def(line)
            if d and not d[4] and (any(t in ("i1", "i8", "i16") for t, _ in d[3]) or d[1] in ("i1", "i8", "i16")):
                name, ret, retext, types, _, cc = d
                ccs = cc + " " if cc else ""
                q = '@"%s.narrow"' % name
                body = re.sub(r"@(?:" + re.escape(name) + r"|\"" + re.escape(name) + r"\")\(", q + "(", line, count=1)
                body = re.sub(r"^define\s+(?:(?:internal|private|dso_local|hidden|protected|weak|weak_odr|linkonce_odr|external|"
                              r"local_unnamed_addr|unnamed_addr)\s+)*", "define internal ", body)
                wret = "i32" if ret in ("i1", "i8", "i16") else ret
                ps, cs, insts = [], [], []
                for k, (t, ext) in enumerate(types):
                    if t in ("i1", "i8", "i16"):
                        ps.append("i32 %%a%d" % k)
                        insts.append("  %%n%d = trunc i32 %%a%d to %s" % (k, k, t))
                        cs.append("%s %s %%n%d" % (t, ext, k))
                    else:
                        ps.append("%s %%a%d" % (t, k))
                        cs.append("%s %%a%d" % (t, k))
                link = "weak hidden" if re.match(r"define\s+weak\b", line) else \
                    "internal" if re.match(r"define\s+(internal|private)\b", line) else "hidden"
                w = ["define %s %s%s @%s(%s) {" % (link, ccs, wret, name if re.fullmatch(r"[-a-zA-Z$._][-a-zA-Z$._0-9]*", name)
                                                 else '"%s"' % name, ", ".join(ps))] + insts
                if ret == "void":
                    w += ["  call %svoid %s(%s)" % (ccs, q, ", ".join(cs)), "  ret void"]
                elif wret != ret:
                    w += ["  %%r = call %s%s %s %s(%s)" % (ccs, retext, ret, q, ", ".join(cs)),
                          "  %%w = %s %s %%r to i32" % ("sext" if retext == "signext" else "zext", ret), "  ret i32 %w"]
                else:
                    w += ["  %%r = call %s%s %s(%s)" % (ccs, ret, q, ", ".join(cs)), "  ret %s %%r" % ret]
                w.append("}")
                out.extend(w + [""])
                out.append(body)
                n += 1
                continue
        out.append(line)
    return "\n".join(out), n


CALLANY = re.compile(r"^(\s*)(%[-\w.$]+ = )?((?:tail |notail )?call )((?:(?:fastcc|ccc|noundef|signext|zeroext|nonnull|inreg|noalias) )*)(\S+) (\([^()]*\) )?(@" + NAME +
                     r"|%[-\w.$]+)\((.*)\)(.*)$")
NARROW = ("i1", "i8", "i16")


def widen_calls(text, counter):
    """every call (direct or through a pointer) passes and returns i1 / i8 / i16 as i32, as widen_defs makes
    every definition take them: wasm's own view, so no call can disagree with its callee over a small int"""
    out = []
    n = 0
    for line in text.split("\n"):
        m = CALLANY.match(line) if " call " in line and "@llvm." not in line and '.narrow"(' not in line else None
        if not m:
            out.append(line)
            continue
        ind, lhs, callkw, fattrs, cret, fnty, callee, args, tail = m.groups()
        al = split_top(args)
        parsed = [type_prefix(a) for a in al]
        kr_ptr = callee.startswith("%") and fnty is not None and fnty.strip() == "(...)"
        if any(p is None for p in parsed) or (not kr_ptr and fnty and re.search(r"\b(i1|i8|i16)\b", fnty) is None and
                                             cret not in NARROW and not any(p[0] in NARROW for p in parsed)):
            out.append(line)
            continue
        if kr_ptr:
            # a call through a pointer cast to `R (*)(...)` (the donors' vtable slots): x86-64 and arm64 pass a
            # variadic call's arguments where a fixed one's go, wasm in a memory block; the call becomes a plain
            # one with the arguments as written, which is what the slot's function takes
            fnty = None
        elif cret not in NARROW and not any(p[0] in NARROW for p in parsed):
            out.append(line)
            continue
        pre, post, newargs = [], [], []
        for ty, attrs, val in parsed:
            if ty in NARROW:
                counter[0] += 1
                tmp = "%%cx.%d" % counter[0]
                pre.append("%s%s = %s %s %s to i32" % (ind, tmp, "sext" if "signext" in attrs else "zext", ty, val))
                newargs.append("i32 " + tmp)
            else:
                newargs.append("%s %s%s" % (ty, attrs, val))
        newret = cret
        if cret in NARROW:
            newret = "i32"
            counter[0] += 1
            tmp = "%%cxr.%d" % counter[0]
            if lhs:
                post.append("%s%strunc i32 %s to %s" % (ind, lhs, tmp, cret))
            lhs = tmp + " = "
        if fnty:
            fnty = re.sub(r"\b(i1|i8|i16)\b", "i32", fnty)
        fattrs = re.sub(r"\b(signext|zeroext) ", "", fattrs) if cret in NARROW else fattrs
        out.extend(pre)
        out.append("%s%s%s%s%s %s%s(%s)%s" % (ind, lhs or "", callkw, fattrs, newret, fnty or "", callee,
                                             ", ".join(newargs), tail))
        out.extend(post)
        n += 1
    return "\n".join(out), n


def adapt_calls(text, defs, counter):
    out = []
    changed = 0
    for line in text.split("\n"):
        m = CALL.match(line) if " call " in line else None
        if not m or m.group(7).strip('"') not in defs:
            out.append(line)
            continue
        ind, lhs, callkw, fattrs, cret, fnty, name, args, tail = m.groups()
        dname, dret, dretext, dtypes, dvar, dcc = defs[name.strip('"')]
        if dname != name.strip('"'):
            name = dname   # a call through an alias (BGBox_ctor = port_ctor_BGBox_ctor) calls its target
            any_alias = True
        else:
            any_alias = False
        al = split_top(args)
        parsed = [type_prefix(a) for a in al]
        if any(p is None for p in parsed):
            out.append(line)
            continue
        pre, newargs, any_change = [], [], False
        for k, (ty, attrs, val) in enumerate(parsed):
            if k < len(dtypes):
                to, ext = dtypes[k]
                counter[0] += 1
                tmp = "%%cw.%d" % counter[0]
                c = conv(ty, to, "signext" if "signext" in attrs else ext, val, tmp)
                if c is not None:
                    pre.append(ind + c)
                    newargs.append("%s %s" % (to, tmp))
                    any_change = True
                    continue
            newargs.append("%s %s%s" % (ty, attrs, val))
        want_var = dvar and (fnty is None or "..." not in fnty)
        if not dvar and fnty is not None and "..." in fnty:
            any_change = True   # a variadic-typed call (a donor's `f(...)` declaration) of a fixed definition
        post = []
        newret = cret
        if lhs and cret != dret and dret != "void":
            counter[0] += 1
            tmp = "%%cwr.%d" % counter[0]
            c = conv(dret, cret, dretext, tmp, lhs.split(" = ")[0])
            if c is not None:
                post.append(ind + c)
                lhs = tmp + " = "
                newret = dret
                any_change = True
        if not any_change and not want_var and not any_alias and not (dvar and cret != dret):
            out.append(line)
            continue
        if dvar:
            fnty = "(%s) " % ", ".join([t for t, _ in dtypes] + ["..."])
            if len(newargs) < len(dtypes):
                out.append(line)   # too few for the fixed parameters: leave it to FixFunctionBitcasts
                continue
            if any(not newargs[k].startswith(dtypes[k][0] + " ") for k in range(len(dtypes))):
                out.append(line)   # a fixed argument of another class (a double for an int): left as it was
                continue
            if newret != dret:
                if lhs and dret == "void" and cret in SMALL:
                    post.append("%s%sadd %s 0, 0" % (ind, lhs, cret))   # the void callee's "result"
                    lhs = None
                elif lhs:
                    out.append(line)
                    continue
                newret = dret
        elif fnty is not None:
            fnty = None   # the call's own type is implied by the (adapted) arguments
        if not lhs and newret != "void":
            counter[0] += 1
            lhs = "%%cwv.%d = " % counter[0]   # an unnamed value result would take the next %N
        out.extend(pre)
        out.append("%s%s%s%s%s %s@%s(%s)%s" % (ind, lhs or "", callkw, fattrs, newret, fnty or "", name,
                                             ", ".join(newargs), tail))
        out.extend(post)
        changed += 1
    return "\n".join(out), changed


def main():
    clang, stamp, objs = sys.argv[1], sys.argv[2], sys.argv[3:]
    texts = {}
    for o in objs:
        ll = o + ".ll"
        if os.path.exists(ll):
            texts[o] = open(ll).read()
    funcs, datas, widened = set(), set(), set()
    for t in texts.values():
        funcs.update(n.strip('"') for n in DEFINE.findall(t))
        for m in GLOBAL.finditer(t):
            rest = m.group(2)
            if " alias " in " " + rest:
                am = re.search(r"\bptr @(" + NAME + r")\s*$", rest)
                continue
            if re.match(r"(?:(?:dso_local|hidden|extern_weak|external)\s+)*(?:external|extern_weak)\b", rest):
                continue
            datas.add(m.group(1).strip('"'))
    # aliases name what they alias
    for t in texts.values():
        for m in re.finditer(r"^@(" + NAME + r") = (?:[a-z_]+\s+)*alias [^,]*, ptr @(" + NAME + r")\s*$", t, re.M):
            a, b = m.group(1).strip('"'), m.group(2).strip('"')
            if b in funcs or b.startswith("_GLOBAL__sub_I_") or b.startswith("_Z"):
                funcs.add(a)
            elif b in datas:
                datas.add(a)
    both = funcs & datas
    if both:
        sys.stderr.write("wasm-fixtypes: defined as both a function and data: %s\n" % " ".join(sorted(both)[:20]))
    for o in list(texts):
        t2, nw = widen_defs(texts[o])
        if nw:
            texts[o] = t2
            with open(o + ".ll", "w") as fh:
                fh.write(t2)
            widened.add(o)
    defs = {}
    for t in texts.values():
        for ln in t.split("\n"):
            if ln.startswith("define ") and not re.match(r"define\s+(internal|private)\b", ln):
                d = parse_def(ln)
                if d:
                    defs[d[0]] = d
    # aliases (port-alias: NEW TARGET, the C1 -> C2 constructors): their calls are adapted to the target
    for t in texts.values():
        for m in re.finditer(r"^@(" + NAME + r") = (?:[a-z_]+\s+)*alias [^,]*, ptr @(" + NAME + r")\s*$", t, re.M):
            a, b = m.group(1).strip('"'), m.group(2).strip('"')
            if b in defs and a not in defs:
                defs[a] = defs[b]
    todo = []
    counter = [0]
    for o, t in texts.items():
        n = [0]

        def to_func(m):
            name = m.group(1).strip('"')
            if name in funcs and name not in datas:
                n[0] += 1
                return "declare void @%s()" % m.group(1)
            return m.group(0)

        def to_data(m):
            name = m.group(1).strip('"')
            if name in datas and name not in funcs:
                n[0] += 1
                return "@%s = external global [0 x i8], align 1" % m.group(1)
            return m.group(0)
        t2 = EXTERN.sub(to_func, t)
        t2 = DECLARE.sub(to_data, t2)
        t2, nw = widen_calls(t2, counter)
        t2, nc = adapt_calls(t2, defs, counter)
        if nc:
            # a call moved from an alias to its target needs the target declared in this unit
            have = set(n.strip('"') for n in re.findall(r"^(?:define|declare)\s[^@]*@(" + NAME + r")\(", t2, re.M))
            have |= set(n.strip('"') for n in re.findall(r"^@(" + NAME + r") = ", t2, re.M))
            need = set(n.strip('"') for n in re.findall(r"\bcall [^@]*@(" + NAME + r")\(", t2)) - have
            if need:
                t2 += "\n" + "\n".join("declare void @%s(...)" % (n if re.fullmatch(r"[-a-zA-Z$._][-a-zA-Z$._0-9]*", n)
                                                                    else '"%s"' % n) for n in sorted(need)) + "\n"
        nc += nw
        n[0] += nc
        if n[0] or o in widened:
            with open(o + ".ll", "w") as fh:
                fh.write(t2)
            todo.append(o)
        elif os.path.getmtime(o + ".ll") > os.path.getmtime(o) if os.path.exists(o) else True:
            todo.append(o)   # an earlier fixup's compile failed

    def build(o):
        return subprocess.run([clang, "--target=wasm32-wasi", os.environ.get("WASM_OPT_LEVEL", "-O2"), "-flto", "-Wno-override-module", "-c", o + ".ll",
                               "-o", o]).returncode
    with ThreadPoolExecutor(max_workers=int(os.environ.get("WASM_FIX_JOBS", "4"))) as ex:
        rcs = list(ex.map(build, todo))
    if any(rcs):
        return 1
    sys.stderr.write("wasm-fixtypes: %d of %d objects re-typed\n" % (len(todo), len(texts)))
    with open(stamp, "w") as fh:
        fh.write("%d\n" % len(todo))
    return 0


if __name__ == "__main__":
    sys.exit(main())
