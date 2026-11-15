nonmatching boot_readIntParm_000021B4, 0x30

glabel boot_readIntParm_000021B4
    /* DD2C20 000021B4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2C24 000021B8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2C28 000021BC AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2C2C 000021C0 0C000704 */  jal        stream_readS32_00001C10
    /* DD2C30 000021C4 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD2C34 000021C8 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD2C38 000021CC 0C00073E */  jal        stream_readS32_00001CF8
    /* DD2C3C 000021D0 24840010 */   addiu     $a0, $a0, 0x10
    /* DD2C40 000021D4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2C44 000021D8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2C48 000021DC 03E00008 */  jr         $ra
    /* DD2C4C 000021E0 00000000 */   nop
endlabel boot_readIntParm_000021B4
