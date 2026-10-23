/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xd440 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000980 = 0x0;
bootup_D440_000610 = 0x0;
bootup_D440_000A50 = 0x0;
bootup_D440_000A94 = 0x0;
bootup_D440_00BF8C = 0x0;
bootup_D440_02258C = 0x0;
bootup_D440_0225B8 = 0x0;
bootup_D440_0225F8 = 0x0;
bootup_D440_054FE0 = 0x0;
bootup_D440_062380 = 0x0;
bootup_D440_062388 = 0x0;
bootup_D440_075E90 = 0x0;
bootup_D440_0779E8 = 0x0;
bootup_D440_desc0 = 0x0;
bootup_D440_desc1 = 0x0;
bootup_D440_desc2 = 0x0;
bootup_D440_desc3 = 0x0;
bootup_D440_desc4 = 0x0;
bootup_D440_desc5 = 0x0;
bootup_D440_desc6 = 0x0;
bootup_D440_desc7 = 0x0;
bootup_D440_desc8 = 0x0;
bootup_D440_desc9 = 0x0;
bootup_D440_root0 = 0x0;
bootup_D440_root1 = 0x0;
bootup_D440_root2 = 0x0;
