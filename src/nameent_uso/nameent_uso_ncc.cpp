/* IDO 7.1 NCC (C++) EXACT donors for nameent.uso (agent-c run 643), REPLACE_FUNC_BODY-spliced over
 * the empty placeholders in nameent_uso.c on both build paths (Makefile NE_NCC_DONOR).
 * nameent.uso is C++: these are the functions whose shape only edg's lowering reproduces (a
 * virtual call keeps the object in v1 and its vtable in v0). */
extern "C" int gl_func_00000000(...);

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
    int nameWord;               /* 0x15C */
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

/* nameent_uso_func_000002F4: board draw: the name-board image (a copy of the Data+0x158 header
 * with the current rider's board texture, virtual slot 11) centred on the cursor x at y 40 in
 * white, then the two black letterbox bars. The dead one-element stores to `scratch1` /
 * `scratch2` emit no code; their frame words give the target's frame (0x80: x at 0x7C, col at
 * 0x64, img at 0x4C, black at 0x3C). */
extern "C" void nameent_uso_func_000002F4(NeProc *self) {
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
    gl_func_00000000(&img); /* gui_beginFrame_000016A4 */
    x = self->cursorX - img.width / 2;
    gl_func_00000000(&img, 128, &col, 255); /* gui_setColorEnvPrim_00002334 */
    gl_func_00000000(&img, x, 40); /* gui_drawImage_00003B80 */
    black.set(0.0f, 0.0f, 0.0f, 0.0f);
    gl_func_00000000(&ne_gGui, 255, &black); /* gui_setFillColor_00001CA8 */
    gl_func_00000000(&ne_gGui, 0, 0, 319, 7, 0x10001); /* gui_fillRect */
    gl_func_00000000(&ne_gGui, 0, 232, 319, 239, 0x10001); /* gui_fillRect */
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
    int w38, w3C, w40, w44, w48, w4C, w50, w54, w58, w5C, w60, w64, w68; /* 0x38..0x68 */
    float f6C, f70, f74, f78, f7C;  /* 0x6C..0x7C */
    Colour c80, c90, cA0;       /* 0x80, 0x90, 0xA0 */
    int nameWord;               /* 0xB0: GameFlow +0x15C */
    int wB4;                    /* 0xB4 */
    unsigned char name2[12];    /* 0xB8: GameFlow +0x158.. */
    int wC4, wC8, wCC;          /* 0xC4..0xCC */
    float fD0;                  /* 0xD0 */
    char spr0[0x18];            /* 0xD4: gui sprites */
    char spr1[0x18];            /* 0xEC */
    char spr2[0x18];            /* 0x104 */
};

/* nameent_uso_func_00000760: the name-entry ctor (`this == 0` allocates 0x11C; the inlined Node
 * base ctor, then the layout constants, the last name from GameFlow +0x158, the colours and three
 * gui sprites). The 0.2f literal is RoData+0 (nameent_uso_func_00000760_rodata pinned at 0). */
extern "C" NeEntry *nameent_uso_func_00000760(NeEntry *self, void *owner, int mode) {
    int i;
    char *o;
    if (self != 0 || (self = (NeEntry *)gl_func_00000000(0x11C)) != 0) { /* mem_new */
        o = (char *)self;
        if (o != 0 || (o = (char *)gl_func_00000000(0x2C)) != 0) {
            gl_func_00000000(o, ne_data + 0x170); /* Node_ctor */
            *(void **)(o + 0x28) = &ne_vt_node;
        }
        self->vptr = &ne_vt_entry;
        self->name = ne_data + 0x178;
        self->mode = mode;
        self->owner = owner;
        if (mode != 0) {
            self->newCount = gl_func_00000000(self->mode); /* Backup_sessionCountNew */
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
        self->w5C = 0;
        self->w64 = 6;
        self->w60 = 0;
        self->w68 = 1;
        self->f74 = 0.0f;
        self->f78 = 0.5f;
        self->f7C = 0.2f;
        self->wB4 = 0;
        self->wC4 = 128;
        self->wC8 = 255;
        self->wCC = 255;
        self->nameWord = D_00000000.nameWord;
        for (i = 0; i < 9; i++) {
            self->name2[i] = D_00000000.name[i];
        }
        if (self->nameWord != 0) {
            self->f70 = 41.0f;
        } else {
            self->f70 = 0.0f;
        }
        self->f6C = self->f70;
        self->c80.set255(128, 255, 128, 255);
        self->c90.clear();
        self->cA0.set255(128, 128, 128, 255);
        self->fD0 = 0.0f;
        gl_func_00000000(self->spr0, (ne_gRowBase + 4) | 0x250000); /* gui_spriteInit */
        gl_func_00000000(self->spr1, 0x250003);
        gl_func_00000000(self->spr2, 0x250000);
    }
    return self;
}
