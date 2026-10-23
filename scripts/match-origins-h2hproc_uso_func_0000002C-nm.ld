/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x28 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
h2hproc_2C_call_0000020C = 0x0;
h2hproc_2C_call_00000274 = 0x0;
h2hproc_2C_call_00000620 = 0x0;
h2hproc_2C_import_10 = 0x0;
h2hproc_2C_import_11 = 0x0;
h2hproc_2C_import_13 = 0x0;
h2hproc_2C_import_15 = 0x0;
h2hproc_2C_import_16 = 0x0;
h2hproc_2C_import_9 = 0x0;
