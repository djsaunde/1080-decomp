/* port/uso: the USO module registry of the PC port (docs/1080_PC_PORT_PLAN.md section 16.5,
 * decision 4, USO_FORMAT section 13).
 *
 * Every module is linked statically.  What stays of a module at run time is its data: one blob
 * per Data / RoData / Bss section at the original layout (port_<mod>_data / _rodata / _bss,
 * generated at build time from the user's ROM by port/tools/gen-uso-blobs.py), the pristine
 * initial bytes of Data / RoData (port_<mod>_data_init ...), and the list of words that hold
 * addresses (port/gen/uso_modules.c, from the module's DataReloc / RoDataReloc records).
 *
 * "Loading" a module restores its Data / RoData from the pristine copy, writes the host address
 * into every relocated word, zeroes its Bss and runs its static initialisers, exactly as uso_load
 * re-reads a module from ROM on the N64 (USO_FORMAT 13.3).  32-bit host only: a relocated word
 * is a 4-byte pointer, stored in host byte order; the rest of the blob stays big-endian as on the
 * cartridge (plan 4.4: the readers swap).
 */
#ifndef PORT_USO_H
#define PORT_USO_H

typedef struct PortUsoReloc {
    unsigned int off;         /* byte offset of the word in its section */
    void *addr;               /* host address it must hold (0: not resolvable, see the comment) */
} PortUsoReloc;

typedef struct PortUsoModule {
    const char *name;         /* "bootup.uso": the file name uso_load is given (after the device) */
    int id;                   /* the ROM module id (Info record) */
    char *data;
    const char *dataInit;
    unsigned int dataSize;
    char *rodata;
    const char *rodataInit;
    unsigned int rodataSize;
    char *bss;
    unsigned int bssSize;
    const PortUsoReloc *dataRel;
    unsigned int nDataRel;
    const PortUsoReloc *rodataRel;
    unsigned int nRodataRel;
    void (*const *inits)(void);   /* static initialisers, 0-terminated (or 0) */
} PortUsoModule;

extern const PortUsoModule port_uso_modules[];
extern const int port_uso_nmodules;

/* registry (the SysApi usoLoad / usoUnload slots call these) */
void port_uso_init(void);                       /* load bootup.uso (always resident) */
int port_uso_load(const char *path);            /* "device/file.uso" -> module id, or < 0 */
int port_uso_unload(const char *path);
int port_uso_isLoaded(int id);
const PortUsoModule *port_uso_find(const char *path);

#endif
