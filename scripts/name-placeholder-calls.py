#!/usr/bin/env python3
"""Name USO placeholder calls: `gl_func_00000000(a)` -> `CALL(AnimPlayer_start)(a)`.

Every call through a USO import or self-relocation is spelled with a zero placeholder
(gl_func_00000000, func_00000000, game_uso_func_00000000, per-site aliases such as
gl_func_00000000_send ...). The real callee is the target of the module's TextReloc
R_MIPS_26 entry at that jal (resolved with scripts/uso-imports.py: imports go to the
exporting module's Sym, self-relocations to the module's own function). This tool puts
that name at the call site with the macros in include/uso_call.h, which expand to the
exact original token (so the bytes cannot change):

    CALL(f) -> gl_func_00000000   FCALL(f) -> func_00000000
    GCALL(f) -> game_uso_func_00000000   XCALL(tok, f) -> tok

Address tokens (run 520): a call spelled with an import's address-named token
(`game_uso_func_055750(8)`, `gl_func_0007A1C0_x(...)`) is a placeholder too when the token is
pinned to 0 in undefined_syms_auto.txt AND undefined in the real object (the link bakes `jal 0`
just like func_00000000). plan names those with XCALL(tok, f) under the same per-site proof;
their relocs count toward the call count == reloc count rule.

Which source call is which jal (the ordering proof)
----------------------------------------------------
Source order is NOT trusted (nested calls, loop conditions and for-increments are
emitted out of order, while-tests are duplicated). Instead the tool compiles a verify
copy of the file in which call site #N is spelled with its own symbol `npc_uc_N`
(declared like the placeholder's nearest preceding declaration), through the same
asm-processor + cc pipeline as the build (CC / CFLAGS / OPT_FLAGS / CC_ONLY_FLAGS /
MIPSISET from the Makefile's target-specific variables). A function is used only when
  * its verify bytes are IDENTICAL to the function in the real build/.../X.c.o (so the
    real object was compiled from this C: REPLACE_FUNC_BODY donor splices and other
    recipes differ and are refused), and
  * every placeholder jal reloc in the real function is covered by some npc_uc_N
    (call count == reloc count; otherwise the whole function is refused).
Then site N's reloc offset(s) give the module Text offset(s), and the ROM TextReloc
there gives the callee. A site whose jals (an inlined/duplicated call) resolve to two
callees is refused.

REPLACE_FUNC_BODY hosts (run 594): a host function whose own verify bytes differ (its body is a
stand-in, the donor object's body is spliced in) is proven through the donor instead:
  1. the host's verify copy compiled with the DONOR object's flags gives fn byte-identical to
     the donor object's fn, which is the real (spliced) host body (words without a reloc):
     its npc_uc_N relocs place the sites in the spliced body as above; else
  2. mirror: the donor source's fn (whose CALL names `check` pins on the donor object's own
     relocs) is token-identical to the host's fn (sites as `@<token>`), or has the same control
     skeleton (control keywords, braces, `;`, `?:`, `&&`/`||`, and each call as
     `@<token>/<argc>`) with the same number of sites: host site k takes donor site k's name.
src/match_imports/X.c donors map to their host object's module (the REPLACE_FUNC_BODY spec).
Relative #includes in the verify copy are made absolute.

Subcommands (run from the project root after a full build; re-execs under the repo .venv
when pyelftools / crunch64 are missing):
  plan FILE.. [--write] [-v]   name every provable placeholder call (dry run unless
                               --write; adds the uso_call.h include after the first
                               file-scope #include run, outside any #if). Files whose
                               function-like macros spell a placeholder call are reported
                               as "macro-hidden sites" (those jals have no source site, so
                               the function fails the call count == reloc count rule)
  check [FILE..] [--strict]    every CALL/FCALL/GCALL/XCALL(name) must name a ROM callee of
                               its function: per function and token, the multiset of names
                               must be contained in the multiset of ROM jal targets of the
                               real .o's relocs against that token (fast, no compile).
                               A mismatched name that is an OLD name in renames.tsv is
                               reported with its current name (a CALL spelled on a base
                               older than the rename: rename-symbol.py rewrites CALL(OLD)
                               like any whole-word token, but only in the tree it runs on).
                               --strict re-derives every site with the verify compile and
                               requires the exact name. Default FILE.. = every src file
                               that uses the macros. Exit 1 on a mismatch.

Names come from the BUILD (build/tenshoe.elf + map, and the Yay0 block .lnk.o files for the
compressed modules). Run it after the readability gate's clean build: an incremental make can
leave a block .lnk.o with pre-rename names (run 508: SnowAI_func_00006FA8, renamed to
SnowAI_pickFinishBranch on main, was still in game_uso_block1_yay0.lnk.o), and `check` on the
clean build then flags the stale CALL name.

Modules: bootup (src/game_libs, src/bootup_uso), game (game_uso), genproc
(timproc_uso_b5), gui, titproc, eddproc, arcproc, h2hproc, mgrproc, timproc
(timproc_uso_b1), trkproc (timproc_uso_b3), map5_data (src/map4_data_uso_b2: that segment
at ROM 0x71C6E4 is map5_data.uso's Text), n64proc, boarder1-5 (run 510: uso-imports
now parses their Data-first layout), and the 2026-11-16 new modules trnproc, viewrec,
nameent, balloon, heli, fire, checkpoint, snoweffects (src/<m>_uso). The kernel has no placeholders.

NCC / MIPSpro donors (.cpp, run 510): the verify copy is built with the object's own Makefile
recipe (NCC or tools/ncc721.sh, then its objcopy --redefine-sym renames; relative #includes
made absolute), its npc_uc_N declarations wrapped in extern "C" (K&R `()` becomes `(...)`).
Compiler-generated jals (operator new, constructors, vec_new) have no source site, so the
call count == reloc count rule is not applied to .cpp; every named site is still pinned by
its own npc_uc_N reloc in a byte-identical object. `check` on a .cpp aggregates per file
(source blocks such as inline members are not object functions).

ROM guard (run 510, all files): a function is used only when the real object's bytes (words
without a reloc) are the module's Text at that name. A donor's extra copy of a function that
is not spliced (game_libs_o2_34548.c's dbg_printfSink) shares the name but not the bytes.

Every subcommand first runs `make RUN_CC_CHECK=0 -j8 build/assets/*_yay0.bin` (the block
images that carry the compressed modules' names; a plain make does not always rebuild
them) and exits with a clear message if they cannot be made. `--no-build` (before the
subcommand) skips the make and only checks that they exist.
"""
import argparse
import collections
import concurrent.futures
import importlib.util
import os
import re
import shutil
import subprocess
import sys

VENV = '/home/danjs/code/decomp/.venv'
try:
    import crunch64  # noqa: F401
    import elftools  # noqa: F401
except ImportError:
    if os.path.exists(VENV + '/bin/python') and not os.environ.get('NPC_REEXEC'):
        os.environ['NPC_REEXEC'] = '1'
        os.execv(VENV + '/bin/python', [VENV + '/bin/python'] + sys.argv)
    sys.exit('needs pyelftools + crunch64: run with /home/danjs/code/decomp/.venv/bin/python')

from elftools.elf.elffile import ELFFile

HERE = os.path.dirname(os.path.abspath(__file__))


def _load(name, fn):
    spec = importlib.util.spec_from_file_location(name, os.path.join(HERE, fn))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


UI = _load('uso_imports', 'uso-imports.py')

