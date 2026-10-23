/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x3638 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
D_000074DC = 0x0;
func_00000000_3638_v = 0x0;
func_3638_attach = 0x0;
func_3638_finalize = 0x0;
func_3638_node = 0x0;
func_3638_notify = 0x0;
