#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) EXACT donors for nameent.uso (agent-c run 643), REPLACE_FUNC_BODY-spliced over
 * the empty placeholders in nameent_uso.c on both build paths (Makefile NE_NCC_DONOR).
 * nameent.uso is C++: these are the functions whose shape only edg's lowering reproduces (a
 * virtual call keeps the object in v1 and its vtable in v0). */
extern "C" int gl_func_00000000(...);
/* void-returning blank import (pinned at 0): a callee that returns nothing leaves v0 free */
extern "C" void gl_func_00000000_void(...);

/* gui.uso Bss (the gui draw state) and nameent.uso's own Data (pinned at 0) */
extern "C" char ne_gGui;
extern "C" char ne_data[];

/* gdl Node with its cfront vtable (vptr at +0x28): the slots this module calls */
struct NeNode {
    char node[0x28];
    virtual void slot0(); virtual void slot1(); virtual void slot2(); virtual void slot3();
    virtual void slot4(); virtual void slot5(); virtual void slot6(); virtual void slot7();
    virtual void slot8(); virtual void slot9(); virtual void slot10();
    virtual int getBoardTexture();      /* slot 11: the rider's board texture */
};

/* GameFlow (bootup Bss+0xF8, include/game/input.h InputMgr): the current player's rider */
struct NePlayer { char pad0[0x108]; NeNode *rider; };
struct NeGameFlow {
    char pad0[0x134];
    NePlayer *player;           /* 0x134 */
    char pad138[0x158 - 0x138];
    unsigned char name[4];      /* 0x158: the last name entered (9 bytes copied, through +0x160) */
    int nameLen;                /* 0x15C */
    char pad160[0x190 - 0x160];
    void *fader;                /* 0x190: the shared Fader */
};
extern "C" NeGameFlow D_00000000;

/* an RGBA float colour */
struct Colour {
    float r, g, b, a;
    void set(float r_, float g_, float b_, float a_) { r = r_; g = g_; b = b_; a = a_; }
    /* from 0..255 components (the divisions stay at run time: edg passes them through set) */
    void set255(int r_, int g_, int b_, int a_) { set(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f, a_ / 255.0f); }
    /* transparent black (the target stores a, b, g, r in that order) */
    void clear() { a = 0.0f; b = 0.0f; g = 0.0f; r = 0.0f; }
};

/* a gui image header (Data+0x158 is the name board's) */
struct NeImage {
    int tex;            /* 0x00 */
    int width;          /* 0x04 */
    int rest[4];
};

/* the NameEnter process */
struct NeProc {
    char pad0[0x58];
    int cursorX;        /* 0x58 */
};

/* NameEntry_drawBoard: board draw: the name-board image (a copy of the Data+0x158 header
 * with the current rider's board texture, virtual slot 11) centred on the cursor x at y 40 in
 * white, then the two black letterbox bars. The dead one-element stores to `scratch1` /
 * `scratch2` emit no code; their frame words give the target's frame (0x80: x at 0x7C, col at
 * 0x64, img at 0x4C, black at 0x3C). */
extern "C" void NameEntry_drawBoard(NeProc *self) {
    int x;
    int scratch1[2];
    Colour col;
    NeImage img;
    Colour black;
    int scratch2[4];

    scratch1[0] = 0;
    scratch2[0] = 0;
    col.set(1.0f, 1.0f, 1.0f, 1.0f);
    img = *(NeImage *)(ne_data + 0x158);
    img.tex = D_00000000.player->rider->getBoardTexture();
    CALL(gui_beginFrame_000016A4)(&img); /* gui_beginFrame_000016A4 */
    x = self->cursorX - img.width / 2;
    CALL(gui_setColorEnvPrim_00002334)(&img, 128, &col, 255); /* gui_setColorEnvPrim_00002334 */
    CALL(gui_drawImage_00003B80)(&img, x, 40); /* gui_drawImage_00003B80 */
    black.set(0.0f, 0.0f, 0.0f, 0.0f);
    CALL(gui_setFillColor_00001CA8)(&ne_gGui, 255, &black); /* gui_setFillColor_00001CA8 */
    CALL(gui_fillRect)(&ne_gGui, 0, 0, 319, 7, 0x10001); /* gui_fillRect */
    CALL(gui_fillRect)(&ne_gGui, 0, 232, 319, 239, 0x10001); /* gui_fillRect */
}

/* the Node vtable (bootup Data+0xC338), the entry's own vtable (nameent Data+0x18) and the gui
 * string-table base (bootup Bss+0x28C): imports / own-section objects whose in-place addend is 0,
 * so each is a symbol pinned at 0 */
extern "C" char ne_vt_node, ne_vt_entry;
extern "C" int ne_gRowBase;

