nonmatching PauseMenu_func_0001024C, 0x14

glabel PauseMenu_func_0001024C
    /* DE0CB8 0001024C AFA40000 */  sw         $a0, 0x0($sp)
    /* DE0CBC 00010250 10000001 */  b          .L00010258
    /* DE0CC0 00010254 00000000 */   nop
  .L00010258:
    /* DE0CC4 00010258 03E00008 */  jr         $ra
    /* DE0CC8 0001025C 00000000 */   nop
endlabel PauseMenu_func_0001024C
