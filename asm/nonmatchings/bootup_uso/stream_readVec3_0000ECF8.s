nonmatching stream_readVec3_0000ECF8, 0x70

glabel stream_readVec3_0000ECF8
    /* DDF764 0000ECF8 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DDF768 0000ECFC 00803825 */  or         $a3, $a0, $zero
    /* DDF76C 0000ED00 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF770 0000ED04 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF774 0000ED08 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF778 0000ED0C AFA70048 */  sw         $a3, 0x48($sp)
    /* DDF77C 0000ED10 27A50038 */  addiu      $a1, $sp, 0x38
    /* DDF780 0000ED14 0C000000 */  jal        func_00000000
    /* DDF784 0000ED18 2406000C */   addiu     $a2, $zero, 0xC
    /* DDF788 0000ED1C 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DDF78C 0000ED20 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDF790 0000ED24 8FA70048 */  lw         $a3, 0x48($sp)
    /* DDF794 0000ED28 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DDF798 0000ED2C ADD90000 */  sw         $t9, 0x0($t6)
    /* DDF79C 0000ED30 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDF7A0 0000ED34 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDF7A4 0000ED38 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDF7A8 0000ED3C ADD90008 */  sw         $t9, 0x8($t6)
    /* DDF7AC 0000ED40 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DDF7B0 0000ED44 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DDF7B4 0000ED48 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DDF7B8 0000ED4C E4E60004 */  swc1       $f6, 0x4($a3)
    /* DDF7BC 0000ED50 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DDF7C0 0000ED54 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DDF7C4 0000ED58 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF7C8 0000ED5C 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DDF7CC 0000ED60 03E00008 */  jr         $ra
    /* DDF7D0 0000ED64 00000000 */   nop
endlabel stream_readVec3_0000ECF8
