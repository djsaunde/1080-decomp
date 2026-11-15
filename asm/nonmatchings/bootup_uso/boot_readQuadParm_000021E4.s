nonmatching boot_readQuadParm_000021E4, 0x30

glabel boot_readQuadParm_000021E4
    /* DD2C50 000021E4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2C54 000021E8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2C58 000021EC AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2C5C 000021F0 0C000704 */  jal        stream_readS32_00001C10
    /* DD2C60 000021F4 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD2C64 000021F8 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD2C68 000021FC 0C00074D */  jal        stream_readQuad_00001D34
    /* DD2C6C 00002200 24840010 */   addiu     $a0, $a0, 0x10
    /* DD2C70 00002204 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2C74 00002208 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2C78 0000220C 03E00008 */  jr         $ra
    /* DD2C7C 00002210 00000000 */   nop
endlabel boot_readQuadParm_000021E4
