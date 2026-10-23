/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_0004AAF0(Owner4AAF0 *self) {
 int i;
 if(self->sourceC0==0) {
  gl_4AAF0_diag(&gl_4AAF0_data+0x2006C);
  return;
 }
 for(i=0;i<self->endD6-self->baseD4;i++) {
  s16 index=self->indicesD0[i];
  if(index!=-1) {
   Context4AAF0 *context=gl_4AAF0_globals.context214;
   s16 *slot=(s16 *)(context->table1C->buffer00+self->baseD4*16+i*16);
   slot[0]=self->sourceC0[index].a;
   slot[1]=self->sourceC0[index].b;
   slot[2]=self->sourceC0[index].c;
  }
 }
 self->sourceC0=0;
}
