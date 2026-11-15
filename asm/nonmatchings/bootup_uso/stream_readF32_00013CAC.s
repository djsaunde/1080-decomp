nonmatching stream_readF32_00013CAC, 0x3C

glabel stream_readF32_00013CAC
    /* DE4718 00013CAC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE471C 00013CB0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DE4720 00013CB4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE4724 00013CB8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE4728 00013CBC 24840000 */  addiu      $a0, $a0, 0x0
    /* DE472C 00013CC0 27A50018 */  addiu      $a1, $sp, 0x18
    /* DE4730 00013CC4 0C000000 */  jal        func_00000000
    /* DE4734 00013CC8 24060004 */   addiu     $a2, $zero, 0x4
    /* DE4738 00013CCC C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DE473C 00013CD0 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE4740 00013CD4 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DE4744 00013CD8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE4748 00013CDC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE474C 00013CE0 03E00008 */  jr         $ra
    /* DE4750 00013CE4 00000000 */   nop
endlabel stream_readF32_00013CAC
