/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x41c0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
game_uso_D_41C0_last = 0x0;
game_uso_D_807FE9B8 = 0x0;
game_uso_D_807FEA00 = 0x0;
game_uso_D_807FEB68 = 0x0;
game_uso_D_807FECA0 = 0x0;
game_uso_D_807FECB4 = 0x0;
game_uso_D_807FECB8 = 0x0;
game_uso_D_807FECBC = 0x0;
game_uso_func_04A188 = 0x0;
game_uso_func_055750 = 0x0;
game_uso_func_07ACE0 = 0x0;
uso_base_init_051C28 = 0x0;
