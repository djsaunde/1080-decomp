nonmatching boot_readVec3Parm_00002184, 0x30

glabel boot_readVec3Parm_00002184
    /* DD2BF0 00002184 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2BF4 00002188 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2BF8 0000218C AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2BFC 00002190 0C000704 */  jal        stream_readS32_00001C10
    /* DD2C00 00002194 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD2C04 00002198 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD2C08 0000219C 0C000722 */  jal        stream_readVec3_00001C88
    /* DD2C0C 000021A0 24840010 */   addiu     $a0, $a0, 0x10
    /* DD2C10 000021A4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2C14 000021A8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2C18 000021AC 03E00008 */  jr         $ra
    /* DD2C1C 000021B0 00000000 */   nop
endlabel boot_readVec3Parm_00002184
