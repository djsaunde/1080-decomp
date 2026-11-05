/* Standard linker bindings; original USO identity is independently audited.
 * gl_func_0002E354 (agent-g run 284): callees at their own addresses (the target's baked jal words), the per-table /
 * scalar float symbols at the values in their names, the two switch jump tables at the USO rodata 0x16E4 / 0x16F8
 * (one .rodata block at 0x16E4), everything else 0. */
SECTIONS {
 .text 0x0 : SUBALIGN(4) { *(.text) *(.text.*) }
 .rodata 0x16E4 : SUBALIGN(4) { *(.rodata) *(.rodata.*) }
 .rdata 0x0 : SUBALIGN(4) { *(.rdata) *(.rdata.*) }
 .data 0x0 : SUBALIGN(4) { *(.data) *(.data.*) }
 .sdata 0x0 : SUBALIGN(4) { *(.sdata) *(.sdata.*) }
 .bss 0x0 : SUBALIGN(4) { *(.bss) *(.bss.*) }
 .sbss 0x0 : SUBALIGN(4) { *(.sbss) *(.sbss.*) }
 /DISCARD/ : { *(.mdebug) *(.reginfo) *(.pdr) *(.comment) *(.MIPS.abiflags) *(.options) }
}
gl_func_00043824 = 0x43824;
gl_func_00043BE4 = 0x43BE4;
gl_func_000428FC = 0x428FC;
gl_func_000438F4 = 0x438F4;
gl_func_00044C38 = 0x44C38;
D_E354_1C330 = 0x1C330;
D_E354_1C3B0 = 0x1C3B0;
D_E354_1C310 = 0x1C310;
D_E354_1C738_a = 0x1C738;
D_E354_1C738_b = 0x1C738;
D_E354_1C738_c = 0x1C738;
D_E354_1C73C_a = 0x1C73C;
D_E354_1C73C_b = 0x1C73C;
D_E354_1C73C_c = 0x1C73C;
D_00000000 = 0x0;
D_E354_G = 0x0;
D_E354_GA = 0x0;
D_E354_GB = 0x0;
D_cfg_E354 = 0x0;
D_raceA_E354 = 0x0;
D_raceB_E354 = 0x0;
D_raceC_E354 = 0x0;
D_raceD_E354 = 0x0;
D_stateB_E354 = 0x0;
D_stateC_E354 = 0x0;
D_stateD_E354 = 0x0;
D_stateE_E354 = 0x0;
D_stateF_E354 = 0x0;
func_00000000 = 0x0;
gl_E354_emitf = 0x0;
gl_E354_setf = 0x0;
gl_E354_v = 0x0;
