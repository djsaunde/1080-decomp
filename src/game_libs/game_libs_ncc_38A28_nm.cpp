/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00038A28: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 52.99; if (c) {} after the key >= 40 report call from a parameter-only fz sweep, 61.2, 87 -> 85 of 81 words).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
extern char *p;
extern int gl_proto_3787C(void*, void*, void*, int, float, int);
extern char *o;
extern char *c;
}
extern "C" void gl_func_00038A28(char *o, char *c) {
    int code = *(int *)c;
    unsigned int i = (unsigned int)(code - 0x64);
    int *p;
    int key;
    int r;
    if (i >= 9) {
        gl_func_00000000((char *)&D_00000000 + 0x0001EC9C, *(int *)(o + 0xC));
        return;
    }
    switch (code) {
        case 0x64:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            *(int *)(o + 0x20) = *p;
            break;
        case 0x65:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            *(int *)(o + 0x0C) = *p;
            break;
        case 0x66:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            if (key >= 40) {
                gl_func_00000000((char *)&D_00000000 + 0x0001EC6C, key); if (c) {}
            }
            r = gl_func_00000000(*(char **)((char *)&D_00000000 + key * 4) + 0x10, o);
            if (*(int *)(o + 0x14) != 0) {
                *(int *)(o + 0x4) = 1;
            }
            *(int *)(o + 0x14) = r;
            break;
        case 0x67:
            p = *(int **)(c + 0x4);
            *(int **)(c + 0x4) = p + 1;
            key = *p;
            if (key >= 40) {
                gl_func_00000000((char *)&D_00000000 + 0x0001EC84, key);
            }
            *(char **)((char *)&D_00000000 + key * 4) = o;
            break;
        default:
            break;
    }
}
