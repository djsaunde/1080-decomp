/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xd14c : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
d14c_ee64_ld = 0x0;
d14c_ee64_st = 0x0;
d14c_ee6c_l1 = 0x0;
d14c_ee6c_l2 = 0x0;
d14c_ee6c_l3 = 0x0;
d14c_ee6c_l4 = 0x0;
d14c_ee6c_s1 = 0x0;
d14c_ee6c_s2 = 0x0;
d14c_ee6c_s3 = 0x0;
d14c_fb8_a = 0x0;
d14c_fb8_b = 0x0;
d14c_fb8_c = 0x0;
import_0010D33C = 0x0;
import_80020078 = 0x0;
import_80087FB8 = 0x0;
import_80800078 = 0x0;
import_80807FB8 = 0x0;
timproc_uso_b5_D_807FDAB8 = 0x0;
timproc_uso_b5_D_807FDB4C = 0x0;
timproc_uso_b5_D_807FDB64 = 0x0;
timproc_uso_b5_D_807FDDEC = 0x0;
timproc_uso_b5_D_807FEE30 = 0x0;
timproc_uso_b5_D_807FEE38 = 0x0;
timproc_uso_b5_D_807FEE48 = 0x0;
timproc_uso_b5_D_807FEE54 = 0x0;
timproc_uso_b5_d14c_alloc = 0x0;
timproc_uso_b5_func_00002B74 = 0x0;
timproc_uso_b5_func_000032C8 = 0x0;
timproc_uso_b5_func_00003890 = 0x0;
timproc_uso_b5_func_00B1B4 = 0x0;
timproc_uso_b5_func_04C678 = 0x0;
timproc_uso_b5_func_04DFFC = 0x0;
timproc_uso_b5_func_0546DC = 0x0;
timproc_uso_b5_func_077574 = 0x0;
timproc_uso_b5_func_077C44 = 0x0;
timproc_uso_b5_func_07ACE0 = 0x0;
