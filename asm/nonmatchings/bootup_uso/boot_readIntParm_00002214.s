nonmatching boot_readIntParm_00002214, 0x30

glabel boot_readIntParm_00002214
    /* DD2C80 00002214 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2C84 00002218 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2C88 0000221C AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2C8C 00002220 0C000704 */  jal        stream_readS32_00001C10
    /* DD2C90 00002224 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD2C94 00002228 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD2C98 0000222C 0C000704 */  jal        stream_readS32_00001C10
    /* DD2C9C 00002230 24840010 */   addiu     $a0, $a0, 0x10
    /* DD2CA0 00002234 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2CA4 00002238 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2CA8 0000223C 03E00008 */  jr         $ra
    /* DD2CAC 00002240 00000000 */   nop
endlabel boot_readIntParm_00002214
