/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void gl_func_00066264(void *unused, char *source, char *destination, unsigned int length) {
    unsigned int remainder;
    unsigned int aligned;
    int scratch[2];
    remainder = length & 3;
    if (remainder != 0) {
        aligned = length & ~3U;
        gl_66264_transfer(destination, source, aligned);
        gl_66264_transfer((char *)scratch, source + aligned, 4);
        if (remainder != 0) {}
        gl_66264_copy((char *)scratch, destination + aligned, remainder);
    } else {
        gl_66264_transfer(destination, source, length);
    }
}
