nonmatching settings_setBit_00012244, 0x80

glabel settings_setBit_00012244
    /* DE2CB0 00012244 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE2CB4 00012248 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE2CB8 0001224C AFA40028 */  sw         $a0, 0x28($sp)
    /* DE2CBC 00012250 AFA5002C */  sw         $a1, 0x2C($sp)
    /* DE2CC0 00012254 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE2CC4 00012258 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE2CC8 0001225C 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE2CCC 00012260 95F80000 */  lhu        $t8, 0x0($t7)
    /* DE2CD0 00012264 8FB9002C */  lw         $t9, 0x2C($sp)
    /* DE2CD4 00012268 24080001 */  addiu      $t0, $zero, 0x1
    /* DE2CD8 0001226C 03284804 */  sllv       $t1, $t0, $t9
    /* DE2CDC 00012270 03095024 */  and        $t2, $t8, $t1
    /* DE2CE0 00012274 1540000C */  bnez       $t2, .L000122A8
    /* DE2CE4 00012278 00000000 */   nop
    /* DE2CE8 0001227C 8FAB0028 */  lw         $t3, 0x28($sp)
    /* DE2CEC 00012280 8D700154 */  lw         $s0, 0x154($t3)
    /* DE2CF0 00012284 960C0000 */  lhu        $t4, 0x0($s0)
    /* DE2CF4 00012288 8FAD002C */  lw         $t5, 0x2C($sp)
    /* DE2CF8 0001228C 240E0001 */  addiu      $t6, $zero, 0x1
    /* DE2CFC 00012290 01AE7804 */  sllv       $t7, $t6, $t5
    /* DE2D00 00012294 018F4025 */  or         $t0, $t4, $t7
    /* DE2D04 00012298 A6080000 */  sh         $t0, 0x0($s0)
    /* DE2D08 0001229C 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE2D0C 000122A0 0C000000 */  jal        func_00000000
    /* DE2D10 000122A4 00000000 */   nop
  .L000122A8:
    /* DE2D14 000122A8 10000001 */  b          .L000122B0
    /* DE2D18 000122AC 00000000 */   nop
  .L000122B0:
    /* DE2D1C 000122B0 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE2D20 000122B4 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE2D24 000122B8 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE2D28 000122BC 03E00008 */  jr         $ra
    /* DE2D2C 000122C0 00000000 */   nop
endlabel settings_setBit_00012244
