#!/usr/bin/env python3
"""Automated lever-sweep: try documented levers on NM-wrap candidates, find matches.

Strategy:
  1. Enumerate NM-wrap functions with fuzzy_match < 100 (from report.json)
  2. For each, extract the C body
  3. Generate variants by applying documented levers
  4. Compile each variant via IDO toolchain
  5. Compare against expected/.o for byte-equality
  6. Report best variant and diff count

Usage (in-tree cheap levers -- see the block comment above run_cheap_levers):
  lever-sweep.py --void   <func> [<func> ...]
  lever-sweep.py --blocks <func> [--k 2]
  lever-sweep.py --void --blocks --from-report 90 --units bootup_uso,game_libs_post0b \\
                 --patch-dir /tmp/patches --quiet-patch

Usage (legacy standalone-compile sweep):
  lever-sweep.py [--fuzzy-min FM] [--size-max SZ] [--limit N] [--funcs F1,F2,...]
  lever-sweep.py --funcs gl_func_00037FAC,gl_func_00066514

Output: per-function lines with best lever and diff count.
"""
import argparse, glob, json, os, re, subprocess, sys, tempfile
from pathlib import Path

REPO = Path(__file__).parent.parent.resolve()
IDO = REPO.parent.parent / 'tools/ido-static-recomp/build/7.1/out/cc'
CFLAGS = ['-O2', '-G', '0', '-mips2', '-32', '-non_shared', '-Xcpluscomm', '-Wab,-r4300_mul']
INCLUDES = ['-I', str(REPO/'include'), '-I', str(REPO/'src')]


def find_nm_block(src_text, fname):
    """Find the function definition inside a #ifdef NON_MATCHING ... #endif block.
    Returns (block_start, block_end, fn_start, fn_end, sig_line_text)."""
    # Find all #ifdef NON_MATCHING ... #endif blocks
    lines = src_text.split('\n')
    block_start = None
    for i, line in enumerate(lines):
        if line.strip() == '#ifdef NON_MATCHING':
            block_start = i
        elif line.strip() == '#endif' and block_start is not None:
            block_text = '\n'.join(lines[block_start:i+1])
            if re.search(rf'\b{re.escape(fname)}\s*\(', block_text):
                # Find the actual function definition
                for j in range(block_start, i):
                    if re.match(rf'^[A-Za-z_][\w \*]*\b{re.escape(fname)}\s*\(', lines[j]):
                        # Find matching closing brace
                        depth = 0
                        seen = False
                        for k in range(j, i):
                            depth += lines[k].count('{') - lines[k].count('}')
                            if '{' in lines[k]:
                                seen = True
                            if seen and depth == 0:
                                return (block_start, i, j, k, lines[j])
            block_start = None
    return None


def get_expected_bytes(unit_path, sym):
    """unit_path like 'src/game_libs/game_libs_post' -> read expected/<unit>.c.o."""
    obj = REPO / 'expected' / f'{unit_path}.c.o'
    if not obj.exists():
        return None
    try:
        out = subprocess.check_output(['mips-linux-gnu-objdump', '-t', str(obj)], text=True)
    except:
        return None
    for line in out.split('\n'):
        if sym in line and '.text' in line:
            parts = line.split()
            try:
                addr = int(parts[0], 16)
                size = int(parts[-2], 16)
                raw = subprocess.check_output([
                    'mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text',
                    str(obj), '/dev/stdout'])
                return raw[addr:addr+size]
            except:
                return None
    return None


def compile_test(c_src, fname):
    """Compile c_src to .o and extract fname's bytes. Returns (bytes, error_str)."""
    if not c_src.endswith('\n'):
        c_src += '\n'
    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(c_src)
        cpath = f.name
    opath = cpath.replace('.c', '.o')
    try:
        r = subprocess.run([str(IDO), '-c'] + CFLAGS + INCLUDES +
                          ['-DNON_MATCHING', '-o', opath, cpath],
                          capture_output=True, text=True, timeout=30)
        # IDO emits warnings on stderr but returns 0 if compile succeeds.
        # Check the .o was actually produced.
        if not os.path.exists(opath) or os.path.getsize(opath) == 0:
            return None, r.stderr[:200]
        out = subprocess.check_output(['mips-linux-gnu-objdump', '-t', opath], text=True)
        for line in out.split('\n'):
            if fname in line and '.text' in line:
                parts = line.split()
                addr = int(parts[0], 16)
                size = int(parts[-2], 16)
                raw = subprocess.check_output([
                    'mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text',
                    opath, '/dev/stdout'])
                return raw[addr:addr+size], None
        return None, 'symbol not found'
    finally:
        for p in (cpath, opath):
            if os.path.exists(p): os.unlink(p)


