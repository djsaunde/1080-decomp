/* game_uso_func_00007C1C NON_MATCHING donor, cfront-style C for IDO 7.1 cfe -O2
 * (agent-c run 230, from agent-g's run 229 port c7c7.c). NM object only: spliced
 * over the INCLUDE_ASM placeholder in build/non_matching (GAMEUSO_7C1C_NM_DONOR);
 * the ROM build keeps INCLUDE_ASM. Replaces the NCC donor game_uso_ncc_7C1C_nm.cpp
 * (96.35) as the NM body. Kit CCXL in docs/IDO_CODEGEN.md has the levers.
 *
 * Steering target in the XZ plane (see the NCC donor for the C++ shape): every
 * bnez &stack / jal 055750(12) guard is a cfront ctor temporary, the pooled
 * return slots are R (s1, 0x354), P2 (s2, 0x21C) and P3 (0x1FC), each inline
 * Vec3::set / XZ ctor goes through the shared float temporaries tx / ty / tz
 * (SUB sites through sa / sc), and int gN placeholders keep every frame home at
 * its target address (frame 0x3B0).
 * The active flag is re-read after the stores (uopt forwards the stored value,
 * the target's v0). Length = target (1071 words, opcode-identical). The empty
 * block at the loop-body top (run 231) lets the &t128 guard address colour, so
 * the ugen t-ring lines up from the t164 site on (98.52 -> 99.81). LEFT: FP
 * colours (b->width f0 / z f14, main third-level x f14, loop perp x f14), the
 * hitC copy load order, the final compare's sum / n0.x swap. */
typedef struct { float x, y, z; } Vec3;
typedef struct Node { char pad0[0x2C]; struct Node *next; Vec3 pos; char pad3C[0x54 - 0x3C]; float width; } Node;
typedef struct W { char pad0[0xB4]; Vec3 pos; char padC0[0x348 - 0xC0]; float speed; char pad34C[0x908 - 0x34C]; struct W *other; } W;
typedef struct Self { char pad0[0x30]; W *w; char pad34[0xA0 - 0x34]; float lim; int active; char padA8[0xCC - 0xA8]; float range; } Self;
extern char D_00000000;
extern void *game_uso_func_055750(unsigned);
extern void game_uso_func_00003FAC(float *, float *, float *, float, float);
extern float game_uso_func_082880(float);
extern void game_uso_func_071028(Vec3 *v);
extern float game_uso_func_00007A98(Self *);

/* XZ(const Vec3 &v): Vec3(v.x, 0, v.z) into temp T through pointer P */
#define XZV(P, T, V) vp = (V); P = &(T); if (P || (P = (Vec3 *)game_uso_func_055750(12))) { tx = vp->x; tz = vp->z; P->x = tx; P->y = 0.0f; P->z = tz; }
#define SUB2(Q, PP, T, A, B) Q = &(T); PP = Q; if (PP || (PP = (Vec3 *)game_uso_func_055750(12))) { vp = (B); tx = (A).x - vp->x; tz = (A).z - vp->z; PP->x = tx; PP->y = 0.0f; PP->z = tz; }
#define SUB2S(Q, PP, T, A, B) Q = &(T); PP = Q; if (PP || (PP = (Vec3 *)game_uso_func_055750(12))) { vp = (B); sa = (A).x - vp->x; sc = (A).z - vp->z; PP->x = sa; PP->y = 0.0f; PP->z = sc; }
/* perp(v) = XZ(v.z, -v.x); the do-while block keeps sa / sc as coloured values */
#define XZG2S(Q, PP, T, A, C) Q = &(T); PP = Q; if (PP || (PP = (Vec3 *)game_uso_func_055750(12))) { do { tx = (A); tz = (C); } while (0); PP->x = tx; PP->y = 0.0f; PP->z = tz; }
/* XZ(a, c) */
#define XZGPS(PP, A, C) if (PP || (PP = (Vec3 *)game_uso_func_055750(12))) { do { m2 = (A); tz = (C); } while (0); PP->x = m2; PP->y = 0.0f; PP->z = tz; }
/* Vec3 r; r.set(v * s) */
#define MULS(R, V, S) vp = &(V); tx = vp->x * (S); ty = vp->y * (S); tz = vp->z * (S); (R).x = tx; (R).y = ty; (R).z = tz;
/* the same with the temporaries rotated (third operator* level at three sites) */
#define MULR(R, V, S) vp = &(V); ty = vp->x * (S); tz = vp->y * (S); tx = vp->z * (S); (R).x = ty; (R).y = tz; (R).z = tx;
/* operator*(width) sites: a fourth shared temp m2 takes z (target f14), and the
 * site sits in its own basic blocks (empty blocks around it) so the width load
 * does not share a live unit with tz and colours f0 (run 231) */
