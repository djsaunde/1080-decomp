#!/usr/bin/env python3
"""PC port: ABI cross-check of every direct call in the linked tenshoe (plan 18.17, run 836; gate step
in scripts/port-gate.sh, run 839).

    port/tools/abi-scan.py <tenshoe> [--baseline port/tools/abi-baseline.tsv] [--update]

For each DW_TAG_call_site in a src/ or port/ unit: the caller-side declaration (call_origin)
against the callee's definition (found from the call instruction's target address):
  RET      float / double return against an integer one (st(0) vs eax);
  SRET     struct return by value against a pointer return (the hidden pointer and its `ret $4`);
  SRETPOP  both by value (listed so a change in the callee's pop shows up);
  Pn       parameter n differs in class where the i386 stack layout or the value changes
           (float / double / int64 / struct against anything else);
  UNPROTO  an unprototyped C call whose definition takes a float (promoted to double) or
           returns a float / double.
  VRET     the caller's declaration returns a value, the definition is void, and the caller reads
           eax after the call (run 842: the ROM's $v0 was the callee's last result, which the
           matched C drops; fix the callee with PORT_V0_TYPE / PORT_V0_RET, include/uso_call.h).
           The read is found in the objdump disassembly after the call, following direct jumps.
With --x64 (the PORT_64 build, plan 21.7: x32 units in an x86-64 executable) the i386-only classes are
dropped (SRETPOP; Pn / UNPROTO, which the register check replaces) and one class is added:
  X64REG   the caller loads an argument register (DW_TAG_call_site_parameter) that the definition
           does not read (extra integer registers alone, with every vector argument the definition
           reads loaded, are trailing K&R arguments and pass): an int / float class mismatch (O32 float bits declared `int`), a
           double / float, or a by-value struct the two sides size differently.  The definition's
           registers follow the SysV classes (rdi rsi rdx rcx r8 r9 / xmm0-7; a struct > 16 bytes in
           memory); definitions with a 1..16-byte struct parameter, or variadic, are not judged.
  X64Pn    a prototyped declaration's parameter n is in another class than the definition's (int /
           pointer against float, float against double, a struct against anything else)
  SRET     (kept) only when the struct is <= 16 bytes: a larger one is the hidden pointer in rdi,
           which is the C side's explicit out-pointer.
Prints one line per distinct finding.  With --baseline, exits 1 when a finding is not in the
baseline (the accepted ones: run 836 left int / float same-size pairs etc. as harmless);
--update rewrites the baseline.  Needs pyelftools (tools/build-venv).
"""
import argparse
import sys, collections, struct, re
from elftools.elf.elffile import ELFFile
ap=argparse.ArgumentParser()
ap.add_argument('elf')
ap.add_argument('--baseline')
ap.add_argument('--update',action='store_true')
ap.add_argument('--x64',action='store_true')
args=ap.parse_args()
path=args.elf
f=open(path,'rb'); e=ELFFile(f); d=e.get_dwarf_info()
text=e.get_section_by_name('.text'); tv=text['sh_addr']; tb=text.data()
def tdie(die):
    if 'DW_AT_type' not in die.attributes: return None
    return die.get_DIE_from_attribute('DW_AT_type')
def tclass(t, depth=0):
    if t is None: return 'void'
    tag=t.tag
    if tag in ('DW_TAG_typedef','DW_TAG_const_type','DW_TAG_volatile_type','DW_TAG_restrict_type'):
        return tclass(tdie(t),depth+1)
    if tag in ('DW_TAG_pointer_type','DW_TAG_reference_type','DW_TAG_rvalue_reference_type'): return 'ptr'
    if tag=='DW_TAG_base_type':
        n=t.attributes['DW_AT_name'].value.decode(); sz=t.attributes['DW_AT_byte_size'].value
        if 'float' in n or 'double' in n: return {4:'float',8:'double',12:'ldouble'}.get(sz,'fp%d'%sz)
        return 'int64' if sz==8 else 'int'
    if tag=='DW_TAG_enumeration_type': return 'int'
    if tag in ('DW_TAG_structure_type','DW_TAG_union_type','DW_TAG_class_type'):
        return 'struct%d'%t.attributes.get('DW_AT_byte_size').value if 'DW_AT_byte_size' in t.attributes else 'struct?'
    if tag=='DW_TAG_array_type': return 'ptr'
    return '?'+tag
