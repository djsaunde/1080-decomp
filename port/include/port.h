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

/* Partial reads (plan 18.12): code that packs bytes / halves into a word by shifting
 * (0xOOAABBCC) and reads them back as u8 / s16 fields of memory relied on big-endian order.
 * PORT_BE_BYTES32(x) is the host word whose bytes in memory are x's big-endian bytes; the
 * HI16 / LO16 forms are the halves a big-endian s16 read at +0 / +2 of the int x gives. */
#define PORT_BE_BYTES32(x) __builtin_bswap32((unsigned int)(x))
#define PORT_BE_HI16(x) ((short)((unsigned int)(x) >> 16))
#define PORT_BE_LO16(x) ((short)(x))

/* RDP-visible memory (plan 19.9).  Two rules, both what libultraship's Fast3D reads:
 *  - RSP data (display-list words, Vtx, Mtx, Vp, Lights) is in HOST order: the game writes it with
 *    stores of the field's own width, and the blob typing swaps static copies (u32 / s16 fields);
 *  - RDP pixels (texels, TLUTs, the colour and depth images) are the N64 BIG-endian byte image:
 *    blobs keep texels as bytes, copies u16 for u16 keep the order, and read-backs store
 *    big-endian.  A CPU store or compare of a 16-bit pixel VALUE goes through PORT_RDP16(v): the
 *    host u16 whose bytes in memory are v's big-endian bytes. */
#define PORT_RDP16(v) ((unsigned short)__builtin_bswap16((unsigned short)(v)))

/* Bitfield order (plan 18.3): IDO allocates bitfields MSB-first, gcc / clang on x86 LSB-first.
 * Code that reads every field through the members is consistent either way; the sites that test
 * or set flag bits with masks and shifts (the audio player / channel / layer / note flag words,
 * whose fields are 1-bit and declared in order) use these under PORT instead of the IDO masks.
 * Field i (declaration order, counting from the word's first byte):
 *   AUDIO_FLAGBIT(i)  its mask inside its byte (byte i / 8)   -- IDO: 0x80 >> (i & 7)
 *   AUDIO_WORDBIT(i)  its mask in the u32 word at +0          -- IDO: 0x80000000 >> i */
#define AUDIO_FLAGBIT(i) (1u << ((i) & 7))
#define AUDIO_WORDBIT(i) (1u << (i))
/* The Stereo union byte {unused:2, bit2:2, strongRight, strongLeft, stereoHeadsetEffects,
 * usesHeadsetPanEffects} arrives in IDO bit order (sequence script bytes, SET_STEREO commands);
 * re-pack it into the host's order (field by field: the 2-bit fields keep their values). */
static __inline__ unsigned char port_stereo_from_ido(unsigned int b) {
    return (unsigned char)(((b >> 6) & 3) | (((b >> 4) & 3) << 2) | (((b >> 3) & 1) << 4)
                           | (((b >> 2) & 1) << 5) | (((b >> 1) & 1) << 6) | ((b & 1) << 7));
}

#endif
