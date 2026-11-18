#!/usr/bin/env python3
"""PC port: the calls abi-scan cannot see (plan 21.12, run 874): indirect calls, cast calls, by-value
aggregates and scalar type punning, from the compiler's own view of every call.

abi-scan.py reads DW_TAG_call_site: a DIRECT call against its callee's definition.  It cannot judge
  - an indirect call (a vtable slot, a callback table, a function pointer): DWARF has no type for it;
  - a direct call through a cast (`((void (*)())f)(...)`): DWARF's call_origin is f's own declaration;
  - a K&R call that passes ints where the definition takes a struct by value (the reverb ring helpers,
    plan 15.11): the ints' halves land in the struct's s16 fields in the host's byte order.
This tool gets the caller's side from a GCC plugin (port/tools/icall/callscan.cc, run after the
"optimized" pass): every call's gimple_call_fntype, the argument types as passed and the type the
result is read as.  The callee side is the definition's DWARF.  The targets of indirect calls come from
a probe build (the plugin's `probe` option puts `__icall_probe(site, fn)` before each indirect call,
port/tools/icall/probe.c records the distinct pairs) run on every port/host script and soak seeds.

    icall-scan.py --calls32 DIR --elf32 TENSHOE --edges FILE... [--calls64 DIR --elf64 TENSHOE64]
                  [--baseline port/tools/icall-baseline.tsv] [--update]

Findings (one line each).  The baseline holds the reviewed ones, each with its verdict after `<TAB># `;
--update keeps the verdicts and writes a new finding as UNREVIEWED.  A NEW or UNREVIEWED one fails the step.
  ICALL32 / ICALL64  an observed indirect edge whose argument classes or result class differ between the
                     site and the definition where the ABI makes it matter: i386 a stack slot of another
                     size or offset, a float / double / struct return against an integer one (st0 / eax), a
                     float return the call type does not pop (x87 stack leak), a void definition whose
                     result the site reads (the ROM's $v0 was the callee's last call's: PORT_V0_TYPE);
                     x86-64 the SysV registers of the arguments (an int against a float register: the O32
                     float BITS in an int, run 872's setAlpha; float against double; a small struct's
                     eightbytes against separate arguments)
  CAST32 / CAST64    the same for a direct call whose call type is not the callee's declared type (a cast)
  STRUCT             a call (direct, cast or observed indirect) to a definition that takes or returns a
                     struct / union by value, where the site passes or reads something else at that
                     position: the fields are read in the host's byte order (15.11)
  ARITY              an observed indirect edge that passes fewer arguments than the definition reads
  PUN                a scalar read overlapping a scalar write of another size on the same object (an int
                     parameter or local read as s16 / u8 halves: big-endian offsets)
  HOME               (x86-64) an access through &param beyond the parameter: the O32 home-slot idiom
                     (`*(Pair2 *)&a1` reads a1 and a2), contiguous on i386 but not on x86-64
  RECV               an address-taken definition with a float / double / struct parameter or a float / double /
                     struct result that no observed edge reached (review its slot's sites by hand)
  SITE               an indirect site that passes a float / double / struct, or reads one, and never ran
"""
import argparse, collections, os, re, struct, sys
from elftools.elf.elffile import ELFFile

ap = argparse.ArgumentParser()
ap.add_argument('--calls32', required=True)
ap.add_argument('--elf32', required=True)
ap.add_argument('--edges', nargs='*', default=[])
ap.add_argument('--calls64')
ap.add_argument('--elf64')
ap.add_argument('--baseline')
ap.add_argument('--update', action='store_true')
ap.add_argument('--verbose', action='store_true')
args = ap.parse_args()


def rel(p):
    return re.sub(r'^.*?/(src|port)/', r'\1/', p)


# ---- the definitions (DWARF), in the plugin's class notation: i<n> p f<n> s<n> u<n> t<n> v
def tdie(d):
    return d.get_DIE_from_attribute('DW_AT_type') if 'DW_AT_type' in d.attributes else None


def strip(t):
    while t is not None and t.tag in ('DW_TAG_typedef', 'DW_TAG_const_type', 'DW_TAG_volatile_type',
                                      'DW_TAG_restrict_type'):
        if t.tag == 'DW_TAG_typedef' and t.attributes.get('DW_AT_name') and t.attributes['DW_AT_name'].value == b'port64_bits_t':
            return t
        t = tdie(t)
    return t


