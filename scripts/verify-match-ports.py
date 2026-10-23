#!/usr/bin/env python3
"""Reproduce the isolated upstream port gates without fetching or publishing.

Requires the project's original toolchain, objdiff-cli, pyelftools, crunch64,
and a local baserom. Run: python3 scripts/verify-match-ports.py --build
Original host definitions retain compiler context; mandatory C donor builds
and independent native proofs are required, so ASM context cannot fake a gain.
"""
from pathlib import Path
import argparse,hashlib,json,io,re,subprocess,struct,collections
from elftools.elf.elffile import ELFFile
import crunch64
from uso_original import modules,u32
from uso_native_audit import native,require,sha,identity_audit,data_audit,original_words
from validate_episode_schema import validate_episode

def command(root,out,args,log):
 with (out/log).open('w') as f:p=subprocess.run(args,cwd=root,stdout=f,stderr=subprocess.STDOUT)
 require(p.returncode==0,'Required command failed; see '+str(out/log))

def verify_bindings(root,row,mods,mode):
 file=row['binding_'+mode];m=json.loads((root/file).read_text());linked,rels=native(root/row['linked_'+mode],row['function']);bound,br=native(root/row['bound_'+mode],row['function']);require(linked==bound,'carrier binding edited instructions');require(hashlib.sha256(linked).hexdigest()==m['raw_sha256'],'linked carrier source hash differs');original,start,_=original_words(root,row,mods);values={};pending=collections.defaultdict(list);last={}
 def bind(old,value):
  new=m['renames'][old];value&=0xffffffff;require(new not in values or values[new]==value,'ambiguous original carrier');values[new]=value
 for z in sorted(rels,key=lambda x:x['offset']):
  off=z['offset'];a=u32(linked,off);b=u32(original,off);old=z['symbol'];kind=z['kind']
  if kind==4:bind(old,(((start+off+4)&0xf0000000)|((b&0x3ffffff)<<2))-((a&0x3ffffff)<<2))
  elif kind==5:pending[old].append((off,a,b))
  elif kind==6:
   highs=pending.pop(old,[]) or ([last[old]] if old in last else []);require(bool(highs),'unpaired native HI/LO')
   for hi,ha,hb in highs:
    sx=lambda x:x-65536 if x&32768 else x
    bind(old,((hb&65535)<<16)+sx(b&65535)-((ha&65535)<<16)-sx(a&65535));last[old]=(hi,ha,hb)
  else:raise ValueError('Unsupported native carrier relocation')
 require(values==m['original_ROM_minus_curated_addend_bindings'],'forged original-ROM carrier value');syms={q[1]:int(q[2],0) for q in re.finditer(r'^\s*(\w+)\s*=\s*(0x[0-9a-fA-F]+|\d+)\s*;', (root/'undefined_syms_auto.txt').read_text(),re.M)};require(all(syms.get(k)==v for k,v in values.items()),'linker carrier value differs');require([(x['offset'],x['kind'],m['renames'][x['symbol']]) for x in rels]==[(x['offset'],x['kind'],x['symbol']) for x in br],'carrier relocation sites/identities differ')

