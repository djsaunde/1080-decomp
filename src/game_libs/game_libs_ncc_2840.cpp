/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00002840 (0x1B8 / 110 words), spliced over a placeholder in both
 * build trees (agent-g run 277). History: NM comparison donor since run 139 (cfe 89.75 -> NCC 90.81), 99.97 at
 * run 276 with every home on the target except p (spill word sp+0x20, target sp+0x28).
 * agent-g run 277: p is an ADDRESS-TAKEN local in the target. The words below p are not temps: with p scalar
 * (copy-propagated, its uses become the item arg0 + 0x2E4) the value lives in a uopt spill temp placed directly
 * under the locals, so its slot only follows the local count (frame = 24 + locals + temps, rounded to 8; the
 * &vec argument adds one unused word). With the code-free `if (&p == 0) {}` p stays a memory variable stored to
 * its OWN home after the computation, the call-1 argument still forwarded in a0 and the value reloaded after
 * each call: p declared after f18 (home M-72 = sp+0x28), one pad word where p was (keeps b4 / d4 / dx / dy at
 * M-28 / M-32 / M-52 / M-56) and one after it (frame 0x70). Memory p also needs its assignment AFTER
 * vec[0] = 1.0f: before it, uopt hoists arg0 + 0x2E4 above the len != 0 branch; after it, the target's
 * join block (arg0 reload in the bc1tl delay, lui for D + 0xCB4 above the branch). 99.97 -> 100.
 * The run-139 .. 276 levers below are unchanged. */
typedef int s32;
typedef float f32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
extern f32 gl_func_00000000_sqf(f32);
}
extern "C" void gl_func_00002840(char *arg0, s32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    /* agent-g run 263: declaration order from a random home-layout search (the volatile pads are
     * unreferenced home-keeping words)
     * agent-g run 273: pair-swap hill climb on the order (dy <-> f0, then d4 <-> temp_v0): dy's home at 56 and
     * d4's at 80 = target (95.23 -> 95.26)
     * agent-g run 277: p moved after f18 (home sp+0x28 = target) with pad words pz / py */
    f32 vec[4];
    volatile int pz;
    char *temp_v0;
    f32 b4; /* agent-g run 271: moved up (single-move decl sweep): b4's home at 84 = target */
    f32 d4;
    f32 f0;
    volatile int u1;
    f32 len;
    volatile int u2;
    f32 dx;
    f32 dy;
    f32 f16;
    f32 f2;
    f32 f18;
    char *p;
    volatile int py;

    temp_v0 = (char *)FW(FW(((*(s32*)((char*)&D_00000000 + 0x134)) + (arg1 * 4)), 0x108), 0x70);
    f0 = (*(f32*)((char*)temp_v0 + 0xBC));
    f16 = (*(f32*)((char*)temp_v0 + 0xA0));
    f2 = (*(f32*)((char*)temp_v0 + 0xDC));
    f18 = (*(f32*)((char*)temp_v0 + 0xA8)); if (f0 != f2) {} if (f0 != f2) {} /* agent-g run 271 (kit CCCXX priorities): two code-free tests on f0 / f2 (was one on f16 / f18, run 263) -- f0 / f2 colour before the two CSE'd sums, which then take f12 / f14 and leave f16 / f18 to 0xA0 / 0xA8 like the target: 94.48 -> 95.21 */
    if ((((f16 * f0) + (f18 * f2)) < ((f0 * arg2) + (f2 * arg3))) && ((((f0 * arg2) + (f2 * arg3)) - ((f16 * f0) + (f18 * f2))) < 4000.0f)) {
        b4 = (*(f32*)((char*)temp_v0 + 0xB4));
        /* agent-g run 276: d4 read right after b4 and WITHOUT the run-263 code-free `if (d4 != dx) {}` -- that
         * test kept a coloured piece of d4 (f0, target reads it into the ugen temp f6), which put the ugen FP
         * ring one step off for dx * dx / dy * dy and everything after; read before dx / dy, its store frees
         * f6 for dy * dy as in the target (statement-order grid b4 / d4 / dx / dy): 95.26 -> 99.88 */
        d4 = (*(f32*)((char*)temp_v0 + 0xD4));
        /* agent-g run 263: plain dx / dy variables (the target keeps both in their homes across the
         * calls); `dx = arg2 - f16` alone leaves the item arg2 - f16 coloured and copied into dx (spill
         * temps 28 / 32) -- the dead parameter stores arg2 = dx / arg3 = dy kill the items (no code):
         * the run-139 volatile sp38 / sp3C copies go (92.82 -> 94.00, + home search 94.18) */
        dx = arg2 - f16; arg2 = dx;
        dy = arg3 - f18; arg3 = dy;
        len = gl_func_00000000_sqf((dx * dx) + (dy * dy));
        if (len != 0.0f) {
            dx /= len;
            dy /= len;
        }
        if (&p == 0) {} /* agent-g run 277: code-free test on p's address: p is a memory variable (home sp+0x28) */
        vec[0] = 1.0f;
        p = arg0 + 0x2E4; /* agent-g run 277: after vec[0] (before it, uopt hoists the add above the branch) */
        vec[2] = 0.0f;
        vec[3] = 0.0f;
        vec[1] = (*(f32*)((char*)&D_00000000 + 0xCB4));
        gl_func_00000000(p);
        gl_func_00000000(p, (s32) (255.0f * (*(f32*)&D_00000000)), vec, 0xFF);
        /* agent-g run 276: the (f32) term first (target add.s f8,f10,f6 = cvt + product): 99.88 -> 99.97 */
        gl_func_00000000(p, (s32) ((f32) (FW(arg0, 0x164) - 0xC) + (((b4 * dx) + (d4 * dy)) * 150.0f)), arg4 - 8, 0);
    }
}
