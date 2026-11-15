nonmatching stream_readF32_0000EF5C, 0x3C

glabel stream_readF32_0000EF5C
    /* DDF9C8 0000EF5C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF9CC 0000EF60 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF9D0 0000EF64 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF9D4 0000EF68 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF9D8 0000EF6C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF9DC 0000EF70 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF9E0 0000EF74 0C000000 */  jal        func_00000000
    /* DDF9E4 0000EF78 24060004 */   addiu     $a2, $zero, 0x4
    /* DDF9E8 0000EF7C C7A40018 */  lwc1       $f4, 0x18($sp)
    /* DDF9EC 0000EF80 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DDF9F0 0000EF84 E5C40000 */  swc1       $f4, 0x0($t6)
    /* DDF9F4 0000EF88 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF9F8 0000EF8C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF9FC 0000EF90 03E00008 */  jr         $ra
    /* DDFA00 0000EF94 00000000 */   nop
endlabel stream_readF32_0000EF5C