MACRO_OF = {'gl_func_00000000': 'CALL', 'func_00000000': 'FCALL', 'game_uso_func_00000000': 'GCALL'}
TOK_OF = {v: k for k, v in MACRO_OF.items()}
PH_RE = re.compile(r'\b(\w*func_00000000\w*)\b')
MAC_RE = re.compile(r'\b(?:(CALL|FCALL|GCALL)\s*\(\s*(\w+)\s*\)|XCALL\s*\(\s*(\w+)\s*,\s*(\w+)\s*\))')
IDENT = re.compile(r'[A-Za-z_]\w*')
KEYWORDS = {'return', 'else', 'case', 'goto', 'do', 'sizeof', 'default'}
MODULE_OF_DIR = {'game_libs': 'bootup', 'bootup_uso': 'bootup', 'game_uso': 'game',
                 'timproc_uso_b5': 'genproc', 'gui_uso': 'gui', 'titproc_uso': 'titproc',
                 'eddproc_uso': 'eddproc', 'arcproc_uso': 'arcproc', 'h2hproc_uso': 'h2hproc',
                 'mgrproc_uso': 'mgrproc', 'timproc_uso_b1': 'timproc', 'timproc_uso_b3': 'trkproc',
                 'n64proc_uso': 'n64proc', 'map4_data_uso_b2': 'map5_data', **{f'boarder{i}_uso': f'boarder{i}' for i in range(1, 6)},
                 # the 2026-11-16 new modules (docs/1080_NEW_MODULES.md)
                 **{f'{m}_uso': m for m in ('trnproc', 'viewrec', 'nameent', 'balloon', 'heli', 'fire',
                                            'checkpoint', 'snoweffects')}}
BUILD_VENV = '/home/danjs/code/decomp/tools/build-venv/bin'
INCLUDE_LINE = '#include "uso_call.h"'
INCLUDE_REL = '../../include/uso_call.h'
SCRATCH = 'build/npc'


# ---------------------------------------------------------------- source scanning
def mask(src):
    """Same length as src; comments, string / char literals and preprocessor directive lines
    (with continuations) blanked to spaces (newlines kept)."""
    out = list(src)
    n, i = len(src), 0
    bol = True
    while i < n:
        ch = src[i]
        if bol:
            j = i
            while j < n and src[j] in ' \t':
                j += 1
            if j < n and src[j] == '#':
                while j < n and src[j] != '\n':
                    if src[j] == '\\' and j + 1 < n and src[j + 1] == '\n':
                        j += 2
                        continue
                    if src.startswith('/*', j):     # a block comment may run past the line
                        k = src.find('*/', j + 2)
                        j = n if k < 0 else k + 2
                        continue
                    j += 1
                for k in range(i, j):
                    if out[k] != '\n':
                        out[k] = ' '
                i = j
                continue
        bol = False
        if src.startswith('/*', i):
            j = src.find('*/', i + 2)
            j = n if j < 0 else j + 2
        elif src.startswith('//', i):
            j = src.find('\n', i)
            j = n if j < 0 else j
        elif ch in '"\'':
            j = i + 1
            while j < n and src[j] != ch and src[j] != '\n':
                j += 2 if src[j] == '\\' else 1
            j += 1
        else:
            if ch == '\n':
                bol = True
            i += 1
            continue
        for k in range(i, min(j, n)):
            if out[k] != '\n':
                out[k] = ' '
        i = j
    return ''.join(out)


def top_blocks(m):
    """[(boundary, open, close, outer)] for brace blocks at file scope; boundary = index after the
    last file-scope ';' or '}' before the block. Transparent (run 618; before, every site in them
    was silently skipped):
      * a C++ linkage block (`extern "C" {`, its string blanked by mask());
      * a struct / class / union body: its inline member functions are blocks too. outer = the
        boundary before the outermost enclosing class, where file-scope declarations can go
        (= boundary for a block not inside a class).
    A K&R definition `T f(a, b) T1 a; T2 b; {` gets the boundary before its parameter list."""
    res, depth, boundary, op = [], 0, 0, None
    trans = []          # open transparent blocks: (kind, outer boundary)
    semis = [0]         # file-scope ';' boundaries since the last block (K&R parameter lists)

    def outer_of(b):
        cls = [o for k, o in trans if k == 'class']
        return cls[0] if cls else b
    for i, ch in enumerate(m):
        if ch == '{':
            if depth == 0:
                h = m[boundary:i]
                if re.fullmatch(r'\s*extern\s*', h):
                    trans.append(('link', None))
                    boundary = i + 1
                    semis = [boundary]
                    continue
                if re.fullmatch(r'\s*(?:typedef\s+)?(?:struct|class|union)\b[^=()]*', h):
                    trans.append(('class', outer_of(boundary)))
                    boundary = i + 1
                    semis = [boundary]
                    continue
                op = i
                if '(' not in h:
                    for b in reversed(semis[:-1]):
                        if re.fullmatch(r'[^;{}()=]*\b[A-Za-z_]\w*\s*\(\s*\w*(?:\s*,\s*\w+)*\s*\)'
                                        r'(?:[^;{}()=]+;)+\s*', m[b:i]):
                            boundary = b
                            break
            depth += 1
        elif ch == '}':
            if depth == 0 and trans:
                trans.pop()
                boundary = i + 1
                semis = [boundary]
                continue
            depth -= 1
            if depth == 0:
                res.append((boundary, op, i, outer_of(boundary)))
                boundary = i + 1
                semis = [boundary]
            depth = max(depth, 0)
        elif ch == ';' and depth == 0:
            boundary = i + 1
            semis.append(boundary)
    return res


class Site:
    __slots__ = ('i', 'pos', 'end', 'tok', 'name', 'func', 'block')

    def __init__(self, i, pos, end, tok, name):
        self.i, self.pos, self.end, self.tok, self.name = i, pos, end, tok, name
        self.func = self.block = None


def is_decl(m, pos):
    start = max(m.rfind(';', 0, pos), m.rfind('{', 0, pos), m.rfind('}', 0, pos)) + 1
    pre = ' '.join(m[start:pos].split())
    if not pre:
        return False
    if re.match(r'(extern|static|typedef)\b', pre):
        return True
    w = re.fullmatch(r'(?:(?:const|volatile|unsigned|signed|struct|union|enum)\s+)*([A-Za-z_]\w*)\s*\**', pre)
    return bool(w) and w.group(1) not in KEYWORDS


def func_header_name(m, boundary, op):
    h = m[boundary:op]
    if '=' in h:
        return None
    mm = re.search(r'([A-Za-z_]\w*)\s*\(', h)
    return mm.group(1) if mm and ')' in h else None


def tok_re(extra=()):
    """PH_RE, plus the given address tokens (run 520) as whole words."""
    if not extra:
        return PH_RE
    return re.compile(r'\b(\w*func_00000000\w*|' + '|'.join(sorted(map(re.escape, extra))) + r')\b')


def scan(src, m=None, extra=()):
    """Call sites: raw placeholder tokens (name None) and macro calls (name set), inside
    file-scope function blocks, in source order. extra: address tokens that are placeholders
    too (zero_tokens() & the object's undefined symbols)."""
    m = m if m is not None else mask(src)
    tre = tok_re(extra)
    blocks = [b for b in top_blocks(m) if func_header_name(m, b[0], b[1])]
    sites = []
    spans = []
    for mt in MAC_RE.finditer(m):
        tok = TOK_OF[mt.group(1)] if mt.group(1) else mt.group(3)
        sites.append((mt.start(), mt.end(), tok, mt.group(2) or mt.group(4)))
        spans.append((mt.start(), mt.end()))
    # placeholder-named DATA (e.g. `extern ViContext *gl_func_00000000_vinext711;`): its uses
    # are not calls; spelling them as npc_uc_N functions breaks the verify compile (run 510)
    data_toks = {mt.group(1) for mt in tre.finditer(m)
                 if is_decl(m, mt.start()) and not re.match(r'\s*\(', m[mt.end():])}
    for mt in tre.finditer(m):
        if is_decl(m, mt.start()) or any(a <= mt.start() < b for a, b in spans):
            continue     # (the token inside an XCALL(tok, f) is that macro's site)
        if mt.group(1) in data_toks:
            continue
        sites.append((mt.start(), mt.end(), mt.group(1), None))
    sites.sort()
    out = []
    for pos, end, tok, name in sites:
        blk = next((b for b in blocks if b[1] < pos < b[2]), None)
        if blk is None:
            continue
        s = Site(len(out), pos, end, tok, name)
        s.block = blk
        s.func = func_header_name(m, blk[0], blk[1])
        out.append(s)
    return out, m


