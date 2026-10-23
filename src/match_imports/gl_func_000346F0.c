/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
char *gl_func_000346F0(unsigned int context, unsigned char *src, int count) {
    extern int gl_346F0_line_write(int *, int, unsigned char *, int);
    unsigned char buf[0x108];
    unsigned int out;
    int i;
    int rem;
    unsigned char *p;
    unsigned char *writer;
    writer = *(unsigned char **)((char *)&D_00000000 + 8);
    if (writer != 0) {
        gl_346F0_line_write((int *)writer, 0, src, count);
    }
    if (count >= 0xFF) count = 0xFF;
    out = 0;
    i = 0;
    if (count > 0) {
        context = count & 3;
        do {
            if (context != 0) {
                rem = context;
                if (1) {}
                p = src;
remainder_loop:
                context = out++;
                buf[context] = *p++ & 0xFFFF;
                i++;
                if (rem != i) goto remainder_loop;
                if (i == count) break;
            }
            p = src + i;
            writer = count + src;
copy_loop:
            context = out++;
            buf[context] = p[0];
            context = out++;
            buf[context] = p[1];
            context = out++;
            buf[context] = p[2];
            context = out++;
            buf[context] = p[3];
            p += 4;
            if (p != writer) goto copy_loop;
        } while (0);
    }
    context = out, out++, buf[context] = 0;
    if (1) { buf[out] = 0; }
    return (char *)1;
}
