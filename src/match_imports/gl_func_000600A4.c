/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
Node600A4 *gl_func_000600A4(char *name,int unused) {
 Owner600A4 *global;
 Node600A4 *node;
 Node600A4 *head;
 global=gl_600A4_root;
 node=0;
 head=global->head40;
 if(head!=0)node=gl_600A4_lookup(global,name,head);
 if(node==0) {
  node=gl_600A4_alloc(0x44);
  gl_600A4_init(node,name);
  global=gl_600A4_root;
  head=global->head40;
  if(head==0) {
   if(global->tail44!=0) {
    gl_600A4_diag(&gl_600A4_data+0x21C8C);
    global=gl_600A4_root;
   }
   global->tail44=node;
  } else {
   node->field14=head->field18;
   gl_600A4_root->head40->field18=node;
   node->field1C=gl_600A4_root->head40;
  }
  global=gl_600A4_root;
  if(global->tail44==0)global->tail44=node;
 }
 node->field20=gl_600A4_root->head40;
 gl_600A4_root->head40=node;
 node->field00=gl_600A4_read();
 return node;
}
