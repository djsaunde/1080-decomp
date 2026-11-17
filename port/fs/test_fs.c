/* port/fs self-test against the user's ROM (host build, any word size):
 *   cc -Iport -o test_fs port/fs/test_fs.c port/fs/fs.c && ./test_fs baserom.z64
 * Values are those of the US 1.0 ROM (sha1 79cd1166...); extract.py's manifest gives the same
 * offsets for all 131 files (plan 17.8). */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "fs/port_fs.h"

static int sFail, sChecks;

void port_ultra_log(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "log: ");
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

#define CHECK(cond)                                                       \
    do {                                                                  \
        sChecks++;                                                        \
        if (!(cond)) {                                                    \
            sFail++;                                                      \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                 \
    } while (0)

int main(int argc, char **argv)
{
    FILE *fp;
    long n;
    unsigned char *rom;
    unsigned long long buf[0x10];
    PortFsFile *f;
    int addr = 0;

    if (argc < 2 || (fp = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "usage: test_fs baserom.z64\n");
        return 2;
    }
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    rom = malloc(n);
    if (rom == NULL || fread(rom, 1, n, fp) != (size_t)n) {
        return 2;
    }
    fclose(fp);

    CHECK(port_fs_init(rom, (unsigned int)n) == 131);
    CHECK(port_fs_device_count() == 2);

    /* the directory: name -> device offset just past the Info record */
    CHECK(port_fs_lookup("mainuso/bootup.uso", NULL) != NULL);
    CHECK(port_fs_lookup("mainuso/bootup.uso", NULL)->offset == 0x7C);
    CHECK(port_fs_lookup("rom/game.uso", NULL)->id == 46);
    CHECK(port_fs_lookup("nodev/game.uso", NULL) == NULL);
    CHECK(port_fs_lookup("rom/nofile.uso", NULL) == NULL);

    /* fileInfo: the ROM address of the Data payload (bootup: Bss, RoData 0x25A0, Data) */
    CHECK(port_fs_info("mainuso/bootup.uso", &addr) == 0 && addr == 0xDA23F0);
    CHECK(port_fs_info("rom/audio_bank.uso", &addr) == 0 && addr == 0xB848);
    CHECK(port_fs_info("rom/nofile.uso", &addr) == PORT_FS_ERR_NOT_FOUND);
    CHECK(port_fs_info("rom/version.bin", &addr) == PORT_FS_ERR_NO_DATA); /* no Data section */

    /* fileOpen: a window on the first record's payload (a .bin's BinaryDataSection) */
    f = port_fs_open("rom/cam_info.bin");
    CHECK(f != NULL);
    if (f != NULL) {
        int start = port_fs_tell(f);

        CHECK(start == 0xD9DB12);
        CHECK(f->limit - f->window == 0x2280);
        CHECK(port_fs_read(f, buf, 16) == 16);
        CHECK(((unsigned char *)buf)[3] == rom[start + 3]);
        CHECK(port_fs_tell(f) == start + 16);
        /* any alignment is accepted on the port (plan 18.9) */
        CHECK(port_fs_read(f, (char *)buf + 1, 4) == 4);
        CHECK(((unsigned char *)buf)[1] == rom[start + 16]);
        port_fs_seek(f, 1, -4);
        /* clamp at the end of the window */
        port_fs_seek(f, 1, 0x2280 - 16 - 8);
        CHECK(port_fs_read(f, buf, 64) == 8);
        CHECK(port_fs_read(f, buf, 64) == 0);
        /* whence 0 sets the device offset */
        CHECK(port_fs_seek(f, 0, (int)f->window) == (int)f->window);
        CHECK(port_fs_tell(f) == start);
        port_fs_close(f);
    }
    CHECK(port_fs_open("rom/nofile.bin") == NULL && port_fs_errno() == PORT_FS_ERR_NOT_FOUND);

    /* a closed handle stays valid and is never reused (fs_closeFile is a no-op; plan 19.10):
     * opens after the close get other handles, and the closed one still reads its own file */
    f = port_fs_open("rom/cam_info.bin");
    CHECK(f != NULL);
    if (f != NULL) {
        int start = port_fs_tell(f), k;
        PortFsFile *g = NULL;

        port_fs_close(f);
        for (k = 0; k < 80; k++) { /* past the static pool */
            g = port_fs_open("rom/version.bin");
            CHECK(g != NULL && g != f);
            port_fs_close(g);
        }
        CHECK(port_fs_tell(f) == start);
        CHECK(port_fs_read(f, buf, 16) == 16 && ((unsigned char *)buf)[3] == rom[start + 3]);
    }

    printf("test_fs: %d checks, %d failures\n", sChecks, sFail);
    free(rom);
    return sFail != 0;
}
