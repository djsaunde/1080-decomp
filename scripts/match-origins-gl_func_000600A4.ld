/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x740a4 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_600A4_alloc = 0x0;
gl_600A4_data = 0x0;
gl_600A4_diag = 0x0;
gl_600A4_init = 0x0;
gl_600A4_lookup = 0x0;
gl_600A4_read = 0x0;
gl_600A4_root = 0x0;
