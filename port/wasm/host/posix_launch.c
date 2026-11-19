/* port/wasm/host/posix_launch.c -- the double-click start-up of the wasm2c build (plan section 30), the macOS
 * counterpart of port/win32/win32_host.c.
 *
 * Started with arguments, nothing changes: the Linux behaviour (--rom, else baserom.z64 in the current directory;
 * 1080.sram / 1080.ini there; headless unless PORT_NATIVE=1). Started with none (Finder, `open 1080.app`), the
 * game plays in its window in real time, and:
 *   - the save, 1080.ini and 1080.log live in the data directory: ~/Library/Application Support/1080-pc on macOS,
 *     $XDG_DATA_HOME/1080-pc (~/.local/share/1080-pc) elsewhere; it becomes the current directory;
 *   - the ROM (no ROM bytes ship with the app) is the path remembered in rom-path.txt there, else baserom.z64 in
 *     the data directory or next to the app (the folder holding 1080.app, or the executable's own folder), else
 *     the only *.z64 / *.n64 / *.v64 in one of those, else a file dialog (macOS: osascript's `choose file`); the
 *     choice is remembered in rom-path.txt;
 *   - stdout / stderr go to 1080.log when they are not a terminal, and a fatal exit shows an alert (macOS). */
#define _DARWIN_C_SOURCE 1
#define _GNU_SOURCE 1
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include "posix_launch.h"

static char sData[PATH_MAX];
static char sRom[PATH_MAX];
static int sGui;

static int file_exists(const char *p) {
    struct stat st;
    return stat(p, &st) == 0 && S_ISREG(st.st_mode);
}

static void mkdir_p(const char *dir) {
    char tmp[PATH_MAX];
    size_t i;
    snprintf(tmp, sizeof(tmp), "%s", dir);
    for (i = 1; tmp[i] != '\0'; i++) {
        if (tmp[i] == '/') {
            tmp[i] = '\0';
            mkdir(tmp, 0755);
            tmp[i] = '/';
        }
    }
    mkdir(tmp, 0755);
}

/* the executable's folder; for .../X.app/Contents/MacOS/exe, the folder holding X.app too */
static void exe_dirs(char *exeDir, char *appDir, size_t n) {
    char raw[PATH_MAX], real[PATH_MAX];
    char *s;
    exeDir[0] = appDir[0] = '\0';
#ifdef __APPLE__
    {
        uint32_t len = sizeof(raw);
        if (_NSGetExecutablePath(raw, &len) != 0) {
            return;
        }
    }
#else
    {
        ssize_t len = readlink("/proc/self/exe", raw, sizeof(raw) - 1);
        if (len <= 0) {
            return;
        }
        raw[len] = '\0';
    }
#endif
    if (realpath(raw, real) == NULL) {
        snprintf(real, sizeof(real), "%s", raw);
    }
    s = strrchr(real, '/');
    if (s == NULL) {
        return;
    }
    *s = '\0';
    snprintf(exeDir, n, "%s", real);
    s = strstr(real, ".app/Contents/MacOS");
    if (s != NULL) {
        *s = '\0';
        s = strrchr(real, '/');
        if (s != NULL) {
            *s = '\0';
            snprintf(appDir, n, "%s", real);
        }
    }
}

static int one_rom_in(const char *dir, char *out, size_t n) {
    DIR *d = opendir(dir);
    struct dirent *e;
    int found = 0;
    if (d == NULL) {
        return 0;
    }
    while ((e = readdir(d)) != NULL) {
        size_t l = strlen(e->d_name);
        const char *x = l > 4 ? e->d_name + l - 4 : "";
        if (strcasecmp(x, ".z64") == 0 || strcasecmp(x, ".n64") == 0 || strcasecmp(x, ".v64") == 0) {
            if (found++ == 0) {
                snprintf(out, n, "%s/%s", dir, e->d_name);
            }
        }
    }
    closedir(d);
    return found == 1;
}

static int remembered(char *out, size_t n) {
    char p[PATH_MAX];
    FILE *f;
    snprintf(p, sizeof(p), "%s/rom-path.txt", sData);
    f = fopen(p, "r");
    if (f == NULL) {
        return 0;
    }
    if (fgets(out, (int)n, f) == NULL) {
        out[0] = '\0';
    }
    fclose(f);
    out[strcspn(out, "\r\n")] = '\0';
    return out[0] != '\0' && file_exists(out);
}

static void remember(const char *rom) {
    char p[PATH_MAX];
    FILE *f;
    snprintf(p, sizeof(p), "%s/rom-path.txt", sData);
    f = fopen(p, "w");
    if (f != NULL) {
        fprintf(f, "%s\n", rom);
        fclose(f);
    }
}