_DECLS = {}


def decl_for(src, m, tok, pos, name):
    """`extern <type> name(<params>); ` copied from tok's nearest preceding declaration
    (default `extern int name();`, the K&R form most files use)."""
    key = (m, tok)      # not id(m): plan threads free and reuse ids across files (run 510)
    if key not in _DECLS:
        lst = []
        for d in re.finditer(r'(?:(?<=[;{}])|^)[ \t\n]*((?:[A-Za-z_]\w*[ \t\*]+)+?)\**' + re.escape(tok) +
                             r'\s*\(([^;{}]*?)\)\s*;', m, re.M):
            head = d.group(1).split()
            if head and head[0] in KEYWORDS:
                continue
            full = ' '.join(m[d.start(1):d.end()].split())
            lst.append((d.start(), re.sub(r'^(extern|static)\s+', '', full)))
        _DECLS[key] = lst
    best = None
    for p0, full in _DECLS[key]:
        if p0 >= pos:
            break
        best = full
    if best is None:
        return f'extern int {name}(); '
    return 'extern ' + re.sub(r'\b' + re.escape(tok) + r'\b', name, best, count=1) + ' '


def strip_static_funcs(m, edits):
    """File-scope `static` on function declarations / definitions: IDO puts static functions
    in .mdebug only (asm-processor's post-process makes the ELF symbols), so the verify object
    would have no symbol for them. Dropping the keyword gives them one; the bytes are compared
    with the real object anyway."""
    depth = 0
    stmt = 0
    for i, ch in enumerate(m):
        if ch == '{':
            if depth == 0:
                _strip(m, stmt, i, edits)
            depth += 1
        elif ch == '}':
            depth = max(depth - 1, 0)
            if depth == 0:
                stmt = i + 1
        elif ch == ';' and depth == 0:
            _strip(m, stmt, i, edits)
            stmt = i + 1


def _strip(m, a, b, edits):
    t = m[a:b]
    mm = re.match(r'\s*static\b', t)
    if mm and '(' in t and ('=' not in t or t.index('(') < t.index('=')):
        p = a + mm.end() - 6
        edits.append((p, p + 6, '      '))


def insert_point(m, boundary):
    i = boundary
    while i < len(m) and m[i] in ' \t\n\r':
        i += 1
    return i


def verify_text(src, m, sites, cpp=False):
    edits = []      # (pos, end, text)
    by_block = collections.defaultdict(list)
    for s in sites:
        edits.append((s.pos, s.end, f'npc_uc_{s.i}'))
        by_block[s.block].append(s)
    for blk, ss in by_block.items():
        ip = insert_point(m, blk[3])
        decls = ''.join(decl_for(src, m, s.tok, s.pos, f'npc_uc_{s.i}') for s in ss)
        if cpp:     # unmangled names; C++ `f()` means (void), so the K&R default becomes (...)
            decls = 'extern "C" { ' + re.sub(r'\(\); $', '(...); ', decls.replace('(); ', '(...); ')) + '} '
        edits.append((ip, ip, decls))
    strip_static_funcs(m, edits)
    out = src
    for pos, end, text in sorted(edits, key=lambda e: (e[0], e[1]), reverse=True):
        out = out[:pos] + text + out[end:]
    return out


# ---------------------------------------------------------------- Makefile flags
class MakeVars:
    def __init__(self, path='Makefile'):
        self.glob, self.tgt = {}, []
        self.lines = open(path).read().replace('\\\n', ' ').split('\n')
        for line in self.lines:
            if line.startswith('\t') or line.startswith('#'):
                continue
            mm = re.match(r'^([A-Za-z_][\w]*)\s*(:=|\?=|=|\+=)\s*(.*)$', line)
            if mm:
                k, op, v = mm.groups()
                if op == '+=':
                    self.glob[k] = self.glob.get(k, '') + ' ' + v
                elif op != '?=' or k not in self.glob:
                    self.glob[k] = v
                continue
            mm = re.match(r'^([^:=#]+?):\s*(?:private\s+)?(?:override\s+)?([A-Za-z_]\w*)\s*(:=|=|\+=)\s*(.*)$', line)
            if mm:
                self.tgt.append((mm.group(1).split(), mm.group(2), mm.group(3), mm.group(4)))

    def expand(self, v, local):
        for _ in range(10):
            nv = re.sub(r'\$\((\w+)\)', lambda x: local.get(x.group(1), self.glob.get(x.group(1), '')), v)
            if nv == v:
                break
            v = nv
        return v.strip()

    def for_target(self, obj):
        loc = {}
        for tgts, k, op, v in self.tgt:
            hit = any(t == obj or ('%' in t and re.fullmatch(re.escape(t).replace('%', '.*'), obj))
                      for t in tgts)
            if hit:
                loc[k] = (loc.get(k, self.glob.get(k, '')) + ' ' + v) if op == '+=' else v
        return {k: self.expand(loc.get(k, self.glob.get(k, '')), loc)
                for k in ('CC', 'CFLAGS', 'OPT_FLAGS', 'CC_ONLY_FLAGS', 'MIPSISET', 'CPPFLAGS')}

    def splices(self):
        """REPLACE_FUNC_BODY specs (run 594): {host_obj: {func: donor_obj}} for the matching-build
        objects (build/src/...); each `func=$(DONOR_VAR)` entry expanded."""
        if getattr(self, '_spl', None) is None:
            self._spl = collections.defaultdict(dict)
            for tgts, k, op, v in self.tgt:
                if k != 'REPLACE_FUNC_BODY':
                    continue
                for item in self.expand(v, {}).split():
                    if '=' not in item:
                        continue
                    fn, donor = item.split('=', 1)
                    for tg in tgts:
                        tg = self.expand(tg, {})
                        if tg.startswith('build/src/'):
                            self._spl[tg][fn] = donor
        return self._spl

    def donor_hosts(self):
        """{donor_obj: [(host_obj, func)]}"""
        out = collections.defaultdict(list)
        for host, d in self.splices().items():
            for fn, donor in d.items():
                out[donor].append((host, fn))
        return out

    def recipe(self, obj):
        """The recipe lines of obj's rule: an explicit rule naming it (targets may be $(VAR)s),
        else the matching pattern rule with the shortest stem (GNU make's choice)."""
        rules = []      # (targets, recipe lines)
        cur = None
        for line in self.lines:
            if line.startswith('\t'):
                if cur is not None:
                    cur.append(line[1:])
                continue
            cur = None
            if not line.strip() or line.startswith('#'):
                continue
            mm = re.match(r'^([^:=#\t][^:=#]*?):(?!=)\s*(.*)$', line)
            if mm and not re.match(r'\s*(?:private\s+)?(?:override\s+)?[A-Za-z_]\w*\s*(:=|=|\+=)', mm.group(2)):
                cur = []
                rules.append((self.expand(mm.group(1), {}).split(), cur))
        exact = [r for t, r in rules if obj in t and r]
        if exact:
            return exact[-1]
        pats = []
        for t, r in rules:
            for x in t:
                if '%' in x and r:
                    a, b = x.split('%', 1)
                    if obj.startswith(a) and obj.endswith(b) and len(obj) >= len(a) + len(b):
                        pats.append((len(obj) - len(a) - len(b), r))
        return min(pats, key=lambda p: p[0])[1] if pats else None