def sig(die):
    # follow specification / abstract_origin for the name/type
    base=die
    for a in ('DW_AT_specification','DW_AT_abstract_origin'):
        if a in base.attributes:
            base=base.get_DIE_from_attribute(a)
    ret=tclass(tdie(base))
    ps=[]; var=False
    src=die if any(c.tag=='DW_TAG_formal_parameter' for c in die.iter_children()) else base
    for c in src.iter_children():
        if c.tag=='DW_TAG_formal_parameter':
            if 'DW_AT_type' in c.attributes: ps.append(tclass(tdie(c)))
            elif 'DW_AT_abstract_origin' in c.attributes: ps.append(tclass(tdie(c.get_DIE_from_attribute('DW_AT_abstract_origin'))))
        elif c.tag=='DW_TAG_unspecified_parameters': var=True
    proto=bool(base.attributes.get('DW_AT_prototyped') and base.attributes['DW_AT_prototyped'].value)
    name=base.attributes.get('DW_AT_linkage_name') or base.attributes.get('DW_AT_name')
    name=name.value.decode() if name else '?'
    return name, ret, ps, var, proto
defs={}   # addr -> (sig, cu name, lang)
sites=[]
for cu in d.iter_CUs():
    top=cu.get_top_DIE()
    cun=top.attributes['DW_AT_name'].value.decode() if 'DW_AT_name' in top.attributes else '?'
    lang=top.attributes['DW_AT_language'].value if 'DW_AT_language' in top.attributes else 0
    if '/src/' not in cun and '/port/' not in cun: continue
    cpp = lang in (4,0x1a,0x21,0x2b)
    cur_fn=None
    for die in cu.iter_DIEs():
        if die.tag=='DW_TAG_subprogram' and 'DW_AT_low_pc' in die.attributes:
            lo=die.attributes['DW_AT_low_pc'].value
            hi=die.attributes['DW_AT_high_pc'].value
            if die.attributes['DW_AT_high_pc'].form!='DW_FORM_addr': hi=lo+hi
            s=sig(die)
            defs[lo]=(s,cun,cpp,hi)
        elif die.tag in ('DW_TAG_call_site','DW_TAG_GNU_call_site'):
            o=die.attributes.get('DW_AT_call_origin') or die.attributes.get('DW_AT_abstract_origin')
            rp=die.attributes.get('DW_AT_call_return_pc') or die.attributes.get('DW_AT_low_pc')
            if o is None or rp is None: continue
            od=die.get_DIE_from_attribute(o.name)
            # the caller function
            p=die.get_parent()
            while p is not None and p.tag!='DW_TAG_subprogram': p=p.get_parent()
            pn=sig(p)[0] if p is not None else '?'
            regs=[]  # --x64: the argument registers the caller loads (DWARF numbers)
            for c in die.iter_children():
                if c.tag in ('DW_TAG_call_site_parameter','DW_TAG_GNU_call_site_parameter') and 'DW_AT_location' in c.attributes:
                    v=c.attributes['DW_AT_location'].value
                    if isinstance(v,list) and v and 0x50<=v[0]<=0x6f: regs.append(v[0]-0x50)
                    elif isinstance(v,list) and len(v)>1 and v[0]==0x90: regs.append(v[1])
            sites.append((cun,cpp,pn,sig(od),rp.value,od,regs))
def target(rp):
    o=rp-5-tv
    if 0<=o<len(tb)-5 and tb[o]==0xE8:
        return (rp+struct.unpack('<i',tb[o+1:o+5])[0])&0xffffffff
    return None
