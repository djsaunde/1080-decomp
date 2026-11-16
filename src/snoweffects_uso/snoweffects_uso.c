#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"
#include "gdl/item.h"

extern int gl_func_00000000();
extern char D_00000000;

typedef struct { int a, b, c, d; } Quad4;
typedef struct { int a, b, c; } Tri3i;
typedef struct { float x, y, z; } Vec3;

/* the 0x40-byte course item the ItemType create methods allocate (0x7D8C / 0x91BC, the
 * edd_ItemType_create_000003BC twin): a ColourItem plus one word the create clears */
typedef struct SnowColourItem {
    /* 0x00 */ ColourItem base;
    /* 0x3C */ s32 unk3C;
} SnowColourItem;

/* one 0x58-byte snow particle record (SnowPool items) */
typedef struct SnowParticle {
    /* 0x00 */ char unk00[0x30];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ char unk34[0x24];
} SnowParticle;

/* a Node-derived particle pool: items[max] from mem_new, count in use */
typedef struct SnowPool {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ SnowParticle *items;
    /* 0x34 */ s32 unk34[2];
    /* 0x3C */ s32 count;
    /* 0x40 */ s32 max;
} SnowPool;

float snoweffects_uso_func_00000000(Vec3 *a, Vec3 *b);

float snoweffects_uso_func_00000000(Vec3 *a, Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float snoweffects_uso_func_00000030(float *p, float *pl) {
    return -(p[1] * pl[1] + p[2] * pl[2] - pl[3]) / pl[0];
}

float snoweffects_uso_func_00000064(float *p, float *pl) {
    return -(p[0] * pl[0] + p[2] * pl[2] - pl[3]) / pl[1];
}

float snoweffects_uso_func_00000098(float *p, float *pl) {
    return -(p[0] * pl[0] + p[1] * pl[1] - pl[3]) / pl[2];
}

void snoweffects_uso_func_000000CC(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00000108(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00000144(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_0000019C(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

void snoweffects_uso_func_0000020C(SnowPool *this, int n) {
    this->items = (SnowParticle *)CALL(mem_new)(n * sizeof(SnowParticle));
    this->count = 0;
    this->max = n;
    VT_SLOT(this->node.vptr, 11).fn((char *)this + VT_SLOT(this->node.vptr, 11).delta);
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000270);

SnowParticle *snoweffects_uso_func_00000680(SnowPool *this) {
    float best = 65536.0f;
    SnowParticle *p = this->items;
    SnowParticle *found = 0;
    int i;
    for (i = 0; i < this->count; i++, p++) {
        if (p->unk30 < best) {
            best = p->unk30;
            found = p;
        }
    }
    return found;
}

SnowParticle *snoweffects_uso_func_000006D0(SnowPool *this) {
    if (this->count < this->max) {
        this->count++;
        return &this->items[this->count - 1];
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000718);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000097C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000A7C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000B44);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00000C90);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000015D0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00001A18);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00001E60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00002310);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000027A8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000028E0);

void snoweffects_uso_func_00003544(char *this, float *out, Vec3 *v) {
    out[0] = snoweffects_uso_func_00000000(v, (Vec3 *)(this + 0x74));
    out[1] = snoweffects_uso_func_00000000(v, (Vec3 *)(this + 0x80));
    out[2] = snoweffects_uso_func_00000000(v, (Vec3 *)(this + 0x8C));
}

void snoweffects_uso_func_000035B0(char *this, float *n, float *p, float s) {
    Vec3 t;
    (void)&this;
    t.x = p[0] - n[0] * s;
    t.y = p[1] - n[1] * s;
    t.z = p[2] - n[2] * s;
    n[3] = snoweffects_uso_func_00000000(&t, (Vec3 *)n);
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003620);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000374C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000039C4);

void snoweffects_uso_func_00003CA4(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_00000108((float *)(a0 + 0x10));
}

