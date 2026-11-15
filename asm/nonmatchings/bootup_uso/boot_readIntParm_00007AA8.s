nonmatching boot_readIntParm_00007AA8, 0x30

glabel boot_readIntParm_00007AA8
    /* DD8514 00007AA8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD8518 00007AAC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD851C 00007AB0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD8520 00007AB4 0C0013FC */  jal        stream_readS32_00004FF0
  alabel D_00007AB8
    /* DD8524 00007AB8 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD8528 00007ABC 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD852C 00007AC0 0C0013FC */  jal        stream_readS32_00004FF0
    /* DD8530 00007AC4 24840010 */   addiu     $a0, $a0, 0x10
    /* DD8534 00007AC8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8538 00007ACC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD853C 00007AD0 03E00008 */  jr         $ra
    /* DD8540 00007AD4 00000000 */   nop
endlabel boot_readIntParm_00007AA8
