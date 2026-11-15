nonmatching settings_vol2Down_00012598, 0x68

glabel settings_vol2Down_00012598
    /* DE3004 00012598 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE3008 0001259C AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE300C 000125A0 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE3010 000125A4 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE3014 000125A8 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE3018 000125AC 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE301C 000125B0 91F80014 */  lbu        $t8, 0x14($t7)
    /* DE3020 000125B4 1B000006 */  blez       $t8, .L000125D0
    /* DE3024 000125B8 00000000 */   nop
    /* DE3028 000125BC 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE302C 000125C0 8F300154 */  lw         $s0, 0x154($t9)
    /* DE3030 000125C4 92080014 */  lbu        $t0, 0x14($s0)
    /* DE3034 000125C8 2509FFFF */  addiu      $t1, $t0, -0x1
    /* DE3038 000125CC A2090014 */  sb         $t1, 0x14($s0)
  .L000125D0:
    /* DE303C 000125D0 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE3040 000125D4 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE3044 000125D8 91640014 */  lbu        $a0, 0x14($t3)
    /* DE3048 000125DC 0C000000 */  jal        func_00000000
    /* DE304C 000125E0 00000000 */   nop
    /* DE3050 000125E4 10000001 */  b          .L000125EC
    /* DE3054 000125E8 00000000 */   nop
  .L000125EC:
    /* DE3058 000125EC 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE305C 000125F0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE3060 000125F4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE3064 000125F8 03E00008 */  jr         $ra
    /* DE3068 000125FC 00000000 */   nop
endlabel settings_vol2Down_00012598
