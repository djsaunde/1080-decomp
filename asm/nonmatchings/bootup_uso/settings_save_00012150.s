nonmatching settings_save_00012150, 0x38

glabel settings_save_00012150
    /* DE2BBC 00012150 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE2BC0 00012154 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE2BC4 00012158 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE2BC8 0001215C 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DE2BCC 00012160 8DC40148 */  lw         $a0, 0x148($t6)
    /* DE2BD0 00012164 8DC50154 */  lw         $a1, 0x154($t6)
    /* DE2BD4 00012168 0C000000 */  jal        func_00000000
    /* DE2BD8 0001216C 00000000 */   nop
    /* DE2BDC 00012170 10000001 */  b          .L00012178
    /* DE2BE0 00012174 00000000 */   nop
  .L00012178:
    /* DE2BE4 00012178 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE2BE8 0001217C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE2BEC 00012180 03E00008 */  jr         $ra
    /* DE2BF0 00012184 00000000 */   nop
endlabel settings_save_00012150
