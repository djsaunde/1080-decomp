/* minimal libultra 2.0I gbi.h subset */
typedef struct { unsigned int w0, w1; } Gwords;
typedef union { Gwords words; long long force_structure_alignment; } Gfx;
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define G_SETTIMG 0xFD
#define G_SETTILE 0xF5
#define G_LOADTILE 0xF4
#define G_LOADBLOCK 0xF3
#define G_SETTILESIZE 0xF2
#define G_LOADTLUT 0xF0
#define G_RDPPIPESYNC 0xE7
#define G_RDPLOADSYNC 0xE6
#define G_RDPTILESYNC 0xE8
#define G_SETOTHERMODE_H 0xBA
#define G_MDSFT_TEXTLUT 14
#define G_TT_NONE 0
#define G_TT_RGBA16 (2 << G_MDSFT_TEXTLUT)
#define G_TX_LOADTILE 7
#define G_TX_RENDERTILE 0
#define G_TEXTURE_IMAGE_FRAC 2
#define G_TX_DXT_FRAC 11
#define G_TX_LDBLK_MAX_TXL 2047
#define G_IM_FMT_RGBA 0
#define G_IM_FMT_CI 2
#define G_IM_FMT_IA 3
#define G_IM_FMT_I 4
#define G_IM_SIZ_4b 0
#define G_IM_SIZ_8b 1
#define G_IM_SIZ_16b 2
#define G_IM_SIZ_32b 3
#define G_IM_SIZ_4b_BYTES 0
#define G_IM_SIZ_4b_TILE_BYTES G_IM_SIZ_4b_BYTES
#define G_IM_SIZ_4b_LINE_BYTES G_IM_SIZ_4b_BYTES
#define G_IM_SIZ_8b_BYTES 1
#define G_IM_SIZ_8b_TILE_BYTES G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_8b_LINE_BYTES G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_16b_BYTES 2
#define G_IM_SIZ_16b_TILE_BYTES G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_16b_LINE_BYTES G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_32b_BYTES 4
#define G_IM_SIZ_32b_TILE_BYTES 2
#define G_IM_SIZ_32b_LINE_BYTES 2
#define G_IM_SIZ_4b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_8b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_16b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_32b_LOAD_BLOCK G_IM_SIZ_32b
#define G_IM_SIZ_4b_SHIFT 2
#define G_IM_SIZ_8b_SHIFT 1
#define G_IM_SIZ_16b_SHIFT 0
#define G_IM_SIZ_32b_SHIFT 0
#define G_IM_SIZ_4b_INCR 3
#define G_IM_SIZ_8b_INCR 1
#define G_IM_SIZ_16b_INCR 0
#define G_IM_SIZ_32b_INCR 0
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#define TXL2WORDS(txls, b_txl) MAX(1, ((txls)*(b_txl)/8))
#define CALC_DXT(width, b_txl) (((1 << G_TX_DXT_FRAC) + TXL2WORDS(width, b_txl) - 1) / TXL2WORDS(width, b_txl))
#define TXL2WORDS_4b(txls) MAX(1, ((txls)/16))
#define CALC_DXT_4b(width) (((1 << G_TX_DXT_FRAC) + TXL2WORDS_4b(width) - 1) / TXL2WORDS_4b(width))

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ Gfx *_g = (Gfx *)(pkt); \
  _g->words.w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(sft, 8, 8) | _SHIFTL(len, 0, 8)); \
  _g->words.w1 = (unsigned int)(data); }
#define gDPSetTextureLUT(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, type)
#define gSetImage(pkt, cmd, fmt, siz, width, i) \
{ Gfx *_g = (Gfx *)(pkt); \
  _g->words.w0 = _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12); \
  _g->words.w1 = (unsigned int)(i); }
#define gDPSetTextureImage(pkt, f, s, w, i) gSetImage(pkt, G_SETTIMG, f, s, w, i)
#define gDPNoParam(pkt, cmd) \
{ Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL(cmd, 24, 8); _g->words.w1 = 0; }
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPLoadSync(pkt) gDPNoParam(pkt, G_RDPLOADSYNC)
#define gDPTileSync(pkt) gDPNoParam(pkt, G_RDPTILESYNC)
#define gDPLoadTileGeneric(pkt, c, tile, uls, ult, lrs, lrt) \
{ Gfx *_g = (Gfx *)(pkt); \
  _g->words.w0 = _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12); \
  _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12); }
#define gDPSetTileSize(pkt, t, uls, ult, lrs, lrt) gDPLoadTileGeneric(pkt, G_SETTILESIZE, t, uls, ult, lrs, lrt)
#define gDPLoadTile(pkt, t, uls, ult, lrs, lrt) gDPLoadTileGeneric(pkt, G_LOADTILE, t, uls, ult, lrs, lrt)
#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
{ Gfx *_g = (Gfx *)(pkt); \
  _g->words.w0 = _SHIFTL(G_SETTILE, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9); \
  _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | \
                 _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4); }
#define gDPLoadBlock(pkt, tile, uls, ult, lrs, dxt) \
{ Gfx *_g = (Gfx *)(pkt); \
  _g->words.w0 = (_SHIFTL(G_LOADBLOCK, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12)); \
  _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL((MIN(lrs, G_TX_LDBLK_MAX_TXL)), 12, 12) | _SHIFTL(dxt, 0, 12)); }
#define gDPLoadTLUTCmd(pkt, tile, count) \
{ Gfx *_g = (Gfx *)pkt; \
  _g->words.w0 = _SHIFTL(G_LOADTLUT, 24, 8); \
  _g->words.w1 = _SHIFTL((tile), 24, 3) | _SHIFTL((count), 14, 10); }
#define gDPLoadTLUT_pal256(pkt, dram) \
{ gDPSetTextureImage(pkt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram); \
  gDPTileSync(pkt); \
  gDPSetTile(pkt, 0, 0, 0, 256, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0); \
  gDPLoadSync(pkt); \
  gDPLoadTLUTCmd(pkt, G_TX_LOADTILE, 255); \
  gDPPipeSync(pkt) }
#define gDPLoadMultiBlock(pkt, timg, tmem, rtile, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
  gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPLoadSync(pkt); \
  gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT)-1, CALC_DXT(width, siz##_BYTES)); \
  gDPPipeSync(pkt); \
  gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) }
#define gDPLoadMultiBlock_4b(pkt, timg, tmem, rtile, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
  gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, tmem, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPLoadSync(pkt); \
  gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)); \
  gDPPipeSync(pkt); \
  gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), tmem, rtile, pal, cmt, maskt, shiftt, cms, masks, shifts); \
  gDPSetTileSize(pkt, rtile, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) }
