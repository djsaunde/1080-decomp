/* IDO 7.1 NCC (C++) NM-only comparison donor for func_0000C234: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 77.39; a second parameter empty test from round 2 gives 79.35).
 * agent-g run 152 (79.55 -> 85.17): arg 2 of the 0x38 lookup call is a 4-byte struct BY VALUE (target `sw a1,140(sp)` + `sw a1,4(sp)` in the jal delay), the
 * counter test is a post-increment compare on the old value (`slti v1,151`), the func_00008A7C sites are per-site symbols (target re-materialises
 * lui/addiu per call, one shared symbol is CSE-spilled to sp+52), the 0x2C mode word reads through a data symbol (lw %lo folded), sp40 declared first.
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
inline void zf(float &f, int z) { f = z; }
#undef NULL
#define NULL 0
typedef struct { f32 x, y, z; } V3C234;
typedef struct {
     s32 unk0;
     s32 unk4;
     s32 unk8;
     f32 d8;
     s16 dc;
     s16 de;
     s32 e0;
     f32 e4;
     f32 e8;
     f32 ec;
     f32 f0;
} C234Rec;
extern "C" char func_00008A40[];
struct S4C234 { s32 v; };
extern "C" int bu_c234_sv(s32, S4C234, char *, int);
extern "C" {
extern float D_bu_ro_pool[];
extern double D_bu_ro_dpool[];
extern f32 D_00000128, D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, D_00000938, D_0000093C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, D_00000938, D_0000093C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, D_00000938, D_0000093C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, D_00000938, D_0000093C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, D_00000868, D_0000086C, D_00000870, D_00000874, D_00000878, D_0000087C, D_00000880, D_00000884, D_00000888, D_0000088C, D_00000890, D_00000894, D_00000898, D_0000089C, D_000008A0, D_000008A4, D_000008A8, D_000008AC, D_000008B0, D_000008B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, D_00000938, D_0000093C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern char D_00000000;
extern void bu_b75c_mvf(char *, char *, f32, char *);
extern int func_00000000(...);
extern void func_00000008(...);
extern float func_411c_rf(void);
extern void func_00008A7C(...);
extern char bu_c234_g8[];
extern void bu_c234_v5(char *, char *, f64, f64, f64);
extern char bu_c234_k0[], bu_c234_k1[], bu_c234_k2[], bu_c234_k3[], bu_c234_k4[];
extern void func_00000000_p4d(void *, s32, f64, f32);
extern void func_00000000_p4d(void *, s32, f64, f32);
extern s32 unk0;
extern s32 unk4;
extern s32 unk8;
extern f32 d8;
extern s16 dc;
extern s16 de;
extern s32 e0;
extern f32 e4;
extern f32 e8;
extern f32 ec;
extern f32 f0;
extern f64 D_00000930;
extern s32 D_000081C0[];
}
extern "C" void func_0000C234(char *arg0) {
    V3C234 sp40;
    C234Rec spCC;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f12_2;
    V3C234 spB4; /* agent-g run 195 (declmove sweep): below temp_f12_2 */
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_3;
    f32 var_f2;
    f32 var_f2_2;
    S4C234 sp8C;
    s16 temp_v0_2;
    V3C234 sp7C;
    s32 temp_a1_2;
    V3C234 sp68;
    s32 temp_v0;
    s32 temp_v0_4;
    V3C234 sp54;
    s32 temp_v1_2;
    s32 var_t0;
    char *sp3C;
    s32 var_v0;
    char *temp_a1;
    char *temp_v0_3;
    char *var_v0_2;


    temp_v0 = *(s32 *)((char *)(arg0) + 0xA80);
    if (temp_v0 != 0) {
        *(s32 *)((char *)(arg0) + 0xA80) = (s32) (temp_v0 - 1);
    }
    spB4 = *(V3C234 *)((char *)(arg0) + 0xB4);
    temp_a1 = arg0 + 0xDC;
    *(f32 *)((char *)(arg0) + 0x890) = spB4.x;
    *(f32 *)((char *)(arg0) + 0x894) = spB4.y;
    *(f32 *)((char *)(arg0) + 0x898) = spB4.z;
    spB4 = *(V3C234 *)((char *)(arg0) + 0x318);
    *(f32 *)((char *)(arg0) + 0x89C) = spB4.x;
    *(f32 *)((char *)(arg0) + 0x8A0) = spB4.y;
    *(f32 *)((char *)(arg0) + 0x8A4) = spB4.z;
    spCC.f0 = 0.0f;
    spCC.ec = 0.0f;
    spCC.e4 = 0.0f;
    spCC.e8 = 1.0f;
    *(f32 *)((char *)(arg0) + 0x9D0) = D_bu_ro_pool[0x920 / 4]; /* agent-g run 231: above the 0x9A2 store (as1 line order: the pool load issues before the spCC stores) */
    *(s16 *)((char *)(arg0) + 0x9A2) = 0; if (arg0) {}
    sp3C = temp_a1;
    if (func_00000000(*(s32 *)((char *)&D_00000000 + 0), temp_a1, 0x457A0000, &spCC) != 0) {
        *(s16 *)((char *)(arg0) + 0x9A0) = func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.dc);
        if (spCC.d8 < D_bu_ro_pool[0x924 / 4]) {
            *(s16 *)((char *)(arg0) + 0x9A2) = *(s16 *)((char *)(arg0) + 0x9A0);
        } else {
            *(s16 *)((char *)(arg0) + 0x9A2) = 0;
        }
        *(s32 *)((char *)(arg0) + 0x9A8) = func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.dc);
        *(f32 *)((char *)(arg0) + 0x9D0) = spCC.d8;
        /* agent-g run 231: the plane distance on its own line before the test (spCC.f0 then loads after it, as1 line
         * order), operands K * (dc * e4 + (e0 + K2) * e8 + e4' * ec) + 4 in the target's load / multiply order: 95.45 -> 98.17 */
        temp_f16 = 4.0f + (D_bu_ro_pool[0x128 / 4] * (((*(f32 *)((char *)(arg0) + 0xDC) * spCC.e4) + ((*(f32 *)((char *)(arg0) + 0xE0) + D_bu_ro_pool[0x928 / 4]) * spCC.e8) + (*(f32 *)((char *)(arg0) + 0xE4) * spCC.ec))));
        if ((spCC.f0 < temp_f16) || (*(s32 *)((char *)(arg0) + 0x938) != 0)) {
            *(s32 *)((char *)(arg0) + 0xA4C) = 1;
            spB4 = *(V3C234 *)&spCC.e4;
            var_t0 = 0;
            *(f32 *)((char *)(arg0) + 0x9D8) = spB4.x;
            *(f32 *)((char *)(arg0) + 0x9DC) = spB4.y;
            *(f32 *)((char *)(arg0) + 0x9E0) = spB4.z;
            *(f32 *)((char *)(arg0) + 0x9E4) = spCC.f0;
            if (D_00000930 < (f64) *(f32 *)((char *)(arg0) + 0x9DC)) {
                var_t0 = 1;
            }
            *(s32 *)((char *)(arg0) + 0xA50) = var_t0;
        } else {
            *(s32 *)((char *)(arg0) + 0xA4C) = 0;
            *(f32 *)((char *)(arg0) + 0x9D8) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x9E0) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x9E4) = (f32) 0;
            *(s32 *)((char *)(arg0) + 0xA50) = 0;
            *(f32 *)((char *)(arg0) + 0x9DC) = 1.0f;
        }
        if ((*(f32 *)((char *)(arg0) + 0x9D0) < (f32) 0) && (spCC.e8 > (f32) 0)) {
            *(f32 *)((char *)(arg0) + 0x31C) = 10.0f;
        }
        if ((*(s32 *)((char *)(arg0) + 0x938) != 0) && (*(f32 *)((char *)(arg0) + 0x348) > 10.0f)) {
#define temp_v1 (*(s16 *)((char *)(arg0) + 0x9A0)) /* run 184: de-named (lever-sweep) */
            var_v0 = 0;
            if (temp_v1 == 0x64) {
                *(s32 *)((char *)(arg0) + 0xA5C) = 0x64;
                var_v0 = 2;
            } else if (temp_v1 == 0x63) {
                *(s32 *)((char *)(arg0) + 0xA5C) = 0x46;
                var_v0 = 1;
            }
            if (var_v0 != 0) {
                temp_f2 = *(f32 *)((char *)(arg0) + 0x970);
                if (temp_f2 < 0.0f) {
                    temp_f0 = -temp_f2;
                } else {
                    temp_f0 = temp_f2;
                }
                func_00000000(*(s32 *)((char *)(arg0) + 0x800), D_000081C0[(s32) ((f32) var_v0 + (temp_f0 * 3.0f))], 2);
            }
        }
        /* Run 109 (agent-c, 69.25 -> 70.94): target block order is default,
         * 0x61, 0x62 (flat goto ladder); the 0x940/0x944/0x948 pool reads are
         * per-address scalars (D_bu_ro_pool[] here CSEs the base, -0.1). */
        temp_v0_2 = *(s16 *)((char *)(arg0) + 0x9A0);
        if (temp_v0_2 == 0x61) goto c61;
        if (temp_v0_2 == 0x62) goto c62;
        *(f32 *)((char *)(arg0) + 0x288) = (f32) D_00000938;
        *(f32 *)((char *)(arg0) + 0x28C) = (f32) D_0000093C;
        *(f32 *)((char *)(arg0) + 0x290) = D_00000940; /* run 200: 288 / 28C / 290 together in both arms (statement-order grid) */
        *(u8 *)((char *)(arg0) + 0xA33) = 0;
        *(u8 *)((char *)(arg0) + 0xA32) = 0;
        *(u8 *)((char *)(arg0) + 0xA31) = 0;
        *(u8 *)((char *)(arg0) + 0xA30) = 0;
        *(f32 *)((char *)(arg0) + 0x910) = (f32) *(f32 *)((char *)(arg0) + 0x5A0);
        goto cend;
