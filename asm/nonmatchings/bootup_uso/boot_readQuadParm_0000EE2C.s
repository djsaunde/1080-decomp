nonmatching boot_readQuadParm_0000EE2C, 0x30

glabel boot_readQuadParm_0000EE2C
    /* DDF898 0000EE2C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF89C 0000EE30 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF8A0 0000EE34 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF8A4 0000EE38 0C003B20 */  jal        stream_readS32_0000EC80
    /* DDF8A8 0000EE3C 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF8AC 0000EE40 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF8B0 0000EE44 0C003B5A */  jal        stream_readQuad_0000ED68
    /* DDF8B4 0000EE48 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF8B8 0000EE4C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF8BC 0000EE50 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF8C0 0000EE54 03E00008 */  jr         $ra
    /* DDF8C4 0000EE58 00000000 */   nop
endlabel boot_readQuadParm_0000EE2C
