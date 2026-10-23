#!/usr/bin/env python3
"""Give native relocations distinct audited carrier names; never edit .text.

Uses the existing standard-library ELF editor, so building C objects needs no
new Python dependency. Bindings are checked independently against the original
ROM by verify-match-ports.py; input and output function bytes must agree.
"""
from pathlib import Path
import argparse,json,hashlib,importlib.util,struct
S=Path(__file__).resolve().parent;spec=importlib.util.spec_from_file_location('existing_splice',S/'replace-function-body.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
def require(ok,why):
 if not ok:raise ValueError(why)
def references(e,fn):
 _,f,_=e.find_symbol(fn);out=[]
 if not e.has_section('.rel.text'):return out
 symbols=dict(e.iter_symtab(e.find_section('.symtab')));strings=e.sections[e.find_section('.symtab')][6];section=e.sections[e.find_section('.rel.text')]
 for at in range(section[4],section[4]+section[5],section[9]):
  off,info=struct.unpack_from(m.REL_FMT,e.data,at)
  if f[1]<=off<f[1]+f[2]:
   symbol=symbols[info>>8];name=e.strtab_string(strings,symbol[0])
   if not name and symbol[5]<e.e_shnum:name=e.section_name(symbol[5])
   out.append({'site':off,'kind':info&255,'symbol':name})
 return out
p=argparse.ArgumentParser(description=__doc__);p.add_argument('input',type=Path);p.add_argument('manifest',type=Path);p.add_argument('output',type=Path);a=p.parse_args();manifest=json.loads(a.manifest.read_text());e=m.Elf(a.input.read_bytes());before=e.text_bytes_for(manifest['function']);require(hashlib.sha256(before).hexdigest()==manifest['raw_sha256'],'native linked body changed');rel=references(e,manifest['function']);ids={old:e.find_or_add_global_undef_symbol(new) for old,new in manifest['renames'].items()};by_offset={z['site']:z for z in rel}
if rel:
 section=e.sections[e.find_section('.rel.text')]
 for at in range(section[4],section[4]+section[5],section[9]):
  off,info=struct.unpack_from(m.REL_FMT,e.data,at)
  if off in by_offset:
   z=by_offset[off];require((info&255)==z['kind'],'native relocation kind changed');struct.pack_into(m.REL_FMT,e.data,at,off,(ids[z['symbol']]<<8)|z['kind'])
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(e.data);changed=references(e,manifest['function']);require(e.text_bytes_for(manifest['function'])==before,'symbol binding altered instructions');require(len(changed)==len(rel),'native relocation count changed');require({x['symbol'] for x in changed}==set(manifest['renames'].values()),'native symbol binding incomplete')
