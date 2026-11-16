/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
#include "gdl/gfxlib.h"
#include "gdl/rcp.h"
void GfxLib_hostioParms(void *arg0) {
    extern char gHostio_44144_group, gHostio_44144_shadowAlpha, gGfxLib_44144_shadowAlpha, gHostio_44144_cvgTest,
        gGfxLib_44144_cvgTest, gHostio_44144_autoLights, gHostio_44144_fifoLen, gFifoLen_3EC0C_44144,
        gGfxLib_44144_fog, gHostio_44144_dumpScreen, gHostio_44144_dumpZBuffer, gHostio_44144_dumpError, gHostio_44144_showDLUsage,
        HostioFnCb_vtbl_2C638_44144_a, GfxLib_hostioSendFbB_44144_cb, HostioFnCb_vtbl_2C638_44144_b, GfxLib_hostioSendFbA_44144_cb,
        HostioFnCb_vtbl_2C638_44144_c, GfxLib_hostioForceError_44144_cb, HostioFnCb_vtbl_2C638_44144_d, GfxLib_hostioPrintDLUsage_44144_cb,
        gGfxLib_44144_rcp, gHostio_44144_rcpStatus, gHostio_44144_gfxStatus, gHostio_44144_end;
    extern char HostioCb_vtbl_1F730_44144_a, HostioCb_vtbl_1F730_44144_b, HostioCb_vtbl_1F730_44144_c, HostioCb_vtbl_1F730_44144_d;
    extern char gRcpBusy_3EC10_44144;
    extern int gGfxStatusStr_3C7C8_44144;
    char sp38[0x100];
    s32 *var_v1;
    s32 *var_a2;
    RcpMgr *rcp;
    gl_44144_register_int(&gHostio_44144_group, (&gl_44144_key_FD34 + 0x1FD34), 0);
    gl_44144_register_float(&gHostio_44144_shadowAlpha, (&gl_44144_key_FD40 + 0x1FD40), (float *)(&gGfxLib_44144_shadowAlpha + 0x24C), 0.0f, 1.0f, 0);
    gl_44144_register_word(&gHostio_44144_cvgTest, (&gl_44144_key_FD50 + 0x1FD50), &gGfxLib_44144_cvgTest + 0x250);
    gl_44144_register_word(&gHostio_44144_autoLights, (&gl_44144_key_FD58 + 0x1FD58), (&gl_44144_key_F970 + 0x1F970));
    gl_44144_register_range(&gHostio_44144_fifoLen, (&gl_44144_key_FD64 + 0x1FD64), &gFifoLen_3EC0C_44144 + 0x0, 0x400, 0x19000, 0);
    gl_44144_setup(&gGfxLib_44144_fog + 0x294);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&HostioCb_vtbl_1F730_44144_a + 0x1F730);
        }
        (var_a2[0] = (s32)&HostioFnCb_vtbl_2C638_44144_a, var_a2[1] = (s32)&GfxLib_hostioSendFbB_44144_cb);
    }
    gl_44144_register_binding(&gHostio_44144_dumpScreen, (&gl_44144_key_FD6C + 0x1FD6C), var_a2, 1);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&HostioCb_vtbl_1F730_44144_b + 0x1F730);
        }
        (var_a2[0] = (s32)&HostioFnCb_vtbl_2C638_44144_b, var_a2[1] = (s32)&GfxLib_hostioSendFbA_44144_cb);
    }
    gl_44144_register_binding(&gHostio_44144_dumpZBuffer, (&gl_44144_key_FD78 + 0x1FD78), var_a2, 1);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&HostioCb_vtbl_1F730_44144_c + 0x1F730);
        }
        (var_a2[0] = (s32)&HostioFnCb_vtbl_2C638_44144_c, var_a2[1] = (s32)&GfxLib_hostioForceError_44144_cb);
    }
    gl_44144_register_binding(&gHostio_44144_dumpError, (&gl_44144_key_FD88 + 0x1FD88), var_a2, 0);
    var_a2 = gl_44144_alloc(8);
    if (var_a2 != 0) {
        var_v1 = var_a2;
        if ((var_a2 != 0) || (var_v1 = gl_44144_alloc(4), var_v1 != 0)) {
            *var_v1 = (s32)(&HostioCb_vtbl_1F730_44144_d + 0x1F730);
        }
        (var_a2[0] = (s32)&HostioFnCb_vtbl_2C638_44144_d, var_a2[1] = (s32)&GfxLib_hostioPrintDLUsage_44144_cb);
    }
    gl_44144_register_binding(&gHostio_44144_showDLUsage, (&gl_44144_key_FD94 + 0x1FD94), var_a2, 1);
    rcp = GFXLIB(&gGfxLib_44144_rcp)->rcp;
    gl_44144_format(sp38, (&gl_44144_key_FDA4 + 0x1FDA4), rcp->rspTaskName, rcp->rdpTaskName, rcp->retraceCount, rcp->rspBusy, rcp->rdpBusy, *(volatile s32 *)&gRcpBusy_3EC10_44144);
    gl_44144_message(&gHostio_44144_rcpStatus, sp38);
    gl_44144_format(sp38, (&gl_44144_key_FDD4 + 0x1FDD4), gGfxStatusStr_3C7C8_44144);
    gl_44144_message(&gHostio_44144_gfxStatus, sp38);
    gl_44144_finish(&gHostio_44144_end);
    gl_44144_dispatch(arg0);
}
