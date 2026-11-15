/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x27e8 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_00000000 = 0x0;
bootup_27E8_func_002014 = 0x0;
bootup_27E8_func_002060 = 0x0;
bootup_27E8_func_002080 = 0x0;
bootup_27E8_func_0020A4 = 0x0;
bootup_27E8_func_002C94 = 0x0;
bootup_27E8_func_002DA4 = 0x0;
bootup_27E8_func_002F90 = 0x0;
bootup_27E8_func_006808 = 0x0;
bootup_27E8_func_0118E4 = 0x0;
bootup_27E8_func_048E7C = 0x0;
bootup_27E8_func_0497D0 = 0x0;
bootup_27E8_func_04C678 = 0x0;
bootup_27E8_func_050E80 = 0x0;
bootup_27E8_func_074710 = 0x0;
bootup_27E8_func_074840 = 0x0;
bootup_27E8_func_07ACE0 = 0x0;
bootup_27E8_ref_7328 = 0x0;
bootup_27E8_ref_7334 = 0x0;
bootup_27E8_ref_7344 = 0x0;
bootup_27E8_ref_7360 = 0x0;
bootup_27E8_ref_7374 = 0x0;
bootup_27E8_ref_7380 = 0x0;
bootup_27E8_ref_738C = 0x0;
bootup_27E8_ref_7390 = 0x0;
bootup_27E8_ref_7394 = 0x0;
bootup_27E8_ref_739C = 0x0;
bootup_27E8_ref_73B4 = 0x0;
bootup_27E8_ref_73CC = 0x0;
bootup_27E8_registration_context = 0x0;
bootup_27E8_scale1 = 0x0;
bootup_27E8_scale2 = 0x0;
bootup_27E8_scale3 = 0x0;
bootup_27E8_singleton_storage = 0x0;
bootup_27E8_timer_object = 0x0;
