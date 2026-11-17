/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
#include "gdl/gfxlib.h"
#ifdef PORT
#include "port_fb.h" /* the framebuffer model (plan 19.10) */
#endif
void Sun_measureVisibility(GameUsoSentinelObject *obj) {
    int count = 0;
    /* import_8005C348 = bootup Bss+0x3C168, gGfxLib (uso-imports.py sites game) */
    char *cursor = (char *)((GfxLib *)&import_8005C348)->mainWindow;
    unsigned short *data = ((GameUsoSentinelContext *)cursor)->buffer->cells;
    int w;
    int h;
    int i;
    int idx;
    float result;
    float prev;
    cursor = (char *)obj->grid;
    w = ((GameUsoSentinelGrid *)cursor)->width;
    h = ((GameUsoSentinelGrid *)cursor)->height;
    cursor = (char *)game_uso_D_807FF2B4;
    for (i = 0; i < 9; i++, cursor += 4) {
        idx = (obj->y + ((B4Offset *)cursor)->row) * w + obj->x + ((B4Offset *)cursor)->column;
        if (idx > 0 && idx < w * h) {
#ifdef PORT /* plan 19.9: the depth image is big-endian, like the N64's (the read-back stores it so) */
            port_fb_read_depth(data, idx); /* plan 19.10: read back after the next task; this reads last frame's */
            if (data[idx] == PORT_RDP16(0xFFFC)) count++;
#else
            if (data[idx] == 0xFFFC) count++;
#endif
        }
    }
    result = (float)count / 9.0f;
    prev = obj->fraction;
    if (result < prev) {
        obj->fraction = result;
    } else {
        obj->fraction = (float)((double)prev + (double)(result - prev) *
            game_uso_D_807FFA68.coefficient);
    }
}
