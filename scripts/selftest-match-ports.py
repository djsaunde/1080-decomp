#!/usr/bin/env python3
"""Focused negative controls; writes temporary build artifacts only."""
from pathlib import Path
import importlib.util,sys,json,io,tempfile,copy,subprocess
from elftools.elf.elffile import ELFFile
from uso_original import modules
from uso_native_audit import identity_audit,require,native
S=Path(__file__).resolve().parent;R=S.parent;spec=importlib.util.spec_from_file_location('port_verifier',S/'verify-match-ports.py');v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v);manifest=json.loads((S/'match-imports.json').read_text());mods=modules(R);results=[]
def rejects(label,action,text):
 try:action()
 except ValueError as e:
  require(text in str(e),'negative control failed for unrelated reason '+label+': '+str(e));results.append({'test':label,'rejected':True,'reason':str(e)})
 else:raise ValueError('negative control unexpectedly accepted '+label)
with tempfile.TemporaryDirectory(prefix='port-negative-',dir=R/'build') as d:
 d=Path(d);row=copy.deepcopy(next(x for x in manifest['functions'] if x['native_proof']['calls']));binding=json.loads((R/row['binding_normal']).read_text());key=next(iter(binding['original_ROM_minus_curated_addend_bindings']));binding['original_ROM_minus_curated_addend_bindings'][key]^=4;p=d/'bad-binding.json';p.write_text(json.dumps(binding));row['binding_normal']=str(p);rejects('forged original carrier',lambda:v.verify_bindings(R,row,mods,'normal'),'forged original-ROM carrier value')
 row=copy.deepcopy(manifest['functions'][0]);p=R/row['native_normal'];raw=bytearray(p.read_bytes());e=ELFFile(io.BytesIO(raw));f=e.get_section_by_name('.symtab').get_symbol_by_name(row['function'])[0];s=e.get_section(f['st_shndx']);at=s['sh_offset']+f['st_value']-s['sh_addr'];raw[at]^=0x10;p=d/'bad-native.o';p.write_bytes(raw);row['native_normal']=str(p);rejects('changed native instruction',lambda:identity_audit(R,row,mods),'native non-relocation instruction differs')
 fix=copy.deepcopy(manifest['source_fixes'][0]);p=R/fix['native_normal'];raw=bytearray(p.read_bytes());e=ELFFile(io.BytesIO(raw));ro=e.get_section_by_name('.rodata');raw[ro['sh_offset']+19]^=4;p=d/'bad-table.o';p.write_bytes(raw);fix['native_normal']=str(p);rejects('wrong source switch-table arm',lambda:v.generated_data(R,fix,mods,'normal'),'generated switch table disagrees with original ROM')
 row=next(x for x in manifest['functions'] if x['function']=='func_0000D900');out=d/'stdlib-bound.elf';p=subprocess.run([sys.executable,'-S',str(S/'bind-native-relocations.py'),str(R/row['linked_normal']),str(R/row['binding_normal']),str(out)],capture_output=True,text=True);require(p.returncode==0,'C build helper needs a new third-party dependency: '+p.stderr);require(native(out,row['function'])==native(R/row['bound_normal'],row['function']),'standard-library-only binding differs');results.append({'test':'C build helper without site packages','passed':True})
result={'passed':True,'controls':results};(R/'build/match-port-verification/selftests.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
