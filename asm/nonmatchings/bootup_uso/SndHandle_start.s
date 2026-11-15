nonmatching SndHandle_start, 0x2C

glabel SndHandle_start
    /* DD12C8 0000085C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD12CC 00000860 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD12D0 00000864 8C850000 */  lw         $a1, 0x0($a0)
    /* DD12D4 00000868 50A00004 */  beql       $a1, $zero, .L0000087C
    /* DD12D8 0000086C 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD12DC 00000870 0C000000 */  jal        func_00000000
    /* DD12E0 00000874 00A02025 */   or        $a0, $a1, $zero
    /* DD12E4 00000878 8FBF0014 */  lw         $ra, 0x14($sp)
  .L0000087C:
    /* DD12E8 0000087C 27BD0018 */  addiu      $sp, $sp, 0x18
  alabel D_00000880
    /* DD12EC 00000880 03E00008 */  jr         $ra
  alabel D_00000884
    /* DD12F0 00000884 00000000 */   nop
endlabel SndHandle_start
