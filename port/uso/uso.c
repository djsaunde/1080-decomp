/* port/uso: module registry and startup relocation (port_uso.h; plan section 16.5). */
#include "port_uso.h"

/* no libc headers: the port still builds without a 32-bit libc (gcc-multilib) */
#define memcpy __builtin_memcpy
#define memset __builtin_memset

static int name_eq(const char *a, const char *b)
{
    while (*a != 0 && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

#define PORT_USO_MAX_ID 135     /* gUsoModuleSlots[135] (USO_FORMAT section 8) */

static unsigned char sLoaded[PORT_USO_MAX_ID];

static void relocate(char *base, const PortUsoReloc *rel, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++) {
        unsigned int v = (unsigned int)(unsigned long)rel[i].addr;
        memcpy(base + rel[i].off, &v, 4);  /* host order: the code dereferences it */
    }
}

/* restore the module's data to its state right after uso_load on the N64 */
static void module_reset(const PortUsoModule *m)
{
    if (m->data != 0) {
        memcpy(m->data, m->dataInit, m->dataSize);
        relocate(m->data, m->dataRel, m->nDataRel);
    }
    if (m->rodata != 0) {
        memcpy(m->rodata, m->rodataInit, m->rodataSize);
        relocate(m->rodata, m->rodataRel, m->nRodataRel);
    }
    if (m->bss != 0) {
        memset(m->bss, 0, m->bssSize);
    }
}

const PortUsoModule *port_uso_find(const char *path)
{
    const char *file = path;
    const char *p;
    int i;

    for (p = path; *p != 0; p++) {
        if (*p == '/') {
            file = p + 1;
        }
    }
    for (i = 0; i < port_uso_nmodules; i++) {
        if (name_eq(port_uso_modules[i].name, file)) {
            return &port_uso_modules[i];
        }
    }
    return 0;
}

int port_uso_isLoaded(int id)
{
    return id >= 0 && id < PORT_USO_MAX_ID && sLoaded[id];
}

int port_uso_load(const char *path)
{
    const PortUsoModule *m = port_uso_find(path);
    void (*const *f)(void);

    if (m == 0) {
        return -0x14;           /* uso_load: name not found */
    }
    if (sLoaded[m->id]) {
        return -0x13;           /* uso_load: already live */
    }
    module_reset(m);
    sLoaded[m->id] = 1;
    for (f = m->inits; f != 0 && *f != 0; f++) {
        (*f)();
    }
    return m->id;
}

int port_uso_unload(const char *path)
{
    const PortUsoModule *m = port_uso_find(path);

    if (m == 0 || !sLoaded[m->id]) {
        return -1;
    }
    sLoaded[m->id] = 0;
    return 0;
}

void port_uso_init(void)
{
    port_uso_load("mainuso/bootup.uso");
}
