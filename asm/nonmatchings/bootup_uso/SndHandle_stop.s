nonmatching SndHandle_stop, 0x2C

glabel SndHandle_stop
    /* DD12F4 00000888 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD12F8 0000088C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD12FC 00000890 8C850000 */  lw         $a1, 0x0($a0)
    /* DD1300 00000894 50A00004 */  beql       $a1, $zero, .L000008A8
    /* DD1304 00000898 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD1308 0000089C 0C000000 */  jal        func_00000000
    /* DD130C 000008A0 00A02025 */   or        $a0, $a1, $zero
    /* DD1310 000008A4 8FBF0014 */  lw         $ra, 0x14($sp)
  .L000008A8:
    /* DD1314 000008A8 27BD0018 */  addiu      $sp, $sp, 0x18
  alabel D_000008AC
    /* DD1318 000008AC 03E00008 */  jr         $ra
  alabel D_000008B0
    /* DD131C 000008B0 00000000 */   nop
endlabel SndHandle_stop