def count_diffs(b1, b2):
    if not b1 or not b2: return 999
    if len(b1) != len(b2): return 999
    import struct
    return sum(1 for i in range(0, len(b1), 4)
               if struct.unpack('>I', b1[i:i+4])[0]
                  != struct.unpack('>I', b2[i:i+4])[0])


def apply_lever_drop_fresh_local(fn_body):
    """Lever: 'T <name> = a0;' as first stmt -> reuse a0 throughout."""
    m = re.search(r'^([\w \*]*\b\w+_func_\w+|\b\w+)\s*\(([^)]*)\)\s*\{(.*)\}\s*$',
                  fn_body, re.DOTALL)
    if not m: return None
    sig = m.group(0)
    # find "T name = a0;" pattern
    inner = m.group(3)
    var_m = re.search(r'^\s*(?:int|void|char|float|register\s+\w+)\s*\*?\s*(\w+)\s*=\s*a0\s*;',
                      inner, re.MULTILINE)
    if not var_m: return None
    var = var_m.group(1)
    if var == 'a0': return None
    # Remove the declaration, replace usages of var with a0
    new_inner = inner.replace(var_m.group(0), '', 1)
    # Replace var as a standalone identifier (word boundary)
    new_inner = re.sub(rf'\b{re.escape(var)}\b', 'a0', new_inner)
    return fn_body[:m.start(3)] + new_inner + fn_body[m.end(3):]


def apply_lever_drop_extra_param(fn_body):
    """Lever: '(int a0, int a1, ...)' with 'a1 = a0;' first stmt -> drop a1 param."""
    m = re.search(r'(\w+_func_\w+|\w+)\s*\(([^)]*)\)\s*\{(.*?)^\}', fn_body, re.DOTALL | re.MULTILINE)
    if not m: return None
    name = m.group(1)
    params = m.group(2)
    body = m.group(3)
    # check first stmt is "aN = a0;"
    first = re.search(r'\s*(\w+)\s*=\s*a0\s*;', body)
    if not first: return None
    extra = first.group(1)
    if not re.match(r'a[1-3]$', extra): return None
    # Drop the extra param from params list
    param_list = [p.strip() for p in params.split(',')]
    new_params = [p for p in param_list if not re.search(rf'\b{re.escape(extra)}\s*$', p)]
    if len(new_params) == len(param_list): return None
    # Remove the assignment + replace extra with a0
    new_body = body.replace(first.group(0), '', 1)
    new_body = re.sub(rf'\b{re.escape(extra)}\b', 'a0', new_body)
    full = fn_body[:m.start(2)] + ', '.join(new_params) + ')' + ' {' + new_body + '}'
    # Reconstruct (rough)
    sig_end = fn_body.find('{', m.start())
    new_sig = re.sub(rf'\(\s*{re.escape(params)}\s*\)', '(' + ', '.join(new_params) + ')',
                     fn_body[:sig_end])
    return new_sig + ' {' + new_body + '}'


def apply_lever_strip_register_kw(fn_body):
    """Remove 'register ' qualifiers from local decls."""
    new = re.sub(r'\bregister\s+', '', fn_body)
    if new == fn_body: return None
    return new


def apply_lever_add_register_kw(fn_body):
    """Add 'register' to the first non-register local in the function body."""
    m = re.search(r'\{[^\n]*\n(\s*)(int|char|void|float|s32|s16|u32|u16)\s+(\*?\s*\w+\s*;)',
                  fn_body)
    if not m: return None
    ws, typ, rest = m.groups()
    new = fn_body[:m.start()] + m.group(0).replace(f'{ws}{typ} {rest}', f'{ws}register {typ} {rest}', 1)
    if new == fn_body: return None
    return new


