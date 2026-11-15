nonmatching boot_readFloatParm_0000E588, 0x30

glabel boot_readFloatParm_0000E588
    /* DDEFF4 0000E588 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDEFF8 0000E58C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDEFFC 0000E590 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF000 0000E594 0C002270 */  jal        stream_readS32_000089C0
    /* DDF004 0000E598 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF008 0000E59C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF00C 0000E5A0 0C002290 */  jal        func_00008A40
    /* DDF010 0000E5A4 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF014 0000E5A8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF018 0000E5AC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF01C 0000E5B0 03E00008 */  jr         $ra
    /* DDF020 0000E5B4 00000000 */   nop
endlabel boot_readFloatParm_0000E588
