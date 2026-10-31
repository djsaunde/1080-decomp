/* IDO 7.1 NCC (C++) donor for gl_func_0005F5F0 (look-at matrix, 229 words): EXACT (agent-g run 247), spliced
 * over the empty placeholder in game_libs_post0b.c on both build paths.
 * Custom guLookAtF with an eye-to-at distance clamp: if dist2 > far2 (D+0x205C = 10000.0f) or dist2 < 10.0f the
 * look vector is reset to (0, 0, 1) (dist2 < 1) or normalised in place, scaled to length 10 and dist2 forced
 * to 100. sqrtf / the normaliser are baked USO jal placeholders (target word 0x0C000000; pinned to 0).
 * History (NM): cfe capped at 85.0; NCC 98.37 with the degenerate-axis constants through int locals (NCC CSEs a
 * literal 1.0f with the d2 < 1.0f compare constant, the target rematerialises it); run 138 / 246 copy-out
 * empty tests (98.69 -> 99.67). Run 247 (kit CCLXXII): the target has NO x / y / z temporaries -- the reset
 * and the normalised copy-out live in v[] itself (`v[i] *= 10.0f; xLook = v[i];`), which removes the three
 * uopt LRs that took f22 / f24 ahead of the 10.0f constant (10.0 f26 -> f22, v[1] / v[2] f14 / f16 -> f16 /
 * f18, all 229 words up to the frame); two unused volatile homes after the last declared local restore the
 * edg home words x / y / z held (frame 152 -> 160) and put the uopt spill slots at 80 / 76 as the target. */
typedef float f32;
extern "C" {
extern int D_00000000;
extern f32 gl_func_0005F5F0_sqrtf(f32);
extern void gl_func_0005F5F0_normalize(f32 *);
}
extern "C" void gl_func_0005F5F0(f32 (*mf)[4], f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 slen;
    f32 len;
    f32 xLook, yLook, zLook;
    f32 xRight, yRight, zRight;
    f32 v[3];
    f32 d2;
    f32 ulen;
    volatile f32 pad0, pad1;

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    d2 = (xLook * xLook) + (yLook * yLook) + (zLook * zLook);
    if ((*(f32 *)((char *)&D_00000000 + 0x205C) < d2) || (d2 < 10.0f)) {
        if (d2 < 1.0f) {
            { int zero = 0; v[0] = zero; }
            v[1] = 0.0f;
            { int one = 1; v[2] = one; }
        } else {
            v[0] = xLook;
            v[1] = yLook;
            v[2] = zLook;
            gl_func_0005F5F0_normalize(v);
        }
        v[0] *= 10.0f; xLook = v[0];
        v[1] *= 10.0f; yLook = v[1];
        v[2] *= 10.0f; zLook = v[2];
        d2 = 100.0f;
    }
    len = -1.0f / gl_func_0005F5F0_sqrtf(d2);
    xLook *= len;
    yLook *= len;
    zLook *= len;
    xRight = (yUp * zLook) - (zUp * yLook);
    yRight = (zUp * xLook) - (xUp * zLook);
    zRight = (xUp * yLook) - (yUp * xLook);
    slen = gl_func_0005F5F0_sqrtf((xRight * xRight) + (yRight * yRight) + (zRight * zRight));
    if (slen < *(f32 *)((char *)&D_00000000 + 0x2060)) {
        yRight = 0.0f;
        xRight = 1.0f;
        zRight = 0.0f;
        slen = 1.0f;
    }
    len = 1.0f / slen;
    xRight *= len;
    yRight *= len;
    zRight *= len;
    xUp = (yLook * zRight) - (zLook * yRight);
    yUp = (zLook * xRight) - (xLook * zRight);
    zUp = (xLook * yRight) - (yLook * xRight);
    ulen = 1.0f / gl_func_0005F5F0_sqrtf((xUp * xUp) + (yUp * yUp) + (zUp * zUp));
    xUp *= ulen;
    yUp *= ulen;
    zUp *= ulen;
    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -((xEye * xRight) + (yEye * yRight) + (zEye * zRight));
    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -((xEye * xUp) + (yEye * yUp) + (zEye * zUp));
    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -((xEye * xLook) + (yEye * yLook) + (zEye * zLook));
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
}
