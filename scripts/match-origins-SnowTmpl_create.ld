/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x84a0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00007540 = 0x0;
D_00007968 = 0x0;
D_00007FE4 = 0x0;
D_00007FEC = 0x0;
D_00007FF0 = 0x0;
bootup_84A0_expected_07620 = 0x0;
bootup_84A0_func_04A188 = 0x0;
bootup_84A0_func_051C28 = 0x0;
bootup_84A0_func_055750 = 0x0;
bootup_84A0_func_074E8 = 0x0;
bootup_84A0_func_07ACE0 = 0x0;
bootup_84A0_singleton = 0x0;