def generated_data(root,row,mods,mode):
 refs=[x for x in row['data_proof'] if x['alias'] in ['.rodata','.rdata']]
 if not refs:return []
 e=ELFFile(io.BytesIO((root/row['native_normal' if mode=='normal' else 'native_nm']).read_bytes()));tab=e.get_section_by_name('.symtab');f=tab.get_symbol_by_name(row['function'])[0];ro=e.get_section_by_name('.rodata');rel=e.get_section_by_name('.rel.rodata');data=ro.data();original=mods[row['module']]['rodata'];result=[];text,_=native(root/row['native_normal' if mode=='normal' else 'native_nm'],row['function'])
 if rel:
  entries=sorted(rel.iter_relocations(),key=lambda x:x['r_offset']);out=bytearray(data[:4*len(entries)]);base=refs[0]['original_addend']-refs[0]['native_addend'];start=row['original_offset']-mods[row['module']].get('curated_text_bias',0)
  for i,z in enumerate(entries):
   require(z['r_offset']==4*i and z['r_info_type']==2,'generated table relocation changed');s=tab.get_symbol(z['r_info_sym']);require(s['st_shndx']==f['st_shndx'],'generated label outside Text');delta=s['st_value']+u32(data,4*i)-f['st_value'];require(0<=delta<row['size'],'generated table target outside complete function');struct.pack_into('>I',out,4*i,start+delta)
  require(bytes(out)==original[base:base+len(out)],'generated switch table disagrees with original ROM');result.append({'function':row['function'],'bytes':len(out),'table_entries':len(entries)})
 else:
  for z in refs:
   opcode=u32(text,z['lo'])>>26;size=8 if opcode==53 else 4 if opcode in [35,49] else None;require(size is not None,'unreviewed generated constant load');a=z['native_addend'];b=z['original_addend'];require(data[a:a+size]==original[b:b+size] and len(data[a:a+size])==size,'generated constant disagrees with original ROM');result.append({'function':row['function'],'bytes':size,'constant':True})
 return result

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,default=Path('.'));p.add_argument('--build',action='store_true');p.add_argument('--without-episodes',action='store_true');a=p.parse_args();root=a.root.resolve();out=root/'build/match-port-verification';out.mkdir(parents=True,exist_ok=True);manifest=json.loads((root/'scripts/match-imports.json').read_text());base=json.loads((root/'scripts/match-upstream-baseline.json').read_text())
 for file,digest in {**manifest['protected_original_inputs'],**manifest['additional_original_compiler_context']}.items():require(sha(root/file)==digest,'protected original input changed '+file)
 if a.build:
  command(root,out,['make','-j2','objects','non_matching_objects'],'objects.log');command(root,out,['make','-j2','verify','verify-blocks'],'ROM-blocks.log');command(root,out,['objdiff-cli','report','generate','-p',str(root),'-o',str(out/'report.json')],'report.log')
 else:require((out/'report.json').exists(),'Run --build once to obtain fresh report evidence')
 report=json.loads((out/'report.json').read_text());require(int(report['measures']['total_code'])==int(base['measures']['total_code'])==762872,'denominator changed');require((root/'tenshoe.z64').read_bytes()==(root/'baserom.z64').read_bytes(),'ROM differs');blocks=[]
 for module,block in [('mgrproc_uso',1),('game_uso',1),('timproc_uso',1),('timproc_uso',3),('timproc_uso',5),('map4_data_uso',2)]:
  raw=crunch64.yay0.decompress((root/f'build/assets/{module}_block{block}_yay0.bin').read_bytes());original=(root/f'assets/{module}_block_{block}.bin').read_bytes();require(raw==original,'strict compressed-block payload differs '+module+str(block));blocks.append({'module':module,'block':block,'strict_equal':True,'bytes':len(raw)})
 mods=modules(root);cfg={u['name']:u for u in json.loads((root/'objdiff.json').read_text())['units']};data_spans=[];mf=(root/'Makefile').read_text()
 for row in manifest['functions']:
  fn=row['function'];require(sha(root/row['source'])==row['source_sha256'],'reviewed source changed '+fn)
  if row['context']:require(sha(root/row['context'])==row['context_sha256'],'reviewed type context changed '+fn)
  require(not re.search(r'INCLUDE_ASM\([^;]*\b'+re.escape(fn)+r'\b',(root/row['source']).read_text()),'target C implementation has ASM fallback');target,_=native(root/cfg[row['unit']]['target_path'],fn)
  for mode,path in [('normal','build/'+row['host']+'.o'),('nm',cfg[row['unit']]['base_path'])]:
   require(native(root/path,fn)[0]==target,'host body not raw-exact '+fn);require(native(root/row['bound_'+mode],fn)[0]==target,'native C donor not raw-exact '+fn);require(row['function']+'='+row['bound_'+mode] in mf,'mandatory C replacement route missing');require(row['bound_'+mode] in mf,'C donor dependency missing');verify_bindings(root,row,mods,mode);data_spans.extend(generated_data(root,row,mods,mode))
  require(identity_audit(root,row,mods)==row['native_proof']==identity_audit(root,row,mods,'non_matching'),'native call identity proof changed '+fn);require(data_audit(root,row,mods)==row['data_proof']==data_audit(root,row,mods,'non_matching'),'native data identity proof changed '+fn)
  if not a.without_episodes:
   ep=root/'episodes'/(fn+'.json');require(not validate_episode(ep,require_match=True),'canonical episode schema rejected '+fn);md=json.loads(ep.read_text())['metadata'];require(md['implementation_path']==row['source'] and md['implementation_sha256']==row['source_sha256'],'episode source provenance differs '+fn);require(md['source_sha256']==sha(root/row['host']),'episode host provenance differs '+fn);require(md['native_call_proof']==row['native_proof'] and md['native_data_proof']==row['data_proof'],'episode original USO proof differs '+fn)
 for fix in manifest.get('source_fixes',[]):
  require(sha(root/fix['source'])==fix['source_sha256'],'reviewed source correction changed');data_spans.extend(generated_data(root,fix,mods,'normal'))
 cache={}
 def hashes(path):
  if path in cache:return cache[path]
  e=ELFFile(io.BytesIO(path.read_bytes()));s=e.get_section_by_name('.symtab');ti=e.get_section_index('.text');result={}
  if ti is not None and s:
   h=e.get_section(ti);d=h.data()
   for f in s.iter_symbols():
    off=f['st_value']-h['sh_addr'];size=f['st_size']
    if f['st_shndx']==ti and size and off>=0 and off+size<=len(d):result.setdefault(f.name,hashlib.sha256(d[off:off+size]).hexdigest())
  cache[path]=result;return result
 now={(u['name'],f['name']):f for u in report['units'] for f in u.get('functions',[])};exact=[];raw=[];partial=[];gained=[]
 for u in base['units']:
  n=hashes(root/'build'/(u['host']+'.o'));nm=hashes(root/u['nm'])
  for f in u['functions']:
   after=now[(u['name'],f['name'])].get('fuzzy_match_percent',0)
   if f['fuzzy']==100 and after!=100:exact.append(f['name'])
   if f['expected_sha256'] and f['normal_sha256']==f['nm_sha256']==f['expected_sha256'] and (n.get(f['name'])!=f['expected_sha256'] or nm.get(f['name'])!=f['expected_sha256']):raw.append(f['name'])
   if 0<f['fuzzy']<100 and after+1e-6<f['fuzzy']:partial.append(f['name'])
   if f['fuzzy']!=100 and after==100:gained.append(f['name'])
 require(not exact and not raw and not partial,'previous exact/raw/partial regression');require(set(gained)=={x['function'] for x in manifest['functions']},'reported gain set differs from audited source functions');gain=sum(x['size'] for x in manifest['functions']);require(int(report['measures']['matched_code'])-int(base['measures']['matched_code'])==gain,'byte gain disagrees with accepted source set')
 result={'passed':True,'accepted_under_local_workflow':not a.without_episodes,'upstream_pin':base['upstream'],'baseline_matched_code':int(base['measures']['matched_code']),'matched_code':int(report['measures']['matched_code']),'denominator':762872,'percent':100*int(report['measures']['matched_code'])/762872,'new_source_functions':len(gained),'gain_bytes':gain,'previous_exact_regressions':exact,'previous_raw_exact_regressions':raw,'previous_partial_score_declines':partial,'episodes_verified':not a.without_episodes,'ROM_md5':hashlib.md5((root/'tenshoe.z64').read_bytes()).hexdigest(),'strict_blocks':blocks,'generated_data_spans_both_modes':data_spans,'native_call_sites':sum(len(x['native_proof']['calls']) for x in manifest['functions']),'native_data_references':sum(len(x['data_proof']) for x in manifest['functions']),'source_manifest_sha256':sha(root/'scripts/match-imports.json')};(out/'result.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
if __name__=='__main__':main()
