/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x42a18 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
gl_2F288_countdown = 0x0;
gl_2F288_countdown_reset = 0x0;
gl_2F288_countdown_store = 0x0;
gl_2F288_emit_byte = 0x0;
gl_2F288_emit_float = 0x0;
gl_2F288_pool1730 = 0x0;
gl_2F288_pool1734 = 0x0;
gl_2F288_pool1738 = 0x0;
gl_2F288_pool173C = 0x0;
gl_2F288_random = 0x0;
gl_2F288_tableC330 = 0x0;
gl_2F288_tableC3B0 = 0x0;