# ---------------------------------------------------------------- ELF
def elf_info(path):
    """{func: (value, size, bytes)}, {offset: symname} of R_MIPS_26 .text relocs."""
    with open(path, 'rb') as f:
        e = ELFFile(f)
        text_idx = [i for i, s in enumerate(e.iter_sections()) if s.name == '.text']
        if not text_idx:
            return {}, {}
        ti = text_idx[0]
        text = e.get_section(ti).data()
        st = e.get_section_by_name('.symtab')
        syms = sorted((s['st_value'], s['st_size'], s.name) for s in st.iter_symbols()
                      if s['st_shndx'] == ti and s.name and s['st_info']['type'] == 'STT_FUNC')
        funcs = {}
        for k, (v, sz, nm) in enumerate(syms):
            if not sz:
                sz = (syms[k + 1][0] if k + 1 < len(syms) else len(text)) - v
            funcs.setdefault(nm, (v, sz, text[v:v + sz]))
        rel = {}
        for sec in e.iter_sections():
            if sec.name in ('.rel.text', '.rela.text'):
                for r in sec.iter_relocations():
                    if r['r_info_type'] == 4:
                        s = st.get_symbol(r['r_info_sym'])
                        rel[r['r_offset']] = s.name or ('sect:' + str(s['st_shndx']))
        return funcs, rel


def reloc_offsets(path):
    """every .text offset with a reloc of any type"""
    with open(path, 'rb') as f:
        e = ELFFile(f)
        return {r['r_offset'] for sec in e.iter_sections() if sec.name in ('.rel.text', '.rela.text')
                for r in sec.iter_relocations()}


def rom_bytes_match(md, base, rb, rv, relocs):
    """The real object's function (rb at .o offset rv) is the code in the module's Text at
    base: every word without a reloc is identical. Guards donors that are not spliced (an NM
    comparison donor shares the host's name but not its bytes)."""
    t = md.secs.get('Text', b'')
    if base < 0 or base + len(rb) > len(t):
        return base < 0 and base + len(rb) <= len(t) and \
            all(rb[k:k + 4] == t[base + k:base + k + 4] for k in range(-base, len(rb), 4)
                if rv + k not in relocs)
    return all(rb[k:k + 4] == t[base + k:base + k + 4] for k in range(0, len(rb), 4) if rv + k not in relocs)


# ---------------------------------------------------------------- ROM resolution
def block_targets():
    """The Yay0 block images uso-imports reads names from (build/assets/X_yay0.text.bin is a
    by-product of the make target build/assets/X_yay0.bin)."""
    return sorted({tb.replace('.text.bin', '.bin') for tb, _ in UI.COMPRESSED})


