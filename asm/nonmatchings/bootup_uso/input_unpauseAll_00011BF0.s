nonmatching input_unpauseAll_00011BF0, 0x80

glabel input_unpauseAll_00011BF0
    /* DE265C 00011BF0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE2660 00011BF4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE2664 00011BF8 AFA40020 */  sw         $a0, 0x20($sp)
    /* DE2668 00011BFC AFA0001C */  sw         $zero, 0x1C($sp)
    /* DE266C 00011C00 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE2670 00011C04 8DCF0124 */  lw         $t7, 0x124($t6)
    /* DE2674 00011C08 19E00011 */  blez       $t7, .L00011C50
    /* DE2678 00011C0C 00000000 */   nop
  .L00011C10:
    /* DE267C 00011C10 8FB80020 */  lw         $t8, 0x20($sp)
    /* DE2680 00011C14 8FB9001C */  lw         $t9, 0x1C($sp)
    /* DE2684 00011C18 00194080 */  sll        $t0, $t9, 2
    /* DE2688 00011C1C 03084821 */  addu       $t1, $t8, $t0
    /* DE268C 00011C20 8D240100 */  lw         $a0, 0x100($t1)
    /* DE2690 00011C24 00002825 */  or         $a1, $zero, $zero
    /* DE2694 00011C28 0C000000 */  jal        func_00000000
    /* DE2698 00011C2C 00000000 */   nop
    /* DE269C 00011C30 8FAA001C */  lw         $t2, 0x1C($sp)
    /* DE26A0 00011C34 254B0001 */  addiu      $t3, $t2, 0x1
    /* DE26A4 00011C38 AFAB001C */  sw         $t3, 0x1C($sp)
    /* DE26A8 00011C3C 8FAC0020 */  lw         $t4, 0x20($sp)
    /* DE26AC 00011C40 8D8D0124 */  lw         $t5, 0x124($t4)
    /* DE26B0 00011C44 016D082A */  slt        $at, $t3, $t5
    /* DE26B4 00011C48 1420FFF1 */  bnez       $at, .L00011C10
    /* DE26B8 00011C4C 00000000 */   nop
  .L00011C50:
    /* DE26BC 00011C50 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE26C0 00011C54 ADC00188 */  sw         $zero, 0x188($t6)
    /* DE26C4 00011C58 10000001 */  b          .L00011C60
    /* DE26C8 00011C5C 00000000 */   nop
  .L00011C60:
    /* DE26CC 00011C60 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE26D0 00011C64 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE26D4 00011C68 03E00008 */  jr         $ra
    /* DE26D8 00011C6C 00000000 */   nop
endlabel input_unpauseAll_00011BF0