void snoweffects_uso_func_00003CD4(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_00000144((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D04(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_000000CC((int *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D34(char *a0) {
    int tag;
    snoweffects_uso_func_000000CC(&tag);
    snoweffects_uso_func_0000019C((Vec3 *)(a0 + 0x10));
}

void snoweffects_uso_func_00003D70(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00003DAC(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00003DE8(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_00003E40(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003EB0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00003EE4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000040B4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004188);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004374);

int snoweffects_uso_func_000046BC(char *this) {
    if (*(int *)(this + 0x144) >= 2) {
        if (--*(int *)(this + 0x84) < 0) {
            *(int *)(this + 0x84) = *(int *)(this + 0x88);
            CALL(snoweffects_uso_func_0000535C)(this);
        }
        return 1;
    }
    *(int *)(this + 0x84) = 1;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004714);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000047D4);

void snoweffects_uso_func_00004E4C(char *a0) {
    *(float *)(a0 + 0x50) = 1.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00004E5C);

void snoweffects_uso_func_000052CC(int a0) {
    (void)&a0;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000052D4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000535C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000053E8);

void snoweffects_uso_func_00005440(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000544C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000559C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00005FD4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006144);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006248);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000634C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000063B4);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006420);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000064B8);

void snoweffects_uso_func_00006B08(char *a0) {
    *(int *)(a0 + 0x1C8) = 0;
    *(int *)(a0 + 0x54) |= 2;
    *(int *)(a0 + 0x1CC) = 0;
    *(float *)(a0 + 0x1D8) = 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00006B28);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000070C0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000718C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_000078E8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007960);

void snoweffects_uso_func_00007CCC(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003DAC((float *)(a0 + 0x10));
}

void snoweffects_uso_func_00007CFC(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003DE8((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_00007D2C(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003D70((int *)(a0 + 0x10));
}

void snoweffects_uso_func_00007D5C(char *a0) {
    int tag;
    snoweffects_uso_func_00003D70(&tag);
    snoweffects_uso_func_00003E40((Vec3 *)(a0 + 0x10));
}

void *snoweffects_uso_func_00007D8C(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        ((SnowColourItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void snoweffects_uso_func_00007E20(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00007E5C(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_00007E98(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_00007EF0(Vec3 *dst) {
    int pad_top[1];
    Tri3i raw;
    int pad_mid[2];
    Tri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst->x = *(float*)&tmp.a;
    dst->y = *(float*)&tmp.b;
    dst->z = *(float*)&tmp.c;
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007F60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00007FE0);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00008C60);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00008D70);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009010);

void snoweffects_uso_func_000090F0(int a0, int a1) {
    (void)&a0;
    (void)&a1;
}

void snoweffects_uso_func_000090FC(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007E5C((float *)(a0 + 0x10));
}

void snoweffects_uso_func_0000912C(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007E98((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_0000915C(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007E20((int *)(a0 + 0x10));
}

void snoweffects_uso_func_0000918C(char *a0) {
    int tag;
    snoweffects_uso_func_00007E20(&tag);
    snoweffects_uso_func_00007EF0((Vec3 *)(a0 + 0x10));
}

void *snoweffects_uso_func_000091BC(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int*)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        ((SnowColourItem *)p1)->unk3C = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int*)p1[0x40 / 4];
    if ((int*)p1[0x40 / 4] != 0) {
        CALL(list_append)((char*)p2 + 0x10, head);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

void snoweffects_uso_func_00009250(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_0000928C(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

void snoweffects_uso_func_000092C8(Quad4 *dst) {
    Quad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

void snoweffects_uso_func_00009320(void) {
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009328);

void snoweffects_uso_func_00009704(char *a0) {
    CALL(gfx_setBlendColour)(&D_00000000, a0 + 0x288);
    CALL(SNode_display)(a0);
}

void snoweffects_uso_func_0000973C(char *a0) {
    CALL(Parms_load_00035C6C)(a0 + 0x240);
    CALL(SNode_loadData)(a0);
}

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009768);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009884);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009BE8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009C48);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009E10);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009E88);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009F44);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_00009FE8);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A10C);

INCLUDE_ASM("asm/nonmatchings/snoweffects_uso/snoweffects_uso", snoweffects_uso_func_0000A760);

void snoweffects_uso_func_0000A870(char *a0) {
    int tag;
    snoweffects_uso_func_00009250(&tag);
    snoweffects_uso_func_0000928C((float *)(a0 + 0x10));
}

void snoweffects_uso_func_0000A8A0(char *a0) {
    int tag;
    snoweffects_uso_func_00009250(&tag);
    snoweffects_uso_func_000092C8((Quad4 *)(a0 + 0x10));
}

void snoweffects_uso_func_0000A8D0(char *a0) {
    int tag;
    snoweffects_uso_func_00009250(&tag);
    snoweffects_uso_func_00009250((int *)(a0 + 0x10));
}

