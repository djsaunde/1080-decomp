/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00023BDC: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 74.43).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern int D_00000000;
extern int gl_func_0001CA10(...);
extern signed char k;
extern char *rec;
extern char *gl_1dcb4_f5(char *, s32, s32);
extern int gl_ref_00039A30(...);
}
extern "C" void gl_func_00023BDC(s32 arg0, char *arg1, int arg2) {
    s32 sp3C;
    s32 sp48;
    char *rec;
    char **listp;
    char *node;
    char **slot;
    int i;
    u8 cnt1;
    u16 cnt2;
    int cnt3;
    int k;

    sp3C = arg0 * 0x14;
    rec = *(char **)((char *)&D_00000000 + 0x2030) + sp3C;
    sp48 = (s32) *(u8 *)(rec + 0x0);
    cnt1 = *(u8 *)(rec + 0x1);
    cnt2 = *(u16 *)(rec + 0x4);
    if ((*(char **)(arg1 + 0x0) != 0) && (cnt1 != 0)) {
        *(char **)(arg1 + 0x0) = *(char **)(arg1 + 0x0) + (int)arg1;
        if ((s32) cnt1 > 0) {
            i = 0;
            do {
                slot = (char **)(*(char **)(arg1 + 0x0) + i);
                node = *slot;
                if (node != 0) {
                    node = node + (int)arg1;
                    *slot = node;
                    if (*(u8 *)(node + 0x2) == 0) {
                        gl_ref_00039A30(node + 4, arg1, arg2);
                        *(u8 *)(node + 0x2) = 1;
                        *(char **)(node + 0xC) = *(char **)(node + 0xC) + (int)arg1;
                    }
                }
                i += 4;
            } while (i != cnt1 * 4);
        }
    }
    if ((*(char **)(arg1 + 0x4) != 0) && (cnt2 != 0)) {
        *(char **)(arg1 + 0x4) = *(char **)(arg1 + 0x4) + (int)arg1;
        if ((s32) cnt2 > 0) {
            i = 0;
            do {
                node = *(char **)(arg1 + 0x4) + i;
                if ((node != 0) && (*(int *)node != 0)) {
                    gl_ref_00039A30(node, arg1, arg2);
                }
                i += 8;
            } while (i != cnt2 * 8);
        }
    }
    listp = (char **)(arg1 + 8);
    if (sp48 >= 0x7F) {
        sp48 = 0x7E;
    }
    k = 2;
    cnt3 = sp48 + 1;
    if (cnt3 >= 2) {
        do {
            node = *(char **)listp;
            if (node != 0) {
                gl_func_0001CA10(0x1ACA4, k, node, arg1);
                node = *(char **)listp + (int)arg1;
                *listp = node;
                if (*(u8 *)(node + 0x0) == 0) {
                    if (*(u8 *)(node + 0x1) != 0) {
                        gl_ref_00039A30(node + 8, arg1, arg2);
                    }
                    gl_ref_00039A30(node + 0x10, arg1, arg2);
                    if (*(u8 *)(node + 0x2) != 0x7F) {
                        gl_ref_00039A30(node + 0x18, arg1, arg2);
                    }
                    *(u8 *)(node + 0x0) = 1;
                    *(char **)(node + 0x4) = *(char **)(node + 0x4) + (int)arg1;
                }
            }
            k += 1;
            listp += 1;
        } while (cnt3 >= k);
    }
    *(char **)(*(char **)((char *)&D_00000000 + 0x2030) + sp3C + 0xC) = *(char **)(arg1 + 0x0);
    *(char **)(*(char **)((char *)&D_00000000 + 0x2030) + sp3C + 0x10) = *(char **)(arg1 + 0x4);
    *(char **)(*(char **)((char *)&D_00000000 + 0x2030) + sp3C + 0x8) = (char *)((int)arg1 + 8);
}
