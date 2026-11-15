nonmatching boot_readQuadParm_0000E630, 0x30

glabel boot_readQuadParm_0000E630
    /* DDF09C 0000E630 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF0A0 0000E634 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF0A4 0000E638 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF0A8 0000E63C 0C002270 */  jal        stream_readS32_000089C0
    /* DDF0AC 0000E640 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF0B0 0000E644 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF0B4 0000E648 0C0022BB */  jal        func_00008AEC
    /* DDF0B8 0000E64C 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF0BC 0000E650 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF0C0 0000E654 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF0C4 0000E658 03E00008 */  jr         $ra
    /* DDF0C8 0000E65C 00000000 */   nop
endlabel boot_readQuadParm_0000E630
