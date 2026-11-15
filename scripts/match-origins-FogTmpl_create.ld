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
D_00007540 = 0x0;
D_00007878 = 0x0;
D_00007FC4 = 0x0;
D_00007FCC = 0x0;
D_00007FD0 = 0x0;
boot8124_alloc = 0x0;
boot8124_child = 0x0;
boot8124_link = 0x0;
boot8124_object_init = 0x0;
boot8124_tail_init = 0x0;
