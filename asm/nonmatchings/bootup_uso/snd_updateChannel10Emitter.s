nonmatching snd_updateChannel10Emitter, 0x44

glabel snd_updateChannel10Emitter
    /* DD0D88 0000031C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD0D8C 00000320 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD0D90 00000324 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0D94 00000328 AFA50024 */  sw         $a1, 0x24($sp)
    /* DD0D98 0000032C 00C03825 */  or         $a3, $a2, $zero
    /* DD0D9C 00000330 00A02025 */  or         $a0, $a1, $zero
    /* DD0DA0 00000334 27A5001C */  addiu      $a1, $sp, 0x1C
    /* DD0DA4 00000338 0C000000 */  jal        func_00000000
    /* DD0DA8 0000033C 27A60018 */   addiu     $a2, $sp, 0x18
    /* DD0DAC 00000340 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD0DB0 00000344 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD0DB4 00000348 0C000000 */  jal        func_00000000
    /* DD0DB8 0000034C 83A6001B */   lb        $a2, 0x1B($sp)
    /* DD0DBC 00000350 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0DC0 00000354 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD0DC4 00000358 03E00008 */  jr         $ra
    /* DD0DC8 0000035C 00000000 */   nop
endlabel snd_updateChannel10Emitter
