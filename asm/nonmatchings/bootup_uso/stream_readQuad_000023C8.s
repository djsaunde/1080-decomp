nonmatching stream_readQuad_000023C8, 0x58

glabel stream_readQuad_000023C8
    /* DD2E34 000023C8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD2E38 000023CC AFA40028 */  sw         $a0, 0x28($sp)
    /* DD2E3C 000023D0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2E40 000023D4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD2E44 000023D8 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2E48 000023DC 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD2E4C 000023E0 0C000000 */  jal        func_00000000
    /* DD2E50 000023E4 24060010 */   addiu     $a2, $zero, 0x10
    /* DD2E54 000023E8 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DD2E58 000023EC 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD2E5C 000023F0 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DD2E60 000023F4 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD2E64 000023F8 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD2E68 000023FC ADD80004 */  sw         $t8, 0x4($t6)
    /* DD2E6C 00002400 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD2E70 00002404 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD2E74 00002408 8DF8000C */  lw         $t8, 0xC($t7)
    /* DD2E78 0000240C ADD8000C */  sw         $t8, 0xC($t6)
    /* DD2E7C 00002410 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2E80 00002414 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD2E84 00002418 03E00008 */  jr         $ra
    /* DD2E88 0000241C 00000000 */   nop
endlabel stream_readQuad_000023C8
