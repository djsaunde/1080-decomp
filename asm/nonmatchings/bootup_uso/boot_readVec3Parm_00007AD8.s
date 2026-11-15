nonmatching boot_readVec3Parm_00007AD8, 0x30

glabel boot_readVec3Parm_00007AD8
    /* DD8544 00007AD8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD8548 00007ADC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD854C 00007AE0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD8550 00007AE4 0C0013FC */  jal        stream_readS32_00004FF0
    /* DD8554 00007AE8 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD8558 00007AEC 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD855C 00007AF0 0C001865 */  jal        stream_readVec3_00006194
    /* DD8560 00007AF4 24840010 */   addiu     $a0, $a0, 0x10
    /* DD8564 00007AF8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8568 00007AFC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD856C 00007B00 03E00008 */  jr         $ra
    /* DD8570 00007B04 00000000 */   nop
endlabel boot_readVec3Parm_00007AD8
