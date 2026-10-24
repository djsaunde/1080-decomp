/* IDO 7.1 cfe -O2 donor for func_80001EC8 (0x2DC / 183 words): the USO
 * section-record loader loop (see the placeholder in kernel_000.c).
 *
 * Decode (agent-g runs 144-145): a 12-byte record at 88(sp) between two unused
 * words; buf = 0 before the size test; the type-8 (BSS) arm clears the buffer
 * by INDEX (`((char *)buf)[i] = 0`; a `*p++` cursor schedules the size test
 * into a beqzl); the flag-1 reloc path writes the sub-size back into rec.size;
 * the size table add goes through an integer-cast address, so uopt treats the
 * store as aliasing the address-taken record and reloads rec.type for the
 * switch (a named-array store keeps type in a register: one word short).
 * The source-order switch emits a 12-entry table indexed type-1. The splice
 * renames its .rodata reloc to func_80001EC8_rodata, pinned so that the
 * reference resolves to jtbl_8000A64C, the table the ROM already carries
 * (runtime addresses, kernel +0x400). Kept out of C_FILES (Makefile). */
typedef struct { int type; unsigned int size; unsigned char b8; unsigned char flags; } Rec;
typedef struct {
    char pad_00[0x34];
    void *(*lookup34)();
    char pad_38[0x4C];
    void (*reloc84)();
    char pad_88[0x10];
    void *(*alloc98)();
    void (*final9C)();
} Cb;
extern Cb D_80012BC0;
extern int func_800009D8();
extern int func_800027E0();
extern int D_80013004;
extern int D_8000A384;
extern int D_80012D30[];
extern int D_80012C64;
extern int D_80012C68;
int func_80001EC8(int *mod, void *file) {
    int pad1;
    Rec rec;
    int pad2;
    void *buf;
    int err = 0;

    do {
        if (func_800009D8(&rec, sizeof(Rec), 1, file) < 0) {
            return D_80013004;
        }
        buf = 0;
        if (rec.size != 0) {
            if (rec.flags & 1) {
                buf = D_80012BC0.alloc98(rec.size);
            } else {
                buf = D_80012BC0.lookup34(rec.size, rec.b8);
            }
            if (buf == 0) {
                return -18;
            }
            if (rec.type == 8) {
                char *p;
                unsigned int i;
                for (i = 0; i < rec.size; i++) {
                    ((char *)buf)[i] = 0;
                }
            } else {
                if (func_800009D8(buf, rec.size, 1, file) < 0) {
                    return D_80013004;
                }
                if (rec.flags & 1) {
                    void *sub = ((void **)buf)[1];
                    void *sub2;
                    D_80012BC0.reloc84(&D_8000A384, rec.size, sub);
                    sub2 = D_80012BC0.lookup34(sub, rec.b8);
                    func_800027E0(buf, sub2);
                    rec.size = (unsigned int)sub;
                    D_80012BC0.final9C(buf);
                    buf = sub2;
                }
            }
            if (rec.type < 14) {
                mod[rec.type + 14] = (int)buf;
            }
        }
        if (rec.type < 14) {
            *(int *)((unsigned int)D_80012D30 + rec.type * 4) += rec.size;
        }
        switch (rec.type) {
        case 12:
            return -27;
        case 9:
            D_80012C64 = *(int *)buf;
            D_80012C68 = ((short *)mod)[0x38];
            break;
        case 1:
            mod[rec.type] = rec.size / sizeof(Rec);
            break;
        case 2:
        case 3:
        case 4:
            mod[rec.type] = rec.size / sizeof(Rec);
            break;
        default:
            mod[rec.type] = rec.size;
            break;
        }
    } while (rec.type != 11 && err == 0);
    return 0;
}
