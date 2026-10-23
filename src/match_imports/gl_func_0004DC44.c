/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
Node4D3E4 *gl_func_0004DC44(Node4D3E4 **head, int flag) {
 Node4D3E4 *node;
 u32 saved;
 if(flag!=0) {
  if(gl_4DC44_busy!=0) {
   do {} while(gl_4DC44_busy!=0);
  }
  saved=gl_4DC44_word(1);
 }
 node=*head;
 if(node==0) {
  gl_4DC44_print(&gl_4DC44_data+0x20330);
 }
 gl_4DC44_remove(node,head);
 if(flag!=0) {
  gl_4DC44_word(saved);
 }
 return node;
}
