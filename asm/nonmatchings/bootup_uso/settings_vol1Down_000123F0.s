nonmatching settings_vol1Down_000123F0, 0x68

glabel settings_vol1Down_000123F0
    /* DE2E5C 000123F0 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE2E60 000123F4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE2E64 000123F8 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE2E68 000123FC AFB00018 */  sw         $s0, 0x18($sp)
    /* DE2E6C 00012400 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE2E70 00012404 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE2E74 00012408 91F80012 */  lbu        $t8, 0x12($t7)
    /* DE2E78 0001240C 1B000006 */  blez       $t8, .L00012428
    /* DE2E7C 00012410 00000000 */   nop
    /* DE2E80 00012414 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE2E84 00012418 8F300154 */  lw         $s0, 0x154($t9)
    /* DE2E88 0001241C 92080012 */  lbu        $t0, 0x12($s0)
    /* DE2E8C 00012420 2509FFFF */  addiu      $t1, $t0, -0x1
    /* DE2E90 00012424 A2090012 */  sb         $t1, 0x12($s0)
  .L00012428:
    /* DE2E94 00012428 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE2E98 0001242C 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE2E9C 00012430 91640012 */  lbu        $a0, 0x12($t3)
    /* DE2EA0 00012434 0C000000 */  jal        func_00000000
    /* DE2EA4 00012438 00000000 */   nop
    /* DE2EA8 0001243C 10000001 */  b          .L00012444
    /* DE2EAC 00012440 00000000 */   nop
  .L00012444:
    /* DE2EB0 00012444 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE2EB4 00012448 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE2EB8 0001244C 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE2EBC 00012450 03E00008 */  jr         $ra
    /* DE2EC0 00012454 00000000 */   nop
endlabel settings_vol1Down_000123F0
