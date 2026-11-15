/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xd900 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x4 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000988 = 0x0;
bu_d900_a68 = 0x0;
bu_d900_cae8 = 0x0;
bu_d900_desc8aec = 0x0;
bu_d900_desc8b44 = 0x0;
bu_d900_game11258 = 0x0;
bu_d900_game112e0 = 0x0;
bu_d900_game6e88 = 0x0;
bu_d900_game6ecc = 0x0;
bu_d900_game6f28 = 0x0;
bu_d900_gl34810 = 0x0;
bu_d900_gl3eb3c = 0x0;
bu_d900_gl61478 = 0x0;
bu_d900_gl65360 = 0x0;
bu_d900_gl65c54 = 0x0;
bu_d900_global = 0x0;
