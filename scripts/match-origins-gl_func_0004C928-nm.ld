/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x60928 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_4C928_find = 0x0;
gl_4C928_log = 0x0;
gl_4C928_mode = 0x0;
gl_4C928_poll = 0x0;
gl_4C928_process = 0x0;
gl_4C928_queue_base = 0x0;
gl_4C928_result_base = 0x0;
gl_4C928_send = 0x0;
gl_4C928_state_base = 0x0;
gl_4C928_strings_base = 0x0;
