/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x4d700 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x1a40 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
D_39094_a1 = 0x0;
D_39094_a2a = 0x0;
D_39094_a2b = 0x0;
D_39094_a3 = 0x0;
D_39094_a4 = 0x0;
D_39094_a5a = 0x0;
D_39094_a5b = 0x0;
D_39094_a6a = 0x0;
D_39094_a7 = 0x0;
D_39094_a8b = 0x0;
D_39094_dat = 0x0;
snw_39094_apply = 0x0;
snw_39094_enable = 0x0;
snw_39094_mask = 0x0;
snw_39094_push = 0x0;
snw_39094_release = 0x0;
snw_39094_transform = 0x0;
snw_39094_translate = 0x0;
