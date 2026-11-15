/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x1f50 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
arcproc_import_174 = 0x0;
arcproc_import_176 = 0x0;
arcproc_import_177 = 0x0;
arcproc_import_179 = 0x0;
arcproc_import_180 = 0x0;
arcproc_import_181 = 0x0;
arcproc_import_182 = 0x0;
arcproc_import_183 = 0x0;
arcproc_import_184 = 0x0;
arcproc_import_185 = 0x0;
arcproc_import_186 = 0x0;
arcproc_import_187 = 0x0;
arcproc_import_188 = 0x0;
arcproc_import_189 = 0x0;
arcproc_import_40 = 0x0;
