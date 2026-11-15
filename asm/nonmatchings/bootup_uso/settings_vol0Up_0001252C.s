nonmatching settings_vol0Up_0001252C, 0x6C

glabel settings_vol0Up_0001252C
    /* DE2F98 0001252C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE2F9C 00012530 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE2FA0 00012534 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE2FA4 00012538 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE2FA8 0001253C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE2FAC 00012540 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE2FB0 00012544 91F80013 */  lbu        $t8, 0x13($t7)
    /* DE2FB4 00012548 2B01000A */  slti       $at, $t8, 0xA
    /* DE2FB8 0001254C 10200006 */  beqz       $at, .L00012568
    /* DE2FBC 00012550 00000000 */   nop
    /* DE2FC0 00012554 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE2FC4 00012558 8F300154 */  lw         $s0, 0x154($t9)
    /* DE2FC8 0001255C 92080013 */  lbu        $t0, 0x13($s0)
    /* DE2FCC 00012560 25090001 */  addiu      $t1, $t0, 0x1
    /* DE2FD0 00012564 A2090013 */  sb         $t1, 0x13($s0)
  .L00012568:
    /* DE2FD4 00012568 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE2FD8 0001256C 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE2FDC 00012570 91640013 */  lbu        $a0, 0x13($t3)
    /* DE2FE0 00012574 0C000000 */  jal        func_00000000
    /* DE2FE4 00012578 00000000 */   nop
    /* DE2FE8 0001257C 10000001 */  b          .L00012584
    /* DE2FEC 00012580 00000000 */   nop
  .L00012584:
    /* DE2FF0 00012584 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE2FF4 00012588 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE2FF8 0001258C 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE2FFC 00012590 03E00008 */  jr         $ra
    /* DE3000 00012594 00000000 */   nop
endlabel settings_vol0Up_0001252C
