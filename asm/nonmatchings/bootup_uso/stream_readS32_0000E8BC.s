nonmatching stream_readS32_0000E8BC, 0x3C

glabel stream_readS32_0000E8BC
    /* DDF328 0000E8BC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF32C 0000E8C0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF330 0000E8C4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF334 0000E8C8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF338 0000E8CC 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF33C 0000E8D0 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF340 0000E8D4 0C000000 */  jal        func_00000000
    /* DDF344 0000E8D8 24060004 */   addiu     $a2, $zero, 0x4
    /* DDF348 0000E8DC 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DDF34C 0000E8E0 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DDF350 0000E8E4 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DDF354 0000E8E8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF358 0000E8EC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF35C 0000E8F0 03E00008 */  jr         $ra
    /* DDF360 0000E8F4 00000000 */   nop
endlabel stream_readS32_0000E8BC
