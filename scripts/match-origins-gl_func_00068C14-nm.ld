/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x7cc9c : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
gl_68C14_append = 0x0;
gl_68C14_assert = 0x0;
gl_68C14_check = 0x0;
gl_68C14_create = 0x0;
gl_68C14_report = 0x0;
gl_68C14_table = 0x0;
