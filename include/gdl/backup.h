#ifndef GDL_BACKUP_H
#define GDL_BACKUP_H

/* libgdl Backup = the SRAM save (game_libs 0x9978-0xC49C, game_libs_tail.c).
 * docs/1080_SYSTEM_LAYER.md section 4 has the chunk table; docs/1080_MODES.md
 * "Run 474" has the evidence for each field.
 *
 * Widths and signedness follow the matched loads / stores (READABILITY_PLAN 9.1).
 * Offsets and sizes are asserted at the bottom. */

/* ---- SRAM layout (Backup_read / Backup_write offsets) ----
 * Every chunk is followed by an 8-byte checksum pair (backup_checksum with
 * BACKUP_CHUNK_KEY). Each chunk and its checksum are mirrored near the top of the
 * 32 KB SRAM; Backup_check<X> falls back to the mirror and restores the primary.
 * The header is the four chunk checksums hashed with BACKUP_HEADER_KEY. */
#define BACKUP_SRAM_HEADER          0x000   /* 8 */
#define BACKUP_SRAM_COURSE          0x008   /* BackupCourse, 0x180 */
#define BACKUP_SRAM_COURSE_SUM      0x188
#define BACKUP_SRAM_ARCADE          0x190   /* BackupArcade, 0x60 */
#define BACKUP_SRAM_ARCADE_SUM      0x1F0
#define BACKUP_SRAM_OPTIONS         0x1F8   /* BackupOptions, 0x18 */
#define BACKUP_SRAM_OPTIONS_SUM     0x210
#define BACKUP_SRAM_GHOST           0x218   /* BackupGhost header, 0x10 */
#define BACKUP_SRAM_GHOST_SUM       0x228
#define BACKUP_SRAM_GHOST_DATA      0x230   /* BackupGhost.count words */
#define BACKUP_SRAM_COURSE_MIRROR       0x7DD8
#define BACKUP_SRAM_COURSE_MIRROR_SUM   0x7F58
#define BACKUP_SRAM_ARCADE_MIRROR       0x7F60
#define BACKUP_SRAM_ARCADE_MIRROR_SUM   0x7FC0
#define BACKUP_SRAM_OPTIONS_MIRROR      0x7FC8
#define BACKUP_SRAM_OPTIONS_MIRROR_SUM  0x7FE0
#define BACKUP_SRAM_GHOST_MIRROR        0x7FE8
#define BACKUP_SRAM_GHOST_MIRROR_SUM    0x7FF8

#define BACKUP_CHUNK_KEY    0xDEADBBAD  /* backup_checksum key of every chunk */
#define BACKUP_HEADER_KEY   0xD1265205  /* key of the header hash */

/* Valid bit in byte 0 of every record entry (Backup_func_00009A2C / 9B98 / 9CDC
 * test it, 9978 / 9AFC / 9BF4 clear it, the setters OR it in). */
#define BACKUP_ENTRY_VALID  0x80

/* ---- records ---- */

/* Best-time entry (8). Setter 9AD8 (time + extra), 99DC (defaults: "ead"-style
 * initials stored as letter - 'a'), 9988 (initials + the packed flag bits a2 | a3 << 3).
 * Compare 9A50: time[0..2] most significant first; Backup_func_0000A1B0 builds the
 * triple from milliseconds as minutes, seconds, hundredths. */
typedef struct BackupTimeEntry {
    /* 0x0 */ u8 flags;         /* BACKUP_ENTRY_VALID | (a3 << 3) | a2 (low 7 bits from 9988) */
    /* 0x1 */ u8 extra;         /* 9AD8 arg 4 */
    /* 0x2 */ u8 name[3];       /* initials, letter - 'a' */
    /* 0x5 */ u8 time[3];       /* minutes, seconds, hundredths */
} BackupTimeEntry;

/* Best-score entry (8). Setter 9BE0 (score), 9B60 (defaults), 9B0C (initials +
 * flag bits); compare 9BBC (score < arg). */
typedef struct BackupScoreEntry {
    /* 0x0 */ u8 flags;         /* BACKUP_ENTRY_VALID | flag bits (as BackupTimeEntry) */
    /* 0x1 */ u8 name[3];
    /* 0x4 */ s32 score;
} BackupScoreEntry;

/* One course's records (0x30): the three best time-attack times, then the three
 * best trick-mode scores (A0CC / A130 count the valid ones, A194 / A1A0 index them;
 * TimRace_checkRecord compares time[0..2] at bytes 5-7). */
typedef struct BackupCourseRecord {
    /* 0x00 */ BackupTimeEntry times[3];
    /* 0x18 */ BackupScoreEntry scores[3];
} BackupCourseRecord;

#define BACKUP_NUM_COURSES  8

/* Course chunk (0x180), indexed by the course number (game global +0x64). */
typedef struct BackupCourse {
    /* 0x000 */ BackupCourseRecord course[BACKUP_NUM_COURSES];
} BackupCourse;

/* Arcade entry (0x20). Setter 9D24 (score, count, stages[]), 9C5C (defaults,
 * count 5), 9C04 (initials + flag bits in the flags word); compare 9D00
 * (score < arg). */