def pops(lo,hi):
    b=tb[lo-tv:hi-tv]
    return b'\xc2\x04\x00' in b
FP={'float','double','ldouble'}
# ---- VRET: does the caller read eax after the call?  (objdump -d of the linked .text, once)
_ins=None
def _load():
    global _ins
    import subprocess
    out=subprocess.run(['objdump','-d','--no-show-raw-insn','-j','.text',path],capture_output=True,text=True).stdout
    _ins={}
    prev=None
    for ln in out.splitlines():
        m=re.match(r'\s+([0-9a-f]+):\t(\S+)\s*(.*)$',ln)
        if not m: continue
        a=int(m.group(1),16); _ins[a]=[m.group(2),m.group(3),None]
        if prev is not None: _ins[prev][2]=a
        prev=a
EAXR=re.compile(r'%(eax|ax|al|ah)\b')
WRITE=('mov','lea','pop','set','cvt')
def reads_eax(a,depth=0,seen=None):
    """True when eax is read before it is written, from address a (direct jumps followed)."""
    if _ins is None: _load()
    seen=set() if seen is None else seen
    for _ in range(24):
        if a is None or a not in _ins or a in seen: return False
        seen.add(a); op,args,nxt=_ins[a]
        if op.startswith('ret') or op.startswith('call'): return False   # a void callee's leftover / clobbered
        if op.startswith('j'):
            t=re.match(r'([0-9a-f]+) <',args)
            if t is None or depth>4: return False
            t=int(t.group(1),16)
            if op=='jmp': a=t; depth+=1; continue
            return reads_eax(t,depth+1,seen) or reads_eax(nxt,depth+1,seen)
        if op in ('cltd','cwtl'): return True
        if EAXR.search(args):
            ops=re.split(r',(?![^(]*\))',args)
            if op.startswith('xchg') and ops[0]==ops[-1]: a=nxt; continue          # nop
            if op.startswith(('xor','sub','sbb')) and len(ops)==2 and ops[0]==ops[1]: return False
            if any(EAXR.search(o) for o in ops[:-1]) or ('(' in ops[-1] and EAXR.search(ops[-1])): return True
            if op.startswith(WRITE) or (op.startswith('imul') and len(ops)==3): return False
            return True
        a=nxt
    return False
found=set()
X64GP=[5,4,1,2,8,9]  # rdi rsi rdx rcx r8 r9
def x64_regs(ps,ret):
    """the argument registers a definition reads, or None when its classes are not judged"""
    gp=[];xm=[]
    if ret.startswith('struct') and ret[6:].isdigit() and int(ret[6:])>16: gp.append(None)  # hidden pointer
    for p in ps:
        if p in FP: xm.append(p)
        elif p.startswith('struct'):
            if not p[6:].isdigit(): return None
            if int(p[6:])<=16: return None
        else: gp.append(p)
    return set(X64GP[:min(len(gp),6)])|set(range(17,17+min(len(xm),8)))
