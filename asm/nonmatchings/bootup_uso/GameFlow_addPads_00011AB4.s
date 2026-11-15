nonmatching GameFlow_addPads_00011AB4, 0xA8

glabel GameFlow_addPads_00011AB4
    /* DE2520 00011AB4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE2524 00011AB8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE2528 00011ABC AFA40020 */  sw         $a0, 0x20($sp)
    /* DE252C 00011AC0 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE2530 00011AC4 ADC00120 */  sw         $zero, 0x120($t6)
    /* DE2534 00011AC8 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DE2538 00011ACC ADE00124 */  sw         $zero, 0x124($t7)
    /* DE253C 00011AD0 2418FFFF */  addiu      $t8, $zero, -0x1
    /* DE2540 00011AD4 8FB90020 */  lw         $t9, 0x20($sp)
    /* DE2544 00011AD8 AF380128 */  sw         $t8, 0x128($t9)
    /* DE2548 00011ADC AFA0001C */  sw         $zero, 0x1C($sp)
  .L00011AE0:
    /* DE254C 00011AE0 8FA80020 */  lw         $t0, 0x20($sp)
    /* DE2550 00011AE4 8D0400C0 */  lw         $a0, 0xC0($t0)
    /* DE2554 00011AE8 0C000000 */  jal        func_00000000
    /* DE2558 00011AEC 00000000 */   nop
    /* DE255C 00011AF0 14400006 */  bnez       $v0, .L00011B0C
    /* DE2560 00011AF4 00000000 */   nop
    /* DE2564 00011AF8 24090001 */  addiu      $t1, $zero, 0x1
    /* DE2568 00011AFC 8FAA0020 */  lw         $t2, 0x20($sp)
    /* DE256C 00011B00 AD49018C */  sw         $t1, 0x18C($t2)
    /* DE2570 00011B04 10000009 */  b          .L00011B2C
    /* DE2574 00011B08 00000000 */   nop
  .L00011B0C:
    /* DE2578 00011B0C 8FAB0020 */  lw         $t3, 0x20($sp)
    /* DE257C 00011B10 01602025 */  or         $a0, $t3, $zero
    /* DE2580 00011B14 8FAC001C */  lw         $t4, 0x1C($sp)
    /* DE2584 00011B18 000C6880 */  sll        $t5, $t4, 2
    /* DE2588 00011B1C 016D7021 */  addu       $t6, $t3, $t5
    /* DE258C 00011B20 8DC500C0 */  lw         $a1, 0xC0($t6)
    /* DE2590 00011B24 0C000000 */  jal        func_00000000
    /* DE2594 00011B28 00000000 */   nop
  .L00011B2C:
    /* DE2598 00011B2C 8FAF001C */  lw         $t7, 0x1C($sp)
    /* DE259C 00011B30 25F80001 */  addiu      $t8, $t7, 0x1
    /* DE25A0 00011B34 AFB8001C */  sw         $t8, 0x1C($sp)
    /* DE25A4 00011B38 2B010002 */  slti       $at, $t8, 0x2
    /* DE25A8 00011B3C 1420FFE8 */  bnez       $at, .L00011AE0
    /* DE25AC 00011B40 00000000 */   nop
    /* DE25B0 00011B44 10000001 */  b          .L00011B4C
    /* DE25B4 00011B48 00000000 */   nop
  .L00011B4C:
    /* DE25B8 00011B4C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE25BC 00011B50 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE25C0 00011B54 03E00008 */  jr         $ra
    /* DE25C4 00011B58 00000000 */   nop
endlabel GameFlow_addPads_00011AB4
