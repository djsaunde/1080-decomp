nonmatching stream_readF32_00006100, 0x3C

glabel stream_readF32_00006100
    /* DD6B6C 00006100 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD6B70 00006104 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD6B74 00006108 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD6B78 0000610C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6B7C 00006110 24840000 */  addiu      $a0, $a0, 0x0
    /* DD6B80 00006114 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD6B84 00006118 0C000000 */  jal        func_00000000
    /* DD6B88 0000611C 24060004 */   addiu     $a2, $zero, 0x4
    /* DD6B8C 00006120 C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DD6B90 00006124 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DD6B94 00006128 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DD6B98 0000612C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD6B9C 00006130 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD6BA0 00006134 03E00008 */  jr         $ra
    /* DD6BA4 00006138 00000000 */   nop
endlabel stream_readF32_00006100