def flat(t, base, out, depth=0):
    """the scalar fields of a small aggregate, as the plugin spells them: i2@0,i2@2 (bitfields b@<byte>)"""
    t = strip(t)
    if t is None or depth > 6:
        out.append('?')
        return
    if t.tag in ('DW_TAG_structure_type', 'DW_TAG_class_type', 'DW_TAG_union_type'):
        alts = []
        for m in t.iter_children():
            if m.tag != 'DW_TAG_member' or 'DW_AT_type' not in m.attributes:
                continue
            loc = m.attributes.get('DW_AT_data_member_location')
            off = loc.value if loc is not None and isinstance(loc.value, int) else 0
            if 'DW_AT_bit_size' in m.attributes:
                bo = m.attributes['DW_AT_data_bit_offset'].value // 8 if 'DW_AT_data_bit_offset' in m.attributes else off
                out.append('b@%d' % (base + bo))
                continue
            if t.tag == 'DW_TAG_union_type':
                sub = []
                flat(tdie(m), base + off, sub, depth + 1)
                alts.append(','.join(sub))
            else:
                flat(tdie(m), base + off, out, depth + 1)
        if t.tag == 'DW_TAG_union_type':
            out.append(''.join(a + '|' for a in alts))
        return
    if t.tag == 'DW_TAG_array_type':
        e = strip(tdie(t))
        es = e.attributes['DW_AT_byte_size'].value if e is not None and 'DW_AT_byte_size' in e.attributes else 0
        n = 0
        for sr in t.iter_children():
            if sr.tag == 'DW_TAG_subrange_type':
                if 'DW_AT_count' in sr.attributes:
                    n = sr.attributes['DW_AT_count'].value
                elif 'DW_AT_upper_bound' in sr.attributes:
                    n = sr.attributes['DW_AT_upper_bound'].value + 1
        if not es or not n:
            out.append('?')
            return
        for i in range(min(n, 64)):
            flat(tdie(t), base + i * es, out, depth + 1)
        return
    out.append('%s@%d' % (tcls(t), base))


def tcls(t):
    if t is None:
        return 'v'
    tag = t.tag
    if tag == 'DW_TAG_typedef':
        n = t.attributes.get('DW_AT_name')
        if n and n.value == b'port64_bits_t':
            return 't4'   # a transparent union: passed as its first member (include/uso_call.h)
        return tcls(tdie(t))
    if tag in ('DW_TAG_const_type', 'DW_TAG_volatile_type', 'DW_TAG_restrict_type'):
        return tcls(tdie(t))
    if tag in ('DW_TAG_pointer_type', 'DW_TAG_reference_type', 'DW_TAG_rvalue_reference_type', 'DW_TAG_array_type',
               'DW_TAG_ptr_to_member_type'):
        return 'p'
    sz = t.attributes['DW_AT_byte_size'].value if 'DW_AT_byte_size' in t.attributes else 0
    if tag == 'DW_TAG_base_type':
        n = t.attributes['DW_AT_name'].value.decode()
        return ('f%d' if ('float' in n or 'double' in n) else 'i%d') % sz
    if tag == 'DW_TAG_enumeration_type':
        return 'i%d' % (sz or 4)
    if tag in ('DW_TAG_structure_type', 'DW_TAG_class_type', 'DW_TAG_union_type'):
        r = ('u%d' if tag == 'DW_TAG_union_type' else 's%d') % sz
        if sz <= 128:
            out = []
            flat(t, 0, out)
            r += '{%s}' % ','.join(x for x in out if x)
        return r
    return '?'