typedef struct BackupArcadeEntry {
    /* 0x00 */ s32 flags;       /* word access: BACKUP_ENTRY_VALID | flag bits */
    /* 0x04 */ s8 count;        /* number of stages[] used (5 in the defaults) */
    /* 0x05 */ u8 name[3];
    /* 0x08 */ s32 stages[5];
    /* 0x1C */ s32 score;
} BackupArcadeEntry;

/* Arcade chunk (0x60): three ranked entries (AD20 indexes, AD2C finds the rank). */
typedef struct BackupArcade {
    /* 0x00 */ BackupArcadeEntry entry[3];
} BackupArcade;

/* Options chunk (0x18), defaults from Backup_resetOptionsData_0000AFC4. */
typedef struct BackupOptions {
    /* 0x00 */ s16 unk0;        /* default 0x1F */
    /* 0x02 */ s16 unk2;        /* default 1 */
    /* 0x04 */ u16 flags;       /* & 3 -> bootup 0x1FC; bit 2 -> gl_flag_AFC4_bit2 */
    /* 0x06 */ u8 unk6[8];
    /* 0x0E */ u8 unkE[4];
    /* 0x12 */ u8 levels[3];    /* default 8 each; passed to bootup 0x21C / 0x23C / 0x25C */
    /* 0x15 */ u8 pad15[3];
} BackupOptions;

/* Ghost (replay) chunk header (0x10). The data (count words) follows in SRAM at
 * BACKUP_SRAM_GHOST_DATA; in RAM `data` points at it (Backup_readGhost). */
typedef struct BackupGhost {
    /* 0x0 */ s32 bits;         /* bitfields: 0-1 mode (A878 / A884), 2-4 (A8A4 / A8B4), 5 (A8D8 / A900),
                                 * 6-9 (A928 / A938), 10-13 course (A95C / A96C; openSession
                                 * keeps the ghost only if mode and course match, AA28) */
    /* 0x4 */ s32 count;        /* data length in words */
    /* 0x8 */ s32 data;         /* RAM pointer (zeroed in the SRAM copy) */
    /* 0xC */ s32 unkC;
} BackupGhost;

/* The Backup object (8, Backup_ctor). */
typedef struct Backup {
    /* 0x0 */ s32 unk0;         /* 6400 */
    /* 0x4 */ s32 ghostBits;    /* copy of BackupGhost.bits (Backup_writeGhost) */
} Backup;

/* The record session (0x18, Backup_openSession / Backup_closeSession; the mode
 * modules wrap it as <mode>_openSession). Held by InputMgr.session,
 * RaceProcess.session and RaceTracker.session. */
typedef struct BackupSession {
    /* 0x00 */ BackupCourse *course;    /* Backup_readCourse */
    /* 0x04 */ BackupArcade *arcade;    /* Backup_readArcade */
    /* 0x08 */ BackupGhost *ghost;      /* Backup_readGhost */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ Backup *backup;          /* openSession arg 1 */
    /* 0x14 */ s32 modes;       /* BACKUP_MODE_*: which chunks the session writes back */
} BackupSession;

/* BackupSession.modes: the mode that opened the session (tim_openSession stores 1,
 * trk_openSession 2). B3DC / B450 write the course chunk back for 1 or 2 and the
 * arcade chunk for 4; B4A4 updates course times (1), course scores (2) or arcade (4).
 * The same value is the mode in BackupGhost.bits 0-1 (Backup_func_0000AA28). */
#define BACKUP_MODE_TIME    1   /* time attack: BackupCourseRecord.times */
#define BACKUP_MODE_TRICK   2   /* trick mode: BackupCourseRecord.scores */
#define BACKUP_MODE_ARCADE  4

typedef char BackupTimeEntry_size_check[sizeof(BackupTimeEntry) == 8 ? 1 : -1];
typedef char BackupScoreEntry_size_check[sizeof(BackupScoreEntry) == 8 ? 1 : -1];
typedef char BackupCourseRecord_size_check[sizeof(BackupCourseRecord) == 0x30 ? 1 : -1];
typedef char BackupCourse_size_check[sizeof(BackupCourse) == 0x180 ? 1 : -1];
typedef char BackupArcadeEntry_size_check[sizeof(BackupArcadeEntry) == 0x20 ? 1 : -1];
typedef char BackupArcadeEntry_score_check[(int)&((BackupArcadeEntry *)0)->score == 0x1C ? 1 : -1];
typedef char BackupArcade_size_check[sizeof(BackupArcade) == 0x60 ? 1 : -1];
typedef char BackupOptions_size_check[sizeof(BackupOptions) == 0x18 ? 1 : -1];
typedef char BackupOptions_levels_check[(int)&((BackupOptions *)0)->levels == 0x12 ? 1 : -1];
typedef char BackupGhost_size_check[sizeof(BackupGhost) == 0x10 ? 1 : -1];
typedef char Backup_size_check[sizeof(Backup) == 8 ? 1 : -1];
typedef char BackupSession_size_check[sizeof(BackupSession) == 0x18 ? 1 : -1];
typedef char BackupSession_modes_check[(int)&((BackupSession *)0)->modes == 0x14 ? 1 : -1];

#endif
