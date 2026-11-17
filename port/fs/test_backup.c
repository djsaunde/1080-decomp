/* The SRAM save format test (docs/1080_PC_PORT_PLAN.md 20; the rule: port/include/port_backup.h).
 *
 *   test_backup
 *       unit: every record kind host -> SRAM -> host is the identity, the SRAM bytes are the
 *       big-endian field values, port_backup_sum is the big-endian word sum, the hardware
 *       checksum of a converted record verifies.
 *   test_backup <save> <bootup Data> [<out.sra>]
 *       <save>: a save the game wrote on a blank SRAM (the port gate's smoke run);
 *       <bootup Data>: the extracted bootup.uso Data section (port/tools/extract:
 *       uso/bootup.uso/02_Data.bin, big-endian, as in the ROM).
 *       Builds the N64 image of the ROM's default records independently of the game code
 *       (Backup_resetCourseData / resetArcadeData / resetOptionsData / resetGhostData, the
 *       checksums, the mirrors, the header hash) and requires <save> to be byte-identical
 *       (the bytes the game leaves uninitialized are taken from <save>: options pad15[3],
 *       the ghost header's bits above bit 13 and unkC).  With <out.sra>, writes a synthetic
 *       big-endian image with NON-default records, a ghost and hardware checksums, and checks
 *       that the port's SRAM reads decode it to the values it was built from; the gate then
 *       boots the game on it and requires the file to stay unchanged (no chunk reset).
 * Every checksum here is computed the hardware's way, on the big-endian bytes, without
 * port_backup_sum. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "gdl/backup.h"
#include "include/port_backup.h"

#define SRAM_SIZE 0x8000

static int sFail, sChecks;

void port_ultra_log(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "log: ");
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

#define CHECK(cond)                                                         \
    do {                                                                    \
        sChecks++;                                                          \
        if (!(cond)) {                                                      \
            sFail++;                                                        \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                   \
    } while (0)

static u32 be32(const u8 *p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}
static void put32(u8 *p, u32 v) {
    p[0] = v >> 24, p[1] = v >> 16, p[2] = v >> 8, p[3] = v;
}
static void put16(u8 *p, u32 v) {
    p[0] = v >> 8, p[1] = v;
}

/* backup_checksum on the N64: the sum of the big-endian words, out[0] = 0xF251F205 - sum,
 * out[1] = key - (out[0] - sum); stored as two big-endian words */
static void hw_checksum(u8 *out, const u8 *src, int len, u32 key) {
    u32 sum = 0, chk;
    int i;

    for (i = 0; i + 4 <= len; i += 4) {
        sum += be32(src + i);
    }
    chk = 0xF251F205u - sum;
    put32(out, chk);
    put32(out + 4, key - (chk - sum));
}

static int hw_checksum_ok(const u8 *sum8, const u8 *src, int len, u32 key) {
    u8 c[8];

    hw_checksum(c, src, len, key);
    return memcmp(c, sum8, 8) == 0;
}

/* chunk table: primary, its sum, mirror, mirror sum, size */
static const struct {
    const char *name;
    int off, sum, mirror, mirrorSum, size;
} kChunks[4] = {
    { "course", BACKUP_SRAM_COURSE, BACKUP_SRAM_COURSE_SUM, BACKUP_SRAM_COURSE_MIRROR,
      BACKUP_SRAM_COURSE_MIRROR_SUM, sizeof(BackupCourse) },
    { "arcade", BACKUP_SRAM_ARCADE, BACKUP_SRAM_ARCADE_SUM, BACKUP_SRAM_ARCADE_MIRROR,
      BACKUP_SRAM_ARCADE_MIRROR_SUM, sizeof(BackupArcade) },
    { "options", BACKUP_SRAM_OPTIONS, BACKUP_SRAM_OPTIONS_SUM, BACKUP_SRAM_OPTIONS_MIRROR,
      BACKUP_SRAM_OPTIONS_MIRROR_SUM, sizeof(BackupOptions) },
    { "ghost", BACKUP_SRAM_GHOST, BACKUP_SRAM_GHOST_SUM, BACKUP_SRAM_GHOST_MIRROR,
      BACKUP_SRAM_GHOST_MIRROR_SUM, sizeof(BackupGhost) },
};

