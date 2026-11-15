/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x52a08 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_E39C_create = 0x0;
gl_E39C_data = 0x0;
gl_E39C_find = 0x0;
gl_E39C_finish = 0x0;
gl_E39C_missing = 0x0;
gl_E39C_pop = 0x0;
gl_E39C_push = 0x0;
gl_E39C_read = 0x0;
gl_E39C_reset = 0x0;
gl_E39C_seek = 0x0;
gl_E39C_stream = 0x0;
snw_3E39C_count_origin = 0x51630;