def ensure_build(no_build=False):
    """Names of the compressed modules (game, genproc, mgrproc, timproc, trkproc, map5_data)
    come from the Yay0 block images, which some make invocations do not (re)build. Make them
    explicitly (a no-op when current); fail clearly when that is impossible."""
    need = ['build/tenshoe.elf', 'build/tenshoe.map'] + [t.replace('.bin', '.text.bin') for t in block_targets()]
    if not no_build:
        env = dict(os.environ)
        if os.path.isdir(BUILD_VENV):
            env['PATH'] = BUILD_VENV + os.pathsep + env.get('PATH', '')
        r = subprocess.run(['make', 'RUN_CC_CHECK=0', '-j8'] + block_targets(), env=env,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if r.returncode:
            print('\n'.join(r.stdout.splitlines()[-10:]), file=sys.stderr)
            sys.exit('make of the Yay0 block targets failed: ' + ' '.join(block_targets()))
    missing = [p for p in need if not os.path.exists(p)]
    if missing:
        sys.exit('missing build products (run make RUN_CC_CHECK=0 -j8 ' + ' '.join(block_targets()) +
                 ' after a full make): ' + ' '.join(missing))


class Rom:
    def __init__(self):
        rom = open('baserom.z64', 'rb').read()
        self.mods = UI.find_modules(rom)
        UI.attach_names(self.mods, '.')
        self.res = UI.Resolver(self.mods)
        self.by_short = {m.short: m for m in self.mods}
        self._jal = {}

    def module_for(self, path):
        d = path.split('/')[1] if path.startswith('src/') else None
        return self.by_short.get(MODULE_OF_DIR.get(d))

    def func_off(self, md, name):
        hits = [v for v, sz, nm in md.funcs if nm == name]
        return hits[0] if len(hits) == 1 else None

    def jal_target(self, md, off):
        if md.short not in self._jal:
            self._jal[md.short] = {o: f1 >> 4 for f1, o, _ in md.relocs('Text') if f1 & 7 == 1}
        idx = self._jal[md.short].get(off)
        if idx is None:
            return None
        ex, name = self.res.target(md, idx)
        return name


# ---------------------------------------------------------------- planning
def obj_of(path):
    return 'build/' + path + '.o'


ADDR_TOK_RE = re.compile(r'\w*func_(?!00000000)[0-9A-Fa-f]{6,8}\w*|\w+_c34458')  # _c34458: NCC donors' blank-jal tokens (run 618)
_ZERO = None


def zero_tokens():
    """Address tokens (`game_uso_func_055750`, `gl_func_0007A1C0_x` ...) pinned to 0 in
    undefined_syms_auto.txt: the link bakes `jal 0` exactly like func_00000000, so the ROM
    TextReloc at the jal names the callee (run 520, ~950 such call sites ROM-wide)."""
    global _ZERO
    if _ZERO is None:
        _ZERO = set()
        try:
            for line in open('undefined_syms_auto.txt', errors='replace'):
                mm = re.match(r'\s*(\w+)\s*=\s*(0|0x0+)\s*;', line)
                if mm and ADDR_TOK_RE.fullmatch(mm.group(1)):
                    _ZERO.add(mm.group(1))
        except OSError:
            pass
    return _ZERO


def undefined_syms(path):
    """names of the undefined (SHN_UNDEF) symbols of an object"""
    with open(path, 'rb') as f:
        st = ELFFile(f).get_section_by_name('.symtab')
        return {s.name for s in st.iter_symbols() if s.name and s['st_shndx'] == 'SHN_UNDEF'} if st else set()


def addr_tokens(path):
    """the address tokens that are placeholders in path's real object: zero-pinned AND undefined
    there (a token defined in the object is a real local function, not an import placeholder)"""
    o = obj_of(path)
    return frozenset(zero_tokens() & undefined_syms(o)) if os.path.exists(o) else frozenset()


def addr_token_files():
    """src files that call (`tok(`) a zero-pinned address token, sorted"""
    z, out = zero_tokens(), []
    for root, _, fs in os.walk('src'):
        for f in fs:
            if f.endswith(('.c', '.cpp')):
                p = os.path.join(root, f)
                if any(mt.group(1) in z for mt in re.finditer(r'\b(\w+)\s*\(', mask(open(p, errors='replace').read()))):
                    out.append(p)
    return sorted(out)


def is_placeholder(symname, tok):
    return symname == tok or bool(PH_RE.fullmatch(symname or '')) or symname.startswith('sect:')


_DONOR_MOD = {}


def donor_module(path, rom, mv):
    """A REPLACE_FUNC_BODY donor outside the module dirs (src/match_imports/X.c, run 594): the
    module of the host object(s) it is spliced into (one module for all of them, else None)."""
    if path not in _DONOR_MOD:
        obj = obj_of(path)
        mods = {rom.module_for(h[len('build/'):-len('.o')])
                for d, hs in mv.donor_hosts().items() if d == obj or d.startswith(obj + '.') for h, _ in hs}
        _DONOR_MOD[path] = mods.pop() if len(mods) == 1 else None
    return _DONOR_MOD[path]


def compile_verify(path, text, mv, flags_of=None, tag='v'):
    """-> (funcs, rel) of the verify object, or raises RuntimeError. flags_of: compile with that
    object's target-specific flags instead of path's own (a donor's, run 594); tag names the
    scratch files so several flag sets of one file can be compiled side by side."""
    obj = obj_of(path)
    fl = mv.for_target(flags_of or obj)
    base = os.path.join(SCRATCH, path) + ('' if tag == 'v' else '.' + tag)
    os.makedirs(os.path.dirname(base), exist_ok=True)
    if path.endswith('.cpp'):
        return compile_verify_cpp(path, text, mv, base)
    vsrc, pre, vobj = base + '.v.c', base + '.pre.c', base + '.v.o'
    d = os.path.dirname(os.path.abspath(path))       # relative #includes (src/match_imports, run 594)
    text = re.sub(r'^(\s*#\s*include\s*")([^"/][^"]*)"',
                  lambda x: x.group(1) + os.path.normpath(os.path.join(d, x.group(2))) + '"'
                  if os.path.exists(os.path.join(d, x.group(2))) else x.group(0), text, flags=re.M)
    open(vsrc, 'w').write(text)
    asmproc = ['python3', 'tools/asm-processor/asm_processor.py'] + fl['OPT_FLAGS'].split()
    with open(pre, 'w') as fo:
        r = subprocess.run(asmproc + [vsrc], stdout=fo, stderr=subprocess.PIPE, text=True)
    if r.returncode:
        raise RuntimeError('asm-processor: ' + r.stderr.strip()[-300:])
    cmd = [fl['CC'] or 'tools/ido-static-recomp/build/7.1/out/cc', '-c'] + fl['CFLAGS'].split() + \
        fl['OPT_FLAGS'].split() + fl['CC_ONLY_FLAGS'].split() + fl['MIPSISET'].split() + \
        fl['CPPFLAGS'].split() + ['-o', vobj, pre]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode or not os.path.exists(vobj):
        errs = [ln for ln in r.stdout.splitlines() if 'Error' in ln][:3]
        raise RuntimeError('cc: ' + ' | '.join(errs or r.stdout.splitlines()[-3:]))
    return elf_info(vobj)


def compile_verify_cpp(path, text, mv, base):
    """NCC / MIPSpro donors (.cpp): run the object's own Makefile recipe (NCC or ncc721.sh, then
    any objcopy --redefine-sym renames) on the verify copy. No asm-processor in these rules."""
    obj = obj_of(path)
    rec = mv.recipe(obj)
    if not rec:
        raise RuntimeError('no Makefile recipe for ' + obj)
    vsrc, vobj = base + '.v.cpp', base + '.v.o'
    # donors build without -I: their includes are relative to the source dir
    d = os.path.dirname(os.path.abspath(path))
    text = re.sub(r'^(\s*#\s*include\s*")([^"/][^"]*)"',
                  lambda x: x.group(1) + os.path.normpath(os.path.join(d, x.group(2))) + '"'
                  if os.path.exists(os.path.join(d, x.group(2))) else x.group(0), text, flags=re.M)
    open(vsrc, 'w').write(text)
    if os.path.exists(vobj):
        os.remove(vobj)
    loc = dict(mv.for_target(obj))
    for line in rec:
        line = line.lstrip('@-').strip()
        if not line or line.startswith('mkdir'):
            continue
        cmd = line.replace('$$', '\0').replace('$@', vobj).replace('$<', vsrc)
        cmd = mv.expand(cmd, loc).replace('\0', '$')
        r = subprocess.run(['bash', '-c', cmd], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if r.returncode:
            errs = [ln for ln in r.stdout.splitlines() if 'rror' in ln][:3]
            raise RuntimeError('ncc: ' + ' | '.join(errs or r.stdout.splitlines()[-3:]))
    if not os.path.exists(vobj):
        raise RuntimeError('ncc: no object')
    return elf_info(vobj)


def plan_file(path, rom, mv, verbose=False):
    """-> dict(path, sites=[Site], derived={i: name}, left={i: reason}, error=str|None)."""
    out = {'path': path, 'sites': [], 'derived': {}, 'left': {}, 'error': None, 'hidden': {}}
    src = open(path, errors='replace').read()
    extra = addr_tokens(path)
    sites, m = scan(src, extra=extra)
    out['sites'] = sites
    out['hidden'] = hidden = macro_hidden(src, m, extra)
    if not sites:
        return out
    md = rom.module_for(path) or donor_module(path, rom, mv)
    if md is None:
        out['error'] = 'no USO module mapped for this src dir (MODULE_OF_DIR)'
        return out
    if not md.funcs:
        out['error'] = f'module {md.short}: no function names (build products missing?)'
        return out
    if not os.path.exists(obj_of(path)):
        out['error'] = 'no built object ' + obj_of(path)
        return out
    try:
        vfuncs, vrel = compile_verify(path, verify_text(src, m, sites, path.endswith('.cpp')), mv)
    except RuntimeError as ex:
        out['error'] = str(ex)
        return out
    rfuncs, rrel = elf_info(obj_of(path))
    rall = reloc_offsets(obj_of(path))
    # npc offsets per site, per function
    site_offs = collections.defaultdict(list)       # i -> [(func, reloff)]
    for off, sym in vrel.items():
        mm = re.fullmatch(r'npc_uc_(\d+)', sym)
        if not mm:
            continue
        fn = next((f for f, (v, sz, _) in vfuncs.items() if v <= off < v + sz), None)
        site_offs[int(mm.group(1))].append((fn, off - vfuncs[fn][0] if fn else None))
    bad_funcs = {}
    mirrored = {}       # site i -> name, from the donor source's checked CALL names (run 594)
    for fn, (v, sz, b) in vfuncs.items():
        mine = [i for i, l in site_offs.items() if any(f == fn for f, _ in l)]
        if not mine:
            continue
        if fn not in rfuncs:
            bad_funcs[fn] = 'not in the real object'
            continue
        rv, rsz, rb = rfuncs[fn]
        if rb != b:
            donor = mv.splices().get(obj_of(path), {}).get(fn)
            alt = donor_offsets(path, src, m, sites, mv, fn, donor, rb, rv, rall) if donor else None
            if isinstance(alt, dict):
                # the host's C compiled with the donor's flags IS the spliced body (run 594):
                # its npc_uc_N relocs pin the sites onto the real (spliced) function
                for i in mine:
                    site_offs[i] = [x for x in site_offs[i] if x[0] != fn]
                for i, offs in alt.items():
                    site_offs[i] = [x for x in site_offs.get(i, []) if x[0] != fn] + [(fn, o) for o in offs]
                mine = [i for i, l in site_offs.items() if any(f == fn for f, _ in l)]
            else:
                mir = mirror_names(src, m, sites, fn, donor) if donor else None
                if isinstance(mir, dict):
                    mirrored.update(mir)
                    continue
                bad_funcs[fn] = 'verify bytes differ from the real object (donor splice / other recipe)' + \
                    (f' -- donor {donor}: {alt}; mirror: {mir}' if donor else '')
                continue
        covered = {o for i in mine for f, o in site_offs[i] if f == fn}
        real_ph = {off - rv for off, s in rrel.items() if rv <= off < rv + rsz and
                   (PH_RE.fullmatch(s) or s in extra or s.startswith('sect:'))}
        # C++: operator new / constructors / vec_new are jals with no source call (often to a
        # placeholder via objcopy --redefine-sym). Each site is still pinned by its own npc_uc_N
        # reloc in a byte-identical object, so those extra jals cannot be mistaken for it.
        if real_ph - covered and not path.endswith('.cpp'):
            bad_funcs[fn] = f'call count != reloc count ({len(covered)} sites, {len(real_ph)} placeholder jals)' + \
                (' -- macro-hidden sites: ' + ', '.join(f'{k} x{v}' for k, v in sorted(hidden[fn].items()))
                 if hidden.get(fn) else '')
            continue
        if rom.func_off(md, fn) is None:
            bad_funcs[fn] = f'not (uniquely) in the {md.short} name table'
        elif not rom_bytes_match(md, rom.func_off(md, fn), rb, rv, rall):
            bad_funcs[fn] = f'real object bytes are not the {md.short} Text at {fn} (unspliced donor?)'
    for s in sites:
        if s.i in mirrored:
            out['derived'][s.i] = mirrored[s.i]
            continue
        offs = site_offs.get(s.i)
        if not offs:
            out['left'][s.i] = 'no jal in the build (not a call, or not compiled)'
            continue
        fns = {f for f, _ in offs}
        if len(fns) != 1 or None in fns:
            out['left'][s.i] = 'jal outside a function symbol'
            continue
        fn = fns.pop()
        if fn in bad_funcs:
            out['left'][s.i] = f'{fn}: {bad_funcs[fn]}'
            continue
        base = rom.func_off(md, fn)
        rv = rfuncs[fn][0]
        names = set()
        for _, o in offs:
            if not is_placeholder(rrel.get(rv + o, ''), s.tok):
                names.add(None)
                continue
            names.add(rom.jal_target(md, base + o))
        if len(names) != 1 or None in names:
            out['left'][s.i] = f'{fn}: callee unresolved or ambiguous {sorted(map(str, names))}'
            continue
        nm = names.pop()
        if not IDENT.fullmatch(nm):
            out['left'][s.i] = f'{fn}: callee {nm} is not a symbol name'
            continue
        if nm == s.tok:
            out['left'][s.i] = f'{fn}: callee is the function named {nm} itself'
            continue
        out['derived'][s.i] = nm
    return out


_DONOR_VERIFY = {}
TOKEN = re.compile(r'\w+|\S')


CTRL = {'if', 'else', 'for', 'while', 'do', 'switch', 'case', 'default', 'return', 'goto', 'break',
        'continue', '{', '}', ';', '?', ':', '&&', '||'}


def site_argc(m, s):
    """argument count of the call at site s (-1: not a call)"""
    k = s.end
    while k < len(m) and m[k] in ' \t\n':
        k += 1
    if k < len(m) and m[k] == ')':       # `CALL(f)` macro: the call's ( follows the macro's )
        k += 1
        while k < len(m) and m[k] in ' \t\n':
            k += 1
    if k >= len(m) or m[k] != '(':
        return -1
    depth, i, n, empty = 1, k + 1, 1, True
    while i < len(m) and depth:
        ch = m[i]
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        elif ch == ',' and depth == 1:
            n += 1
        if depth and not ch.isspace():
            empty = False
        i += 1
    return 0 if empty else n


def skeleton(m, blk, sites):
    """The control skeleton of a block: control keywords, braces, `;`, `?:`, `&&`/`||` and each
    call site as `@<tok>/<argc>`, in source order."""
    toks = body_tokens(m, blk, sites, argc=True)
    out = []
    for i, x in enumerate(toks):
        if x.startswith('@') or x in CTRL:
            out.append(x)
        elif x in '&|' and i + 1 < len(toks) and toks[i + 1] == x:
            pass
    return out


def body_tokens(m, blk, sites, argc=False):
    """Tokens of a function block (header .. closing brace) with each call site spelled `@<tok>`
    (`@<tok>/<argc>` with argc)."""
    a, op, cl = blk
    edits = sorted((s.pos, s.end, '@' + s.tok + (f'/{site_argc(m, s)}' if argc else '')) for s in sites if s.block == blk)
    out, k = [], a
    for pos, end, rep_ in edits:
        out += TOKEN.findall(m[k:pos]) + [rep_]
        k = end
    return out + TOKEN.findall(m[k:cl + 1])


def donor_source(donor):
    s = donor[len('build/'):] if donor.startswith('build/') else donor
    s = re.sub(r'\.o(?:\.linked\.elf\.bound\.elf)?$', '', s)
    return s if os.path.exists(s) else None


def mirror_names(src, m, sites, fn, donor):
    """REPLACE_FUNC_BODY host whose C is the donor's C (run 594): when fn's body in the host is
    token-identical to fn's body in the donor source (comments, whitespace and the CALL spelling
    aside: every site compared as `@<placeholder token>`), the host's site k IS the donor's site
    k, whose name the donor carries (CALL-named there; `check` pins it on the donor object's
    relocs). -> {host site i: name}, or a reason string."""
    dsrc_path = donor_source(donor)
    if not dsrc_path:
        return 'no donor source'
    dsrc = open(dsrc_path, errors='replace').read()
    dsites, dm = scan(dsrc, extra=addr_tokens(dsrc_path))
    dblk = {s.block for s in dsites if s.func == fn}
    hblk = {s.block for s in sites if s.func == fn}
    if len(dblk) != 1 or len(hblk) != 1:
        return 'fn is not one block in both sources'
    dblk, hblk = dblk.pop(), hblk.pop()
    ds = [s for s in dsites if s.block == dblk]
    hs = [s for s in sites if s.block == hblk]
    if body_tokens(dm, dblk, dsites) != body_tokens(m, hblk, sites):
        # a stand-in body that differs in types / casts / decl order only (run 594): the same
        # control skeleton with the same calls (token + argument count) in the same places
        if len(ds) != len(hs):
            return f'host has {len(hs)} sites, the donor {len(ds)}'
        if skeleton(dm, dblk, dsites) != skeleton(m, hblk, sites):
            return 'host control skeleton / call sequence differs from the donor\'s'
        if len({(s.tok, s.name) for s in ds}) > 1 and len(set(skeleton(dm, dblk, dsites))) < 3:
            return 'no skeleton to order the calls by'
    ds = [s for s in dsites if s.block == dblk]
    hs = [s for s in sites if s.block == hblk]
    if len(ds) != len(hs) or any(h.tok != d.tok for h, d in zip(hs, ds)):
        return 'site tokens differ from the donor\'s'
    got = {h.i: d.name for h, d in zip(hs, ds) if h.name is None and d.name}
    return got if got else 'the donor\'s matching sites are unnamed too'



def donor_offsets(path, src, m, sites, mv, fn, donor, rb, rv, rall):
    """REPLACE_FUNC_BODY host (run 594): compile the host's verify copy with the DONOR object's
    target-specific flags. When that compile's fn is byte-identical to the donor object's fn and
    to the real (spliced) host object's fn (words without a reloc), its npc_uc_N relocs place
    each site in the spliced body: -> {site i: [offset in fn]}; else a reason string."""
    if not os.path.exists(donor) or donor.endswith('.cpp.o') != path.endswith('.cpp'):
        return 'donor object missing or of another language'
    if path.endswith('.cpp'):
        return 'C++ host: the donor recipe is not replayed'
    dfuncs, _ = elf_info(donor)
    if fn not in dfuncs:
        return 'fn not in the donor object'
    db = dfuncs[fn][2]
    if len(db) != len(rb):
        return 'donor size differs from the spliced body'
    if not all(rb[k:k + 4] == db[k:k + 4] for k in range(0, len(rb), 4) if rv + k not in rall):
        return 'the real object\'s body is not the donor\'s (not spliced?)'
    key = (path, tuple(sorted(mv.for_target(donor).items())))
    if key not in _DONOR_VERIFY:
        tag = 'd%d' % len([k for k in _DONOR_VERIFY if k[0] == path])
        try:
            _DONOR_VERIFY[key] = compile_verify(path, verify_text(src, m, sites), mv, flags_of=donor, tag=tag)
        except RuntimeError as ex:
            _DONOR_VERIFY[key] = str(ex)
    got = _DONOR_VERIFY[key]
    if isinstance(got, str):
        return 'verify compile with the donor flags failed: ' + got
    vfuncs, vrel = got
    if fn not in vfuncs:
        return 'fn not in the donor-flag verify object'
    v, sz, vb = vfuncs[fn]
    if vb != db:
        return 'the host C with the donor flags is not the donor body'
    out = collections.defaultdict(list)
    for off, sym in vrel.items():
        mm = re.fullmatch(r'npc_uc_(\d+)', sym)
        if mm and v <= off < v + sz:
            out[int(mm.group(1))].append(off - v)
    return dict(out)


def spelled(tok, name):
    mac = MACRO_OF.get(tok)
    return f'{mac}({name})' if mac else f'XCALL({tok}, {name})'


def include_point(src):
    """Offset just after the first FILE-SCOPE run of #include lines (outside every #if), or 0.
    agent-h run 514: inserting after the last #include in the first 20000 characters put the
    include inside a mid-file `#ifdef NON_MATCHING` block of titproc_uso.c, so the matching build
    never saw it."""
    # comments blanked (same length), so a `#` or `#if` inside a comment does not count
    t = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), src, flags=re.S)
    t = re.sub(r'//[^\n]*', lambda m: ' ' * len(m.group(0)), t)
    depth, pos, end, seen = 0, 0, None, False
    for line in t.splitlines(keepends=True):
        st = line.strip()
        if re.match(r'#\s*if', st):
            depth += 1
        elif re.match(r'#\s*endif', st):
            depth -= 1
        elif depth == 0 and re.match(r'#\s*include\b', st):
            seen, end = True, pos + len(line)
        elif seen and depth == 0 and st and not st.startswith('#'):
            break       # first code after the include run
        pos += len(line)
    return end if end is not None else 0


