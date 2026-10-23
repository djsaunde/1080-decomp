/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00042684(GL470E4Object *self, Pair2 *arg1, short color)
{
  Pair2 cfg;
  Pair2 *p;
  int idx;
  int r, g, b;
  int flags;
  GL470E4Packet *entry;
  {
    cfg = *arg1;
    p = &cfg;
    flags = 0xF00;
    p->b = (p->b & (~flags)) | 0x400;
  }
  flags = (p->b & (~0x80000)) & (~8);
  if (arg1)
  {
  }
  p->b = flags;
  gl_42684_call_57194(self, p);
  gl_42684_call_5703C(self, p);
  gl_42684_call_56FF4(self, p);
  gl_42684_call_57104(self, p);
  idx = self->list->count;
  self->list->count = idx + 1;
  entry = (GL470E4Packet *) (((int) self->list->packets) + (idx * 8));
    entry->command = 0xFA000000; r = (short)(((color >> 10) << 3) & 255); g = (short)(((color >> 5) & 31) << 3); b = (short)((color << 3) & 255); entry->color = ((r & 255) << 24) | ((g & 255) << 16) | ((b & 255) << 8) | 255;
}
