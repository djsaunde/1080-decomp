/* IDO 7.1 NCC (C++) NM-only comparison donor for SnowBCollider_onContact (collision impulse, 209 words).
 * agent-g run 157 / 182: the cfe NM body under NCC (amt in $f20), 97.77.
 * agent-g run 184 (kit CXXXIII): rewritten as C++ -- `lwc1 180(a0); addiu a0,a0,180` = an inline
 * V3::operator+=(const V3 &) on b->ctx->vel (and ctx->acc), `addiu v1,v1,2408 / 2576` = edg's compound-assignment
 * lvalue temps (`ctx->imp += amt`, `ctx->fl |= b->fl`), ctx->cnt as `cnt = cnt + 1`; frame 184 = target (was 200);
 * t declared in its own block; `q = (t < 0.0f) ? -t : t`. The surface-code call returns an unsigned char
 * (per-site alias func_8bd4_c): that brings back the target's `move a1,v0` copy of the code before the 0x62 test
 * (as a0 here). 97.77 -> 97.92. LEFT: the code copy register (a0 vs a1), the second scaledBy result 16 high.
 * agent-c run 204: the 0x61/0x62/0x63 skip test is ONE `||` expression on an int code (the target's
 * xori/sltiu/bnez chain with the code copy in a1 and v0 consumed by the 0x62 test), and ctx->acc accumulates
 * through a reference-returning V3::add (the acc lvalue temp then takes v0 with the target's addiu order):
 * 97.84 -> 98.73 standalone. LEFT: the vel += temp (a0 in the target; a ref-returning += there fixes it but
 * grows the frame by 8) and the second scaledBy result 16 high.
 * agent-c run 238: EXACT (98.83 -> 100). (1) The second scaling is its own inline (scaledBy2) whose result r is
 * declared AFTER hx / hy / hz (`f32 hx, hy, hz; V3 r; hx = ...`): the three scalar homes then sit above r, so r
 * lands 16 bytes lower (target sp+64); (2) the else arm's |f970| is one variable (`q = f970; q = q < 0 ? -q : q;`),
 * one home fewer below r, so the frame stays 184; (3) vel += impA through a named reference
 * (`V3 &v = b->ctx->vel; v += impA;`): the vel pointer takes a0 with the target's lwc1 / addiu order.
 * Spliced over a placeholder on both build paths via REPLACE_FUNC_BODY.
 * agent-g run 475: the Body / Ctx views renamed to SnowBCollider / RiderView with snowboarder.h field names (same bytes). */
typedef float f32;
typedef unsigned short u16;
extern "C" {
extern char D_00000000;
extern int func_00000000(...);
/* per-site import aliases (USO relocs at 0; agent-g run 475 resolved the targets with the bootup TextReloc table): */
extern unsigned char func_8bd4_c(...);          /* BasePolyShape_func_00052A4C(shape, tri): the triangle's surface code */
extern void func_8bd4_a(void *, void *, void *, void *, float); /* SnowBCollider_func_0000E2D0: the edge-grip response */
extern void func_8bd4_b(void *, float, void *, float);          /* Friction_vfn11_00063F34 (the collider's base) */
extern void func_8bd4_heavy(void *, void *, void *);            /* DNode_addImpulseAt_00065250(rider, point, force) */
}
struct V3 {
    f32 x, y, z;
    V3() {}
    void operator+=(const V3 &o) { x += o.x; y += o.y; z += o.z; }
    V3 &add(const V3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
};
static inline V3 scaledBy(const V3 &d, f32 s) {
    V3 r; f32 hx = d.x * s; f32 hy = d.y * s; f32 hz = d.z * s;
    r.x = hx; r.y = hy; r.z = hz;
    return r;
}
static inline V3 scaledBy2(const V3 &d, f32 s) {
    f32 hx, hy, hz; V3 r; hx = d.x * s; hy = d.y * s; hz = d.z * s;
    r.x = hx; r.y = hy; r.z = hz;
    return r;
}
/* The rider (include/game/snowboarder.h SnowBoarder) as this donor sees it: the same offsets, with V3 members so
 * the inline V3 operators (part of the match) apply. pos 0xB4, contactCount 0x938, contactNormal 0x93C,
 * contactDepth 0x968, edge 0x970, contactSurface 0x9A4, hitFlags 0xA10. */
struct RiderView {
    char p0[0xB4]; V3 pos; char pC0[0x938 - 0xC0];
    int contactCount; V3 contactNormal; char p948[0x968 - 0x948]; f32 contactDepth; char p96C[4]; f32 edge;
    char p974[0x9A4 - 0x974]; short contactSurface; char p9A6[0xA10 - 0x9A6]; u16 hitFlags;
};
/* the import behind D_00000000 here: +0x84 is the course collision shape (procColl queries the same word) */
struct GameWorld { char p[0x84]; int courseShape; };
/* SnowBCollider (0x178, vtbl bootup Data 0x8698, ctor 8B44): one collision probe of the rider. flags (read from what
 * each branch does): 0x1F0 = a body probe (a non-snow hit pushes the rider back and sets the hitFlags that pick a
 * stumble), 0xF = a board probe (edge grip and friction). */
struct SnowBCollider {
    char p0[0x114];
    char pointVel[0xC];     /* 0x114: velocity at the probe; SnowBCollider_func_0000E2D0 projects it onto the contact plane */
    char worldPoint[0x2C];  /* 0x120: probe position in world space (the impulse application point) */
    int hitTri;             /* 0x14C: index of the touched course triangle (surface lookup) */
    char p150[0x170 - 0x150];
    RiderView *rider;       /* 0x170 */
    u16 flags;              /* 0x174: ORed into rider->hitFlags on contact */
};
extern "C" void SnowBCollider_onContact(SnowBCollider *b, float amt, V3 *dir, int a3) {
    u16 fl = b->flags;
    int skip;
    int code;
    V3 impA, nnA;

    if (fl & 0x1F0) {
        if (amt < 0.0f) {
            code = func_8bd4_c((*(GameWorld **)&D_00000000)->courseShape, b->hitTri);
            skip = code == 0x61 || code == 0x62 || code == 0x63;
            if (!skip) {
                b->rider->contactCount++;
                b->rider->contactSurface = code;
                if (amt < -30.0f) amt = -30.0f;
                b->rider->hitFlags |= b->flags;
                impA = scaledBy(*dir, (float)((double)-amt * 0.5));
                { V3 &v = b->rider->pos; v += impA; }
                b->rider->contactNormal.add(*dir);
                b->rider->contactDepth += amt;
                nnA = scaledBy2(*dir, -amt);
                func_8bd4_heavy(b->rider, b->worldPoint, &nnA);
            }
        }
        return;
    }
    if (fl & 0xF) {
        if (amt < 50.0f) {
            b->rider->contactCount++;
            b->rider->hitFlags |= b->flags;
            b->rider->contactDepth += amt;
            b->rider->contactNormal.add(*dir);
            f32 q = b->rider->edge;
            q = (q < 0.0f) ? -q : q;
            func_8bd4_a(b->rider, b->worldPoint, b->pointVel, dir, q * q);
            func_8bd4_b(b, amt, dir, 0.1f);
        }
    }
}
