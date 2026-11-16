/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x8124 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
FloatParm_vtbl_7540 = 0x0;
Fog_vtbl_7878 = 0x0;
sStr_Parms_7FC4 = 0x0;
sTag_fin_7FCC = 0x0;
sTag_rng_7FD0 = 0x0;
boot8124_alloc = 0x0;
boot8124_child = 0x0;
boot8124_link = 0x0;
boot8124_object_init = 0x0;
boot8124_tail_init = 0x0;
