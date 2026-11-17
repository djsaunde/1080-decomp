/* IDO 7.1 NCC (C++) EXACT donors for trnproc.uso (agent-c run 643), REPLACE_FUNC_BODY-spliced over
 * the empty placeholders in trnproc_uso.c on both build paths (Makefile TRN_NCC_DONOR).
 * trnproc.uso is C++: these are the functions whose shape only edg's lowering reproduces. */
extern "C" int gl_func_00000000(...);

/* game.uso Bss+0x8: the TrickTable */
extern "C" char *trn_gTrickTable;

/* bootup Bss+0xF8: the InputMgr (input_checkPressed / Released's first argument) */
extern "C" char D_00000000;
/* void-returning blank import (pinned at 0): a call whose callee returns nothing leaves v0 free */
extern "C" void gl_func_00000000_void(...);

/* gui.uso Bss+0x8: the gui colour state; trnproc Bss (the HUD sprites) and Data (the format strings) */
extern "C" char trn_gGui;
extern "C" char trn_bss[];
extern "C" char trn_data[];

/* the HUD sprites (RaceSprite in include/gdl/race.h): the image's width places the next one */
struct RaceSpriteImage {
    char pad0[0x20];
    short w;                /* 0x20 */
    int getWidth() { return w; }
};
struct RaceSprite {
    char pad0[0x10];
    RaceSpriteImage *image; /* 0x10 */
    int fileId;             /* 0x14 */
    int width() { return image->getWidth(); }
};
/* trnproc Bss+0x0 / +0xF0: the button sprite tables (entries 0..31 / 32..); the second is a
 * symbol pinned at 0xF0, so edg keeps the -32 * 24 apart from its address */
extern "C" RaceSprite trn_sprites[];
extern "C" RaceSprite trn_sprites_F0[];

/* an RGBA float colour (gui_setColor / gui_setPrimColor / gui_setColorEnvPrim take its address) */
struct Colour {
    float r, g, b, a;
    void set(float r_, float g_, float b_, float a_) { r = r_; g = g_; b = b_; a = a_; }
    Colour() { r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f; }
    /* from 0..255 components (the divisions stay at run time: edg passes them through set) */
    Colour(int r_, int g_, int b_, int a_) { set(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f, a_ / 255.0f); }
};

/* the player record (race info [0]): unlocked-trick bits at +0x8C0 */
struct TrnPlayer {
    char pad0[0x8C0];
    int unlocked;           /* 0x8C0 */
    char pad8c4[0x9A8 - 0x8C4];
    int flags9A8;           /* 0x9A8: bit 0 clears the lesson's course flag */
};
struct TrnRaceInfo {
    TrnPlayer *player;      /* 0x00 */
    char pad4[0x34 - 0x4];
    int over;               /* 0x34: the run is over (pause the race) */
};
/* the TrnRace process */
struct TrnRace {
    char pad0[0x48];
    void *proc48;           /* 0x48: the process Process_stop halts with the results screen */
    char pad4c[0x4DC - 0x4C];
    int running;            /* 0x4DC: 1 while the race runs */
};

/* TrnDataManager (a RaceTracker subclass, 0x19C bytes) */
struct TrnDataManager {
    char pad0[0x44];
    TrnRaceInfo *info;      /* 0x44 */
    char pad48[0x54 - 0x48];
    int gui;                /* 0x54: the gui draw context */
    char pad58[0xB8 - 0x58];
    TrnRace *race;          /* 0xB8 */
    char padbc[0xD0 - 0xBC];
    int pulse;              /* 0xD0: the current lesson row's blinking alpha (-16 per frame) */
    int frame;              /* 0xD4: lesson HUD frame counter */
    int hudX;               /* 0xD8: lesson HUD layout (24, 8, -1, 40) */
    int hudY;               /* 0xDC */
    int hudDX;              /* 0xE0 */
    int hudDY;              /* 0xE4 */
    char pade8[0xF0 - 0xE8];
    int lesson;             /* 0xF0: current lesson (TrickTable index) */
    int armed;              /* 0xF4 */
    int course;             /* 0xF8: 1 = the restricted course, < 0 = no lesson */
    int results;            /* 0xFC: results screen: 0 off, 1 browsing, 2 picked (wait for release) */
    int cursor;             /* 0x100 */
    int top;                /* 0x104: first of the 12 shown rows */
    int count;              /* 0x108: lesson count */
    char pad10c[0x118 - 0x10C];
    int inputs[32];         /* 0x118: the lesson's input list, -1 terminated */
    int button;             /* 0x198: the button that picked the results row */
};