c61:
        *(f32 *)((char *)(arg0) + 0x288) = (f32) *(f32 *)((char *)(arg0) + 0x120);
        *(f32 *)((char *)(arg0) + 0x28C) = (f32) *(f32 *)((char *)(arg0) + 0x138);
        *(f32 *)((char *)(arg0) + 0x290) = (f32) *(f32 *)((char *)(arg0) + 0x1B0);
        *(f32 *)((char *)(arg0) + 0x910) = (f32) *(f32 *)((char *)(arg0) + 0x570);
        func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.de, arg0 + 0xA30);
        goto cend;
c62:
        *(f32 *)((char *)(arg0) + 0x288) = 0.0f;
        *(f32 *)((char *)(arg0) + 0x28C) = D_00000944;
        *(f32 *)((char *)(arg0) + 0x290) = D_00000948;
        *(f32 *)((char *)(arg0) + 0x910) = (f32) *(f32 *)((char *)(arg0) + 0x588);
        func_00000000(*(s32 *)((char *)((*(s32 *)((char *)&D_00000000 + 0))) + 0x84), spCC.de, arg0 + 0xA30);
cend:
        *(s32 *)((char *)(arg0) + 0x95C) = 0;
    } else {
        *(s32 *)((char *)(arg0) + 0xA4C) = 0;
        *(f32 *)((char *)(arg0) + 0x9D8) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x9E0) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x9E4) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x9DC) = 1.0f;
        if ((*(s32 *)((char *)(arg0) + 0x95C))++ >= 0x97) {
            sp8C.v = *(s32 *)((char *)&func_00008A40 + 0x38);
            temp_v0_3 = (char *)(bu_c234_sv(*(s32 *)((char *)&D_00000000 + 0), sp8C, sp3C, 0));
            if (temp_v0_3 != 0) {
                sp7C = *(V3C234 *)((char *)(temp_v0_3) + 0x30);
                sp7C.y += 200.0f;
                func_00000000(arg0, &sp7C);
                *(s32 *)((char *)(arg0) + 0x95C) = 0;
                func_00000000(bu_c234_k0 + 0);
                bu_c234_v5(bu_c234_k1 + 0x18, bu_c234_k2 + 0x14, (f64) sp7C.x, (f64) sp7C.y, (f64) sp7C.z);
            }
        }
    }
    *(f32 *)((char *)(arg0) + 0x968) = 0.0f;
    *(s32 *)((char *)(arg0) + 0x938) = 0;
    zf(*(f32 *)((char *)(arg0) + 0x93C), 0); /* run 196: int-zero inline (kit CLVIII): the target's own f0 zero */
    zf(*(f32 *)((char *)(arg0) + 0x940), 0);
    zf(*(f32 *)((char *)(arg0) + 0x944), 0);
    func_00000000(bu_c234_k3 + 0x28);
    func_00000000(arg0);
    func_00000000(bu_c234_k4 + 0x34);
    if (*(s32 *)((char *)(arg0) + 0x9A8) & 0x400) {
        func_00000000(arg0 + 0x808, 1);
        var_f2 = D_bu_ro_pool[0x94C / 4];
    } else {
        func_00000000(arg0 + 0x808, 0);
        var_f2 = 1.0f;
    }
    if (*(s32 *)(bu_c234_g8 + 0x2C) != 2) {
        temp_f0 = *(f32 *)((char *)(arg0) + 0x91C);
        *(f32 *)((char *)(arg0) + 0x91C) = (f32) ((f64) temp_f0 + ((f64) (var_f2 - temp_f0) * D_bu_ro_dpool[0x950 / 8]));
        func_00000000(*(s32 *)((char *)(arg0) + 0x850), *(s32 *)((char *)(arg0) + 0x91C));
    }
    temp_v0_4 = *(s32 *)((char *)(arg0) + 0x938);
    if (temp_v0_4 != 0) {
#define temp_f2_2 ((f32) temp_v0_4) /* run 184: de-named (lever-sweep) */
        var_v0_2 = arg0 + 0x93C;
        *(f32 *)((char *)(arg0) + 0x93C) = (f32) (*(f32 *)((char *)(arg0) + 0x93C) / temp_f2_2);
        *(f32 *)((char *)(arg0) + 0x940) = (f32) (*(f32 *)((char *)(arg0) + 0x940) / temp_f2_2);
        *(f32 *)((char *)(arg0) + 0x944) = (f32) (*(f32 *)((char *)(arg0) + 0x944) / temp_f2_2); /* run 200: 0x944 before 0x968 (target load order; div block word-for-word) */
        *(f32 *)((char *)(arg0) + 0x968) = (f32) (*(f32 *)((char *)(arg0) + 0x968) / temp_f2_2); if (1) {}
        temp_f12 = *(f32 *)((char *)(arg0) + 0x968);
        if (temp_f12 < (f32) 0) {
            var_v0_2 = arg0 + 0x93C; if (1) {} /* run 182 (lever-sweep bbsplit, +1 word toward the target) */
            { /* run 156: held products, stored afterwards (kit LXXXVI) */
                f32 hx = *(f32 *)((char *)(arg0) + 0x93C) * temp_f12;
                f32 hy = *(f32 *)((char *)(var_v0_2) + 0x4) * temp_f12;
                f32 hz = *(f32 *)((char *)(var_v0_2) + 0x8) * temp_f12;
                sp68.x = hx; sp68.y = hy; sp68.z = hz;
            }
            spB4 = sp68;
            sp54 = spB4;
            *(f32 *)((char *)(arg0) + 0x920) = sp54.x;
            *(f32 *)((char *)(arg0) + 0x924) = sp54.y;
            *(f32 *)((char *)(arg0) + 0x928) = sp54.z;
        } else {
            *(f32 *)((char *)(arg0) + 0x920) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x924) = (f32) 0;
            *(f32 *)((char *)(arg0) + 0x928) = (f32) 0;
        }
        spB4 = *(V3C234 *)((char *)(var_v0_2) + 0x0);
        *(f32 *)((char *)(arg0) + 0x948) = spB4.x;
        *(f32 *)((char *)(arg0) + 0x94C) = spB4.y;
        *(f32 *)((char *)(arg0) + 0x950) = spB4.z;
        var_f2_2 = (*(f32 *)((char *)(arg0) + 0x33C) * *(f32 *)((char *)(arg0) + 0x93C)) + (*(f32 *)((char *)(arg0) + 0x340) * *(f32 *)((char *)(arg0) + 0x940)) + (*(f32 *)((char *)(arg0) + 0x344) * *(f32 *)((char *)(arg0) + 0x944));
        if (var_f2_2 > 0.0f) {
            var_f2_2 = 0.0f;
        }
        temp_f0 = -var_f2_2; if (1) {} /* run 182 (lever-sweep bbsplit) */
        { /* run 156: held products */
            f32 hx = *(f32 *)((char *)(var_v0_2) + 0x0) * temp_f0;
            f32 hy = *(f32 *)((char *)(var_v0_2) + 0x4) * temp_f0;
            f32 hz = *(f32 *)((char *)(var_v0_2) + 0x8) * temp_f0; /* run 200: no longer volatile (run 182's +2-word lever is surplus after the 0x944 order fix) */
            sp40.x = hx; sp40.y = hy; sp40.z = hz;
        }
        spB4 = sp40;
        sp54 = spB4;
        *(f32 *)((char *)(arg0) + 0x92C) = sp54.x;
        *(f32 *)((char *)(arg0) + 0x930) = sp54.y;
        *(f32 *)((char *)(arg0) + 0x934) = sp54.z;
        *(s32 *)((char *)(arg0) + 0x9D4) = 0; /* run 200: after the 0x934 store (statement-order grid) */
    } else {
        *(f32 *)((char *)(arg0) + 0x920) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x924) = (f32) 0;
        *(s32 *)((char *)(arg0) + 0x9D4) = (s32) (*(s32 *)((char *)(arg0) + 0x9D4) + 1);
        *(f32 *)((char *)(arg0) + 0x928) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x92C) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x930) = (f32) 0;
        *(f32 *)((char *)(arg0) + 0x934) = (f32) 0;
    }
    temp_f2_3 = *(f32 *)((char *)(arg0) + 0x92C);
    temp_f12_2 = *(f32 *)((char *)(arg0) + 0x930);