def apply_lever_swap_first_two_decls(fn_body):
    """Swap the order of first two local declarations."""
    m = re.search(r'(\{[^\n]*\n)((?:\s*(?:int|char|void|float|s\d+|u\d+)\s*\*?\s*\w+(?:\s*=\s*[^;]*)?\s*;\s*\n){2})',
                  fn_body)
    if not m: return None
    decls = re.findall(r'(\s*(?:int|char|void|float|s\d+|u\d+)\s*\*?\s*\w+(?:\s*=\s*[^;]*)?\s*;\s*\n)',
                       m.group(2))
    if len(decls) < 2: return None
    new_decls = decls[1] + decls[0] + ''.join(decls[2:])
    new = fn_body[:m.start(2)] + new_decls + fn_body[m.end(2):]
    if new == fn_body: return None
    return new


def apply_lever_volatile_arg(fn_body):
    """Add volatile read of first arg at function start."""
    # Find first param name
    m = re.match(r'.*?\(([^)]*?)\)\s*\{', fn_body, re.DOTALL)
    if not m: return None
    params = m.group(1).strip().split(',')
    if not params or '*' not in params[0] or not params[0].strip(): return None
    # Skip if already has volatile
    if 'volatile' in fn_body[m.end():m.end()+200]: return None
    body_start = m.end()
    # Get first identifier as param name
    pm = re.search(r'\*?\s*(\w+)\s*$', params[0])
    if not pm: return None
    pname = pm.group(1)
    inject = f'\n    (void)*(volatile char *){pname};\n'
    return fn_body[:body_start] + inject + fn_body[body_start:]


LEVERS = [
    ('drop_fresh_local', apply_lever_drop_fresh_local),
    ('drop_extra_param', apply_lever_drop_extra_param),
    ('strip_register_kw', apply_lever_strip_register_kw),
    ('add_register_kw', apply_lever_add_register_kw),
    ('swap_first_two_decls', apply_lever_swap_first_two_decls),
]


def sweep_function(fname, fuzzy_pct, unit_path):
    """Try each lever on the function, report results."""
    # Find source file
    src_file = None
    src_text = None
    for p in glob.glob(str(REPO/'src/**/*.c'), recursive=True):
        text = open(p).read()
        if find_nm_block(text, fname):
            src_file = p
            src_text = text
            break
    if not src_file:
        return f'NOFILE {fname}'

    # Extract NM block
    block = find_nm_block(src_text, fname)
    if not block:
        return f'NOBLOCK {fname}'
    bs, be, fs, fe, sig_line = block
    fn_body = '\n'.join(src_text.split('\n')[fs:fe+1])

    # Get expected bytes
    expected = get_expected_bytes(unit_path, fname)
    if not expected:
        return f'NOEXP {fname}'

    # Extract extern decls / typedefs immediately preceding the NM block
    lines = src_text.split('\n')
    preamble = []
    # Walk backward from block start, collect extern/typedef/typedef-struct lines
    for j in range(bs-1, max(0, bs-30), -1):
        l = lines[j].strip()
        if l.startswith(('extern ', 'typedef ', 'struct ', '/*', ' *', '*/')) or l == '':
            preamble.insert(0, lines[j])
        elif l.startswith('}') and 'struct' in '\n'.join(preamble[:3] if preamble else ['']):
            preamble.insert(0, lines[j])
        else:
            break
    headers = '\n'.join(preamble)
    # Strip preprocessor directives that won't work standalone
    headers = re.sub(r'#include.*', '', headers)
    headers = re.sub(r'INCLUDE_ASM\([^)]*\);', '', headers)
    # Add minimal stubs
    stub = '''
typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16;
typedef char s8; typedef unsigned char u8; typedef float f32; typedef double f64;
#ifndef NULL
#define NULL ((void*)0)
#endif
extern char D_00000000;
extern int gl_func_00000000();
'''

    # Baseline: compile current body
    test_c = stub + headers + '\n' + fn_body
    base_bytes, err = compile_test(test_c, fname)
    if err:
        return f'NOCOMPILE {fname}: {err[:80]}'
    base_diffs = count_diffs(base_bytes, expected)

    # Try each lever
    results = [('baseline', base_diffs)]
    for lname, lfn in LEVERS:
        variant = lfn(fn_body)
        if variant is None or variant == fn_body:
            continue
        test_c = stub + headers + '\n' + variant
        var_bytes, err = compile_test(test_c, fname)
        if err: continue
        d = count_diffs(var_bytes, expected)
        results.append((lname, d))

    best = min(results, key=lambda x: x[1])
    return f'  {fname:42}  base={base_diffs:3}  best={best[0]:20}={best[1]:3}  fuzzy={fuzzy_pct:.1f}'


