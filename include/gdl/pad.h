#ifndef GDL_PAD_H
#define GDL_PAD_H

/*
 * libgdl controller layer (game_libs post1b 0x66C74-0x67AC0, plus the pad_*Global
 * wrappers and pad_decodeButtonMap_00034CC8 in post0b; agent-b run 515).
 *
 * Two objects:
 *
 * PadDev: the controller device block, one per game (the Bss object gSysDevBlock_3A4E8,
 * Sym 1505, whose inferred span is 5112 = 0x13F8 bytes = sizeof(PadDev)). pad_init_00066C74
 * builds it: the message queue at +0x11B0 with 4 slots at +0x11C8, osSetEventMesg(5 = SI)
 * on it, the pad thread (osCreateThread(&thread, 6, pad_threadLoop, this, stack top =
 * &thread, 14)), so the 4 KB stack is the first 0x1000 bytes. pad_threadLoop_00066EEC
 * (NCC donor game_libs_ncc_66EEC.cpp) receives the messages: 5 = SI event (osPfsIsPlug,
 * then osMotorInit + osMotorStop per plugged pak), 1 = start a read (push the motor values,
 * osContStartReadData, readDone = 0), 2 = read finished (gl_osContGetReadData(conts),
 * pad_initMotors, readDone = 1). The layout below is libultra's: osContInit(&mq,
 * &present, status), osPfsInitPak / osMotorInit(&mq, &pfs[i], i), and
 * pad_decodeButtonMap_00034CC8 reads conts[port - 1] (6-byte OSContPad).
 * The libultra types are spelled as sized byte arrays: game_libs_post1b.c also holds
 * the game_libs copies of libultra (gl_osDestroyThread, gl___osLeoResume ...) with their
 * own local typedefs.
 *
 * PadCtl: one controller as the game sees it (0x80; GameFlow_create builds two, and
 * the bootup modes see the same object as InputPad in include/game/input.h). pad_open
 * attaches it to a port (1-based; 0 = none) and pad_reset_000673FC clears it; pad_update
 * (donor game_libs_o2_675A4.c) runs once per frame:
 *   - plays the rumble pattern (a 0xFF-terminated byte string; each byte goes to
 *     pad_setSlotValueGlobal(port - 1) = PadDev.motorValue[]);
 *   - prevHeld = held; held = 0; reads the port through gSysLib (pad_decodeButtonMap
 *     fills rawStickX / rawStickY / held from conts[port - 1]);
 *   - mirrorX negates the stick x byte; replay PLAY / RECORD (InputFrame below);
 *   - stick = raw / 80 with a 0.2 dead zone; an injected input (injectPending) ORs
 *     injectButtons in and replaces the stick with injectStick;
 *   - the stick sets the direction bits 0x2000 up / 0x4000 right / 0x8000 down /
 *     0x10000 left past +-0.5; pressed = held & ~prevHeld, released = ~held & prevHeld;
 *   - a second press of the same buttons within 10 frames sets doubleTap;
 *   - dirIndex = the u8 table at Data base + ((held & 0x1E000) >> 13).
 */

/* The libultra-shaped types below use plain C types: include/game/input.h includes this
 * header for InputPad (= PadCtl), and its NCC .cpp users have no s8 / u8 / s16 / f32. */

/* One recorded input frame (4 bytes): pad_update RECORD stores the raw stick bytes and
 * (recordMask & held & 0x1700); PLAY loads the stick bytes back and ORs the buttons into held. */
typedef struct InputFrame {
    /* 0x0 */ signed char stickX;
    /* 0x1 */ signed char stickY;
    /* 0x2 */ short buttons;
} InputFrame;

#define PAD_REPLAY_OFF      0
#define PAD_REPLAY_PLAY     1   /* pad_startPlay */
#define PAD_REPLAY_RECORD   2   /* pad_startRecord */

/* libultra OSContPad / OSContStatus, field for field (6 / 4 bytes) */
typedef struct PadContData {
    /* 0x0 */ unsigned short button;
    /* 0x2 */ signed char stickX;
    /* 0x3 */ signed char stickY;
    /* 0x4 */ unsigned char errNo;
} PadContData;

typedef struct PadContStatus {
    /* 0x0 */ unsigned short type;
    /* 0x2 */ unsigned char status;
    /* 0x3 */ unsigned char errNo;                     /* 8 = CONT_NO_RESPONSE_ERROR */
} PadContStatus;

/* the controller device block (0x13F8) */
typedef struct PadDev {
    /* 0x0000 */ char stack[0x1000];        /* pad thread stack (sp = &thread) */
    /* 0x1000 */ char thread[0x1B0];        /* OSThread */
    /* 0x11B0 */ char mq[0x18];             /* OSMesgQueue: SI events + read requests */
    /* 0x11C8 */ char msgs[4][4];           /* OSMesg[4] */
    /* 0x11D8 */ char pad11D8[0x1200 - 0x11D8];
    /* 0x1200 */ PadContData conts[4];      /* gl_osContGetReadData */
    /* 0x1218 */ PadContStatus status[4];   /* osContInit */
    /* 0x1228 */ char pfs[4][0x68];         /* OSPfs[4] (osPfsInitPak / osMotorInit) */
    /* 0x13C8 */ int pakState[4];           /* 3 at init; pad_probePaks: 0 motor init failed,
                                             * 2 rumble pak, 1 other pak (osPfsInitPak == 0) */
    /* 0x13D8 */ unsigned char motorOn[4];             /* motor state: 3 after a successful osMotorStart,
                                             * counted down by pad_updateMotor when osMotorStop
                                             * fails; it returns early when this equals the
                                             * request */
    /* 0x13DC */ unsigned char hasMotor[4];            /* rumble pak present (pad_initMotors) */
    /* 0x13E0 */ short motorValue[4];         /* requested rumble per port (pad_setSlotValue);
                                             * pushed by the thread on message 1 */
    /* 0x13E8 */ int readDone;              /* set by the thread when a read finishes;
                                             * pad_waitIdle spins on it */
    /* 0x13EC */ unsigned char present;                /* osContInit bit pattern (pad_getFlag) */
    char pad13ED[0x13F0 - 0x13ED];
    /* 0x13F0 */ int motorRetry;            /* pad_initMotors re-probes the paks every 32 calls */
    char pad13F4[0x13F8 - 0x13F4];
} PadDev;

