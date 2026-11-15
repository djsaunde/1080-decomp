nonmatching SnowBoard_softReset, 0x30

glabel SnowBoard_softReset
    /* DDFE70 0000F404 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDFE74 0000F408 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDFE78 0000F40C AFA40018 */  sw         $a0, 0x18($sp)
    /* DDFE7C 0000F410 8FA40018 */  lw         $a0, 0x18($sp)
    /* DDFE80 0000F414 0C000000 */  jal        func_00000000
    /* DDFE84 0000F418 00000000 */   nop
    /* DDFE88 0000F41C 10000001 */  b          .L0000F424
    /* DDFE8C 0000F420 00000000 */   nop
  .L0000F424:
    /* DDFE90 0000F424 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDFE94 0000F428 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDFE98 0000F42C 03E00008 */  jr         $ra
    /* DDFE9C 0000F430 00000000 */   nop
endlabel SnowBoard_softReset
