/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xc98c : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
import_80020098 = 0x0;
import_8002022C = 0x0;
timproc_uso_b5_D_807FF640 = 0x0;
timproc_uso_b5_D_807FF700 = 0x0;
timproc_uso_b5_D_807FFB50 = 0x0;
timproc_uso_b5_func_00003FF8 = 0x0;
timproc_uso_b5_func_000040BC = 0x0;
timproc_uso_b5_func_01EEE4 = 0x0;
timproc_uso_b5_func_01EFC8 = 0x0;
