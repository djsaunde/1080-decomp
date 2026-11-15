/* Standard linker bindings; original USO identity is independently audited. */
SECTIONS {
 .text 0x58144 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x0 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_44144_alloc = 0x0;
gl_44144_dispatch = 0x0;
gl_44144_finish = 0x0;
gl_44144_format = 0x0;
gl_44144_key_F970 = 0x0;
gl_44144_key_FD34 = 0x0;
gl_44144_key_FD40 = 0x0;
gl_44144_key_FD50 = 0x0;
gl_44144_key_FD58 = 0x0;
gl_44144_key_FD64 = 0x0;
gl_44144_key_FD6C = 0x0;
gl_44144_key_FD78 = 0x0;
gl_44144_key_FD88 = 0x0;
gl_44144_key_FD94 = 0x0;
gl_44144_key_FDA4 = 0x0;
gl_44144_key_FDD4 = 0x0;
gl_44144_message = 0x0;
gl_44144_register_binding = 0x0;
gl_44144_register_float = 0x0;
gl_44144_register_int = 0x0;
gl_44144_register_range = 0x0;
gl_44144_register_word = 0x0;
gl_44144_setup = 0x0;
gl_d44144_f730a = 0x0;
gl_d44144_f730b = 0x0;
gl_d44144_f730c = 0x0;
gl_d44144_f730d = 0x0;
gl_d44144_gp = 0x0;
gl_d44144_k1 = 0x0;
gl_d44144_k2 = 0x0;
gl_d44144_k2c = 0x0;
gl_d44144_k3 = 0x0;
gl_d44144_k3c = 0x0;
gl_d44144_k4 = 0x0;
gl_d44144_k5 = 0x0;
gl_d44144_k5c = 0x0;
gl_d44144_k6 = 0x0;
gl_d44144_r1 = 0x0;
gl_d44144_r2 = 0x0;
gl_d44144_r3 = 0x0;
gl_d44144_r4 = 0x0;
gl_d44144_s1a = 0x0;
gl_d44144_s1b = 0x0;
gl_d44144_s2a = 0x0;
gl_d44144_s2b = 0x0;
gl_d44144_s3a = 0x0;
gl_d44144_s3b = 0x0;
gl_d44144_s4a = 0x0;
gl_d44144_s4b = 0x0;
gl_d44144_t1 = 0x0;
gl_d44144_t2 = 0x0;
gl_d44144_t3 = 0x0;
gl_d44144_t4 = 0x0;
gl_d44144_v13 = 0x0;
