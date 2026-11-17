/* port/gfx: dumper unit test on the synthetic frame (synth.c).
 *   test_dlump [out.dlmp]   checks the dump; optionally writes it for
 *                           tools/dlump.py and the replayer. */

#include "dlump.h"
#include "synth.h"

#include <stdio.h>
#include <string.h>

static int sFail;
#define CHECK(c)                                                         \
    do {                                                                 \
        if (!(c)) {                                                      \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c);          \
            sFail++;                                                     \
        }                                                                \
    } while (0)

typedef struct Found {
    unsigned int kind, addr, size, aux;
} Found;

static Found sFound[256];
static int sNFound;

static int has(unsigned int kind, unsigned int addr, unsigned int size) {
    int i;
    for (i = 0; i < sNFound; i++) {
        if (sFound[i].kind == kind && sFound[i].addr <= addr && addr + size <= sFound[i].addr + sFound[i].size) {
            return 1;
        }
    }
    return 0;
}

static int count(unsigned int kind) {
    int i, n = 0;
    for (i = 0; i < sNFound; i++) {
        n += sFound[i].kind == kind;
    }
    return n;
}

static void parse(const unsigned char *d, size_t n) {
    const DlumpHeader *h = (const DlumpHeader *)d;
    size_t off = sizeof(DlumpHeader);
    unsigned int i;

    CHECK(memcmp(h->magic, DLUMP_MAGIC, 8) == 0);
    CHECK(h->version == DLUMP_VERSION && h->headerSize == 64 && h->byteOrderMark == DLUMP_BYTE_ORDER_MARK);
    sNFound = 0;
    for (i = 0; i <= h->chunkCount; i++) {
        const DlumpChunk *c = (const DlumpChunk *)(d + off);
        CHECK(off + sizeof(DlumpChunk) <= n);
        if (off + sizeof(DlumpChunk) > n) {
            return;
        }
        if (i == h->chunkCount) {
            CHECK(c->kind == DLUMP_END);
            off += sizeof(DlumpChunk);
            break;
        }
        if (c->kind == DLUMP_ERROR) {
            printf("  dump error at 0x%08x: %s\n", c->addr, (const char *)(c + 1));
        }
        if (sNFound < 256) {
            sFound[sNFound].kind = c->kind;
            sFound[sNFound].addr = c->addr;
            sFound[sNFound].size = c->size;
            sFound[sNFound].aux = c->aux;
            sNFound++;
        }
        off += sizeof(DlumpChunk) + ((c->size + 3) & ~3u);
    }
    CHECK(off == n);
}

int main(int argc, char **argv) {
    const SynthFrame *fr = synth_build(0);
    DlumpBuf buf = { 0 };
    DlumpOptions opt = { 0, NULL, NULL, 0 };
    DlumpExtra extra[2];
    static const dlump_u32 depthIdx[2] = { 120 * 320 + 160, 0 };
    const DlumpHeader *h;
    int errs;

    /* the framebuffer model's records (plan 19.10) ride along as extra chunks */
    extra[0].kind = DLUMP_FB_READ;
    extra[0].addr = fr->cimgAddr;
    extra[0].size = 0;
    extra[0].aux = 320;
    extra[0].data = NULL;
    extra[1].kind = DLUMP_FB_DEPTH;
    extra[1].addr = fr->zimgAddr;
    extra[1].size = sizeof(depthIdx);
    extra[1].aux = 320;
    extra[1].data = depthIdx;
    opt.extra = extra;
    opt.nExtra = 2;
    opt.arenaBase = fr->arenaBase;
    errs = dlump_task(&buf, &opt, 7, fr->taskAddr, 1, fr->rootAddr, fr->rootSize);
    CHECK(errs == 0);
    h = (const DlumpHeader *)buf.data;
    parse(buf.data, buf.size);

    CHECK(h->seq == 7 && h->rootAddr == fr->rootAddr && h->rootSize == fr->rootSize);
    /* root 22 + clear 9 + shared 5 twice + tail 2, the dead ENDDL after the branch not walked */
    CHECK(h->cmdCount == 21 + 9 + 5 + 5 + 2);
    CHECK(h->flags == 0);
    CHECK(has(DLUMP_TASK, fr->taskAddr, 0x40));
    CHECK(has(DLUMP_DL, fr->rootAddr, fr->rootSize - 8));
    CHECK(!has(DLUMP_DL, fr->rootAddr + fr->rootSize - 8, 8)); /* unreached ENDDL */
    CHECK(has(DLUMP_DL, fr->sharedDl, 5 * 8));
    CHECK(has(DLUMP_DL, fr->branchDl, 2 * 8));
    CHECK(has(DLUMP_VTX, fr->vtxAddr, fr->vtxSize));
    CHECK(count(DLUMP_MTX) == 2);
    CHECK(has(DLUMP_MOVEMEM, fr->vpAddr, 16));
    CHECK(has(DLUMP_TEXTURE, fr->texAddr[0], fr->texSize[0])); /* shared list, 1st caller */
    CHECK(has(DLUMP_TEXTURE, fr->texAddr[1], fr->texSize[1])); /* shared list, 2nd caller */
    CHECK(count(DLUMP_TEXTURE) == 2);
    CHECK(has(DLUMP_TLUT, fr->tlutAddr, fr->tlutSize));
    CHECK(has(DLUMP_IMAGE, fr->cimgAddr, 0) && has(DLUMP_IMAGE, fr->zimgAddr, 0));
    CHECK(count(DLUMP_IMAGE) == 2);
    CHECK(count(DLUMP_ERROR) == 0);
    CHECK(has(DLUMP_FB_READ, fr->cimgAddr, 0) && count(DLUMP_FB_READ) == 1);
    CHECK(has(DLUMP_FB_DEPTH, fr->zimgAddr, sizeof(depthIdx)) && count(DLUMP_FB_DEPTH) == 1);
    opt.extra = NULL;
    opt.nExtra = 0;

    if (argc > 1) {
        FILE *f = fopen(argv[1], "wb");
        CHECK(f != NULL);
        if (f != NULL) {
            fwrite(buf.data, 1, buf.size, f);
            fclose(f);
        }
    }
    dlump_buf_free(&buf);

    /* error paths: a bad root, a G_DL loop that overflows the stack */
    {
        static unsigned int loop[2] __attribute__((aligned(8)));
        DlumpBuf b2 = { 0 };
        unsigned int la = (unsigned int)((size_t)loop - fr->arenaBase);
        int e;
        loop[0] = 0x06000000u;
        loop[1] = la; /* calls itself */
        opt.arenaBase = fr->arenaBase;
        e = dlump_task(&b2, &opt, 0, 0, 1, 0x10u, 8);
        CHECK(e == 1);
        dlump_buf_free(&b2);
        if ((size_t)loop - fr->arenaBase < 0x100000000ull) {
            e = dlump_task(&b2, &opt, 0, 0, 1, la, 8);
            CHECK(e == 1);
            h = (const DlumpHeader *)b2.data;
            CHECK(h->cmdCount == DLUMP_DL_STACK + 1);
            dlump_buf_free(&b2);
        }
    }

    printf("test_dlump: %s (%d failures)\n", sFail ? "FAIL" : "ok", sFail);
    return sFail != 0;
}
