#include "common.h"

/* kernel_000_o1: -O1 file-split holding the two tail functions of kernel_000
 * (kdebug_send, kdebugserver) that are genuinely -O1 (verified 2026-06-04:
 * each spills every local to the stack and reloads per use; an -O2 build keeps
 * them in $s/$a regs and scores ~34-39%, an -O1 build scores 72-78%). Their
 * kernel_000 siblings kdebug_send_packet / kdebug_clear_IP6 stay -O2 (they score HIGHER
 * at -O2), so only these two are carved out. objdiff compares per-function so the
 * .ld placement (right after kernel_000.c.o) is purely cosmetic. */

extern void* __osRunningThread;

/* kdebug_send: chunked-write driver. Borrows hardware (init via
 * func_80004C7C if __osRdb_IP6_Empty was clear), processes arg1 bytes from arg0
 * in chunks of <=3 via func_80004C08, re-arming the hardware between chunks,
 * then conditionally re-invokes func_80004C7C if __osRdb_IP6_Empty was already set
 * on entry. -O1 file-split (matched 2026-06-23). */
extern s32 __osRdb_IP6_Empty;
extern void func_80004C7C(void);
extern void func_80004C08(char *p, s32 n);
void kdebug_send(char *arg0, s32 arg1) {
    s32 chunkSize;
    s32 offset;
    s32 wasInit;

    offset = 0;
    if (__osRdb_IP6_Empty == 0) {
        func_80004C7C();
        wasInit = 0;
    } else {
        wasInit = 1;
    }

    while (arg1 != 0) {
        chunkSize = ((u32)arg1 < 3) ? arg1 : 3;
        func_80004C08(arg0 + offset, chunkSize);
        arg1 -= chunkSize;
        offset += chunkSize;
        if (arg1 != 0) {
            func_80004C7C();
        }
    }

    if (wasInit != 0) {
        func_80004C7C();
    }
}

/* kdebugserver: 64-insn rmon/kdebugserver packet parser. Receives 24
 * bits per call (high 3 bytes of arg0); appends them to packet buffer
 * kdebug_buffer[] at write-position kdebug_numChars. After append:
 *   - if buf[0] == 2: send a fixed-size packet (0x190 bytes from
 *     __osRunningThread+0x20 via func_80004CE8); reset position
 *   - else if pos < 9: return (need more data)
 *   - else if buf[0] == 1: parse two u32s (BE) at buf[1] and buf[5]
 *     via func_80004BB0; pass to func_80004CE8; reset position
 *   - else: return (other tag — discard)
 * -O1 split: byte-exact match (loop reads kdebug_numChars + i directly, no
 * cached pos; ++i pre-incr in the while test; Y,X decl order = reverse
 * stack-slot numbering at -O1). */
extern u32 kdebug_numChars;
extern u8 kdebug_buffer[];
extern void func_80004CE8(u32 a, u32 b);
extern u32 func_80004BB0(u8 *p);
void kdebugserver(u32 arg0) {
    s32 i;
    u32 Y, X;

    i = 0;
    do {
        kdebug_buffer[kdebug_numChars] = ((u8*)&arg0)[1 + i];
        kdebug_numChars = kdebug_numChars + 1;
    } while ((u32)++i < 3);

    if (kdebug_buffer[0] == 2) {
        func_80004CE8((u32)((u8*)__osRunningThread + 0x20), 0x190);
        kdebug_numChars = 0;
    } else if (kdebug_numChars >= 9) {
        if (kdebug_buffer[0] == 1) {
            X = func_80004BB0(&kdebug_buffer[1]);
            Y = func_80004BB0(&kdebug_buffer[5]);
            func_80004CE8(X, Y);
            kdebug_numChars = 0;
        }
    }
}
