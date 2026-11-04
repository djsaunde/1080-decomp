/* IDO 7.1 -O2 donor for gl_func_0004E424 -- 3D distance estimate
 * (0xC4 / 49 words; 49/49 word-exact), agent-g run 264.
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 40304 / 52CD4 recipe; no relocations, no rodata: the 2.0f is lui / mtc1).
 * |dx|, |dy|, |dz| sorted into min / mid / max; returns max + (min + mid) / 2.
 *
 * Load-bearing (history in the post0b placeholder comment):
 *  - z0 (the stack-passed third float) is VOLATILE: the target reads it from its
 *    home once, next to the y1 / z1 loads (agent-c run 178).
 *  - dy / dz are copies of y1 / z1 taken first, then the parameters are zeroed
 *    (copy-then-zero hint drop: y1 / z1 are never read again), then both
 *    subtractions on one line: the loads go straight into dy's f0 / dz's f2 in
 *    the target's y1, z1, z0 order (kit CCCII: a plain `dy = y1 - y0` leaves the
 *    item y1 - y0 coloured and copied).
 *  - the max is its own variable m (zero stays the 0.0f constant only; zero / m
 *    then share f16) and the halved sum goes through x1 (dead by then).
 */
float gl_func_0004E424(float x0, float y0, volatile float z0, float x1, float y1, float z1) {
    float dy;
    float dz;
    float zero = 0.0f;
    float m;

    x1 = x1 - x0;
    dy = y1; y1 = 0;
    dz = z1; z1 = 0;
    dy -= y0; dz = dz - z0;
    if (x1 < zero) x1 = -x1;
    if (dy < zero) dy = -dy;
    if (dz < zero) dz = -dz;
    if (x1 < dy) {
        x0 = x1;
        y0 = dy;
    } else {
        x0 = dy;
        y0 = x1;
    }
    if (dz < y0) {
        m = y0;
        y0 = dz;
    } else {
        m = dz;
    }
    x1 = (x0 + y0) / (float)2;
    return m + x1;
}
