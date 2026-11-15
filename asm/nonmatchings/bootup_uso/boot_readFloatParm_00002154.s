nonmatching boot_readFloatParm_00002154, 0x30

glabel boot_readFloatParm_00002154
    /* DD2BC0 00002154 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2BC4 00002158 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2BC8 0000215C AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2BCC 00002160 0C000704 */  jal        stream_readS32_00001C10
    /* DD2BD0 00002164 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD2BD4 00002168 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD2BD8 0000216C 0C000713 */  jal        stream_readF32_00001C4C
    /* DD2BDC 00002170 24840010 */   addiu     $a0, $a0, 0x10
    /* DD2BE0 00002174 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2BE4 00002178 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2BE8 0000217C 03E00008 */  jr         $ra
    /* DD2BEC 00002180 00000000 */   nop
endlabel boot_readFloatParm_00002154
