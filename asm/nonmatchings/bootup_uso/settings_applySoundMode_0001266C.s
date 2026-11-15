nonmatching settings_applySoundMode_0001266C, 0x80

glabel settings_applySoundMode_0001266C
    /* DE30D8 0001266C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE30DC 00012670 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE30E0 00012674 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE30E4 00012678 AFA5002C */  sw         $a1, 0x2C($sp)
    /* DE30E8 0001267C AFB00018 */  sw         $s0, 0x18($sp)
    /* DE30EC 00012680 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE30F0 00012684 8DD00154 */  lw         $s0, 0x154($t6)
    /* DE30F4 00012688 26100004 */  addiu      $s0, $s0, 0x4
    /* DE30F8 0001268C 960F0000 */  lhu        $t7, 0x0($s0)
    /* DE30FC 00012690 2401FFFC */  addiu      $at, $zero, -0x4
    /* DE3100 00012694 01E1C024 */  and        $t8, $t7, $at
    /* DE3104 00012698 A6180000 */  sh         $t8, 0x0($s0)
    /* DE3108 0001269C 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE310C 000126A0 8F300154 */  lw         $s0, 0x154($t9)
    /* DE3110 000126A4 26100004 */  addiu      $s0, $s0, 0x4
    /* DE3114 000126A8 96080000 */  lhu        $t0, 0x0($s0)
    /* DE3118 000126AC 8FA9002C */  lw         $t1, 0x2C($sp)
    /* DE311C 000126B0 01095025 */  or         $t2, $t0, $t1
    /* DE3120 000126B4 A60A0000 */  sh         $t2, 0x0($s0)
    /* DE3124 000126B8 8FAB0028 */  lw         $t3, 0x28($sp)
    /* DE3128 000126BC 8D6C0154 */  lw         $t4, 0x154($t3)
    /* DE312C 000126C0 95840004 */  lhu        $a0, 0x4($t4)
    /* DE3130 000126C4 30840003 */  andi       $a0, $a0, 0x3
    /* DE3134 000126C8 0C000000 */  jal        func_00000000
    /* DE3138 000126CC 00000000 */   nop
    /* DE313C 000126D0 10000001 */  b          .L000126D8
    /* DE3140 000126D4 00000000 */   nop
  .L000126D8:
    /* DE3144 000126D8 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE3148 000126DC 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE314C 000126E0 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE3150 000126E4 03E00008 */  jr         $ra
    /* DE3154 000126E8 00000000 */   nop
endlabel settings_applySoundMode_0001266C
