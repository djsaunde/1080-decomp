nonmatching SndHandle_setFlag1A, 0x2C

glabel SndHandle_setFlag1A
    /* DD14D4 00000A68 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD14D8 00000A6C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD14DC 00000A70 8C850000 */  lw         $a1, 0x0($a0)
    /* DD14E0 00000A74 50A00004 */  beql       $a1, $zero, .L00000A88
    /* DD14E4 00000A78 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD14E8 00000A7C 0C000000 */  jal        func_00000000
    /* DD14EC 00000A80 00A02025 */   or        $a0, $a1, $zero
    /* DD14F0 00000A84 8FBF0014 */  lw         $ra, 0x14($sp)
  .L00000A88:
    /* DD14F4 00000A88 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD14F8 00000A8C 03E00008 */  jr         $ra
    /* DD14FC 00000A90 00000000 */   nop
endlabel SndHandle_setFlag1A
