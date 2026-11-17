/* port/fs/backup_sram.c: the SRAM save's byte order (docs/1080_PC_PORT_PLAN.md 20, M5 prep).
 *
 * THE RULE.  The save file (1080.sram, PORT_SRAM_SIZE bytes) is the N64 SRAM image byte for byte:
 * big-endian, the layout of gdl/backup.h, the checksums the Backup code computes on hardware.  A
 * .sra from an emulator loads as is, and the port's file loads in an emulator.
 *
 *   - In RAM every Backup record is in HOST order (the game's own field stores / loads, as on
 *     any other struct).
 *   - The conversion happens at the SRAM boundary and nowhere else: Backup_read converts the
 *     chunk it just read (port_backup_from_sram), Backup_write writes a big-endian COPY
 *     (port_backup_to_sram; the caller's record, often the live options record, is not
 *     touched).  The chunk's SRAM offset says which record it is (backup_kind_at below):
 *     every Backup_read / Backup_write in the game addresses one whole chunk at its start.
 *   - The checksums are computed over the record's BIG-ENDIAN image (port_backup_sum), so the
 *     8-byte sums and the header hash are the hardware's values.  backup_checksum /
 *     backup_checksumOk get the record without its offset; the length identifies it (0x180
 *     course, 0x60 arcade, 0x18 options; every other checksummed buffer, the 0x10 ghost header
 *     and the 0x20 header input, is all words, whose sum does not depend on the byte order).
 *
 * Field widths (from the matched loads / stores, gdl/backup.h):
 *   course   BackupTimeEntry all bytes; BackupScoreEntry.score s32 (+4)
 *   arcade   BackupArcadeEntry.flags s32 (+0, word access), count / name bytes,
 *            stages[5] + score s32 (+8..+0x1F)
 *   options  unlocks / seqStep / flags u16 (+0, +2, +4), the rest bytes
 *   ghost    the header is 4 words (bits, count, data = 0 in SRAM, unkC); the data is
 *            InputFrame (gdl/pad.h): stickX / stickY bytes, buttons s16 (+2)
 *   sums     two s32 per chunk; the header hash two s32
 */
#ifndef PORT_BACKUP_H
#define PORT_BACKUP_H

#ifdef __cplusplus
extern "C" {
#endif

enum PortBackupKind {
    PORT_BACKUP_UNKNOWN = 0, /* not a chunk start: left as is (logged once) */
    PORT_BACKUP_WORDS,       /* the header hash, the 8-byte sums, the ghost header */
    PORT_BACKUP_COURSE,      /* BackupCourse */
    PORT_BACKUP_ARCADE,      /* BackupArcade */
    PORT_BACKUP_OPTIONS,     /* BackupOptions */
    PORT_BACKUP_GHOST_DATA   /* InputFrame[count] */
};

/* the record at SRAM offset sramOff (a chunk start), and the checksummed record of len bytes */
int port_backup_kind_at(int sramOff);
int port_backup_kind_of_len(int len);

/* Convert size bytes of a record of the given kind in place, host <-> big-endian (the same
 * swap both ways on a little-endian host).  Whole elements only; a trailing part is left. */
void port_backup_swap(void *buf, int kind, int size);

/* Backup_read, after the DMA: the chunk at sramOff, big-endian -> host, in place. */
void port_backup_from_sram(void *buf, int sramOff, int size);

/* Backup_write, before the DMA: a big-endian copy of the host record (a static buffer, valid
 * until the next call).  size > PORT_SRAM_SIZE: the record itself, unconverted (logged). */
void *port_backup_to_sram(const void *buf, int sramOff, int size);

/* backup_checksum / backup_checksumOk: the sum of the len / 4 big-endian words of the record's
 * SRAM image (what the N64 CPU sums). */
int port_backup_sum(const void *buf, int len);

#ifdef __cplusplus
}
#endif

#endif
