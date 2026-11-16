/* Self-test for port/ultra (build: the shim's .c files + port/host_headless.c
 * + this file; no game code, no ROM needed). Exit status 0 = all pass. */

#include "ultra_int.h"

static int sFails;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            host_log("FAIL %s:%d: %s", __FILE__, __LINE__, #cond);           \
            sFails++;                                                        \
        }                                                                    \
    } while (0)

static u8 sRom[0x2000];

static void init(int deadlockFatal, u32 maxIdle, const char *sram) {
    PortUltraConfig c;

    memset(&c, 0, sizeof(c));
    c.rom = sRom;
    c.romSize = sizeof(sRom);
    c.sramPath = sram;
    c.deadlockFatal = deadlockFatal;
    c.maxIdleRetraces = maxIdle;
    port_ultra_init(&c);
    osInitialize();
}

static void test_queue(void) {
    OSMesgQueue q;
    OSMesg buf[2], m;

    init(0, 4, NULL);
    osCreateMesgQueue(&q, buf, 2);
    CHECK(osSendMesg(&q, (OSMesg)1, OS_MESG_NOBLOCK) == 0);
    CHECK(osSendMesg(&q, (OSMesg)2, OS_MESG_NOBLOCK) == 0);
    CHECK(osSendMesg(&q, (OSMesg)3, OS_MESG_NOBLOCK) == -1);
    CHECK(q.validCount == 2);
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == 0 && m == (OSMesg)1);
    CHECK(osJamMesg(&q, (OSMesg)9, OS_MESG_NOBLOCK) == 0);
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == 0 && m == (OSMesg)9);
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == 0 && m == (OSMesg)2);
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == -1);
}

static void test_vi_wait(void) {
    OSMesgQueue q;
    OSMesg buf[4], m = NULL;

    init(0, 8, NULL);
    osCreateMesgQueue(&q, buf, 4);
    osViSetEvent(&q, (OSMesg)0x64, 2);
    CHECK(osRecvMesg(&q, &m, OS_MESG_BLOCK) == 0 && m == (OSMesg)0x64);
    CHECK(port_ultra_stats()->idleRetraces == 2);
    CHECK(osGetCount() == 2 * PORT_CYCLES_PER_RETRACE);
    CHECK(osGetTime() == (OSTime)2 * PORT_CYCLES_PER_RETRACE);
}

static OSMesgQueue sSvcQ;
static OSMesg sSvcBuf[1];
static int sSvcRuns;

static void svc_entry(void *arg) {
    (void)arg;
}

static int svc_step(void *arg) {
    sSvcRuns++;
    if (sSvcRuns == 3) {
        osSendMesg(&sSvcQ, arg, OS_MESG_NOBLOCK);
        return 1;
    }
    return sSvcRuns < 3;
}

static void test_service(void) {
    static OSThread t;
    OSMesg m = NULL;

    init(0, 8, NULL);
    sSvcRuns = 0;
    port_ultra_register_service(svc_entry, svc_step, "test");
    osCreateMesgQueue(&sSvcQ, sSvcBuf, 1);
    osCreateThread(&t, 7, svc_entry, (void *)0x55, NULL, 20);
    osStartThread(&t);
    CHECK(osRecvMesg(&sSvcQ, &m, OS_MESG_BLOCK) == 0 && m == (OSMesg)0x55);
    CHECK(sSvcRuns == 3);
    CHECK(port_ultra_stats()->idleRetraces == 0);
    CHECK(osGetThreadPri(&t) == 20);
    {
        extern OSThread *gl___osActiveQueue_34458_head;
        OSThread *n = gl___osActiveQueue_34458_head;
        int count = 0;
        CHECK(n == &t);
        while (n->priority != -1 && count < 10) {
            n = n->tlnext;
            count++;
        }
        CHECK(count == 2); /* t + the simulation thread */
        osCreateThread(&t, 7, svc_entry, (void *)0x55, NULL, 21); /* re-create: no duplicate */
        for (n = gl___osActiveQueue_34458_head, count = 0; n->priority != -1 && count < 10; count++) {
            n = n->tlnext;
        }
        CHECK(count == 2);
        osDestroyThread(&t);
        CHECK(gl___osActiveQueue_34458_head != &t);
    }
}

static void test_deadlock(void) {
    OSMesgQueue q;
    OSMesg buf[1], m = NULL;

    init(0, 5, NULL);
    osCreateMesgQueue(&q, buf, 1);
    CHECK(osRecvMesg(&q, &m, OS_MESG_BLOCK) == -1);
    CHECK(port_ultra_stats()->deadlocks == 1);
    CHECK(port_ultra_stats()->idleRetraces == 5);
}

