/* port/android/android_main.c -- SDL_main of the Android app (plan section 31).
 *
 * SDLActivity (dev.djsaunde.tenshoe.TenshoeActivity) loads libSDL2.so and libmain.so and calls SDL_main on its SDL
 * thread. This:
 *   - makes the app's internal files directory (/data/user/0/<id>/files) the current directory: 1080.sram, 1080.ini,
 *     1080.log live there;
 *   - sends stdout / stderr to 1080.log and to logcat (tag "tenshoe");
 *   - finds the ROM: baserom.z64 (or the only *.z64 / *.n64 / *.v64) in the external files directory
 *     (Android/data/<id>/files: adb push / a file manager), else baserom.z64 in the internal one; else it asks the
 *     activity for the file picker (requestRom: ACTION_OPEN_DOCUMENT, the choice copied to files/baserom.z64) and
 *     waits. A file that is no 1080 Snowboarding ROM is deleted (internal copy) and the picker asked for again;
 *   - runs the game (port/wasm/host/wasm_main.c's main as port_wasm_main) with --rom <path> --realtime and the
 *     in-process window (PORT_NATIVE=1), on a thread with a 64 MB stack (the SDL thread's is the Java default).
 * A fatal exit shows a message box naming the log. */
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <jni.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#include <android/log.h>

#include "SDL.h"

#include "posix_launch.h"

int port_wasm_main(int argc, char **argv);

static char sFiles[1024], sExt[1024], sRom[1100];

/* ---------------- the log: 1080.log + logcat ---------------- */

static int sLogPipe[2] = { -1, -1 };
static FILE *sLogFile;

static void *log_thread(void *arg) {
    char buf[4096], line[4096];
    size_t n = 0;
    ssize_t r;
    (void)arg;
    while ((r = read(sLogPipe[0], buf, sizeof(buf))) > 0) {
        ssize_t i;
        if (sLogFile != NULL) {
            fwrite(buf, 1, (size_t)r, sLogFile);
            fflush(sLogFile);
        }
        for (i = 0; i < r; i++) {
            if (buf[i] == '\n' || n + 1 >= sizeof(line)) {
                line[n] = '\0';
                __android_log_write(ANDROID_LOG_INFO, "tenshoe", line);
                n = 0;
                if (buf[i] == '\n') {
                    continue;
                }
            }
            line[n++] = buf[i];
        }
    }
    return NULL;
}

static void log_start(void) {
    pthread_t t;
    char path[1100];
    snprintf(path, sizeof(path), "%s/1080.log", sFiles);
    sLogFile = fopen(path, "w");
    if (pipe(sLogPipe) != 0) {
        return;
    }
    dup2(sLogPipe[1], 1);
    dup2(sLogPipe[1], 2);
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    pthread_create(&t, NULL, log_thread, NULL);
    pthread_detach(t);
}

/* ---------------- the ROM ---------------- */

static int file_exists(const char *p) {
    struct stat st;
    return stat(p, &st) == 0 && S_ISREG(st.st_mode) && st.st_size > 0;
}

/* a 1080 Snowboarding ROM in any of the three byte orders (main.c accepts .z64 / .v64 / .n64): the header's
 * magic and "1080" at the start of the internal name (0x20) */
static int rom_ok(const char *p) {
    unsigned char h[0x40], z[0x40];
    FILE *f = fopen(p, "rb");
    size_t n, i;
    if (f == NULL) {
        return 0;
    }
    n = fread(h, 1, sizeof(h), f);
    fclose(f);
    if (n != sizeof(h)) {
        return 0;
    }
    for (i = 0; i < sizeof(h); i++) {
        if (h[0] == 0x80 && h[1] == 0x37) {
            z[i] = h[i]; /* z64: big-endian */
        } else if (h[0] == 0x37 && h[1] == 0x80) {
            z[i] = h[i ^ 1]; /* v64: 16-bit swapped */
        } else if (h[0] == 0x40 && h[1] == 0x12) {
            z[i] = h[i ^ 3]; /* n64: 32-bit little-endian */
        } else {
            return 0;
        }
    }
    return z[0] == 0x80 && z[1] == 0x37 && z[2] == 0x12 && z[3] == 0x40 && memcmp(z + 0x20, "1080", 4) == 0;
}

static int one_rom_in(const char *dir, char *out, size_t size) {
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
                snprintf(out, size, "%s/%s", dir, e->d_name);
            }
        }
    }
    closedir(d);
    return found == 1;
}

