#include "common.h"



/* split from kernel_004.c - 2026-06-10 kernel ROM-order relayout */


/* osCreateMesgQueue (with absorbed func_80004FE0 fragment) split out to kernel_053.c (-O1) */

/* func_80004FE0 merged into osCreateMesgQueue */

/* osInvalDCache = osInvalDCache (handwritten cache ops). Real source; see docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/invaldcache.s")

/* __rmonSendFault split out to kernel_051.c (-O1) */


/* __rmonIOflush split out to kernel_050.c (-O1) */

/* __rmonIOputw split out to kernel_052.c (-O1) */


/* __rmonIOhandler split out to kernel_055.c (-O1) */
