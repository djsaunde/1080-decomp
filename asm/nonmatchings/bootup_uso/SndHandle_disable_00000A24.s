nonmatching SndHandle_disable_00000A24, 0x2C

glabel SndHandle_disable_00000A24
    /* DD1490 00000A24 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD1494 00000A28 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1498 00000A2C 8C850000 */  lw         $a1, 0x0($a0)
    /* DD149C 00000A30 50A00004 */  beql       $a1, $zero, .L00000A44
    /* DD14A0 00000A34 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD14A4 00000A38 0C000000 */  jal        func_00000000
    /* DD14A8 00000A3C 00A02025 */   or        $a0, $a1, $zero
    /* DD14AC 00000A40 8FBF0014 */  lw         $ra, 0x14($sp)
  .L00000A44:
    /* DD14B0 00000A44 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD14B4 00000A48 03E00008 */  jr         $ra
    /* DD14B8 00000A4C 00000000 */   nop
endlabel SndHandle_disable_00000A24
