nonmatching stream_readS32_00001CF8, 0x3C

glabel stream_readS32_00001CF8
    /* DD2764 00001CF8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2768 00001CFC AFA40020 */  sw         $a0, 0x20($sp)
    /* DD276C 00001D00 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2770 00001D04 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD2774 00001D08 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2778 00001D0C 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD277C 00001D10 0C000000 */  jal        func_00000000
    /* DD2780 00001D14 24060004 */   addiu     $a2, $zero, 0x4
    /* DD2784 00001D18 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD2788 00001D1C 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD278C 00001D20 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD2790 00001D24 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2794 00001D28 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2798 00001D2C 03E00008 */  jr         $ra
    /* DD279C 00001D30 00000000 */   nop
endlabel stream_readS32_00001CF8
