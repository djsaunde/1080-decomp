/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x577c0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
gl_437C0_alloc_410E4 = 0x0;
gl_437C0_ctor_table = 0x0;
gl_437C0_enable_430D8 = 0x0;
gl_437C0_finish_57700 = 0x0;
gl_437C0_init_517E4 = 0x0;
gl_437C0_lightA = 0x0;
gl_437C0_lightB = 0x0;
gl_437C0_lightC = 0x0;
gl_437C0_light_45CB0 = 0x0;
gl_437C0_link_43BEC = 0x0;
gl_437C0_physical_6C384 = 0x0;
gl_437C0_reset_56814 = 0x0;
gl_437C0_update_454C4 = 0x0;
gl_437C0_value_table = 0x0;
