/* port/fs/backup_sram.c: the SRAM save in the N64's byte order (the rule: port/include/port_backup.h,
 * docs/1080_PC_PORT_PLAN.md 20).  The game's Backup_read / Backup_write / backup_checksum /
 * backup_checksumOk (game_libs_tail.c) call these under PORT.
 *
 * Plain C, no libc headers (as port/fs/fs.c). */
#include "common.h"
#include "gdl/backup.h"
#include "include/port_backup.h"

#define SRAM_SIZE 0x8000 /* PORT_SRAM_SIZE (port/ultra/ultra.h) */

void port_ultra_log(const char *fmt, ...);

static u8 sImage[SRAM_SIZE];
static int sWarned;

static void swap32(u8 *b) {
    u8 t = b[0];
    b[0] = b[3];
    b[3] = t;
    t = b[1];
    b[1] = b[2];
    b[2] = t;
}

static void swap16(u8 *b) {
    u8 t = b[0];
    b[0] = b[1];
    b[1] = t;
}

int port_backup_kind_at(int sramOff) {
    switch (sramOff) {
    case BACKUP_SRAM_HEADER:
    case BACKUP_SRAM_COURSE_SUM:
    case BACKUP_SRAM_ARCADE_SUM:
    case BACKUP_SRAM_OPTIONS_SUM:
    case BACKUP_SRAM_GHOST_SUM:
    case BACKUP_SRAM_COURSE_MIRROR_SUM:
    case BACKUP_SRAM_ARCADE_MIRROR_SUM:
    case BACKUP_SRAM_OPTIONS_MIRROR_SUM:
    case BACKUP_SRAM_GHOST_MIRROR_SUM:
    case BACKUP_SRAM_GHOST:
    case BACKUP_SRAM_GHOST_MIRROR:
        return PORT_BACKUP_WORDS;
    case BACKUP_SRAM_COURSE:
    case BACKUP_SRAM_COURSE_MIRROR:
        return PORT_BACKUP_COURSE;
    case BACKUP_SRAM_ARCADE:
    case BACKUP_SRAM_ARCADE_MIRROR:
        return PORT_BACKUP_ARCADE;
    case BACKUP_SRAM_OPTIONS:
    case BACKUP_SRAM_OPTIONS_MIRROR:
        return PORT_BACKUP_OPTIONS;
    case BACKUP_SRAM_GHOST_DATA:
        return PORT_BACKUP_GHOST_DATA;
    }
    return PORT_BACKUP_UNKNOWN;
}

int port_backup_kind_of_len(int len) {
    switch (len) {
    case sizeof(BackupCourse):
        return PORT_BACKUP_COURSE;
    case sizeof(BackupArcade):
        return PORT_BACKUP_ARCADE;
    case sizeof(BackupOptions):
        return PORT_BACKUP_OPTIONS;
    }
    return PORT_BACKUP_WORDS;
}

void port_backup_swap(void *buf, int kind, int size) {
    u8 *b = (u8 *)buf;
    int i, j;

    switch (kind) {
    case PORT_BACKUP_WORDS:
        for (i = 0; i + 4 <= size; i += 4) {
            swap32(b + i);
        }
        break;
    case PORT_BACKUP_COURSE: /* BackupCourseRecord: times[3] bytes, scores[3].score */
        for (i = 0; i + (int)sizeof(BackupCourseRecord) <= size; i += sizeof(BackupCourseRecord)) {
            for (j = 0; j < 3; j++) {
                swap32(b + i + 0x18 + j * sizeof(BackupScoreEntry) + 4);
            }
        }
        break;
    case PORT_BACKUP_ARCADE: /* BackupArcadeEntry: flags word, 4 bytes, stages[5] + score */
        for (i = 0; i + (int)sizeof(BackupArcadeEntry) <= size; i += sizeof(BackupArcadeEntry)) {
            swap32(b + i);
            for (j = 8; j < 0x20; j += 4) {
                swap32(b + i + j);
            }
        }
        break;
    case PORT_BACKUP_OPTIONS: /* unlocks, seqStep, flags */
        for (i = 0; i < 6 && i + 2 <= size; i += 2) {
            swap16(b + i);
        }
        break;
    case PORT_BACKUP_GHOST_DATA: /* InputFrame: stickX, stickY, buttons */
        for (i = 0; i + 4 <= size; i += 4) {
            swap16(b + i + 2);
        }
        break;
    default:
        break;
    }
}

static int kind_checked(int sramOff, int size) {
    int kind = port_backup_kind_at(sramOff);

    if (kind == PORT_BACKUP_UNKNOWN && size > 0 && !sWarned) {
        sWarned = 1;
        port_ultra_log("backup: SRAM access at 0x%X (0x%X bytes) is not a chunk start: no byte-order conversion",
                       sramOff, size);
    }
    return kind;
}

void port_backup_from_sram(void *buf, int sramOff, int size) {
    port_backup_swap(buf, kind_checked(sramOff, size), size);
}

void *port_backup_to_sram(const void *buf, int sramOff, int size) {
    int kind = kind_checked(sramOff, size);
    int i;

    if (size < 0 || size > SRAM_SIZE) {
        port_ultra_log("backup: SRAM write of 0x%X bytes at 0x%X: too big to convert", size, sramOff);
        return (void *)buf;
    }
    for (i = 0; i < size; i++) {
        sImage[i] = ((const u8 *)buf)[i];
    }
    port_backup_swap(sImage, kind, size);
    return sImage;
}

int port_backup_sum(const void *buf, int len) {
    int kind = port_backup_kind_of_len(len);
    u8 tmp[sizeof(BackupCourse)];
    const u8 *b = (const u8 *)buf;
    u32 sum = 0;
    int i, n = len / 4;

    if (kind == PORT_BACKUP_WORDS) { /* the same words in either order */
        for (i = 0; i < n; i++) {
            sum += ((const u32 *)buf)[i];
        }
        return (int)sum;
    }
    for (i = 0; i < n * 4; i++) {
        tmp[i] = b[i];
    }
    port_backup_swap(tmp, kind, n * 4);
    for (i = 0; i < n * 4; i += 4) {
        sum += ((u32)tmp[i] << 24) | ((u32)tmp[i + 1] << 16) | ((u32)tmp[i + 2] << 8) | tmp[i + 3];
    }
    return (int)sum;
}
