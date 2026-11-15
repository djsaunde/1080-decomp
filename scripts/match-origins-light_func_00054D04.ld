/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x68d04 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_54d04_bias = 0x0;
gl_54d04_get = 0x0;
gl_54d04_normalize = 0x0;
gl_54d04_project = 0x0;
gl_54d04_report = 0x0;
gl_54d04_root = 0x0;
gl_54d04_scale = 0x0;
gl_54d04_strings = 0x0;
