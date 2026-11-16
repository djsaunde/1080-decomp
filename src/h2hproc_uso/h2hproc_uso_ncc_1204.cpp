/* IDO 7.1 NCC (C++) donor for h2h_WinsDisplay_update_00001204 (0x15C / 87 words): fade a
 * 0..720 alpha up or down by 16, then run the registered state callback. Decode fix:
 * each clamp is NESTED inside its step (`if (a < 0xFF) { a += 16; if (a >= 0x100)
 * a = 0x2D0; }`) -- both branches of the target skip to the shared tail; the old NM
 * clamped unconditionally. The two calls whose results are discarded go through a
 * void alias (kit XVIII #3) so the record base takes v0. Spliced into
 * h2hproc_uso.c.o via REPLACE_FUNC_BODY. */
/* Field views (agent-h run 533). NCC donors build without -I include and edg rejects the headers'
 * &((T *)0)->f asserts, so this donor carries layout mirrors of the types it touches (keep them in
 * step): H2HWinsDisplay and WhiteFader (game/modeproc.h), RaceProcess.timeout (gdl/race.h) and the
 * GuiMenu cursor / rows (game/guimenu.h; rows[cursor].arg is the selected row's callback). */
typedef struct H2HWinsDisplay {
    /* 0x00 */ char node[0x2C];
    /* 0x2C */ int race;
    /* 0x30 */ int menu;
    /* 0x34 */ int fader;
} H2HWinsDisplay;
#define H2HWINS(p) ((H2HWinsDisplay *)(p))
typedef struct WhiteFader { char node[0x2C]; float colour[4]; int alpha; } WhiteFader;
#define WFADER(p) ((WhiteFader *)(p))
typedef struct RaceProcess { char pad000[0x4F4]; int timeout; } RaceProcess;
#define RACEPROC(p) ((RaceProcess *)(p))
typedef void (*GuiRowFn)(void); /* as game/guimenu.h */
typedef struct GuiRow { int label; int enabled; int id; GuiRowFn arg; char sub[0x18]; } GuiRow;
typedef struct GuiMenu { char head[0x7C]; int cursor; int locked; GuiRow rows[16]; } GuiMenu;
#define GUIMENU(p) ((GuiMenu *)(p))

extern "C" {
extern char D_00000000;
int gl_func_00000000(...); void h2h_void_1204(...);
void h2h_WinsDisplay_update_00001204(char *a0) {
    char *p1;
    char *v0;

    if (RACEPROC(H2HWINS(a0)->race)->timeout < 2) {
        int *fp;
        v0 = (char *)H2HWINS(a0)->fader;
        fp = &WFADER(v0)->alpha;
        if (*fp < 0xFF) {
            *fp += 0x10;
            v0 = (char *)H2HWINS(a0)->fader;
            if (WFADER(v0)->alpha >= 0x100) {
                WFADER(v0)->alpha = 0x2D0;
            }
        }
    } else {
        int *fp;
        v0 = (char *)H2HWINS(a0)->fader;
        fp = &WFADER(v0)->alpha;
        if (*fp > 0) {
            *fp -= 0x10;
            v0 = (char *)H2HWINS(a0)->fader;
            if (WFADER(v0)->alpha < 0) {
                WFADER(v0)->alpha = 0;
            }
        }
    }
    if (gl_func_00000000(&D_00000000, 0x40100) != 0) {
        if (gl_func_00000000(*(int*)(&D_00000000 + 0x190)) != 0) {
            p1 = (char *)H2HWINS(a0)->menu;
            if (GUIMENU(p1)->rows[GUIMENU(p1)->cursor].arg != 0) {
                h2h_void_1204(5);
                *(int*)((char*)&D_00000000 + 0x4) = H2HWINS(a0)->menu;
                *(int*)((char*)&D_00000000 + 0x0) = H2HWINS(a0)->race;
                p1 = (char *)H2HWINS(a0)->menu;
                GUIMENU(p1)->rows[GUIMENU(p1)->cursor].arg();
            }
        }
    }
    h2h_void_1204(a0);
}
}
