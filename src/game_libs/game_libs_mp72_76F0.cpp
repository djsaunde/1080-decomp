/* MIPSpro 7.2.1 C++ (o32 edgcpfe, EDG 2.29) NON_MATCHING donor for gl_func_000076F0 (577 words), compiled by
 * tools/ncc721.sh and spliced into the non_matching object only (agent-h run 355). The ROM build keeps
 * INCLUDE_ASM.
 *
 * The target lowers `switch (sp20)` (0..3) and `switch (self->unk4D8)` (0..3) as 4-entry jump tables, which only
 * EDG 2.29's switch rule produces; the cfe NM body in game_libs.c needs phantom `case 4:` labels. This is that
 * body (agent-g / agent-c runs) with the phantom labels dropped; the 16 cfront-style vtable calls
 * (`vt->e[k].fn(vt->e[k].off + self)`) are plain C++ virtual calls on a class whose vptr is at +0x28 (entry k =
 * virtual function k: same code as the explicit form). 99.0 (cfe) -> 99.0035. agent-h run 364 (kit CDLI):
 * the 4F4 test re-reads the field after the store (as1 forwards it: the target's `or t3,t2,zero`) and its bit
 * test is one signed shift (`sll t5; bgezl t5`), 99.0035 -> 99.2981 (2312 B: the head's extra word is left);
 * the final bit-19 test as one signed shift too (`sll t3; bgezl t3`), -> 99.3328.
 * LEFT: the head `beqzl at; lw t3,0x500(s0)` (target: `beqz at` with the table-index `sll` in the delay slot,
 * although the default block starts with the same `lw t3,0x500(s0)`), t6/t7 ring swaps, a v0/v1 swap on the
 * unk4DC switch value and the float-literal %lo offsets (RoData layout, scored as relocations). */
