nonmatching SndHandle_setScaleIdx31_000008D4, 0x20

glabel SndHandle_setScaleIdx31_000008D4
    /* DD1340 000008D4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD1344 000008D8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1348 000008DC 0C000000 */  jal        func_00000000
    /* DD134C 000008E0 8C840000 */   lw        $a0, 0x0($a0)
    /* DD1350 000008E4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1354 000008E8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD1358 000008EC 03E00008 */  jr         $ra
    /* DD135C 000008F0 00000000 */   nop
endlabel SndHandle_setScaleIdx31_000008D4
