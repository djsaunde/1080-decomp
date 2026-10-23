/* IDO 7.1 NCC (C++) NM donor for func_0001304C (fire-grid blur + texture
 * upload display list, 0x8D8 / 566 words). Shape- and frame-exact (noreg 0);
 * 42 raw words left, all in the display-list tail: the target's per-packet
 * pointer rotation is a1 a2 a3 a1 a2 t0 ... where ours is a strict a1 a2 a3, which
 * shifts the hoisted constants by one register. Recipe (agent-c run 125):
 * - declaration order x, y, i, j / up, dn / k / lf, rt / count = frame 96, k at 68;
 * - `if (f->timer != 0) f->timer--; if (f->timer == 0)` = the t6 decrement temp;
 * - centre cell through i (strength-reduced 4*i), texel through j = i copied at
 *   the top of the x loop, `i++` at the end (the early a3 increment + t1 copy);
 * - `for (count = 0, y = 0; ...)` and `for (x = 0, i = y * 32; ...)` set the
 *   init order; next() with a named result `Gfx *g` places the constants.
 * Spliced into the NON_MATCHING comparison object only. */
typedef struct { unsigned int w0, w1; } Gwords;
typedef union { Gwords words; long long force_structure_alignment; } Gfx;
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; Gfx *g = &dl->buf[n]; return g; }
};
#define G2(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (unsigned int)(a); _g->words.w1 = (unsigned int)(b); }
struct Fire {
    char pad[0x40];
    int state;          /* 0x40 */
    int pad44;
    float f48;          /* 0x48 */
    void *tex0;         /* 0x4C */
    unsigned char *tex1; /* 0x50 */
    int pad54;
    float *grid;        /* 0x58 */
    char pad5C[0x68 - 0x5C];
    int flag68;         /* 0x68 */
    int timerOn;        /* 0x6C */
    int timer;          /* 0x70 */
    int frame;          /* 0x74 */
};
extern "C" {
void func_00000000(...);
extern char D_00000000[];
extern float D_00000C50, D_00000C54, D_00000C58, D_00000C5C, D_00000C60;
extern char func_0000023C[];
}
extern "C" void func_0001304C(Fire *f) {
    int x, y, i, j;
    float up, dn;
    float k;
    float lf, rt;
    int count;
    if (f->state == 0) return;
    f->frame++;
    if (f->timerOn) {
        if (f->timer != 0) f->timer--;
        if (f->timer == 0) {
            func_00000000(D_00000000, 0);
            f->timerOn = 0;
            if (f->state == 2) {
                f->state = 0;
                f->f48 = 1.0f;
                return;
            }
        }
    }
    func_00000000(f);
    if (f->state == 2) k = D_00000C50;
    if (f->state == 3) {
        if (f->flag68) k = D_00000C54;
        else k = D_00000C58;
    }
    if (f->state == 1) {
        if (f->flag68) k = D_00000C5C;
        else k = D_00000C60;
    }
    for (count = 0, y = 0; y != 32; y++) {
        for (x = 0, i = y * 32; x != 32; x++) {
            j = i;
            if (y - 1 < 0) up = 0.0f;
            else up = f->grid[((y - 1) & 31) * 32 + x];
            if (y == 31) dn = 0.0f;
            else dn = f->grid[((y + 1) & 31) * 32 + x];
            if (x - 1 < 0) lf = 0.0f;
            else lf = f->grid[y * 32 + ((x - 1) & 31)];
            if (x == 31) rt = 0.0f;
            else rt = f->grid[y * 32 + ((x + 1) & 31)];
            f->grid[i] += (up + dn + lf + rt) * k;
            if (f->grid[i] > 1.0f) {
                f->grid[i] = 1.0f;
                count++;
            }
            f->tex1[j] = (unsigned int)(((f->state == 2 || f->state == 3) ? f->grid[i] : 1.0f - f->grid[i]) * 255.0f);
            i++;
        }
    }
    if (f->state == 3 && count > 1024) f->f48 = 1.0f;
    if (count == 1024) {
        f->f48 = 1.0f;
        if (f->state == 1) f->state = 0;
    }
    Ctx *c = *(Ctx **)(*(char **)(func_0000023C + 0x18) + 0x158);
    G2(c->next(), 0xE7000000, 0);
    G2(c->next(), 0xBA001402, 0x100000);
    G2(c->next(), 0xBA001301, 0);
    G2(c->next(), 0xB900031D, 0x504240);
    G2(c->next(), 0xFCFFFFFF, 0xFFFCF438);
    G2(c->next(), 0xBA000C02, 0x2000);
    if (f->flag68 == 0) G2(c->next(), 0xBA000602, 0xC0);
    G2(c->next(), 0xF8000000, 0xFFFFFFFF);
    G2(c->next(), 0xFD100000, f->tex0);
    G2(c->next(), 0xF5100000, 0x07014050);
    G2(c->next(), 0xE6000000, 0);
    G2(c->next(), 0xF3000000, 0x073FF100);
    G2(c->next(), 0xE7000000, 0);
    G2(c->next(), 0xF5101000, 0x00014050);
    G2(c->next(), 0xF2000000, 0x0007C07C);
    G2(c->next(), 0xFD900000, f->tex1);
    G2(c->next(), 0xF5900100, 0x07014050);
    G2(c->next(), 0xE6000000, 0);
    G2(c->next(), 0xF3000000, 0x071FF200);
    G2(c->next(), 0xE7000000, 0);
    G2(c->next(), 0xF5880900, 0x01014050);
    G2(c->next(), 0xF2000000, 0x0107C07C);
    G2(c->next(), 0xBA001001, 0);
    G2(c->next(), 0xE45003C0, 0);
    G2(c->next(), 0xB4000000, 0);
    G2(c->next(), 0xB3000000, 0x00630084);
    G2(c->next(), 0xE7000000, 0);
    G2(c->next(), 0xBA001301, 0x80000);
}
