#!/usr/bin/env python3
"""PC port: the per-module type map of the USO data blobs (docs/1080_PC_PORT_PLAN.md, "M1/M2:
byte order").

    scripts/port-gen-blob-types.py [--report]

The USO Data / RoData sections are big-endian on the cartridge.  port/tools/gen-uso-blobs.py
byte-swaps them at blob-build time from the map this script writes, port/gen/blob_types.tsv
(module, section, offset, size, type, source; offsets / sizes / types only, no ROM bytes).

Every typing source is the ROM itself (the module records parsed by scripts/uso-imports.py),
so the map covers modules with no src/ yet:

  access   every HI16/LO16 TextReloc pair of every module's Text that targets a Data / RoData
           Sym (imports followed to the exporter): the LO16 instruction's opcode is the access
           width (lb/lbu/sb 1, lh/lhu/sh 2, lw/sw 4, lwc1/swc1 f32, ldc1/sdc1/ld/sd 8).  An addiu
           (address taken) is followed forward through the function: loads / stores off that
           register (or a copy, or base + constant) type base + imm ("field"); off base + index
           ("index") they type one element and the run is extended (below).
  reloc    DataReloc / RoDataReloc R_MIPS_32 words: port/uso writes them in host order, so they
           are never swapped here (type ptr, kept as on the cartridge).
  gfx      display lists: a reloc'd word at +4 of a GBI (F3DEX, GBI1) command whose opcode
           takes an address, walked back and forward over GBI1 opcodes to G_ENDDL: u32 pairs.
           G_VTX targets: Vtx (s16 x 6 + u8 x 4).  G_MTX targets: Mtx (u32 x 16).
           G_SETTIMG targets and G_MOVEMEM lights stay bytes (libultraship's Fast3D reads
           texels, TLUTs and lights byte by byte, big-endian).
  shape    shape command streams (op 27 verts / 28 tris / 4 uv s16 x 2 / 25 palette bytes / 29
           materials -> textures; op 35 collision grid) at every symbol, pointer target and
           address the code takes.
  tex:hdr  image symbols: a {0, 1} word pair at a Sym directly followed by the texel Sym (u32 x 2,
           then bytes); covers textures only code reaches (a built G_SETTIMG).
  vtbl     cfront vtable slots {s16 delta, s16 index, fn}: the head word of a relocated pointer
           to Text in an 8-byte-stride table (u16 x 2; every slot on this ROM is 0 today).
  pmf      cfront pointer-to-member records {s16 delta, s16 vslot, fn} (GdlPmf / PortCfPmf): a
           {small delta, -1} head the code copies, before a relocated pointer to Text, and the
           null {0, 0, 0} next to one (u16 x 2, the fn word ptr / u32).
  str      NUL-terminated printable runs (>= 4 chars) at an aligned or referenced start, and
           1-3 chars at a referenced start (parm tags "he1", formats "%s").
  zero     all-zero bytes outside every typed run (any swap is a no-op).
  audio    bootup Data ranges port/audio swaps at run time (plan 15.6): excluded here, a second
           swap would undo them.
  index    an indexed-access element type extended forward to the next symbol / reloc target /
           typed byte (lower confidence; the source column says so).

Conflicts (one byte typed with two widths) are written as type `conflict` and not swapped;
--report lists them.  Untyped non-zero bytes are left as on the cartridge (`unknown`).
"""
import argparse
import collections
import importlib.util
import os
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)
spec = importlib.util.spec_from_file_location("npc", str(ROOT / "scripts/name-placeholder-calls.py"))
npc = importlib.util.module_from_spec(spec)
spec.loader.exec_module(npc)
UI = npc.UI

# opcode -> (width, type) of a load / store
MEMOP = {0x20: (1, "u8"), 0x24: (1, "u8"), 0x28: (1, "u8"),
         0x21: (2, "u16"), 0x25: (2, "u16"), 0x29: (2, "u16"),
         0x23: (4, "u32"), 0x2B: (4, "u32"),
         0x31: (4, "f32"), 0x39: (4, "f32"),
         0x35: (8, "f64"), 0x3D: (8, "f64"), 0x37: (8, "u64"), 0x3F: (8, "u64")}
WIDTH = {"u8": 1, "str": 1, "bytes": 1, "u16": 2, "s16": 2, "u32": 4, "f32": 4, "ptr": 4,
         "u64": 8, "f64": 8}
SECS = ("Data", "RoData")

# plan 15.6: bootup Data swapped by port_audio_swap_static at run time
AUDIO_RANGES = [(0x17D50, 0x1A2E0), (0x1CB00, 0x1CDE0), (0x1D620, 0x1E0D0)]

