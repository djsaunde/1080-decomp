/* IDO 7.1 NCC (C++) donor for func_0000E740 (0xB4 / 45 words) -- copies a
 * 64x32 u16 tile out of the root object's buffer (every 4th sample) into
 * self->pix.
 *
 * C++ (agent-c run 116): the buffer comes from a virtual getter on the root
 * object at func_0000023C+4 (vptr +0x28, slot 11: lh +0x60 / lw +0x64). With
 * the source pointer stepped per element (`*dst++ = *src; src += 4;`, then
 * `src += 0x540` per row) IDO unrolls the inner loop x4 into the target's
 * 0/8/16/24 offsets with one `addiu v1,v1,32`. The cfe NM indexed src[4*k]
 * by hand and scored 26.4. Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY;
 * the root reference is func_0000023C itself (same reloc as the target). */
extern "C" {
extern char func_0000023C[];
}
struct VO { char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void v10(); virtual char *buf(); };
struct Obj { char pad[0x1C]; unsigned short *pix; };
extern "C" void func_0000E740(Obj *self, int a1) {
    unsigned short *src = (unsigned short *)((*(VO **)(func_0000023C + 4))->buf() + 0x9140);
    unsigned short *dst = self->pix;
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            *dst++ = *src;
            src += 4;
        }
        src += 0x540;
    }
}
