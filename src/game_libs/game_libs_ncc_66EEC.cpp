#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for pad_threadLoop_00066EEC (0x198 / 102 words): message-pump
 * loop. osRecvMesg-style receive (mq @+0x11B0, &msg, 1) then a `switch` on the
 * message type with the cases in SOURCE order 0/5/6/1/4/2: edg lowers a switch
 * to a source-order compare chain whose last test is `beq last; nop; b default`
 * (the cfe goto ladder inverts that tail to `bnel top; b c2`, kit LXI).
 * The one nop before the unreachable epilogue is as1's 32-byte alignment of the
 * dead epilogue block, which depends on the function's offset in the compiled
 * .text: the original TU had it at 0x18 mod 32 (the expected object's TU offset
 * 0x3F88 is 0x08 mod 32 because game_libs_post1b is a later split). The
 * 0x18-byte gl_ncc_66eec_phase function only sets that phase in this donor
 * object; it is not spliced anywhere.
 * Spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY. */
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef short s16;
extern "C" int func_00000000(...);
extern "C" int gl_ncc_66eec_phase(int *p) { p[0] = 1; p[1] = 2; return 0; }
extern "C" void pad_threadLoop_00066EEC(char *arg0) {
    s32 i;
    s32 flag;
    u32 *sp54;
    u32 v;

    flag = 0;
    for (;;) {
        FCALL(gl_osRecvMesg)(arg0 + 0x11B0, &sp54, 1);
        v = *sp54;
        switch (v) {
        case 0:
            break;
        case 5: {
            u8 sp4F;
            sp4F = 0;
            FCALL(osPfsIsPlug)(arg0 + 0x11B0, &sp4F);
            for (i = 0; i != 4; i++) {
                if (sp4F & (1 << i)) {
                    if (FCALL(osMotorInit)(arg0 + 0x11B0, (int)arg0 + i * 0x68 + 0x1228, i) == 0) {
                        FCALL(gl_osMotorStop)((int)arg0 + i * 0x68 + 0x1228);
                    }
                }
            }
            flag = 1;
            break;
        }
        case 6:
            break;
        case 1:
            if (flag == 0) {
                for (i = 0; i != 4; i++) {
                    /* RAW: donor-mirror: C++ donor (NCC Cfront); its local mirror structs cannot include the C headers */
                    FCALL(pad_updateMotor_00067084)(arg0, i, *(s16 *)(arg0 + i * 2 + 0x13E0));
                }
            }
            FCALL(osContStartReadData)(arg0 + 0x11B0);
            *(int *)(arg0 + 0x13E8) = 0; /* RAW: donor-mirror (as above) */
            break;
        case 4:
            break;
        case 2:
            FCALL(gl_osContGetReadData)(arg0 + 0x1200);
            if (flag == 0) {
                FCALL(pad_initMotors_00067264)(arg0);
            }
            *(int *)(arg0 + 0x13E8) = 1; /* RAW: donor-mirror (as above) */
            break;
        }
    }
}
