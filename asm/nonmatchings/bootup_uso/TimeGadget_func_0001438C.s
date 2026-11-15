nonmatching TimeGadget_func_0001438C, 0x128

glabel TimeGadget_func_0001438C
    /* DE4DF8 0001438C 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* DE4DFC 00014390 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE4E00 00014394 00808025 */  or         $s0, $a0, $zero
    /* DE4E04 00014398 14800005 */  bnez       $a0, .L000143B0
    /* DE4E08 0001439C AFBF001C */   sw        $ra, 0x1C($sp)
    /* DE4E0C 000143A0 0C000000 */  jal        func_00000000
    /* DE4E10 000143A4 24040038 */   addiu     $a0, $zero, 0x38
    /* DE4E14 000143A8 1040003D */  beqz       $v0, .L000144A0
    /* DE4E18 000143AC 00408025 */   or        $s0, $v0, $zero
  .L000143B0:
    /* DE4E1C 000143B0 16000005 */  bnez       $s0, .L000143C8
    /* DE4E20 000143B4 02001825 */   or        $v1, $s0, $zero
    /* DE4E24 000143B8 0C000000 */  jal        func_00000000
    /* DE4E28 000143BC 24040008 */   addiu     $a0, $zero, 0x8
    /* DE4E2C 000143C0 10400005 */  beqz       $v0, .L000143D8
    /* DE4E30 000143C4 00401825 */   or        $v1, $v0, $zero
  .L000143C8:
    /* DE4E34 000143C8 3C0E0001 */  lui        $t6, %hi(func_0000CACC + 0xC)
    /* DE4E38 000143CC 25CECAD8 */  addiu      $t6, $t6, %lo(func_0000CACC + 0xC)
    /* DE4E3C 000143D0 AC6E0000 */  sw         $t6, 0x0($v1)
    /* DE4E40 000143D4 AC600004 */  sw         $zero, 0x4($v1)
  .L000143D8:
    /* DE4E44 000143D8 3C0F0001 */  lui        $t7, %hi(SnowBoarder_func_0000CAE8 + 0x4)
    /* DE4E48 000143DC 8DEFCAEC */  lw         $t7, %lo(SnowBoarder_func_0000CAE8 + 0x4)($t7)
    /* DE4E4C 000143E0 2401FFF8 */  addiu      $at, $zero, -0x8
    /* DE4E50 000143E4 26040008 */  addiu      $a0, $s0, 0x8
    /* DE4E54 000143E8 AFAF003C */  sw         $t7, 0x3C($sp)
    /* DE4E58 000143EC 16010005 */  bne        $s0, $at, .L00014404
    /* DE4E5C 000143F0 AFAF0024 */   sw        $t7, 0x24($sp)
    /* DE4E60 000143F4 0C000000 */  jal        func_00000000
    /* DE4E64 000143F8 24040018 */   addiu     $a0, $zero, 0x18
    /* DE4E68 000143FC 1040000F */  beqz       $v0, .L0001443C
    /* DE4E6C 00014400 00402025 */   or        $a0, $v0, $zero
  .L00014404:
    /* DE4E70 00014404 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE4E74 00014408 02002825 */  or         $a1, $s0, $zero
    /* DE4E78 0001440C 24070001 */  addiu      $a3, $zero, 0x1
    /* DE4E7C 00014410 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE4E80 00014414 0C000000 */  jal        func_00000000
    /* DE4E84 00014418 AFA60008 */   sw        $a2, 0x8($sp)
    /* DE4E88 0001441C 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE4E8C 00014420 3C0141F0 */  lui        $at, (0x41F00000 >> 16)
    /* DE4E90 00014424 44812000 */  mtc1       $at, $f4
    /* DE4E94 00014428 3C080001 */  lui        $t0, %hi(D_0000C83C)
    /* DE4E98 0001442C 2508C83C */  addiu      $t0, $t0, %lo(D_0000C83C)
    /* DE4E9C 00014430 AC88000C */  sw         $t0, 0xC($a0)
    /* DE4EA0 00014434 AC800014 */  sw         $zero, 0x14($a0)
    /* DE4EA4 00014438 E4840010 */  swc1       $f4, 0x10($a0)
  .L0001443C:
    /* DE4EA8 0001443C 3C090001 */  lui        $t1, %hi(SnowBoarder_func_0000CAE8 + 0x8)
    /* DE4EAC 00014440 8D29CAF0 */  lw         $t1, %lo(SnowBoarder_func_0000CAE8 + 0x8)($t1)
    /* DE4EB0 00014444 2401FFE0 */  addiu      $at, $zero, -0x20
    /* DE4EB4 00014448 26040020 */  addiu      $a0, $s0, 0x20
    /* DE4EB8 0001444C AFA90038 */  sw         $t1, 0x38($sp)
    /* DE4EBC 00014450 16010005 */  bne        $s0, $at, .L00014468
    /* DE4EC0 00014454 AFA90024 */   sw        $t1, 0x24($sp)
    /* DE4EC4 00014458 0C000000 */  jal        func_00000000
    /* DE4EC8 0001445C 24040018 */   addiu     $a0, $zero, 0x18
    /* DE4ECC 00014460 1040000F */  beqz       $v0, .L000144A0
    /* DE4ED0 00014464 00402025 */   or        $a0, $v0, $zero
  .L00014468:
    /* DE4ED4 00014468 8FA60024 */  lw         $a2, 0x24($sp)
    /* DE4ED8 0001446C 02002825 */  or         $a1, $s0, $zero
    /* DE4EDC 00014470 24070001 */  addiu      $a3, $zero, 0x1
    /* DE4EE0 00014474 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE4EE4 00014478 0C000000 */  jal        func_00000000
    /* DE4EE8 0001447C AFA60008 */   sw        $a2, 0x8($sp)
    /* DE4EEC 00014480 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE4EF0 00014484 3C0141F0 */  lui        $at, (0x41F00000 >> 16)
    /* DE4EF4 00014488 44813000 */  mtc1       $at, $f6
    /* DE4EF8 0001448C 3C0C0001 */  lui        $t4, %hi(D_0000C83C)
    /* DE4EFC 00014490 258CC83C */  addiu      $t4, $t4, %lo(D_0000C83C)
    /* DE4F00 00014494 AC8C000C */  sw         $t4, 0xC($a0)
    /* DE4F04 00014498 AC800014 */  sw         $zero, 0x14($a0)
    /* DE4F08 0001449C E4860010 */  swc1       $f6, 0x10($a0)
  .L000144A0:
    /* DE4F0C 000144A0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE4F10 000144A4 02001025 */  or         $v0, $s0, $zero
    /* DE4F14 000144A8 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE4F18 000144AC 03E00008 */  jr         $ra
    /* DE4F1C 000144B0 27BD0040 */   addiu     $sp, $sp, 0x40
endlabel TimeGadget_func_0001438C