def load_defs(path):
    """address -> (name, ret, [params], variadic); name -> address"""
    e = ELFFile(open(path, 'rb'))
    defs = {}
    for cu in e.get_dwarf_info().iter_CUs():
        for die in cu.iter_DIEs():
            if die.tag != 'DW_TAG_subprogram' or 'DW_AT_low_pc' not in die.attributes:
                continue
            base = die
            for a in ('DW_AT_specification', 'DW_AT_abstract_origin'):
                if a in base.attributes:
                    base = base.get_DIE_from_attribute(a)
            src = die if any(c.tag == 'DW_TAG_formal_parameter' for c in die.iter_children()) else base
            ps, var = [], False
            for c in src.iter_children():
                if c.tag == 'DW_TAG_formal_parameter':
                    if 'DW_AT_type' in c.attributes:
                        ps.append(tcls(tdie(c)))
                    elif 'DW_AT_abstract_origin' in c.attributes:
                        ps.append(tcls(tdie(c.get_DIE_from_attribute('DW_AT_abstract_origin'))))
                elif c.tag == 'DW_TAG_unspecified_parameters':
                    var = True
            n = base.attributes.get('DW_AT_linkage_name') or base.attributes.get('DW_AT_name')
            defs[die.attributes['DW_AT_low_pc'].value] = (n.value.decode() if n else '?', tcls(tdie(base)), ps, var)
    syms, addr = {}, {}
    for s in e.get_section_by_name('.symtab').iter_symbols():
        if s['st_value'] and (s['st_info']['type'] == 'STT_FUNC' or s['st_value'] in defs):
            # (STT_NOTYPE: the token aliases of port/gen/aliases.ld)
            if s['st_info']['type'] == 'STT_FUNC':
                syms.setdefault(s['st_value'], s.name)
            addr.setdefault(s.name, s['st_value'])
    # address-taken: a word in the data (vtables, the USO relocation tables, callback tables) or an
    # immediate operand in the code equal to a definition's address
    taken = set()
    for sec in e.iter_sections():
        if sec['sh_type'] != 'SHT_PROGBITS' or not (sec['sh_flags'] & 2):
            continue
        b = sec.data()
        if sec['sh_flags'] & 4:   # code: imm32 operands (mov / push $fn; scan every byte offset)
            for o in range(len(b) - 3):
                v = struct.unpack_from('<I', b, o)[0]
                if v in defs and v in syms:
                    taken.add(v)
        else:
            for o in range(0, len(b) - 3, 4):
                v = struct.unpack_from('<I', b, o)[0]
                if v in defs:
                    taken.add(v)
    return defs, syms, addr, taken


def splitc(t):
    """a comma list whose items may hold {...} field lists"""
    out, depth, cur = [], 0, ''
    for ch in t:
        depth += ch == '{'
        depth -= ch == '}'
        if ch == ',' and depth == 0:
            out.append(cur)
            cur = ''
        else:
            cur += ch
    if cur:
        out.append(cur)
    return out


def load_calls(d):
    sites, puns, aggs, homes = [], [], [], []
    for f in sorted(os.listdir(d)):
        p = os.path.join(d, f)
        if f.endswith('.calls'):
            for ln in open(p, errors='replace'):
                c = ln.rstrip('\n').split('\t')
                if len(c) < 13:
                    continue
                sid, loc, caller, callee, extra, ret, proto, var, params, argv, argx, lhs, ftype = c[:13]
                if callee.startswith('*') and len(callee) > 1:
                    callee = callee[1:]   # an asm label (the CALL / XCALL tokens: include/uso_call.h)
                sites.append(dict(id=sid, loc=loc, unit=rel(loc.rsplit(':', 2)[0]), caller=caller, callee=callee,
                                  extra=extra, ret=ret, proto=proto == 'P', var=var == 'V',
                                  params=splitc(params), args=splitc(argv),
                                  lhs=lhs, ftype=ftype))
        elif f.endswith('.pun'):
            for ln in open(p, errors='replace'):
                c = ln.rstrip('\n').split('\t')
                {'PUN': puns, 'HOME': homes}.get(c[0], aggs).append(c)
    return sites, puns, aggs, homes


FP = ('f4', 'f8', 'f12')


def agg(c):
    return c[:1] in ('s', 'u')


def canon(c):
    return re.sub(r',?\|,?', '|', c)


def size(c):
    m = re.match(r'^[a-z](\d+)', c)
    return int(m.group(1)) if m else (4 if c == 'p' else 0)


def fields(c):
    """an aggregate's flattened fields [(class, offset)], or None (too big / unknown / a union)"""
    m = re.match(r'^[su]\d+\{(.*)\}$', c)
    if not m or '?' in m.group(1) or '|' in m.group(1) or c[0] == 'u':
        return None
    out = []
    for x in m.group(1).split(','):
        k, _, o = x.partition('@')
        if not o.isdigit():
            return None
        out.append((k, int(o)))
    return out or None


