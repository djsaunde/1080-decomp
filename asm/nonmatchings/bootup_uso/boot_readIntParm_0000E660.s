nonmatching boot_readIntParm_0000E660, 0x30

glabel boot_readIntParm_0000E660
    /* DDF0CC 0000E660 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF0D0 0000E664 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF0D4 0000E668 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF0D8 0000E66C 0C002270 */  jal        stream_readS32_000089C0
    /* DDF0DC 0000E670 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF0E0 0000E674 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF0E4 0000E678 0C002270 */  jal        stream_readS32_000089C0
    /* DDF0E8 0000E67C 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF0EC 0000E680 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF0F0 0000E684 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF0F4 0000E688 03E00008 */  jr         $ra
    /* DDF0F8 0000E68C 00000000 */   nop
endlabel boot_readIntParm_0000E660
