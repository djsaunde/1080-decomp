/* port/gfx: port_gfx_task end to end on the -m32 target (make test M32=1).
 * The synthetic frame is built at its real host addresses and submitted as an
 * OSTask_t, with PORT_GFX_DUMP / _FIRST / _EVERY / _COUNT set, so this covers
 * the env handling, the mincore() readability check and the file naming. */

#include "../port_libc.h"
#include "../ultra/ultra.h"
#include "dlump.h"
#include "synth.h"

#include <stdarg.h>

static int sLogs;
void port_ultra_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    sLogs++;
}

int setenv(const char *name, const char *value, int overwrite);

int main(int argc, char **argv) {
    const SynthFrame *fr = synth_build(1);
    OSTask_t *task;
    char path[600];
    FILE *f;
    int i, fail = 0;
    const char *dir = argc > 1 ? argv[1] : ".";

    if (sizeof(void *) != 4 || fr->arenaBase != 0) {
        printf("test_port_gfx: needs the 32-bit target (skipped)\n");
        return 0;
    }
    setenv("PORT_GFX_DUMP", dir, 1);
    setenv("PORT_GFX_DUMP_FIRST", "1", 1);
    setenv("PORT_GFX_DUMP_EVERY", "2", 1);
    setenv("PORT_GFX_DUMP_COUNT", "3", 1);
    task = (OSTask_t *)(size_t)fr->taskAddr;
    for (i = 0; i < 8; i++) {
        snprintf(path, sizeof(path), "%s/gfx_%06d.dlmp", dir, i);
        remove(path);
    }
    for (i = 0; i < 8; i++) {
        if (i == 5) { /* an unmapped root: mincore() must refuse it, not crash */
            task->data_ptr = (u64 *)0x7FFF0000u;
        }
        port_gfx_task(task); /* tasks 1, 3 and 5 are dumped (5 with one walk error) */
    }
    for (i = 0; i < 8; i++) {
        int want = (i == 1 || i == 3 || i == 5);
        snprintf(path, sizeof(path), "%s/gfx_%06d.dlmp", dir, i);
        f = fopen(path, "rb");
        if ((f != NULL) != want) {
            printf("FAIL: %s %s\n", path, want ? "missing" : "unexpected");
            fail++;
        }
        if (f != NULL) {
            fclose(f);
        }
    }
    if (sLogs != 1) { /* the walk-error line for task 5 */
        printf("FAIL: %d log lines, expected 1\n", sLogs);
        fail++;
    }
    printf("test_port_gfx: %s\n", fail ? "FAIL" : "ok");
    return fail != 0;
}