static int dialog(char *out, size_t n) {
#ifdef __APPLE__
    FILE *p = popen("/usr/bin/osascript -e 'POSIX path of (choose file with prompt "
                    "\"1080 Snowboarding: choose your ROM (US 1.0, .z64 / .n64 / .v64)\")' 2>/dev/null", "r");
    int ok;
    if (p == NULL) {
        return 0;
    }
    ok = fgets(out, (int)n, p) != NULL;
    pclose(p);
    out[strcspn(out, "\r\n")] = '\0';
    return ok && file_exists(out);
#else
    (void)out, (void)n;
    return 0;
#endif
}

void port_posix_alert(const char *text) {
#ifdef __APPLE__
    if (sGui) {
        char cmd[2048], esc[1024];
        size_t i, j = 0;
        for (i = 0; text[i] != '\0' && j + 2 < sizeof(esc); i++) {
            if (text[i] == '"' || text[i] == '\\' || text[i] == '\'') {
                continue;
            }
            esc[j++] = text[i];
        }
        esc[j] = '\0';
        snprintf(cmd, sizeof(cmd), "/usr/bin/osascript -e 'display alert \"1080 Snowboarding\" message \"%s\"' "
                 ">/dev/null 2>&1", esc);
        if (system(cmd) != 0) {
            /* nothing more to do */
        }
    }
#else
    (void)text;
#endif
}

int port_posix_gui(void) {
    return sGui;
}

const char *port_posix_data_dir(void) {
    return sData;
}

/* argv for the game: the caller's, or (no arguments) --rom <found> --realtime */
char **port_posix_launch(int argc, char **argv, int *argcOut) {
    char exeDir[PATH_MAX], appDir[PATH_MAX], logp[PATH_MAX];
    const char *home = getenv("HOME");
    static char *args[8];
    int n = 0;

    *argcOut = argc;
    if (argc > 1) {
        return argv;
    }
    sGui = 1;
#ifdef __APPLE__
    snprintf(sData, sizeof(sData), "%s/Library/Application Support/1080-pc", home != NULL ? home : "/tmp");
#else
    if (getenv("XDG_DATA_HOME") != NULL && getenv("XDG_DATA_HOME")[0] != '\0') {
        snprintf(sData, sizeof(sData), "%s/1080-pc", getenv("XDG_DATA_HOME"));
    } else {
        snprintf(sData, sizeof(sData), "%s/.local/share/1080-pc", home != NULL ? home : "/tmp");
    }
#endif
    mkdir_p(sData);
    if (chdir(sData) != 0) {
        fprintf(stderr, "PORT: cannot use %s: %s\n", sData, strerror(errno));
    }
    if (!isatty(2)) {
        snprintf(logp, sizeof(logp), "%s/1080.log", sData);
        if (freopen(logp, "w", stderr) != NULL) {
            dup2(2, 1);
            setvbuf(stderr, NULL, _IONBF, 0);
        }
    }
    exe_dirs(exeDir, appDir, sizeof(exeDir));
    {
        char p[PATH_MAX];
        const char *dirs[3];
        int i, nd = 0;
        dirs[nd++] = sData;
        if (appDir[0] != '\0') {
            dirs[nd++] = appDir;
        }
        if (exeDir[0] != '\0') {
            dirs[nd++] = exeDir;
        }
        sRom[0] = '\0';
        if (!remembered(sRom, sizeof(sRom))) {
            sRom[0] = '\0';
            for (i = 0; i < nd && sRom[0] == '\0'; i++) {
                snprintf(p, sizeof(p), "%s/baserom.z64", dirs[i]);
                if (file_exists(p)) {
                    snprintf(sRom, sizeof(sRom), "%s", p);
                }
            }
            for (i = 0; i < nd && sRom[0] == '\0'; i++) {
                if (!one_rom_in(dirs[i], sRom, sizeof(sRom))) {
                    sRom[0] = '\0';
                }
            }
            if (sRom[0] == '\0' && dialog(sRom, sizeof(sRom))) {
                remember(sRom);
            }
        }
    }
    if (sRom[0] == '\0') {
        fprintf(stderr, "PORT: no ROM: put baserom.z64 in %s (or next to the app), or start with --rom PATH\n", sData);
        port_posix_alert("No ROM found. Put your 1080 Snowboarding ROM (US 1.0) named baserom.z64 in "
                         "~/Library/Application Support/1080-pc or next to the app, and open it again.");
        exit(1);
    }
    fprintf(stderr, "PORT: ROM %s, data in %s\n", sRom, sData);
    if (getenv("PORT_NATIVE") == NULL) {
        setenv("PORT_NATIVE", "1", 1); /* the in-process window (port_live.c) */
    }
    args[n++] = argv[0];
    args[n++] = (char *)"--rom";
    args[n++] = sRom;
    args[n++] = (char *)"--realtime";
    args[n] = NULL;
    *argcOut = n;
    return args;
}
