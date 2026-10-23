/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xd550 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
d550_ee6c_l1 = 0x0;
d550_ee6c_l2 = 0x0;
d550_ee6c_l3 = 0x0;
d550_ee6c_l4 = 0x0;
d550_ee6c_s1 = 0x0;
d550_ee6c_s2 = 0x0;
d550_ee6c_s3 = 0x0;
import_000A12EC = 0x0;
import_80087FB8 = 0x0;
import_80807FB8 = 0x0;
timproc_uso_b5_D_807FDB64 = 0x0;
timproc_uso_b5_D_807FEE60 = 0x0;
timproc_uso_b5_D_807FEE68 = 0x0;
timproc_uso_b5_D_807FEE74 = 0x0;
timproc_uso_b5_D_807FEE7C = 0x0;
timproc_uso_b5_D_807FEE84 = 0x0;
timproc_uso_b5_func_00002B74 = 0x0;
timproc_uso_b5_func_000032C8 = 0x0;
timproc_uso_b5_func_00003890 = 0x0;
timproc_uso_b5_func_04C678 = 0x0;
timproc_uso_b5_func_04DFFC = 0x0;
timproc_uso_b5_func_0546DC = 0x0;
timproc_uso_b5_func_055750 = 0x0;
timproc_uso_b5_func_07ACE0 = 0x0;
