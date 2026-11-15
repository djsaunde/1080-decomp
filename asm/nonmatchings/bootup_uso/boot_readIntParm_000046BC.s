nonmatching boot_readIntParm_000046BC, 0x30

glabel boot_readIntParm_000046BC
    /* DD5128 000046BC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD512C 000046C0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5130 000046C4 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD5134 000046C8 0C0008B8 */  jal        stream_readS32_000022E0
    /* DD5138 000046CC 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD513C 000046D0 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD5140 000046D4 0C0008B8 */  jal        stream_readS32_000022E0
    /* DD5144 000046D8 24840010 */   addiu     $a0, $a0, 0x10
    /* DD5148 000046DC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD514C 000046E0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD5150 000046E4 03E00008 */  jr         $ra
    /* DD5154 000046E8 00000000 */   nop
endlabel boot_readIntParm_000046BC
