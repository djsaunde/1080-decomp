/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gl_func_0005F5F0 (look-at matrix,
 * 229 words). NM object only (build/non_matching); the ROM build keeps INCLUDE_ASM.
 * cfe capped at 85.0; under NCC 98.37 (11 raw) with: no dead[] / if (yLook) {} cfe levers,
 * the degenerate-axis constants through int locals (z = one, x = zero: NCC CSEs a literal
 * 1.0f with the d2 < 1.0f compare constant, the target rematerialises it), the normalised
 * copy-out read z, y, x, NCC frame order slen, len, xLook.., xRight.., v[3], then d2 and a
 * separate ulen for the up-vector scale (one frame word per declared local).
 * RESIDUAL: x colours f14 (target f0), z * 10 lands in f12 (target f18), and the mf home
 * store is scheduled in the jal delay instead of at the top of both branch arms. */
typedef float f32;
extern "C" {
extern int D_00000000;
extern f32 sqrtf(f32);
extern void gl_vec3_normalize_0005F5F0(f32 *);
}
extern "C" void gl_func_0005F5F0(f32 (*mf)[4], f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 slen;
    f32 len;
    f32 xLook, yLook, zLook;
    f32 xRight, yRight, zRight;
    f32 v[3];
    f32 d2;
    f32 ulen;

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    d2 = (xLook * xLook) + (yLook * yLook) + (zLook * zLook);
    if ((*(f32 *)((char *)&D_00000000 + 0x205C) < d2) || (d2 < 10.0f)) {
        f32 x, y, z;
        if (d2 < 1.0f) {
            { int zero = 0; x = zero; }
            y = 0.0f;
            { int one = 1; z = one; }
        } else {
            v[0] = xLook;
            v[1] = yLook;
            v[2] = zLook;
            gl_vec3_normalize_0005F5F0(v);
            z = v[2];
            y = v[1];
            x = v[0];
        }
        xLook = x * 10.0f;
        yLook = y * 10.0f;
        zLook = z * 10.0f;
        v[0] = xLook;
        v[1] = yLook;
        v[2] = zLook;
        d2 = 100.0f;
    }
    len = -1.0f / sqrtf(d2);
    xLook *= len;
    yLook *= len;
    zLook *= len;
    xRight = (yUp * zLook) - (zUp * yLook);
    yRight = (zUp * xLook) - (xUp * zLook);
    zRight = (xUp * yLook) - (yUp * xLook);
    slen = sqrtf((xRight * xRight) + (yRight * yRight) + (zRight * zRight));
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
    ulen = 1.0f / sqrtf((xUp * xUp) + (yUp * yUp) + (zUp * zUp));
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
