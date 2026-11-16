nonmatching stream_readF32_00008A40, 0x3C

glabel stream_readF32_00008A40
    /* DD94AC 00008A40 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD94B0 00008A44 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD94B4 00008A48 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD94B8 00008A4C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD94BC 00008A50 24840000 */  addiu      $a0, $a0, 0x0
    /* DD94C0 00008A54 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD94C4 00008A58 0C000000 */  jal        func_00000000
    /* DD94C8 00008A5C 24060004 */   addiu     $a2, $zero, 0x4
    /* DD94CC 00008A60 C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DD94D0 00008A64 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DD94D4 00008A68 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DD94D8 00008A6C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD94DC 00008A70 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD94E0 00008A74 03E00008 */  jr         $ra
    /* DD94E4 00008A78 00000000 */   nop
endlabel stream_readF32_00008A40
