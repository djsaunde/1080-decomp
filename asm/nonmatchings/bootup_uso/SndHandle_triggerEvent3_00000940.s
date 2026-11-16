nonmatching SndHandle_triggerEvent3_00000940, 0x4C

glabel SndHandle_triggerEvent3_00000940
    /* DD13AC 00000940 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD13B0 00000944 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD13B4 00000948 AFA50024 */  sw         $a1, 0x24($sp)
    /* DD13B8 0000094C 8C850008 */  lw         $a1, 0x8($a0)
    /* DD13BC 00000950 0C000000 */  jal        func_00000000
    /* DD13C0 00000954 AFA40020 */   sw        $a0, 0x20($sp)
    /* DD13C4 00000958 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD13C8 0000095C 00402825 */  or         $a1, $v0, $zero
    /* DD13CC 00000960 24060003 */  addiu      $a2, $zero, 0x3
    /* DD13D0 00000964 8C8E0000 */  lw         $t6, 0x0($a0)
    /* DD13D4 00000968 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD13D8 0000096C 11C00003 */  beqz       $t6, .L0000097C
    /* DD13DC 00000970 AFAE0018 */   sw        $t6, 0x18($sp)
    /* DD13E0 00000974 0C000000 */  jal        func_00000000
    /* DD13E4 00000978 01C02025 */   or        $a0, $t6, $zero
  .L0000097C:
    /* DD13E8 0000097C 8FBF0014 */  lw         $ra, 0x14($sp)
  alabel kRo_SnowBoarder_update_980
    /* DD13EC 00000980 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD13F0 00000984 03E00008 */  jr         $ra
  alabel kRo_SnowBoarder_handleMsg_988
    /* DD13F4 00000988 00000000 */   nop
endlabel SndHandle_triggerEvent3_00000940