/* the sums + mirrors + header of an image whose primary chunks are filled in */
static void hw_seal(u8 *img) {
    u8 data[0x20];
    int c;

    for (c = 0; c < 4; c++) {
        hw_checksum(img + kChunks[c].sum, img + kChunks[c].off, kChunks[c].size, BACKUP_CHUNK_KEY);
        memcpy(img + kChunks[c].mirror, img + kChunks[c].off, kChunks[c].size);
        memcpy(img + kChunks[c].mirrorSum, img + kChunks[c].sum, 8);
        memcpy(data + c * 8, img + kChunks[c].sum, 8);
    }
    hw_checksum(img + BACKUP_SRAM_HEADER, data, 0x20, BACKUP_HEADER_KEY);
}

/* every hardware check Backup_ctor makes, on the big-endian image */
static void check_image_sealed(const u8 *img, const char *what) {
    u8 data[0x20];
    int c;

    for (c = 0; c < 4; c++) {
        int ok = hw_checksum_ok(img + kChunks[c].sum, img + kChunks[c].off, kChunks[c].size,
                                BACKUP_CHUNK_KEY);
        int okM = hw_checksum_ok(img + kChunks[c].mirrorSum, img + kChunks[c].mirror, kChunks[c].size,
                                 BACKUP_CHUNK_KEY);
        int same = memcmp(img + kChunks[c].off, img + kChunks[c].mirror, kChunks[c].size) == 0;
        if (!ok || !okM || !same) {
            fprintf(stderr, "FAIL %s: %s chunk: checksum %d, mirror checksum %d, mirror equal %d\n", what,
                    kChunks[c].name, ok, okM, same);
        }
        CHECK(ok && okM && same);
        memcpy(data + c * 8, img + kChunks[c].sum, 8);
    }
    CHECK(hw_checksum_ok(img + BACKUP_SRAM_HEADER, data, 0x20, BACKUP_HEADER_KEY));
}

/* ---------------------------------------------------------------- unit */

/* the host-side backup_checksum of the port: port_backup_sum + the same formula */
static void port_checksum(s32 *out, const void *rec, int len, s32 key) {
    s32 sum = port_backup_sum(rec, len);
    s32 chk = 0xF251F205 - sum;

    out[0] = chk;
    out[1] = key - (chk - sum);
}

