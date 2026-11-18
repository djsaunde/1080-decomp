/* PC port, plan 21.12: the runtime half of port/tools/icall/callscan.cc's `probe` option.  The plugin puts
 * __icall_probe(site, fn) before every indirect call of the probe build; this records the distinct
 * (site id, target address) pairs and writes them at exit to $ICALL_OUT.<pid> ("%08x %x" per line).
 * Linked into the probe build only (port/tools/icall-trace.py). */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N (1u << 18)
static unsigned ti[N], tf[N];
static int inited;
static void dump(void)
{
    const char *p = getenv("ICALL_OUT");
    char fn[1024];
    FILE *f;
    unsigned i;
    if (!p) return;
    snprintf(fn, sizeof fn, "%s.%d", p, (int)getpid());
    f = fopen(fn, "w");
    if (!f) return;
    for (i = 0; i < N; i++)
        if (tf[i]) fprintf(f, "%08x %x\n", ti[i], tf[i]);
    fclose(f);
}
void __icall_probe(unsigned id, void *fn)
{
    unsigned a = (unsigned)(unsigned long)fn;
    unsigned h = (id * 2654435761u ^ a * 40503u) & (N - 1);
    if (!inited) {
        inited = 1;
        atexit(dump);
    }
    while (tf[h]) {
        if (tf[h] == a && ti[h] == id) return;
        h = (h + 1) & (N - 1);
    }
    ti[h] = id;
    tf[h] = a;
}
