/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
void game_uso_func_0000B4B8(GameUsoSentinelObject *obj) {
    int count = 0;
    char *cursor = *(char **)(&import_8005C348 + 0x240);
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
            if (data[idx] == 0xFFFC) count++;
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
