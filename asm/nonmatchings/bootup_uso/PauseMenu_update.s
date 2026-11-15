nonmatching PauseMenu_update, 0x1F4

glabel PauseMenu_update
    /* DE0DB8 0001034C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE0DBC 00010350 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE0DC0 00010354 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE0DC4 00010358 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE0DC8 0001035C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE0DCC 00010360 8DCF0034 */  lw         $t7, 0x34($t6)
    /* DE0DD0 00010364 29E100FF */  slti       $at, $t7, 0xFF
    /* DE0DD4 00010368 10200006 */  beqz       $at, .L00010384
    /* DE0DD8 0001036C 00000000 */   nop
    /* DE0DDC 00010370 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0DE0 00010374 26100034 */  addiu      $s0, $s0, 0x34
    /* DE0DE4 00010378 8E180000 */  lw         $t8, 0x0($s0)
    /* DE0DE8 0001037C 27190001 */  addiu      $t9, $t8, 0x1
    /* DE0DEC 00010380 AE190000 */  sw         $t9, 0x0($s0)
  .L00010384:
    /* DE0DF0 00010384 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0DF4 00010388 8E080038 */  lw         $t0, 0x38($s0)
    /* DE0DF8 0001038C 25090001 */  addiu      $t1, $t0, 0x1
    /* DE0DFC 00010390 AE090038 */  sw         $t1, 0x38($s0)
    /* DE0E00 00010394 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE0E04 00010398 8D4B0080 */  lw         $t3, 0x80($t2)
    /* DE0E08 0001039C 1560002F */  bnez       $t3, .L0001045C
    /* DE0E0C 000103A0 00000000 */   nop
    /* DE0E10 000103A4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE0E14 000103A8 24840000 */  addiu      $a0, $a0, 0x0
    /* DE0E18 000103AC 24052004 */  addiu      $a1, $zero, 0x2004
    /* DE0E1C 000103B0 0C000000 */  jal        func_00000000
    /* DE0E20 000103B4 00000000 */   nop
    /* DE0E24 000103B8 10400028 */  beqz       $v0, .L0001045C
    /* DE0E28 000103BC 00000000 */   nop
    /* DE0E2C 000103C0 24040001 */  addiu      $a0, $zero, 0x1
    /* DE0E30 000103C4 0C000000 */  jal        func_00000000
    /* DE0E34 000103C8 00000000 */   nop
    /* DE0E38 000103CC AFA00024 */  sw         $zero, 0x24($sp)
    /* DE0E3C 000103D0 8FAC0028 */  lw         $t4, 0x28($sp)
    /* DE0E40 000103D4 8D8D0078 */  lw         $t5, 0x78($t4)
    /* DE0E44 000103D8 19A00020 */  blez       $t5, .L0001045C
    /* DE0E48 000103DC 00000000 */   nop
  .L000103E0:
    /* DE0E4C 000103E0 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0E50 000103E4 8E0E007C */  lw         $t6, 0x7C($s0)
    /* DE0E54 000103E8 25CFFFFF */  addiu      $t7, $t6, -0x1
    /* DE0E58 000103EC AE0F007C */  sw         $t7, 0x7C($s0)
    /* DE0E5C 000103F0 8FB80028 */  lw         $t8, 0x28($sp)
    /* DE0E60 000103F4 8F19007C */  lw         $t9, 0x7C($t8)
    /* DE0E64 000103F8 07210005 */  bgez       $t9, .L00010410
    /* DE0E68 000103FC 00000000 */   nop
    /* DE0E6C 00010400 8FA80028 */  lw         $t0, 0x28($sp)
    /* DE0E70 00010404 8D090078 */  lw         $t1, 0x78($t0)
    /* DE0E74 00010408 252AFFFF */  addiu      $t2, $t1, -0x1
    /* DE0E78 0001040C AD0A007C */  sw         $t2, 0x7C($t0)
  .L00010410:
    /* DE0E7C 00010410 8FAB0028 */  lw         $t3, 0x28($sp)
    /* DE0E80 00010414 8D6C007C */  lw         $t4, 0x7C($t3)
    /* DE0E84 00010418 000C6880 */  sll        $t5, $t4, 2
    /* DE0E88 0001041C 01AC6821 */  addu       $t5, $t5, $t4
    /* DE0E8C 00010420 000D68C0 */  sll        $t5, $t5, 3
    /* DE0E90 00010424 016D7021 */  addu       $t6, $t3, $t5
    /* DE0E94 00010428 8DCF0088 */  lw         $t7, 0x88($t6)
    /* DE0E98 0001042C 11E00003 */  beqz       $t7, .L0001043C
    /* DE0E9C 00010430 00000000 */   nop
    /* DE0EA0 00010434 10000009 */  b          .L0001045C
    /* DE0EA4 00010438 00000000 */   nop
  .L0001043C:
    /* DE0EA8 0001043C 8FB80024 */  lw         $t8, 0x24($sp)
    /* DE0EAC 00010440 27190001 */  addiu      $t9, $t8, 0x1
    /* DE0EB0 00010444 AFB90024 */  sw         $t9, 0x24($sp)
    /* DE0EB4 00010448 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE0EB8 0001044C 8D2A0078 */  lw         $t2, 0x78($t1)
    /* DE0EBC 00010450 032A082A */  slt        $at, $t9, $t2
    /* DE0EC0 00010454 1420FFE2 */  bnez       $at, .L000103E0
    /* DE0EC4 00010458 00000000 */   nop
  .L0001045C:
    /* DE0EC8 0001045C 8FA80028 */  lw         $t0, 0x28($sp)
    /* DE0ECC 00010460 8D0C0080 */  lw         $t4, 0x80($t0)
    /* DE0ED0 00010464 1580002F */  bnez       $t4, .L00010524
    /* DE0ED4 00010468 00000000 */   nop
    /* DE0ED8 0001046C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE0EDC 00010470 24840000 */  addiu      $a0, $a0, 0x0
    /* DE0EE0 00010474 34058008 */  ori        $a1, $zero, 0x8008
    /* DE0EE4 00010478 0C000000 */  jal        func_00000000
    /* DE0EE8 0001047C 00000000 */   nop
    /* DE0EEC 00010480 10400028 */  beqz       $v0, .L00010524
    /* DE0EF0 00010484 00000000 */   nop
    /* DE0EF4 00010488 24040001 */  addiu      $a0, $zero, 0x1
    /* DE0EF8 0001048C 0C000000 */  jal        func_00000000
    /* DE0EFC 00010490 00000000 */   nop
    /* DE0F00 00010494 AFA00024 */  sw         $zero, 0x24($sp)
    /* DE0F04 00010498 8FAB0028 */  lw         $t3, 0x28($sp)
    /* DE0F08 0001049C 8D6D0078 */  lw         $t5, 0x78($t3)
    /* DE0F0C 000104A0 19A00020 */  blez       $t5, .L00010524
    /* DE0F10 000104A4 00000000 */   nop
  .L000104A8:
    /* DE0F14 000104A8 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0F18 000104AC 8E0E007C */  lw         $t6, 0x7C($s0)
    /* DE0F1C 000104B0 25CF0001 */  addiu      $t7, $t6, 0x1
    /* DE0F20 000104B4 AE0F007C */  sw         $t7, 0x7C($s0)
    /* DE0F24 000104B8 8FB80028 */  lw         $t8, 0x28($sp)
    /* DE0F28 000104BC 8F09007C */  lw         $t1, 0x7C($t8)
    /* DE0F2C 000104C0 8F190078 */  lw         $t9, 0x78($t8)
    /* DE0F30 000104C4 0139082A */  slt        $at, $t1, $t9
    /* DE0F34 000104C8 14200003 */  bnez       $at, .L000104D8
    /* DE0F38 000104CC 00000000 */   nop
    /* DE0F3C 000104D0 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE0F40 000104D4 AD40007C */  sw         $zero, 0x7C($t2)
  .L000104D8:
    /* DE0F44 000104D8 8FA80028 */  lw         $t0, 0x28($sp)
    /* DE0F48 000104DC 8D0C007C */  lw         $t4, 0x7C($t0)
    /* DE0F4C 000104E0 000C5880 */  sll        $t3, $t4, 2
    /* DE0F50 000104E4 016C5821 */  addu       $t3, $t3, $t4
    /* DE0F54 000104E8 000B58C0 */  sll        $t3, $t3, 3
    /* DE0F58 000104EC 010B6821 */  addu       $t5, $t0, $t3
    /* DE0F5C 000104F0 8DAE0088 */  lw         $t6, 0x88($t5)
    /* DE0F60 000104F4 11C00003 */  beqz       $t6, .L00010504
    /* DE0F64 000104F8 00000000 */   nop
    /* DE0F68 000104FC 10000009 */  b          .L00010524
    /* DE0F6C 00010500 00000000 */   nop
  .L00010504:
    /* DE0F70 00010504 8FAF0024 */  lw         $t7, 0x24($sp)
    /* DE0F74 00010508 25F80001 */  addiu      $t8, $t7, 0x1
    /* DE0F78 0001050C AFB80024 */  sw         $t8, 0x24($sp)
    /* DE0F7C 00010510 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE0F80 00010514 8D390078 */  lw         $t9, 0x78($t1)
    /* DE0F84 00010518 0319082A */  slt        $at, $t8, $t9
    /* DE0F88 0001051C 1420FFE2 */  bnez       $at, .L000104A8
    /* DE0F8C 00010520 00000000 */   nop
  .L00010524:
    /* DE0F90 00010524 10000001 */  b          .L0001052C
    /* DE0F94 00010528 00000000 */   nop
  .L0001052C:
    /* DE0F98 0001052C 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE0F9C 00010530 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE0FA0 00010534 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE0FA4 00010538 03E00008 */  jr         $ra
    /* DE0FA8 0001053C 00000000 */   nop
endlabel PauseMenu_update
