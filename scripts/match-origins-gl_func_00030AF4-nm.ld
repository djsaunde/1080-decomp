/* Standard linker bindings; original USO identity is independently audited.
 * gl_func_00030AF4 (agent-g run 288): callees at their own addresses (the target's baked jal words; the void aliases
 * gl_func_00030AF4_{42364,44F18,460D0}v at their callee's), the two arena addresses at the values in their names
 * (baked lui / addiu), the TU's .rodata (switch-1 table, 0.1f literal, switch-2 / switch-3 tables) at the USO RoData
 * 0x1874 (target 0x1874 / 0x18AC / 0x18B0 / 0x18E0), everything else 0. */
SECTIONS {
 .text 0x0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x1874 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_func_00041D0C = 0x41D0C;
gl_func_00042364 = 0x42364;
gl_func_00030AF4_42364v = 0x42364;
gl_func_00030AF4_460D0v = 0x460D0;
gl_func_00041ED4 = 0x41ED4;
gl_func_00041F0C = 0x41F0C;
gl_func_00041F44 = 0x41F44;
gl_func_00030AF4_44F18v = 0x44F18;
gl_data_30AF4_45010 = 0x45010;
gl_data_30AF4_45F78 = 0x45F78;
D_30AF4_b = 0x0;
D_30AF4_c = 0x0;
D_30AF4_d = 0x0;
D_30AF4_e = 0x0;
D_30AF4_f = 0x0;
D_30AF4_g = 0x0;
D_30AF4_h = 0x0;
D_30AF4_i = 0x0;
D_30AF4_j = 0x0;
D_30AF4_k = 0x0;
D_30AF4_l = 0x0;
D_30AF4_m = 0x0;
D_30AF4_n = 0x0;
D_30AF4_o = 0x0;
D_30AF4_p = 0x0;
D_30AF4_q = 0x0;
D_30AF4_r = 0x0;
D_30AF4_s = 0x0;
D_30AF4_t = 0x0;
D_30AF4_v = 0x0;
D_30AF4_w = 0x0;
D_30AF4_x = 0x0;
gl_cmd_30AF4 = 0x0;
gl_state_30AF4 = 0x0;
gl_func_emit = 0x0;
gl_func_emit_30AF4v = 0x0;
