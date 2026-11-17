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
 * Audio (game -> presenter, plan 19.12): a byte ring of the game's AI output (32000 Hz
 * stereo s16, host order), described by the LiveAudio block at LIVE_AUDIO_HDR inside the
 * header page; its data lives at dataOffset from the start of the file (the presenter places
 * it after the slots and sizes the file to cover it). The game appends every AI buffer
 * (osAiSetNextBuffer -> host_audio_queue) while it fits and counts an overrun otherwise; the
 * presenter's audio thread consumes it into PulseAudio (or drains it when muted / without a
 * server). A presenter without audio leaves magic 0 and the game sends nothing.
 *
 * Read-backs (presenter -> game, plan 19.10 / 19.11, run 859): the
 * DLUMP_FB_READ / DLUMP_FB_DEPTH chunks travel in the frame; the answers come
 * back in a second file, /dev/shm/<name>-fb (LiveFbReply below), so this
 * header's layout is untouched.
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

/* ---------------- the framebuffer replies (run 859) ----------------
 *
 * /dev/shm/<name>-fb, created by the presenter before it writes LIVE_MAGIC, holds one
 * LiveFbReply: the answers to the frame's DLUMP_FB_DEPTH / DLUMP_FB_READ requests, read back
 * (patch 0003) at the end of the frame's Run. The game (port_live.c) picks the newest reply up at
 * each gfx task and stores it into the images it asked about, after port_gfx.c's depth-clear model:
 * the clear stays the fallback for every pixel no reply covers (no presenter, an older presenter
 * without the file, the first frames). Latency: one to two steps (the N64's reader sees the last
 * completed frame; here the presenter renders asynchronously).
 *
 * Seqlock: the presenter makes seq odd, writes, makes it even (release); the game copies the
 * fields it uses and keeps them only if seq was even and unchanged across the copy. */
#define LIVE_FB_MAGIC 0x5242464Cu /* "LFBR" */
#define LIVE_FB_VERSION 1u
#define LIVE_FB_W 320
#define LIVE_FB_H 240
#define LIVE_FB_DEPTH 64 /* = PORT_FB_MAX_DEPTH */
#define LIVE_FB_COLOR 4  /* = PORT_FB_MAX_COLOR */

typedef struct LiveFbDepth {
    live_u32 img; /* the requested Z image (the game's full pointer, as in the FB_DEPTH chunk) */
    live_u32 idx; /* y * 320 + x */
    live_u32 z;   /* the N64 Z value (0xFFFC = far), host order; the game stores it big-endian */
} LiveFbDepth;

typedef struct LiveFbColor {
    live_u32 img; /* the requested colour image (the FB_READ chunk's address) */
    live_u32 pad;
} LiveFbColor;

typedef struct LiveFbReply {
    live_u32 magic;   /* LIVE_FB_MAGIC */
    live_u32 version; /* LIVE_FB_VERSION */
    volatile live_u32 seq; /* the seqlock (odd while the presenter writes) */
    live_u32 frameSeq;     /* LiveFrame.seq of the frame answered */
    live_u32 step;         /* LiveFrame.step of the frame answered */
    live_u32 nDepth;
    live_u32 nColor;
    live_u32 reserved[9];
    LiveFbDepth depth[LIVE_FB_DEPTH];
    LiveFbColor color[LIVE_FB_COLOR];
    live_u16 pixels[LIVE_FB_COLOR][LIVE_FB_W * LIVE_FB_H]; /* RGBA5551, big-endian (the N64 image) */
} LiveFbReply;

/* the audio ring (plan 19.12): its own block in the header page, apart from LiveShmHeader */
#define LIVE_AUDIO_MAGIC 0x4F445541u /* "AUDO" */
#define LIVE_AUDIO_HDR 2048u         /* offset of LiveAudio in the header page */
#define LIVE_AUDIO_DEFAULT_BYTES (1u << 17) /* 128 KB: 1.02 s of 32000 Hz stereo s16 */

typedef struct LiveAudio {
    live_u32 magic;      /* LIVE_AUDIO_MAGIC, written by the presenter before LiveShmHeader.magic */
    live_u32 rate;       /* 32000 */
    live_u32 channels;   /* 2 (s16 frames of 4 bytes) */
    live_u32 bytes;      /* ring size, a power of two */
    live_u32 dataOffset; /* the ring's offset from the start of the file */
    volatile live_u32 head;      /* bytes written by the game (free-running) */
    volatile live_u32 tail;      /* bytes consumed by the presenter (free-running) */
    volatile live_u32 overruns;  /* game: AI buffers dropped because the ring was full */
    volatile live_u32 underruns; /* presenter: times the ring ran dry while playing */
    volatile live_u32 trimmed;   /* presenter: bytes skipped to keep the latency bounded */
    volatile live_u32 state;     /* presenter: LIVE_AUDIO_* below */
    live_u32 reserved[5];
} LiveAudio;

#define LIVE_AUDIO_OFF 0u   /* not started / failed: drained, not played */
#define LIVE_AUDIO_PULSE 1u /* playing through PulseAudio */
#define LIVE_AUDIO_MUTED 2u /* --mute: drained, not played */

typedef char live_audio_fits_[(sizeof(LiveShmHeader) <= LIVE_AUDIO_HDR &&
                               LIVE_AUDIO_HDR + sizeof(LiveAudio) <= LIVE_HEADER_BYTES) ? 1 : -1];

#endif
