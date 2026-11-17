#ifndef PORT_GFX_SYNTH_H
#define PORT_GFX_SYNTH_H

/* port/gfx: a synthetic 1080-style F3DEX 1.x frame for the dumper test and the
 * replayer's first-triangle proof. Everything is built inside one arena, so the
 * same code works on the 32-bit game target and a 64-bit host: a game address
 * is (pointer - arenaBase). */

#include <stddef.h>

typedef struct SynthFrame {
    size_t arenaBase;      /* DlumpOptions.arenaBase for this frame */
    unsigned int taskAddr; /* the OSTask_t-shaped block (0x40 bytes) */
    unsigned int rootAddr;
    unsigned int rootSize;
    /* expectations for the test */
    unsigned int texAddr[2], texSize[2], tlutAddr, tlutSize;
    unsigned int vtxAddr, vtxSize, sharedDl, branchDl, vpAddr, cimgAddr, zimgAddr;
} SynthFrame;

/* Builds the frame in a static arena and returns it. realPointers (honoured on
 * 32-bit hosts only): use the arena's host addresses, so arenaBase is 0 and the
 * frame can go through port_gfx_task exactly as a game task does. */
const SynthFrame *synth_build(int realPointers);

#endif