/* the name-entry object (0x11C bytes; layout from the ctor; the input handler is E44) */
struct NeEntry {
    char pad0[0xC];
    const char *name;           /* 0x0C */
    char pad10[0x28 - 0x10];
    void *vptr;                 /* 0x28 */
    int mode;                   /* 0x2C: the record session (0 = none) */
    void *owner;                /* 0x30: the NameEnter process */
    int newCount;               /* 0x34: Backup_sessionCountNew(mode) */
    int w38, w3C, w40, w44, w48, w4C, w50, w54, w58; /* 0x38..0x58: board / letter layout */
    int repeat;                 /* 0x5C: stick held frames (auto-repeat after repeatDelay) */
    int repeatTick;             /* 0x60 */
    int repeatDelay;            /* 0x64: 6 */
    int repeatRate;             /* 0x68: 1 */
    float pos;                  /* 0x6C: the letter wheel's position (0..42, eased toward target) */
    float target;               /* 0x70: the selected letter (41 = END) */
    float vel;                  /* 0x74 */
    float maxVel;               /* 0x78: 0.5 */
    float gain;                 /* 0x7C: 0.2 */
    Colour c80, c90, cA0;       /* 0x80, 0x90, 0xA0 */
    int len;                    /* 0xB0: letters entered (GameFlow +0x15C) */
    int done;                   /* 0xB4: 3 letters entered: the wheel goes to END */
    unsigned char name2[12];    /* 0xB8: the letters (42 = blank; GameFlow +0x158..) */
    int wC4, wC8, wCC;          /* 0xC4..0xCC */
    float fD0;                  /* 0xD0 */
    char spr0[0x18];            /* 0xD4: gui sprites */
    char spr1[0x18];            /* 0xEC */
    char spr2[0x18];            /* 0x104 */
};

/* NameEnterMessage_ctor: the name-entry ctor (`this == 0` allocates 0x11C; the inlined Node
 * base ctor, then the layout constants, the last name from GameFlow +0x158, the colours and three
 * gui sprites). The 0.2f literal is RoData+0 (NameEnterMessage_ctor_rodata pinned at 0). */
extern "C" NeEntry *NameEnterMessage_ctor(NeEntry *self, void *owner, int mode) {
    int i;
    char *o;
    if (self != 0 || (self = (NeEntry *)CALL(mem_new)(0x11C)) != 0) { /* mem_new */
        o = (char *)self;
        if (o != 0 || (o = (char *)CALL(mem_new)(0x2C)) != 0) {
            CALL(Node_ctor)(o, ne_data + 0x170); /* Node_ctor */
            *(void **)(o + 0x28) = &ne_vt_node;
        }
        self->vptr = &ne_vt_entry;
        self->name = ne_data + 0x178;
        self->mode = mode;
        self->owner = owner;
        if (mode != 0) {
            self->newCount = CALL(Backup_sessionCountNew)(self->mode); /* Backup_sessionCountNew */
        } else {
            self->newCount = 0;
        }
        self->w38 = 160;
        self->w3C = 200;
        self->w40 = 124;
        self->w44 = 160;
        self->w48 = 208;
        self->w4C = 160;
        self->w58 = 22;
        self->w50 = 5;
        self->w54 = 33;
        self->repeat = 0;
        self->repeatDelay = 6;
        self->repeatTick = 0;
        self->repeatRate = 1;
        self->vel = 0.0f;
        self->maxVel = 0.5f;
        self->gain = 0.2f;
        self->done = 0;
        self->wC4 = 128;
        self->wC8 = 255;
        self->wCC = 255;
        self->len = D_00000000.nameLen;
        for (i = 0; i < 9; i++) {
            self->name2[i] = D_00000000.name[i];
        }
        if (self->len != 0) {
            self->target = 41.0f;
        } else {
            self->target = 0.0f;
        }
        self->pos = self->target;
        self->c80.set255(128, 255, 128, 255);
        self->c90.clear();
        self->cA0.set255(128, 128, 128, 255);
        self->fD0 = 0.0f;
        CALL(gui_spriteInit)(self->spr0, (ne_gRowBase + 4) | 0x250000); /* gui_spriteInit */
        CALL(gui_spriteInit)(self->spr1, 0x250003);
        CALL(gui_spriteInit)(self->spr2, 0x250000);
    }
    return self;
}

/* the owning NameEnter process: +0x6C = a name was entered (NameEntry_updateEntry) */
struct NeOwner { char pad0[0x6C]; int entered; };
/* input_getStickX_00011D78 returns a float: a typed alias of the blank import (pinned at 0) */
extern "C" float ne_input_getStickX(void *input);

/* gui_drawSpriteScroll_00003714 takes its two scales as floats: a typed alias of the blank
 * import (pinned at 0) */
extern "C" int ne_gui_drawSpriteScroll(char *spr, int x, int y, int idx, int h, float sx, float sy);

