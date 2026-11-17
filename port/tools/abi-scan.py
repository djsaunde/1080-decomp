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
    if tag in ('DW_TAG_typedef','DW_TAG_const_type','DW_TAG_volatile_type'):
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
            sites.append((cun,cpp,pn,sig(od),rp.value,od))
def target(rp):
    o=rp-5-tv
    if 0<=o<len(tb)-5 and tb[o]==0xE8:
        return (rp+struct.unpack('<i',tb[o+1:o+5])[0])&0xffffffff
    return None
def pops(lo,hi):
    b=tb[lo-tv:hi-tv]
    return b'\xc2\x04\x00' in b
FP={'float','double','ldouble'}
found=set()
for cun,cpp,pn,(dn,dret,dps,dvar,dproto),rp,od in sites:
    t=target(rp)
    if t is None or t not in defs: continue
    (n,ret,ps,var,proto),dcun,dcpp,hi=defs[t]
    issues=[]
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
