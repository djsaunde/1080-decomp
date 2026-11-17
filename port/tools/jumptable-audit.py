#!/usr/bin/env python3
"""jumptable-audit: do the C `case` labels of every switch agree with the ROM's jump table?

Many switches here are matched against a jump table the build does NOT take from the compiled C:
REPLACE_FUNC_BODY donors whose local .rodata reloc is renamed `<func>_rodata` and pinned to the
table the USO ships, USO units whose .rodata is discarded or pinned NOLOAD, the match_imports
ports. The matching build then checks only .text, and .text cannot see which case value goes to
which arm: IDO lays the arms out in source order, so a permutation of the labels compiles to the
same bytes. The PC port's gcc builds its own table from the labels, so a wrong label is a wrong
dispatch there (Node_doCommand, agent-g run 822; the audio commands, agent-c run 829, whose
game_libs-only version of this tool this is the tree-wide successor of; agent-e run 831).

Method (every module, no per-module constants to maintain):
  1. C side. Every build/src/**/*.o: a run of consecutive R_MIPS_32 / GPREL32 relocs in a data
     section that point into .text (split where the code loads a new table) is a compiled jump
     table: index -> arm, function-relative. The case base is read from the `addiu rX,rY,-base`
     before the `sltiu` bound check.
  2. Where the function lives: its .o bytes, reloc fields masked, are searched in the baserom
     (at all four byte alignments: the newer USOs sit at 2 mod 4) and in every decompressed
     Yay0 block.
  3. ROM side. A ROM table word is S + arm offset (S = the function's address in its module's
     text space). Constants per code image are bootstrapped by vote: candidate windows (N words
     whose S-relative values are all arms of the function) give D = S - code offset, and the
     unique top-voted ones give E = table offset - the table address the code itself loads
     (`lui`/`lw`). Every table is then read at its code's own address + E (a module with a single
     table falls back to the plausible window nearest the code).
  4. Compare entry by entry. Any difference = the C labels disagree with the ROM.

Statuses: OK; WRONG (fails); UNRESOLVED (fails unless in UNRESOLVED_OK); PORT-ONLY (the true
labels cannot compile to the ROM bytes; fixed under #ifdef PORT, see PORT_ONLY); SPLICED (a host
unit's stale placeholder table, superseded by its donor); LAYOUT (a layout-only `_lay` function).

Usage: port/tools/jumptable-audit.py [-v|--verbose] [--json OUT]   (project root, after make)
"""
import argparse, collections, glob, json, os, struct, sys
try:
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    import crunch64  # noqa: F401  (the Yay0 blocks)
except ImportError:  # re-exec under a venv that has pyelftools + crunch64
    for _py in ('/home/danjs/code/decomp/tools/build-venv/bin/python3', '/home/danjs/code/decomp/.venv/bin/python'):
        if os.path.exists(_py) and os.path.realpath(sys.executable) != os.path.realpath(_py):
            os.execv(_py, [_py, os.path.abspath(__file__)] + sys.argv[1:])
    sys.exit('jumptable-audit: needs pyelftools + crunch64')

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # port/tools/ -> project root

# Tables that cannot be located in the ROM for a known reason (key: obj path:function).
UNRESOLVED_OK = {
}

# Functions whose correct labels cannot compile to the ROM's bytes, corrected under #ifdef PORT
# only (the matching objects audited here keep the .text-only labels). Key: function name.
PORT_ONLY = {
    # the ROM table never reaches the `case 0x6C: return;` arm (+0x130): with the true labels that
    # arm is dead and IDO drops it (agent-c run 829 / agent-e run 831); NDC_* under PORT (run 822)
    'Node_doCommand': 'NDC_* labels under PORT (src/game_libs/game_libs_o2_38A28.c)',
}


def u32(b, o):
    return struct.unpack_from('>I', b, o)[0]


