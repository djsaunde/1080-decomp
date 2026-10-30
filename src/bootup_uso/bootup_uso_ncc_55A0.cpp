/* IDO 7.1 NCC (C++) donor for func_000055A0 (agent-g run 176: mechanical port of the cfe NM body).
 * EXACT in agent-g run 241, REPLACE_FUNC_BODY-spliced over the bootup_uso.c placeholder on both build paths.
 * Reloc binding from the target relocations: every blank (reloc-free) lui/addiu pair references a 0-pinned
 * D_55A0_* name (gRoot -> D_55A0_root, the list table -> D_55A0_it0), the others are pinned to their target
 * addresses in undefined_syms_auto.txt. */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct { int w; } SC4_;
extern "C" {
extern char D_00000000;
extern int func_00000000(...);
extern void *D_55A0_root; /* zero-page USO-GOT global (gRoot); reloaded per use (no CSE) */
/* tail-block init globals (decode-progress 2026-06-26): the four transform-group
 * registrations + final 0x58 block pass a distinct per-group global as arg2
 * (D_00007E58..68), the first 0x48 node uses &D_00007E48 and func_00007E50's
 * word value, the 2nd 0x48 uses D_00007E54, and obj+0x28 stores &D_00007600. */
extern int D_00007E48, D_00007E54, D_00007E58, D_00007E5C, D_00007E60, D_00007E64, D_00007E68;
extern char D_00007600;
extern void func_00007E50(int *a0);
extern int D_00007E20, D_55A0_it0; /* arg1==1 list-init globals (decode 2026-06-26; the list table is blank in the target) */
extern int D_00007E34; /* actor-0 sub-init preload value (decode 2026-06-26) */
extern char D_00007AB8; /* actor-0 obj+0x28 symbol store (decode 2026-06-26) */
#define DROOT ((int *)D_55A0_root)
#define DI(o) (*(int *)((char *)&D_00000000 + (o)))
#define DF(o) (*(float *)((char *)&D_00000000 + (o)))
extern f32 D_55A0_ro598, D_55A0_ro59C, D_55A0_ro5A0, D_55A0_ro5A4, D_55A0_ro5A8, D_55A0_ro5AC;  /* per-address RoData pool scalars (func_0000057C + K): target re-emits lui at per load */
#define DP(o) ((void *)((char *)&D_00000000 + (o)))
#define OI(p, o) (*(int *)((char *)(p) + (o)))
#define OF(p, o) (*(float *)((char *)(p) + (o)))

/* scratch[0] == sp+0xC4. Indices below are byte-offset/4 from 0xC4. */
#define M(off) mm[((off) - 0xC4) / 4]
/* RUN 107 (agent-c, byvalue-struct-scan `out` tell): 51.9 -> 87.6. Every
 * sub-object ctor takes a 4-byte struct BY VALUE (`lw a1,92(sp); sw a1,4(sp)`):
 * zero-page global word -> per-object temp (v0..v3 / t4..t10, kept alive by
 * an address escape so their dead stores stay) -> inline-param copies p1/p2
 * shared by all objects, copied as `.w` ints (a struct assignment goes through
 * addiu-addressed copies). Each reloc-blind zero-page site is its own extern
 * (D_55A0_*), so uopt cannot CSE or dead-store them; one merged flags variable
 * (s0); address-taken list iterator; actor zeros as (f32)0 so 0.0f is not
 * hoisted into $f20. Residual: frame 0x170 vs 0x180, arg1 home reloads, the
 * list-iterator spill shape, t-register numbering. */
typedef struct { int w; } S4_55A0;
typedef struct { float x, y, z, w; } V4_55A0;
extern S4_55A0 D_55A0_v0, D_55A0_v1, D_55A0_v2, D_55A0_v3;
extern char D_55A0_s28_0, D_55A0_s28_1, D_55A0_s28_2, D_55A0_s28_3;
extern char D_55A0_o28_0, D_55A0_o28_1, D_55A0_o28_2, D_55A0_o28_3;
extern char D_55A0_o48_0, D_55A0_o48_1, D_55A0_o48_2;
typedef struct { int a, b; } P8_55A0;
extern P8_55A0 D_55A0_pair;
extern char D_55A0_la, D_55A0_lb, D_55A0_n4, D_55A0_n2;
extern char D_55A0_tb, D_55A0_o28_4, D_55A0_s28_5, D_55A0_o28_5;
extern int D_55A0_g5, D_55A0_g6, D_55A0_g7, D_55A0_g8, D_55A0_g9, D_55A0_g10;
typedef struct { int a, b; } G2_55A0;
typedef struct { int w, x, y; } R12_55A0;
extern G2_55A0 D_55A0_G;
extern int D_55A0_w0, D_55A0_r0, D_55A0_z0, D_55A0_list, D_55A0_listchk, D_55A0_list2;
extern char D_55A0_lb0, D_55A0_fmt, D_55A0_cfg;
#define CFG55A0(o) (*(int *)(&D_55A0_cfg + (o)))
}
static inline f32 fi(int v) { return v; }
extern "C" void func_000055A0(int arg0, int arg1, int arg2) {
    S4_55A0 t4, t5, t6, t7, t8, t9, t10;
    float m[40]; /* sp+0xC4 .. sp+0x163 */
    int var_s0;
    char *nl;
    /* agent-g run 235 frame map (every target sp offset but vec2's): the list object's nl is its own nl0 (target
     * home 0x84, the old mode_kd slot), the trailing 0x48 actor's inner pointer is s5 (home 0x78, obj's old slot;
     * obj / n4 live in s-regs, their homes moved into the never-accessed 0x9C..0xBB gap), vec1 / vec2 are block
     * locals of the actor-1 / actor-2 blocks (target 0x44 / 0x28), n3 last at function scope (target spill 0x54),
     * no block-scope q pointers (their block home sat at 0x54) */
    V4_55A0 vecp1; char *obj; char *n4; int vk1; int vk2;
    R12_55A0 *it;
    S4_55A0 v0, v1, v2, v3;
    char *nl0;
    float *mm = m;
    S4_55A0 p1;
    char *s5;
    V4_55A0 vec0;
    char *sub, *node;
    S4_55A0 p2;
    char *n2, *n3;

    if (0 * ((int)&v0 + (int)&v1 + (int)&v2 + (int)&v3 + (int)&t4 + (int)&t5 + (int)&t10 + (int)&p1 + (int)&vecp1 + (int)&vk1 + (int)&vk2)) {}
    D_55A0_w0 = D_55A0_r0 | 8; if (1) {}
    D_55A0_G.b = 0xAA002;
    D_55A0_G.a = 0;
    var_s0 = arg2 | 0x10;
    M(0x154) = fi(0); M(0x158) = fi(0); M(0x15C) = fi(0); M(0x160) = fi(0);
    M(0x144) = fi(0); M(0x148) = fi(0); M(0x150) = fi(0);
    M(0x134) = fi(0); M(0x138) = fi(0); M(0x140) = fi(0); M(0x130) = fi(0);
    M(0x114) = fi(0); M(0x11C) = fi(0); M(0x120) = fi(0);
    M(0x104) = fi(0); M(0x10C) = fi(0); M(0x110) = fi(0);
    M(0x14C) = 1.0f; M(0x124) = 1.0f; M(0x128) = 1.0f; M(0x12C) = 1.0f; M(0x118) = 1.0f; M(0xC4) = 1.0f;
    M(0xF4) = fi(0); M(0xFC) = fi(0); M(0x100) = fi(0);
    M(0xEC) = fi(0); M(0xF0) = fi(0);
    M(0xD8) = fi(0); M(0xDC) = fi(0); M(0xE0) = fi(0);
    M(0xC8) = fi(0); M(0xCC) = fi(0); M(0xD0) = fi(0);
    M(0x13C) = 0.5f; M(0x108) = 0.5f; M(0xE4) = 0.5f; M(0xE8) = 0.5f; M(0xD4) = 0.5f;
    M(0xF8) = D_55A0_ro598;
    D_55A0_z0 = 0;

    if (arg1 == 1) {
        D_55A0_G.b = 0x28002;
        D_55A0_G.a = 0;
        D_55A0_list = func_00000000(0, &D_00007E20, &D_55A0_lb0, 0);
        it = (R12_55A0 *)&D_55A0_it0;
        if (D_55A0_listchk != 0) {
            do {
                func_00000000(&D_55A0_fmt, it->w, it);
                func_00000000(D_55A0_list2, it);
                it++;
            } while (it->w != 0);
            /* agent-g run 241: `it` is a plain register variable in the target (a2, spilled to its home 0x98 around
             * the two calls, never stored back after it++): no &it in the frame-home list; six post-loop reads add
             * adjsave units so it loses the s-register (fewer than six: s1). 97.29 -> 98.07, length = target. */
            if (it) {} if (it) {} if (it) {} if (it) {} if (it) {} if (it) {}
        }
    }

    if (arg1 != 6) {
        var_s0 = var_s0 | 0x20;
    }
    var_s0 |= 4;
    if ((arg1 == 6) || (arg1 == 0xB)) {
        var_s0 &= ~0x704;
    }
#define mode (CFG55A0(0x34))
    if (mode == 5) {
        var_s0 |= 1;
    }
    if (mode == 6) {
        var_s0 |= 2;
    }
    if ((arg1 == 2) || (arg1 == 3) || (arg1 == 4) || (arg1 == 9) || (CFG55A0(0x64) == 7)) {
        var_s0 |= 8;
    }
    if (arg1 == 1) {
        var_s0 = 0xFFFF;
    }
    OI(DROOT, 0x34) = var_s0;

    if (var_s0 & 4) {
        /* actor 0 (0x74) */
        /* agent-g run 241: whole-struct copies (one load, two stores through a ugen temp, not a coloured
         * register; v0 stored first); the other actors copy p1 from their v: 98.07 -> 98.98 */
        p1.w = (v0 = D_55A0_v0).w;
        obj = (char *)func_00000000(0x74);
        if (obj != 0) {
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, 0);
                OI(sub, 0x28) = (int)&D_55A0_s28_0;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_0;
            node = obj + 0x5C;
            if (node != 0 || (node = (char *)func_00000000(0x18)) != 0) {
                func_00000000(node);
            }
            OF(obj, 0x4C) = (f32)0; OF(obj, 0x50) = (f32)0; OF(obj, 0x54) = (f32)0; OF(obj, 0x58) = 1.0f; if (1) {}
            vec0.w = 1.0f; vec0.x = D_55A0_ro59C; vec0.y = D_55A0_ro59C; vec0.z = D_55A0_ro59C;
            func_00000000(obj + 0x5C, &vec0, 993, 997);
            OI(obj, 0x38) = 1; OI(obj, 0x48) = (int)&D_55A0_o48_0;
        }
        func_00000000(DROOT, obj, 4);

        /* actor 1 (0x74) */
        v1 = D_55A0_v1;
        p1 = v1;
        obj = (char *)func_00000000(0x74);
        if (obj != 0) {
            V4_55A0 vec1;
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, 0);
                OI(sub, 0x28) = (int)&D_55A0_s28_1;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_1;
            n2 = obj + 0x5C;
            if (n2 != 0 || (n2 = (char *)func_00000000(0x18)) != 0) {
                func_00000000(n2);
            }
            OF(obj, 0x4C) = (f32)0; OF(obj, 0x50) = (f32)0; OF(obj, 0x54) = (f32)0; OF(obj, 0x58) = 1.0f; if (1) {}
            vec1.w = 1.0f; vec1.x = D_55A0_ro5A0; vec1.y = D_55A0_ro5A0; vec1.z = D_55A0_ro5A0;
            func_00000000(obj + 0x5C, &vec1, 993, 997);
            OI(obj, 0x38) = 1; OI(obj, 0x48) = (int)&D_55A0_o48_1;
        }
        func_00000000(DROOT, obj, 4);

        /* actor 2 (0x74) */
        /* agent-g run 241: a dead nested call-result temp grows edg's pool here, so the dead-stored call-result
         * vreg is homed above vec2 (vec2 sp+0x28 = target) instead of below it */
        if (0) { func_00000000(func_00000000(0)); }
        v2 = D_55A0_v2;
        p1 = v2;
        obj = (char *)func_00000000(0x74);
        if (obj != 0) {
            int b0 = 0; int b1 = 0; V4_55A0 vec2;
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, 0);
                OI(sub, 0x28) = (int)&D_55A0_s28_2;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_2;
            n2 = obj + 0x5C;
            if (n2 != 0 || (n2 = (char *)func_00000000(0x18)) != 0) {
                func_00000000(n2);
            }
            OF(obj, 0x4C) = (f32)0; OF(obj, 0x50) = (f32)0; OF(obj, 0x54) = (f32)0; OF(obj, 0x58) = 1.0f;
            vec2.x = D_55A0_ro5A4; vec2.y = D_55A0_ro5A8; vec2.z = D_55A0_ro5AC; vec2.w = 1.0f;
            func_00000000(obj + 0x5C, &vec2, 993, 997);
            OI(obj, 0x38) = 1; OI(obj, 0x48) = (int)&D_55A0_o48_2;
        }
        func_00000000(DROOT, obj, 4);

        /* list object (0x6C) */
        D_55A0_G.b = 0x28002;
        D_55A0_G.a = 0;
        nl0 = (char *)func_00000000(0, &D_55A0_la, &D_55A0_lb, 0);
        v3 = D_55A0_v3;
        p1 = v3;
        obj = (char *)func_00000000(0x6C);
        if (obj != 0) {
            p2.w = p1.w;
            sub = obj;
            if (sub != 0 || (sub = (char *)func_00000000(0x48)) != 0) {
                func_00000000(sub, p2, nl0);
                OI(sub, 0x28) = (int)&D_55A0_s28_3;
            }
            OI(obj, 0x28) = (int)&D_55A0_o28_3;
            n2 = obj + 0x48;
            if (n2 != 0 || (n2 = (char *)func_00000000(0x10)) != 0) {
                n3 = n2;
                if (n3 != 0 || (n3 = (char *)func_00000000(0x10)) != 0) {
                    n4 = n3;
                    if (n4 != 0 || (n4 = (char *)func_00000000(4)) != 0) {
                        OI(n4, 0) = (int)&D_55A0_n4;
                    }
                    OI(n3, 0x4) = (int)obj; *(P8_55A0 *)(n3 + 8) = D_55A0_pair;
                }
                OI(n2, 0) = (int)&D_55A0_n2;
            }
            DI(0) = 0;
            OI(obj, 0x5C) = 2;
            OI(obj, 0x64) = 0;
            OI(obj, 0x60) = 0;
            OI(obj, 0x58) = 0;
            OI(obj, 0x68) = 0;
        }
        func_00000000(DROOT, obj, 0xFFFF);
    }

    func_00000000(DROOT, DP(0));

    /* trailing 0x48 / 0x58 objects + transform-group registration */
    nl = (char *)func_00000000(0, &D_00007E48, &D_55A0_tb, 0);
    *(SC4_ *)&(t4.w) = *(SC4_ *)&(*(int *)&func_00007E50);
    p1.w = t4.w;
    obj = (char *)func_00000000(0x48);
    if (nl) {} /* agent-g run 241: nl's reload before the obj test (target lw a2 above the beqz) */
    if (obj != 0) {
        func_00000000(obj, p1, nl);
        OI(obj, 0x28) = (int)&D_55A0_o28_4;
    }
    func_00000000(DROOT, obj, 0xFFFF);

    t5 = *(S4_55A0 *)&D_00007E54;
    p1 = t5;
    obj = (char *)func_00000000(0x48);
    if (obj != 0) {
        p2.w = p1.w;
        s5 = obj;
        if (s5 != 0 || (s5 = (char *)func_00000000(0x48)) != 0) {
            func_00000000(s5, p2, D_55A0_g5);
            OI(s5, 0x28) = (int)&D_55A0_s28_5;
        }
        OI(obj, 0x28) = (int)&D_55A0_o28_5;
    }
    func_00000000(DROOT, obj, 0xFFFF);

    func_00000000(DROOT, func_00000000(0, (t6.w = *(int *)&D_00007E58, t6), D_55A0_g6, &M(0x144)), 0xFFFF);
    func_00000000(DROOT, func_00000000(0, (t7.w = *(int *)&D_00007E5C, t7), D_55A0_g7, &M(0x114)), 0xFFFF);
    func_00000000(DROOT, func_00000000(0, (t8.w = *(int *)&D_00007E60, t8), D_55A0_g8, &M(0xD4)), 0xFFFF);
    func_00000000(DROOT, func_00000000(0, (t9.w = *(int *)&D_00007E64, t9), D_55A0_g9, &M(0xC4)), 0xFFFF);

    t10 = *(S4_55A0 *)&D_00007E68;
    p1 = t10;
    obj = (char *)func_00000000(0x58);
    if (obj != 0) {
        func_00000000(obj, p1, D_55A0_g10, &M(0xE4));
        OI(obj, 0x28) = (int)&D_00007600;
    }
    func_00000000(DROOT, obj, 0xFFFF);
}
