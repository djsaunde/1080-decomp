/* IDO 7.1 NCC (C++) donor for SnowAI_update (0x144 / 81 words): mode-value
 * pick + mode dispatch. The selector is ONE 8-way C++ switch -- edg lowers it to a
 * case-order compare chain (cfe tables any >= 5-case switch, which capped the cfe NM
 * at 82.5 with nested 3+4 switches). The 0x758 sub-object store is an inline
 * Val::set() whose `this` re-reads a0->0x30 (gives `addiu v0,a1,0x758; swc1 0x10(v0)`),
 * and the state pointer is read through a0->0x30 too, so the const 2 colours v0.
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY. */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/snowai.h" /* NCC has no -I include */
extern "C" {
void SnowAI_steer(char *);
void SnowAI_finishRun(char *);
void SnowAI_timeoutRun(char *);
void SnowAI_recoverStuck(char *);
}
struct Val { char pad[0x10]; float v; void set(float f) { v = f; } }; /* a parm: value at +0x10 */
#define AI(p) ((SnowAI *)(p))
#define MS(k) AI(a0)->parms.parm[SNOWAI_PARM_MS + (k)].value /* "ms1".."ms8": per-character max speed */
extern "C" void SnowAI_update(char *a0) {
    if (AI(a0)->initPending != 0) {
        AI(a0)->initPending = 0;
        SnowBoarder *rival = AI(a0)->rider->rival;
        /* CPU rider vs a pad rider. The unsigned compares keep the const 2 in v0 and the aiState load in the
         * branch-likely delay slots (signed s32 compares colour it a2 and fill nops) */
        if (rival != 0 && (u32)AI(a0)->rider->controller == 2 && (u32)rival->controller != 2) {
            float value;
            switch ((int)rival->character) {
            case 1: value = MS(0); break;
            case 2: value = MS(1); break;
            case 3: value = MS(2); break;
            case 4: value = MS(3); break;
            case 5: value = MS(4); break;
            case 6: value = MS(5); break;
            case 7: value = MS(6); break;
            default: value = MS(7); break;
            }
            ((Val *)&AI(a0)->rider->parms.max)->set(value);
        }
    }
    AI(a0)->unk3C = 0.0f;
    switch (AI(a0)->aiState) {
    case 0: SnowAI_steer(a0); break;
    case 1: SnowAI_finishRun(a0); break;
    case 2: SnowAI_timeoutRun(a0); break;
    case 3: SnowAI_recoverStuck(a0); break;
    }
}
