/* IDO 7.1 NCC (C++) donor for game_uso_func_000057D8 (0x144 / 81 words): mode-value
 * pick + mode dispatch. The selector is ONE 8-way C++ switch -- edg lowers it to a
 * case-order compare chain (cfe tables any >= 5-case switch, which capped the cfe NM
 * at 82.5 with nested 3+4 switches). The 0x758 sub-object store is an inline
 * Val::set() whose `this` re-reads a0->0x30 (gives `addiu v0,a1,0x758; swc1 0x10(v0)`),
 * and the state pointer is read through a0->0x30 too, so the const 2 colours v0.
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY. */
extern "C" {
void game_uso_func_0000591C(char *);
void game_uso_func_00006A30(char *);
void game_uso_func_00006CF0(char *);
void game_uso_func_000071E0(char *);
}
struct Val { char pad[0x10]; float v; void set(float f) { v = f; } };
#define AT(T, p, o) (*(T *)((char *)(p) + (o)))
extern "C" void game_uso_func_000057D8(char *a0) {
    if (AT(int, a0, 0x4D8) != 0) {
        AT(int, a0, 0x4D8) = 0;
        char *st = AT(char *, AT(char *, a0, 0x30), 0x908);
        if (st != 0 && AT(unsigned, AT(char *, a0, 0x30), 0x848) == 2 && AT(unsigned, st, 0x848) != 2) {
            float value;
            switch (AT(int, st, 0x8C4)) {
            case 1: value = AT(float, a0, 0x414); break;
            case 2: value = AT(float, a0, 0x42C); break;
            case 3: value = AT(float, a0, 0x444); break;
            case 4: value = AT(float, a0, 0x45C); break;
            case 5: value = AT(float, a0, 0x474); break;
            case 6: value = AT(float, a0, 0x48C); break;
            case 7: value = AT(float, a0, 0x4A4); break;
            default: value = AT(float, a0, 0x4BC); break;
            }
            ((Val *)(AT(char *, a0, 0x30) + 0x758))->set(value);
        }
    }
    AT(float, a0, 0x3C) = 0.0f;
    switch (AT(int, a0, 0x64)) {
    case 0: game_uso_func_0000591C(a0); break;
    case 1: game_uso_func_00006A30(a0); break;
    case 2: game_uso_func_00006CF0(a0); break;
    case 3: game_uso_func_000071E0(a0); break;
    }
}