def add_include(path, src):
    if 'uso_call.h"' in src:
        return src
    if path.endswith('.cpp'):
        line = f'#include "{INCLUDE_REL}"  /* NCC donors build without -I include */\n'
    else:
        line = INCLUDE_LINE + '\n'
    i = include_point(src)
    if i and src[i - 1] != '\n':
        line = '\n' + line
    return src[:i] + line + src[i:]


MACRO_DEF_RE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+(\w+)\(([^)]*)\)((?:[^\n]*\\\n)*[^\n]*)', re.M)


def macro_hidden(src, m, extra=()):
    """{function: Counter(macro name -> uses)} for uses of THIS file's function-like macros whose
    body spells a placeholder call (`#define SEND(x) gl_func_00000000(x)`). scan() sees only the
    macro name at the use, so those jals have no site: the function's site count falls short of
    its jal count and plan refuses the whole function (agent-h run 514: game_createMap, 58 sites
    vs 65 jals). Not expanded: a CALL name would have to go into the shared macro body."""
    tre = tok_re(extra)
    names = {mt.group(1) for mt in MACRO_DEF_RE.finditer(src) if tre.search(mt.group(3))}
    out = collections.defaultdict(collections.Counter)
    if not names:
        return out
    blocks = [b for b in top_blocks(m) if func_header_name(m, b[0], b[1])]
    for mt in re.finditer(r'\b(' + '|'.join(sorted(names)) + r')\s*\(', m):
        blk = next((b for b in blocks if b[1] < mt.start() < b[2]), None)
        if blk:
            out[func_header_name(m, blk[0], blk[1])][mt.group(1)] += 1
    return out