# ---------------------------------------------------------------------------
# In-tree cheap-lever sweeps (--void / --blocks)
#
# Unlike the legacy standalone-compile sweep above (which compiles an isolated
# body and can falsely diverge from the full TU), these modes edit the real
# source file, rebuild ONLY the function's build/non_matching .o via make, and
# score with objdiff using the same setting the report uses
# (functionRelocDiffs=none).  The source is always restored (try/finally plus
# SIGINT/SIGTERM handlers, and a backup copy under build/lever-sweep/), the NM
# .o is rebuilt from the restored source, and the best variant is printed as a
# unified diff that `git apply` accepts.
#
#   --void   For every statement-level call whose result is discarded, retarget
#            it to `extern void <callee>_<tag>v(<callee's params>);` -- one site
#            at a time, then each callee's sites together, then pairs of sites.
#            A void callee frees $v0 so IDO stops ring-shifting the post-call
#            registers (docs/IDO_CODEGEN.md, "void callee frees v0").
#   --blocks Insert an emission-free `if (1) {}` after each statement end (one
#            position at a time, K adders per position), then greedily combine
#            improving positions.  Each empty block adds a uopt basic block,
#            which moves the hold-vs-reload threshold (firstUseCost).
#
# Notes: the alias/adder edits only touch the NM body, which is never linked.
# For game_libs_post0b the NM .o is text-clipped (NON_MATCHING_TEXT_CLIP_*); a
# size-changing variant only affects functions near the clip, so early
# functions score correctly, but re-probe the clip before committing.
# ---------------------------------------------------------------------------
import difflib, itertools, signal, shutil, atexit

OBJDIFF_CFG = ['-c', 'functionRelocDiffs=none']
_RESTORE = {}


def _restore_all(*_a):
    for path, text in list(_RESTORE.items()):
        with open(path, 'w') as f:
            f.write(text)
        _RESTORE.pop(path, None)


def _sig(signum, _frame):
    _restore_all()
    sys.exit(128 + signum)


