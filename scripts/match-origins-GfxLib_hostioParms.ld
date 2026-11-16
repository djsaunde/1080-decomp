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
HostioCb_vtbl_1F730_44144_a = 0x0;
HostioCb_vtbl_1F730_44144_b = 0x0;
HostioCb_vtbl_1F730_44144_c = 0x0;
HostioCb_vtbl_1F730_44144_d = 0x0;
gGfxLib_44144_rcp = 0x0;
gHostio_44144_group = 0x0;
gHostio_44144_shadowAlpha = 0x0;
gGfxLib_44144_shadowAlpha = 0x0;
gHostio_44144_cvgTest = 0x0;
gGfxLib_44144_cvgTest = 0x0;
gHostio_44144_autoLights = 0x0;
gHostio_44144_fifoLen = 0x0;
gFifoLen_3EC0C_44144 = 0x0;
gGfxLib_44144_fog = 0x0;
gHostio_44144_dumpScreen = 0x0;
gHostio_44144_dumpZBuffer = 0x0;
gHostio_44144_dumpError = 0x0;
gHostio_44144_showDLUsage = 0x0;
HostioFnCb_vtbl_2C638_44144_a = 0x0;
GfxLib_hostioSendFbB_44144_cb = 0x0;
HostioFnCb_vtbl_2C638_44144_b = 0x0;
GfxLib_hostioSendFbA_44144_cb = 0x0;
HostioFnCb_vtbl_2C638_44144_c = 0x0;
GfxLib_hostioForceError_44144_cb = 0x0;
HostioFnCb_vtbl_2C638_44144_d = 0x0;
GfxLib_hostioPrintDLUsage_44144_cb = 0x0;
gRcpBusy_3EC10_44144 = 0x0;
gHostio_44144_rcpStatus = 0x0;
gHostio_44144_gfxStatus = 0x0;
gHostio_44144_end = 0x0;
gGfxStatusStr_3C7C8_44144 = 0x0;
