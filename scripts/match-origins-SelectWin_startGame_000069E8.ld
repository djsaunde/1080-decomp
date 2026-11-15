/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x69e8 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
import_000A53B4 = 0x0;
import_80000310 = 0x0;
import_8002013C = 0x0;
import_80020150 = 0x0;
timproc_uso_b5_69E8_global = 0x0;
timproc_uso_b5_69E8_print = 0x0;
timproc_uso_b5_D_807FE794 = 0x0;
timproc_uso_b5_D_807FEB74 = 0x0;
timproc_uso_b5_func_00F954 = 0x0;
