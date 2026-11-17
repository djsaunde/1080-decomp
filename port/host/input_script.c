/* Scripted controller input for the headless host (--input FILE; PC port plan 9.4 / 17,
 * M3 menu / race tests).
 *
 * The script is a text file of step-timed controller states. host_headless.c's
 * host_read_pads() asks it for the pads once per simulation step (main.c's input_poll),
 * so the states go through the same path as live input: HostPad -> port_si_set_pads ->
 * osContStartReadData snapshot -> the pad thread's osContGetReadData -> pad_update.
 *
 * Format (one directive per line; '#' starts a comment; keywords are case-insensitive):
 *
 *   PADS <n>                  controllers 1..n are plugged in from boot (default 1; a pad
 *                             named by any F line is plugged in too). osContInit reads
 *                             this at boot, so it is not timed.
 *   RUMBLE <pad>              report a rumble pak in that controller.
 *   F <when> [P<n>] <state>   hold <state> on controller n (default 1) during <when>.
 *                             More P<n> groups may follow on the same line.
 *
 *   <when>   N        step N only
 *            N-M      steps N..M inclusive
 *            N:L      L steps from N (N..N+L-1)
 *            +D ...   relative: N = the previous F line's first step + D; also +D:L,
 *                     and +D-+E (both ends relative)
 *   <state>  buttons: A B Z START L R DU DD DL DR (the D-pad; also UP DOWN LEFT RIGHT)
 *                     CU CD CL CR, or a raw OSContPad button word 0xNNNN;
 *            STICK x,y   raw stick bytes (N64 range about -80..80; clamped to -128..127).
 *
 * A step is main.c's step count, which equals SysLib.frameCount (the F lines of
 * --trace state.log and the I lines of --record), so a state.log line can be turned
 * into a script time directly. Steps no line covers are neutral (no buttons, centred
 * stick). Overlapping lines OR their buttons; the last line's STICK wins.
 *
 * Timing: the pad thread snapshots the SI during the step's two virtual retraces,
 * after the update ran, so a state held on step N is first seen by pad_update on
 * step N+1. A 1-step press gives exactly one `pressed` edge; hold 2-3 steps when a
 * menu debounces. A press and the next press of the same button need a released
 * step in between.
 *
 * Example:
 *   PADS 1
 *   F 700:3 START          # title: Press START
 *   F +40:3 DD             # menu cursor down
 *   F +30:3 A
 *   F 900-1200 STICK 0,80 A
 */

#include "port_libc.h"
#include "host.h"

typedef struct ScriptEvent {
    host_u32 first;
    host_u32 last;
    host_u16 buttons;
    host_u8 pad;
    host_u8 hasStick;
    host_s8 stickX;
    host_s8 stickY;
} ScriptEvent;

static ScriptEvent *sEvents;
static host_u32 sCount;
static host_u32 sCap;
static host_u32 sStep;
static host_u32 sLastStep;
static int sNumPads = 1;
static host_u8 sRumble[HOST_MAX_PADS];
static int sLoaded;

static const struct {
    const char *name;
    host_u16 bit;
} kButtons[] = {
    { "A", 0x8000 },     { "B", 0x4000 },    { "Z", 0x2000 },     { "START", 0x1000 },
    { "DU", 0x0800 },    { "DD", 0x0400 },   { "DL", 0x0200 },    { "DR", 0x0100 },
    { "UP", 0x0800 },    { "DOWN", 0x0400 }, { "LEFT", 0x0200 },  { "RIGHT", 0x0100 },
    { "L", 0x0020 },     { "R", 0x0010 },    { "CU", 0x0008 },    { "CD", 0x0004 },
    { "CL", 0x0002 },    { "CR", 0x0001 },
};

static int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static char up(char c) {
    return (c >= 'a' && c <= 'z') ? (char)(c - 'a' + 'A') : c;
}

static int tok_eq(const char *tok, const char *name) {
    while (*tok != '\0' && *name != '\0' && up(*tok) == *name) {
        tok++;
        name++;
    }
    return *tok == '\0' && *name == '\0';
}

