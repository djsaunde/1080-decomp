/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void gl_func_00030A20(s32 arg0,s32 arg1) {
 s32 temp_v0;
 gl_30A20_pair(arg0,arg1);
 gl_30A20_slot1435=arg0;
 gl_30A20_slot1436=arg1;
 gl_30A20_set(0xF);
 temp_v0=gl_30A20_gate1432;
 if(temp_v0==0 || temp_v0==4) {
  if(arg0>=5) gl_30A20_clear(gl_30A20_slot1435+0x29);
  else gl_30A20_clear(gl_30A20_slot1435+4);
  gl_30A20_clear(0xE);
  if(arg1>=5) {
   gl_30A20_clear(gl_30A20_slot1436+0x29);
   return;
  }
  gl_30A20_clear(gl_30A20_slot1436+4);
 }
}