def comment_mask(text):
    """Boolean list: True where text[i] is inside a comment / string / char literal."""
    m = [False] * len(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            j = text.find('\n', i)
            j = n if j < 0 else j
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            j += 1
        else:
            i += 1
            continue
        for k in range(i, min(j, n)):
            m[k] = True
        i = j
    return m


def _in_nm_branch(text, pos):
    """True if pos lies in the true branch of an enclosing `#ifdef NON_MATCHING`."""
    stack = []
    for mm in re.finditer(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|elif|else|endif)\b(.*)$', text[:pos], re.M):
        d, rest = mm.group(1), mm.group(2)
        if d in ('if', 'ifdef', 'ifndef'):
            stack.append(d == 'ifdef' and re.match(r'\s*NON_MATCHING\b', rest) is not None
                         or d == 'if' and re.match(r'\s*defined\s*\(?\s*NON_MATCHING\b', rest) is not None)
        elif d in ('else', 'elif') and stack:
            stack[-1] = False
        elif d == 'endif' and stack:
            stack.pop()
    return any(stack)


def locate_nm_def(text, func):
    """(start, end) of func's definition (ANSI or K&R) inside a NON_MATCHING branch."""
    cm = comment_mask(text)
    pat = re.compile(r'^[A-Za-z_][^\n;{}#=]*?\b' + re.escape(func) + r'\s*\(', re.M)
    for mm in pat.finditer(text):
        if cm[mm.start()]:
            continue
        close = _match_paren(text, mm.end() - 1, cm)
        if close < 0:
            continue
        k = close + 1
        while k < len(text) and (text[k] in ' \t\n' or cm[k]):
            k += 1
        if k >= len(text) or text[k] in ';,=)':
            continue  # prototype / call
        ob = text.find('{', k)
        if ob < 0 or text[k:ob].count('(') != text[k:ob].count(')'):
            continue
        if not _in_nm_branch(text, mm.start()):
            continue
        depth = 0
        for j in range(ob, len(text)):
            if cm[j]:
                continue
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
                if depth == 0:
                    return mm.start(), j + 1
    return None


def load_objdiff_units():
    cfg = json.load(open(REPO / 'objdiff.json'))
    return {u['name']: u for u in cfg['units']}


def unit_of(func, units):
    """Unit name holding func: tracked report.json first, else scan sources."""
    rp = REPO / 'report.json'
    if rp.exists():
        for u in json.load(open(rp))['units']:
            for f in u.get('functions', []):
                if f['name'] == func:
                    return u['name']
    for name, u in units.items():
        sp = REPO / u.get('metadata', {}).get('source_path', '')
        if sp.is_file() and locate_nm_def(sp.read_text(), func):
            return name
    return None


class Scorer:
    def __init__(self, func, unit, units, verbose=False):
        u = units[unit]
        self.func, self.unit = func, unit
        self.src = REPO / u['metadata']['source_path']
        self.obj = REPO / u['base_path']
        self.verbose = verbose
        self.orig = self.src.read_text()
        self.builds = 0

    def score_text(self, text):
        """Write text to the source, rebuild the NM .o, return match % or None."""
        _RESTORE[str(self.src)] = self.orig
        self.src.write_text(text)
        try:
            if self.obj.exists():
                self.obj.unlink()
            r = subprocess.run(['make', str(self.obj.relative_to(REPO))], cwd=REPO,
                               capture_output=True, text=True)
            self.builds += 1
            if r.returncode != 0 or not self.obj.exists():
                if self.verbose:
                    print('    build failed:', (r.stdout + r.stderr)[-300:], file=sys.stderr)
                return None
            r = subprocess.run(['objdiff-cli', 'diff', '-p', '.', '-u', self.unit, '-o', '-',
                                '--format', 'json'] + OBJDIFF_CFG + [self.func],
                               cwd=REPO, capture_output=True, text=True)
            try:
                d = json.loads(r.stdout)
            except ValueError:
                return None
            for s in d.get('left', {}).get('symbols', []):
                if s.get('name') == self.func:
                    return float(s.get('match_percent', 0.0))
            return None
        finally:
            self.src.write_text(self.orig)

    def finish(self):
        """Restore source (already restored) and rebuild the NM .o from it."""
        _restore_all()
        if self.obj.exists():
            self.obj.unlink()
        subprocess.run(['make', str(self.obj.relative_to(REPO))], cwd=REPO, capture_output=True)

    def patch(self, text):
        rel = str(self.src.relative_to(REPO))
        return ''.join(difflib.unified_diff(self.orig.splitlines(True), text.splitlines(True),
                                            'a/' + rel, 'b/' + rel))


C_KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'do', 'else', 'case',
              'goto', 'break', 'continue', 'default'}


def _match_paren(text, i, cm):
    """text[i] == '(' -> index of matching ')'."""
    d = 0
    for j in range(i, len(text)):
        if cm[j]:
            continue
        if text[j] == '(':
            d += 1
        elif text[j] == ')':
            d -= 1
            if d == 0:
                return j
    return -1


def _stmt_start(body, k, cm):
    """True if position k (start of an identifier) begins a statement."""
    j = k - 1
    while j >= 0 and (body[j] in ' \t\n' or cm[j]):
        j -= 1
    if j < 0:
        return False
    if body[j] in ';{}':
        return True
    if body[j] == ':':  # label / case, not ?: (heuristic: no '?' on the line)
        line = body[body.rfind('\n', 0, j) + 1:j]
        return '?' not in line
    if body[j - 3:j + 1] == 'else' or (body[j - 1:j + 1] == 'do' and not body[j - 2:j - 1].isalnum()):
        return True
    if body[j] != ')':
        return False
    d = 0
    while j >= 0:
        if not cm[j]:
            if body[j] == ')':
                d += 1
            elif body[j] == '(':
                d -= 1
                if d == 0:
                    break
        j -= 1
    j -= 1
    while j >= 0 and body[j] in ' \t\n':
        j -= 1
    w = re.search(r'(\w+)$', body[max(0, j - 8):j + 1])
    return bool(w) and w.group(1) in ('if', 'while', 'for')