#define MULW(R, V, S) if (1) {} vp = &(V); tx = vp->x * (S); ty = vp->y * (S); m2 = vp->z * (S); (R).x = tx; (R).y = ty; (R).z = m2;
/* the main mode-1 third-level site: x through m2 */
#define MULM(R, V, S) vp = &(V); m2 = vp->x * (S); ty = vp->y * (S); tz = vp->z * (S); (R).x = m2; (R).y = ty; (R).z = tz;

#define METV tx
#define THRV ty
#define M2V tx
#define DOTV dot
#define SUB2VS(Q, PP, T, A, B) vp = (B); Q = &(T); PP = Q; if (PP || (PP = (Vec3 *)game_uso_func_055750(12))) { sa = (A).x - vp->x; sc = (A).z - vp->z; PP->x = sa; PP->y = 0.0f; PP->z = sc; }

#define XZU2(P, T, V) P = &(T); if (P || (P = (Vec3 *)game_uso_func_055750(12))) { vp = (V); do { tx = vp->x; tz = vp->z; } while (0); P->x = tx; P->y = 0.0f; P->z = tz; }

#define SUBQ2(PP, A, B) if (PP || (PP = (Vec3 *)game_uso_func_055750(12))) { vp = (B); do { sa = (A).x - vp->x; sc = (A).z - vp->z; } while (0); PP->x = sa; PP->y = 0.0f; PP->z = sc; }

