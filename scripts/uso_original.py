"""Read original USO identities; never infer a callee from a zero-valued alias."""
from pathlib import Path
import hashlib,re,struct
import crunch64
ROM_MD5='fa27089c425dbab99f19245c5c997613'
MAGIC=bytes.fromhex('12345678')
def u32(d,p):return struct.unpack_from('>I',d,p)[0]
def bootup(rom,start=0xd9fe28):
 p=start+12;sec={}
 for _ in range(64):
  kind,size=u32(rom,p+4),u32(rom,p+8)
  if kind in [10,11]:break
  assert kind in range(1,10) and kind not in sec
  sec[kind]=rom[p+12:p+12+size] if kind!=8 else b'';p+=12+(0 if kind==8 else (size+3)&~3)
 # Bootup payloads carry an original four-byte marker.
 text=sec[5][4:];ro=sec[7][4:];rel={u32(sec[2],i+8):(u32(sec[2],i+4)>>4,u32(sec[2],i+4)&15) for i in range(0,len(sec[2]),12)}
 return {'rom_offset':start,'text':text,'rodata':ro,'symbols':sec[1],'relocs':rel,'sections':sec,'format':'bootup'}
def ordinary(rom,start):
 d=rom[start:];assert d[:4]==MAGIC;inner=d.find(MAGIC,4);assert 0<inner<256;p=inner+16;sec={}
 for _ in range(64):
  found=None
  for shift in range(4):
   kind,size,flags=struct.unpack_from('>III',d,p+shift)
   if kind in {1,2,3,4,5,6,7,8,9,11} and flags>>16 in {0x1000,0x1001,0x0800,0x2000,0x2001}:found=p+shift;break
  if found is None:raise ValueError('USO directory ended before EOF')
  a=found+12
  if flags&0x10000:
   payload=crunch64.yay0.decompress(d[a:]);p=a+len(crunch64.yay0.compress(payload))
  else:payload=d[a:a+size] if kind!=8 else b'';p=a+(size if kind!=8 else 0)
  assert kind not in sec;sec[kind]=payload
  if kind==11:break
 assert all(k in sec for k in [1,2,5,11])
 rel={u32(sec[2],i+4):(u32(sec[2],i)>>4,u32(sec[2],i)&15) for i in range(0,len(sec[2]),12)}
 return {'rom_offset':start,'text':sec[5],'rodata':sec.get(7,b''),'symbols':sec[1],'relocs':rel,'sections':sec,'format':'ordinary'}
def modules(root):
 root=Path(root);rom=(root/'baserom.z64').read_bytes();assert hashlib.md5(rom).hexdigest()==ROM_MD5;y=(root/'tenshoe.yaml').read_text();out={'bootup_uso':bootup(rom)}
 for name in ['arcproc_uso','mgrproc_uso','game_uso','gui_uso','h2hproc_uso','titproc_uso']:
  m=re.search(r'\[(0x[0-9a-fA-F]+), bin, '+name+r'_pre\]',y);assert m;out[name]=ordinary(rom,int(m[1],16));out[name]['curated_text_bias']=4 if name in ['arcproc_uso','gui_uso','h2hproc_uso'] else 0
 positions=[]
 for filename in [b'timproc.uso\0',b'trkproc.uso\0',b'genproc.uso\0']:
  p=0
  while True:
   p=rom.find(filename,p)
   if p<0:break
   start=p-12
   if rom[start:start+4]==MAGIC:positions.append(start)
   p+=1
 candidates=[ordinary(rom,p) for p in positions]
 for block in [1,3,5]:
  original=crunch64.yay0.decompress((root/f'assets/timproc_uso_block{block}_yay0.bin').read_bytes());matches=[x for x in candidates if x['text']==original];assert len(matches)==1,('timproc',block,[(x['rom_offset'],len(x['text'])) for x in candidates]);out[f'timproc_uso_b{block}']=matches[0]
 return out