for cun,cpp,pn,(dn,dret,dps,dvar,dproto),rp,od,regs in sites:
    t=target(rp)
    if t is None or t not in defs: continue
    (n,ret,ps,var,proto),dcun,dcpp,hi=defs[t]
    issues=[]
    if args.x64:
        exp=None if var else x64_regs(ps,ret)
        extra=sorted(set(regs)-exp) if exp is not None else []
        # extra GP registers alone are trailing arguments the definition ignores (K&R calls with more
        # arguments than the callee names): harmless unless a vector argument is missing as well
        if extra and all(r < 17 for r in extra) and not ({r for r in exp if r >= 17} - set(regs)):
            extra=[]
        if extra:
            nm={5:'rdi',4:'rsi',1:'rdx',2:'rcx',8:'r8',9:'r9'}
            issues.append('X64REG caller sets %s, def(%s) does not read it'%(','.join(nm.get(r,'xmm%d'%(r-17)) for r in extra),','.join(ps)))
        unp = (not dproto and not cpp) or (dvar and not dps)
        if not unp:  # a prototyped declaration: the classes position by position
            kc=lambda t:'G' if t in ('int','ptr','int64','struct4','struct8') else t  # a 4 / 8-byte struct: assumed INTEGER class
            for k in range(min(len(dps),len(ps))):
                if kc(dps[k])!=kc(ps[k]):
                    issues.append('X64P%d decl %s def %s'%(k,dps[k],ps[k]))
        sz=lambda t:int(t[6:]) if t.startswith('struct') and t[6:].isdigit() else 0
        if (dret.startswith('struct') != ret.startswith('struct')) and max(sz(dret),sz(ret))<=16:
            issues.append('SRET decl %s def %s'%(dret,ret))
        if ret=='void' and dret not in ('void',) and dret not in FP and reads_eax(rp):
            issues.append('VRET decl %s def void, eax read after the call'%dret)
        if (dret in FP) != (ret in FP) and dret!='void' and ret!='void':
            issues.append('RET decl %s def %s'%(dret,ret))
        for i in issues:
            found.add('%s\t%s\t%s\t%s\t%s\tdecl(%s%s)->%s'%(i.split()[0],re.sub(r'^.*?/(src|port)/',r'\1/',cun),pn,n,i,','.join(dps),',...' if dvar else '',dret))
        continue
    if ret=='void' and dret not in ('void',) and dret not in FP and reads_eax(rp):
        issues.append('VRET decl %s def void, eax read after the call'%dret)
    # return
    if (dret in FP) != (ret in FP) and dret!='void' and ret!='void':
        issues.append('RET decl %s def %s'%(dret,ret))
    if dret.startswith('struct') != ret.startswith('struct'):
        issues.append('SRET decl %s def %s'%(dret,ret))
    elif ret.startswith('struct') and dret.startswith('struct'):
        issues.append('SRETPOP def_pops=%d'%pops(t,hi))
    unp = (not dproto and not cpp) or (dvar and not dps)
    if not unp:
        for k in range(min(len(dps),len(ps))):
            a,b=dps[k],ps[k]
            if a==b: continue
            if (a in FP or b in FP) or a in('int64',) or b in ('int64',) or a.startswith('struct') or b.startswith('struct'):
                issues.append('P%d decl %s def %s'%(k,a,b))
    else:
        if any(p in ('float',) for p in ps) or ret in FP:
            issues.append('UNPROTO def(%s)->%s'%(','.join(ps),ret))
    for i in issues:
        found.add('%s\t%s\t%s\t%s\t%s\tdecl(%s%s)->%s'%(i.split()[0],re.sub(r'^.*?/(src|port)/',r'\1/',cun),pn,n,i,','.join(dps),',...' if dvar else '',dret))
found=sorted(found)
cls=collections.Counter(x.split('\t')[0] if not x.startswith('P') else 'P' for x in found)
if args.update and args.baseline:
    open(args.baseline,'w').write('# port/tools/abi-scan.py --update: accepted ABI findings (class, caller unit, caller, callee, issue, decl)\n'+''.join(x+'\n' for x in found))
    print('abi-scan: baseline written, %d findings' % len(found)); sys.exit(0)
if not args.baseline:
    print('\n'.join(found)); sys.exit(0)
base={l.rstrip('\n') for l in open(args.baseline) if l.strip() and not l.startswith('#')}
new=[x for x in found if x not in base]; gone=[x for x in base if x not in set(found)]
for x in new: print('NEW\t'+x)
print('abi-scan: %d sites checked, %d findings (%s), %d NEW, %d fixed vs baseline' % (len(sites), len(found), ' '.join('%s %d'%kv for kv in sorted(cls.items())), len(new), len(gone)))
sys.exit(1 if new else 0)
