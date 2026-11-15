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

/* "New record" bit in byte 0 of every record entry: the setters (<x>EntrySet) OR it
 * in, the default tables never set it, <x>EntryIsNew tests it (the session counts
 * drive the name-entry screen), and Backup_writeCourse / writeArcade clear it on
 * every entry (courseClearNew / arcadeClearNew) before the chunk goes to SRAM. */
#define BACKUP_ENTRY_NEW    0x80

/* The low 7 flag bits: character | board << 3. The defaults use (i % 5) and
 * ((i % 8) << 3) (courseRecordResetTimes); <x>EntrySetName packs a2 | a3 << 3. */
#define BACKUP_ENTRY_CHAR(f)    ((f) & 7)
#define BACKUP_ENTRY_BOARD(f)   (((f) >> 3) & 0xF)

/* ---- records ---- */

/* Best-time entry (8). Backup_timeEntrySet (time + extra), timeEntryInit (defaults:
 * "ead"-style initials stored as letter - 'a'), timeEntrySetName (initials + character /
 * board). timeEntryIsSlower compares time[0..2] most significant first;
 * Backup_courseRecordFindTimeRank builds the triple from milliseconds as minutes,
 * seconds, hundredths. */
typedef struct BackupTimeEntry {
    /* 0x0 */ u8 flags;         /* BACKUP_ENTRY_NEW | board << 3 | character */
    /* 0x1 */ u8 extra;         /* timeEntrySet arg 4 */
    /* 0x2 */ u8 name[3];       /* initials, letter - 'a' */
    /* 0x5 */ u8 time[3];       /* minutes, seconds, hundredths */
} BackupTimeEntry;

/* Best-score entry (8). Backup_scoreEntrySet (score), scoreEntryInit (defaults),
 * scoreEntrySetName (initials + character / board); scoreEntryIsLower (score < arg). */
typedef struct BackupScoreEntry {
    /* 0x0 */ u8 flags;         /* as BackupTimeEntry.flags */
    /* 0x1 */ u8 name[3];
    /* 0x4 */ s32 score;
} BackupScoreEntry;

/* One course's records (0x30): the three best time-attack times, then the three
 * best trick-mode scores (courseRecordCountNewTimes / Scores count the new ones,
 * courseRecordGetTime / GetScore index them;
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

/* Arcade entry (0x20). Backup_arcadeEntrySet (score, count, stages[]), arcadeEntryInit
 * (defaults, count 5), arcadeEntrySetName (initials + character / board in the flags
 * word); arcadeEntryIsLower (score < arg). */
typedef struct BackupArcadeEntry {
    /* 0x00 */ s32 flags;       /* word access: BACKUP_ENTRY_NEW | board << 3 | character */
    /* 0x04 */ s8 count;        /* number of stages[] used (5 in the defaults) */
    /* 0x05 */ u8 name[3];
    /* 0x08 */ s32 stages[5];
    /* 0x1C */ s32 score;
} BackupArcadeEntry;

/* Arcade chunk (0x60): three ranked entries (arcadeGetEntry indexes, arcadeFindRank
 * finds the rank, arcadeInsert shifts the lower ones down). */
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
    /* 0x0 */ s32 bits;         /* Backup_ghostGet / Set<field>: 0-1 Mode, 2-4 Bits2, 5 Flag5,
                                 * 6-9 Bits6, 10-13 Course. openSession keeps the ghost only
                                 * if mode and course match (ghostMatches). app_createGame
                                 * copies Bits2 / Flag5 / Bits6 to game global +0x181..0x183
                                 * (probably the ghost rider's character / variant / board) */
    /* 0x4 */ s32 count;        /* data length in words */
    /* 0x8 */ s32 data;         /* RAM pointer (zeroed in the SRAM copy) */
    /* 0xC */ s32 unkC;
} BackupGhost;

/* The Backup object (8, Backup_ctor). */
typedef struct Backup {
    /* 0x0 */ s32 unk0;         /* 6400 */
    /* 0x4 */ s32 ghostBits;    /* copy of BackupGhost.bits (Backup_writeGhost, Backup_cacheGhostBits) */
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
 * trk_openSession 2). Backup_writeCourseArcadeOptions / sessionClearNew handle the
 * course chunk for 1 or 2 and the arcade chunk for 4; sessionSetNewNames names the new
 * course times (1), course scores (2) or arcade entries (4).
 * The same value is the mode in BackupGhost.bits 0-1 (Backup_ghostMatches). */
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
