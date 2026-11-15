nonmatching stream_readVec3_00006194, 0x70

glabel stream_readVec3_00006194
    /* DD6C00 00006194 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD6C04 00006198 00803825 */  or         $a3, $a0, $zero
    /* DD6C08 0000619C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD6C0C 000061A0 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6C10 000061A4 24840000 */  addiu      $a0, $a0, 0x0
    /* DD6C14 000061A8 AFA70048 */  sw         $a3, 0x48($sp)
    /* DD6C18 000061AC 27A50038 */  addiu      $a1, $sp, 0x38
    /* DD6C1C 000061B0 0C000000 */  jal        func_00000000
    /* DD6C20 000061B4 2406000C */   addiu     $a2, $zero, 0xC
    /* DD6C24 000061B8 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DD6C28 000061BC 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD6C2C 000061C0 8FA70048 */  lw         $a3, 0x48($sp)
    /* DD6C30 000061C4 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DD6C34 000061C8 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD6C38 000061CC 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD6C3C 000061D0 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD6C40 000061D4 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD6C44 000061D8 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD6C48 000061DC C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DD6C4C 000061E0 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DD6C50 000061E4 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DD6C54 000061E8 E4E60004 */  swc1       $f6, 0x4($a3)
    /* DD6C58 000061EC C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DD6C5C 000061F0 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DD6C60 000061F4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD6C64 000061F8 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD6C68 000061FC 03E00008 */  jr         $ra
    /* DD6C6C 00006200 00000000 */   nop
endlabel stream_readVec3_00006194
