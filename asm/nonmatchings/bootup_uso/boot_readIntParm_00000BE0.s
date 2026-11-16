nonmatching boot_readIntParm_00000BE0, 0x30

glabel boot_readIntParm_00000BE0
    /* DD164C 00000BE0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD1650 00000BE4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1654 00000BE8 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD1658 00000BEC 0C000002 */  jal        stream_readS32_00000008
    /* DD165C 00000BF0 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD1660 00000BF4 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD1664 00000BF8 0C000002 */  jal        stream_readS32_00000008
    /* DD1668 00000BFC 24840010 */   addiu     $a0, $a0, 0x10
    /* DD166C 00000C00 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1670 00000C04 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD1674 00000C08 03E00008 */  jr         $ra
    /* DD1678 00000C0C 00000000 */   nop
endlabel boot_readIntParm_00000BE0
