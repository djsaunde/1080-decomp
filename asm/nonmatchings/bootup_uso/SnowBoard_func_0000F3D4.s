nonmatching SnowBoard_func_0000F3D4, 0x30

glabel SnowBoard_func_0000F3D4
    /* DDFE40 0000F3D4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDFE44 0000F3D8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFE48 0000F3DC AFA40018 */  sw         $a0, 0x18($sp)
    /* DDFE4C 0000F3E0 8FA40018 */  lw         $a0, 0x18($sp)
    /* DDFE50 0000F3E4 0C000000 */  jal        func_00000000
    /* DDFE54 0000F3E8 00000000 */   nop
    /* DDFE58 0000F3EC 10000001 */  b          .L0000F3F4
    /* DDFE5C 0000F3F0 00000000 */   nop
  .L0000F3F4:
    /* DDFE60 0000F3F4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFE64 0000F3F8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDFE68 0000F3FC 03E00008 */  jr         $ra
    /* DDFE6C 0000F400 00000000 */   nop
endlabel SnowBoard_func_0000F3D4
