nonmatching boot_readQuadParm_0000EB88, 0x30

glabel boot_readQuadParm_0000EB88
    /* DDF5F4 0000EB88 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF5F8 0000EB8C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF5FC 0000EB90 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF600 0000EB94 0C003A2F */  jal        stream_readS32_0000E8BC
    /* DDF604 0000EB98 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF608 0000EB9C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF60C 0000EBA0 0C003A69 */  jal        stream_readQuad_0000E9A4
    /* DDF610 0000EBA4 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF614 0000EBA8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF618 0000EBAC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF61C 0000EBB0 03E00008 */  jr         $ra
    /* DDF620 0000EBB4 00000000 */   nop
endlabel boot_readQuadParm_0000EB88