/* one controller (0x80) */
typedef struct PadCtl {
    /* 0x00 */ float stickX;                  /* rawStickX / 80, 0 inside the 0.2 dead zone */
    /* 0x04 */ float stickY;
    /* 0x08 */ signed char rawStickX;
    /* 0x09 */ signed char rawStickY;
    char pad0A[0x10 - 0x0A];
    /* 0x10 */ int held;                    /* 1080 button bits (pad_decodeButtonMap) */
    /* 0x14 */ int prevHeld;
    /* 0x18 */ int pressed;
    /* 0x1C */ int released;
    /* 0x20 */ int doubleTap;               /* pressed & lastPressed while tapTimer runs */
    /* 0x24 */ int lastPressed;
    /* 0x28 */ int unk28;                   /* 0 at reset */
    /* 0x2C */ int unk2C;                   /* 0 at reset */
    /* 0x30 */ int unk30;                   /* 0 at reset */
    /* 0x34 */ int port;                    /* 1-based controller port; 0 = not attached */
    /* 0x38 */ int tapTimer;                /* 10 after a press, counts down */
    /* 0x3C */ int injectPending;           /* one frame of injected input (below) */
    /* 0x40 */ int injectButtons;
    /* 0x44 */ float injectStickX;
    /* 0x48 */ float injectStickY;
    /* 0x4C */ int replayMode;              /* PAD_REPLAY_*; drops to OFF at replayLen */
    /* 0x50 */ int replayFrame;
    /* 0x54 */ int replayLen;
    /* 0x58 */ int *replayLenOut;
    /* 0x5C */ InputFrame *replayFrames;
    char pad60[0x64 - 0x60];
    /* 0x64 */ int unk64;                   /* pad_open: result of its first call (this, port) */
    /* 0x68 */ int paused;                  /* pad_setPause; pad_update skips the read */
    /* 0x6C */ int recordMask;              /* -1 at reset */
    /* 0x70 */ int mirrorX;
    /* 0x74 */ short rumblePos;               /* index into rumble */
    /* 0x76 */ short rumblePrio;              /* pad_setIfNewer drops a lower-priority pattern */
    /* 0x78 */ unsigned char *rumble;                  /* 0xFF-terminated motor bytes; null = none */
    /* 0x7C */ short dirIndex;
    char pad7E[0x80 - 0x7E];
} PadCtl;

#ifndef __cplusplus /* edg (NCC donors) rejects the &((T *)0)->f constant form */
typedef char PadContData_size_check[sizeof(PadContData) == 6 ? 1 : -1];
typedef char PadDev_size_check[sizeof(PadDev) == 0x13F8 ? 1 : -1];
typedef char PadDev_pakState_check[(int)&((PadDev *)0)->pakState == 0x13C8 ? 1 : -1];
typedef char PadCtl_size_check[sizeof(PadCtl) == 0x80 ? 1 : -1];
typedef char PadCtl_port_check[(int)&((PadCtl *)0)->port == 0x34 ? 1 : -1];
typedef char PadCtl_rumble_check[(int)&((PadCtl *)0)->rumble == 0x78 ? 1 : -1];
typedef char InputFrame_size_check[sizeof(InputFrame) == 4 ? 1 : -1];
#endif

#define PADDEV(p) ((PadDev *)(p))
#define PADCTL(p) ((PadCtl *)(p))

/* GdlInput: the debug input object the libgdl debug controllers keep a pointer to (CamFollow.owner +0x38,
 * BallRotator.input +0x1CC). Only the fields those two read are known (agent-b run 542): the pad, the
 * cursor's previous / current screen points (BallRotator), a pan vector and the 3D window whose camera
 * CamFollow pans along (CamFollow_update). */
typedef struct GdlInput {
    /* 0x000 */ char pad00[0xB4];
    /* 0x0B4 */ PadCtl *pad;                /* held (+0x10): CamFollow buttons 1/2/4/8/0x200, BallRotator buttonMask */
    /* 0x0B8 */ int unkB8;
    /* 0x0BC */ float prevPoint[3];         /* previous cursor screen point */
    /* 0x0C8 */ char padC8[0xD4 - 0xC8];
    /* 0x0D4 */ float pan[3];               /* CamFollow: times (-10, 10, 30) while button 0x200 */
    /* 0x0E0 */ char padE0[0xF8 - 0xE0];
    /* 0x0F8 */ float curPoint[3];          /* current cursor screen point */
    /* 0x104 */ int unk104;
    /* 0x108 */ int enable;                 /* BallRotator drags only while set */
    /* 0x10C */ int unk10C;
    /* 0x110 */ void *window;               /* the 3D window (CamWindow: camera at +0x70) */
} GdlInput;

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char GdlInput_pad_check[(int)&((GdlInput *)0)->pad == 0xB4 ? 1 : -1];
typedef char GdlInput_window_check[(int)&((GdlInput *)0)->window == 0x110 ? 1 : -1];
#endif

#define GDLINPUT(p) ((GdlInput *)(p))

#endif /* GDL_PAD_H */
