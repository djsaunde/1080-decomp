/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xc14 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
h2hproc_C18_alloc = 0x0;
h2hproc_C18_bind = 0x0;
h2hproc_C18_create = 0x0;
h2hproc_C18_data_Sym140 = 0x0;
h2hproc_C18_finish = 0x0;
h2hproc_C18_init = 0x0;
h2hproc_C18_link = 0x0;
h2hproc_C18_make0 = 0x0;
h2hproc_C18_make1 = 0x0;