#define temp_f14 (*(f32 *)((char *)(arg0) + 0x934)) /* run 184: de-named (lever-sweep) */
    temp_f16 = *(f32 *)((char *)(arg0) + 0x910);
    *(f32 *)((char *)(arg0) + 0xA1C) = (f32) (*(f32 *)((char *)(arg0) + 0xA1C) + (temp_f16 * ((*(f32 *)((char *)(arg0) + 0x3BC) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3C0) * temp_f12_2) + (*(f32 *)((char *)(arg0) + 0x3C4) * temp_f14))));
    *(f32 *)((char *)(arg0) + 0xA20) = (f32) (*(f32 *)((char *)(arg0) + 0xA20) + (temp_f16 * ((*(f32 *)((char *)(arg0) + 0x3B0) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3B4) * temp_f12_2) +(*(f32 *)((char *)(arg0) + 0x3B8) * temp_f14))));
    *(f32 *)((char *)(arg0) + 0xA24) = (f32) (*(f32 *)((char *)(arg0) + 0xA24) - ((*(f32 *)((char *)(arg0) + 0x3C8) * temp_f2_3) + (*(f32 *)((char *)(arg0) + 0x3CC) * temp_f12_2) +(*(f32 *)((char *)(arg0) + 0x3D0) * temp_f14)));
}