/* TrnDataManager_buildLessonInputs: build lesson idx's input list from the TrickTable input mask
 * (TrickTable_getInputMask_0000C378): 35 = button, 8 / 9 = the 0x5000 / 0xA000 pairs, 33 / 34,
 * 255 = new line, the stick direction (16C8) - 1, then the single-button bits.
 * edg's n++ temporary keeps the index in its own register at every store (no strength
 * reduction, unlike cfe). `scratch` is a dead one-word array: its frame word sits above n
 * and mask, which puts them at the target's sp+0x20 / +0x1C (frame 0x28). */
extern "C" void TrnDataManager_buildLessonInputs(TrnDataManager *self, int idx) {
    int scratch[1];
    int n;
    int mask;
    int dir;

    n = 0;
    scratch[0] = 0;
    mask = gl_func_00000000(trn_gTrickTable, idx); /* TrickTable_getInputMask_0000C378 */
    if (mask & 0x100000) {
        self->inputs[n++] = 35;
        if (mask & 0x5000) self->inputs[n++] = 8;
        if (mask & 0xA000) self->inputs[n++] = 9;
        self->inputs[n++] = 255;
    }
    if (mask & 0x200000) {
        self->inputs[n++] = 35;
        if (mask & 0x5000) self->inputs[n++] = 8;
        if (mask & 0xA000) self->inputs[n++] = 9;
        self->inputs[n++] = 255;
        self->inputs[n++] = 35;
        if (mask & 0x5000) self->inputs[n++] = 8;
        if (mask & 0xA000) self->inputs[n++] = 9;
        self->inputs[n++] = 33;
        self->inputs[n++] = 255;
    }
    if (mask & 4) self->inputs[n++] = 35;
    dir = gl_func_00000000(self, mask); /* TrnDataManager_stickDirSprite */
    if (dir != 0) self->inputs[n++] = dir - 1;
    if (mask & 0x1000) self->inputs[n++] = 6;
    if (mask & 0x2000) self->inputs[n++] = 2;
    if (mask & 0x4000) self->inputs[n++] = 8;
    if (mask & 0x8000) self->inputs[n++] = 9;
    if (mask & 0x20000) self->inputs[n++] = 0;
    if (mask & 0x10000) self->inputs[n++] = 4;
    if (mask & 2) self->inputs[n++] = 33;
    if (mask & 1) self->inputs[n++] = 34;
    self->inputs[n] = -1;
}

/* TrnDataManager_isLessonAvailable: is lesson idx available: its trick's unlock mask
 * (TrickTable_getField34_0000C394) against the player's unlocked bits; on the restricted
 * course a trick flagged 4 (TrickTable_getFlags_0000C3B0) is excluded. edg evaluates the
 * `&` left operand (the spilled mask) first, which cfe never does. */
extern "C" int TrnDataManager_isLessonAvailable(TrnDataManager *self, int idx) {
    int mask = gl_func_00000000(trn_gTrickTable, idx);
    int flags = gl_func_00000000(trn_gTrickTable, idx);
    if (mask & self->info->player->unlocked) {
        if (self->course == 1) {
            if (flags & 4) {
                return 0;
            }
            return 1;
        }
        return 1;
    }
    return 0;
}

/* TrnDataManager_update: TrnDataManager update. A run that is over pauses the race (9A0).
 * While the race runs: Start (0x20) either disarms the lesson (sound 0x802) or opens the results
 * screen at the current lesson (race paused, its +0x48 process stopped); on the results screen
 * the list scrolls (1098), A (0x100) or 0x40000 picks a row, and on that button's release an
 * available lesson (1778) becomes current and armed (13C4); a pick or Start closes the screen.
 * The two void callees (Process_stop, 1098) go through the void import: v0 then holds the
 * lesson / the results state. */
