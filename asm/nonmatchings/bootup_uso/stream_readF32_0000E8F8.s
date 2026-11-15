nonmatching stream_readF32_0000E8F8, 0x3C

glabel stream_readF32_0000E8F8
    /* DDF364 0000E8F8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF368 0000E8FC AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF36C 0000E900 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF370 0000E904 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF374 0000E908 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF378 0000E90C 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF37C 0000E910 0C000000 */  jal        func_00000000
    /* DDF380 0000E914 24060004 */   addiu     $a2, $zero, 0x4
    /* DDF384 0000E918 C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DDF388 0000E91C 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DDF38C 0000E920 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DDF390 0000E924 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF394 0000E928 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF398 0000E92C 03E00008 */  jr         $ra
    /* DDF39C 0000E930 00000000 */   nop
endlabel stream_readF32_0000E8F8