def run_plans(files, rom, mv, verbose):
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as ex:
        return list(ex.map(lambda p: plan_file(p, rom, mv, verbose), files))


def cmd_plan(a, rom, mv):
    tot_named = tot_left = 0
    only = None     # --funcs: name only these (unit, function) pairs' sites (.cpp units: whole file)
    if a.funcs:
        only = {tuple(l.split('\t')[:2]) for l in open(a.funcs) if l.strip() and not l.startswith('#')}
    for res in run_plans(a.files, rom, mv, a.verbose):
        path, sites = res['path'], res['sites']
        raw = [s for s in sites if s.name is None]
        if res['error']:
            print(f'{path}: {len(raw)} raw placeholder sites LEFT: {res["error"]}')
            tot_left += len(raw)
            continue
        named = [s for s in raw if s.i in res['derived']]
        if only is not None and not path.endswith('.cpp'):
            named = [s for s in named if (path, s.func) in only]
        left = [s for s in raw if s not in named]
        if res.get('hidden'):
            hid = res['hidden']
            print(f'{path}: macro-hidden sites (placeholder calls inside function-like macro bodies, not '
                  f'counted as sites): ' + '; '.join(f'{f}: ' + ', '.join(f'{k} x{v}' for k, v in sorted(c.items()))
                                                    for f, c in sorted(hid.items())))
        why = collections.Counter(re.sub(r'^\w+: ', '', res['left'][s.i]) for s in left)
        print(f'{path}: {len(named)} named, {len(left)} left')
        for r, n in why.most_common():
            print(f'    left {n:4d}: {r}')
        if a.verbose:
            for s in left:
                print(f'      {s.func}: {s.tok} -- {res["left"][s.i]}')
            for s in named:
                print(f'      {s.func}: {s.tok} -> {res["derived"][s.i]}')
        tot_named += len(named)
        tot_left += len(left)
        if a.write and named:
            src = open(path, errors='replace').read()
            for s in sorted(named, key=lambda s: s.pos, reverse=True):
                assert src[s.pos:s.end] == s.tok
                src = src[:s.pos] + spelled(s.tok, res['derived'][s.i]) + src[s.end:]
            open(path, 'w').write(add_include(path, src))
    print(f'total: {tot_named} named, {tot_left} left' + ('' if a.write else ' (dry run; --write to apply)'))


def macro_files():
    out = []
    for root, _, fs in os.walk('src'):
        for f in fs:
            if f.endswith(('.c', '.cpp')):
                p = os.path.join(root, f)
                if MAC_RE.search(mask(open(p, errors='replace').read())):
                    out.append(p)
    return sorted(out)


_RENAMES = None


def current_name(name):
    """Follow renames.tsv (OLD<TAB>NEW, chains allowed) from name; None when name is not an old name."""
    global _RENAMES
    if _RENAMES is None:
        _RENAMES = {}
        try:
            for line in open('renames.tsv', errors='replace'):
                f = line.split('#', 1)[0].split()
                if len(f) >= 2 and not line.startswith('#'):
                    _RENAMES[f[0]] = f[1]
        except OSError:
            pass
    seen, n = set(), name
    while n in _RENAMES and n not in seen:
        seen.add(n)
        n = _RENAMES[n]
    return None if n == name else n


