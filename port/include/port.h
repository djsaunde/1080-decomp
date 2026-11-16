/* Forced into every port compile (port/CMakeLists.txt).  PORT-only definitions
 * that cannot be passed as -D (CMake drops function-like macros). */
#ifndef PORT_PORT_H
#define PORT_PORT_H

/* asm-processor's bare GLOBAL_ASM("x.s") lines: the shim / port code replaces
 * the hand-written assembly (plan decision 6). */
#define GLOBAL_ASM(x)

/* Byte order (plan "M1/M2: byte order"): ROM files are big-endian.  The fixed-size parm / stream
 * readers (fs_gdlRead{Int,Float,Quad,Vec3}_*, stream_read*_*) swap their buffer in place under
 * PORT.  The tag readers (fs_gdlReadInt_00035AA4, stream_readS32_00005EF8) do not: tags compare
 * against unswapped string words from USO data. */
static __inline__ void port_be_swap32s(void *p, int nbytes) {
    unsigned char *b = (unsigned char *)p, t;
    int i;
    for (i = 0; i + 4 <= nbytes; i += 4) {
        t = b[i]; b[i] = b[i + 3]; b[i + 3] = t;
        t = b[i + 1]; b[i + 1] = b[i + 2]; b[i + 2] = t;
    }
}
#define PORT_BE_SWAP32S(p, nbytes) port_be_swap32s((p), (nbytes))
static __inline__ void port_be_swap16s(void *p, int nbytes) {
    unsigned char *b = (unsigned char *)p, t;
    int i;
    for (i = 0; i + 2 <= nbytes; i += 2) {
        t = b[i]; b[i] = b[i + 1]; b[i + 1] = t;
    }
}
#define PORT_BE_SWAP16S(p, nbytes) port_be_swap16s((p), (nbytes))

#endif
