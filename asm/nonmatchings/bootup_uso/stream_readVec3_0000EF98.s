nonmatching stream_readVec3_0000EF98, 0x70

glabel stream_readVec3_0000EF98
    /* DDFA04 0000EF98 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DDFA08 0000EF9C 00803825 */  or         $a3, $a0, $zero
    /* DDFA0C 0000EFA0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFA10 0000EFA4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDFA14 0000EFA8 24840000 */  addiu      $a0, $a0, 0x0
    /* DDFA18 0000EFAC AFA70048 */  sw         $a3, 0x48($sp)
    /* DDFA1C 0000EFB0 27A50038 */  addiu      $a1, $sp, 0x38
    /* DDFA20 0000EFB4 0C000000 */  jal        func_00000000
    /* DDFA24 0000EFB8 2406000C */   addiu     $a2, $zero, 0xC
    /* DDFA28 0000EFBC 27AF0038 */  addiu      $t7, $sp, 0x38
    /* DDFA2C 0000EFC0 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDFA30 0000EFC4 8FA70048 */  lw         $a3, 0x48($sp)
    /* DDFA34 0000EFC8 27AE0024 */  addiu      $t6, $sp, 0x24
    /* DDFA38 0000EFCC ADD90000 */  sw         $t9, 0x0($t6)
    /* DDFA3C 0000EFD0 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDFA40 0000EFD4 ADD80004 */  sw         $t8, 0x4($t6)
    /* DDFA44 0000EFD8 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDFA48 0000EFDC ADD90008 */  sw         $t9, 0x8($t6)
    /* DDFA4C 0000EFE0 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DDFA50 0000EFE4 E4E40000 */  swc1       $f4, 0x0($a3)
    /* DDFA54 0000EFE8 C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DDFA58 0000EFEC E4E60004 */  swc1       $f6, 0x4($a3)
    /* DDFA5C 0000EFF0 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DDFA60 0000EFF4 E4E80008 */  swc1       $f8, 0x8($a3)
    /* DDFA64 0000EFF8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFA68 0000EFFC 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DDFA6C 0000F000 03E00008 */  jr         $ra
    /* DDFA70 0000F004 00000000 */   nop
endlabel stream_readVec3_0000EF98
