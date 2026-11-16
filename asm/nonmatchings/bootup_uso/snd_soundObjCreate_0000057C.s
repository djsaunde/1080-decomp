nonmatching snd_soundObjCreate_0000057C, 0x94

glabel snd_soundObjCreate_0000057C
    /* DD0FE8 0000057C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0FEC 00000580 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0FF0 00000584 00803825 */  or         $a3, $a0, $zero
    /* DD0FF4 00000588 AFA60020 */  sw         $a2, 0x20($sp)
    /* DD0FF8 0000058C AFA70018 */  sw         $a3, 0x18($sp)
    /* DD0FFC 00000590 24040064 */  addiu      $a0, $zero, 0x64
    /* DD1000 00000594 0C000000 */  jal        func_00000000
    /* DD1004 00000598 AFA5001C */   sw        $a1, 0x1C($sp)
    /* DD1008 0000059C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD100C 000005A0 8FA70018 */  lw         $a3, 0x18($sp)
    /* DD1010 000005A4 00003025 */  or         $a2, $zero, $zero
    /* DD1014 000005A8 10A0000B */  beqz       $a1, .L000005D8
    /* DD1018 000005AC ACE20000 */   sw        $v0, 0x0($a3)
    /* DD101C 000005B0 24010001 */  addiu      $at, $zero, 0x1
    /* DD1020 000005B4 10A10005 */  beq        $a1, $at, .L000005CC
    /* DD1024 000005B8 24010002 */   addiu     $at, $zero, 0x2
    /* DD1028 000005BC 50A10006 */  beql       $a1, $at, .L000005D8
    /* DD102C 000005C0 24060002 */   addiu     $a2, $zero, 0x2
    /* DD1030 000005C4 10000005 */  b          .L000005DC
    /* DD1034 000005C8 8FAE0020 */   lw        $t6, 0x20($sp)
  .L000005CC:
    /* DD1038 000005CC 10000002 */  b          .L000005D8
    /* DD103C 000005D0 24060001 */   addiu     $a2, $zero, 0x1
    /* DD1040 000005D4 24060002 */  addiu      $a2, $zero, 0x2
  .L000005D8:
    /* DD1044 000005D8 8FAE0020 */  lw         $t6, 0x20($sp)
  .L000005DC:
    /* DD1048 000005DC 8CEF0000 */  lw         $t7, 0x0($a3)
    /* DD104C 000005E0 44802000 */  mtc1       $zero, $f4
    /* DD1050 000005E4 00C02825 */  or         $a1, $a2, $zero
  alabel D_000005E8
    /* DD1054 000005E8 A1EE0022 */  sb         $t6, 0x22($t7)
  alabel D_000005EC
    /* DD1058 000005EC 8CF80000 */  lw         $t8, 0x0($a3)
  alabel D_000005F0
    /* DD105C 000005F0 E7040010 */  swc1       $f4, 0x10($t8)
    /* DD1060 000005F4 ACE00008 */  sw         $zero, 0x8($a3)
    /* DD1064 000005F8 0C000000 */  jal        func_00000000
    /* DD1068 000005FC 8CE40000 */   lw        $a0, 0x0($a3)
    /* DD106C 00000600 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1070 00000604 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD1074 00000608 03E00008 */  jr         $ra
    /* DD1078 0000060C 00000000 */   nop
endlabel snd_soundObjCreate_0000057C
