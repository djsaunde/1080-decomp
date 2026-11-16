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
mem_new_44144 = 0x0;
Node_hostioParms_44144 = 0x0;
hostio_endGroup_44144 = 0x0;
gl_sprintf_44144 = 0x0;
gAutoLights_1F970_44144 = 0x0;
sKey_44144_GfxLibParms = 0x0;
sKey_44144_ShadowAlpha = 0x0;
sKey_44144_CvgTest = 0x0;
sKey_44144_AutoLights = 0x0;
sKey_44144_FifoLen = 0x0;
sKey_44144_DumpScreen = 0x0;
sKey_44144_DumpZBuffer = 0x0;
sKey_44144_DumpError = 0x0;
sKey_44144_ShowDLUsage = 0x0;
sKey_44144_RcpStatusFmt = 0x0;
sKey_44144_GfxStatusFmt = 0x0;
hostio_addText_44144 = 0x0;
hostio_addButton_44144 = 0x0;
hostio_addFloat_44144 = 0x0;
hostio_beginGroup_44144 = 0x0;
hostio_addInt_44144 = 0x0;
hostio_addBool_44144 = 0x0;
FogParms_hostioParms_44144 = 0x0;
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
