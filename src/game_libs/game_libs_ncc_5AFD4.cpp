/* IDO 7.1 NCC (C++) EXACT donor for gl_func_0005AFD4 (0x9C / 39 words), agent-c run 162.
 * Heap init: register the range, clear the counters, make the empty free-list head, then make the whole range
 * one block {hdr = 0xFF << 24 | size / 16, minus one; next / prev = the heap head; magic 0x87654321} and
 * link it. The cfe body (91.77, 40 words) folded `(size >> 4) + 0xFF000000 - 1` into one 0xFEFFFFFF
 * constant; the target adds 0xFF000000 (held in a2, reused for the head's hdr store) and THEN subtracts
 * one -- that is `b->hdr = (size >> 4) + 0xFF000000; b->hdr--;`: uopt forwards the first store into the
 * decrement but does not re-fold the already-built sum (`b->hdr -= 1` folds). Spliced over an empty
 * placeholder in both build trees. */
extern "C" {
void gl_func_00000000(...);
extern char D_00000000[];
}
struct Blk { unsigned hdr; Blk *next; Blk *prev; unsigned magic; };
struct Heap { Blk head; Blk list; unsigned n20; unsigned n24; int size; };
extern "C" void gl_func_0005AFD4(Heap *h, Blk *b, char *end) {
    gl_func_00000000(D_00000000 + 0x219F8, b, end, 0x10);
    h->n24 = 0;
    int size = end - (char *)b;
    h->size = size;
    h->n20 = 0;
    h->list.next = &h->list;
    h->list.prev = &h->list;
    h->list.hdr = 0;
    h->list.magic = 0x87654321;
    b->next = &h->head;
    b->prev = &h->head;
    b->hdr = ((unsigned)size >> 4) + 0xFF000000;
    b->hdr--;
    b->magic = 0x87654321;
    h->head.hdr = 0xFF000000;
    h->head.next = b;
    h->head.prev = b;
}
