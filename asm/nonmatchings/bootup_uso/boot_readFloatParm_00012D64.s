nonmatching boot_readFloatParm_00012D64, 0x40

glabel boot_readFloatParm_00012D64
    /* DE37D0 00012D64 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE37D4 00012D68 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE37D8 00012D6C AFA40020 */  sw         $a0, 0x20($sp)
    /* DE37DC 00012D70 27A4001C */  addiu      $a0, $sp, 0x1C
    /* DE37E0 00012D74 0C003C7C */  jal        stream_readS32_0000F1F0
    /* DE37E4 00012D78 00000000 */   nop
    /* DE37E8 00012D7C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DE37EC 00012D80 24840010 */  addiu      $a0, $a0, 0x10
    /* DE37F0 00012D84 0C003C8F */  jal        stream_readF32_0000F23C
    /* DE37F4 00012D88 00000000 */   nop
    /* DE37F8 00012D8C 10000001 */  b          .L00012D94
    /* DE37FC 00012D90 00000000 */   nop
  .L00012D94:
    /* DE3800 00012D94 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE3804 00012D98 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE3808 00012D9C 03E00008 */  jr         $ra
    /* DE380C 00012DA0 00000000 */   nop
endlabel boot_readFloatParm_00012D64
