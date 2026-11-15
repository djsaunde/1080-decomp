/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x1460 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
import_8005C108 = 0x0;
timproc_uso_b5_D_807FE8FC = 0x0;
timproc_uso_b5_D_807FE90C = 0x0;
timproc_uso_b5_D_807FE918 = 0x0;
timproc_uso_b5_func_062880 = 0x0;
timproc_uso_b5_func_068D54 = 0x0;
timproc_uso_b5_func_07ACE0 = 0x0;