struct VO76 {
    char pad[0x28];
    virtual int m1(); virtual int m2(); virtual int m3(); virtual int m4(); virtual int m5(); virtual int m6();
    virtual int m7(); virtual int m8(); virtual int m9(); virtual int m10(); virtual int m11(); virtual int m12();
    virtual int m13(); virtual int m14(); virtual int m15(); virtual int m16(); virtual int m17(); virtual int m18();
    virtual int m19(); virtual int m20(); virtual int m21(); virtual int m22(); virtual int m23(); virtual int m24();
    virtual int m25(); virtual int m26(); virtual int m27(); virtual int m28(); virtual int m29(); virtual int m30();
    virtual int m31(); virtual int m32(); virtual int m33(); virtual int m34(); virtual int m35(); virtual int m36();
    virtual int m37();
};
extern "C" {
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
int func_00000000(...);
extern int D_00000000;
extern char D_00000000_76F0;

typedef struct GlObj76F0VtEnt {
    s16 off;
    s16 pad;
    s32 (*fn)(char *);
} GlObj76F0VtEnt;

typedef struct GlObj76F0Vt {
    GlObj76F0VtEnt e[38];
} GlObj76F0Vt;

typedef struct GlObj76F0Sub {
    char pad0[0x80];
    s32 unk80;
} GlObj76F0Sub;

typedef struct GlObj76F0 {
    char pad0[0x28];
    GlObj76F0Vt *vt;
    char pad2C[0x30 - 0x2C];
    s32 unk30;
    char pad34[0x48 - 0x34];
    GlObj76F0Sub *unk48;
    char pad4C[0x4D8 - 0x4C];
    s32 unk4D8;
    s32 unk4DC;
    s32 unk4E0;
    s32 unk4E4;
    s32 unk4E8;
    s32 unk4EC;
    s32 unk4F0;
    s32 unk4F4;
    s32 unk4F8;
    s32 pad4FC;
    s32 unk500;
    char pad504[0x50C - 0x504];
    s32 unk50C;
    s32 pad510;
    s32 unk514;
    s32 unk518;
    char pad51C[0x534 - 0x51C];
    f32 unk534;
    f32 unk538;
    s32 pad53C;
    s32 unk540;
    s32 pad544;
    f32 unk548;
    f32 unk54C;
    f32 unk550;
} GlObj76F0;

extern f32 D_76F0_fadeA;
extern f32 D_76F0_fadeB;
extern f32 D_76F0_fadeC;

void gl_func_000076F0(GlObj76F0 *self) {
    s32 sp24;
    s32 sp20;
    s32 var_v0;
    s32 temp;
    GlObj76F0Vt *vt; /* unused: kept from the cfe body (frame layout) */
    char *varg;      /* unused: kept from the cfe body (frame layout) */

    if (self->unk30 == 0) {
        switch (self->unk4E0) {
        case 0:
            self->unk4E4 = self->unk4E4 - 1;
            if (self->unk4E4 <= 0) {
                self->unk4E4 = self->unk4E8 * 4;
                self->unk4E0 = 1;
                ((VO76 *)self)->m18();
            }
            break;
        case 1:
            sp24 = self->unk4E4 / self->unk4E8;
            self->unk4E4 = self->unk4E4 - 1;
            sp20 = self->unk4E4 / self->unk4E8;
            if (self->unk4F0 & 0x10000) {
                if (sp24 != sp20) {
                    self->unk50C = 0;
                    switch (sp20) {
                    case 0:
                        func_00000000(0x11);
                        self->unk534 = 1.0f;
                        self->unk538 = 1.0f;
                        self->unk550 = 0.0f;
                        self->unk548 = 160.0f;
                        self->unk54C = 120.0f;
                        if (*(s32 *)((char *)&D_00000000_76F0 + 0x34) == 2) {
                            self->unk54C = 60.0f;
                        }
                        break;
                    case 3:
                        self->unk540 = 0xFF;
                        self->unk534 = 1.0f;
                        self->unk538 = 1.0f;
                        func_00000000(0x12);
                        func_00000000(0x10);
                        break;
                    case 1:
                    case 2:
                        self->unk540 = 0xFF;
                        self->unk534 = 1.0f;
                        self->unk538 = 1.0f;
                        func_00000000(0x12);
                        break;
                    }
                }
                if ((sp20 == 0) && (self->unk50C > 0)) {
                    func_00000000((char *)self + 0x5A0);
                    func_00000000((char *)self + 0x5B8);
                    func_00000000((char *)self + 0x5D0);
                    func_00000000((char *)self + 0x5E8);
                }
                self->unk50C = self->unk50C + 1;
            }
            if ((sp24 == 1) && (sp20 == 0)) {
                func_00000000((char *)&D_00000000, 1);
                if (((u32)self->unk4F0 << 15) >> 31) {
                    func_00000000((char *)self + 0x588);
                }
                *(s32 *)((char *)&D_00000000 + 0x78) = 0;
                self->unk4DC = 1;
                func_00000000(*(s32 *)((char *)&D_00000000 + 0x138));
            }
            if (-self->unk4E8 >= (var_v0 = self->unk4E4)) {
                self->unk4E0 = 2;
            }
            break;
        case 2:
            break;
        case 3:
            if (((VO76 *)self)->m19() != 0) {
                if (self->unk518 != 0) {
                    self->unk518 = self->unk518 - 1;
                }
                if (self->unk518 == 0) {
                    self->unk500 = 1;
                }
                self->unk4E4 = self->unk4E4 - 1;
                self->unk4EC = self->unk4EC + 1;
            }
            if ((self->unk4EC >= 0x3D) && (func_00000000((char *)&D_00000000, 0x40100) != 0)) {
                self->unk4E4 = 0;
            }
            if (self->unk4E4 < 0) {
                self->unk4E0 = 7;
                ((VO76 *)self)->m22();
            }
            break;
        case 4:
        case 5:
            if (((VO76 *)self)->m20() != 0) {
                if (self->unk518 != 0) {
                    self->unk518 = self->unk518 - 1;
                }
                if (self->unk518 == 0) {
                    self->unk500 = 1;
                }
                self->unk4E4 = self->unk4E4 - 1;
                self->unk4EC = self->unk4EC + 1;
            }
            if ((self->unk4EC >= 0x1F) && (func_00000000((char *)&D_00000000, 0x40100) != 0)) {
                self->unk4E4 = 0;
            }
            if (self->unk4E4 < 0) {
                self->unk4E0 = 7;
                ((VO76 *)self)->m25();
            }
            break;
        }
        if ((self->unk500 != 0)
            && ((((VO76 *)self)->m28(),
                 temp = self->unk4F8, (temp == 0)) || (temp == 5))) {
            func_00000000(self);
        }
        if (self->unk4E0 == 7) {
            ((VO76 *)self)->m21();
        }
    }
    switch (var_v0 = self->unk4DC) {
    case 1:
        if (self->unk4F0 & 0x10000) {
            if (D_76F0_fadeA < 1.0f) {
                D_76F0_fadeA += 0.1f;
            }
            if (*(s32 *)((char *)&D_00000000_76F0 + 0x34) != 3) {
                if (D_76F0_fadeB < 1.0f) {
                    D_76F0_fadeB += 0.1f;
                }
            }
            if (D_76F0_fadeC < 1.0f) {
                D_76F0_fadeC += 0.1f;
            }
            if (self->unk4DC) {}
        }
        if (self->unk4DC == 0) break;
        if (self->unk30 != 0) break;
        if (self->unk4F4 <= 0) break;
                self->unk4F4 = self->unk4F4 - 1;
                if (self->unk4F4 == 0) {
                    if ((s32)((u32)self->unk4F0 << 11) < 0) {
                        ((VO76 *)self)->m23();
                        func_00000000(self, -1, 0);
                    } else {
                        ((VO76 *)self)->m24();
                    }
                }
        break;
    case 2:
        if (!(self->unk4F0 & 0x10000)) break;
        if (self->unk500 == 0) break;
        {
            if (D_76F0_fadeA > 0.0f) {
                D_76F0_fadeA -= 0.05f;
                if (D_76F0_fadeA < 0.0f) {
                    D_76F0_fadeA = 0.0f;
                }
            }
            if (D_76F0_fadeA <= 0.0f) {
                if (D_76F0_fadeB > 0.0f) {
                    D_76F0_fadeB -= 0.01f;
                    if (D_76F0_fadeB < 0.0f) {
                        D_76F0_fadeB = 0.0f;
                    }
                }
            }
            if ((self->unk4F8 == 0) && (*(s32 *)((char *)&D_00000000_76F0 + 0x34) == 3)) {
                if (D_76F0_fadeC < 1.0f) {
                    D_76F0_fadeC += 0.1f;
                }
            }
        }
        break;
    }
    if ((self->unk4DC != 2) && (self->unk4D8 == 0)) {
        if (self->unk30 == 0) {
            if ((func_00000000(*(s32 *)((char *)&D_00000000 + 0x190)) != 0) && (self->unk48 != 0)) {
                temp = func_00000000((char *)&D_00000000, 0x40000);
                self->unk514 = temp;
                if (temp != 0) {
                    ((VO76 *)self)->m17();
                    func_00000000(self);
                    func_00000000(*(s32 *)((char *)&D_00000000 + 0x138));
                }
            }
        } else {
            var_v0 = self->unk48->unk80;
            if (var_v0 == 0) {
                if (func_00000000((char *)&D_00000000, 0x100) != 0) {
                    self->unk48->unk80 = 0x100;
                }
                if (func_00000000((char *)&D_00000000, 0x40000) != 0) {
                    self->unk48->unk80 = 0x40000;
                }
            } else if (func_00000000((char *)&D_00000000, var_v0 + 0 * var_v0) != 0) {
                ((VO76 *)self)->m26();
                self->unk48->unk80 = 0;
            }
        }
    }
    switch (self->unk4D8) {
    case 0:
        ((VO76 *)self)->m34();
        break;
    case 1:
        ((VO76 *)self)->m35();
        break;
    case 2:
        ((VO76 *)self)->m36();
        break;
    case 3:
        ((VO76 *)self)->m37();
        break;
    }
    if (((s32)((u32)self->unk4F0 << 12) < 0)
        && ((func_00000000((char *)&D_00000000, 0x300) != 0) || (func_00000000((char *)&D_00000000, 0x40000) != 0))) {
        ((VO76 *)self)->m27();
    }
}

}
