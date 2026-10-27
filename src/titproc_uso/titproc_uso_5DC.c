/* titproc_uso_func_000005DC (0x590 / 356 words) -- 12-case jumptable state
 * machine. EXACT under IDO 7.1 cfe -O2 (agent-g run 178). Spliced over the
 * unit's INCLUDE_ASM by REPLACE_FUNC_BODY (Makefile TITPROC_5DC_DONOR) so the
 * switch's donor-local .rodata jumptable reloc is renamed
 * titproc_uso_func_000005DC_rodata and pinned to the table the USO ships at
 * RoData +0x0. Decode history: see the note above the placeholder in
 * titproc_uso.c. */
#include "common.h"
extern char import_00020098;
extern int titproc_uso_func_0120A8();
extern int titproc_uso_func_0027E8();
extern int titproc_uso_func_000C54();
extern int titproc_uso_func_01DA48();
extern int titproc_uso_func_000B6C();
extern int titproc_uso_func_000C0C();
extern int titproc_uso_func_000418();
extern int titproc_uso_func_011CD8();
extern int titproc_uso_func_001E9C();
extern int titproc_uso_func_01EF10();
extern int titproc_uso_func_01EF44();
extern int titproc_uso_func_01EF94();
extern int titproc_uso_func_01EFC8();
extern int import_000B3268();


