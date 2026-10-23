/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
s32 game_libs_func_0002F1B8(f32 key, f32 *arr, s32 n) {
    s32 mid = n >> 1;
    s32 step = mid + 1;
    s32 nxt;
    f32 d, d2, d3;
    f32 *p;
    step = step >> 1;
loop:
    p = (f32 *)((char *)arr + mid * 4);
    nxt = mid + 1;
    d = key - p[0];
    if (0.0f < d) {
        if (n == nxt) {
            goto retmid;
        }
        d2 = p[1] - key;
        if (0.0f < d2) {
            if (!(d2 < d)) {
                goto retmid;
            }
            mid++;
            goto retmid;
        }
        mid += step;
        step = step + 1;
        step = step >> 1;
        goto loop;
    }
    if (mid == 0) {
        goto retmid;
    }
    d = -d;
    d3 = key - p[-1];
    if (0.0f < d3) {
        if (!(d3 < d)) {
            goto retmid;
        }
        mid--;
        goto retmid;
    }
    mid -= step;
    step = step + 1;
    step = step >> 1;
    goto loop;
retmid:
    return mid;
}
