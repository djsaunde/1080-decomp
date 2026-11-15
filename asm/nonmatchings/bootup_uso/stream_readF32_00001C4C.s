nonmatching stream_readF32_00001C4C, 0x3C

glabel stream_readF32_00001C4C
    /* DD26B8 00001C4C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD26BC 00001C50 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD26C0 00001C54 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD26C4 00001C58 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD26C8 00001C5C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD26CC 00001C60 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD26D0 00001C64 0C000000 */  jal        func_00000000
    /* DD26D4 00001C68 24060004 */   addiu     $a2, $zero, 0x4
    /* DD26D8 00001C6C C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DD26DC 00001C70 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DD26E0 00001C74 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DD26E4 00001C78 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD26E8 00001C7C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD26EC 00001C80 03E00008 */  jr         $ra
    /* DD26F0 00001C84 00000000 */   nop
endlabel stream_readF32_00001C4C