/* NameEnterMessage_display: the name-entry display: the board sprites fade in (+0xD0 += 0.05),
 * the entered letters and the three cursor slots (the current one tinted +0x80), then the letter
 * wheel: 2 * +0x50 + 1 letters around the wheel position, each scaled by its distance from the
 * centre (x 160) and the centre letter enlarged (1.2) at full alpha, clipped to x 40..284.
 * Frame: the colour is four member stores and the fade a plain `x = x + k` (an inline set() or a
 * `+=` on self->fD0 takes an edg temporary word, which would push the loop-1 spill slot down);
 * the two dead scratch words put the scales at sp+0x74 / +0x70. */
extern "C" void NameEnterMessage_display(NeEntry *self) {
    char *spr;
    Colour col;
    float wheel;
    int x0;
    int i;
    int j;
    int k;
    int x;
    int alpha;
    float fx;
    float dist;
    float d;
    float a;
    int scratch;
    int scratch2;
    float sx;
    float sy;

    scratch = 0;
    scratch2 = 0;
    col.r = 1.0f;
    col.g = 1.0f;
    col.b = 1.0f;
    col.a = 1.0f;
    if (self->fD0 < 1.0f) {
        self->fD0 = self->fD0 + 0.05f;
    }
    CALL(gui_spriteRefresh)(self->spr1);
    CALL(gui_setColorEnvPrim_000017DC)(self->spr1, (int)(255.0f * self->fD0), &col, 255);
    CALL(gui_drawSprite)(self->spr1, self->w38, self->w3C, 3);
    CALL(gui_spriteRefresh)(self->spr0);
    CALL(gui_setColorEnvPrim_000017DC)(self->spr0, (int)(255.0f * self->fD0), &self->cA0, 255);
    CALL(gui_drawSprite)(self->spr0, self->w40, self->w44, 3);
    spr = self->spr2;
    CALL(gui_spriteRefresh)(spr);
    CALL(gui_setColorEnvPrim_000017DC)(spr, (int)(255.0f * self->fD0), &col, 255);
    for (i = 0; i < self->len; i++) {
        if (self->name2[i] < 41) {
            CALL(gui_drawSpriteIA8Clip_00002BB0)(self->spr2, self->w48 + i * self->w58 - 12, self->w4C - 12, self->name2[i] * 22 + 2, 20);
        }
    }
    for (i = 0; i < 3; i++) {
        if (i == self->len) {
            CALL(gui_setColorEnvPrim_000017DC)(self->spr2, (int)(255.0f * self->fD0), &self->c80, 255);
        } else {
            CALL(gui_setColorEnvPrim_000017DC)(self->spr2, (int)(255.0f * self->fD0), &col, 255);
        }
        CALL(gui_drawSpriteIA8Clip_00002BB0)(self->spr2, self->w48 + i * self->w58 - 12, self->w4C - 12, 926, 20);
    }
    CALL(gui_spriteRefresh)(self->spr2);
    wheel = self->pos;
    x0 = (int)((float)self->w38 - (wheel - (float)(int)wheel) * (float)self->w54);
    CALL(gui_setScissor)(&ne_gGui, 40, 0, 284, 239); /* gui_setScissor */
    for (j = -self->w50; j <= self->w50; j++) {
        k = (int)self->pos + j;
        if (k < 0) {
            k += 42;
        }
        if (k >= 42) {
            k -= 42;
        }
        x = x0 + j * self->w54;
        fx = (float)x;
        if (160.0f < fx) {
            dist = -(160.0f - fx);
        } else {
            dist = 160.0f - fx;
        }
        d = dist / 160.0f;
        a = 1.0f - d;
        if (d > 1.0f) {
            a = 0.0f;
        }
        alpha = (int)(64.0f * a * self->fD0);
        if (alpha < 0) {
            alpha = 0;
        }
        if (a > 0.92f) {
            alpha = 255;
            sx = 1.2f;
            sy = 1.2f;
        } else {
            sx = 1.0f;
            sy = 1.0f;
        }
        CALL(gui_setColorEnvPrim_000017DC)(self->spr2, alpha, &col, 255);
        ne_gui_drawSpriteScroll(self->spr2, x, self->w3C, k * 22 + 2, 20, sx, sy);
    }
    CALL(gui_setScissor)(&ne_gGui, 0, 0, 319, 239);
}

/* NameEnterMessage_update: the name-entry input: the letter wheel (42 entries: letters, 39 =
 * space, 40 = back, 41 = END) steps with 0x10000 / 0x4000 or the stick (auto-repeat), the
 * position eases toward the target (gain +0x7C, clamped to +-maxVel, wrapping at 42); 0x200
 * deletes a letter, A / 0x40000 picks the letter under the wheel (END fades out with
 * owner->entered set). Once 3 letters are in, the wheel is held on END. The 0.05f literal is
 * RoData+0x10 (NameEnterMessage_update_rodata pinned). The dead stores to `scratch1` /
 * `scratch2` emit no code; their words give the target's frame (0x30: sel at 0x24, stick 0x1C). */
