/* IDO 7.1 NCC (C++) NM donor for func_0000D900 (event handler, 0x4C4 / 305
 * words). The dispatcher is an edg case-order compare chain (20, 9, 6, 1000,
 * 1001, 1003, 1002, 1004, 1005..1007, default), so the function is C++.
 * 10 raw words left (agent-c run 125):
 * - the operator* temps (r sp+56, return temp sp+68) sit 16 bytes below the
 *   target's (72 / 84): the float->double compare temp and the clamp temps are
 *   allocated before them here and after them in the target;
 * - case 6 keeps 0.0 in f12 (target f2);
 * - bu_d900_k990 needs a pin at 0x990 (target spells it func_0000098C + 4).
 * What fixed the rest: the clamp as an inline function with if-returns into a
 * named k (`mov.s f0,f2` copy + 0/50 sharing f12), Vec3 ctor forwarding to
 * set() (x/y/z product temps f0/f2/f14 and the f12 copy of k), dot products in
 * natural x + y + z order, a named Data *p held across the stores, block locals
 * k / s and a block-scope int r for the frame.
 * Spliced into the NON_MATCHING comparison object only. */
struct Vec3 {
    float x, y, z;
    Vec3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    Vec3(float a, float b, float c) { set(a, b, c); }
};
inline Vec3 operator*(const Vec3 &a, float k) { Vec3 r(a.x * k, a.y * k, a.z * k); return r; }
inline float clampf(float v, float lo, float hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct S4 { int w; };
struct Ob { char pad[0xC]; unsigned short id; };
struct Data { Vec3 pos; float speed; char pad10[0x28 - 0x10]; Ob *o; char pad2c[0xE0 - 0x2C]; Vec3 vel; };
struct Ev { int type; Data *data; };
extern "C" {
int func_00000000(...);
extern double D_00000988;
extern double bu_d900_k990;
extern char bu_d900_aec[], bu_d900_b44[];
extern int D_00000000;
}
#define F(p,o) (*(float *)((char *)(p) + (o)))
#define W(p,o) (*(int *)((char *)(p) + (o)))
#define H(p,o) (*(unsigned short *)((char *)(p) + (o)))
extern "C" void func_0000D900(char *self, Ev *ev) {
    S4 sp8C;
    Data *p;
    S4 *sp84;
    Vec3 vA;
    Vec3 d;
    float k;
    float s;
    switch (ev->type) {
    case 20:
        if (W(self, 0xA80) == 0) {
            p = ev->data;
            if (p->speed < D_00000988 && p->o->id == 0x74) {
                func_00000000(self + 0x808);
                return;
            }
            k = clampf(p->speed, 0.0f, 50.0f);
            d = p->pos * k;
            F(self, 0xB4) += d.x;
            F(self, 0xB8) += d.y;
            F(self, 0xBC) += d.z;
            F(self, 0x318) += d.x;
            F(self, 0x31C) += d.y;
            F(self, 0x320) += d.z;
            s = F(self, 0x3BC) * d.x + F(self, 0x3C0) * d.y + F(self, 0x3C4) * d.z;
            F(self, 0xA1C) = F(self, 0xA1C) + s * bu_d900_k990;
            s = F(self, 0x3C8) * d.x + F(self, 0x3CC) * d.y + F(self, 0x3D0) * d.z;
            H(self, 0xA10) |= 0x1F0;
            F(self, 0xA24) = F(self, 0xA24) + -s * 0.5;
            H(self, 0x9A4) = p->o->id;
            W(self, 0xA80) = 10;
        }
        break;
    case 9:
        func_00000000(self);
        break;
    case 6: {
        Vec3 v = ev->data->vel;
        v.x *= 4.0f;
        v.y *= 4.0f;
        v.z *= 4.0f;
        F(self, 0xB4) += v.x;
        F(self, 0xB8) += v.y;
        F(self, 0xBC) += v.z;
        F(self, 0x320) = 0.0f;
        F(self, 0x31C) = 0.0f;
        F(self, 0x318) = 0.0f;
        break;
    }
    case 1000:
        W(self, 0xA54) = 1;
        W(self, 0xA58) |= 4;
        func_00000000(W(self, 0x804));
        func_00000000(bu_d900_aec + 0x4C);
        break;
    case 1001:
        W(self, 0xA54) = 2;
        W(self, 0xA58) |= 4;
        func_00000000(W(self, 0x804));
        func_00000000(bu_d900_aec + 0x54);
        break;
    case 1003:
        W(self, 0xA54) = 3;
        W(self, 0xA58) |= 4;
        func_00000000(bu_d900_b44 + 0x4);
        func_00000000(W(self, 0x804));
        break;
    case 1002:
        W(self, 0xA54) = 4;
        break;
    case 1004:
        W(self, 0xA54) = 5;
        break;
    case 1005:
    case 1006:
    case 1007:
        W(self, 0xA54) = 6;
        func_00000000(bu_d900_b44 + 0x10);
        if (W(self, 0x848) == 2) {
            sp84 = (S4 *)(bu_d900_b44 + 0x24);
            func_00000000(W(self, 0x840));
        } else {
            sp84 = (S4 *)(bu_d900_b44 + 0x28);
            func_00000000(W(self, 0x840));
        }
        sp8C.w = sp84->w;
        int r = func_00000000(D_00000000, sp8C, 0, self);
        if (r != 0) {
            func_00000000(self, r + 0x30);
            return;
        }
        func_00000000(bu_d900_b44 + 0x2C, sp84);
        break;
    default:
        func_00000000(self);
        break;
    }
}
