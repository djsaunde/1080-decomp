nonmatching game_createBGBox, 0x1CC

glabel game_createBGBox
    /* DD69A0 00005F34 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* DD69A4 00005F38 AFB0001C */  sw         $s0, 0x1C($sp)
    /* DD69A8 00005F3C 00A08025 */  or         $s0, $a1, $zero
    /* DD69AC 00005F40 AFA7004C */  sw         $a3, 0x4C($sp)
    /* DD69B0 00005F44 00803825 */  or         $a3, $a0, $zero
    /* DD69B4 00005F48 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD69B8 00005F4C AFA40040 */  sw         $a0, 0x40($sp)
    /* DD69BC 00005F50 AFA60048 */  sw         $a2, 0x48($sp)
    /* DD69C0 00005F54 3C050000 */  lui        $a1, %hi(D_00007E6C)
    /* DD69C4 00005F58 AFB10020 */  sw         $s1, 0x20($sp)
    /* DD69C8 00005F5C 24A57E6C */  addiu      $a1, $a1, %lo(D_00007E6C)
    /* DD69CC 00005F60 00003025 */  or         $a2, $zero, $zero
    /* DD69D0 00005F64 0C000000 */  jal        func_00000000
    /* DD69D4 00005F68 00002025 */   or        $a0, $zero, $zero
    /* DD69D8 00005F6C 00408825 */  or         $s1, $v0, $zero
    /* DD69DC 00005F70 00002025 */  or         $a0, $zero, $zero
    /* DD69E0 00005F74 02002825 */  or         $a1, $s0, $zero
    /* DD69E4 00005F78 00003025 */  or         $a2, $zero, $zero
    /* DD69E8 00005F7C 00003825 */  or         $a3, $zero, $zero
    /* DD69EC 00005F80 0C000000 */  jal        func_00000000
    /* DD69F0 00005F84 AFA00010 */   sw        $zero, 0x10($sp)
    /* DD69F4 00005F88 26240010 */  addiu      $a0, $s1, 0x10
    /* DD69F8 00005F8C AFA40028 */  sw         $a0, 0x28($sp)
    /* DD69FC 00005F90 00408025 */  or         $s0, $v0, $zero
    /* DD6A00 00005F94 0C000000 */  jal        func_00000000
    /* DD6A04 00005F98 00402825 */   or        $a1, $v0, $zero
    /* DD6A08 00005F9C 8E0E0014 */  lw         $t6, 0x14($s0)
    /* DD6A0C 00005FA0 00002025 */  or         $a0, $zero, $zero
    /* DD6A10 00005FA4 00003025 */  or         $a2, $zero, $zero
    /* DD6A14 00005FA8 11C00003 */  beqz       $t6, .L00005FB8
    /* DD6A18 00005FAC 2407005A */   addiu     $a3, $zero, 0x5A
    /* DD6A1C 00005FB0 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD6A20 00005FB4 AE0F0004 */  sw         $t7, 0x4($s0)
  .L00005FB8:
    /* DD6A24 00005FB8 AE110014 */  sw         $s1, 0x14($s0)
    /* DD6A28 00005FBC AFA00010 */  sw         $zero, 0x10($sp)
    /* DD6A2C 00005FC0 0C000000 */  jal        func_00000000
    /* DD6A30 00005FC4 8FA50048 */   lw        $a1, 0x48($sp)
    /* DD6A34 00005FC8 00408025 */  or         $s0, $v0, $zero
    /* DD6A38 00005FCC 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD6A3C 00005FD0 0C000000 */  jal        func_00000000
    /* DD6A40 00005FD4 00402825 */   or        $a1, $v0, $zero
    /* DD6A44 00005FD8 8E180014 */  lw         $t8, 0x14($s0)
    /* DD6A48 00005FDC 00002025 */  or         $a0, $zero, $zero
    /* DD6A4C 00005FE0 00003025 */  or         $a2, $zero, $zero
    /* DD6A50 00005FE4 13000003 */  beqz       $t8, .L00005FF4
    /* DD6A54 00005FE8 240700B4 */   addiu     $a3, $zero, 0xB4
    /* DD6A58 00005FEC 24190001 */  addiu      $t9, $zero, 0x1
    /* DD6A5C 00005FF0 AE190004 */  sw         $t9, 0x4($s0)
  .L00005FF4:
    /* DD6A60 00005FF4 AE110014 */  sw         $s1, 0x14($s0)
    /* DD6A64 00005FF8 AFA00010 */  sw         $zero, 0x10($sp)
    /* DD6A68 00005FFC 0C000000 */  jal        func_00000000
    /* DD6A6C 00006000 8FA5004C */   lw        $a1, 0x4C($sp)
    /* DD6A70 00006004 00408025 */  or         $s0, $v0, $zero
    /* DD6A74 00006008 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD6A78 0000600C 0C000000 */  jal        func_00000000
    /* DD6A7C 00006010 00402825 */   or        $a1, $v0, $zero
    /* DD6A80 00006014 8E080014 */  lw         $t0, 0x14($s0)
    /* DD6A84 00006018 00002025 */  or         $a0, $zero, $zero
    /* DD6A88 0000601C 00003025 */  or         $a2, $zero, $zero
    /* DD6A8C 00006020 11000003 */  beqz       $t0, .L00006030
    /* DD6A90 00006024 2407010E */   addiu     $a3, $zero, 0x10E
    /* DD6A94 00006028 24090001 */  addiu      $t1, $zero, 0x1
    /* DD6A98 0000602C AE090004 */  sw         $t1, 0x4($s0)
  .L00006030:
    /* DD6A9C 00006030 AE110014 */  sw         $s1, 0x14($s0)
    /* DD6AA0 00006034 AFA00010 */  sw         $zero, 0x10($sp)
    /* DD6AA4 00006038 0C000000 */  jal        func_00000000
    /* DD6AA8 0000603C 8FA50050 */   lw        $a1, 0x50($sp)
    /* DD6AAC 00006040 00408025 */  or         $s0, $v0, $zero
    /* DD6AB0 00006044 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD6AB4 00006048 0C000000 */  jal        func_00000000
    /* DD6AB8 0000604C 00402825 */   or        $a1, $v0, $zero
    /* DD6ABC 00006050 8E0A0014 */  lw         $t2, 0x14($s0)
    /* DD6AC0 00006054 00002025 */  or         $a0, $zero, $zero
    /* DD6AC4 00006058 2406FFA6 */  addiu      $a2, $zero, -0x5A
    /* DD6AC8 0000605C 11400003 */  beqz       $t2, .L0000606C
    /* DD6ACC 00006060 00003825 */   or        $a3, $zero, $zero
    /* DD6AD0 00006064 240B0001 */  addiu      $t3, $zero, 0x1
    /* DD6AD4 00006068 AE0B0004 */  sw         $t3, 0x4($s0)
  .L0000606C:
    /* DD6AD8 0000606C AE110014 */  sw         $s1, 0x14($s0)
    /* DD6ADC 00006070 AFA00010 */  sw         $zero, 0x10($sp)
    /* DD6AE0 00006074 0C000000 */  jal        func_00000000
    /* DD6AE4 00006078 8FA50054 */   lw        $a1, 0x54($sp)
    /* DD6AE8 0000607C 00408025 */  or         $s0, $v0, $zero
    /* DD6AEC 00006080 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD6AF0 00006084 0C000000 */  jal        func_00000000
    /* DD6AF4 00006088 00402825 */   or        $a1, $v0, $zero
    /* DD6AF8 0000608C 8E0C0014 */  lw         $t4, 0x14($s0)
    /* DD6AFC 00006090 240D0001 */  addiu      $t5, $zero, 0x1
    /* DD6B00 00006094 00002025 */  or         $a0, $zero, $zero
    /* DD6B04 00006098 11800002 */  beqz       $t4, .L000060A4
    /* DD6B08 0000609C 2406005A */   addiu     $a2, $zero, 0x5A
    /* DD6B0C 000060A0 AE0D0004 */  sw         $t5, 0x4($s0)
  .L000060A4:
    /* DD6B10 000060A4 8FA50058 */  lw         $a1, 0x58($sp)
    /* DD6B14 000060A8 AE110014 */  sw         $s1, 0x14($s0)
    /* DD6B18 000060AC 00003825 */  or         $a3, $zero, $zero
    /* DD6B1C 000060B0 50A0000E */  beql       $a1, $zero, .L000060EC
    /* DD6B20 000060B4 8FBF0024 */   lw        $ra, 0x24($sp)
    /* DD6B24 000060B8 0C000000 */  jal        func_00000000
    /* DD6B28 000060BC AFA00010 */   sw        $zero, 0x10($sp)
    /* DD6B2C 000060C0 00408025 */  or         $s0, $v0, $zero
    /* DD6B30 000060C4 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD6B34 000060C8 0C000000 */  jal        func_00000000
    /* DD6B38 000060CC 00402825 */   or        $a1, $v0, $zero
    /* DD6B3C 000060D0 8E0E0014 */  lw         $t6, 0x14($s0)
    /* DD6B40 000060D4 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD6B44 000060D8 51C00003 */  beql       $t6, $zero, .L000060E8
    /* DD6B48 000060DC AE110014 */   sw        $s1, 0x14($s0)
    /* DD6B4C 000060E0 AE0F0004 */  sw         $t7, 0x4($s0)
    /* DD6B50 000060E4 AE110014 */  sw         $s1, 0x14($s0)
  .L000060E8:
    /* DD6B54 000060E8 8FBF0024 */  lw         $ra, 0x24($sp)
  .L000060EC:
    /* DD6B58 000060EC 02201025 */  or         $v0, $s1, $zero
    /* DD6B5C 000060F0 8FB10020 */  lw         $s1, 0x20($sp)
    /* DD6B60 000060F4 8FB0001C */  lw         $s0, 0x1C($sp)
    /* DD6B64 000060F8 03E00008 */  jr         $ra
    /* DD6B68 000060FC 27BD0040 */   addiu     $sp, $sp, 0x40
endlabel game_createBGBox
