/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x12e00 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_0000C730 = 0x0;
D_0000C738 = 0x0;
boot12E00_alloc = 0x0;
boot12E00_alloc_aligned = 0x0;
boot12E00_base_init = 0x0;
boot12E00_desc_base = 0x0;
boot12E00_desc_self = 0x0;
boot12E00_exchange_context = 0x0;
