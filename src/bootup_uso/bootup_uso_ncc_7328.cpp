/* IDO 7.1 NCC (C++) donor for func_00007328 (0x1C0 / 112 words): drain the
 * message queue -- for each message look the id up in the handler list (stack
 * {cur, nxt} iterator, the same class as game_libs 38964 / 3D48C) and fire
 * the handler's virtual slot 8, else report + reply; then the flag tail.
 *
 * Load-bearing (cfe NM was 66.6):
 * - the list iterator's next() advances through its own nxt member,
 *   `(cur = nxt) ? (nxt = nxt->next, cur->data) : 0`;
 * - Entry::matches(Msg) takes the message BY VALUE (the per-iteration copy
 *   through the s1 = sp+52 pointer);
 * - frame 88: msg/out/found at function scope (found above the iterator),
 *   the handler call without a named local, `mode` in its own block (a
 *   function-scope `mode` costs a slot above the iterator);
 * - tail: the |= 2 before the 800 store (ring t5/t3/t4).
 * Spliced into bootup_uso.c.o via REPLACE_FUNC_BODY. */
extern "C" {
void func_00000000(...);
void func_00005EF8(void *);
void func_0000502C(void *);
void func_00004FF0(void *);
extern char D_00000000, D_00007F90, D_00007FA0;
extern char func_0000027C[], func_00000008[], func_0000029C[];
}
struct Msg73 { int id; };
struct Handler73 { char pad[0x28]; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void handle(); };
struct Entry73 { Handler73 *h; int id; int matches(Msg73 m) { return m.id == id; } };
struct Link73 { Entry73 *data; Link73 *next; };
struct Iter73 {
    Link73 *cur; Link73 *nxt;
    void start(Link73 *h) { nxt = h; }
    Entry73 *next() { return (cur = nxt) ? (nxt = nxt->next, cur->data) : 0; }
    Entry73 *get() { return cur->data; }
};
extern "C" void func_00007328(char *self) {
    Msg73 msg;
    int out;
    Entry73 *found;
    for (;;) {
        msg.id = 0;
        func_00005EF8(&msg);
        if (msg.id == 0) break;
        Iter73 it;
        func_0000502C(&out);
        found = 0;
        it.start(*(Link73 **)(self + 0x2C));
        while (it.next() != 0) {
            if (it.get()->matches(msg)) {
                found = it.get();
                break;
            }
        }
        if (found != 0) {
            func_00000000(&D_00007F90, &found->id);
            Handler73 *h = found->h;
            h->handle();
        } else {
            func_00000000(&D_00007FA0, &msg);
            func_00000000(&D_00000000, 1, out);
        }
    }
    func_0000502C(self + 0x38);
    if (*(int *)(self + 0x38) & 1) {
        func_00000000(func_0000027C + 0x18);
    }
    if (*(int *)(self + 0x38) & 2) {
        func_00004FF0(self + 0x30);
        func_00004FF0(self + 0x34);
    }
    { int mode = *(int *)(func_00000008 + 0x34);
    if (mode == 6 || mode == 11) {
        *(float *)(func_0000029C + 4) = 1.0f;
        *(int *)(self + 0x38) |= 2;
        *(int *)(self + 0x34) = 800;
    } }
}
