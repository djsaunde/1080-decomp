nonmatching stream_readF32_00000044, 0x3C

glabel stream_readF32_00000044
    /* DD0AB0 00000044 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD0AB4 00000048 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD0AB8 0000004C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0ABC 00000050 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD0AC0 00000054 24840000 */  addiu      $a0, $a0, 0x0
    /* DD0AC4 00000058 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD0AC8 0000005C 0C000000 */  jal        func_00000000
    /* DD0ACC 00000060 24060004 */   addiu     $a2, $zero, 0x4
    /* DD0AD0 00000064 C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DD0AD4 00000068 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DD0AD8 0000006C E5C40000 */  swc1       $f4, 0x0($t6)
    /* DD0ADC 00000070 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0AE0 00000074 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD0AE4 00000078 03E00008 */  jr         $ra
    /* DD0AE8 0000007C 00000000 */   nop
endlabel stream_readF32_00000044