def obj_tables(path):
    """Yield dicts: func, targets (fn-relative), code (bytes), mask (bytes), base, obj."""
    with open(path, 'rb') as f:
        e = ELFFile(f)
        secs = list(e.iter_sections())
        st = e.get_section_by_name('.symtab')
        if st is None:
            return []
        syms = list(st.iter_symbols())
        relsec = {}
        for rs in secs:
            if isinstance(rs, RelocationSection):
                relsec[rs['sh_info']] = rs
        out = []
        for tidx, rs in relsec.items():
            tsec = secs[tidx]
            if not (tsec.name.startswith('.rodata') or tsec.name.startswith('.data')):
                continue
            data = tsec.data()
            ents = []
            for r in rs.iter_relocations():
                if r['r_info_type'] not in (2, 12):
                    continue
                s = syms[r['r_info_sym']]
                sh = s['st_shndx']
                if not isinstance(sh, int) or not secs[sh].name.startswith('.text'):
                    continue
                add = r['r_addend'] if r.is_RELA() else u32(data, r['r_offset'])
                ents.append((r['r_offset'], sh, s['st_value'] + add))
            ents.sort()
            # table starts = the offsets the code loads from (LO16 against this section)
            starts = set()
            for trs in relsec.values():
                if not secs[trs['sh_info']].name.startswith('.text'):
                    continue
                tdat = secs[trs['sh_info']].data()
                for r in trs.iter_relocations():
                    if r['r_info_type'] == 6 and syms[r['r_info_sym']]['st_shndx'] == tidx:
                        starts.add(struct.unpack_from('>h', tdat, r['r_offset'] + 2)[0] + syms[r['r_info_sym']]['st_value'])
            i = 0
            while i < len(ents):
                j = i
                while (j + 1 < len(ents) and ents[j + 1][0] == ents[j][0] + 4 and ents[j + 1][1] == ents[i][1]
                       and ents[j + 1][0] not in starts):
                    j += 1
                grp = ents[i:j + 1]
                i = j + 1
                shx = grp[0][1]
                fns = sorted((s['st_value'], s['st_size'], s.name) for s in syms
                             if s['st_shndx'] == shx and s.name and s['st_info']['type'] == 'STT_FUNC')
                if not fns:
                    fns = sorted((s['st_value'], 0, s.name) for s in syms
                                 if s['st_shndx'] == shx and s.name and s['st_info']['type'] == 'STT_NOTYPE'
                                 and not s.name.startswith('.') and not s.name.startswith('$'))
                t0 = min(g[2] for g in grp)
                fn = None
                for k, (v, sz, nm) in enumerate(fns):
                    if v <= t0:
                        end = v + sz if sz else (fns[k + 1][0] if k + 1 < len(fns) else len(secs[shx].data()))
                        fn = (v, end, nm)
                if fn is None:
                    continue
                v, end, nm = fn
                tg = [g[2] - v for g in grp]
                if len(set(tg)) < 2 or not all(0 < t < end - v for t in tg):
                    continue  # a pointer array, not a jump table
                text = secs[shx].data()
                code = text[v:end]
                mask = bytearray(b'\xff' * len(code))
                trs = relsec.get(shx)
                lo_load = None
                if trs is not None:
                    for r in trs.iter_relocations():
                        o = r['r_offset'] - v
                        if not 0 <= o < len(code):
                            continue
                        ty = r['r_info_type']
                        if ty == 4:
                            mask[o:o + 4] = b'\xfc\x00\x00\x00'
                        elif ty in (5, 6, 7):
                            mask[o + 2:o + 4] = b'\x00\x00'
                        elif ty == 2:
                            mask[o:o + 4] = b'\x00\x00\x00\x00'
                        rsym = syms[r['r_info_sym']]
                        if ty == 6 and (rsym['st_shndx'] == tidx or (rsym['st_shndx'] == 'SHN_UNDEF'
                                                                     and rsym.name == nm + '_rodata')):
                            imm = struct.unpack_from('>h', text, r['r_offset'] + 2)[0] + syms[r['r_info_sym']]['st_value']
                            if imm == grp[0][0]:
                                lo_load = o
                out.append(dict(obj=os.path.relpath(path, ROOT), func=nm, roff=grp[0][0], targets=tg,
                                code=code, mask=bytes(mask), lo=lo_load, base=case_base(code, lo_load)))
        return out


def case_base(code, lo):
    """Case value of index 0: the `addiu rX,rY,-K` feeding `sltiu at,rX,N` before the table load."""
    if lo is None:
        return None
    for k in range(lo - 4, max(-4, lo - 4 * 14), -4):
        w = u32(code, k)
        if w >> 26 == 0x0B:  # sltiu
            rx = (w >> 21) & 31
            for m in range(k - 4, max(-4, k - 4 * 10), -4):
                x = u32(code, m)
                if x >> 26 == 0x09 and (x >> 16) & 31 == rx and ((x >> 21) & 31) != rx:  # addiu rx, ry, imm
                    return -struct.unpack('>h', code[m + 2:m + 4])[0]
            return 0
    return None


def table_addr(img, co, lo):
    """The table address the code loads: `lui at,HI` ... `lw rX,LO(at)` at code offset lo."""
    lw = u32(img, co + lo)
    base = (lw >> 21) & 31
    lov = struct.unpack('>h', img[co + lo + 2:co + lo + 4])[0]
    for k in range(lo - 4, max(-4, lo - 4 * 40), -4):
        w = u32(img, co + k)
        if w >> 26 == 0x0F and (w >> 16) & 31 == base:
            return ((w & 0xFFFF) << 16) + lov
    return lov


