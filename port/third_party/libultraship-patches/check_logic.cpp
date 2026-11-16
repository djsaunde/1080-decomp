// Standalone checks of the logic in the libultraship patch series (copied expressions, not linked).
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <vector>
#define G_BL_CLR_MEM 1
#define G_BL_0 3
static int fails = 0;
#define CHECK(c, ...) do { if (!(c)) { fails++; printf("FAIL: " __VA_ARGS__); printf("\n"); } } while (0)

static bool invisibleOld(uint32_t l) {
    return (l & (3 << 24)) == (G_BL_0 << 24) && (l & (3 << 20)) == (G_BL_CLR_MEM << 20);
}
static bool invisibleNew(uint32_t l, bool oneCycle) {
    const uint32_t bl_shift = oneCycle ? 2 : 0;
    return (l & (3U << (24 + bl_shift))) == ((uint32_t)G_BL_0 << (24 + bl_shift)) &&
           (l & (3U << (20 + bl_shift))) == ((uint32_t)G_BL_CLR_MEM << (20 + bl_shift));
}

int main() {
    // 1080 render-mode words (gfx_renderModeWord / gfx_renderMode2Word constants)
    const uint32_t words[] = { 0x03024000, 0x03027008, 0x0C080000, 0x0C4C3078, 0x1041C8, 0x104240, 0x1045D8,
                               0x104A50, 0x105078, 0x112048, 0x112230, 0x112438, 0x112478, 0x113048, 0x113078,
                               0x20004DD8, 0x4041C8, 0x404240, 0x4045D8, 0x404845D8, 0x404845F8, 0x40484DD8,
                               0x404A50, 0x404DD8, 0x405078, 0x442008, 0x442048, 0x442230, 0x442438, 0x442478,
                               0x442D58, 0x443048, 0x443078, 0xC084000, 0xC084200, 0xC087008, 0xC8000000, 0x800000 };
    for (uint32_t w : words) {
        bool n = invisibleNew(w, true);
        if (n || invisibleOld(w))
            printf("1-cycle 0x%08X: old %d new %d\n", w, invisibleOld(w), n);
        CHECK(n == (w == 0x0C4C3078), "1-cycle invisible 0x%08X", w);
        // 2-cycle keeps the old test
        CHECK(invisibleNew(w, false) == invisibleOld(w), "2-cycle 0x%08X", w);
    }
    // A mode that sets both cycles to IN*0 + MEM*0 is invisible either way
    uint32_t both = 0x0C4C3078 | (0x3 << 24) | (0x1 << 20) | (0x3 << 16);
    CHECK(invisibleOld(both) && invisibleNew(both, true), "both cycles");

    // G_LINE3D decode: w1 = v0*2 << 16 | v1*2 << 8 | wd
    uint32_t w1 = (5 * 2) << 16 | (17 * 2) << 8 | 6;
    CHECK(((w1 >> 16) & 0xFF) / 2 == 5 && ((w1 >> 8) & 0xFF) / 2 == 17 && (w1 & 0xFF) == 6, "line decode");

    // Line extrusion: thickness on screen (output pixels) must be (1.5 + wd / 2) * ratioY, for any w
    struct V { float x, y, w; };
    const float vp_w = 1280, vp_h = 960, ratioY = 4.0f; // 320x240 native at 4x
    for (int t = 0; t < 4; t++) {
        V v1 = { 0.1f * t, -0.3f, 1.0f + t }, v2 = { -0.5f * (t + 1), 0.7f * t, 3.0f - 0.5f * t };
        v1.x *= v1.w; v1.y *= v1.w; v2.x *= v2.w; v2.y *= v2.w; // to clip space
        uint8_t width = (uint8_t)(t * 3);
        const float half_px = (1.5f + width * 0.5f) * 0.5f * ratioY;
        float dx = (v2.x / v2.w - v1.x / v1.w) * vp_w * 0.5f;
        float dy = (v2.y / v2.w - v1.y / v1.w) * vp_h * 0.5f;
        float len = sqrtf(dx * dx + dy * dy);
        const float nx = -dy / len * half_px * 2.0f / vp_w;
        const float ny = dx / len * half_px * 2.0f / vp_h;
        V q0 = { v1.x + nx * v1.w, v1.y + ny * v1.w, v1.w }, q1 = { v1.x - nx * v1.w, v1.y - ny * v1.w, v1.w };
        V q3 = { v2.x + nx * v2.w, v2.y + ny * v2.w, v2.w };
        auto px = [&](V v, float& sx, float& sy) { sx = v.x / v.w * vp_w * 0.5f; sy = v.y / v.w * vp_h * 0.5f; };
        float ax, ay, bx, by, cx, cy;
        px(q0, ax, ay); px(q1, bx, by); px(q3, cx, cy);
        float thick = hypotf(ax - bx, ay - by);
        // q0 -> q3 must be parallel to the line (same side)
        float cross = (cx - ax) * dy - (cy - ay) * dx;
        CHECK(fabsf(thick - 2 * half_px) < 1e-3f, "thickness %f vs %f", thick, 2 * half_px);
        CHECK(fabsf(cross) < 1e-2f * len, "edge not parallel %f", cross);
    }

    // Read-back region copy + byte swap (little-endian host)
    const uint32_t W = 320, H = 240;
    std::vector<uint16_t> fb(W * H);
    for (uint32_t i = 0; i < W * H; i++) fb[i] = (uint16_t)(i * 7);
    uint16_t dst[4 * 3];
    uint32_t rx = 318, ry = 10, rw = 4, rh = 3;
    for (uint32_t row = 0; row < rh; row++)
        for (uint32_t col = 0; col < rw; col++) {
            uint32_t x = rx + col, y = ry + row;
            uint16_t p = (x < W && y < H) ? fb[(size_t)y * W + x] : 0;
            p = (uint16_t)((p >> 8) | (p << 8));
            dst[row * rw + col] = p;
        }
    const uint8_t* b = (const uint8_t*)&dst[0];
    uint16_t want = fb[10 * W + 318];
    CHECK(b[0] == (want >> 8) && b[1] == (want & 0xFF), "big-endian store");
    CHECK(dst[2] == 0 && dst[3] == 0, "out-of-range pixels are 0");

    // Range test used by TextureCacheInvalidateRange
    uintptr_t start = 0x1000;
    size_t size = 0x100;
    auto inRange = [start, size](const void* p) { return p != nullptr && (uintptr_t)p - start < size; };
    CHECK(inRange((void*)0x1000) && inRange((void*)0x10FF) && !inRange((void*)0x1100) && !inRange((void*)0xFFF) &&
              !inRange(nullptr), "range");

    printf(fails ? "%d FAILED\n" : "all passed\n", fails);
    return fails != 0;
}