void titproc_uso_func_000005DC(char *a0, int a1) {
    int d;
    int done;
    int s5v;
    int *r;
    int t;
    int mask;
    d = (int)&import_00020098;
    done = 0;
    do {
        switch (a1) {
        case 0: {
            titproc_uso_func_0120A8(d, 0, 0, 0);
            done = 1;
            *(int *)(d + 0x68) = 0;
            *(int *)(d + 0x8C) = 0x820000;
            r = (int *)titproc_uso_func_0027E8(d, 2, 9, 1);
            t = (int)r;
            titproc_uso_func_07ACE0((char *)&import_00020098 + 0x10, r); if (a0) {}
            if (*(int *)((char *)t + 0x14) != 0) {
                *(int *)((char *)t + 0x4) = 1;
            }
            *(int *)((char *)t + 0x14) = d;
            titproc_uso_func_01DA48(a0, 0, titproc_uso_func_000C54(0, r));
            break;
        }
        case 1:
            titproc_uso_func_000B6C(a0, -1);
            done = 1;
            import_000B3268(a0, 0x9FFF0, 0x10000, *(int *)a0);
            *(int *)(d + 0x44) = 2;
            break;
        case 2:
            titproc_uso_func_000C0C(a0);
            *(int *)(d + 0x40) = 0;
            break;
        case 3: {
            *(int *)(d + 0x40) = *(int *)(d + 0x44), *(int *)(d + 0x44) = 0;
            s5v = 0x800000;
            break;
        }
        case 4: {
            mask = ((s5v | 0x10000) | 0x80000) | 0x898;
            t = titproc_uso_func_000418();
            done = 1;
            titproc_uso_func_0120A8(d, 4, t, 0);
            r = (int *)titproc_uso_func_0027E8(d, t, 10, 1);
            t = (int)r;
            titproc_uso_func_07ACE0((char *)&import_00020098 + 0x10, r);
            if (*(int *)((char *)t + 0x14) != 0) {
                *(int *)((char *)t + 0x4) = 1;
            }
            *(int *)((char *)t + 0x14) = d;
            titproc_uso_func_011CD8(d, 1);
            titproc_uso_func_01DA48(a0, 0, titproc_uso_func_001E9C(0, mask, r, 0));
            *(int *)(d + 0x40) = 0;
            break;
        }
        case 5: {
            mask = ((s5v | 0x1010000) | 0x80000) | 0x898;
            t = titproc_uso_func_000418();
            done = 1;
            titproc_uso_func_0120A8(d, 4, t, 0);
            r = (int *)titproc_uso_func_0027E8(d, t, 11, 2);
            t = (int)r;
            titproc_uso_func_07ACE0((char *)&import_00020098 + 0x10, r);
            if (*(int *)((char *)t + 0x14) != 0) {
                *(int *)((char *)t + 0x4) = 1;
            }
            *(int *)((char *)t + 0x14) = d;
            titproc_uso_func_011CD8(d, 1);
            titproc_uso_func_01DA48(a0, 0, titproc_uso_func_001E9C(0, mask, r, 0));
            *(int *)(d + 0x40) = 0;
            break;
        }
        case 6:
            titproc_uso_func_000C0C(a0);
            s5v = 0x2100000;
            *(int *)(d + 0x40) = 10;
            break;
        case 7: {
            t = s5v & 0x2000000;
            mask = (s5v | 0x8020000) | 0x80000;
            titproc_uso_func_000B6C(a0, 2);
            done = 1;
            *(char *)(d + 0x181) = titproc_uso_func_01EF10(*(int *)(*(int *)a0 + 8));
            *(char *)(d + 0x182) = titproc_uso_func_01EF44(*(int *)(*(int *)a0 + 8));
            *(char *)(d + 0x183) = titproc_uso_func_01EF94(*(int *)(*(int *)a0 + 8));
            if (t != 0) {
                titproc_uso_func_0120A8(d, 10, titproc_uso_func_01EFC8(*(int *)(*(int *)a0 + 8)), 2);
            } else {
                titproc_uso_func_0120A8(d, 4, titproc_uso_func_01EFC8(*(int *)(*(int *)a0 + 8)), 0);
            }
            *(int *)(d + 0x34) = 6;
            t = titproc_uso_func_01EFC8(*(int *)(*(int *)a0 + 8));
            *(int *)(d + 0x64) = t;
            r = (int *)titproc_uso_func_0027E8(d, t, 4, 1);
            *(int *)(d + 0x34) = 1;
            t = (int)r;
            titproc_uso_func_07ACE0((char *)&import_00020098 + 0x10, r);
            if (*(int *)((char *)t + 0x14) != 0) {
                *(int *)((char *)t + 0x4) = 1;
            }
            *(int *)((char *)t + 0x14) = d;
            titproc_uso_func_011CD8(d, 1);
            t = *(int *)a0;
            titproc_uso_func_01DA48(a0, 0, titproc_uso_func_001E9C(0, (int)&((char (*)[2])mask)[0][*(int *)(*(int *)(t + 8) + 4)], r, t));
            *(int *)(d + 0x40) = 11;
            break;
        }
        case 8:
            titproc_uso_func_000C0C(a0);
            s5v = 0x2100000;
            *(int *)(d + 0x40) = 9;
            break;
        case 9: {
            t = s5v & 0x2000000;
            mask = (s5v | 0x8020000) | 0x80000;
            titproc_uso_func_000B6C(a0, 1);
            done = 1;
            *(char *)(d + 0x181) = titproc_uso_func_01EF10(*(int *)(*(int *)a0 + 8));
            *(char *)(d + 0x182) = titproc_uso_func_01EF44(*(int *)(*(int *)a0 + 8));
            *(char *)(d + 0x183) = titproc_uso_func_01EF94(*(int *)(*(int *)a0 + 8));
            if (t != 0) {
                titproc_uso_func_0120A8(d, 10, titproc_uso_func_01EFC8(*(int *)(*(int *)a0 + 8)), 1);
            } else {
                titproc_uso_func_0120A8(d, 4, titproc_uso_func_01EFC8(*(int *)(*(int *)a0 + 8)), 0);
            }
            t = titproc_uso_func_01EFC8(*(int *)(*(int *)a0 + 8));
            *(int *)(d + 0x64) = t;
            r = (int *)titproc_uso_func_0027E8(d, t, 4, 1);
            t = (int)r;
            titproc_uso_func_07ACE0((char *)&import_00020098 + 0x10, r);
            if (*(int *)((char *)t + 0x14) != 0) {
                *(int *)((char *)t + 0x4) = 1;
            }
            *(int *)((char *)t + 0x14) = d;
            titproc_uso_func_011CD8(d, 1);
            t = *(int *)a0;
            titproc_uso_func_01DA48(a0, 0, titproc_uso_func_001E9C(0, (int)&((char (*)[2])mask)[0][*(int *)(*(int *)(t + 8) + 4)], r, t));
            *(int *)(d + 0x40) = 11;
            break;
        }
        case 10:
            break;
        case 11:
            titproc_uso_func_000C0C(a0);
            *(int *)(d + 0x40) = *(int *)(d + 0x44);
            break;
        }
        a1 = *(int *)(d + 0x40);
    } while (done == 0);
    /* t is uninitialised on the case-1 exit path. Allowed only because the target
     * itself relies on an uninitialised local in this same function: its prologue
     * loads s5v from its never-written home (`lw s5,0x5C(sp)`, 0x8FB5005C at
     * 0x620; s5v is only assigned in cases 3/6/8 and read in 4/5/7/9). This empty
     * test keeps t live around the loop so the address constant outranks it
     * (d = s0, t = s1 as in the target). */
    if (t) {}
}