def _body_brace(body):
    cm = comment_mask(body)
    close = _match_paren(body, body.index('('), cm)
    return body.index('{', close)


def find_void_sites(text, start, end, func):
    """[(abs_pos, callee)] for discarded-result statement calls in text[start:end]."""
    body = text[start:end]
    cm = comment_mask(text)[start:end]
    brace = _body_brace(body)
    sites = []
    for mm in re.finditer(r'\b([A-Za-z_]\w*)\s*\(', body):
        name = mm.group(1)
        k = mm.start(1)
        if k < brace or cm[k] or name in C_KEYWORDS or name == func:
            continue
        if not _stmt_start(body, k, cm):
            continue
        close = _match_paren(body, mm.end() - 1, cm)
        if close < 0:
            continue
        rest = body[close + 1:].lstrip()
        if not rest.startswith(';'):
            continue
        sites.append((start + k, name))
    return sites


def callee_proto(text, callee, before):
    """(is_void, params-string) from the last declaration of callee before `before`."""
    pat = re.compile(r'^[ \t]*(?:extern\s+)?([\w\s\*]+?)\b' + re.escape(callee) +
                     r'\s*\(([^;{)]*)\)\s*;', re.M)
    cm = comment_mask(text[:before])
    best = None
    for mm in pat.finditer(text, 0, before):
        if not cm[mm.start(1)]:
            best = mm
    if not best:
        return False, ''
    ret = best.group(1).strip()
    return re.fullmatch(r'(?:static\s+)?void', ret) is not None, best.group(2).strip()


_HDR_MACROS = None


def function_macros(text):
    """Names of function-like macros defined in text or any include/**/*.h."""
    global _HDR_MACROS
    pat = re.compile(r'^[ \t]*#[ \t]*define[ \t]+(\w+)\(', re.M)
    if _HDR_MACROS is None:
        _HDR_MACROS = set()
        for h in glob.glob(str(REPO / 'include/**/*.h'), recursive=True):
            try:
                _HDR_MACROS.update(pat.findall(open(h, errors='replace').read()))
            except OSError:
                pass
    return _HDR_MACROS | set(pat.findall(text))


def build_void_variant(text, start, sites, chosen, func):
    """Retarget the chosen sites to void aliases; insert alias decls before the def."""
    tag = re.sub(r'^.*?([0-9A-Fa-f]{4,5})$', r'\1', func)[-5:]
    decls, out = {}, text
    for pos, callee in sorted((sites[i] for i in chosen), reverse=True):
        alias = f'{callee}_{tag}v'
        if alias not in decls:
            decls[alias] = callee_proto(text, callee, start)[1]
        out = out[:pos] + alias + out[pos + len(callee):]
    hdr = ''.join(f'extern void {a}({p});\n' for a, p in sorted(decls.items())
                  if not re.search(r'\b' + re.escape(a) + r'\s*\(', text[:start]))
    return out[:start] + hdr + out[start:]


def sweep_void(sc, args):
    text = sc.orig
    loc = locate_nm_def(text, sc.func)
    if not loc:
        print(f'{sc.func}: no NM definition found in {sc.src}')
        return None
    start, end = loc
    macros = function_macros(text)
    sites = [s for s in find_void_sites(text, start, end, sc.func)
             if s[1] not in macros and not callee_proto(text, s[1], start)[0]]
    base = sc.score_text(text)
    print(f'{sc.func} [{sc.unit}] base={base}  void-candidate sites={len(sites)}')
    if base is None or not sites:
        return None
    results = {}

    def tryset(chosen, label):
        chosen = tuple(sorted(set(chosen)))
        if chosen in results:
            return results[chosen][0]
        v = build_void_variant(text, start, sites, chosen, sc.func)
        r = sc.score_text(v)
        results[chosen] = (r, v)
        if args.verbose or (r is not None and r != base):
            print(f'  {label:28s} {r}')
        return r

    for i, (pos, callee) in enumerate(sites):
        ln = text[:pos].count('\n') + 1
        tryset([i], f'site{i} {callee}@L{ln}')
    by_callee = {}
    for i, (_, c) in enumerate(sites):
        by_callee.setdefault(c, []).append(i)
    for c, idx in by_callee.items():
        if len(idx) > 1:
            tryset(idx, f'all {c} ({len(idx)})')
    if len(sites) > 1:
        tryset(range(len(sites)), 'all sites')
    # pairs: every pair when small, else pairs among the non-regressing singles
    if len(sites) <= args.max_pair_sites:
        pool = range(len(sites))
    else:
        singles = sorted(range(len(sites)), key=lambda i: -(results[(i,)][0] or -1))
        pool = [i for i in singles if (results[(i,)][0] or -1) >= base][:args.max_pair_sites]
    for a, b in itertools.combinations(pool, 2):
        tryset([a, b], f'pair site{a}+site{b}')
    return base, results


