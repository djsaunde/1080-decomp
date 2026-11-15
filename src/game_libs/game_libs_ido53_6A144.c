/* IDO 5.3 -O2 -mips2 donor for fsin = libultra
 * fsin/__sinf (libreultra gu/sinf.c verbatim). PROVEN EXACT 2026-09-05:
 * 112/112 words identical on the first compile against the concatenated
 * 0x6A144..0x6A303 stream (six splat fragments unified: 6A144/6A1BC/6A1C4/
 * 6A2C0/6A2DC/6A2F8). 5.3 -O3 emits the same words; 7.1 -O2/-O3 diverge
 * in the polynomial chain (different mul.d/add.d interleave), 5.3 -O1 is
 * 124 words. Static rodata (P[5]/rpi/pihi/pilo/zero) lives in the ROM's
 * game_libs data at segment offsets 0x2310..0x2353 -> extern gl_ref_*
 * with baked values (same recipe as the fcos donor game_libs_o2_70FCC.c,
 * whose table sits at 0x24E0); __libm_qnan_f is the runtime-reloc blanked
 * extern D_00000000_qnanf (shared with fcos).
 * Spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY. */
typedef union {
    struct {
        unsigned int hi, lo;
    } word;
    double d;
} du;

typedef union {
    unsigned int i;
    float f;
} fu;

#define ROUND(d) (int)(((d) >= 0.0) ? ((d) + 0.5) : ((d) - 0.5))

extern const du kRo_fsin_2310[]; /* P[5]: sin poly coeffs on +/- pi/2 */
extern const du kRo_fsin_2338;   /* rpi  = 1/pi */
extern const du kRo_fsin_2340;   /* pihi */
extern const du kRo_fsin_2348;   /* pilo */
extern const fu kRo_fsin_2350;   /* zero */
extern float D_00000000_qnanf;     /* __libm_qnan_f */

float fsin(float x) {
    double dx, xsq, poly;
    double dn;
    int n;
    double result;
    int ix, xpt;

    ix = *(int *)&x;
    xpt = (ix >> 22);
    xpt &= 0x1ff;

    /* xpt is exponent(x) + 1 bit of mantissa */

    if (xpt < 0xff) {
        /* |x| < 1.5 */
        dx = x;

        if (xpt >= 0xe6) {
            /* |x| >= 2^(-12): standard polynomial approximation */
            xsq = dx * dx;
            poly = ((kRo_fsin_2310[4].d * xsq + kRo_fsin_2310[3].d) * xsq + kRo_fsin_2310[2].d) * xsq
                   + kRo_fsin_2310[1].d;
            result = dx + (dx * xsq) * poly;
            return ((float)result);
        }

        return (x);
    }

    if (xpt < 0x136) {
        /* |x| < 2^28: reduce argument to +/- pi/2 */
        dx = x;

        dn = dx * kRo_fsin_2338.d;
        n = ROUND(dn);
        dn = n;

        dx = dx - dn * kRo_fsin_2340.d;
        dx = dx - dn * kRo_fsin_2348.d; /* dx = x - n*pi */

        /* compute sin(dx) as before, negating result if n is odd */
        xsq = dx * dx;
        poly = ((kRo_fsin_2310[4].d * xsq + kRo_fsin_2310[3].d) * xsq + kRo_fsin_2310[2].d) * xsq
               + kRo_fsin_2310[1].d;
        result = dx + (dx * xsq) * poly;

        if ((n & 1) == 0)
            return ((float)result);

        return (-(float)result);
    }

    if (x != x) {
        /* x is a NaN; return a quiet NaN */
        return (D_00000000_qnanf);
    }

    /* just give up and return 0.0 */
    return (kRo_fsin_2350.f);
}
