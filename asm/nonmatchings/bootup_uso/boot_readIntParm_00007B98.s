nonmatching boot_readIntParm_00007B98, 0x30

glabel boot_readIntParm_00007B98
    /* DD8604 00007B98 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD8608 00007B9C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD860C 00007BA0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD8610 00007BA4 0C0013FC */  jal        stream_readS32_00004FF0
    /* DD8614 00007BA8 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD8618 00007BAC 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD861C 00007BB0 0C00140B */  jal        stream_readS32_0000502C
    /* DD8620 00007BB4 24840010 */   addiu     $a0, $a0, 0x10
    /* DD8624 00007BB8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8628 00007BBC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD862C 00007BC0 03E00008 */  jr         $ra
    /* DD8630 00007BC4 00000000 */   nop
endlabel boot_readIntParm_00007B98