def words32(c):
    """the i386 stack words of one argument: W a whole 4-byte scalar (an int / pointer / float: the same bytes
    in either type), D a word of an 8-byte scalar, H a word made of sub-word fields, ? unknown"""
    if not agg(c):
        n = (slot32(c) + 3) // 4
        return ['W'] if n == 1 else ['D%s%d' % (c, i) for i in range(n)]
    fl = fields(c)
    n = (size(c) + 3) // 4
    if fl is None:
        return [canon(c) + '#%d' % i for i in range(n)]
    out = []
    for w in range(n):
        inw = [(k, o) for k, o in fl if w * 4 <= o < w * 4 + 4]
        if len(inw) == 1 and inw[0][1] == w * 4 and size(inw[0][0]) == 4 and inw[0][0][0] in 'ipft':
            out.append('W')
        elif len(inw) == 1 and inw[0][1] == w * 4 and size(inw[0][0]) == 8:
            out.append('D%s0' % inw[0][0])
        else:
            out.append('H' + ','.join('%s@%d' % (k, o - w * 4) for k, o in inw))
    return out


def slot32(c):
    """i386 cdecl: the stack bytes an argument of this class takes"""
    if c in ('i8', 'f8'):
        return 8
    if c == 'f12':
        return 12
    if agg(c):
        return (size(c) + 3) & ~3
    return 4


