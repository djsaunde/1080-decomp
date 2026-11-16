/* port/ultra: SI (controllers), controller pak and rumble pak.
 *
 * The pads come from port_si_set_pads(), which main.c's input_poll() calls
 * once per step from the host layer (HostPad). osContStartReadData snapshots
 * them and posts the SI event, as the SI interrupt would after the PIF read;
 * osContGetReadData returns the snapshot. Game-level record / replay hooks in
 * pad_update (port/trace.h) sit above this, so replays do not depend on it.
 *
 * Controller paks are reported absent (1080 saves to SRAM). A rumble pak is
 * reported where the host says it can rumble; osMotorStart / osMotorStop
 * drive host_set_rumble. */

#include "ultra_int.h"

static OSContPad sLive[MAXCONTROLLERS];
static OSContPad sSnap[MAXCONTROLLERS];
static u8 sConnected[MAXCONTROLLERS];
static u8 sRumble[MAXCONTROLLERS];

void port_si_set_pads(const OSContPad pads[MAXCONTROLLERS], const u8 connected[MAXCONTROLLERS],
                      const u8 hasRumble[MAXCONTROLLERS]) {
    memcpy(sLive, pads, sizeof(sLive));
    memcpy(sConnected, connected, sizeof(sConnected));
    memcpy(sRumble, hasRumble, sizeof(sRumble));
}

static void fill_status(OSContStatus *status) {
    int i;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        status[i].type = sConnected[i] ? CONT_TYPE_NORMAL : 0;
        status[i].status = 0; /* no controller pak inserted */
        status[i].errno = sConnected[i] ? 0 : CONT_NO_RESPONSE_ERROR;
    }
}

s32 osContInit(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *status) {
    u8 bits = 0;
    int i;

    (void)mq;
    fill_status(status);
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (sConnected[i]) {
            bits |= (u8)(1 << i);
        }
    }
    *bitpattern = bits;
    return 0;
}

s32 osContStartQuery(OSMesgQueue *mq) {
    (void)mq;
    port_ultra_post_event(OS_EVENT_SI);
    return 0;
}

void osContGetQuery(OSContStatus *status) {
    fill_status(status);
}

s32 osContStartReadData(OSMesgQueue *mq) {
    int i;

    (void)mq; /* the completion goes to the OS_EVENT_SI queue, which is mq in practice */
    for (i = 0; i < MAXCONTROLLERS; i++) {
        sSnap[i] = sLive[i];
        sSnap[i].errno = sConnected[i] ? 0 : CONT_NO_RESPONSE_ERROR;
        if (!sConnected[i]) {
            sSnap[i].button = 0;
            sSnap[i].stick_x = 0;
            sSnap[i].stick_y = 0;
        }
    }
    port_ultra_activity++;
    port_ultra_post_event(OS_EVENT_SI);
    return 0;
}

void osContGetReadData(OSContPad *pads) {
    memcpy(pads, sSnap, sizeof(sSnap));
}

s32 osPfsIsPlug(OSMesgQueue *mq, u8 *pattern) {
    u8 bits = 0;
    int i;

    (void)mq;
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (sConnected[i] && sRumble[i]) {
            bits |= (u8)(1 << i); /* "something is plugged in": the rumble pak */
        }
    }
    *pattern = bits;
    return 0;
}

/* No controller pak: pad_probePaks then tries osMotorInit. */
s32 osPfsInitPak(OSMesgQueue *mq, OSPfs *pfs, s32 channel) {
    (void)mq;
    (void)pfs;
    (void)channel;
    return PFS_ERR_NOPACK;
}

s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, s32 channel) {
    if (channel < 0 || channel >= MAXCONTROLLERS || !sConnected[channel] || !sRumble[channel]) {
        return PFS_ERR_NOPACK;
    }
    pfs->queue = mq;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->activebank = 0xFF;
    return 0;
}

s32 osMotorStart(OSPfs *pfs) {
    host_set_rumble(pfs->channel, 1);
    return 0;
}

s32 osMotorStop(OSPfs *pfs) {
    host_set_rumble(pfs->channel, 0);
    return 0;
}

void port_si_reset(void) {
    memset(sLive, 0, sizeof(sLive));
    memset(sSnap, 0, sizeof(sSnap));
    memset(sConnected, 0, sizeof(sConnected));
    memset(sRumble, 0, sizeof(sRumble));
    sConnected[0] = 1;
}

PORT_ALIAS(osContInit, gl_osContInit);
PORT_ALIAS(osContGetReadData, gl_osContGetReadData);
PORT_ALIAS(osMotorStart, gl_osMotorStart);
PORT_ALIAS(osMotorStop, gl_osMotorStop);