static void unit(void) {
    BackupCourse crs, back;
    BackupArcade arc, arcBack;
    BackupOptions opt, optBack;
    BackupGhost gh, ghBack;
    struct InputFrameT { s8 x, y; s16 buttons; } frames[5], framesBack[5];
    u8 img[SRAM_SIZE];
    s32 sum[2];
    u8 *p;
    int i;

    memset(&crs, 0, sizeof(crs));
    crs.course[2].times[1].flags = 0x80 | (3 << 3) | 2;
    crs.course[2].times[1].name[0] = 4;
    crs.course[2].times[1].time[0] = 1;
    crs.course[2].times[1].time[1] = 23;
    crs.course[2].times[1].time[2] = 45;
    crs.course[7].scores[2].flags = 0x11;
    crs.course[7].scores[2].score = 0x01020304;
    p = port_backup_to_sram(&crs, BACKUP_SRAM_COURSE, sizeof(crs));
    memcpy(img, p, sizeof(crs));
    CHECK(img[2 * 0x30 + 8 + 0] == (0x80 | (3 << 3) | 2)); /* bytes stay put */
    CHECK(img[2 * 0x30 + 8 + 5] == 1 && img[2 * 0x30 + 8 + 7] == 45);
    CHECK(be32(img + 7 * 0x30 + 0x18 + 2 * 8 + 4) == 0x01020304); /* score big-endian */
    CHECK(img[7 * 0x30 + 0x18 + 2 * 8] == 0x11);
    CHECK(crs.course[7].scores[2].score == 0x01020304); /* the caller's record is not touched */
    memcpy(&back, img, sizeof(back));
    port_backup_from_sram(&back, BACKUP_SRAM_COURSE_MIRROR, sizeof(back));
    CHECK(memcmp(&back, &crs, sizeof(crs)) == 0);
    port_checksum(sum, &crs, sizeof(crs), BACKUP_CHUNK_KEY);
    p = port_backup_to_sram(sum, BACKUP_SRAM_COURSE_SUM, 8);
    CHECK(hw_checksum_ok(p, img, sizeof(crs), BACKUP_CHUNK_KEY));

    memset(&arc, 0, sizeof(arc));
    arc.entry[1].flags = 0x80 | 0x29;
    arc.entry[1].count = 5;
    arc.entry[1].name[2] = 7;
    for (i = 0; i < 5; i++) {
        arc.entry[1].stages[i] = 15000 + i * 1000;
    }
    arc.entry[1].score = 123456;
    p = port_backup_to_sram(&arc, BACKUP_SRAM_ARCADE, sizeof(arc));
    memcpy(img, p, sizeof(arc));
    CHECK(be32(img + 0x20) == (0x80 | 0x29)); /* flags is a word: the bit lands in byte 3 */
    CHECK(img[0x24] == 5 && img[0x27] == 7);
    CHECK(be32(img + 0x28 + 4 * 4) == 19000 && be32(img + 0x3C) == 123456);
    memcpy(&arcBack, img, sizeof(arc));
    port_backup_from_sram(&arcBack, BACKUP_SRAM_ARCADE, sizeof(arc));
    CHECK(memcmp(&arcBack, &arc, sizeof(arc)) == 0);
    port_checksum(sum, &arc, sizeof(arc), BACKUP_CHUNK_KEY);
    p = port_backup_to_sram(sum, BACKUP_SRAM_ARCADE_SUM, 8);
    CHECK(hw_checksum_ok(p, img, sizeof(arc), BACKUP_CHUNK_KEY));

    memset(&opt, 0, sizeof(opt));
    opt.unlocks = 0x1F;
    opt.seqStep = 3;
    opt.flags = 0x0109;
    opt.unk6[1] = 0xAB;
    opt.levels[2] = 6;
    p = port_backup_to_sram(&opt, BACKUP_SRAM_OPTIONS, sizeof(opt));
    memcpy(img, p, sizeof(opt));
    CHECK(img[0] == 0 && img[1] == 0x1F && img[3] == 3 && img[4] == 1 && img[5] == 9);
    CHECK(img[7] == 0xAB && img[0x14] == 6);
    memcpy(&optBack, img, sizeof(opt));
    port_backup_from_sram(&optBack, BACKUP_SRAM_OPTIONS_MIRROR, sizeof(opt));
    CHECK(memcmp(&optBack, &opt, sizeof(opt)) == 0);
    port_checksum(sum, &opt, sizeof(opt), BACKUP_CHUNK_KEY);
    p = port_backup_to_sram(sum, BACKUP_SRAM_OPTIONS_MIRROR_SUM, 8);
    CHECK(hw_checksum_ok(p, img, sizeof(opt), BACKUP_CHUNK_KEY));

    memset(&gh, 0, sizeof(gh));
    gh.bits = 1 | (2 << 2) | 0x20 | (5 << 6) | (3 << 10);
    gh.count = 5;
    p = port_backup_to_sram(&gh, BACKUP_SRAM_GHOST, sizeof(gh));
    memcpy(img, p, sizeof(gh));
    CHECK(be32(img) == (u32)gh.bits && be32(img + 4) == 5);
    memcpy(&ghBack, img, sizeof(gh));
    port_backup_from_sram(&ghBack, BACKUP_SRAM_GHOST, sizeof(gh));
    CHECK(memcmp(&ghBack, &gh, sizeof(gh)) == 0);
    port_checksum(sum, &gh, sizeof(gh), BACKUP_CHUNK_KEY);
    p = port_backup_to_sram(sum, BACKUP_SRAM_GHOST_SUM, 8);
    CHECK(hw_checksum_ok(p, img, sizeof(gh), BACKUP_CHUNK_KEY));

    for (i = 0; i < 5; i++) {
        frames[i].x = (s8)(-40 + i * 20);
        frames[i].y = (s8)(i * 7);
        frames[i].buttons = (s16)(0x1000 << (i % 3) | 0x100 * i);
    }
    p = port_backup_to_sram(frames, BACKUP_SRAM_GHOST_DATA, sizeof(frames));
    memcpy(img, p, sizeof(frames));
    CHECK((s8)img[0] == -40 && img[3 * 4 + 1] == 21);
    CHECK(((img[4 * 4 + 2] << 8) | img[4 * 4 + 3]) == (u16)frames[4].buttons); /* buttons big-endian */
    memcpy(framesBack, img, sizeof(frames));
    port_backup_from_sram(framesBack, BACKUP_SRAM_GHOST_DATA, sizeof(frames));
    CHECK(memcmp(framesBack, frames, sizeof(frames)) == 0);

    /* the header hash: the four sums as read back (host words) */
    {
        s32 sums[8];
        for (i = 0; i < 8; i++) {
            sums[i] = 0x01000000 * i + 0x3456 * i;
        }
        port_checksum(sum, sums, 0x20, BACKUP_HEADER_KEY);
        p = port_backup_to_sram(sums, BACKUP_SRAM_HEADER, 0x20); /* (only the kind matters here) */
        memcpy(img, p, 0x20);
        p = port_backup_to_sram(sum, BACKUP_SRAM_HEADER, 8);
        CHECK(hw_checksum_ok(p, img, 0x20, BACKUP_HEADER_KEY));
    }
    CHECK(port_backup_kind_at(0x1234) == PORT_BACKUP_UNKNOWN);
}

