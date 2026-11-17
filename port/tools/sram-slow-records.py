#!/usr/bin/env python3
"""Make a save whose time-attack records are all slow, so any finished run is a top-3 time and the
name-entry screen (nameent.uso) follows the results (PC port plan 20 / run 855).

    port/tools/sram-slow-records.py IN.sram OUT.sram [MIN SEC HUNDREDTHS]

IN is an N64 SRAM image (the port's 1080.sram, e.g. from a 30-frame boot on a blank save). Every
course's three BackupTimeEntry times (gdl/backup.h: course record k at 0x8 + k * 0x30, entry j at
+j * 8, time[3] at +5) become MIN'SEC"HUNDREDTHS + j (default 9'59"97..99); the four chunk checksums,
the mirrors and the header hash are recomputed the hardware's way (port/fs/test_backup.c hw_seal),
so the game loads it with no chunk reset (1 SRAM write on boot, the header)."""
import struct
import sys

CHUNKS = [  # primary, sum, mirror, mirror sum, size
    (0x008, 0x188, 0x7DD8, 0x7F58, 0x180),  # course
    (0x190, 0x1F0, 0x7F60, 0x7FC0, 0x60),   # arcade
    (0x1F8, 0x210, 0x7FC8, 0x7FE0, 0x18),   # options
    (0x218, 0x228, 0x7FE8, 0x7FF8, 0x10),   # ghost header
]
CHUNK_KEY, HEADER_KEY = 0xDEADBBAD, 0xD1265205


def checksum(src, key):
    s = sum(struct.unpack('>%dI' % (len(src) // 4), bytes(src))) & 0xFFFFFFFF
    c0 = (0xF251F205 - s) & 0xFFFFFFFF
    return struct.pack('>II', c0, (key - (c0 - s)) & 0xFFFFFFFF)


def main():
    if len(sys.argv) not in (3, 6):
        sys.exit(__doc__)
    img = bytearray(open(sys.argv[1], 'rb').read())
    if len(img) != 0x8000:
        sys.exit('%s: not a 32 KB SRAM image' % sys.argv[1])
    m, s, c = (int(x) for x in sys.argv[3:6]) if len(sys.argv) == 6 else (9, 59, 97)
    for k in range(8):
        for j in range(3):
            e = 0x8 + k * 0x30 + j * 8
            img[e + 5:e + 8] = bytes((m, s, min(c + j, 99)))
    hdr = b''
    for off, sm, mir, mirs, size in CHUNKS:
        s8 = checksum(img[off:off + size], CHUNK_KEY)
        img[sm:sm + 8] = s8
        img[mir:mir + size] = img[off:off + size]
        img[mirs:mirs + 8] = s8
        hdr += s8
    img[0:8] = checksum(hdr, HEADER_KEY)
    open(sys.argv[2], 'wb').write(img)


if __name__ == '__main__':
    main()
