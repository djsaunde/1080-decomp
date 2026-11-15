nonmatching stream_readVec3_00002358, 0x70

glabel stream_readVec3_00002358
    /* DD2DC4 00002358 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD2DC8 0000235C 00803825 */  or         $a3, $a0, $zero
    /* DD2DCC 00002360 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2DD0 00002364 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD2DD4 00002368 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2DD8 0000236C AFA70048 */  sw         $a3, 0x48($sp)
    /* DD2DDC 00002370 27A50038 */  addiu      $a1, $sp, 0x38
    /* DD2DE0 00002374 0C000000 */  jal        func_00000000
    /* DD2DE4 00002378 2406000C */   addiu     $a2, $zero, 0xC
    /* DD2DE8 0000237C 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DD2DEC 00002380 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD2DF0 00002384 8FA70048 */  lw         $a3, 0x48($sp)
    /* DD2DF4 00002388 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DD2DF8 0000238C ADD90000 */  sw         $t9, 0x0($t6)
    /* DD2DFC 00002390 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD2E00 00002394 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD2E04 00002398 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD2E08 0000239C ADD90008 */  sw         $t9, 0x8($t6)
    /* DD2E0C 000023A0 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DD2E10 000023A4 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DD2E14 000023A8 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DD2E18 000023AC E4E60004 */  swc1       $f6, 0x4($a3)
    /* DD2E1C 000023B0 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DD2E20 000023B4 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DD2E24 000023B8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2E28 000023BC 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD2E2C 000023C0 03E00008 */  jr         $ra
    /* DD2E30 000023C4 00000000 */   nop
endlabel stream_readVec3_00002358