extern "C" void NameEnterMessage_update(NeEntry *self) {
    float d;
    float tgt;
    int sel;
    int scratch1;
    float stick;
    int scratch2;

    scratch1 = 0;
    scratch2 = 0;
    if (self->done != 0 && self->target < 41.0f) {
        self->target = 41.0f;
        tgt = self->target;
    } else {
        if (self->done != 0 && self->pos >= 41.0f) {
            self->done = 0;
        }
        stick = ne_input_getStickX(&D_00000000);
        if (self->repeat < self->repeatDelay) {
            if (CALL(input_checkPressed_00011E00)(&D_00000000, 0x10000)) { /* input_checkPressed_00011E00 */
                self->target -= 1.0f;
                if (self->len == 3 && self->target < 40.0f) {
                    self->target = 40.0f;
                } else {
                    XCALL(gl_func_00000000_void, snd_postEvent)(157); /* snd_postEvent */
                }
            }
            if (CALL(input_checkPressed_00011E00)(&D_00000000, 0x4000)) {
                self->target += 1.0f;
                if (self->len == 3 && self->target >= 42.0f) {
                    self->target = 41.0f;
                } else {
                    XCALL(gl_func_00000000_void, snd_postEvent)(157);
                }
            }
        }
        if (stick == 0.0f) {
            self->repeat = 0;
            self->repeatTick = 0;
        } else {
            self->repeat++;
            if (self->repeat > self->repeatDelay) {
                self->repeatTick++;
                if (self->repeatTick > self->repeatRate) {
                    self->repeatTick = 0;
                    if (stick < 0.0f) {
                        self->target -= 1.0f;
                        if (self->len == 3 && self->target < 40.0f) {
                            self->target = 40.0f;
                        } else {
                            XCALL(gl_func_00000000_void, snd_postEvent)(157);
                        }
                    }
                    if (stick > 0.0f) {
                        self->target += 1.0f;
                        if (self->len == 3 && self->target >= 42.0f) {
                            self->target = 41.0f;
                        } else {
                            XCALL(gl_func_00000000_void, snd_postEvent)(157);
                        }
                    }
                }
            }
        }
        if (self->target < 0.0f) {
            self->target += 42.0f;
        }
        if (self->target >= 42.0f) {
            self->target -= 42.0f;
        }
        tgt = self->target;
    }
    d = tgt - self->pos;
    if (d > 21.0f) {
        d -= 42.0f;
    } else if (d < -21.0f) {
        d += 42.0f;
    }
    if ((d < 0.0f ? -d : d) < 0.05f) {
        self->pos = tgt;
    } else {
        self->vel = d * self->gain;
        if (self->vel > self->maxVel) {
            self->vel = self->maxVel;
        } else if (self->vel < -self->maxVel) {
            self->vel = -self->maxVel;
        }
        self->pos += self->vel;
    }
    if (self->pos < 0.0f) {
        self->pos += 42.0f;
    }
    if (self->pos >= 42.0f) {
        self->pos -= 42.0f;
    }
    sel = (int)(self->pos + 0.5f);
    if (self->done == 0 && self->len > 0 && CALL(input_checkPressed_00011E00)(&D_00000000, 0x200)) {
        XCALL(gl_func_00000000_void, snd_postEvent)(168);
        self->done = 0;
        self->len--;
        self->name2[self->len] = 42;
    }
    if (self->done == 0 && CALL(input_checkPressed_00011E00)(&D_00000000, 0x40100)) {
        if (sel == 41) {
            if (CALL(Fader_isDone_00013980)(D_00000000.fader)) { /* Fader_isDone_00013980 */
                XCALL(gl_func_00000000_void, snd_postEvent)(160);
                ((NeOwner *)self->owner)->entered = 1;
            }
            return;
        }
        if (sel == 40) {
            if (self->len > 0) {
                XCALL(gl_func_00000000_void, snd_postEvent)(168);
                self->len--;
                self->name2[self->len] = 42;
            }
            return;
        }
        if (sel == 39) {
            if (self->len < 3) {
                XCALL(gl_func_00000000_void, snd_postEvent)(167);
                self->name2[self->len] = 42;
                self->len++;
            }
        } else {
            if (self->len < 3) {
                XCALL(gl_func_00000000_void, snd_postEvent)(167);
                self->name2[self->len] = self->pos + 0.5f;
                if (self->name2[self->len] >= 42) {
                    self->name2[self->len] -= 42;
                }
                self->len++;
            }
        }
        if (self->len == 3) {
            self->done = 1;
        }
    }
}
