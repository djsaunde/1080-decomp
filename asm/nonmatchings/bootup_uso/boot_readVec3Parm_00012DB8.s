nonmatching boot_readVec3Parm_00012DB8, 0x40

glabel boot_readVec3Parm_00012DB8
    /* DE3824 00012DB8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE3828 00012DBC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE382C 00012DC0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DE3830 00012DC4 27A4001C */  addiu      $a0, $sp, 0x1C
    /* DE3834 00012DC8 0C003C7C */  jal        stream_readS32_0000F1F0
    /* DE3838 00012DCC 00000000 */   nop
    /* DE383C 00012DD0 8FA40020 */  lw         $a0, 0x20($sp)
    /* DE3840 00012DD4 24840010 */  addiu      $a0, $a0, 0x10
    /* DE3844 00012DD8 0C003CBB */  jal        stream_readVec3_0000F2EC
    /* DE3848 00012DDC 00000000 */   nop
    /* DE384C 00012DE0 10000001 */  b          .L00012DE8
    /* DE3850 00012DE4 00000000 */   nop
  .L00012DE8:
    /* DE3854 00012DE8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE3858 00012DEC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE385C 00012DF0 03E00008 */  jr         $ra
    /* DE3860 00012DF4 00000000 */   nop
endlabel boot_readVec3Parm_00012DB8
    /* DE3864 00012DF8 00000000 */  nop
    /* DE3868 00012DFC 00000000 */  nop
