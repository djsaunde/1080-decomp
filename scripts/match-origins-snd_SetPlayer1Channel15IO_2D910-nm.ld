/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x410a0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_0001C1BC = 0x0;
D_0001C1BC_b = 0x0;
D_0001C1C0 = 0x0;
D_0001C1C0_b = 0x0;
D_0001C1C4 = 0x0;
D_0001C1C4_b = 0x0;
D_0001C1C4_c = 0x0;
D_0001C1C4_d = 0x0;
D_0001C1C4_e = 0x0;
D_0001C1C4_f = 0x0;
D_0001C1C4_g = 0x0;
D_0001C1C4_h = 0x0;
D_0001C1C4_i = 0x0;
D_0001C1C4_j = 0x0;
D_0001C1C8 = 0x0;
D_0001C1C8_b = 0x0;
D_0001C1C8_c = 0x0;
D_0001C1CC = 0x0;
D_0001C1CC_b = 0x0;
D_0001C1CC_c = 0x0;
D_0001C1CC_d = 0x0;
D_0001C1CC_e = 0x0;
D_0001C1CC_f = 0x0;
D_0001C1CC_g = 0x0;
D_0001C1CC_h = 0x0;
D_0001C1CC_i = 0x0;
D_0001C1CC_j = 0x0;
D_0001C1CC_k = 0x0;
D_0001C1D0 = 0x0;
D_0001C1D0_b = 0x0;
D_0001C1D0_c = 0x0;
D_0001C1D0_d = 0x0;
D_0001C1D0_e = 0x0;
D_0001C1D0_f = 0x0;
D_0001C1D0_g = 0x0;
D_0001C1D0_h = 0x0;
D_0001C1D4 = 0x0;
D_0001C1D4_b = 0x0;
D_0001C1D4_c = 0x0;
D_0001C1D4_d = 0x0;
D_0001C1D4_e = 0x0;
D_0001C1D4_f = 0x0;
D_0001C1D4_g = 0x0;
gl_2D910_mode = 0x0;
gl_2D910_ready = 0x0;
gl_2D910_set_control = 0x0;
gl_2D910_set_control_byte = 0x0;