extern "C" void TrnDataManager_update(TrnDataManager *self) {
    int picked = 0;
    if (self->info->player->flags9A8 & 1) {
        self->course = 0;
    }
    gl_func_00000000(self); /* RaceTracker_passCheckpoints */
    gl_func_00000000(self); /* RaceTracker_update */
    if (self->info->over != 0) {
        gl_func_00000000(self->race); /* TrnRace_forceEnd */
        return;
    }
    if (self->race->running == 1) {
        if (self->results == 0) {
            if (self->course >= 0 && gl_func_00000000(&D_00000000, 0x20)) { /* input_checkPressed_00011E00 */
                if (self->armed != 0) {
                    self->armed = 0;
                    gl_func_00000000(0x802); /* snd_postEvent */
                } else {
                    gl_func_00000000(self->race); /* Process_pause_0000F9E8 */
                    gl_func_00000000_void(self->race->proc48); /* Process_stop */
                    self->results = 1;
                    self->top = self->lesson;
                    if (self->lesson > self->count - 12) {
                        self->top = self->count - 12;
                    }
                    self->cursor = self->lesson;
                }
            }
        } else {
            gl_func_00000000_void(self); /* TrnDataManager_scrollResults */
            switch (self->results) {
            case 1:
                if (gl_func_00000000(&D_00000000, 0x100)) { /* input_checkPressed_00011E00 */
                    self->button = 0x100;
                    self->results = 2;
                }
                if (gl_func_00000000(&D_00000000, 0x40000)) {
                    self->button = 0x40000;
                    self->results = 2;
                }
                break;
            case 2:
                /* input_checkReleased_00011FA8, TrnDataManager_isLessonAvailable */
                if (gl_func_00000000(&D_00000000, self->button) && gl_func_00000000(self, self->cursor)) {
                    self->armed = 1;
                    self->lesson = self->cursor;
                    gl_func_00000000(self, self->lesson); /* TrnDataManager_buildLessonInputs */
                    picked = 1;
                    self->button = 0;
                }
                break;
            }
            if (picked || gl_func_00000000(&D_00000000, 0x20)) {
                gl_func_00000000(self->race); /* Process_resume_0000FAE8 */
                self->results = 0;
            }
        }
    }
    gl_func_00000000(self); /* Node_update */
}

/* TrnDataManager_drawLesson: TrnDataManager lesson HUD: the lesson title ("%d: %s" with the
 * TrickTable entry name, red once the lesson's trick is done), then the input list (+0x118, -1
 * terminated): 255 starts a new line, any other entry is a button sprite, with the "+" sprite
 * (Bss+0x168) between two buttons on one line. The 0.2f / 0.8f literals are RoData 0x20 / 0x24
 * (TrnDataManager_drawLesson_rodata pinned at 0x20). Only the button draw goes through the
 * int-returning import: its block then keeps the image pointer off v0 (a0, as the target). */
extern "C" void TrnDataManager_drawLesson(TrnDataManager *self) {
    char buf[64];
    int x;
    int y;
    int i;
    float col[4];
    int nl;
    RaceSprite *spr;

    gl_func_00000000(self->gui); /* gui_setupRenderMode_0000055C */
    if (gl_func_00000000(self, self->lesson)) { /* TrnDataManager_isTrickDone */
        col[0] = 0.8f;
        col[1] = 0.2f;
        col[2] = 0.2f;
        col[3] = 1.0f;
    } else {
        col[0] = 1.0f;
        col[1] = 1.0f;
        col[2] = 1.0f;
        col[3] = 1.0f;
    }
    gl_func_00000000(&trn_gGui, 255, col); /* gui_setColor_00001A5C */
    /* gl_sprintf "%d: %s" with TrickTable_getEntry_0000C344, then gui_drawString */
    gl_func_00000000(buf, trn_data + 0x328, self->lesson + 1, gl_func_00000000(trn_gTrickTable, self->lesson));
    gl_func_00000000(self->gui, 32, self->hudY + 8, buf);
    self->frame++;
    x = self->hudX + self->hudDX;
    y = self->hudY + self->hudDY;
    i = 0;
    while (self->inputs[i] != -1) {
        nl = 0;
        if (self->inputs[i] != 255) {
            if (self->inputs[i] < 32) {
                spr = &trn_sprites[self->inputs[i]];
            } else {
                spr = &trn_sprites_F0[self->inputs[i] - 32];
            }
            gl_func_00000000_void(spr); /* gui_spriteRefresh */
            gl_func_00000000_void(spr); /* gui_setCombine_00001794 */
            gl_func_00000000(spr, x, y, 2); /* gui_drawSprite */
            x += spr->width();
        } else {
            y += 28;
            nl = 1;
            x = self->hudX + self->hudDX;
        }
        i++;
        if (self->inputs[i] != -1 && self->inputs[i] != 255 && nl == 0) {
            gl_func_00000000_void(trn_bss + 0x168);
            gl_func_00000000_void(trn_bss + 0x168);
            gl_func_00000000_void(trn_bss + 0x168, x, y, 2);
            x += ((RaceSprite *)(trn_bss + 0x168))->width();
        }
    }
}

