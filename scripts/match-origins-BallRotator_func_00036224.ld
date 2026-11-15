/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x4a224 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_36224_between = 0x0;
gl_36224_conjugate = 0x0;
gl_36224_direction = 0x0;
gl_36224_matrix = 0x0;
gl_36224_product = 0x0;
gl_36224_rotation = 0x0;
gl_36224_translate = 0x0;