/* TenshoeActivity.requestRom(message): the dialog, then the picker (on the UI thread) */
static void request_rom(const char *message) {
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv();
    jobject activity = (jobject)SDL_AndroidGetActivity();
    jclass cls;
    jmethodID mid;
    jstring msg;
    if (env == NULL || activity == NULL) {
        return;
    }
    cls = (*env)->GetObjectClass(env, activity);
    mid = (*env)->GetMethodID(env, cls, "requestRom", "(Ljava/lang/String;)V");
    if (mid != NULL) {
        msg = (*env)->NewStringUTF(env, message);
        (*env)->CallVoidMethod(env, activity, mid, msg);
        (*env)->DeleteLocalRef(env, msg);
    }
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
    }
    (*env)->DeleteLocalRef(env, cls);
    (*env)->DeleteLocalRef(env, activity);
}

/* the ROM's path in sRom, or 0 (quit) */
static int find_rom(void) {
    char internal[1100];
    const char *why = "Choose your 1080 Snowboarding ROM (US 1.0; .z64, .v64 or .n64). It is copied into the app.";
    snprintf(internal, sizeof(internal), "%s/baserom.z64", sFiles);
    for (;;) {
        if (sExt[0] != '\0') {
            snprintf(sRom, sizeof(sRom), "%s/baserom.z64", sExt);
            if ((file_exists(sRom) || one_rom_in(sExt, sRom, sizeof(sRom))) && rom_ok(sRom)) {
                return 1;
            }
        }
        if (file_exists(internal)) {
            if (rom_ok(internal)) {
                snprintf(sRom, sizeof(sRom), "%s", internal);
                return 1;
            }
            fprintf(stderr, "PORT: %s is not a 1080 Snowboarding ROM: deleted\n", internal);
            unlink(internal);
            why = "That file is not a 1080 Snowboarding ROM. Choose the ROM (US 1.0; .z64, .v64 or .n64).";
        }
        fprintf(stderr, "PORT: no ROM in %s or %s: asking for one\n", sExt[0] ? sExt : "(no external dir)", sFiles);
        request_rom(why);
        /* wait for the copy (TenshoeActivity writes baserom.z64.part, then renames it) or a quit */
        while (!file_exists(internal)) {
            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT || ev.type == SDL_APP_TERMINATING) {
                    return 0;
                }
            }
            SDL_Delay(100);
        }
    }
}

/* ---------------- the game ---------------- */

static void alert(const char *text) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "1080 Snowboarding", text, NULL);
}

static void *game_thread(void *arg) {
    static char *args[6];
    int n = 0;
    (void)arg;
    args[n++] = (char *)"tenshoe";
    args[n++] = (char *)"--rom";
    args[n++] = sRom;
    args[n++] = (char *)"--realtime";
    args[n] = NULL;
    return (void *)(intptr_t)port_wasm_main(n, args);
}

int SDL_main(int argc, char *argv[]) {
    const char *p;
    pthread_attr_t attr;
    pthread_t t;
    void *rc = NULL;
    (void)argc, (void)argv;
    SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1"); /* Back: the settings menu (live_window_sdl.cpp) */

    p = SDL_AndroidGetInternalStoragePath();
    snprintf(sFiles, sizeof(sFiles), "%s", p != NULL ? p : ".");
    mkdir(sFiles, 0700);
    p = (SDL_AndroidGetExternalStorageState() & SDL_ANDROID_EXTERNAL_STORAGE_READ) ? SDL_AndroidGetExternalStoragePath()
                                                                                   : NULL;
    snprintf(sExt, sizeof(sExt), "%s", p != NULL ? p : "");
    if (chdir(sFiles) != 0) {
        __android_log_print(ANDROID_LOG_ERROR, "tenshoe", "cannot use %s: %s", sFiles, strerror(errno));
    }
    log_start();
    fprintf(stderr, "PORT: Android, data in %s, ROM folder %s\n", sFiles, sExt[0] ? sExt : "(none)");
    SDL_Init(SDL_INIT_EVENTS);
    if (!find_rom()) {
        fprintf(stderr, "PORT: no ROM; quitting\n");
        return 0;
    }
    fprintf(stderr, "PORT: ROM %s\n", sRom);
    setenv("PORT_NATIVE", "1", 1); /* the in-process window (port_live.c) */
    port_posix_set_gui(sFiles, alert);
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 64u << 20);
    if (pthread_create(&t, &attr, game_thread, NULL) != 0) {
        game_thread(NULL);
    } else {
        pthread_join(t, &rc);
    }
    return (int)(intptr_t)rc;
}
