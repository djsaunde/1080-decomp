nonmatching item_createChk, 0xB0

glabel item_createChk
    /* DD5CF0 00005284 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD5CF4 00005288 AFA40038 */  sw         $a0, 0x38($sp)
    /* DD5CF8 0000528C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5CFC 00005290 3C040000 */  lui        $a0, %hi(sStr_rom_checkpoint_uso_7DD4)
    /* DD5D00 00005294 0C000000 */  jal        func_00000000
    /* DD5D04 00005298 24847DD4 */   addiu     $a0, $a0, %lo(sStr_rom_checkpoint_uso_7DD4)
    /* DD5D08 0000529C 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DD5D0C 000052A0 2404004C */  addiu      $a0, $zero, 0x4C
    /* DD5D10 000052A4 0C000000 */  jal        func_00000000
    /* DD5D14 000052A8 AFAE002C */   sw        $t6, 0x2C($sp)
    /* DD5D18 000052AC 1040001C */  beqz       $v0, .L00005320
    /* DD5D1C 000052B0 00401825 */   or        $v1, $v0, $zero
    /* DD5D20 000052B4 8FAF002C */  lw         $t7, 0x2C($sp)
    /* DD5D24 000052B8 00402025 */  or         $a0, $v0, $zero
    /* DD5D28 000052BC 14400007 */  bnez       $v0, .L000052DC
    /* DD5D2C 000052C0 AFAF001C */   sw        $t7, 0x1C($sp)
    /* DD5D30 000052C4 24040048 */  addiu      $a0, $zero, 0x48
    /* DD5D34 000052C8 0C000000 */  jal        func_00000000
    /* DD5D38 000052CC AFA20030 */   sw        $v0, 0x30($sp)
    /* DD5D3C 000052D0 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5D40 000052D4 1040000C */  beqz       $v0, .L00005308
    /* DD5D44 000052D8 00402025 */   or        $a0, $v0, $zero
  .L000052DC:
    /* DD5D48 000052DC 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5D4C 000052E0 00003025 */  or         $a2, $zero, $zero
    /* DD5D50 000052E4 AFA30030 */  sw         $v1, 0x30($sp)
    /* DD5D54 000052E8 AFA40024 */  sw         $a0, 0x24($sp)
    /* DD5D58 000052EC 0C000000 */  jal        func_00000000
    /* DD5D5C 000052F0 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5D60 000052F4 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD5D64 000052F8 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD5D68 000052FC 27390000 */  addiu      $t9, $t9, 0x0
    /* DD5D6C 00005300 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5D70 00005304 AC990028 */  sw         $t9, 0x28($a0)
  .L00005308:
    /* DD5D74 00005308 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD5D78 0000530C 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD5D7C 00005310 25290000 */  addiu      $t1, $t1, 0x0
    /* DD5D80 00005314 25080000 */  addiu      $t0, $t0, 0x0
    /* DD5D84 00005318 AC680028 */  sw         $t0, 0x28($v1)
    /* DD5D88 0000531C AC690048 */  sw         $t1, 0x48($v1)
  .L00005320:
    /* DD5D8C 00005320 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5D90 00005324 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD5D94 00005328 00601025 */  or         $v0, $v1, $zero
    /* DD5D98 0000532C 03E00008 */  jr         $ra
    /* DD5D9C 00005330 00000000 */   nop
endlabel item_createChk
