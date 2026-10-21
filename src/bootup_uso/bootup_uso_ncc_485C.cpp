/* IDO 7.1 NCC (C++) donor for func_0000485C (0xB8 / 46 words) -- pushes the
 * object's vector and the root transform's scaled vector to a debug/draw
 * sink (func_00000080 + 0x20 is the sink's datum, passed to every call).
 *
 * C++ (agent-c run 116). Under cfe the body capped at 94.1: the two stack
 * vectors and the a2 reload of the homed arg cannot both come out. Under edg:
 * - V3(float, float, float) forwards to an inline set() (the 5062C rule):
 *   gives every FP register and the a0 home / a2 reloads;
 * - b1 is a function-scope local, b2 lives in an inner block: edg allocates
 *   block-scope locals when the block opens, i.e. AFTER the pool slots of
 *   b1's inline ctor, which puts b2 at sp+0x20 under 5 pool slots (target
 *   layout b1 0x40 / b2 0x20; function scope gives b2 0x34);
 * - b2's constructor and the call share ONE source line: on separate lines
 *   uopt schedules the x store before the a0 %lo (one swapped pair); IDO
 *   line boundaries limit the scheduler (PERM_FORCE_SAMELINE effect).
 * docs/IDO_CODEGEN.md#kit-xvi-agent-c-run-116. Spliced into bootup_uso.c.o
 * via REPLACE_FUNC_BODY; bu_485c_root / bu_485c_g are blank (0) aliases. */
struct V3 { float x, y, z;
    V3(float a, float b, float c) { set(a, b, c); }
    void set(float a, float b, float c) { x = a; y = b; z = c; } };
extern "C" {
void func_00000000(...);
extern char func_00000080[];
extern char bu_485c_g[];
extern char *bu_485c_root;
}
#define STR (func_00000080 + 0x20)
#define F(p,o) (*(float *)((char *)(p) + (o)))
extern "C" void func_0000485C(char *a0) {
    char *v0;
    func_00000000(STR);
    V3 b1(F(a0,0x30), F(a0,0x34), F(a0,0x30));
    func_00000000(STR, &b1);
    v0 = *(char **)(bu_485c_root + 0x70);
    {
        V3 b2(F(v0,0xA0) * F(bu_485c_g,0x128), F(v0,0xA4) * F(bu_485c_g,0x12C) + F(a0,0x2C), F(v0,0xA8) * F(bu_485c_g,0x130)); func_00000000(STR, &b2);
    }
    func_00000000(a0);
}