/* ---------------------------------------------------------------- the ROM defaults */

static u8 *read_file(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    u8 *b;

    if (f == NULL) {
        fprintf(stderr, "FAIL: cannot open %s\n", path);
        exit(2);
    }
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*size + 1);
    if (fread(b, 1, *size, f) != (size_t)*size) {
        fprintf(stderr, "FAIL: cannot read %s\n", path);
        exit(2);
    }
    fclose(f);
    return b;
}

/* The defaults image, built from the reset functions' logic (game_libs_tail.c) and the bootup
 * Data tables: D268 course times (8 x 3 x 3 words), D388 course scores (8 x 3 words), D3E8 arcade
 * (3 x {score, stages[5]}), D430 / D434 / D438 the "ead" initials. */
static void build_defaults(u8 *img, const u8 *data, const u8 *save) {
    int k, j, i;
    u8 *e;

    memset(img, 0, SRAM_SIZE);
    for (k = 0; k < BACKUP_NUM_COURSES; k++) {
        u8 *rec = img + BACKUP_SRAM_COURSE + k * sizeof(BackupCourseRecord);
        const u8 *times = data + 0xD268 + k * 0x24, *scores = data + 0xD388 + k * 0xC;
        i = k * 3;
        for (j = 0; j < 3; j++) {
            int flags = ((i % 5 + j) % 5) + (((i % 8 + j) % 8) << 3);
            e = rec + j * 8; /* BackupTimeEntry */
            e[0] = flags, e[1] = 0;
            e[2] = data[0xD430] - 'a', e[3] = data[0xD431] - 'a', e[4] = data[0xD432] - 'a';
            e[5] = times[j * 12 + 3], e[6] = times[j * 12 + 7], e[7] = times[j * 12 + 11];
            e = rec + 0x18 + j * 8; /* BackupScoreEntry */
            e[0] = flags;
            e[1] = data[0xD434] - 'a', e[2] = data[0xD435] - 'a', e[3] = data[0xD436] - 'a';
            memcpy(e + 4, scores + j * 4, 4);
        }
    }
    for (i = 0; i < 3; i++) {
        const u8 *src = data + 0xD3E8 + i * 0x18;
        e = img + BACKUP_SRAM_ARCADE + i * sizeof(BackupArcadeEntry);
        put32(e, (i + ((i + 4) << 3)) & 0xFF);
        e[4] = 5;
        e[5] = data[0xD438] - 'a', e[6] = data[0xD439] - 'a', e[7] = data[0xD43A] - 'a';
        memcpy(e + 8, src + 4, 20);
        memcpy(e + 0x1C, src, 4);
    }
    e = img + BACKUP_SRAM_OPTIONS;
    put16(e + 0, 0x1F);
    put16(e + 2, 1);
    put16(e + 4, 8);
    e[0x12] = e[0x13] = e[0x14] = 8;
    memcpy(e + 0x15, save + BACKUP_SRAM_OPTIONS + 0x15, 3); /* pad15: not initialized */
    e = img + BACKUP_SRAM_GHOST;
    put32(e, be32(save + BACKUP_SRAM_GHOST) & ~0x3FFFu); /* the setters clear bits 0-13 only */
    memcpy(e + 0xC, save + BACKUP_SRAM_GHOST + 0xC, 4);  /* unkC: not initialized */
    hw_seal(img);
}

