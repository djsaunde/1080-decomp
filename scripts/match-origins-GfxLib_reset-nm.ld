/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x58918 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
D_44918_arg0c = 0x0;
D_44918_flt = 0x0;
D_44918_fzero = 0x0;
D_44918_ma = 0x0;
D_44918_mb = 0x0;
gl_44918_init_43EAC = 0x0;
gl_44918_notify_4DD4C = 0x0;
gl_44918_range_3523C = 0x0;
gl_44918_reset_4E00C = 0x0;
gl_44918_reset_56814 = 0x0;
gl_44918_set_46788 = 0x0;
gl_44918_yield_732C4 = 0x0;
