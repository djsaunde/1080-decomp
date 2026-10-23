/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void gl_func_00068C14(int *self) {
    extern int D_00000000;
    extern int gl_68C14_table;
    extern void gl_68C14_check(void *);
    extern void gl_68C14_assert(const char *, const char *, int);
    extern void gl_68C14_report(const char *, ...);
    extern int *gl_68C14_create(int *, int, int);
    extern void gl_68C14_append(List66620 *, int);
    int count1;
    int *bufstruct;
    int *obj;
    int *p;
    int pad[1];
    int count2;
    (void)&count1;
    gl_68C14_check(self);
    bufstruct = (int *)self[0];
    p = (int *)bufstruct[0];
    bufstruct[0] = (int)(p + 1);
    count2 = p[0];
    self[1] = count2;
    count1 = count2;
    if (count2 >= 4) {
        gl_68C14_assert((char *)&D_00000000 + 0x2C4F4, (char *)&D_00000000 + 0x2C508, 221);
    }
    bufstruct = (int *)self[0];
    p = (int *)bufstruct[0];
    bufstruct[0] = (int)(p + 1);
    count2 = p[0];
    self[1] = count2;
    if (count2 == 0) {
        gl_68C14_report((char *)&D_00000000 + 0x2C538, count1, count2);
    }
    obj = gl_68C14_create(0, self[0x14 / 4], count2);
    ((int *)self[0x3C / 4])[0x18 / 4] = (int)obj;
    self[0x10 / 4] = (int)obj;
    self[0x18 / 4] = (int)obj;
    p = (int *)((int *)self[0x3C / 4])[0x10 / 4];
    gl_68C14_append((List66620 *)((char *)p + 0x10), (int)obj);
    if (((AttachmentPrefix *)obj)->field_14 != 0) {
        ((AttachmentPrefix *)obj)->field_04 = 1;
    }
    ((AttachmentPrefix *)obj)->field_14 = p;
    *(int *)((char *)&gl_68C14_table + count1 * 4) = self[0x18 / 4];
}
