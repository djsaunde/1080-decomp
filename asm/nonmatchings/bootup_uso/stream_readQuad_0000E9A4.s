nonmatching stream_readQuad_0000E9A4, 0x58

glabel stream_readQuad_0000E9A4
    /* DDF410 0000E9A4 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDF414 0000E9A8 AFA40028 */  sw         $a0, 0x28($sp)
    /* DDF418 0000E9AC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF41C 0000E9B0 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF420 0000E9B4 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF424 0000E9B8 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF428 0000E9BC 0C000000 */  jal        func_00000000
    /* DDF42C 0000E9C0 24060010 */   addiu     $a2, $zero, 0x10
    /* DDF430 0000E9C4 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DDF434 0000E9C8 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDF438 0000E9CC 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DDF43C 0000E9D0 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDF440 0000E9D4 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDF444 0000E9D8 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDF448 0000E9DC 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDF44C 0000E9E0 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDF450 0000E9E4 8DF8000C */  lw         $t8, 0xC($t7)
    /* DDF454 0000E9E8 ADD8000C */  sw         $t8, 0xC($t6)
    /* DDF458 0000E9EC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF45C 0000E9F0 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDF460 0000E9F4 03E00008 */  jr         $ra
    /* DDF464 0000E9F8 00000000 */   nop
endlabel stream_readQuad_0000E9A4
