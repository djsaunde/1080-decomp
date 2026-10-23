/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for game_uso_func_00007C1C
 * (1071 words). NM object only (build/non_matching); the ROM build keeps
 * INCLUDE_ASM. The cfe NM body recompiled unchanged under NCC: 88.2 -> 89.3
 * fuzzy, 1058 -> 1065 words (toward the target's 1071). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
#define NULL 0
typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b, c; } Tri3i;
extern "C" int game_uso_func_00003FAC(...);
extern "C" {
extern char D_00000000;
extern int game_uso_func_055750(...);
extern void game_uso_func_047484(int *dst, int a, int b, int c, float f);
extern void game_uso_func_047484(int *dst, int a, int b, int c, float f);
extern void game_uso_func_047484(int *dst, int a, int b, int c, float f);
extern float game_uso_func_082880(float);
extern void game_uso_func_071028(Vec3 *v);
extern float game_uso_func_082880(float);
extern float game_uso_func_00007A98(int *);
}
extern "C" int game_uso_func_00007C1C(char *arg0, char *arg1, char *arg2, char *arg3,
                           char *arg4, f32 *arg5) {
    Vec3 result;                     /* sp+932 */
    Vec3 cpA;                        /* sp+920 */
    Vec3 nA;                         /* sp+908 */
    Vec3 scratch;                    /* sp+852 shared word-copy stage */
    Vec3 nB;                         /* sp+840 */
    Vec3 stg1;                       /* sp+820 */
    Vec3 mA;                         /* sp+808 */
    Vec3 mC2;                        /* sp+796 */
    int mode;                        /* sp+792 */
    Vec3 stg2;                       /* sp+780 */
    Vec3 segDir;                     /* sp+768 */
    Vec3 resDir;                     /* sp+756 */
    Vec3 hitA;                       /* sp+736 */
    Vec3 hitB;                       /* sp+724 */
    int hit;                         /* sp+720 */
    Vec3 hitC;                       /* sp+704 */
    Vec3 mB;                         /* sp+692 */
    Vec3 mE;                         /* sp+652 */
    Vec3 mC;                         /* sp+640 */
    Vec3 mD;                         /* sp+628 */
    Vec3 mP;                         /* sp+616 */
    Vec3 mD2;                        /* sp+604 */
    Vec3 s1c;                        /* sp+592 */
    Vec3 s1bb;                       /* sp+580 */
    Vec3 s1b;                        /* sp+568 */
    Vec3 s1a;                        /* sp+556 */
    Vec3 mid2;                       /* sp+540 shared mid stage */
    Vec3 s1m2;                       /* sp+528 */
    Vec3 mid3;                       /* sp+508 shared mid stage */
    Vec3 s1m3;                       /* sp+496 */
    Vec3 s2c;                        /* sp+460 */
    Vec3 s2bb;                       /* sp+448 */
    Vec3 s2b;                        /* sp+436 */
    Vec3 s2a;                        /* sp+424 */
    Vec3 s2m2;                       /* sp+412 */
    Vec3 s2m3;                       /* sp+400 */
    Vec3 lSeg;                       /* sp+380 */
    Vec3 lA;                         /* sp+368 */
    Vec3 lE;                         /* sp+356 */
    Vec3 lDir;                       /* sp+344 */
    Vec3 lRD;                        /* sp+332 */
    Vec3 lB;                         /* sp+320 */
    Vec3 lP;                         /* sp+308 */
    Vec3 lF;                         /* sp+296 */
    Vec3 l1c;                        /* sp+284 */
    Vec3 l1bb;                       /* sp+272 */
    Vec3 l1b;                        /* sp+260 */
    Vec3 l1a;                        /* sp+248 */
    Vec3 l1m2;                       /* sp+236 */
    Vec3 l1m3;                       /* sp+224 */
    Vec3 l2c;                        /* sp+208 */
    Vec3 l2bb;                       /* sp+196 */
    Vec3 l2b;                        /* sp+184 */
    Vec3 l2a;                        /* sp+172 */
    Vec3 l2m2;                       /* sp+160 */
    Vec3 l2m3;                       /* sp+148 */
    Vec3 fA;                         /* sp+136 */
    Vec3 fB;                         /* sp+124 */
    Vec3 n0;                         /* sp+112 */
    Vec3 n1;                         /* sp+100 */
    Vec3 n2;                         /* sp+88 */
    int notFirst;                    /* sp+84 */
    int firstFlag;                   /* sp+80 */
    Vec3 *p;
    f32 *q;
    char *node;
    float k;
    float metric, thresh;
    f32 tx, tz;

    if (arg5 != NULL) {
        *arg5 = 0.0f;
    }

    if (arg3 == NULL) {
        q = (f32 *)((int)arg2 + 0x30);
        p = 0; if (1) { p = &nA; }
        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tx = *q;
            tz = *(f32 *)((int)q + 8);
            p->y = 0.0f;
            p->x = tx;
            p->z = tz;
        }
        p = 0; if (1) { p = &nB; }
        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tx = nA.x - *(f32 *)(arg4 + 0x0);
            tz = nA.z - *(f32 *)(arg4 + 0x8);
            p->y = 0.0f;
            p->x = tx;
            p->z = tz;
        }
        *(Tri3i *)&scratch = *(Tri3i *)&nB;
        *(Tri3i *)&cpA = *(Tri3i *)&scratch;
        result.y = 0.0f;
        result.z = cpA.z;
        result.x = cpA.x;
    } else {
        mode = 0;
        q = (f32 *)((int)arg3 + 0x30);
        p = 0; if (1) { p = &mA; }
        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tz = *(f32 *)((int)q + 8);
            tx = *q;
            p->y = 0.0f;
            p->z = tz;
            p->x = tx;
        }
        q = (f32 *)((int)arg2 + 0x30);
        p = 0; if (1) { p = &mB; }
        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tx = mA.x - *q;
            tz = mA.z - *(f32 *)((int)q + 8);
            p->y = 0.0f;
            p->x = tx;
            p->z = tz;
        }
        *(Tri3i *)&scratch = *(Tri3i *)&mB;
        *(Tri3i *)&stg1 = *(Tri3i *)&scratch;
        *(Tri3i *)&stg2 = *(Tri3i *)&stg1;

        if (*(int *)(*(int *)(arg1 + 0x30) + 0x908) != 0) {
            float dot = game_uso_func_00007A98((int *)arg1);
            int active = *(int *)(arg1 + 0xA4);
            if (active == 0) {
                if (*(f32 *)(arg1 + 0xA0) <= dot) {
                    active = 1;
                    *(int *)(arg1 + 0xA4) = 1;
                }
            } else if (dot <= -*(f32 *)(arg1 + 0xA0)) {
                *(int *)(arg1 + 0xA4) = 0;
                active = 0;
            }
            if (active == 1 && dot <= *(f32 *)((char *)&D_00000000 + 0xFC)) {
                char *src = (char *)*(int *)(*(int *)(arg1 + 0x30) + 0x908) + 0xB4;
                p = 0; if (1) { p = &mC; }
                if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                    tx = *(f32 *)(src + 0x0);
                    tz = *(f32 *)(src + 0x8);
                    p->y = 0.0f;
                    p->x = tx;
                    p->z = tz;
                }
                q = (f32 *)((int)arg2 + 0x30);
                p = 0; if (1) { p = &mD; }
                if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                    tx = mC.x - *q;
                    tz = mC.z - *(f32 *)((int)q + 8);
                    p->y = 0.0f;
                    p->x = tx;
                    p->z = tz;
                }
                *(Tri3i *)&scratch = *(Tri3i *)&mD;
                *(Tri3i *)&mE = *(Tri3i *)&scratch;
                mode = (stg2.x * mE.z <= stg2.z * mE.x) ? 2 : 1;
            }
        }

        p = 0; if (1) { p = &mP; }

        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tx = stg2.z;
            tz = -stg2.x;
            p->y = 0.0f;
            p->x = tx;
            p->z = tz;
        }
        *(Tri3i *)&scratch = *(Tri3i *)&mP;
        *(Tri3i *)&segDir = *(Tri3i *)&scratch;
        game_uso_func_071028(&segDir);

        q = (f32 *)((int)arg3 + 0x30);

        p = 0; if (1) { p = &mC2; }

        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tz = *(f32 *)((int)q + 8);
            tx = *q;
            p->y = 0.0f;
            p->z = tz;
            p->x = tx;
        }
        p = 0; if (1) { p = &mD2; }
        if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
            tx = mC2.x - *(f32 *)(arg4 + 0x0);
            tz = mC2.z - *(f32 *)(arg4 + 0x8);
            p->y = 0.0f;
            p->x = tx;
            p->z = tz;
        }
        *(Tri3i *)&scratch = *(Tri3i *)&mD2;
        *(Tri3i *)&resDir = *(Tri3i *)&scratch;
        result.x = scratch.x; result.y = scratch.y; result.z = scratch.z;

        if (mode == 1) {
            s1a.x = segDir.x * 250.0f;
            s1a.y = segDir.y * 250.0f;
            s1a.z = segDir.z * 250.0f;
            *(Tri3i *)&scratch = *(Tri3i *)&s1a;
            *(Tri3i *)&s1b = *(Tri3i *)&scratch;
            k = *(f32 *)(arg3 + 0x54);
            s1m2.x = s1b.x * k;
            s1m2.y = s1b.y * k;
            s1m2.z = s1b.z * k;
            *(Tri3i *)&mid2 = *(Tri3i *)&s1m2;
            *(Tri3i *)&s1bb = *(Tri3i *)&mid2;
            s1m3.x = s1bb.x * 0.5f;
            s1m3.y = s1bb.y * 0.5f;
            s1m3.z = s1bb.z * 0.5f;
            *(Tri3i *)&mid3 = *(Tri3i *)&s1m3;
            *(Tri3i *)&s1c = *(Tri3i *)&mid3;
            result.x += s1c.x;
            result.y += s1c.y;
            result.z += s1c.z;
        } else if (mode == 2) {
            s2a.x = segDir.x * 250.0f;
            s2a.y = segDir.y * 250.0f;
            s2a.z = segDir.z * 250.0f;
            *(Tri3i *)&scratch = *(Tri3i *)&s2a;
            *(Tri3i *)&s2b = *(Tri3i *)&scratch;
            k = *(f32 *)(arg3 + 0x54);
            s2m2.x = s2b.x * k;
            s2m2.y = s2b.y * k;
            s2m2.z = s2b.z * k;
            *(Tri3i *)&mid2 = *(Tri3i *)&s2m2;
            *(Tri3i *)&s2bb = *(Tri3i *)&mid2;
            s2m3.x = s2bb.x * 0.5f;
            s2m3.y = s2bb.y * 0.5f;
            s2m3.z = s2bb.z * 0.5f;
            *(Tri3i *)&mid3 = *(Tri3i *)&s2m3;
            *(Tri3i *)&s2c = *(Tri3i *)&mid3;
            result.x -= s2c.x;
            result.y -= s2c.y;
            result.z -= s2c.z;
        }

        metric = game_uso_func_082880(resDir.x * resDir.x + resDir.z * resDir.z);
        hit = 0;
        thresh = *(f32 *)(arg3 + 0x54) * 250.0f;
        if (thresh < metric) {
            game_uso_func_00003FAC(&hitA.x, &hitB.x, &resDir.x, metric, thresh);
            hit = 1;
        }

        node = *(char **)(arg3 + 0x2C);
        while (node != NULL) {
            notFirst = (hit != 0);
            firstFlag = (notFirst == 0);

            q = (f32 *)((int)node + 0x30);

            p = 0; if (1) { p = &lA; }

            if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                tx = *q;
                tz = *(f32 *)((int)q + 8);
                p->y = 0.0f;
                p->x = tx;
                p->z = tz;
            }
            q = (f32 *)((int)arg3 + 0x30);
            p = 0; if (1) { p = &lB; }
            if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                tz = lA.z - *(f32 *)((int)q + 8);
                tx = lA.x - *q;
                p->y = 0.0f;
                p->z = tz;
                p->x = tx;
            }
            *(Tri3i *)&scratch = *(Tri3i *)&lB;
            *(Tri3i *)&lSeg = *(Tri3i *)&scratch;

            p = 0; if (1) { p = &lP; }

            if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                tx = lSeg.z;
                tz = -lSeg.x;
                p->y = 0.0f;
                p->x = tx;
                p->z = tz;
            }
            *(Tri3i *)&mid2 = *(Tri3i *)&lP;
            *(Tri3i *)&lDir = *(Tri3i *)&mid2;
            game_uso_func_071028(&lDir);

            q = (f32 *)((int)node + 0x30);

            p = 0; if (1) { p = &lE; }

            if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                tx = *q;
                tz = *(f32 *)((int)q + 8);
                p->y = 0.0f;
                p->x = tx;
                p->z = tz;
            }
            p = 0; if (1) { p = &lF; }
            if (p != NULL || (p = (Vec3 *)game_uso_func_055750(0xC)) != NULL) {
                tx = lE.x - *(f32 *)(arg4 + 0x0);
                tz = lE.z - *(f32 *)(arg4 + 0x8);
                p->y = 0.0f;
                p->x = tx;
                p->z = tz;
            }
            *(Tri3i *)&scratch = *(Tri3i *)&lF;
            *(Tri3i *)&lRD = *(Tri3i *)&scratch;
            hitC.x = scratch.x; hitC.y = scratch.y; hitC.z = scratch.z;

            if (mode == 1) {
                l1a.x = lDir.x * 250.0f;
                l1a.y = lDir.y * 250.0f;
                l1a.z = lDir.z * 250.0f;
                *(Tri3i *)&scratch = *(Tri3i *)&l1a;
                *(Tri3i *)&l1b = *(Tri3i *)&scratch;
                k = *(f32 *)(node + 0x54);
                l1m2.x = l1b.x * k;
                l1m2.y = l1b.y * k;
                l1m2.z = l1b.z * k;
                *(Tri3i *)&mid2 = *(Tri3i *)&l1m2;
                *(Tri3i *)&l1bb = *(Tri3i *)&mid2;
                l1m3.x = l1bb.x * 0.5f;
                l1m3.y = l1bb.y * 0.5f;
                l1m3.z = l1bb.z * 0.5f;
                *(Tri3i *)&mid3 = *(Tri3i *)&l1m3;
                *(Tri3i *)&l1c = *(Tri3i *)&mid3;
                hitC.x += l1c.x;
                hitC.y += l1c.y;
                hitC.z += l1c.z;
            } else if (mode == 2) {
                l2a.x = lDir.x * 250.0f;
                l2a.y = lDir.y * 250.0f;
                l2a.z = lDir.z * 250.0f;
                *(Tri3i *)&scratch = *(Tri3i *)&l2a;
                *(Tri3i *)&l2b = *(Tri3i *)&scratch;
                k = *(f32 *)(node + 0x54);
                l2m2.x = l2b.x * k;
                l2m2.y = l2b.y * k;
                l2m2.z = l2b.z * k;
                *(Tri3i *)&mid2 = *(Tri3i *)&l2m2;
                *(Tri3i *)&l2bb = *(Tri3i *)&mid2;
                l2m3.x = l2bb.x * 0.5f;
                l2m3.y = l2bb.y * 0.5f;
                l2m3.z = l2bb.z * 0.5f;
                *(Tri3i *)&mid3 = *(Tri3i *)&l2m3;
                *(Tri3i *)&l2c = *(Tri3i *)&mid3;
                hitC.x -= l2c.x;
                hitC.y -= l2c.y;
                hitC.z -= l2c.z;
            }

            if (firstFlag ||
                ((hitC.x * hitA.z <= hitC.z * hitA.x) &&
                 (hitC.z * hitB.x <= hitC.x * hitB.z))) {
                float m2 = game_uso_func_082880(lRD.x * lRD.x + lRD.z * lRD.z);
                float t2 = *(f32 *)(node + 0x54) * 250.0f;
                if (t2 < m2) {
                    game_uso_func_00003FAC(&fA.x, &fB.x, &lRD.x, m2, t2);
                    hit = 1;
                    if (notFirst) {
                        if (hitA.z * fA.x < hitA.x * fA.z) {
                            *(Tri3i *)&scratch = *(Tri3i *)&fA;
                            hitA.x = scratch.x; hitA.y = scratch.y; hitA.z = scratch.z;
                        }
                        if (hitB.x * fB.z < hitB.z * fB.x) {
                            *(Tri3i *)&scratch = *(Tri3i *)&fB;
                            hitB.x = scratch.x; hitB.y = scratch.y; hitB.z = scratch.z;
                        }
                    } else {
                        *(Tri3i *)&scratch = *(Tri3i *)&fA;
                        hitA.x = scratch.x; hitA.y = scratch.y; hitA.z = scratch.z;
                        *(Tri3i *)&scratch = *(Tri3i *)&fB;
                        hitB.x = scratch.x; hitB.y = scratch.y; hitB.z = scratch.z;
                    }
                }
                *(Tri3i *)&scratch = *(Tri3i *)&hitC;
                result.x = scratch.x; result.y = scratch.y; result.z = scratch.z;
                arg3 = node;
                node = *(char **)(node + 0x2C);
                continue;
            }
            break;
        }

        if (node != NULL &&
            game_uso_func_082880(result.x * result.x + result.z * result.z) <
                *(f32 *)(*(int *)(arg1 + 0x30) + 0x348) * *(f32 *)(arg1 + 0xCC)) {
            *(Tri3i *)&n0 = *(Tri3i *)&hitC;
            game_uso_func_071028(&n0);
            *(Tri3i *)&n1 = *(Tri3i *)&hitA;
            game_uso_func_071028(&n1);
            *(Tri3i *)&n2 = *(Tri3i *)&hitB;
            game_uso_func_071028(&n2);
            if ((n0.x * n2.x + n0.z * n2.z) < (n0.x * n1.x + n0.z * n1.z)) {
                *(Tri3i *)&scratch = *(Tri3i *)&hitA;
                result.x = scratch.x; result.y = scratch.y; result.z = scratch.z;
            } else {
                *(Tri3i *)&scratch = *(Tri3i *)&hitB;
                result.x = scratch.x; result.y = scratch.y; result.z = scratch.z;
            }
        }
    }

    *(Tri3i *)arg0 = *(Tri3i *)&result;
    return (int)arg0;
}
