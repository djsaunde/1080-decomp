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

#endif
