/* port/win32: the Windows build's start-up (plan section 27): where the ROM and the save are, the log when
 * there is no console, the 1 ms timer period, and a message box for a fatal error. main.c calls
 * port_win32_setup before it reads the ROM.
 *
 * The ROM (no ROM data ships with the executable): --rom PATH, else baserom.z64 next to the exe, else
 * baserom.z64 in the current directory, else the only *.z64 / *.n64 / *.v64 next to the exe, else a file
 * dialog. The path picked in the dialog is remembered in rom-path.txt next to the save.
 * The save: --sram PATH; started with no arguments (a double-click), 1080.sram next to the exe when that folder
 * is writable, else %APPDATA%\1080-pc\1080.sram; with arguments, 1080.sram in the current directory, as on
 * Linux (the test scripts run each case in its own directory). Started with no arguments (a double-click) the game runs in real time in its
 * window; --headless runs without one (the Linux build's default; tests). */
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "win32_host.h"

static char sExeDir[MAX_PATH];
static char sRom[MAX_PATH];
static char sSram[MAX_PATH];
static char sMemo[MAX_PATH];
static int sConsole; /* stdout / stderr reach a console or a pipe */

static int file_exists(const char *p) {
    DWORD a = GetFileAttributesA(p);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

static int dir_writable(const char *dir) {
    char p[MAX_PATH];
    HANDLE h;
    snprintf(p, sizeof(p), "%s\\.1080-write-test", dir);
    h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        return 0;
    }
    CloseHandle(h);
    return 1;
}

static int has_handle(DWORD which) {
    HANDLE h = GetStdHandle(which);
    return h != NULL && h != INVALID_HANDLE_VALUE && GetFileType(h) != FILE_TYPE_UNKNOWN;
}

static int one_rom_in(const char *dir, char *out, size_t outSize) {
    static const char *pats[] = { "*.z64", "*.n64", "*.v64" };
    char pat[MAX_PATH], found[MAX_PATH] = "";
    int n = 0, i;
    for (i = 0; i < 3; i++) {
        WIN32_FIND_DATAA fd;
        HANDLE h;
        snprintf(pat, sizeof(pat), "%s\\%s", dir, pats[i]);
        h = FindFirstFileA(pat, &fd);
        if (h == INVALID_HANDLE_VALUE) {
            continue;
        }
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                snprintf(found, sizeof(found), "%s\\%s", dir, fd.cFileName);
                n++;
            }
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    if (n == 1) {
        snprintf(out, outSize, "%s", found);
        return 1;
    }
    return 0;
}

static int remembered_rom(char *out, size_t outSize) {
    FILE *f = fopen(sMemo, "r");
    char line[MAX_PATH];
    int ok = 0;
    if (f == NULL) {
        return 0;
    }
    if (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (file_exists(line)) {
            snprintf(out, outSize, "%s", line);
            ok = 1;
        }
    }
    fclose(f);
    return ok;
}

static int ask_rom(char *out, size_t outSize) {
    OPENFILENAMEA ofn;
    char file[MAX_PATH] = "";
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "N64 ROM (*.z64;*.n64;*.v64)\0*.z64;*.n64;*.v64\0All files\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = sizeof(file);
    ofn.lpstrTitle = "1080 Snowboarding: choose your US 1.0 ROM (baserom.z64)";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    if (!GetOpenFileNameA(&ofn)) {
        return 0;
    }
    snprintf(out, outSize, "%s", file);
    return 1;
}

/* a crash: the exception, eip and the stack words that point into the executable's code (no frame pointers at
 * -O2: a heuristic backtrace), for addr2line -e tenshoe.exe; the Linux build's SIGSEGV report (main.c) */
static LONG WINAPI crash_filter(EXCEPTION_POINTERS *ep) {
    const EXCEPTION_RECORD *er = ep->ExceptionRecord;
    const CONTEXT *c = ep->ContextRecord;
    const unsigned char *base = (const unsigned char *)GetModuleHandleA(NULL);
    const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)(base + ((const IMAGE_DOS_HEADER *)base)->e_lfanew);
    DWORD lo = (DWORD)(size_t)base, hi = lo + nt->OptionalHeader.SizeOfImage;
    const DWORD *sp = (const DWORD *)(size_t)c->Esp;
    int i, n = 0;
    char msg[256];
    fprintf(stderr, "PORT: exception 0x%08lx at eip 0x%08lx (address 0x%08lx), esp 0x%08lx\n",
            (unsigned long)er->ExceptionCode, (unsigned long)c->Eip,
            (unsigned long)(er->NumberParameters >= 2 ? er->ExceptionInformation[1] : 0), (unsigned long)c->Esp);
    fprintf(stderr, "PORT: stack code addresses:");
    for (i = 0; i < 4096 && n < 32; i++) {
        MEMORY_BASIC_INFORMATION mi;
        if (((size_t)(sp + i) & 0xFFF) == 0 && (VirtualQuery(sp + i, &mi, sizeof(mi)) != sizeof(mi) || mi.State != MEM_COMMIT)) {
            break;
        }
        if (sp[i] >= lo && sp[i] < hi) {
            fprintf(stderr, " 0x%08lx", (unsigned long)sp[i]);
            n++;
        }
    }
    fprintf(stderr, "\n");
    fflush(stderr);
    snprintf(msg, sizeof(msg), "The game crashed (exception 0x%08lx at 0x%08lx). Details are in 1080.log.",
             (unsigned long)er->ExceptionCode, (unsigned long)c->Eip);
    port_win32_message("1080 Snowboarding", msg);
    return EXCEPTION_EXECUTE_HANDLER;
}

