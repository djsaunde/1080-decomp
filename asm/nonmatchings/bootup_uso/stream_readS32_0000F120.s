nonmatching stream_readS32_0000F120, 0x3C

glabel stream_readS32_0000F120
    /* DDFB8C 0000F120 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDFB90 0000F124 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDFB94 0000F128 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFB98 0000F12C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDFB9C 0000F130 24840000 */  addiu      $a0, $a0, 0x0
    /* DDFBA0 0000F134 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDFBA4 0000F138 0C000000 */  jal        func_00000000
    /* DDFBA8 0000F13C 24060004 */   addiu     $a2, $zero, 0x4
    /* DDFBAC 0000F140 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DDFBB0 0000F144 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DDFBB4 0000F148 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DDFBB8 0000F14C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFBBC 0000F150 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDFBC0 0000F154 03E00008 */  jr         $ra
    /* DDFBC4 0000F158 00000000 */   nop
endlabel stream_readS32_0000F120
