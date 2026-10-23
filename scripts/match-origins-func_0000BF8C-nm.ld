/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0xbf8c : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
bootup_BF8C_call0 = 0x0;
bootup_BF8C_call1 = 0x0;
bootup_BF8C_call2 = 0x0;
bootup_BF8C_call3 = 0x0;
bootup_BF8C_call4 = 0x0;
bootup_BF8C_call5 = 0x0;
bootup_BF8C_call6 = 0x0;
bootup_BF8C_call7 = 0x0;
bootup_BF8C_constants = 0x0;
bootup_BF8C_double = 0x4c;
bootup_BF8C_ref20 = 0x0;
bootup_BF8C_ref2C = 0x0;