static void test_pi(const char *sramPath) {
    OSMesgQueue q;
    OSMesg buf[2], m = NULL;
    OSIoMesg mb;
    OSPiHandle sram;
    u8 out[16], in[16];
    u32 w = 0;
    int i;

    for (i = 0; i < (int)sizeof(sRom); i++) {
        sRom[i] = (u8)(i * 7);
    }
    sRom[0] = 0x80;
    sRom[1] = 0x37;
    sRom[2] = 0x12;
    sRom[3] = 0x40;
    init(0, 4, sramPath);
    osCreateMesgQueue(&q, buf, 2);
    memset(out, 0, sizeof(out));
    CHECK(osPiStartDma(&mb, 0, OS_READ, 0x100, out, 16, &q) == 0);
    CHECK(osRecvMesg(&q, &m, OS_MESG_BLOCK) == 0 && m == (OSMesg)&mb);
    CHECK(out[0] == (u8)(0x100 * 7) && out[15] == (u8)(0x10F * 7));
    CHECK(osPiReadIo(0, &w) == 0 && w == 0x80371240);
    CHECK(osEPiStartDma(osCartRomInit(), &mb, OS_READ) == 0); /* same mb, re-sent */
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == 0);

    /* SRAM through an app_initSystem-style handle */
    memset(&sram, 0, sizeof(sram));
    sram.baseAddress = 0xA8000000;
    sram.domain = 1;
    osEPiLinkHandle(&sram);
    for (i = 0; i < 16; i++) {
        in[i] = (u8)(0xA0 + i);
    }
    memset(&mb, 0, sizeof(mb));
    mb.hdr.retQueue = &q;
    mb.dramAddr = in;
    mb.devAddr = 0x100;
    mb.size = 16;
    CHECK(osEPiStartDma(&sram, &mb, OS_WRITE) == 0);
    CHECK(osRecvMesg(&q, &m, OS_MESG_BLOCK) == 0);
    memset(out, 0, sizeof(out));
    mb.dramAddr = out;
    CHECK(osEPiStartDma(&sram, &mb, OS_READ) == 0);
    CHECK(osRecvMesg(&q, &m, OS_MESG_BLOCK) == 0);
    CHECK(memcmp(in, out, 16) == 0);
    CHECK(port_sram_image()[0x10F] == 0xAF);
    CHECK(osEPiStartDma(osCartRomInit(), &mb, OS_WRITE) == -1); /* ROM is read-only */
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == 0);
    /* reload from the save file */
    if (sramPath != NULL) {
        init(0, 4, sramPath);
        CHECK(port_sram_image()[0x100] == 0xA0);
    }
}

static void test_timer_ai_si(void) {
    OSMesgQueue q;
    OSMesg buf[4], m = NULL;
    OSTimer t;
    OSContPad in[MAXCONTROLLERS], out[MAXCONTROLLERS];
    u8 con[MAXCONTROLLERS] = { 1, 0, 0, 0 }, rum[MAXCONTROLLERS] = { 1, 0, 0, 0 };
    OSContStatus st[MAXCONTROLLERS];
    OSPfs pfs;
    u8 bits = 0;

    init(0, 10, NULL);
    osCreateMesgQueue(&q, buf, 4);
    osSetTimer(&t, (OSTime)3 * PORT_CYCLES_PER_RETRACE, 0, &q, (OSMesg)0x77);
    port_vi_retrace();
    port_vi_retrace();
    CHECK(q.validCount == 0);
    port_vi_retrace();
    CHECK(osRecvMesg(&q, &m, OS_MESG_NOBLOCK) == 0 && m == (OSMesg)0x77);

    CHECK(osAiSetFrequency(32000) == 32006);

    memset(in, 0, sizeof(in));
    in[0].button = 0x8000;
    in[0].stick_x = 40;
    in[0].stick_y = -12;
    port_si_set_pads(in, con, rum);
    osSetEventMesg(OS_EVENT_SI, &q, (OSMesg)5);
    CHECK(osContInit(&q, &bits, st) == 0 && bits == 1 && st[0].type == CONT_TYPE_NORMAL);
    CHECK(osContStartReadData(&q) == 0);
    CHECK(osRecvMesg(&q, &m, OS_MESG_BLOCK) == 0 && m == (OSMesg)5);
    osContGetReadData(out);
    CHECK(out[0].button == 0x8000 && out[0].stick_x == 40 && out[0].stick_y == -12 &&
          out[0].errno == 0);
    CHECK(out[1].errno == CONT_NO_RESPONSE_ERROR);
    CHECK(osPfsIsPlug(&q, &bits) == 0 && bits == 1);
    CHECK(osPfsInitPak(&q, &pfs, 0) == PFS_ERR_NOPACK);
    CHECK(osMotorInit(&q, &pfs, 0) == 0 && osMotorStart(&pfs) == 0 && osMotorStop(&pfs) == 0);
    CHECK(osMotorInit(&q, &pfs, 1) == PFS_ERR_NOPACK);
    CHECK(osVirtualToPhysical(&q) == (u32)(size_t)&q);
}

int main(int argc, char **argv) {
    test_queue();
    test_vi_wait();
    test_service();
    test_deadlock();
    test_pi(argc > 1 ? argv[1] : NULL);
    test_timer_ai_si();
    host_log("test_ultra: %s (%d failures)", sFails ? "FAIL" : "PASS", sFails);
    return sFails != 0;
}