static void check_defaults(const char *savePath, const char *dataPath, const char *outPath) {
    long saveSize, dataSize;
    u8 *save = read_file(savePath, &saveSize), *data = read_file(dataPath, &dataSize);
    static u8 img[SRAM_SIZE];
    int i, diffs = 0;

    CHECK(saveSize == SRAM_SIZE);
    CHECK(dataSize > 0xD440 && memcmp(data + 0xD430, "ead", 4) == 0);
    if (saveSize != SRAM_SIZE || dataSize <= 0xD440) {
        return;
    }
    check_image_sealed(save, "game-written save");
    build_defaults(img, data, save);
    for (i = 0; i < SRAM_SIZE; i++) {
        if (img[i] != save[i] && diffs++ < 8) {
            fprintf(stderr, "FAIL: save byte 0x%04X = %02X, the ROM defaults' N64 image has %02X\n", i,
                    save[i], img[i]);
        }
    }
    CHECK(diffs == 0);
    if (outPath != NULL) {
        BackupCourse crs;
        BackupArcade arc;
        BackupOptions opt;
        BackupGhost gh;
        struct { s8 x, y; s16 buttons; } fr[3];
        u8 *e;
        FILE *f;

        /* non-default records */
        e = img + BACKUP_SRAM_COURSE + 5 * sizeof(BackupCourseRecord);
        e[5] = 1, e[6] = 2, e[7] = 3;                /* course 5 best time 1'02"03 */
        put32(e + 0x18 + 4, 987654);                 /* course 5 best score */
        e = img + BACKUP_SRAM_ARCADE + 2 * sizeof(BackupArcadeEntry);
        put32(e + 0x1C, 77777);                      /* arcade rank 3 score */
        put32(e + 8 + 4 * 4, 0x00ABCDEF);            /* its last stage */
        e = img + BACKUP_SRAM_OPTIONS;
        put16(e + 0, 0x3F), put16(e + 2, 4), put16(e + 4, 0x9);
        e[0x12] = 3, e[0x13] = 10, e[0x14] = 0;
        e = img + BACKUP_SRAM_GHOST;                 /* a 3-frame time-attack ghost, course 2 */
        put32(e, 1 | (4 << 2) | (6 << 6) | (2 << 10));
        put32(e + 4, 3);
        put32(e + 8, 0);
        put32(e + 0xC, 0);
        e = img + BACKUP_SRAM_GHOST_DATA;
        e[0] = 0x14, e[1] = 0xF0, put16(e + 2, 0x1000);
        e[4] = 0x50, e[5] = 0x00, put16(e + 6, 0x0300);
        e[8] = 0xB0, e[9] = 0x7F, put16(e + 10, 0x1700);
        hw_seal(img);
        check_image_sealed(img, "synthetic image");

        /* what the port's Backup_read hands the game */
        memcpy(&crs, img + BACKUP_SRAM_COURSE, sizeof(crs));
        port_backup_from_sram(&crs, BACKUP_SRAM_COURSE, sizeof(crs));
        CHECK(crs.course[5].times[0].time[0] == 1 && crs.course[5].times[0].time[2] == 3);
        CHECK(crs.course[5].scores[0].score == 987654);
        CHECK(crs.course[0].scores[0].score == (s32)be32(data + 0xD388));
        memcpy(&arc, img + BACKUP_SRAM_ARCADE, sizeof(arc));
        port_backup_from_sram(&arc, BACKUP_SRAM_ARCADE, sizeof(arc));
        CHECK(arc.entry[2].score == 77777 && arc.entry[2].stages[4] == 0x00ABCDEF);
        CHECK(arc.entry[1].flags == 1 + (5 << 3) && arc.entry[1].count == 5);
        CHECK(arc.entry[0].score == (s32)be32(data + 0xD3E8));
        memcpy(&opt, img + BACKUP_SRAM_OPTIONS_MIRROR, sizeof(opt));
        port_backup_from_sram(&opt, BACKUP_SRAM_OPTIONS_MIRROR, sizeof(opt));
        CHECK(opt.unlocks == 0x3F && opt.seqStep == 4 && opt.flags == 9);
        CHECK(opt.levels[0] == 3 && opt.levels[1] == 10 && opt.levels[2] == 0);
        memcpy(&gh, img + BACKUP_SRAM_GHOST, sizeof(gh));
        port_backup_from_sram(&gh, BACKUP_SRAM_GHOST, sizeof(gh));
        CHECK((gh.bits & 3) == 1 && ((gh.bits >> 2) & 7) == 4 && ((gh.bits >> 6) & 0xF) == 6);
        CHECK(((gh.bits >> 10) & 0xF) == 2 && gh.count == 3);
        memcpy(fr, img + BACKUP_SRAM_GHOST_DATA, sizeof(fr));
        port_backup_from_sram(fr, BACKUP_SRAM_GHOST_DATA, gh.count * 4);
        CHECK(fr[0].x == 0x14 && fr[0].y == -16 && fr[0].buttons == 0x1000);
        CHECK(fr[1].x == 0x50 && fr[1].buttons == 0x0300 && fr[2].x == -80 && fr[2].buttons == 0x1700);

        f = fopen(outPath, "wb");
        CHECK(f != NULL && fwrite(img, 1, SRAM_SIZE, f) == SRAM_SIZE);
        if (f != NULL) {
            fclose(f);
        }
    }
    free(save);
    free(data);
}

int main(int argc, char **argv) {
    unit();
    if (argc >= 3) {
        check_defaults(argv[1], argv[2], argc >= 4 ? argv[3] : NULL);
    } else if (argc != 1) {
        fprintf(stderr, "usage: test_backup [<save> <bootup Data> [<out.sra>]]\n");
        return 2;
    }
    printf("test_backup: %d/%d checks passed%s\n", sChecks - sFail, sChecks,
           argc >= 3 ? " (unit + game save vs the ROM defaults' N64 image)" : " (unit)");
    return sFail != 0;
}
