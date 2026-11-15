nonmatching settings_vol1Up_00012458, 0x6C

glabel settings_vol1Up_00012458
    /* DE2EC4 00012458 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE2EC8 0001245C AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE2ECC 00012460 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE2ED0 00012464 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE2ED4 00012468 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE2ED8 0001246C 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE2EDC 00012470 91F80012 */  lbu        $t8, 0x12($t7)
    /* DE2EE0 00012474 2B01000A */  slti       $at, $t8, 0xA
    /* DE2EE4 00012478 10200006 */  beqz       $at, .L00012494
    /* DE2EE8 0001247C 00000000 */   nop
    /* DE2EEC 00012480 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE2EF0 00012484 8F300154 */  lw         $s0, 0x154($t9)
    /* DE2EF4 00012488 92080012 */  lbu        $t0, 0x12($s0)
    /* DE2EF8 0001248C 25090001 */  addiu      $t1, $t0, 0x1
    /* DE2EFC 00012490 A2090012 */  sb         $t1, 0x12($s0)
  .L00012494:
    /* DE2F00 00012494 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE2F04 00012498 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE2F08 0001249C 91640012 */  lbu        $a0, 0x12($t3)
    /* DE2F0C 000124A0 0C000000 */  jal        func_00000000
    /* DE2F10 000124A4 00000000 */   nop
    /* DE2F14 000124A8 10000001 */  b          .L000124B0
    /* DE2F18 000124AC 00000000 */   nop
  .L000124B0:
    /* DE2F1C 000124B0 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE2F20 000124B4 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE2F24 000124B8 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE2F28 000124BC 03E00008 */  jr         $ra
    /* DE2F2C 000124C0 00000000 */   nop
endlabel settings_vol1Up_00012458