def find_block_positions(text, start, end):
    body = text[start:end]
    cm = comment_mask(text)[start:end]
    brace = _body_brace(body)
    pos = []
    for mm in re.finditer(r'[;}][ \t]*(?=\n)', body):
        p = mm.start()
        if p <= brace or cm[p] or mm.end() >= len(body) - 1:
            continue
        line = body[body.rfind('\n', 0, p) + 1:p + 1].strip()
        if line.startswith('#'):
            continue
        pos.append(start + mm.start() + 1)
    return pos


def sweep_blocks(sc, args):
    text = sc.orig
    loc = locate_nm_def(text, sc.func)
    if not loc:
        print(f'{sc.func}: no NM definition found in {sc.src}')
        return None
    start, end = loc
    pos = find_block_positions(text, start, end)
    base = sc.score_text(text)
    print(f'{sc.func} [{sc.unit}] base={base}  positions={len(pos)}  k={args.k}')
    if base is None:
        return None
    ins = ' if (1) {}' * args.k
    results = {}

    def variant(chosen):
        out = text
        for p in sorted((pos[i] for i in chosen), reverse=True):
            out = out[:p] + ins + out[p:]
        return out

    def tryset(chosen, label):
        chosen = tuple(sorted(set(chosen)))
        if chosen in results:
            return results[chosen][0]
        v = variant(chosen)
        r = sc.score_text(v)
        results[chosen] = (r, v)
        if args.verbose or (r is not None and r != base):
            print(f'  {label:28s} {r}')
        return r

    for i, p in enumerate(pos):
        ln = text[:p].count('\n') + 1
        snippet = text[text.rfind('\n', 0, p) + 1:p].strip()[:50]
        tryset([i], f'pos{i} L{ln} {snippet}')
    # greedy combination over improving singles
    good = sorted([i for i in range(len(pos)) if (results[(i,)][0] or -1) > base],
                  key=lambda i: -results[(i,)][0])
    if good:
        cur, cur_s = (good[0],), results[(good[0],)][0]
        improved = True
        while improved:
            improved = False
            for i in good:
                if i in cur:
                    continue
                r = tryset(cur + (i,), f'greedy {list(cur + (i,))}')
                if r is not None and r > cur_s:
                    cur, cur_s, improved = tuple(sorted(cur + (i,))), r, True
    return base, results