def check_fast(path, src, rom, quiet=False):
    """-> (named calls, mismatches) for one file's text."""
    sites, _ = scan(src)
    mac = [s for s in sites if s.name is not None]
    if not mac:
        return 0, 0
    md = rom.module_for(path) or donor_module(path, rom, MakeVars())
    if md is None or not md.funcs:
        print(f'{path}: module not resolvable' + (f' ({md.short}: no function names)' if md else ''))
        return len(mac), len(mac)
    rfuncs, rrel = elf_info(obj_of(path))
    cpp = path.endswith('.cpp')
    if cpp:     # C++ source blocks (members, inlines) are not object functions: per file
        rall = reloc_offsets(obj_of(path))
        live = [f for f in rfuncs if rom.func_off(md, f) is not None and
                rom_bytes_match(md, rom.func_off(md, f), rfuncs[f][2], rfuncs[f][0], rall)]
    want = collections.defaultdict(collections.Counter)
    for s in mac:
        want[(None if cpp else s.func, s.tok)][s.name] += 1
    n = bad = 0
    for (fn, tok), cnt in want.items():
        n += sum(cnt.values())
        have = collections.Counter()
        # a static function whose object symbol a POST_COMPILE add-elf-func-symbol.py injected under a
        # segment-prefixed name (viMgrMain -> gl_viMgrMain, game_libs_ido53_74EF4.c; agent-a run 598)
        fo = fn if fn in rfuncs else ('gl_' + fn) if fn and ('gl_' + fn) in rfuncs else None
        for f in (live if cpp else [fo] if fo else []):
            base = rom.func_off(md, f)
            if base is None:
                continue
            rv, rsz, _ = rfuncs[f]
            for off, sym in rrel.items():
                # C++: any placeholder (a recipe's objcopy may rename func_00000000 -> gl_func_00000000,
                # or a decl may be mangled tok__F...)
                if rv <= off < rv + rsz and (sym == tok or sym.startswith('sect:') or
                                             (cpp and PH_RE.fullmatch(sym))):
                    have[rom.jal_target(md, base + off - rv)] += 1
        over = cnt - have
        if over:
            bad += sum(over.values())
            if not quiet:
                print(f'{path}: {fn}: {dict(over)} not among the ROM callees of its {tok} jals {dict(have)}')
                # a CALL name spelled before a rename landed on main (renamed after the naming run's
                # base, so rename-symbol.py never saw the CALL): point at the current name
                for old in over:
                    cur = current_name(old)
                    if cur:
                        print(f'    {old} was renamed (renames.tsv): use {cur}'
                              + ('' if cur in have else ' (not among the ROM callees either)'))
    return n, bad


def cmd_selftest(a, rom, mv):
    """check must catch a wrong name (fast) and a swapped pair inside one function (--strict)."""
    path = 'src/game_uso/game_uso.c'
    src = open(path).read()
    n, bad = check_fast(path, src, rom, quiet=True)
    assert n > 200 and bad == 0, (n, bad)
    i = src.index('CALL(fs_gdlReadFile)')
    wrong = src[:i] + 'CALL(mem_new)' + src[i + len('CALL(fs_gdlReadFile)'):]
    assert check_fast(path, wrong, rom, quiet=True)[1] == 1
    # the include goes after the first file-scope include run, never into a later #if block
    t = '/* #if x */\n#include "common.h"\n#include "a.h"\nint x;\n#ifdef NON_MATCHING\n#include "b.c"\n#endif\n'
    assert add_include('x.c', t).split('\n')[3] == INCLUDE_LINE
    t = '#define SEND(x) gl_func_00000000(x)\nvoid f(void) { SEND(1); gl_func_00000000(2); }\n'
    assert dict(macro_hidden(t, mask(t))) == {'f': collections.Counter({'SEND': 1})}
    # a pre-rename spelling is flagged and maps back to the current name through renames.tsv
    current_name('')
    back = {v: k for k, v in _RENAMES.items()}
    nm = next(mt.group(2) for mt in MAC_RE.finditer(src) if mt.group(2) in back)
    stale = src.replace(f'CALL({nm})', f'CALL({back[nm]})', 1)
    assert check_fast(path, stale, rom, quiet=True)[1] == 1 and current_name(back[nm]) == nm
    # address tokens (run 520): zero-pinned import placeholders such as game_uso_func_055750 are
    # sites only when passed as extra (and only when undefined in the real object)
    assert 'game_uso_func_055750' in zero_tokens() and 'game_uso_func_00000000' not in zero_tokens()
    ext = addr_tokens(path)
    assert 'game_uso_func_055750' in ext
    t = 'extern int game_uso_func_055750();\nvoid f(void) { game_uso_func_055750(8); }\n'
    assert not scan(t)[0] and [x.tok for x in scan(t, extra=ext)[0]] == ['game_uso_func_055750']
    t = '#define NEW(n) game_uso_func_055750(n)\nvoid f(void) { NEW(1); }\n'
    assert not macro_hidden(t, mask(t)) and dict(macro_hidden(t, mask(t), ext)) == {'f': collections.Counter({'NEW': 1})}
    # an XCALL on an address token is checked like CALL: a wrong name is a mismatch
    xc = next(mt for mt in MAC_RE.finditer(mask(src)) if mt.group(3) == 'game_uso_func_055750')
    assert xc.group(4) == 'mem_new', xc.group(0)
    wrong = src[:xc.start()] + 'XCALL(game_uso_func_055750, Parms_addParm)' + src[xc.end():]
    assert check_fast(path, wrong, rom, quiet=True)[1] == 1
    # cam_reset_000008FC calls three different callees: swap the first two names
    sites, m = scan(src)
    cr = [s for s in sites if s.func == 'cam_reset_000008FC' and s.name]
    x, y = cr[0], cr[1]
    assert x.name != y.name
    sw = src[:x.pos] + spelled(y.tok, y.name) + src[x.end:y.pos] + spelled(x.tok, x.name) + src[y.end:]
    assert check_fast(path, sw, rom, quiet=True)[1] == 0          # same multiset: fast check passes
    real = open(path).read()
    try:
        open(path, 'w').write(sw)
        res = plan_file(path, rom, mv)
    finally:
        open(path, 'w').write(real)
    got = [(s.name, res['derived'].get(s.i)) for s in res['sites'] if s.func == 'cam_reset_000008FC' and s.name]
    assert got[0][0] != got[0][1] and got[1][0] != got[1][1], got   # strict re-derivation catches it
    # and the per-site compile re-derives every XCALL(game_uso_func_055750, mem_new)
    xs = [(s.name, res['derived'].get(s.i)) for s in res['sites'] if s.tok == 'game_uso_func_055750' and s.name]
    assert len(xs) > 50 and all(n == d for n, d in xs), xs[:5]
    print('selftest OK')
    return 0


def cmd_check(a, rom, mv):
    files = a.files or macro_files()
    bad = n = 0
    if a.strict:
        for res in run_plans(files, rom, mv, False):
            for s in res['sites']:
                if s.name is None:
                    continue
                n += 1
                got = res['derived'].get(s.i)
                if got != s.name:
                    bad += 1
                    print(f'{res["path"]}: {s.func}: {spelled(s.tok, s.name)} -- '
                          f'{"ROM callee " + got if got else res["error"] or res["left"].get(s.i)}')
    else:
        for path in files:
            nn, bb = check_fast(path, open(path, errors='replace').read(), rom)
            n += nn
            bad += bb
    print(f'check: {n} named calls, {bad} mismatches')
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('plan'); p.add_argument('files', nargs='*')
    p.add_argument('--addr-token-files', action='store_true',
                   help='add every src file that calls a zero-pinned address token (sorted)')
    p.add_argument('--write', action='store_true'); p.add_argument('-v', '--verbose', action='store_true')
    p.add_argument('--funcs', help='TSV of unit<TAB>function (e.g. port/gen/binds-left.tsv): name only the '
                   'sites inside those C functions (a .cpp unit is named whole)')
    p = sub.add_parser('check'); p.add_argument('files', nargs='*')
    p.add_argument('--strict', action='store_true')
    sub.add_parser('selftest')
    ap.add_argument('--no-build', action='store_true',
                    help='do not make the Yay0 block targets (only check they exist)')
    a = ap.parse_args()
    ensure_build(a.no_build)
    rom, mv = Rom(), MakeVars()
    if a.cmd == 'plan':
        a.files = sorted(set(a.files) | (set(addr_token_files()) if a.addr_token_files else set()))
        if not a.files:
            ap.error('plan: no files (give FILE.. or --addr-token-files)')
    rc = {'plan': cmd_plan, 'check': cmd_check, 'selftest': cmd_selftest}[a.cmd](a, rom, mv)
    shutil.rmtree(SCRATCH, ignore_errors=True) if a.cmd == 'plan' and not a.verbose else None
    sys.exit(rc or 0)


if __name__ == '__main__':
    main()
