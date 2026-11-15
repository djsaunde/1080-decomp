#ifndef GDL_GL_DATA_H
#define GDL_GL_DATA_H

/* game_libs (bootup.uso) data accesses: the `&D_00000000 + K` sites.
 *
 * The decomp links each USO at VRAM 0 and the ROM holds the PRE-relocation
 * instruction words, so every data reference is spelled off the zero
 * placeholder D_00000000. What K means depends on which bootup.uso Sym entry
 * the TextReloc at that instruction names (READABILITY_PLAN section 9,
 * "USO section-base accesses"):
 *
 *   Sym tag byte 0x?4 = a SECTION BASE (0x54 Text, 0x64 Data, 0x74 RoData,
 *     0x84 Bss; value 0): K is the offset inside that section. Two sites with
 *     the same K but different sections are different objects.
 *   Sym tag byte 0x?2 = a named OBJECT (0x62 Data, 0x72 RoData, 0x82 Bss;
 *     value = its section offset): the instruction immediates are 0, so K is
 *     only the field offset inside the object, and the object's identity is
 *     in the reloc table, not in the code.
 *
 * So no single struct can be overlaid at D_00000000. Instead, each object
 * gets a NAMED ACCESSOR MACRO whose expansion is exactly the expression the
 * matched body already used. The preprocessed source is then unchanged up to
 * redundant parentheses, which is what makes the conversion byte-safe; the
 * name records the reloc evidence (section, offset, Sym index).
 *
 * D_00000000 is declared per TU (int / char / void *); the macros only take
 * its address, so they work with any of those declarations. */

/* a site relocated against the Data section base Sym (tag 0x640000, sym 2) */
#define GL_DATA(off) ((char *)&D_00000000 + (off))

/* ---- named bootup.uso objects (agent-a run 479; READABILITY_PLAN 9.1) ----
 *
 * OBJECT ALIASES: the name stands for the placeholder SYMBOL itself, so every
 * spelling the matched bodies use stays token-identical after preprocessing:
 *   &gHostio                  == &D_00000000
 *   (char *)&gGfxLib + 0xA0   == (char *)&D_00000000 + 0xA0
 *   INPUT_MGR(gInputMgr).pads == INPUT_MGR(D_00000000).pads   (game/input.h)
 * Only sites spelled with plain D_00000000 can use them. Per-site zero aliases
 * (D_415A4_s1, D_00000000_3c740, gl_d_4c5e4_flag, ...) exist to defeat IDO's
 * address CSE and must keep their own symbol; they are listed against their
 * object in READABILITY_PLAN 9.1, not replaced.
 *
 * Every comment names the bootup.uso Sym the TextRelocs point at.
 * `scripts/uso-data-sites.py check` verifies that each function using one of
 * these names really has a site relocated against that Sym; run it after a
 * conversion (with a built tree), then the readability gate.
 *
 * Sizes are inferred from the next named object in the section. */
#define gAudioContext D_00000000 /* Bss+0x345C0 Sym 1255, 0x5C30 bytes: the audio driver context (OoT gAudioContext; 133 AudioHeap/AudioLoad/AudioSeq functions) */
#define gGfxLib       D_00000000 /* Bss+0x3C168 Sym 154, 0x5D8 bytes: the GfxLib global (docs/1080_GRAPHICS.md section 2: +0xA0 modelview, +0x1C4 flags, +0x254 current ScreenWindow) */
#define gSysLib       D_00000000 /* Bss+0x3C740 Sym 73, 0x28 bytes: the system-services object passed as `this` to heap_tempAlloc*, the fs_gdlRead* parm readers, pad_testButton, sys_timeInFrames (169 functions); App_main queries its memory range first. Role inferred from callers */
#define gHostio       D_00000000 /* Bss+0x3C768 Sym 338, 8 bytes: the HostIO connection, first argument of every hostio_beginGroup/add/endGroup call in the 47 <Class>_hostioParms functions */
#define gInputMgr     D_00000000 /* Bss+0xF8 Sym 272, 0x194 bytes: the InputMgr (include/game/input.h; 79 functions) */

#endif
