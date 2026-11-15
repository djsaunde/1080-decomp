nonmatching stream_readVec3_0000E934, 0x70

glabel stream_readVec3_0000E934
    /* DDF3A0 0000E934 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DDF3A4 0000E938 00803825 */  or         $a3, $a0, $zero
    /* DDF3A8 0000E93C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF3AC 0000E940 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF3B0 0000E944 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF3B4 0000E948 AFA70048 */  sw         $a3, 0x48($sp)
    /* DDF3B8 0000E94C 27A50038 */  addiu      $a1, $sp, 0x38
    /* DDF3BC 0000E950 0C000000 */  jal        func_00000000
    /* DDF3C0 0000E954 2406000C */   addiu     $a2, $zero, 0xC
    /* DDF3C4 0000E958 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DDF3C8 0000E95C 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDF3CC 0000E960 8FA70048 */  lw         $a3, 0x48($sp)
    /* DDF3D0 0000E964 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DDF3D4 0000E968 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDF3D8 0000E96C 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDF3DC 0000E970 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDF3E0 0000E974 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDF3E4 0000E978 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDF3E8 0000E97C C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DDF3EC 0000E980 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DDF3F0 0000E984 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DDF3F4 0000E988 E4E60004 */  swc1       $f6, 0x4($a3)
    /* DDF3F8 0000E98C C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DDF3FC 0000E990 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DDF400 0000E994 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF404 0000E998 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DDF408 0000E99C 03E00008 */  jr         $ra
    /* DDF40C 0000E9A0 00000000 */   nop
endlabel stream_readVec3_0000E934
