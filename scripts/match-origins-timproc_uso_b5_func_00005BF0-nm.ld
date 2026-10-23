/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x5bf0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
import_0024CCF4 = 0x0;
import_80020150 = 0x0;
import_80074E78 = 0x0;
timproc_uso_b5_D_807FE6C4 = 0x0;
timproc_uso_b5_D_807FE6E4 = 0x0;
timproc_uso_b5_D_807FE6E4_b = 0x0;
timproc_uso_b5_func_00003A28 = 0x0;
timproc_uso_b5_func_00008688 = 0x0;
timproc_uso_b5_func_00008988 = 0x0;
timproc_uso_b5_func_00008A38_entry = 0x0;
