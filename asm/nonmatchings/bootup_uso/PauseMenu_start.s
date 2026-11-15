nonmatching PauseMenu_start, 0x44

glabel PauseMenu_start
    /* DE0CCC 00010260 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE0CD0 00010264 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE0CD4 00010268 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE0CD8 0001026C 8FA40018 */  lw         $a0, 0x18($sp)
    /* DE0CDC 00010270 0C000000 */  jal        func_00000000
    /* DE0CE0 00010274 00000000 */   nop
    /* DE0CE4 00010278 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DE0CE8 0001027C ADC00080 */  sw         $zero, 0x80($t6)
    /* DE0CEC 00010280 8FA40018 */  lw         $a0, 0x18($sp)
    /* DE0CF0 00010284 0C000000 */  jal        func_00000000
    /* DE0CF4 00010288 00000000 */   nop
    /* DE0CF8 0001028C 10000001 */  b          .L00010294
    /* DE0CFC 00010290 00000000 */   nop
  .L00010294:
    /* DE0D00 00010294 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE0D04 00010298 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE0D08 0001029C 03E00008 */  jr         $ra
    /* DE0D0C 000102A0 00000000 */   nop
endlabel PauseMenu_start