/* TrnDataManager_drawResults: TrnDataManager results screen: the header sprite (+0x180), then 12
 * rows from the window top (+0x104): "%d" right-aligned at 86 and the trick name at 90, red
 * (0.8, 0.2, 0.2) once the lesson is done (13A0); the current available lesson blinks
 * (alpha +0xD0), available ones are opaque, the rest alpha 64; then the cursor sprite (+0x198)
 * tinted (233, 255, 0) at the cursor row. The float literals are the RoData words at 0x28 /
 * 0x2C (after B78's two in the donor .rodata: TrnDataManager_drawResults_rodata is pinned at 0x20). */
extern "C" void TrnDataManager_drawResults(TrnDataManager *self) {
    Colour col;
    Colour hl(233, 255, 0, 255);
    char buf[128];
    int row;
    int idx;
    int y;
    int cy;

    self->pulse -= 16;
    gl_func_00000000(&trn_gGui, 255, &col); /* gui_setColor_00001A5C */
    gl_func_00000000(trn_bss + 0x180); /* gui_spriteRefresh */
    gl_func_00000000(trn_bss + 0x180, 160, 30, 3); /* gui_drawSprite */
    gl_func_00000000(self->gui); /* gui_setupRenderMode_0000055C */
    gl_func_00000000(self->gui); /* gui_setPrimColorClamped_000008C0 */
    for (row = 0, y = 40; row < 12; row++, y += 14) {
        idx = row + self->top;
        if (gl_func_00000000(self, idx)) { /* TrnDataManager_isTrickDone */
            col.r = 0.8f;
            col.g = 0.2f;
            col.b = 0.2f;
            col.a = 1.0f;
        } else {
            col.r = 1.0f;
            col.g = 1.0f;
            col.b = 1.0f;
            col.a = 1.0f;
        }
        /* TrnDataManager_isLessonAvailable, gui_setPrimColor_000006B8 */
        if (idx == self->lesson && gl_func_00000000(self, idx)) {
            gl_func_00000000(self->gui, self->pulse, &col, 255);
        } else if (gl_func_00000000(self, idx)) {
            gl_func_00000000(self->gui, 255, &col, 255);
        } else {
            gl_func_00000000(self->gui, 64, &col, 255);
        }
        gl_func_00000000(buf, trn_data + 0x330, idx + 1); /* gl_sprintf "%d" */
        /* gui_drawString_00000D04 at gui_rightAlignStringX_000015F4 */
        gl_func_00000000(self->gui, gl_func_00000000(self->gui, 86, buf), y, buf);
        /* gl_sprintf "%s" with TrickTable_getEntry_0000C344, gui_drawString */
        gl_func_00000000(buf, trn_data + 0x334, gl_func_00000000(trn_gTrickTable, idx));
        gl_func_00000000(self->gui, 90, y, buf);
    }
    cy = (self->cursor - self->top) * 14 + 49;
    gl_func_00000000(trn_bss + 0x198); /* gui_spriteRefresh */
    gl_func_00000000(trn_bss + 0x198, 255, &hl, 255); /* gui_setColorEnvPrim_000017DC */
    gl_func_00000000(trn_bss + 0x198, 42, cy, 3); /* gui_drawSprite */
}