Vec3 game_uso_func_00007C1C(Self *s, Node *a, Node *b, Vec3 *pos, float *out) {
    Vec3 result;
    Vec3 d;
    Vec3 t38c;
    Vec3 *p;
    Vec3 *q;
    Vec3 *vp;
    float tx, tz;
    float dot;
    int active;
    float metric;
    float thresh;
    float ty;
    float m2;
    Vec3 R;
    Vec3 t348;
    int g348[2];
    Vec3 t334;
    Vec3 t328;
    Vec3 t31c;
    int mode;
    Vec3 seg;
    Vec3 dir;
    Vec3 rd;
    int grd[2];
    Vec3 hitA;
    Vec3 hitB;
    int hit;
    float sa;
    Vec3 hitC;
    Vec3 t2b4;
    int g2b4a;
    Vec3 *pB;
    int g2b4b[5];
    Vec3 e;
    Vec3 t280;
    Vec3 t274;
    Vec3 t268;
    Vec3 t25c;
    Vec3 V3a, V2a, V1a, r1a;
    float sc;
    Vec3 P2;
    Vec3 r2a;
    int g210[2];
    Vec3 P3;
    Vec3 r3a;
    int g1f0a[4];
    Vec3 *pC;
    int g1f0b;
    Vec3 V3b, V2b, V1b, r1b, r2b, r3b;
    Node *n;
    Node **pb;
    Vec3 lbind;
    Vec3 t170;
    Vec3 t164;
    Vec3 ldir;
    Vec3 lrd;
    Vec3 t140;
    Vec3 t134;
    Vec3 t128;
    Vec3 V3c, V2c, V1c, r1c, r2c, r3c;
    int ge0;
    Vec3 V3d, V2d, V1d, r1d, r2d, r3d;
    Vec3 fA;
    Vec3 fB;
    Vec3 n0;
    Vec3 n1;
    Vec3 n2;

    pb = &b;
    if (out != 0) {
        *out = 0.0f;
    }
    if (b == 0) {
        XZV(p, t38c, &a->pos)
        SUB2(q, p, t348, t38c, pos)
        R = t348;
        d = R;
        result.y = 0.0f;
        result.x = d.x; result.z = d.z;
    } else {
        mode = 0;
        XZU2(p, t328, &b->pos)
        SUB2VS(q, pB, t2b4, t328, &a->pos)
        R = t2b4; t334 = R;
        seg = t334;
        if (s->w->other != 0) {
            DOTV = game_uso_func_00007A98(s);
            active = s->active;
            if (active == 0) {
                if (s->lim <= DOTV) {
                    s->active = 1;
                }
            } else if (DOTV <= -s->lim) {
                s->active = 0;
            }
            active = s->active;
            if (active == 1 && DOTV <= *(float *)((char *)&D_00000000 + 0xFC)) {
                XZV(p, t280, &s->w->other->pos)
                SUB2VS(q, p, t274, t280, &a->pos)
                R = t274;
                e = R;
                if (seg.x * e.z <= seg.z * e.x) {
                    mode = 2;
                } else {
                    mode = 1;
                }
            }
        }
        XZG2S(q, vp, t268, seg.z, -seg.x)
        R = t268; dir = R;
        game_uso_func_071028(&dir);
        XZU2(p, t31c, &b->pos)
        SUB2S(q, p, t25c, t31c, pos)
        R = t25c;
        rd = R;
        R = rd;
        result.x = R.x; result.y = R.y; result.z = R.z;
        if (mode == 1) {
            MULS(r1a, dir, 250.0f)
            R = r1a; V1a = R;
            MULW(r2a, V1a, b->width)
            P2 = r2a; V2a = P2; if (1) {}
            MULM(r3a, V2a, 0.5f)
            P3 = r3a; V3a = P3;
            result.x += V3a.x; result.y += V3a.y; result.z += V3a.z;
        } else if (mode == 2) {
            MULS(r1b, dir, 250.0f)
            R = r1b; V1b = R;
            MULW(r2b, V1b, b->width)
            P2 = r2b; V2b = P2; if (1) {}
            MULR(r3b, V2b, 0.5f)
            P3 = r3b; V3b = P3;
            result.x -= V3b.x; result.y -= V3b.y; result.z -= V3b.z;
        }
        METV = game_uso_func_082880(rd.x * rd.x + rd.z * rd.z);
        THRV = b->width * 250.0f;
        hit = 0;
        if (THRV < METV) {
            game_uso_func_00003FAC(&hitA.x, &hitB.x, &rd.x, METV, THRV);
            hit = 1;
        }
        n = b->next;
        for (; n != 0; n = n->next + 0 * (int)(n->next)) { if (1) {}
            p = &t170; pC = &t134; pB = &t140; XZV(q, t170, &n->pos)
            SUBQ2(pB, t170, &b->pos)
            R = t140;
            lbind = R;
            XZGPS(pC, lbind.z, -lbind.x)
            P2 = t134;
            ldir = P2;
            game_uso_func_071028(&ldir);
            XZV(p, t164, &n->pos)
            SUB2S(q, p, t128, t164, pos)
            R = t128;
            lrd = R;
            R = lrd;
            hitC.x = R.x; hitC.y = R.y; hitC.z = R.z;
            if (mode == 1) {
                MULS(r1c, ldir, 250.0f)
                R = r1c; V1c = R;
                MULW(r2c, V1c, n->width)
                P2 = r2c; V2c = P2; if (1) {}
                MULR(r3c, V2c, 0.5f)
                P3 = r3c; V3c = P3;
                hitC.x += V3c.x; hitC.y += V3c.y; hitC.z += V3c.z;
            } else if (mode == 2) {
                MULS(r1d, ldir, 250.0f)
                R = r1d; V1d = R;
                MULW(r2d, V1d, n->width)
                P2 = r2d; V2d = P2; if (1) {}
                MULR(r3d, V2d, 0.5f)
                P3 = r3d; V3d = P3;
                hitC.x -= V3d.x; hitC.y -= V3d.y; hitC.z -= V3d.z;
            }
            if ((unsigned)((hit != 0) == 0) ||
                ((hitC.x * hitA.z <= hitC.z * hitA.x) &&
                 (hitC.z * hitB.x <= hitC.x * hitB.z))) {
                M2V = game_uso_func_082880(lrd.x * lrd.x + lrd.z * lrd.z);
                if (n->width * 250.0f < M2V) {
                    game_uso_func_00003FAC(&fA.x, &fB.x, &lrd.x, M2V, n->width * 250.0f);
                    if (hit != 0) {
                        if (hitA.z * fA.x < hitA.x * fA.z) {
                            R = fA; hitA.x = R.x; hitA.y = R.y; hitA.z = R.z;
                        }
                        if (hitB.x * fB.z < hitB.z * fB.x) {
                            R = fB; hitB.x = R.x; hitB.y = R.y; hitB.z = R.z;
                        }
                    } else {
                        R = fA; hitA.x = R.x; hitA.y = R.y; hitA.z = R.z;
                        R = fB; hitB.x = R.x; hitB.y = R.y; hitB.z = R.z;
                    }
                    hit = 1;
                }
                R = hitC; result.x = R.x; result.y = R.y; result.z = R.z;
                b = n;
                continue;
            }
            break;
        }
        if (n != 0 &&
            game_uso_func_082880(result.x * result.x + result.z * result.z) <
                s->w->speed * s->range) {
            n0 = hitC;
            game_uso_func_071028(&n0);
            n1 = hitA;
            game_uso_func_071028(&n1);
            n2 = hitB;
            game_uso_func_071028(&n2);
            if (((n0.x * n2.x + n0.z * n2.z) < (n0.x * n1.x + n0.z * n1.z)) + 0 * (int)(n0.x * n2.x + n0.z * n2.z + n0.x + n0.z + n0.x * n0.z)) {
                R = hitA; result.x = R.x; result.y = R.y; result.z = R.z;
            } else {
                R = hitB; result.x = R.x; result.y = R.y; result.z = R.z;
            }
        }
    }
    if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} 
    return result;
}