void port_win32_message(const char *title, const char *text) {
    if (!sConsole) {
        MessageBoxA(NULL, text, title, MB_OK | MB_ICONERROR);
    }
}

void port_win32_setup(int argc, char **argv, const char **romPath, int romGiven, const char **sramPath,
                      int sramGiven, int *realtime) {
    char *slash;
    int headless = 0, i;

    timeBeginPeriod(1); /* Sleep / the pacing loop at 1 ms instead of 15.6 ms */
    SetUnhandledExceptionFilter(crash_filter);
    GetModuleFileNameA(NULL, sExeDir, sizeof(sExeDir));
    slash = strrchr(sExeDir, '\\');
    if (slash != NULL) {
        *slash = '\0';
    }
    /* a double-clicked GUI program has no stdout: the log goes next to the save */
    sConsole = has_handle(STD_OUTPUT_HANDLE) || has_handle(STD_ERROR_HANDLE);
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--headless") == 0) {
            headless = 1;
        }
    }
    if (!sramGiven && argc == 1) {
        if (dir_writable(sExeDir)) {
            snprintf(sSram, sizeof(sSram), "%s\\1080.sram", sExeDir);
        } else {
            const char *app = getenv("APPDATA");
            char dir[MAX_PATH];
            snprintf(dir, sizeof(dir), "%s\\1080-pc", app != NULL ? app : ".");
            CreateDirectoryA(dir, NULL);
            snprintf(sSram, sizeof(sSram), "%s\\1080.sram", dir);
        }
        *sramPath = sSram;
    }
    snprintf(sMemo, sizeof(sMemo), "%s", *sramPath);
    slash = strrchr(sMemo, '\\');
    if (slash == NULL) {
        slash = strrchr(sMemo, '/');
    }
    snprintf(slash != NULL ? slash + 1 : sMemo, sizeof(sMemo) - (size_t)(slash != NULL ? slash + 1 - sMemo : 0),
             "%s", "rom-path.txt");
    if (getenv("PORT_SETTINGS") == NULL) {
        /* the settings file (port/settings.h, 1080.ini) next to the save: the exe's folder after a double-click */
        char ini[MAX_PATH + 16], *sl;
        snprintf(ini, sizeof(ini), "PORT_SETTINGS=%s", sMemo);
        sl = strrchr(ini, '\\');
        if (sl == NULL) {
            sl = strrchr(ini, '=');
        }
        snprintf(sl + 1, sizeof(ini) - (size_t)(sl + 1 - ini), "%s", "1080.ini");
        _putenv(ini);
    }
    if (!sConsole) {
        char log[MAX_PATH];
        snprintf(log, sizeof(log), "%s", sMemo);
        slash = strrchr(log, '\\');
        snprintf(slash != NULL ? slash + 1 : log, sizeof(log) - (size_t)(slash != NULL ? slash + 1 - log : 0), "%s",
                 "1080.log");
        freopen(log, "w", stdout);
        freopen(log, "a", stderr);
        setvbuf(stderr, NULL, _IONBF, 0);
    }
    if (!romGiven) {
        char p[MAX_PATH];
        snprintf(p, sizeof(p), "%s\\baserom.z64", sExeDir);
        if (file_exists(p)) {
            snprintf(sRom, sizeof(sRom), "%s", p);
        } else if (file_exists("baserom.z64")) {
            snprintf(sRom, sizeof(sRom), "baserom.z64");
        } else if (!one_rom_in(sExeDir, sRom, sizeof(sRom)) && !remembered_rom(sRom, sizeof(sRom))) {
            if (!ask_rom(sRom, sizeof(sRom))) {
                port_win32_message("1080 Snowboarding",
                                   "No ROM chosen.\n\nPut your own 1080 Snowboarding (US 1.0) ROM next to the program as "
                                   "baserom.z64, or start it with --rom PATH.");
                exit(2);
            }
            {
                FILE *f = fopen(sMemo, "w");
                if (f != NULL) {
                    fprintf(f, "%s\n", sRom);
                    fclose(f);
                }
            }
        }
        *romPath = sRom;
    }
    if (!headless && getenv("PORT_NATIVE") == NULL) {
        _putenv("PORT_NATIVE=1"); /* the in-process window (port_live.c) */
    }
    if (argc == 1) {
        *realtime = 1; /* a double-click: play */
    }
    printf("win32: ROM %s, save %s%s\n", *romPath, *sramPath, headless ? ", headless" : "");
    fflush(stdout);
}
