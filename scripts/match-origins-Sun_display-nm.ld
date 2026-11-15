/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xb274 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
game_uso_B274_context = 0x0;
game_uso_B274_set_prim_color = 0x0;
game_uso_D_807FFA40 = 0x0;
game_uso_D_807FFA50 = 0x0;
game_uso_D_807FFA58 = 0x0;
game_uso_func_04D700 = 0x0;
game_uso_func_063734 = 0x0;
import_000F8678 = 0x0;
import_8005C1A8 = 0x0;
