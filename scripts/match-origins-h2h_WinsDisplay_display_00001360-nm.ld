/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x135c : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
h2hproc_1360_import_157 = 0x0;
h2hproc_1360_import_158 = 0x0;
h2hproc_1360_import_159 = 0x0;
h2hproc_1360_import_160 = 0x0;
h2hproc_1360_import_161 = 0x0;
h2hproc_1360_import_162 = 0x0;
h2hproc_1360_import_163 = 0x0;
h2hproc_1360_import_164 = 0x0;
h2hproc_1360_import_165 = 0x0;
snw_h2h_1360_import149 = 0x0;
snw_h2h_1360_text_data = 0x0;
