/* IDO 7.1 NCC (C++) donor for Audio_InitNoteFreeList (0xA4 / 41 words), OoT's
 * Audio_InitNoteFreeList: clear the four free lists, then for every Note of
 * gAudioContext.notes[numNotes] set listItem.u.value = the note, listItem.prev = NULL,
 * and push it onto noteFreeLists.disabled. Under NCC the addu operand order
 * follows the source (kit XX): the stored value is an int sum (off first), the
 * store address a pointer sum (base first), the call argument an int sum cast
 * back to a pointer, so the loop keeps a byte offset instead of &notes[i].
 * Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern void gl_func_00000000(...);
extern int D_00000000;
#include "../../include/gdl/gl_data.h"  /* object aliases (NCC donors build without -I include) */
#include "../../include/uso_call.h"
#include "../../include/audio.h"
#define gAudioCtx (*(AudioContext *)&gAudioContext)
void Audio_InitNoteFreeList(void) {
    NotePool *pool = &gAudioCtx.noteFreeLists;
    int i;
    int off;
    CALL(Audio_InitNoteLists)(pool);
    for (i = 0, off = 0; i < gAudioCtx.numNotes; i++, off += sizeof(Note)) {
        Note *notes = gAudioCtx.notes;
        ((Note *)((char *)notes + off))->listItem.u.value = (void *)(off + (int)notes);
        ((Note *)((char *)gAudioCtx.notes + off))->listItem.prev = 0;
        CALL(AudioSeq_AudioListPushBack)(pool, (AudioListItem *)(off + (int)gAudioCtx.notes));
    }
}

}
