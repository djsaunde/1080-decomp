/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x54cac : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x1b00 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl40640_context = 0x0;
gl40640_copy = 0x0;
gl40640_current = 0x0;
gl40640_mask = 0x0;
gl40640_mul = 0x0;
gl40640_pop = 0x0;
gl40640_push = 0x0;
gl40640_reset = 0x0;
gl40640_translate = 0x0;