def run_cheap_levers(args):
    sys.stdout.reconfigure(line_buffering=True)
    for t in ('make', 'objdiff-cli'):
        if not shutil.which(t):
            sys.exit(f'{t} not in PATH (export PATH=<monorepo>/tools/build-venv/bin:$PATH)')
    signal.signal(signal.SIGINT, _sig)
    signal.signal(signal.SIGTERM, _sig)
    atexit.register(_restore_all)
    units = load_objdiff_units()
    funcs = [f for f in args.func if f]
    if args.from_report is not None:
        own = [u.strip() for u in args.units.split(',')] if args.units else None
        rep = json.load(open(REPO / 'report.json'))
        for u in rep['units']:
            if own and not any(u['name'].endswith(o) or ('/' + o + '/') in u['name'] + '/' for o in own):
                continue
            for f in u.get('functions', []):
                fm = f.get('fuzzy_match_percent', 0)
                if args.from_report <= fm < 100.0:
                    funcs.append(f['name'])
    modes = [m for m in ('void', 'blocks') if getattr(args, m)]
    bakdir = REPO / 'build' / 'lever-sweep'
    bakdir.mkdir(parents=True, exist_ok=True)
    summary = []
    for func in funcs:
        unit = args.unit or unit_of(func, units)
        if not unit or unit not in units:
            print(f'{func}: unit not found')
            continue
        sc = Scorer(func, unit, units, args.verbose)
        (bakdir / (sc.src.name + '.orig')).write_text(sc.orig)
        try:
            for mode in modes:
                res = (sweep_void if mode == 'void' else sweep_blocks)(sc, args)
                if not res:
                    continue
                base, results = res
                scored = [(r, k, v) for k, (r, v) in results.items() if r is not None]
                if not scored:
                    continue
                best = max(scored, key=lambda x: (x[0], -len(x[1])))
                gain = best[0] - base
                print(f'BEST {mode} {func}: {base:.4f} -> {best[0]:.4f} ({gain:+.4f}) set={list(best[1])} '
                      f'[{sc.builds} builds]')
                summary.append((func, mode, base, best[0]))
                if gain > args.min_gain:
                    p = sc.patch(best[2])
                    if args.patch_dir:
                        pd = Path(args.patch_dir)
                        pd.mkdir(parents=True, exist_ok=True)
                        (pd / f'{func}.{mode}.patch').write_text(p)
                    if not args.quiet_patch:
                        print(p)
        finally:
            sc.finish()
    if len(summary) > 1:
        print('\nSUMMARY (gains only)')
        for func, mode, b, s in sorted(summary, key=lambda x: x[2] - x[3]):
            if s - b > args.min_gain:
                print(f'  {func:40s} {mode:6s} {b:8.4f} -> {s:8.4f} ({s - b:+.4f})')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--fuzzy-min', type=float, default=80.0)
    ap.add_argument('--fuzzy-max', type=float, default=100.0)
    ap.add_argument('--size-max', type=int, default=300)
    ap.add_argument('--limit', type=int, default=20)
    ap.add_argument('--funcs', type=str, default='')
    # in-tree cheap levers
    ap.add_argument('--void', action='store_true', help='void-callee sweep (in-tree)')
    ap.add_argument('--blocks', action='store_true', help='`if (1) {}` adder sweep (in-tree)')
    ap.add_argument('func', nargs='*', help='function(s) for --void/--blocks')
    ap.add_argument('--unit', help='objdiff unit name (default: looked up)')
    ap.add_argument('--from-report', type=float, metavar='PCT',
                    help='also sweep every report.json fn with PCT <= fuzzy < 100')
    ap.add_argument('--units', help='with --from-report: comma list of unit-name suffixes / dirs')
    ap.add_argument('--k', type=int, default=1, help='--blocks: adders per position')
    ap.add_argument('--max-pair-sites', type=int, default=12,
                    help='--void: try all site pairs when <= N sites, else pairs of top N')
    ap.add_argument('--min-gain', type=float, default=0.0)
    ap.add_argument('--patch-dir', help='write <func>.<mode>.patch files here')
    ap.add_argument('--quiet-patch', action='store_true', help='do not print patches')
    ap.add_argument('-v', '--verbose', action='store_true')
    args = ap.parse_args()
    if args.void or args.blocks:
        os.chdir(REPO)
        run_cheap_levers(args)
        return

    # Generate fresh report
    print('Generating fresh report...', file=sys.stderr)
    subprocess.run(['objdiff-cli', 'report', 'generate', '-o', '/tmp/sweep-report.json'],
                   capture_output=True, cwd=REPO)
    r = json.load(open('/tmp/sweep-report.json'))

    targets = []
    explicit = set(args.funcs.split(',')) if args.funcs else set()
    for u in r['units']:
        for fn in u.get('functions', []):
            name = fn['name']
            fm = fn.get('fuzzy_match_percent', 100)
            sz = int(fn['size'])
            if explicit:
                if name in explicit: targets.append((fm, sz, name, u['name']))
            else:
                if args.fuzzy_min <= fm < args.fuzzy_max and sz <= args.size_max:
                    targets.append((fm, sz, name, u['name']))

    targets.sort(key=lambda x: (-x[0], x[1]))
    if not explicit:
        targets = targets[:args.limit]

    print(f'Testing {len(targets)} candidates...', file=sys.stderr)
    for fm, sz, name, unit in targets:
        try:
            print(sweep_function(name, fm, unit))
        except Exception as e:
            print(f'  ERROR {name}: {e}')


if __name__ == '__main__':
    main()
