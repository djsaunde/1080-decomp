#include "common.h"

/* kernel 0x80009E60..0x80009FD0 (ROM 0xAE60..0xAFD0): data, not code.
 *
 * Splat filed this span as .rodata and named two of its labels func_*;
 * it is the USO loader's tables and printf strings. It is written here as
 * C data. IDO 7.1 always puts address-initialised objects (even `const`
 * ones) in .data, and the tables lead the strings in ROM, so the whole span
 * is one .data section: tenshoe.ld links kernel_026.c.o(.data) first in the
 * kernel data block, ahead of kernel.data.bin at 0x80009FD0. The strings are
 * plain `char[]` (also .data). IDO 4-aligns each array, which reproduces the
 * ROM's zero padding between strings; the section's 16-byte alignment gives
 * the trailing 4 zero bytes.
 *
 * Names: func_80009EA0 / func_80009EB0 keep their names because kernel code
 * jals those addresses (kdebug_clear_IP6, __osDevMgrMain, kernel_000_c.c). The
 * linker resolves those calls to these labels, so renaming would break the
 * link. The objdiff target (expected/src/kernel/kernel_026.c.o) is built from
 * the INCLUDE_ASM .s files in asm/nonmatchings/kernel/, which carry the same
 * labels and section. */

typedef struct {
    const char *name;
    u32 romStart;
    u32 romEnd;
    u32 vram;
    u32 unk10;
} KernelUsoEntry; /* size 0x14 */

extern char sStr_mainuso_8000A590[];
extern char sStr_rom_8000A598[];
extern char sStr_Info_8000A5B0[];
extern char sStr_Sym_8000A5B8[];
extern char sStr_Text_Reloc_8000A5BC[];
extern char sStr_Data_Reloc_8000A5C8[];
extern char sStr_RoData_Reloc_8000A5D4[];
extern char sStr_Text_8000A5E4[];
extern char sStr_Data_8000A5EC[];
extern char sStr_RoData_8000A5F4[];
extern char sStr_Bss_8000A5FC[];
extern char sStr_EntrySym_8000A600[];
extern char sStr_EndOfFile_8000A60C[];
extern char sStr_End_8000A618[];
extern char sStr_BinaryDataSection_8000A61C[];
extern char sStr_DiskInfo_8000A630[];

const KernelUsoEntry k_gFsDevices[] = {
    { sStr_mainuso_8000A590, 0x00D9FDB0, 0x00E984B0, 0x8001FFA0, 0 },
    { sStr_rom_8000A598, 0x0000B7C0, 0x00D9FDB0, 0, 0 },
    { NULL, 0, 0, 0, 0 },
};
/* UNNAMED: unknown-role the zero word after k_gFsDevices' 3 entries (0x3C bytes) and before func_80009EA0; no kernel code references it (nor its runtime alias 0x8000A29C) */
u32 D_80009E9C = 0;

const char *const func_80009EA0[] = {
    sStr_Info_8000A5B0, sStr_Sym_8000A5B8, sStr_Text_Reloc_8000A5BC, sStr_Data_Reloc_8000A5C8,
};

const char *const func_80009EB0[] = {
    sStr_RoData_Reloc_8000A5D4, sStr_Text_8000A5E4, sStr_Data_8000A5EC, sStr_RoData_8000A5F4, sStr_Bss_8000A5FC,
    sStr_EntrySym_8000A600, sStr_EndOfFile_8000A60C, sStr_End_8000A618, sStr_BinaryDataSection_8000A61C, sStr_DiskInfo_8000A630,
    NULL,       NULL,       NULL,
};

char sFmt_Text_range_80009EE4[] = "%s: Text=%x-%x\n";
char sFmt_slot_line_80009EF4[] = "%d = %x,%s\n";
char sFmt_USO_unloading_80009F00[] = "USO: unloading %s\n";
char sFmt_USO_loading_80009F14[] = "USO: loading %s\n";
char sStr_rb_80009F28[] = "rb";
char sFmt_USO_Version_80009F2C[] = "USO Version: %s\n";
char sStr_3_1_80009F40[] = "3.1";
char sStr_unknown_80009F44[] = "unknown";
char sFmt_MemUsed_80009F4C[] = "MemUsed = %d\n";
char sFmt_section_bytes_80009F5C[] = "%s = %d\n";
char sStr_rb_80009F68[] = "rb";
char sFmt_unkown_reloc_type_80009F6C[] = "unkown reloc type %x\n"; /* sic: "unkown" in ROM */
char sFmt_USO_UnCompress_80009F84[] = "USO: UnCompress %d to %d\n";
char sStr_cross_referenced_symbol_section_not_loaded_80009FA0[] = "cross referenced symbol section not loaded";
