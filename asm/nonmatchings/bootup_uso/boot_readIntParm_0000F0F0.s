nonmatching boot_readIntParm_0000F0F0, 0x30

glabel boot_readIntParm_0000F0F0
    /* DDFB5C 0000F0F0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDFB60 0000F0F4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFB64 0000F0F8 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDFB68 0000F0FC 0C003BC8 */  jal        stream_readS32_0000EF20
    /* DDFB6C 0000F100 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDFB70 0000F104 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDFB74 0000F108 0C003BC8 */  jal        stream_readS32_0000EF20
    /* DDFB78 0000F10C 24840010 */   addiu     $a0, $a0, 0x10
    /* DDFB7C 0000F110 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFB80 0000F114 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDFB84 0000F118 03E00008 */  jr         $ra
    /* DDFB88 0000F11C 00000000 */   nop
endlabel boot_readIntParm_0000F0F0
