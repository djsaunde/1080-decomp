nonmatching stream_readS32_0000EC80, 0x3C

glabel stream_readS32_0000EC80
    /* DDF6EC 0000EC80 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF6F0 0000EC84 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF6F4 0000EC88 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF6F8 0000EC8C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF6FC 0000EC90 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF700 0000EC94 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF704 0000EC98 0C000000 */  jal        func_00000000
    /* DDF708 0000EC9C 24060004 */   addiu     $a2, $zero, 0x4
    /* DDF70C 0000ECA0 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DDF710 0000ECA4 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DDF714 0000ECA8 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DDF718 0000ECAC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF71C 0000ECB0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF720 0000ECB4 03E00008 */  jr         $ra
    /* DDF724 0000ECB8 00000000 */   nop
endlabel stream_readS32_0000EC80
