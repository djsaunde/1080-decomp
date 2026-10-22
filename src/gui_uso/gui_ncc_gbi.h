/* Minimal gbi.h clone for the gui_uso NCC donors: the macro bodies are the
 * libreultra 2.0I PR/gbi.h text (references/libreultra/include/2.0I/PR/gbi.h,
 * _SHIFTL from mbi.h), so edg sees the same expression trees the original
 * source did. Only what the gui texture loaders use is reproduced. */
#ifndef GUI_NCC_GBI_H
#define GUI_NCC_GBI_H
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
struct Gwords { unsigned int w0, w1; };
union Gfx { Gwords words; long long force_structure_alignment; };
#define G_IM_FMT_RGBA 0
#define G_IM_FMT_IA 3
#define G_IM_SIZ_8b 1
#define G_IM_SIZ_16b 2
#define G_IM_SIZ_32b 3
#define G_IM_SIZ_8b_TILE_BYTES 1
#define G_IM_SIZ_8b_LINE_BYTES 1
#define G_IM_SIZ_16b_TILE_BYTES 2
#define G_IM_SIZ_16b_LINE_BYTES 2
#define G_IM_SIZ_32b_TILE_BYTES 2
#define G_IM_SIZ_32b_LINE_BYTES 2
#define G_SETTIMG 0xfd
#define G_SETTILE 0xf5
#define G_LOADTILE 0xf4
#define G_SETTILESIZE 0xf2
#define G_RDPPIPESYNC 0xe7
#define G_RDPLOADSYNC 0xe6
#define G_TX_LOADTILE 7
#define G_TEXTURE_IMAGE_FRAC 2
#define gSetImage(pkt, cmd, fmt, siz, width, i) \
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | \
		       _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12); \
	_g->words.w1 = (unsigned int)(i); \
}
#define gDPSetTextureImage(pkt, f, s, w, i) gSetImage(pkt, G_SETTIMG, f, s, w, i)
#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = _SHIFTL(G_SETTILE, 24, 8) | _SHIFTL(fmt, 21, 3) | \
		       _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | \
		       _SHIFTL(tmem, 0, 9); \
	_g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | \
		       _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | \
		       _SHIFTL(shiftt, 10, 4) |_SHIFTL(cms, 8, 2) | \
		       _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4); \
}
#define gDPLoadTileGeneric(pkt, c, tile, uls, ult, lrs, lrt) \
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | \
		      _SHIFTL(ult, 0, 12); \
	_g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | \
		      _SHIFTL(lrt, 0, 12); \
}
#define gDPSetTileSize(pkt, t, uls, ult, lrs, lrt) gDPLoadTileGeneric(pkt, G_SETTILESIZE, t, uls, ult, lrs, lrt)
#define gDPLoadTile(pkt, t, uls, ult, lrs, lrt) gDPLoadTileGeneric(pkt, G_LOADTILE, t, uls, ult, lrs, lrt)
#define gDPNoParam(pkt, cmd) \
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = _SHIFTL(cmd, 24, 8); \
	_g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPLoadSync(pkt) gDPNoParam(pkt, G_RDPLOADSYNC)
#endif