def pieces64(c):
    """the SysV x86-64 registers one argument takes: G (integer), X4 / X8 / X44 (SSE: a float, a double, two
    floats), M (memory)"""
    if not agg(c):
        if c in ('f4', 'f8'):
            return ['X' + c[1:]]
        if c == 'f12':
            return ['M']
        return ['G']
    fl = fields(c)
    sz = size(c)
    if sz > 16:
        return ['M']
    if fl is None:
        return ['?' + canon(c)]
    out = []
    for e in range((sz + 7) // 8):
        inside = [(k, o) for k, o in fl if e * 8 <= o < e * 8 + 8]
        if inside and all(k in ('f4', 'f8') for k, _ in inside):
            out.append('X' + ''.join(k[1:] for k, _ in inside))
        else:
            out.append('G')
    return out


def ret32(c):
    if c == 'v':
        return 'v'
    if c in FP:
        return 'st0'
    if agg(c):
        return 'sret'
    return 'edx:eax' if c == 'i8' else 'eax'


def ret64(c):
    if c == 'v':
        return 'v'
    p = pieces64(c)
    return 'sret' if p == ['M'] else '/'.join(p)


def compare(site, d, x64):
    """issues of a call (site: what the caller passes / reads) against a definition d"""
    name, dret, dps, dvar = d
    issues = []
    passed = site['args']
    if len(passed) < len(dps) and not dvar:
        issues.append(('ARITY', 'passes %d args, def reads %d' % (len(passed), len(dps))))
    n = min(len(passed), len(dps))
    # by-value aggregates: a word of sub-word fields against a word passed whole (or another field layout) is
    # read in the host byte order (15.11); the same struct on both sides is the same layout (one compiler)
    wa = [w for c in passed[:n] for w in words32(c)]
    wd = [w for c in dps[:n] for w in words32(c)]
    if any(agg(c) for c in passed[:n] + dps[:n]):
        for k, (x, y) in enumerate(zip(wa, wd)):
            if x != y and (x[0] in 'H?' or y[0] in 'H?' or x.startswith('s') or y.startswith('s')):
                issues.append(('STRUCT', 'stack word %d: passes %s, def takes %s (args %s / def %s)' % (
                    k, x, y, ','.join(passed[:n]), ','.join(dps[:n]))))
                break
    lhs = site['lhs']
    if lhs != '-' and (agg(lhs) or agg(dret)) and canon(lhs) != canon(dret):
        issues.append(('STRUCT', 'result read as %s, def returns %s' % (lhs, dret)))
    if x64:
        pa = [p for c in passed[:n] for p in pieces64(c)]
        pd = [p for c in dps[:n] for p in pieces64(c)]
        if pa != pd:
            k = next((i for i, (x, y) in enumerate(zip(pa, pd)) if x != y), min(len(pa), len(pd)))
            issues.append(('ABI', 'x86-64 registers: passes %s, def reads %s (args %s / def %s)' % (
                ' '.join(pa), ' '.join(pd), ','.join(passed[:n]), ','.join(dps[:n]))))
        if lhs != '-' and dret != 'v' and ret64(lhs) != ret64(dret):
            issues.append(('ABI', 'x86-64 result: read as %s (%s), def returns %s (%s)' % (lhs, ret64(lhs), dret, ret64(dret))))
    else:
        for k in range(n):
            if k == n - 1 and agg(passed[k]) and agg(dps[k]) and slot32(passed[k]) > slot32(dps[k]):
                break   # the definition reads a prefix of the caller's (larger) struct: the same bytes
            if slot32(passed[k]) != slot32(dps[k]):
                issues.append(('ABI', 'i386 arg%d: passes %s (%d bytes), def takes %s (%d): the later stack slots shift' % (
                    k, passed[k], slot32(passed[k]), dps[k], slot32(dps[k]))))
                break
        if dret in FP and site['ret'] not in FP and lhs == '-':
            # the callee leaves its result in st0; a call typed void / int never pops it: after 8 such calls the
            # x87 stack overflows and every later float return reads a NaN
            issues.append(('ABI', 'i386 result: def returns %s in st0, the call type returns %s (x87 stack leak)' % (dret, site['ret'])))
        if lhs != '-' and dret != 'v' and ret32(lhs) != ret32(dret):
            issues.append(('ABI', 'i386 result: read as %s (%s), def returns %s (%s)' % (lhs, ret32(lhs), dret, ret32(dret))))
    if lhs != '-' and dret == 'v':
        issues.append(('ABI', 'result read as %s, def is void' % lhs))
    return issues


defs32, syms32, addr32, taken32 = load_defs(args.elf32)
sites32, puns, aggs, homes = load_calls(args.calls32)
byid32 = {s['id']: s for s in sites32 if s['callee'] == '*'}
if args.calls64:
    defs64, syms64, addr64, _ = load_defs(args.elf64)
    sites64, _, _, homes64 = load_calls(args.calls64)
    byid64 = {s['id']: s for s in sites64 if s['callee'] == '*'}
    byloc64 = collections.defaultdict(list)
    for s in sites64:
        byloc64[(s['loc'], s['callee'])].append(s)

found = set()


def add(cls, site, callee, text):
    key = '%s\t%s\t%s\t%s\t%s' % (cls, site['unit'], site['caller'], callee, text)
    if args.verbose:
        key += '\t' + site['loc'] + '\t' + site['extra'][:120]
    found.add(key)


def sig(d):
    return '%s(%s%s)' % (d[1], ','.join(d[2]), ',...' if d[3] else '')


# ---- indirect calls: the observed edges
edges = collections.defaultdict(set)
for f in args.edges:
    for ln in open(f):
        p = ln.split()
        if len(p) == 2:
            edges[p[0]].add(int(p[1], 16))
reached = set()
for sid, tgts in edges.items():
    s = byid32.get(sid)
    if s is None:
        continue
    for t in tgts:
        d = defs32.get(t)
        if d is None:
            continue   # a libc / host function without DWARF
        reached.add(t)
        tn = d[0]
        for cls, txt in compare(s, d, False):
            add({'ABI': 'ICALL32'}.get(cls, cls), s, tn, txt + '; def ' + sig(d))
        if args.calls64:
            s64 = byid64.get(sid)
            a64 = addr64.get(syms32.get(t, tn)) or addr64.get(tn)
            d64 = defs64.get(a64) if a64 else None
            if s64 is None or d64 is None:
                add('ICALL64', s, tn, 'not judged: %s' % ('no x86-64 site' if s64 is None else 'no x86-64 definition'))
                continue
            for cls, txt in compare(s64, d64, True):
                if cls == 'ABI':
                    add('ICALL64', s, tn, txt + '; def ' + sig(d64))
                elif cls == 'STRUCT' and txt not in {x for c, x in compare(s, d, False)}:
                    add('STRUCT', s, tn, txt + ' (x86-64)')

# ---- direct calls: a cast call type, and by-value aggregates against K&R / other arguments
def direct(sites, defs, addr, x64):
    for s in sites:
        if s['callee'] == '*':
            continue
        a = addr.get(s['callee'])
        d = defs.get(a) if a else None
        if d is None:
            continue
        cast = not s['extra'].startswith(s['ret'] + ('(' if s['proto'] else 'K(')) or \
            s['extra'] != '%s%s(%s%s)' % (s['ret'], '' if s['proto'] else 'K', ','.join(s['params']), ',...' if s['var'] else '')
        for cls, txt in compare(s, d, x64):
            if cls == 'STRUCT' and txt.startswith('result'):
                continue   # a struct result against a pointer one: abi-scan's SRET / SRETPOP
            if cls == 'STRUCT':
                add('STRUCT', s, d[0], txt + ('' if not x64 else ' (x86-64)') + '; def ' + sig(d))
            elif cls == 'ABI' and cast:
                add('CAST64' if x64 else 'CAST32', s, d[0], txt + '; call type ' + s['ftype'] + ', decl ' + s['extra'] + '; def ' + sig(d))


direct(sites32, defs32, addr32, False)
if args.calls64:
    direct(sites64, defs64, addr64, True)
    # x86-64-only STRUCT lines duplicate the i386 ones when both builds agree
    for k in [k for k in found if k.startswith('STRUCT') and k.split('\t')[4].split('; def')[0].endswith(' (x86-64)')]:
        alt = k.replace(' (x86-64)', '')
        if any(x.startswith(alt.split('; def')[0]) for x in found if x is not k and not x.endswith('(x86-64)')):
            found.discard(k)

# ---- scalar type punning
for c in puns:
    _, loc, fn, var, kind, ty, w, r = c[:8]
    unit = rel(loc.rsplit(':', 1)[0])
    found.add('PUN\t%s\t%s\t%s\t%s %s %s: written %s, read %s' % (
        unit, fn, var, kind, var, ty, re.sub(r':\d+$', '', w[1:]), re.sub(r':\d+$', '', r[1:])))

# ---- an access through &param beyond the parameter: the O32 home-slot idiom (x86-64 spills register args anywhere)
# (x86-64 only: i386 cdecl keeps the arguments contiguous on the stack, as the O32 homes are)
for c in (homes64 if args.calls64 else []):
    _, loc, fn, var, _, ty, rw, acc = c[:8]
    found.add('HOME\t%s\t%s\t%s\t%s %s through &%s, a %s parameter (x86-64 homes it alone)' % (
        rel(loc.rsplit(':', 1)[0]), fn, var, rw, acc, var, ty))

# ---- coverage: address-taken float / struct receivers no edge reached, FP / struct sites that never ran
for t in sorted(taken32):
    d = defs32.get(t)
    if d is None or t in reached:
        continue
    if any(p in FP or agg(p) for p in d[2]) or d[1] in FP or agg(d[1]):
        found.add('RECV\t-\t-\t%s\taddress-taken, %s, no observed indirect edge' % (d[0], sig(d)))
ran = set(edges)
for s in byid32.values():
    if s['id'] in ran:
        continue
    if any(a in FP or agg(a) for a in s['args']) or (s['lhs'] != '-' and (s['lhs'] in FP or agg(s['lhs']))):
        add('SITE', s, '*', 'never ran: passes (%s) -> %s via %s' % (','.join(s['args']), s['lhs'], s['ftype']))

found = sorted(found)
cls = collections.Counter(x.split('\t')[0] for x in found)
summary = '%d indirect sites (%d ran, %d edges), %d calls' % (len(byid32), len(ran & set(byid32)), sum(len(v) for v in edges.values()), len(sites32))
def read_baseline(path):
    """finding -> verdict (each baseline line is `finding<TAB># verdict`: why it is harmless, or how it was fixed)"""
    out = {}
    if path and os.path.exists(path):
        for l in open(path):
            if l.strip() and not l.startswith('#'):
                f, _, v = l.rstrip('\n').partition('\t# ')
                out[f] = v
    return out


if args.update and args.baseline:
    old = read_baseline(args.baseline)
    open(args.baseline, 'w').write(
        '# port/tools/icall-scan.py --update: the reviewed findings (class, caller unit, caller, callee, issue) and\n'
        '# after `<TAB># ` the verdict; plan 21.12.  A new finding is written as UNREVIEWED: review it, fix it or write why\n'
        '# it is harmless.\n'
        + ''.join('%s\t# %s\n' % (x, old.get(x, 'UNREVIEWED')) for x in found))
    print('icall-scan: baseline written, %d findings (%d unreviewed); %s' % (
        len(found), sum(1 for x in found if x not in old), summary))
    sys.exit(0)
if not args.baseline:
    print('\n'.join(found))
    print('icall-scan: %s; %d findings (%s)' % (summary, len(found), ' '.join('%s %d' % kv for kv in sorted(cls.items()))))
    sys.exit(0)
base = read_baseline(args.baseline)
new = [x for x in found if x not in base]
gone = [x for x in base if x not in set(found)]
unrev = [x for x in found if base.get(x, '').startswith('UNREVIEWED')]
for x in new:
    print('NEW\t' + x)
for x in gone:
    print('GONE\t' + x)
for x in unrev:
    print('UNREVIEWED\t' + x)
print('icall-scan: %s; %d findings (%s), %d NEW, %d unreviewed, %d fixed vs baseline' % (
    summary, len(found), ' '.join('%s %d' % kv for kv in sorted(cls.items())), len(new), len(unrev), len(gone)))
sys.exit(1 if new or unrev else 0)
