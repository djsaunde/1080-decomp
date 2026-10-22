/* IDO 7.1 NCC (C++) donor for game_libs_func_0002F720 (0x180 / 96 words): sound-voice
 * parameter update (pan from a signed byte, three table lookups through the 0x43824
 * helper, a damped float, a clamped rate). Two spellings the cfe NM could not reach:
 * the high-nibble byte is a store plus a compound shift `p[0xF] = t / 2; p[0xF] <<= 4;`
 * (store-forwarded, both stores kept; a plain double store is DSE'd), and edg keeps
 * the source order of `fl * K` and of the 0xE store after the rate. The FP pool reads
 * are per-site symbols pinned at their pool offsets. Spliced into game_libs_post.c.o
 * via REPLACE_FUNC_BODY. */
typedef float f32;
extern "C" {
int gl_ref_00043824(float, ...);
extern int D_00000000;
extern int D_2F720_t1, D_2F720_t2, D_2F720_t3;
extern int D_2F720_lut, D_2F720_flut;
extern int D_2F720_p1740, D_2F720_p1744, D_2F720_p1748, D_2F720_p174C;
void game_libs_func_0002F720(char *o, f32 in, signed char idxb) {
    float dlt;
    int v;
    char *p;
    int t;
    float fl;
    float lo;

    in = in * 180.0f;
    dlt = in - *(float *)(o + 0x5C);
    *(float *)(o + 0x5C) = in;
    v = idxb;
    p = o + 0x54;
    if (idxb >= 0x40) {
        v = 0x7F - idxb;
    }
    if (v < -0x40) {
        v = -0x80 - v;
    }
    *(unsigned char *)(p + 0xC) = v + 0x40;

    t = gl_ref_00043824(in, (char *)&D_2F720_t1, 8);
    *(unsigned char *)(p + 0xD) = *((unsigned char *)&D_2F720_lut + t);
    t = gl_ref_00043824(in, (char *)&D_2F720_t2, 0x10);
    fl = *(float *)((char *)&D_2F720_flut + t * 4);
    *(float *)(p + 0) = (*(float *)(p + 0) * *(float *)&D_2F720_p1740) + (fl * *(float *)&D_2F720_p1744);
    t = gl_ref_00043824(in, (char *)&D_2F720_t3, 0x10);
    lo = *(float *)&D_2F720_p1748;
    *(unsigned char *)(p + 0xF) = t / 2;
    *(unsigned char *)(p + 0xF) <<= 4;

    *(float *)(p + 4) = 1.0f - (dlt * 0.0625f);
    *(unsigned char *)(p + 0xE) = idxb & 0x80;
    if (*(float *)(p + 4) < lo) {
        *(float *)(p + 4) = lo;
    }
    if (*(float *)&D_2F720_p174C < *(float *)(p + 4)) {
        *(float *)(p + 4) = *(float *)&D_2F720_p174C;
    }
}
}
