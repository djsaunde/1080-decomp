/* port/gfx: the live channel between the 32-bit game and the 64-bit presenter
 * (PC port plan 19.11, option (c) of 19.1).
 *
 * One shared-memory file, /dev/shm/<name>, created by the presenter
 * (port/gfx/fast3d/presenter.cpp) and opened by the game when PORT_LIVE=<name>
 * is set (port/gfx/port_live.c). Plain C, fixed-width fields, the same layout in
 * an i386 and an x86-64 process (both little-endian, 4-byte fields only).
 *
 *     LiveShmHeader                     LIVE_HEADER_BYTES (4096)
 *     slot[0 .. slotCount - 1]          slotBytes each
 *
 * Frames (game -> presenter): a single-producer / single-consumer ring of
 * slots. One slot = one game step's gfx tasks (what capture.sh -M composites
 * into one image): a LiveFrame header, then each task's DLMP image (dlump.h,
 * exactly what PORT_GFX_DUMP would have written) back to back. The game fills
 * slot head % slotCount only while head - tail < slotCount, then publishes it
 * by incrementing head; a full ring DROPS the frame (the game never waits for
 * the presenter, so it keeps real time). The presenter renders slot
 * tail % slotCount and increments tail.
 *
 * Input (presenter -> game): pads[] is the N64 controller state the presenter
 * built from the keyboard / gamepad; the game copies it at each step's input
 * poll (host_read_pads). padSeq increments on every write (a debugging aid).
 *
 * Read-backs (planned, plan 19.10 / 19.11): the DLUMP_FB_READ / DLUMP_FB_DEPTH
 * chunks already travel in the frame; the answers (the 320x240 RGBA5551 frame,
 * the requested depth pixels) would come back in the reserved fbReply area
 * with one frame of latency, the semantics the game already has (11.2). Not
 * served yet.
 */
#ifndef PORT_GFX_LIVE_SHM_H
#define PORT_GFX_LIVE_SHM_H

typedef unsigned int live_u32;
typedef unsigned short live_u16;
typedef signed char live_s8;
typedef unsigned char live_u8;

#define LIVE_MAGIC 0x4556494Cu /* "LIVE" */
#define LIVE_VERSION 1u
#define LIVE_HEADER_BYTES 4096u
#define LIVE_DEFAULT_SLOTS 3u
#define LIVE_DEFAULT_SLOT_BYTES (48u << 20)
#define LIVE_PADS 4

/* one N64 controller: host.h HostPad's fields */
typedef struct LivePad {
    live_u16 button; /* OSContPad bits (host.h) */
    live_s8 stick_x;
    live_s8 stick_y;
    live_u8 connected;
    live_u8 hasRumble;
    live_u8 pad[2];
} LivePad;

typedef struct LiveShmHeader {
    live_u32 magic;     /* LIVE_MAGIC, written last by the presenter */
    live_u32 version;   /* LIVE_VERSION */
    live_u32 slotCount; /* ring slots */
    live_u32 slotBytes; /* bytes per slot, LiveFrame included */
    /* the ring (read / written with __atomic acquire / release) */
    volatile live_u32 head; /* frames published by the game */
    volatile live_u32 tail; /* frames consumed by the presenter */
    volatile live_u32 dropped; /* frames the game dropped because the ring was full */
    volatile live_u32 tooBig;  /* frames the game dropped because they did not fit a slot */
    /* liveness */
    volatile live_u32 gamePid;       /* set by the game when it attaches */
    volatile live_u32 presenterPid;  /* set by the presenter */
    volatile live_u32 quit;          /* presenter -> game: the window was closed */
    volatile live_u32 gameExited;    /* game -> presenter: the game ended */
    /* input */
    volatile live_u32 padSeq;
    LivePad pads[LIVE_PADS];
    volatile live_u32 gameStep; /* the step of the last published frame */
    live_u32 reserved[32];
} LiveShmHeader;

/* the start of every slot */
typedef struct LiveFrame {
    live_u32 bytes;  /* LiveFrame + the DLMP images */
    live_u32 nTasks; /* DLMP images that follow */
    live_u32 step;   /* SysLib.frameCount of the step (state.log's F) */
    live_u32 seq;    /* the frame's number since boot (head at publish) */
} LiveFrame;

#endif