static host_s8 clamp_s8(long v) {
    return (host_s8)(v < -128 ? -128 : v > 127 ? 127 : v);
}

static ScriptEvent *add_event(void) {
    if (sCount == sCap) {
        host_u32 n = sCap ? sCap * 2 : 64;
        ScriptEvent *e = (ScriptEvent *)malloc(n * sizeof(ScriptEvent));
        if (e == NULL) {
            host_fatal("input: out of memory");
        }
        if (sEvents != NULL) {
            memcpy(e, sEvents, sCount * sizeof(ScriptEvent));
            free(sEvents);
        }
        sEvents = e;
        sCap = n;
    }
    memset(&sEvents[sCount], 0, sizeof(ScriptEvent));
    return &sEvents[sCount++];
}

/* <when>: N | N-M | N:L, each optionally +D (relative to the previous F line's first step) */
static int parse_when(const char *t, host_u32 prevFirst, host_u32 *first, host_u32 *last) {
    char *end;
    unsigned long a, b;
    int rel = 0;

    if (*t == '+') {
        rel = 1;
        t++;
    }
    a = strtoul(t, &end, 10);
    if (end == t) {
        return -1;
    }
    if (rel) {
        a += prevFirst;
    }
    *first = *last = (host_u32)a;
    if (*end == '-' || *end == ':') {
        char sep = *end;
        t = end + 1;
        if (sep == '-' && rel) {
            if (*t != '+') {
                return -1;
            }
            t++;
        }
        b = strtoul(t, &end, 10);
        if (end == t) {
            return -1;
        }
        if (sep == '-') {
            if (rel) {
                b += prevFirst;
            }
            if (b < a) {
                return -1;
            }
            *last = (host_u32)b;
        } else {
            if (b == 0) {
                return -1;
            }
            *last = (host_u32)(a + b - 1);
        }
    }
    return *end == '\0' ? 0 : -1;
}

static int parse_state_token(const char *t, ScriptEvent *e) {
    unsigned i;

    for (i = 0; i < sizeof(kButtons) / sizeof(kButtons[0]); i++) {
        if (tok_eq(t, kButtons[i].name)) {
            e->buttons |= kButtons[i].bit;
            return 0;
        }
    }
    if (t[0] == '0' && (t[1] == 'x' || t[1] == 'X')) {
        char *end;
        unsigned long v = strtoul(t + 2, &end, 16);
        if (end != t + 2 && *end == '\0' && v <= 0xFFFF) {
            e->buttons |= (host_u16)v;
            return 0;
        }
    }
    return -1;
}

