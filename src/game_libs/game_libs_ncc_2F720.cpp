/* IDO 7.1 NCC (C++) donor for snd_SoundObjSetAngle (0x180 / 96 words): sound-voice
 * parameter update (pan from a signed byte, three table lookups through the 0x43824
 * helper, a damped float, a clamped rate). Two spellings the cfe NM could not reach:
 * the high-nibble byte is a store plus a compound shift `p[0xF] = t / 2; p[0xF] <<= 4;`
 * (store-forwarded, both stores kept; a plain double store is DSE'd), and edg keeps
 * the source order of `fl * K` and of the 0xE store after the rate. The FP pool reads
 * are per-site symbols pinned at their pool offsets. Spliced into game_libs_post.c.o
 * via REPLACE_FUNC_BODY. */
extern "C" {
#include "../../include/audio.h"
int gl_ref_00043824(float, ...);
extern int D_00000000;
extern int D_2F720_t1, D_2F720_t2, D_2F720_t3;
extern int D_2F720_lut, D_2F720_flut;
extern int kRo_snd_SoundObjSetAngle_1740, kRo_snd_SoundObjSetAngle_1744, kRo_snd_SoundObjSetAngle_1748, kRo_snd_SoundObjSetAngle_174C;
void snd_SoundObjSetAngle(SoundObj *o, f32 in, signed char idxb) {
    float dlt;
    int v;
    SoundVoice *p;
    int t;
    float fl;
    float lo;

    in = in * 180.0f;
    dlt = in - o->voice.angle;
    o->voice.angle = in;
    v = idxb;
    p = &o->voice;
    if (idxb >= 0x40) {
        v = 0x7F - idxb;
    }
    if (v < -0x40) {
        v = -0x80 - v;
    }
    p->pan[0] = v + 0x40;

    t = gl_ref_00043824(in, (char *)&D_2F720_t1, 8);
    p->pan[1] = *((unsigned char *)&D_2F720_lut + t);
    t = gl_ref_00043824(in, (char *)&D_2F720_t2, 0x10);
    fl = *(float *)((char *)&D_2F720_flut + t * 4);
    p->volScale = (p->volScale * *(float *)&kRo_snd_SoundObjSetAngle_1740) + (fl * *(float *)&kRo_snd_SoundObjSetAngle_1744);
    t = gl_ref_00043824(in, (char *)&D_2F720_t3, 0x10);
    lo = *(float *)&kRo_snd_SoundObjSetAngle_1748;
    p->pan[3] = t / 2;
    p->pan[3] <<= 4;

    p->turnAtten = 1.0f - (dlt * 0.0625f);
    p->pan[2] = idxb & 0x80;
    if (p->turnAtten < lo) {
        p->turnAtten = lo;
    }
    if (*(float *)&kRo_snd_SoundObjSetAngle_174C < p->turnAtten) {
        p->turnAtten = *(float *)&kRo_snd_SoundObjSetAngle_174C;
    }
}
}
