#include "common.h"



/* split from kernel_004.c - 2026-06-10 kernel ROM-order relayout */


/* func_80004FD0 (with absorbed func_80004FE0 fragment) split out to kernel_053.c (-O1) */

/* func_80004FE0 merged into func_80004FD0 */

/* func_80005000 = osInvalDCache (handwritten cache ops). Real source; see docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/invaldcache.s")

/* func_800050C0 split out to kernel_051.c (-O1) */


/* func_80005134 split out to kernel_050.c (-O1) */

/* func_80005184 split out to kernel_052.c (-O1) */


/* func_800051F0 split out to kernel_055.c (-O1) */
