/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
int gl_func_00060AD0(LineBuf60BD4 *self, int a1, unsigned char *src, int count) {
    unsigned char *cursor;
    LineRec60BD4 *rec;
    unsigned char *end;
    unsigned char *saved_end;
    if (count > 0) {
        end = count + src;
        saved_end = end;
        cursor = src;
        do {
            unsigned char ch = *cursor;
            rec = &self->recs[self->cur];
            if (ch == 10) {
                rec->buf[rec->len] = 0;
                gl_line_advance_60BD4(self);
            } else {
                int len = rec->len;
                if (len >= self->limit) {
                    rec->buf[len] = 0;
                    gl_line_advance_60BD4(self, len);
                    end = 0;
                    if (saved_end == (unsigned char *)10) {}
                    cursor--;
                } else {
                    rec->len = len + 1;
                    rec->buf[len] = ch;
                }
            }
            cursor++;
        } while (cursor < count + src);
    }
    rec = &self->recs[self->cur];
    rec->buf[rec->len] = 0;
    (void)a1;
    return 1;
}
