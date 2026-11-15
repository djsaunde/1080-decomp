nonmatching boot_readVec3Parm_0000E5B8, 0x30

glabel boot_readVec3Parm_0000E5B8
    /* DDF024 0000E5B8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF028 0000E5BC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF02C 0000E5C0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF030 0000E5C4 0C002270 */  jal        stream_readS32_000089C0
    /* DDF034 0000E5C8 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF038 0000E5CC 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF03C 0000E5D0 0C00229F */  jal        func_00008A7C
    /* DDF040 0000E5D4 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF044 0000E5D8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF048 0000E5DC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF04C 0000E5E0 03E00008 */  jr         $ra
    /* DDF050 0000E5E4 00000000 */   nop
endlabel boot_readVec3Parm_0000E5B8