# Layouts known from the C side (plan 18.1), applied last: they override the inference.
# (module, section, base, [(offset, size, type), ...] per record, count, stride, source)
KNOWN = [
    # snd_HandleSeqEvent_30AF4's command word (gl_cmd_30AF4): a u32.  Its low-byte reads
    # (CMD_B3, lb +3) are fixed at the read site under PORT, so the word is swapped.
    ("bootup", "Data", 0x1C0AC, [(0, 4, "u32")], 1, 4, "known"),
    # osViModeTable (sym1947, gl_osViModeTable_44B78_modes): 42 OSViMode records of 0x50,
    # {u8 type + 3 pad, OSViCommonRegs 9 x u32, OSViFieldRegs 2 x 5 x u32}; types 0x00-0x29.
    ("bootup", "Data", 0x2D700, [(0, 4, "u8"), (4, 0x4C, "u32")], 42, 0x50, "known"),
    # charsel: 6 blocks of 0x10220, each 8-bit texels (0x10010) and a TLUT (0x200), both bytes
    # (Fast3D reads texels and TLUTs big-endian), then the record genproc loads:
    # {0x110, 0x100, image ptr, TLUT ptr} (the pointers are DataReloc words, typed ptr already).
    ("charsel", "Data", 0, [(0, 0x10210, "u8"), (0x10210, 8, "u32")], 6, 0x10220, "known"),
    # snoweffects LocalLight flicker sequences (snoweffects_uso.c SnowFlicker {s32 count; u8 *vals},
    # snow_flickerTbl_958 = {0x93C, 0x930, 0x950}): the brightness bytes, then the records' counts
    ("snoweffects", "Data", 0x910, [(0, 0x20, "u8")], 1, 0x20, "known"),
    ("snoweffects", "Data", 0x938, [(0, 4, "u8")], 1, 4, "known"),
    ("snoweffects", "Data", 0x944, [(0, 0xC, "u8")], 1, 0xC, "known"),
    ("snoweffects", "Data", 0x930, [(0, 4, "u32")], 1, 4, "known"),
    ("snoweffects", "Data", 0x93C, [(0, 4, "u32")], 1, 4, "known"),
    ("snoweffects", "Data", 0x950, [(0, 4, "u32")], 1, 4, "known"),
    # viewrec vr_sndTbl_4990 (Data 0x318, SoundOptions_update): s32 sound id per row, 4 rows
    ("viewrec", "Data", 0x318, [(0, 4, "u32")], 4, 4, "known"),
    # --- bootup Data, run 813 ---
    # RSP ucode DMEM images (task.ucode_data of the RcpTask_setUcode* setters: Sgi20H 0x11990 /
    # 0x13190, Sgi20HOut 0x13990, F3DEX_NoN 0x14190, F3DEX 0x15550, F3DLP_Rej 0x16550, F3DLX_Rej
    # 0x16D50, Sgi20HFifo 0x17550; 0x12190 / 0x12990 / 0x14D50 / 0x15D50 are the unreferenced
    # siblings) up to the audio tables: read by the RSP only, never by the port (Fast3D): bytes
    ("bootup", "Data", 0x11990, [(0, 0x17D50 - 0x11990, "u8")], 1, 0x17D50 - 0x11990, "known:ucode-data"),
    # gfx_drawFbSplash_00048184: the 128x128 u16 (RGBA5551) splash it copies into the colour
    # buffer as u16 (D_48184_img)
    ("bootup", "Data", 0x22A30, [(0, 2, "u16")], 128 * 128, 2, "known:splash"),
    # math_atanLookup_00067FE4: 1025 u16 at D+0x2AA60, indexed by round(a0 / a1 * 1024)
    ("bootup", "Data", 0x2AA60, [(0, 2, "u16")], 1025, 2, "known:atan"),
    # math_acosf: D_69CC4_tbl, u16 indexed by (int)(|x| * 2048), x in [0, 1]: 2049 entries
    ("bootup", "Data", 0x2C650, [(0, 2, "u16")], 2049, 2, "known:acos"),
    # __osViInit's three OSViMode records (libultra vi.c: the NTSC / PAL / MPAL lan1 modes it
    # picks by osTvType), the osViModeTable layout above
    ("bootup", "Data", 0x2E530, [(0, 4, "u8"), (4, 0x4C, "u32")], 3, 0x50, "known:OSViMode"),
    # the course descriptors (include/game/course.h, WorldCourse.desc): COURSE_OP_* streams of
    # s32 words {argc << 16 | op, argc args} that game_createMap interprets, 0x67F0..0x6E30
    # (the "World Registry" strings start at 0x6E30); their path arguments stay ptr
    ("bootup", "Data", 0x67F0, [(0, 4, "u32")], (0x6E30 - 0x67F0) // 4, 4, "known:course-desc"),
    # the record / options defaults Backup_resetCourseData_0000A670 / Backup_resetArcadeData_0000AAEC
    # pass on: per course 0x24 of times (3 x 3 s32, Backup_courseRecordResetTimes) at 0xD268, then
    # 0xC of scores (3 s32) at 0xD388, then 3 arcade entries of 0x18 (s32 x 6) at 0xD3E8
    ("bootup", "Data", 0xD268, [(0, 4, "u32")], (0xD430 - 0xD268) // 4, 4, "known:backup-defaults"),
    # aspMain's DMEM data image (aspMainDataStart, game_libs_rsp_ucode_31DF8; plan 15.2): RSP only,
    # port/audio carries its own copy of the resample LUT (mixer_resample_lut): bytes
    ("bootup", "Data", 0x1D340, [(0, 0x2E0, "u8")], 1, 0x2E0, "known:aspMain-data"),
    # the rider node list's op 114 transforms (s16[10]: pos, quat / 32767, scale; QNode_doCommand
    # case 0x72) tile 0xA0F8..0xA3A0 in 0x14 strides; the stream reaches 20 of the 34, the
    # symbols at 0xA134 / 0xA1AC / 0xA210 / 0xA238 / 0xA29C / 0xA300 / 0xA328 are on the grid
    ("bootup", "Data", 0xA0F8, [(0, 0x14, "u16")], 34, 0x14, "known:nodecmd:xform"),
    # --- bootup Data, run 815 (readers in the comments) ---
    # game_initDdm_00002C94 (addiu + lw Data 0x7198): {s32 id, u32 packed} word pairs up to the
    # {0, 0} terminator at 0x7278
    ("bootup", "Data", 0x7198, [(0, 4, "u32")], (0x7278 - 0x7198) // 4, 4, "known:ddm-table"),
    # item_createSnow (addiu Data 0x7280): four f32 1.0
    ("bootup", "Data", 0x7280, [(0, 4, "f32")], 4, 4, "known:snow-rgba"),
    # the camera viewport records gen_createCamera_0000283C / game_setupSolo / game_setupDemoSolo
    # (0x7290), h2h_setupCameras_00000C18 (0x72AC) and RaceProcess_createReplayCam (0x7300) pass
    # on: 7 s32 {x, y, w, h, 4, far, 0x400 / 0x800} (320 x 240, 320 x 120 halves, 192 x 96)
    ("bootup", "Data", 0x7290, [(0, 0x1C, "u32")], 5, 0x1C, "known:cam-viewport"),
    # item_createFog (addiu Data 0x7B4C): f32 {0, 0.5, 0, 1, 1, 1, 0} fog parameters
    ("bootup", "Data", 0x7B4C, [(0, 4, "f32")], 8, 4, "known:fog-parms"),
    # item_registerTemplates (addiu Data 0x7B6C): {s32 offset, s32 0, s32 count} records up to
    # the {0, 0, 0} terminator at 0x7C14
    ("bootup", "Data", 0x7B6C, [(0, 0xC, "u32")], 15, 0xC, "known:item-pools"),
    # SnowBoarder_updateOrient (addiu Data 0x8170) -> pad_startRumble: 0xFF-terminated rumble
    # motor bytes (gdl/pad.h PadCtl.rumble); the pointer table at 0x81C0 reaches 0x8180 / 0x8190
    ("bootup", "Data", 0x8170, [(0, 0x50, "u8")], 1, 0x50, "known:rumble"),
    # SnowBoarder_ctor's tint list (addiu Data 0x8478, Text 0xAD50 loop): 0x18-byte records
    # {s32 key, s32 slot, f32 rgba[4]} up to key 0 (0x8670); table[slot] = &rec->rgba
    ("bootup", "Data", 0x8478, [(0, 8, "u32"), (8, 0x10, "f32")], 22, 0x18, "known:sb-tint"),
    # --- bootup Data, run 821 ---
    # OoT synthesis.c D_801304B0 (u16[8] {0x7FFF, 0xD001, ...}, unreferenced there too), after
    # sEnvMixerOp / the three dmemDests words (gl_aud_sEnvMixer*, 0x1A2D0..0x1A2DC)
    ("bootup", "Data", 0x1A2E0, [(0, 2, "u16")], 8, 2, "known:oot-synthesis"),
    # OoT seqplayer.c sSeqInstructionArgsTable: one u8 per command 0xB0..0xFF (the 0x81 / 0x42
    # arg-kind bytes), up to the "Audio:Track :Call" string at 0x1BCB0
    ("bootup", "Data", 0x1BC60, [(0, 0x50, "u8")], 1, 0x50, "known:oot-seq-args"),
    # snd_ResetVolumeLevels' lw of Sym 0x1C128 / +0x20: 11 f32 volume steps 0.0, 0.1 .. 1.0
    ("bootup", "Data", 0x1C128, [(0, 4, "f32")], 11, 4, "known:snd-vol-steps"),
    # a byte permutation of 0..15 between the snd f32 curves (no reader; bytes either way)
    ("bootup", "Data", 0x1C320, [(0, 0x10, "u8")], 1, 0x10, "known:bytes"),
    # sys_fatalHaltLoop's gl_fatalHaltExitCombo: 0-terminated s32 pad-button masks
    ("bootup", "Data", 0x1E21C, [(0, 4, "u32")], 7, 4, "known:halt-combo"),
    # unreferenced f32 data at two Syms: a 4x4 identity (1.0 every 0x14) and {-0.48, 0.8, 0.36, 1}
    ("bootup", "Data", 0x1E9A4, [(0, 4, "f32")], 20, 4, "known:f32-values"),
    # Sym 0x1F984 (next to the GfxLib screen size words 0x1F974..): 4 records of 10 u16
    # ({0, 0, 0, 0xFF, 0, 0, 0x80, 0, 0, 0x32} x 3, then {0xFF x 3, 0x80 x 3, 0x32 x 3, 0});
    # reader not located, u16 by the values (0x0080 / 0x00FF halves)
    ("bootup", "Data", 0x1F984, [(0, 2, "u16")], 40, 2, "known:u16-values"),
    # 12 f32 vec3 at the Syms 0x20350 .. 0x203D4 (0x1F9D8.. pointer table targets 0x2038C);
    # the str rule took 0x203C8's 0.5f (3f 00 00 00) for "?"
    ("bootup", "Data", 0x20350, [(0, 4, "f32")], 36, 4, "known:f32-vec3"),
    # 3 f32 vec3 at the Syms 0x20598 / 0x205A4 / 0x205B0 ({-100, 100, -100}, ...)
    ("bootup", "Data", 0x20598, [(0, 4, "f32")], 9, 4, "known:f32-vec3"),
    # Sym 0x21130 (before the "ProcNodeList" string): a shape stream {op 6 (0), op 6 (0), op 2}
    ("bootup", "Data", 0x21130, [(0, 4, "u32")], 5, 4, "known:shape"),
    # fault_printFlags' tables (Flag55FB4 {s32 mask, s32 bits, char *name}, game_libs_post0b.c),
    # mask-0 terminated; fault_printThread passes them (fpcsr = 0x214A4): 82 records to 0x215B8
    ("bootup", "Data", 0x211E0, [(0, 8, "u32")], 82, 12, "known:fault-flags"),
    # math_mtxChain_0005E288 (lwc1 0x21AC8 / 0x21ACC): 10 f32 series coefficients 1, -1/2, 3/8, ...
    ("bootup", "Data", 0x21AC8, [(0, 4, "f32")], 10, 4, "known:series"),
    # math_mtxToQuat (addiu 0x21AF0): Shoemake's next[3] = {1, 2, 0}
    ("bootup", "Data", 0x21AF0, [(0, 4, "u32")], 3, 4, "known:quat-next"),
    # libultra __osThreadTail {OSThread *next = 0, OSPri priority = -1} (__osRunQueue /
    # __osActiveQueue at 0x2D698 / 0x2D69C point to it)
    ("bootup", "Data", 0x2D690, [(0, 4, "u32")], 2, 4, "known:osThreadTail"),
    # f64 1.0 in the RoData literal pool
    ("bootup", "RoData", 0x24E0, [(0, 8, "f64")], 1, 8, "known:f64-values"),
    # the shape streams at the Syms 0x20D2C .. 0x20D80 after the 0x20D04 mesh: flag ops 13 / 15 /
    # 17 {1} and op 2 (no mesh op, so walk_assets does not take them)
    ("bootup", "Data", 0x20D2C, [(0, 4, "u32")], 23, 4, "known:shape"),
    # the head of the f32 table at Sym 0x70E8 (addiu, Text 0x4DA4; its +4.. were typed "index")
    ("bootup", "Data", 0x70E8, [(0, 4, "f32")], 1, 4, "known:f32-table"),
    # Sym 0x1FC74 (s32 7000) in the s32 row 0x1FC60..0x1FCA0 the code takes each address of
    ("bootup", "Data", 0x1FC74, [(0, 4, "u32")], 1, 4, "known:s32-row"),
    # --- game Data, run 821 ---
    # lh 0xCC4 / 0xCC6 and addiu 0xCC8 (game Text 0xB4D4..): 9 s16 pairs {-2, 0, 2} to 0xCE8
    ("game", "Data", 0xCC4, [(0, 2, "u16")], 18, 2, "known:s16-pairs"),
    # Sym 0x1068: s32 {0, 100, 150, 200, 250, 300, 350, 400, 500, 3000, -1}
    ("game", "Data", 0x1068, [(0, 4, "u32")], 11, 4, "known:s32-table"),
    # the rumble patterns (addiu 0x109C / 0x10A4 / 0x10AC / 0x10C0 / 0x10DC -> pad_startRumble,
    # as bootup 0x8170): 0xFF-terminated motor bytes up to Sym 0x10FC
    ("game", "Data", 0x109C, [(0, 0x60, "u8")], 1, 0x60, "known:rumble"),
    # --- genproc Data, run 821 ---
    # the s32 tables the code indexes (0xF08.. access / index around the gap at 0xF20), the 8
    # s32 {mask, 0x100} words the pointer at 0xF60 reaches, and 4 rows of 11 s32 at 0x11B0
    ("genproc", "Data", 0xF08, [(0, 4, "u32")], 11, 4, "known:s32-table"),
    ("genproc", "Data", 0xF40, [(0, 4, "u32")], 8, 4, "known:s32-table"),
    ("genproc", "Data", 0x11B0, [(0, 4, "u32")], 44, 4, "known:s32-table"),
    # boarder3 0x3B28: 231 s16[3] (the unreferenced sibling of the op 32 remap array; values
    # ffcf ff89 ffc8 ...)
    ("boarder3", "Data", 0x3B28, [(0, 2, "u16")], 231 * 3, 2, "known:s16-values"),
]
# gamegfx (Data only, no relocs): five images, each an 8-byte header {0, 1} at an exported
# symbol followed by texel bytes up to the next header.  The header reader is not located
# (gamegfx is reached through its exports); {0, 1} is typed as two words.
_GAMEGFX = [0x0, 0x848, 0x1C50, 0x33B8, 0x3E10, 0x4820]
for _a, _b in zip(_GAMEGFX, _GAMEGFX[1:]):
    KNOWN.append(("gamegfx", "Data", _a, [(0, 8, "u32"), (8, _b - _a - 8, "u8")], 1, _b - _a, "known"))

# GBI1 (F3DEX) opcodes; address-taking ones: G_MTX 01, G_MOVEMEM 03, G_VTX 04, G_DL 06,
# G_SETTIMG FD, G_SETZIMG FE, G_SETCIMG FF
GBI1_OPS = set(range(0x00, 0x08)) | set(range(0xAF, 0xC0)) | set(range(0xE4, 0x100))
ADDR_OPS = {0x01, 0x03, 0x04, 0x06, 0xFD, 0xFE, 0xFF}
G_ENDDL = 0xB8


def plausible(d, o, typ):
    """a float element worth extending an indexed run over: 0 or a normal exponent near 1, and
    not text (bootup RoData 0x2588 "NaN\0Inf\0" ends the powers-of-ten table)"""
    el = d[o:o + (4 if typ == "f32" else 8)]
    if re.search(rb"[\x20-\x7e]{3}\x00", bytes(el)):
        return False
    if typ == "f32":
        w = struct.unpack_from(">I", d, o)[0]
        e = (w >> 23) & 0xFF
        return w & 0x7FFFFFFF == 0 or 127 - 40 <= e <= 127 + 40
    w = struct.unpack_from(">Q", d, o)[0]
    e = (w >> 52) & 0x7FF
    return w & 0x7FFFFFFFFFFFFFFF == 0 or 1023 - 900 <= e <= 1023 + 900


def sext(v):
    return v - 0x10000 if v & 0x8000 else v


class Map:
    """per (module, section): byte -> (type, source)"""

    def __init__(self, data):
        self.data = data
        self.n = len(data)
        self.t = [None] * self.n
        self.src = [None] * self.n
        self.st = [None] * self.n
        self.conf = []

    def put(self, off, typ, src, count=1, force=False):
        """type count elements of typ at off.  Relocated words and audio ranges are never
        retyped; an element overlapping one of another width or start is a conflict."""
        w = WIDTH.get(typ, 1)
        for e in range(count):
            o = off + e * w
            if o < 0 or o + w > self.n:
                return
            cur = self.t[o:o + w]
            if not force and any(c in ("ptr", "audio") for c in cur):
                continue
            if force or all(c is None for c in cur):
                for b in range(w):
                    self.t[o + b], self.src[o + b], self.st[o + b] = typ, src, o
                continue
            if all(c is not None and c != "conflict" and WIDTH.get(c, 1) == w and self.st[o + b] == o
                   for b, c in enumerate(cur)):
                continue          # same width and start (u32 / f32 swap alike)
            if w == 1 and all(WIDTH.get(c, 1) == 1 for c in cur if c is not None):
                continue
            first = next(b for b, c in enumerate(cur) if c is not None)
            self.conf.append((o, cur[first], typ, src, self.src[o + first]))
            for b in range(w):
                if self.t[o + b] is not None:
                    self.t[o + b] = "conflict"


def indexed(text, h, lo_off, rs):
    """the LO16 access's base register is not the bare lui: an addu into it (or from it into
    rs) lies between the HI16 and the LO16 instruction"""
    if h is None:
        return False
    reg, hi_off = h
    if rs != reg:
        return True
    if not 0 < lo_off - hi_off <= 4 * 32:
        return False
    for pc in range(hi_off + 4, lo_off, 4):
        w = struct.unpack_from(">I", text, pc)[0]
        if w >> 26 == 0 and w & 0x3F in (0x21, 0x2D) and (w >> 11) & 31 == rs:
            return True
    return False


def analyse_text(md, rt, maps, stats):
    """HI16/LO16 pairs in md's Text that land in a Data / RoData section; follow addiu bases"""
    text = md.secs.get("Text")
    if not text:
        return
    rel = sorted(((o, f1 & 7, f1 >> 4) for f1, o, _ in md.relocs("Text")))
    hi, hireg = {}, {}
    for off, typ, idx in rel:
        if typ == 3:
            w = struct.unpack_from(">I", text, off)[0]
            hi[idx] = w & 0xFFFF
            hireg[idx] = ((w >> 16) & 31, off)
            continue
        if typ != 2 or idx not in hi:
            continue
        t = rt.raw(md, idx)
        if t is None:
            continue
        ex, sec, v = t
        if sec not in SECS or (ex.short, sec) not in maps:
            continue
        m = maps[(ex.short, sec)]
        ins = struct.unpack_from(">I", text, off)[0]
        op, rs, rtg, imm = ins >> 26, (ins >> 21) & 31, (ins >> 16) & 31, sext(ins & 0xFFFF)
        addr = (v + (hi[idx] << 16) + imm) & 0xFFFFFFFF
        if addr >= m.n:
            stats["access outside its section"] += 1
            continue
        if op in MEMOP:
            m.put(addr, MEMOP[op][1], "access")
            stats["direct access"] += 1
            if indexed(text, hireg.get(idx), off, rs):
                # lui r; addu r, r, index; lw x, %lo(sym)(r): an indexed element (math_acosf,
                # snd_SoundObjUpdate's f32 curves); extended forward below like "index"
                m.idx_hints.append((addr, MEMOP[op][1]))
                stats["indexed direct access"] += 1
        elif op == 0x09:      # addiu: address taken
            stats["address taken"] += 1
            m.addr_taken.add(addr)
            ADDIU_SITES[(md.short, off)] = ((ex.short, sec), addr, rtg)
            follow(md, text, off + 4, rtg, addr, m, stats)


# addiu (address taken) sites: (module, Text offset) -> ((module, section), address, dest reg)
ADDIU_SITES = {}
# readers that take an array and its element count in argument registers: (module, Text offset)
# -> (array reg, count reg, element type).  snd_FindNearestF32_2F1B8(f32 key, f32 *arr, s32 n)
# (game_libs_post.c): the binary search over a sorted f32 table (snd_SoundObjUpdate's pitch /
# volume curves, snd_SoundObjSetAngle's angle / pan tables)
ARRAY_ARGS = {("bootup", 0x43824): (5, 6, "f32")}


def call_arrays(rom, rt, maps, stats):
    """type the array a call to an ARRAY_ARGS reader passes: the addiu that sets the array
    register and the `addiu reg, zero, n` that sets the count, within the 8 instructions before
    the jal or in its delay slot"""
    for md in rom.mods:
        text = md.secs.get("Text")
        if not text:
            continue
        j26 = {o: f1 >> 4 for f1, o, _ in md.relocs("Text") if f1 & 7 == 1}
        for pc in range(0, len(text) - 4, 4):
            ins = struct.unpack_from(">I", text, pc)[0]
            if ins >> 26 != 0x03:
                continue
            tgt = None
            if pc in j26:
                t = rt.raw(md, j26[pc])
                if t and t[1] == "Text":
                    tgt = (t[0].short, t[2] + ((ins & 0x3FFFFFF) << 2))
            else:
                tgt = (md.short, (ins & 0x3FFFFFF) << 2)
            if tgt not in ARRAY_ARGS:
                continue
            areg, nreg, typ = ARRAY_ARGS[tgt]
            arr = n = None
            for q in list(range(max(0, pc - 32), pc, 4)) + [pc + 4]:
                w = struct.unpack_from(">I", text, q)[0]
                site = ADDIU_SITES.get((md.short, q))
                if site and site[2] == areg:
                    arr = site
                if w >> 26 == 0x09 and (w >> 21) & 31 == 0 and (w >> 16) & 31 == nreg:
                    n = w & 0xFFFF
            if arr is None or not n:
                stats["array call: args not found"] += 1
                continue
            m = maps.get(arr[0])
            if m is not None:
                m.put(arr[1], typ, "call:array", n)
                stats["array call"] += 1


def func_end(md, off):
    for v, sz, nm in md.funcs:
        if v <= off < v + max(sz, 4):
            return v + sz
    return off + 256


def follow(md, text, pc, reg, base, m, stats):
    """linear forward walk: regs holding base + k (exact) or base + index (idx)"""
    regs = {reg: (0, False)}
    end = min(func_end(md, pc - 4), pc + 4 * 96, len(text))
    while pc < end and regs:
        ins = struct.unpack_from(">I", text, pc)[0]
        op, rs, rt_, rd, imm = ins >> 26, (ins >> 21) & 31, (ins >> 16) & 31, (ins >> 11) & 31, sext(ins & 0xFFFF)
        fn = ins & 0x3F
        if op in MEMOP and rs in regs:
            k, idx = regs[rs]
            a = base + k + imm
            if 0 <= a < m.n:
                m.put(a, MEMOP[op][1], "index" if idx else "field")
                if idx:
                    m.idx_hints.append((a, MEMOP[op][1]))
                stats["field" if not idx else "index"] += 1
        # register definitions
        dst = None
        if op == 0x09:                                  # addiu
            dst = rt_
            if rs in regs:
                k, idx = regs[rs]
                regs[rt_] = (k + imm, idx)
                pc += 4
                continue
        elif op == 0 and fn in (0x21, 0x25, 0x2D):      # addu / or / daddu
            dst = rd
            a, b = rs in regs, rt_ in regs
            if (a and rt_ == 0) or (b and rs == 0):
                regs[rd] = regs[rs if a else rt_]
                pc += 4
                continue
            if a != b:
                k, _ = regs[rs if a else rt_]
                regs[rd] = (k, True)
                pc += 4
                continue
        elif op == 0:
            if fn == 0x08 or fn == 0x09:                # jr / jalr: one delay slot more
                end = min(end, pc + 8)
            dst = rd
        elif op in (0x02, 0x03):                        # j / jal
            if op == 0x02:
                end = min(end, pc + 8)
            # calls clobber the caller-saved registers after the delay slot
            for r in list(regs):
                if not (16 <= r <= 23 or r in (29, 30)):
                    regs.pop(r)
            pc += 4
            continue
        elif op in (0x0F, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E) or op in MEMOP and op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x37):
            dst = rt_
        elif op == 0x0F:
            dst = rt_
        if dst is not None and dst in regs:
            regs.pop(dst)
        pc += 4


def gfx_walk(md, sec, m, relocs, stats):
    """display lists around reloc'd GBI address words; Vtx / Mtx targets"""
    d = m.data
    vtx, mtx = [], []
    seen = set()
    for off in relocs:
        if off % 8 != 4 or off < 4 or d[off - 4] not in ADDR_OPS:
            continue
        start = off - 4
        if start in seen:
            continue
        # back to the DL start
        s = start
        while s >= 8 and d[s - 8] in GBI1_OPS and d[s - 8] != G_ENDDL and (s - 8) not in seen:
            s -= 8
        e = start
        ok = True
        while e + 8 <= m.n and d[e] != G_ENDDL:
            if d[e] not in GBI1_OPS:
                ok = False
                break
            e += 8
        if not ok or e + 8 > m.n:
            continue
        e += 8
        for c in range(s, e, 8):
            seen.add(c)
            m.put(c, "u32", "gfx", 2)
            op, w0 = d[c], struct.unpack_from(">I", d, c)[0]
            k = struct.unpack_from(">I", d, c + 4)[0]
            if (c + 4) not in relocs:
                continue
            if op == 0x04:
                lo = w0 & 0xFFFF
                n = lo >> 10 if (lo & 0x3FF) == ((lo >> 10) * 16 - 1) & 0x3FF else lo // 16
                vtx.append((c + 4, n))
            elif op == 0x01:
                mtx.append(c + 4)
        stats["gfx: display-list commands"] += (e - s) // 8
    # static display lists with no address word (gfx_initRDPState's RDP init lists at bootup
    # Data 0x224D0 / 0x224F0): an address the code takes that parses as immediate GBI1 commands
    # (opcode >= 0xAF; no G_SPNOOP, so zero runs never match) up to G_ENDDL
    for a in sorted(getattr(m, "addr_taken", ())):
        if a % 8 or a in seen:
            continue
        e = a
        while e + 8 <= m.n and e - a < 8 * 512:
            op = d[e]
            if op not in GBI1_OPS or op < 0xAF and not (op in ADDR_OPS and e + 4 in relocs):
                break
            e += 8
            if op == G_ENDDL:
                break
        if e - a < 16 or d[e - 8] != G_ENDDL:
            continue
        for c in range(a, e, 8):
            seen.add(c)
            m.put(c, "u32", "gfx:static", 2)
        stats["gfx: static display lists"] += 1
    return vtx, mtx


# shape command stream (GRAPHICS 7.1, BasePolyShape_parse): word = argc << 16 | op, args, op 2 ends.
# per op: (argc, [(arg index of the pointer, element type, count source)]); count source:
# "n" = arg 0, "verts" / "tris" = this stream's op 27 / 28 counts, int = fixed
SHAPE_OPS = {0: 0, 1: 0, 4: 1, 6: 1, 7: 1, 8: 1, 9: 1, 10: 1, 11: 1, 12: 1, 18: 1, 19: 1, 20: 1,
             21: 1, 25: 1, 26: 2, 27: 2, 28: 2, 29: 2, 30: 2, 31: 0, 32: 2, 34: 2, 35: 1, 36: 3,
             3: 1, 13: 1, 14: 1, 15: 1, 17: 1, 22: 1, 23: 1, 24: 1}
# GDL_MC material commands (GRAPHICS 6.2) and their argument words
MAT_ARGS = {200: 1, 201: 4, 202: 5, 203: 7, 204: 1, 205: 1, 206: 1, 207: 1, 208: 1, 209: 1, 210: 1, 211: 1}


def parse_shape(m, o):
    """-> list of (op, arg offset) up to op 2, or None if o is not a shape stream"""
    d, cmds = m.data, []
    while o + 4 <= m.n and len(cmds) < 4096:
        w = struct.unpack_from(">I", d, o)[0]
        op, argc = w & 0xFFFF, w >> 16
        if op == 2 and argc == 0:
            cmds.append((2, o))
            return cmds
        if op not in SHAPE_OPS or argc != SHAPE_OPS[op]:
            return None
        cmds.append((op, o))
        o += 4 + 4 * argc
    return None


def walk_assets(maps, ptrs, stats):
    """shape streams at every Data symbol / pointer target; material streams and textures from them"""
    def word(m, o):
        return struct.unpack_from(">I", m.data, o)[0]

    def tgt(key, o):
        return ptrs[key].get(o)

    def typed_run(key, o, typ, count, src):
        if key in maps and count > 0:
            maps[key].put(o, typ, src, count)

    def collgrid(gk, g):
        """CollGrid (gdl/coll.h, BasePolyShape +0xA0, parse cmd 35): {s32 w, h, x0, z0; f32 cellW,
        cellH; s16 *lists; s32 *cells}.  cells[z * w + x] indexes lists (-1 = empty); each list
        is a -1-terminated run of s16 triangle indices."""
        if gk not in maps or g + 0x20 > maps[gk].n:
            return
        gm = maps[gk]
        gm.put(g, "u32", "collgrid", 4)
        gm.put(g + 0x10, "f32", "collgrid", 2)
        w, h = word(gm, g), word(gm, g + 4)
        lt, ct = tgt(gk, g + 0x18), tgt(gk, g + 0x1C)
        if not (0 < w <= 1024 and 0 < h <= 1024) or not lt or not ct or ct[0] not in maps:
            return
        cm, co = maps[ct[0]], ct[1]
        if co + 4 * w * h > cm.n:
            return
        cm.put(co, "u32", "collgrid:cells", w * h)
        top = max((word(cm, co + 4 * i) for i in range(w * h)
                   if word(cm, co + 4 * i) != 0xFFFFFFFF), default=None)
        if top is None or lt[0] not in maps:
            return
        lm, lo = maps[lt[0]], lt[1]
        e = lo + 2 * top
        while e + 2 <= lm.n and struct.unpack_from(">H", lm.data, e)[0] != 0xFFFF:
            e += 2
        lm.put(lo, "u16", "collgrid:lists", (e + 2 - lo) // 2)
        stats["collision grids"] += 1

    for key, m in maps.items():
        for o in sorted(m.starts):
            if o % 4 or m.t[o] not in (None, "zero"):
                continue
            cmds = parse_shape(m, o)
            # a mesh (26 / 27 / 28), or a material list alone whose pointer is relocated
            # (fire 0x227C: {op 29: 8 mats at 0x2058, op 2}, the flame frames)
            if not cmds or not any(op in (26, 27, 28) or op == 29 and tgt(key, c + 8)
                                   for op, c in cmds):
                continue
            stats["shape streams"] += 1
            end = cmds[-1][1] + 4
            m.put(o, "u32", "shape", (end - o) // 4)
            nv = nt = 0
            for op, c in cmds:
                if op in (27, 28, 26):
                    if op == 27:
                        nv = word(m, c + 4)
                    else:
                        nt = word(m, c + 4)
            for op, c in cmds:
                a = [c + 4 + 4 * i for i in range(SHAPE_OPS.get(op, 0))]
                if op == 27 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 3 * nv, "shape:verts")
                elif op == 28 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 4 * nt, "shape:tris")
                elif op == 26 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 3 * nt, "shape:triList")
                elif op == 25 and tgt(key, a[0]):
                    # palette (+0x5C, polyshape.h): 3 bytes per vertex (RGB888, or s8 x 3 normals:
                    # charsel 0x60CD8 = (0, -127, 0) x 4, balloon 0x420)
                    typed_run(*tgt(key, a[0]), "u8", 3 * nv, "shape:palette")
                elif op == 4 and tgt(key, a[0]):
                    # uv (+0x64, BasePolyShape_addPoints): s16 (s, t) per vertex (charsel 0x60CE4,
                    # balloon 0x5DC {0x160, 0x3DE} ...).  Run 624 typed this u8 x 3 (unswapped)
                    typed_run(*tgt(key, a[0]), "u16", 2 * nv, "shape:uv")
                elif op in (7, 8) and tgt(key, a[0]):
                    typed_run(*tgt(key, a[0]), "u8", nt if op == 7 else nv, "shape:bytes")
                elif op == 9 and tgt(key, a[0]):
                    typed_run(*tgt(key, a[0]), "u32", nv, "shape:vtxColours")
                elif op == 34:
                    for i, n in ((0, nv), (1, nt)):
                        if tgt(key, a[i]):
                            typed_run(*tgt(key, a[i]), "f32", 3 * n, "shape:normals")
                elif op == 30 and tgt(key, a[1]):
                    # BasePolyShape +0x48 count / +0x6C table (polyshape.h): PolySeg {s32, u16 v[2]}
                    # records (game_libs_ncc_4A308.cpp PolyShape_makeEdgeVtxs reads v[] as u16)
                    n30 = word(m, a[0])
                    if 0 < n30 <= 0x10000:
                        for i in range(n30):
                            tk, to = tgt(key, a[1])
                            typed_run(tk, to + 8 * i, "u32", 1, "shape:polySeg")
                            typed_run(tk, to + 8 * i + 4, "u16", 2, "shape:polySeg")
                elif op == 32 and tgt(key, a[1]):
                    # remap (+0x70) is 3 u16 per entry, and its count is the command's own arg 0
                    # (+0x74), not nVerts: boarder3 0x4094 has 231 entries for 155 vertices
                    n32 = word(m, a[0])
                    typed_run(*tgt(key, a[1]), "u16", 3 * (n32 if 0 < n32 <= 0x10000 else nv), "shape:remap")
                elif op == 29 and tgt(key, a[1]):
                    walk_mats(maps, ptrs, *tgt(key, a[1]), stats)
                elif op == 35 and tgt(key, a[0]):
                    collgrid(*tgt(key, a[0]))


# libgdl node command streams (ProcNodeList -> nodelist_dispatchList_000558A0 / nodelist_walk_0005534C
# / nodelist_dispatch, game_libs_ncc_*.cpp; Node_doCommand / MNode / QNode_doCommand): words
# {argc << 16 | op, argc args} like the shape stream.  A list starts with op 1000 (the
# ProcNodeList assert) and ends at a 0 word; op 1003 {fn, ...} opens a nested walk that its own
# 0 word closes; op 1017 {list} runs a sub-list.  Op 114 (0x72) points to an s16[10] transform
# {pos x3, quat x4 / 32767, scale x3}.  The streams are found from the data (every module's
# Data / RoData, a 1000 word at a symbol / pointer target that parses to its end with known ops):
# the rider node lists (bootup Data 0xA924 and 0xABBC, SnowBoarder_loadBoardModel_0000B1B4;
# include/game/snowboarder.h) and map5_data 0x9D4 (an MNode "name" {scale, rotate, translate}
# around a 1003 shape).
NODE_OPS = {0} | set(range(100, 121)) | set(range(1000, 1020))


def parse_node(m, pm, o):
    """-> [(op, offset, argc)] of the node list at o up to its closing 0 word, or None"""
    cmds, depth = [], 0
    while o + 4 <= m.n and len(cmds) < 8192:
        if o in pm or m.t[o] == "ptr":
            return None
        w = struct.unpack_from(">I", m.data, o)[0]
        argc, op = w >> 16, w & 0xFFFF
        if op not in NODE_OPS or argc > 16 or o + 4 + 4 * argc > m.n:
            return None
        if w == 0:
            cmds.append((0, o, 0))
            if depth == 0:
                return cmds
            depth -= 1
            o += 4
            continue
        if op == 1003:
            depth += 1
        cmds.append((op, o, argc))
        o += 4 + 4 * argc
    return None


# op 1003 {fn, ...} runs fn on the stream (NodeListState); skin_cmdSkinMgr (bootup Text 0x7D280)
# reads {slot, SkinBone *table}: gdl/skin.h SkinBone {s16 count, s16 pad, SkinWeight *}, SkinWeight
# {s16 vert, s16 weight}
SKIN_CMD_SKINMGR = ("bootup", 0x7D280)


def skin_bones(maps, ptrs, tk, to, stats):
    m, pm = maps[tk], ptrs[tk]
    for i in range(64):
        e = to + 8 * i
        if e + 8 > m.n:
            break
        cnt, pad = struct.unpack_from(">hh", m.data, e)
        has = (e + 4) in pm
        if pad or cnt < 0 or (cnt > 0) != has or (not has and struct.unpack_from(">I", m.data, e + 4)[0]):
            break
        if cnt:
            m.put(e, "u16", "skin:bone", 2)
            wk, wo = pm[e + 4]
            if wk in maps:
                maps[wk].put(wo, "u16", "skin:weights", 2 * cnt)
            stats["skin bones"] += 1


def node_streams(maps, ptrs, stats, rt=None, rom=None):
    todo = [(key, o, True) for key, m in sorted(maps.items()) for o in sorted(m.starts)
            if o % 4 == 0 and o + 4 <= m.n and struct.unpack_from(">I", m.data, o)[0] == 1000]
    done = set()
    while todo:
        key, o, root = todo.pop(0)
        if (key, o) in done or key not in maps:
            continue
        done.add((key, o))
        short, sec = key
        m, pm = maps[key], ptrs[key]
        cmds = parse_node(m, pm, o)
        if not cmds:
            if not root:
                stats["node stream: did not parse"] += 1
            continue
        for op, c, argc in cmds:
            m.put(c, "u32", "nodecmd", 1 + argc)       # relocated args stay ptr (put skips them)
            if op == 114 and (c + 4) in pm and pm[c + 4][0] in maps:
                tk, to = pm[c + 4]
                maps[tk].put(to, "u16", "nodecmd:xform", 10)
            if op == 1017 and argc == 1 and (c + 4) in pm:
                todo.append(pm[c + 4] + (False,))
            if op == 1003 and argc == 3 and (c + 12) in pm and rt is not None:
                md = rom.by_short[short]
                rel = {off: f1 for f1, off, _ in md.relocs(sec)}
                t = rt.raw(md, rel[c + 4] >> 4) if (c + 4) in rel else None
                if t and (t[0].short, t[1]) == (SKIN_CMD_SKINMGR[0], "Text") and \
                        (t[2] + struct.unpack_from(">I", m.data, c + 4)[0]) & 0xFFFFFFFF == SKIN_CMD_SKINMGR[1]:
                    skin_bones(maps, ptrs, *pm[c + 12], stats)
        stats["node streams"] += 1


def tex_headers(m, bounds, stats):
    """image symbols: an 8-byte {0, 1} record at a Sym immediately followed by a second Sym, the
    texels (every module that has them: gamegfx, the boarder banks, charfir / charsps, heli,
    balloon, checkpoint, fire, snoweffects).  The header is two words (reader not located, as for
    gamegfx); the texels are bytes up to the next symbol / pointer target / typed non-byte data.
    Texels a material or a static display list reaches are typed the same way already; this also
    covers the ones code loads with a built G_SETTIMG (snoweffects *_buildTexDL)."""
    d = m.data
    for v in sorted(m.starts):
        if v % 4 or v + 8 not in m.starts or v + 8 > m.n or struct.unpack_from(">II", d, v) != (0, 1):
            continue
        if any(c not in (None, "u32") for c in m.t[v:v + 8]):
            continue
        m.put(v, "u32", "tex:hdr", 2)
        e = v + 8
        while e < m.n and (e == v + 8 or e not in bounds) and m.t[e] in (None, "u8"):
            e += 1
        # the last image of a module runs on to the next symbol: drop a zero tail of 16+ bytes
        # (it is typed zero below), so that the run does not swallow a following zero table
        z = e
        while z > v + 8 and d[z - 1] == 0:
            z -= 1
        if e - z >= 16:
            e = z
        for b in range(v + 8, e):
            if m.t[b] is None:
                m.t[b], m.src[b], m.st[b] = "u8", "tex:hdr-texels", b
        stats["texture headers"] += 1


def vtables(m, bounds, stats):
    """cfront vtable slots {s16 delta, s16 index, fn} (include/gdl/node.h GdlVtEnt, read by
    VT_SLOT and port/include/port_cfront.h CF_Vk as s16): the word before a relocated pointer to
    Text, when the slot is part of a table (another such slot 8 bytes away, or the table's
    8-byte header {0, 0} after a symbol / pointer target) and its index half is 0 (cfront
    vtables: every slot on this ROM has index 0, and nearly every delta is 0 too).  Typed as
    two u16 so a nonzero delta survives the swap."""
    d = m.data
    for p in sorted(m.text_ptrs):
        h = p - 4
        if h < 0 or any(c is not None for c in m.t[h:p]):
            continue
        w = struct.unpack_from(">I", d, h)[0]
        if w & 0xFFFF or 0x1000 <= w >> 16 < 0xF000:      # index 0, a small delta
            continue
        # a neighbour slot 8 bytes away (not a jump table: its head word is no pointer), or the
        # table's {0, 0} header after a symbol / pointer target
        if not any(q in m.text_ptrs and q - 4 not in m.text_ptrs and m.t[q - 4] != "ptr"
                   for q in (p - 8, p + 8)) and (h - 8) not in bounds:
            continue
        m.put(h, "u16", "vtbl", 2)
        stats["vtable slots"] += 1


def pmfs(m, bounds, stats, found):
    """cfront pointer-to-member records (include/gdl/pmf.h GdlPmf {s16 delta, s16 vslot, fn},
    read as s16 by the C MemberCallback_invoke family and by port_cfront.h PortCfPmf in the
    game_uso RiderAnim donors): the vtable-slot rule's other half.  A head {small delta, -1}
    (non-virtual) before a relocated pointer to Text, which the code copies into the callback /
    state objects (a code access at the head: the readers themselves go through those heap
    copies), and the null PMF {0, 0, 0} the code takes the address of next to such a record (the
    RiderAnim `!= null` compares).  The head was typed u32 by the copy; it is retyped as two u16,
    or the host reads delta -1 / index 0 (a virtual call through garbage)."""
    d = m.data
    heads = set()
    for p in sorted(m.text_ptrs):
        h = p - 4
        if h < 0 or h % 4:
            continue
        w = struct.unpack_from(">I", d, h)[0]
        if w & 0xFFFF != 0xFFFF or 0x1000 <= w >> 16 < 0xF000:      # vslot -1, a small delta
            continue
        if any(c not in (None, "u32") for c in m.t[h:p]):
            continue
        if not any(s in ("field", "access", "index") for s in m.src[h:p]) and h not in m.addr_taken:
            stats["pmf: head without a code access"] += 1
            continue
        heads.add(h)
    for h in sorted(heads):
        m.put(h, "u16", "pmf", 2, force=True)
        found.append(h)
        stats["pmf records"] += 1
    for h in sorted(m.addr_taken):
        if h % 4 or h + 8 > m.n or bytes(d[h:h + 8]) != bytes(8) or not (h - 8 in heads or h + 8 in heads):
            continue
        if any(c not in (None, "u32", "zero") for c in m.t[h:h + 8]):
            continue
        if m.src[h + 8] == "vtbl":    # a vtable's {0, 0} header (the constructors store its address)
            continue
        m.put(h, "u16", "pmf:null", 2, force=True)
        m.put(h + 4, "u32", "pmf:null", force=True)
        found.append(h)
        stats["pmf null records"] += 1


def walk_mats(maps, ptrs, key, o, stats):
    """parseMats token loop: 100 (a material: GDL_MC commands up to 0), 101, 0 ends"""
    if key not in maps:
        return
    m = maps[key]
    start = o
    while o + 4 <= m.n:
        tok = struct.unpack_from(">I", m.data, o)[0]
        o += 4
        if tok == 0:
            break
        if tok == 101:
            continue
        if tok != 100:
            stats["mat stream: bad token"] += 1
            return
        while o + 4 <= m.n:
            op = struct.unpack_from(">I", m.data, o)[0]
            o += 4
            if op == 0:
                break
            if op not in MAT_ARGS:
                stats["mat stream: unknown op"] += 1
                return
            if op == 203:
                fmt, w, h, fl = [struct.unpack_from(">I", m.data, o + 4 * i)[0] for i in (0, 2, 3, 4)]
                t = ptrs[key].get(o + 4)
                if t:
                    texture(maps, ptrs, t, fmt, w, h, fl, stats)
            o += 4 * MAT_ARGS[op]
    m.put(start, "u32", "mats", (o - start) // 4)
    stats["mat streams"] += 1


def texture(maps, ptrs, t, fmt, w, h, fl, stats):
    """texels stay bytes (Fast3D reads texels / TLUTs big-endian); records are u32 words"""
    key, o = t
    if key not in maps:
        return
    m = maps[key]
    bpp = fmt & 0xFF
    if fl & 0x10:          # image record {image, uls, ult, width, height}
        m.put(o, "u32", "tex:record", 5)
        img = ptrs[key].get(o)
        if img and img[0] in maps:
            iw, ih = [struct.unpack_from(">I", m.data, o + 4 * i)[0] for i in (3, 4)]
            texels(maps, ptrs, img, fmt, iw, ih, stats)
        return
    texels(maps, ptrs, t, fmt, w, h, stats)


def texels(maps, ptrs, t, fmt, w, h, stats):
    key, o = t
    m = maps[key]
    bpp = fmt & 0xFF
    if fmt == 0x508:       # CI record {?, ?, pixels, palette}
        m.put(o, "u32", "tex:ci-record", 4)
        for i, n in ((2, w * h), (3, 0x200)):
            p = ptrs[key].get(o + 4 * i)
            if p and p[0] in maps:
                maps[p[0]].put(p[1], "u8", "tex:texels", n)
        stats["textures"] += 1
        return
    if bpp in (4, 8, 16, 32) and 0 < w <= 1024 and 0 < h <= 1024:
        m.put(o, "u8", "tex:texels", w * h * bpp // 8)
        stats["textures"] += 1
    else:
        stats["texture: unknown format"] += 1


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--report", action="store_true")
    ap.add_argument("--out", default="port/gen/blob_types.tsv")
    a = ap.parse_args()
    rom = npc.Rom()
    pgb_spec = importlib.util.spec_from_file_location("pgb", str(ROOT / "scripts/port-gen-binds.py"))
    pgb = importlib.util.module_from_spec(pgb_spec)
    argv, sys.argv = sys.argv, [sys.argv[0]]
    pgb_spec.loader.exec_module(pgb)
    sys.argv = argv
    rt = pgb.RomTargets(rom)
    keep = []
    for line in open("port/gen/uso_blobs.tsv"):
        if line.strip() and not line.startswith("#"):
            short, _, sec, size = line.split("\t")
            if sec in SECS:
                keep.append((short, sec))
    maps = {}
    for short, sec in keep:
        m = Map(rom.by_short[short].secs[sec])
        m.idx_hints = []
        m.text_ptrs = set()
        m.addr_taken = set()
        maps[(short, sec)] = m
    stats = collections.Counter()
    # 1. relocated words + audio ranges first: they win
    reloc_offs = {}
    targets = collections.defaultdict(set)
    for (short, sec), m in maps.items():
        md = rom.by_short[short]
        offs = set()
        for f1, off, _ in md.relocs(sec):
            if f1 & 7 == 4:
                offs.add(off)
                m.put(off, "ptr", "reloc", force=True)
                t = rt.raw(md, f1 >> 4)
                if t and t[1] == "Text":
                    m.text_ptrs.add(off)
                if t and t[1] in SECS:
                    k = struct.unpack_from(">I", m.data, off)[0]
                    targets[(t[0].short, t[1])].add((t[2] + k) & 0xFFFFFFFF)
        reloc_offs[(short, sec)] = offs
        if short == "bootup" and sec == "Data":
            for lo, hi in AUDIO_RANGES:
                for b in range(lo, hi):
                    m.t[b], m.src[b] = "audio", "audio"
    # 2. code accesses (every module's Text, imports followed)
    for md in rom.mods:
        analyse_text(md, rt, maps, stats)
    call_arrays(rom, rt, maps, stats)
    # 3. display lists
    for (short, sec), m in maps.items():
        vtx, mtx = gfx_walk(rom.by_short[short], sec, m, reloc_offs[(short, sec)], stats)
        md = rom.by_short[short]
        rel = {off: f1 for f1, off, _ in md.relocs(sec)}
        for off, n in vtx + [(o, None) for o in mtx]:
            t = rt.raw(md, rel[off] >> 4)
            if not t or (t[0].short, t[1]) not in maps:
                continue
            tm = maps[(t[0].short, t[1])]
            p = (t[2] + struct.unpack_from(">I", m.data, off)[0]) & 0xFFFFFFFF
            if n is None:
                tm.put(p, "u32", "gfx:Mtx", 16)
                stats["gfx: Mtx"] += 1
            else:
                for i in range(n):
                    tm.put(p + 16 * i, "u16", "gfx:Vtx", 6)
                    tm.put(p + 16 * i + 12, "u8", "gfx:Vtx", 4)
                stats["gfx: Vtx"] += n
    # 3b. asset streams: shapes, materials, textures
    ptrs = {}
    for (short, sec), m in maps.items():
        md = rom.by_short[short]
        pm = {}
        for f1, off, _ in md.relocs(sec):
            if f1 & 7 != 4:
                continue
            t = rt.raw(md, f1 >> 4)
            if t and t[1] in SECS:
                pm[off] = ((t[0].short, t[1]), (t[2] + struct.unpack_from(">I", m.data, off)[0]) & 0xFFFFFFFF)
        ptrs[(short, sec)] = pm
        m.starts = set(targets[(short, sec)]) | m.addr_taken
        for info, v, w3 in md.syms:
            if (info >> 16) & 7 == 2 and UI.SEC_OF_TAG.get((info >> 20) & 0xF) == sec:
                m.starts.add(v)
    walk_assets(maps, ptrs, stats)
    node_streams(maps, ptrs, stats, rt, rom)
    for (short, sec), m in maps.items():
        bounds = m.starts | set(targets[(short, sec)])
        tex_headers(m, bounds, stats)
        vtables(m, bounds, stats)
        m.pmfs = []
        pmfs(m, bounds, stats, m.pmfs)
    # 4. strings, index extension, zeros
    for (short, sec), m in maps.items():
        md = rom.by_short[short]
        bounds = set(targets[(short, sec)])
        for info, v, w3 in md.syms:
            k = (info >> 16) & 7
            if k == 2 and UI.SEC_OF_TAG.get((info >> 20) & 0xF) == sec:
                bounds.add(v)
        d = m.data
        i = 0
        while i < m.n:
            if m.t[i] is None and (0x20 <= d[i] < 0x7F or d[i] == 10) and (i % 4 == 0 or i in bounds):
                j = i
                while j < m.n and (0x20 <= d[j] < 0x7F or d[j] in (9, 10, 13, 0x1B)) and m.t[j] in (None, "u8"):
                    j += 1
                # 4+ characters anywhere; 1-3 at an address the code takes or a pointer holds
                # (parm tags "d00" / "he1" / "Env", formats "%s" / "%d_", viewrec "fff")
                # (a leading "\n" too: "\nContents of %s", the game.uso "\nDAM" / "\nNULL\n" tags)
                npr = sum(1 for b in d[i:j] if 0x20 <= b < 0x7F)
                if j < m.n and d[j] == 0 and npr and (d[i] != 10 or npr >= 3) and \
                        (j - i >= 4 or j > i and i in m.addr_taken | bounds):
                    for b in range(i, j + 1):
                        if m.t[b] in (None, "u8"):
                            m.t[b], m.src[b] = "str", "str"
                    stats["strings"] += 1
                    i = j + 1
                    continue
            i += 1
        for a0, typ in sorted(set(m.idx_hints)):
            w = WIDTH[typ]
            p = a0 + w
            while p + w <= m.n and p not in bounds and all(m.t[p + b] is None for b in range(w)):
                if typ in ("f32", "f64") and not plausible(d, p, typ):
                    break
                for b in range(w):
                    m.t[p + b], m.src[p + b] = typ, "index"
                p += w
        if sec == "RoData":
            # RoData literal-pool words no access typed (genproc 0x160 0.1f, 0x240 1e7f, 0x390
            # 0.3f; bootup 0xD98 32 / 255.0f): f32 when the value is a float near 1 that is not text
            for o in range(0, m.n - 3, 4):
                if any(m.t[o + b] is not None for b in range(4)) or not any(d[o:o + 4]):
                    continue
                w = struct.unpack_from(">I", d, o)[0]
                if 127 - 30 <= (w >> 23) & 0xFF <= 127 + 30 and plausible(d, o, "f32"):
                    for b in range(4):
                        m.t[o + b], m.src[o + b], m.st[o + b] = "f32", "value:f32", o
                    stats["rodata f32 values"] += 1
        b = 0
        while b < m.n:     # zero runs of 8+ bytes; shorter ones stay inside their unknown run
            e = b
            while e < m.n and m.t[e] is None and d[e] == 0:
                e += 1
            if e - b >= 8:
                for z in range(b, e):
                    m.t[z], m.src[z] = "zero", "zero"
            b = e + 1 if e == b else e
    # 4b. known layouts
    for short, sec, base, fields, count, stride, src in KNOWN:
        m = maps.get((short, sec))
        if m is None:
            continue
        lo, hi = base, base + count * stride
        m.conf = [c for c in m.conf if not lo <= c[0] < hi]
        for r in range(count):
            for fo, size, typ in fields:
                o = base + r * stride + fo
                w = WIDTH.get(typ, 1)
                for e in range(o, o + size, w):
                    if typ != "u8" and any(m.t[e + b] in ("ptr", "audio") for b in range(w)):
                        continue      # relocated words keep their host-order write
                    for b in range(w):
                        m.t[e + b], m.src[e + b], m.st[e + b] = typ, src, e
        stats["known layouts"] += 1
    # 5. write runs
    out = ["# generated by scripts/port-gen-blob-types.py -- do not edit.  Plan \"M1/M2: byte order\".",
           "# module\tsection\toffset\tsize\ttype\tsource   (types u8 str zero ptr audio conflict: not swapped; u16 u32 f32 u64 f64: swapped per element)"]
    cov = collections.Counter()
    covmod = {}
    conflicts = []
    for (short, sec), m in sorted(maps.items()):
        b = 0
        mc = collections.Counter()
        while b < m.n:
            t = m.t[b] or "unknown"
            s = m.src[b] or "-"
            e = b + 1
            while e < m.n and (m.t[e] or "unknown") == t and (m.src[e] or "-") == s:
                e += 1
            w = WIDTH.get(t, 1)
            if w > 1:   # align the run to whole elements
                e = b + (e - b) // w * w
                if e == b:
                    t, e = "conflict", b + 1
                    m.t[b] = "conflict"
            out.append("%s\t%s\t0x%X\t0x%X\t%s\t%s" % (short, sec, b, e - b, t, s))
            cov[t] += e - b
            mc[t] += e - b
            b = e
        covmod[(short, sec)] = mc
        conflicts += [(short, sec) + c for c in m.conf]
    Path(a.out).write_text("\n".join(out) + "\n")
    total = sum(cov.values())
    print("blob bytes %d; by type: %s" % (total, ", ".join("%s %d (%.1f%%)" % (k, v, 100.0 * v / total) for k, v in cov.most_common())))
    print("analysis: %s" % dict(stats))
    print("conflict bytes: %d (%d first-seen sites)" % (cov["conflict"], len(conflicts)))
    if a.report:
        for (short, sec), mc in sorted(covmod.items(), key=lambda x: -x[1]["unknown"]):
            n = sum(mc.values())
            print("  %-10s %-6s %7d  unknown %6d (%.0f%%)  %s" % (short, sec, n, mc["unknown"], 100.0 * mc["unknown"] / n,
                  " ".join("%s=%d" % kv for kv in mc.most_common(5))))
        for (short, sec), m in sorted(maps.items()):
            if m.pmfs:
                print("  pmf %s %s: %s" % (short, sec, " ".join("0x%X" % h for h in m.pmfs)))
        for c in conflicts[:60]:
            print("  conflict %s %s +0x%X: %s vs %s (%s / %s)" % c)
    return 0


if __name__ == "__main__":
    sys.exit(main())