static int parse_line(char *s, int lineNo, host_u32 *prevFirst) {
    char *tok[64];
    int n = 0, i;
    char *p = s;

    while (*p != '\0' && *p != '#') {
        while (is_space(*p)) {
            p++;
        }
        if (*p == '\0' || *p == '#') {
            break;
        }
        if (n == 64) {
            host_log("input: line %d: too many tokens", lineNo);
            return -1;
        }
        tok[n++] = p;
        while (*p != '\0' && *p != '#' && !is_space(*p)) {
            p++;
        }
        if (*p == '#') {
            *p = '\0';
            break;
        }
        if (*p != '\0') {
            *p++ = '\0';
        }
    }
    if (n == 0) {
        return 0;
    }
    if (tok_eq(tok[0], "PADS") && n == 2) {
        long v = strtol(tok[1], NULL, 10);
        if (v < 1 || v > HOST_MAX_PADS) {
            host_log("input: line %d: PADS takes 1..4", lineNo);
            return -1;
        }
        sNumPads = (int)v;
        return 0;
    }
    if (tok_eq(tok[0], "RUMBLE") && n == 2) {
        long v = strtol(tok[1], NULL, 10);
        if (v < 1 || v > HOST_MAX_PADS) {
            host_log("input: line %d: RUMBLE takes a pad 1..4", lineNo);
            return -1;
        }
        sRumble[v - 1] = 1;
        return 0;
    }
    if (tok_eq(tok[0], "F") && n >= 2) {
        host_u32 first, last;
        ScriptEvent *e;
        if (parse_when(tok[1], *prevFirst, &first, &last) != 0) {
            host_log("input: line %d: bad step range '%s'", lineNo, tok[1]);
            return -1;
        }
        *prevFirst = first;
        e = add_event();
        e->first = first;
        e->last = last;
        e->pad = 0;
        for (i = 2; i < n; i++) {
            const char *t = tok[i];
            if ((t[0] == 'P' || t[0] == 'p') && t[1] >= '1' && t[1] <= '4' && t[2] == '\0') {
                if (e->buttons != 0 || e->hasStick) {
                    e = add_event(); /* a new controller group on the same line */
                    e->first = first;
                    e->last = last;
                }
                e->pad = (host_u8)(t[1] - '1');
                continue;
            }
            if (tok_eq(t, "STICK") && i + 1 < n) {
                char *end;
                long x = strtol(tok[i + 1], &end, 10);
                long y;
                if (*end != ',') {
                    host_log("input: line %d: STICK wants x,y", lineNo);
                    return -1;
                }
                y = strtol(end + 1, &end, 10);
                if (*end != '\0') {
                    host_log("input: line %d: STICK wants x,y", lineNo);
                    return -1;
                }
                e->hasStick = 1;
                e->stickX = clamp_s8(x);
                e->stickY = clamp_s8(y);
                i++;
                continue;
            }
            if (parse_state_token(t, e) != 0) {
                host_log("input: line %d: unknown button '%s'", lineNo, t);
                return -1;
            }
        }
        if (last > sLastStep) {
            sLastStep = last;
        }
        return 0;
    }
    host_log("input: line %d: unknown directive '%s'", lineNo, tok[0]);
    return -1;
}

int host_input_script_open(const char *path) {
    char *text;
    host_u32 size, i, start = 0, prevFirst = 0;
    int lineNo = 1;

    if (host_file_read_all(path, (void **)&text, &size) != 0) {
        host_log("input: cannot read %s", path);
        return -1;
    }
    for (i = 0; i <= size; i++) {
        if (i == size || text[i] == '\n') {
            char line[1024];
            host_u32 len = i - start;
            if (len >= sizeof(line)) {
                host_log("input: line %d too long", lineNo);
                host_free(text);
                return -1;
            }
            memcpy(line, text + start, len);
            line[len] = '\0';
            if (parse_line(line, lineNo, &prevFirst) != 0) {
                host_free(text);
                return -1;
            }
            start = i + 1;
            lineNo++;
        }
    }
    host_free(text);
    for (i = 0; i < sCount; i++) {
        if (sEvents[i].pad + 1 > sNumPads) {
            sNumPads = sEvents[i].pad + 1;
        }
    }
    sLoaded = 1;
    sStep = 0;
    host_log("input: %s: %u states, %d pad(s), last step %u", path, (unsigned)sCount, sNumPads,
             (unsigned)sLastStep);
    return 0;
}

int host_input_script_active(void) {
    return sLoaded;
}

static void pads_at(host_u32 step, HostPad pads[HOST_MAX_PADS]);

/* The pads for the current step; advances the step. */
void host_input_script_step(HostPad pads[HOST_MAX_PADS]) {
    pads_at(sStep, pads);
    sStep++;
}

/* Step 0's pads, for the boot-time SI reset (connections and rumble paks). */
void host_input_script_boot(HostPad pads[HOST_MAX_PADS]) {
    pads_at(0, pads);
}

static void pads_at(host_u32 step, HostPad pads[HOST_MAX_PADS]) {
    host_u32 i;
    int p;

    memset(pads, 0, HOST_MAX_PADS * sizeof(HostPad));
    for (p = 0; p < HOST_MAX_PADS; p++) {
        pads[p].connected = p < sNumPads;
        pads[p].hasRumble = sRumble[p];
    }
    for (i = 0; i < sCount; i++) {
        const ScriptEvent *e = &sEvents[i];
        if (step < e->first || step > e->last) {
            continue;
        }
        pads[e->pad].button |= e->buttons;
        if (e->hasStick) {
            pads[e->pad].stick_x = e->stickX;
            pads[e->pad].stick_y = e->stickY;
        }
    }
}
