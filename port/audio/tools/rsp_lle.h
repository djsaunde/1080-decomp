/* port/audio/tools/rsp_lle.h: 1080's audio microcode on a minimal RSP interpreter (rsp_lle.c). */
#ifndef RSP_LLE_H
#define RSP_LLE_H
#include <stdint.h>

typedef void *(*RspLleTranslate)(uint32_t addr);

typedef struct {
    unsigned long tasks, cmds, insns, task_cmd, dma_bytes, dma_unmapped, dma_misaligned, bad_insn;
} RspLleStats;

extern RspLleStats rsp_lle_stats;
/* per command of the list being run: 1 = a LOADBUFF that loads sample bytes (copied unswapped) */
extern const uint8_t *rsp_lle_sample_flags;

/* called before each DMA write to DRAM (host bytes about to change, the writing command's opcode) */
extern void (*rsp_lle_write_log)(uint8_t *host, uint32_t len, uint32_t op);

int rsp_lle_load(const uint8_t *rom, uint32_t romSize);
/* runs one audio task (the command list as the host holds it); 1 = it ended with the ucode's break */
int rsp_lle_run(const uint32_t *cmds, uint32_t ncmds, RspLleTranslate translate);

/* emulator-style: a big-endian RDRAM image and the task-start IMEM / DMEM, each byte a at [a ^ swz] */
int rsp_lle_run_raw(uint8_t *rdram, uint32_t size, const uint8_t *imem, const uint8_t *dmem, uint32_t swz);

#endif