def rom_pos(img, off):
    """Approximate ROM position of an image offset (for the nearest-window fallback)."""
    if img.startswith('yay0@'):
        return int(img[5:], 16)
    return off + (int(img[4:]) if img.startswith('rom@') else 0)


def load_images():
    rom = open(os.path.join(ROOT, 'baserom.z64'), 'rb').read()
    imgs = [('rom', rom)] + [('rom@%d' % k, rom[k:]) for k in (1, 2, 3)]
    try:
        import crunch64
        p = 0
        while True:
            p = rom.find(b'Yay0', p)
            if p < 0:
                break
            try:
                imgs.append(('yay0@%X' % p, bytes(crunch64.yay0.decompress(rom[p:p + 0x200000]))))
            except Exception:
                pass
            p += 4
    except ImportError:
        print('warning: no crunch64; Yay0 blocks not searched', file=sys.stderr)
    return imgs


MAXR = 0x4000


class Image:
    def __init__(self, name, data):
        self.name, self.data = name, data
        n = len(data) // 4
        self.w = struct.unpack('>%dI' % n, data[:n * 4])
        self.runs = []
        s = 0
        w = self.w
        for i in range(1, n + 1):
            if i == n or w[i] == 0 or w[i - 1] == 0 or abs(w[i] - w[i - 1]) > MAXR:
                if i - s >= 2:
                    self.runs.append((s, i))
                s = i

    def windows(self, tg):
        N = len(tg)
        U = sorted(set(tg))
        R = U[-1] - U[0]
        Us = set(U)
        w = self.w
        for s, e in self.runs:
            if e - s < N:
                continue
            for p in range(s, e - N + 1):
                win = w[p:p + N]
                mn = min(win)
                if max(win) - mn != R:
                    continue
                S = mn - U[0]
                if all((x - S) in Us for x in win):
                    yield p * 4, S, [x - S for x in win]

    def find_code(self, code, mask):
        # anchor on the longest run of fully-unmasked aligned words
        best = (0, 0)
        k = 0
        while k < len(code):
            st = k
            while k < len(code) and mask[k:k + 4] == b'\xff\xff\xff\xff':
                k += 4
            if k - st > best[1] - best[0]:
                best = (st, k)
            k += 4
        a, b = best
        if b - a < 12:
            return []
        anchor = code[a:b]
        hits = []
        p = self.data.find(anchor)
        while p >= 0:
            o = p - a
            if o >= 0 and o % 4 == 0 and o + len(code) <= len(self.data):
                seg = self.data[o:o + len(code)]
                if all((seg[i] & mask[i]) == (code[i] & mask[i]) for i in range(len(code))):
                    hits.append(o)
            p = self.data.find(anchor, p + 1)
        return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-v', '--verbose', action='store_true')
    ap.add_argument('--json')
    a = ap.parse_args()
    os.chdir(ROOT)
    tables = []
    for p in sorted(glob.glob('build/src/**/*.o', recursive=True)):
        try:
            tables += obj_tables(p)
        except Exception as ex:
            print('warning: %s: %s' % (p, ex), file=sys.stderr)
    if not tables:
        print('jumptable-audit: no jump tables found (is the tree built?)')
        return 2
    imgs = [Image(n, d) for n, d in load_images()]
    # candidates
    for t in tables:
        t['code_at'] = [(im.name, o) for im in imgs for o in im.find_code(t['code'], t['mask'])]
        t['wins'] = [(im.name, o, S, rw) for im in imgs for (o, S, rw) in im.windows(t['targets'])]
    imgd = {im.name: im for im in imgs}
    # 1. bootstrap: D = S - code offset per (code image, table image), by vote over the
    #    set-membership windows of every table
    votes = collections.Counter()
    for t in tables:
        votes.update({(cn, wn, S - co) for cn, co in t['code_at'] for wn, wo, S, rw in t['wins']})
    # 2. the tables whose window is unique at the top vote fix E = table offset - code's table address
    evotes = collections.Counter()
    for t in tables:
        picks = [(votes[(cn, wn, S - co)], cn, co, wn, wo, S) for cn, co in t['code_at'] for wn, wo, S, rw in t['wins']]
        if not picks or t['lo'] is None:
            continue
        top = max(p[0] for p in picks)
        sel = [p for p in picks if p[0] == top]
        if len(sel) != 1 or (top < 2 and len(picks) > 1):
            continue
        _, cn, co, wn, wo, S = sel[0]
        evotes[(cn, wn, S - co, wo - table_addr(imgd[cn].data, co, t['lo']))] += 1
    consts = collections.defaultdict(list)  # code image -> [(table image, D, E, votes)]
    for (cn, wn, D, E), v in evotes.most_common():
        consts[cn].append((wn, D, E, v))
    if a.verbose:
        for cn, l in sorted(consts.items()):
            print('constants %s: %s' % (cn, ', '.join('%s D=0x%X E=0x%X (%d)' % (wn, D & 0xFFFFFFFF, E & 0xFFFFFFFF, v) for wn, D, E, v in l)))
    # 3. read every table at its code's own address
    res = []
    bad = 0
    for t in tables:
        N = len(t['targets'])
        size = len(t['code'])
        status, rom, where, diff = 'UNRESOLVED', None, '', []
        found = []
        for cn, co in t['code_at']:
            if t['lo'] is None:
                break
            addr = table_addr(imgd[cn].data, co, t['lo'])
            for wn, D, E, v in consts.get(cn, []):
                d = imgd[wn].data
                o = addr + E
                if not 0 <= o <= len(d) - 4 * N:
                    continue
                S = co + D
                rw = [u32(d, o + 4 * k) - S for k in range(N)]
                if all(0 < x < size for x in rw):
                    found.append((tuple(rw), '%s+0x%X (table addr 0x%X, S=0x%X, code %s+0x%X)' % (wn, o, addr, S, cn, co)))
        if not found and t['code_at']:
            # a module with no learned constants (one table): the plausible window nearest the code
            near = []
            for cn, co in t['code_at']:
                for wn, wo, S, rw in t['wins']:
                    if not (0 <= S < 0x200000 or 0x80000000 <= S < 0x80400000):
                        continue
                    dist = abs(rom_pos(wn, wo) - rom_pos(cn, co if cn.startswith('rom') else 0))
                    if dist < 0x40000:
                        near.append((dist, tuple(rw), '%s+0x%X (nearest plausible window, S=0x%X, code %s+0x%X)' % (wn, wo, S, cn, co)))
            if near:
                near.sort()
                found = [near[0][1:]]
        maps = {f[0] for f in found}
        if len(maps) == 1:
            rom = list(maps.pop())
            where = found[0][1]
            status = 'OK' if rom == t['targets'] else 'WRONG'
        else:
            where = ('%d conflicting readings' % len(maps)) if maps else (
                'code found %d, table address %s' % (len(t['code_at']), 'unknown' if t['lo'] is None else 'unread'))
        key = '%s:%s' % (t['obj'], t['func'])
        t['status'] = status
        if status == 'UNRESOLVED' and t['func'].endswith('_lay'):
            status = where = 'LAYOUT'  # a layout-only donor function (places the table; not spliced)
        if status == 'WRONG' and t['func'] in PORT_ONLY:
            status, where = 'PORT-ONLY', PORT_ONLY[t['func']]
        if status == 'WRONG':
            b = t['base'] if t['base'] is not None else 0
            lab = lambda i: ('0x%X' % (b + i)) if t['base'] is not None else ('#%d' % i)
            c = t['targets']
            carm = collections.defaultdict(list)
            for i, x in enumerate(c):
                carm[x].append(lab(i))
            for i, (x, y) in enumerate(zip(c, rom)):
                if x != y:
                    diff.append('case %s: C -> arm +0x%X (C labels %s); ROM -> arm +0x%X (C labels %s)' % (
                        lab(i), x, '/'.join(carm[x]), y, '/'.join(carm.get(y, ['none']))))
            bad += 1
        elif status == 'UNRESOLVED' and key not in UNRESOLVED_OK:
            bad += 1
        res.append(dict(key=key, func=t['func'], status=status, where=where, n=N,
                        base=t['base'], c=t['targets'], rom=rom, diff=diff))
    # a host unit's own stale table for a function a donor was spliced over: the host keeps its
    # placeholder's .rodata, while the code at that place is the donor's (checked via the donor)
    done = {(t['func'], c) for t in tables if t['status'] in ('OK', 'WRONG') for c in t['code_at']}
    for t, r in zip(tables, res):
        if r['status'] == 'UNRESOLVED' and any((t['func'], c) in done for c in t['code_at']):
            r['status'], r['where'] = 'SPLICED', 'host table superseded by the donor'
            bad -= r['key'] not in UNRESOLVED_OK
    for r in res:
        if r['status'] not in ('OK', 'SPLICED', 'LAYOUT', 'PORT-ONLY') or a.verbose:
            print('%-10s %s  [%d entries, base %s] %s' % (r['status'], r['key'], r['n'],
                  '?' if r['base'] is None else '0x%X' % r['base'], r['where']))
            for d in r['diff']:
                print('             ' + d)
    cnt = collections.Counter(r['status'] for r in res)
    print('jumptable-audit: %d jump tables: %s' % (len(res), ', '.join('%s %d' % kv for kv in sorted(cnt.items()))))
    if a.json:
        json.dump(res, open(a.json, 'w'), indent=1)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
