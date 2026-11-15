nonmatching boot_readIntParm_0000EE5C, 0x30

glabel boot_readIntParm_0000EE5C
    /* DDF8C8 0000EE5C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF8CC 0000EE60 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF8D0 0000EE64 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF8D4 0000EE68 0C003B20 */  jal        stream_readS32_0000EC80
    /* DDF8D8 0000EE6C 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF8DC 0000EE70 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF8E0 0000EE74 0C003B20 */  jal        stream_readS32_0000EC80
    /* DDF8E4 0000EE78 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF8E8 0000EE7C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF8EC 0000EE80 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF8F0 0000EE84 03E00008 */  jr         $ra
    /* DDF8F4 0000EE88 00000000 */   nop
endlabel boot_readIntParm_0000EE5C
