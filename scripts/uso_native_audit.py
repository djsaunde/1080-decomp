"""Deterministic native-function/USO identity checks for the integration candidate."""
from pathlib import Path
from collections import defaultdict
import hashlib,io,re,struct
from elftools.elf.elffile import ELFFile
from uso_original import u32

def require(ok,why):
 if not ok:raise ValueError(why)
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def native(path,name):
 e=ELFFile(io.BytesIO(Path(path).read_bytes()));t=e.get_section_by_name('.symtab');ss=t.get_symbol_by_name(name);require(ss is not None,'missing native function '+name);f=ss[0];s=e.get_section(f['st_shndx']);a=f['st_value']-s['sh_addr'];require(a>=0 and a+f['st_size']<=s['sh_size'],'invalid native function extent');data=s.data()[a:a+f['st_size']];rels=[]
 for rs in e.iter_sections():
  if rs['sh_type']!='SHT_REL' or rs['sh_info']!=f['st_shndx']:continue
  for z in rs.iter_relocations():
   off=z['r_offset']-f['st_value']
   if 0<=off<f['st_size']:
    sy=t.get_symbol(z['r_info_sym']);name2=sy.name or (e.get_section(sy['st_shndx']).name if isinstance(sy['st_shndx'],int) else '')
    rels.append({'offset':int(off),'kind':z['r_info_type'],'symbol':name2,'symbol_index':z['r_info_sym']})
 return data,rels
def original_words(root,row,mods):
 module=row['module'];start=row['original_offset'];size=row['size']
 if module in mods:
  m=mods[module];start-=m.get('curated_text_bias',0);require(0<=start and start+size<=len(m['text']),'original function extent outside USO');return m['text'][start:start+size],start,m
 # Uncompressed kernel: original ROM offsets in the immutable assembly are checked.
 rom=(Path(root)/'baserom.z64').read_bytes();found={}
 for line in (Path(root)/row['asm']).read_text().splitlines():
  q=re.search(r'/\*\s*([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+([0-9a-fA-F]{8})\s*\*/',line)
  if q:
   off,addr,word=(int(x,16) for x in q.groups());require(u32(rom,off)==word,'kernel ASM/ROM evidence mismatch');found[addr]=word
 require(all(start+i in found for i in range(0,size,4)),'kernel original instruction coverage missing');return b''.join(struct.pack('>I',found[start+i]) for i in range(0,size,4)),start,None

def identity_audit(root,row,mods,mode='normal'):
 root=Path(root);path=row['native_normal'] if mode=='normal' else row['native_nm'];data,rels=native(root/path,row['function']);original,start,m=original_words(root,row,mods);require(len(data)==row['size'],'native function size differs');masks=defaultdict(int)
 for z in rels:
  require(z['kind'] in [4,5,6],'unsupported native text relocation');masks[z['offset']]|=0x3ffffff if z['kind']==4 else 0xffff
 for p in range(0,len(data),4):
  actual=u32(data,p);wanted=u32(original,p);require(((actual^wanted)&~masks[p])==0,'native non-relocation instruction differs '+row['function']+'+'+hex(p))
 calls=[];groups=defaultdict(set)
 for z in rels:
  if z['kind']!=4:continue
  p=z['offset'];word=u32(original,p);require(word>>26 in [2,3],'native call relocation does not correspond to J/JAL');entry=m['relocs'].get(start+p) if m else None;addend=(word&0x3ffffff)<<2
  if entry:
   require(entry[1]==1,'original call relocation kind differs');key=row['module']+':symbol_'+str(entry[0])+':addend_'+hex(addend)
  else:key=row['module']+':absolute_'+hex(((start+p+4)&0xf0000000)|addend)
  groups[z['symbol']].add(key);calls.append(dict({k:v for k,v in z.items() if k!='symbol_index'},original_identity=key,original_site=row['original_offset']+p,payload_site=start+p,original_symbol_index=entry[0] if entry else None,original_addend=addend,opcode=word>>26))
 for alias,targets in groups.items():require(len(targets)==1,'one compiler/C call alias conflates original USO identities: '+row['function']+' '+alias+' '+str(sorted(targets)))
 indirect=[p for p in range(0,len(data),4) if u32(data,p)&0xfc00003f==9];return {'calls':calls,'indirect_sites':indirect,'native_sha256':hashlib.sha256(data).hexdigest()}

def data_audit(root,row,mods,mode='normal'):
 path=row['native_normal'] if mode=='normal' else row['native_nm'];data,rel=native(Path(root)/path,row['function']);original,start,m=original_words(root,row,mods);pending=defaultdict(list);last={};refs=[];groups=defaultdict(set)
 for z in sorted(rel,key=lambda z:z['offset']):
  if z['kind'] not in [5,6]:continue
  sid=z['symbol_index'];off=z['offset'];word=u32(data,off)
  if z['kind']==5:pending[sid].append((off,word));continue
  highs=pending.pop(sid,[]) or ([last[sid]] if sid in last else [])
  require(bool(highs),'native LO16 has no proven HI16')
  for hi,hw in highs:
   low=(word&65535)-(65536 if word&32768 else 0);add=((hw&65535)<<16)+low;oh=u32(original,hi);ol=u32(original,off);lo=(ol&65535)-(65536 if ol&32768 else 0);original_add=((oh&65535)<<16)+lo
   if m:
    h=m['relocs'].get(start+hi);l=m['relocs'].get(start+off);require(h and l and h[0]==l[0] and h[1]==3 and l[1]==2,'data reference has no common original USO identity');key=row['module']+':symbol_'+str(l[0])+':base_addend_'+hex(original_add-add)
   else:key=row['module']+':absolute_base_'+hex((original_add-add)&0xffffffff)
   groups[z['symbol']].add(key);refs.append({'alias':z['symbol'],'hi':hi,'lo':off,'native_addend':add,'original_addend':original_add,'base_identity':key});last[sid]=(hi,hw)
 for name,keys in groups.items():require(len(keys)==1,'one C data alias conflates original objects: '+row['function']+' '+name)
 return refs
