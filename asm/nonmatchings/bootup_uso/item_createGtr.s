nonmatching item_createGtr, 0xB4

glabel item_createGtr
    /* DD5DA0 00005334 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD5DA4 00005338 AFA40038 */  sw         $a0, 0x38($sp)
    /* DD5DA8 0000533C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5DAC 00005340 3C040000 */  lui        $a0, %hi(D_00007DE8)
    /* DD5DB0 00005344 0C000000 */  jal        func_00000000
    /* DD5DB4 00005348 24847DE8 */   addiu     $a0, $a0, %lo(D_00007DE8)
    /* DD5DB8 0000534C 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DD5DBC 00005350 24040050 */  addiu      $a0, $zero, 0x50
    /* DD5DC0 00005354 0C000000 */  jal        func_00000000
    /* DD5DC4 00005358 AFAE002C */   sw        $t6, 0x2C($sp)
    /* DD5DC8 0000535C 1040001D */  beqz       $v0, .L000053D4
    /* DD5DCC 00005360 00401825 */   or        $v1, $v0, $zero
    /* DD5DD0 00005364 8FAF002C */  lw         $t7, 0x2C($sp)
    /* DD5DD4 00005368 00402025 */  or         $a0, $v0, $zero
    /* DD5DD8 0000536C 14400007 */  bnez       $v0, .L0000538C
    /* DD5DDC 00005370 AFAF001C */   sw        $t7, 0x1C($sp)
    /* DD5DE0 00005374 24040048 */  addiu      $a0, $zero, 0x48
    /* DD5DE4 00005378 0C000000 */  jal        func_00000000
    /* DD5DE8 0000537C AFA20030 */   sw        $v0, 0x30($sp)
    /* DD5DEC 00005380 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5DF0 00005384 1040000C */  beqz       $v0, .L000053B8
    /* DD5DF4 00005388 00402025 */   or        $a0, $v0, $zero
  .L0000538C:
    /* DD5DF8 0000538C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5DFC 00005390 00003025 */  or         $a2, $zero, $zero
    /* DD5E00 00005394 AFA30030 */  sw         $v1, 0x30($sp)
    /* DD5E04 00005398 AFA40024 */  sw         $a0, 0x24($sp)
    /* DD5E08 0000539C 0C000000 */  jal        func_00000000
    /* DD5E0C 000053A0 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5E10 000053A4 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD5E14 000053A8 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD5E18 000053AC 27390000 */  addiu      $t9, $t9, 0x0
    /* DD5E1C 000053B0 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5E20 000053B4 AC990028 */  sw         $t9, 0x28($a0)
  .L000053B8:
    /* DD5E24 000053B8 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD5E28 000053BC 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD5E2C 000053C0 25290000 */  addiu      $t1, $t1, 0x0
    /* DD5E30 000053C4 25080000 */  addiu      $t0, $t0, 0x0
    /* DD5E34 000053C8 AC680028 */  sw         $t0, 0x28($v1)
    /* DD5E38 000053CC AC69004C */  sw         $t1, 0x4C($v1)
    /* DD5E3C 000053D0 AC600048 */  sw         $zero, 0x48($v1)
  .L000053D4:
    /* DD5E40 000053D4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5E44 000053D8 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD5E48 000053DC 00601025 */  or         $v0, $v1, $zero
    /* DD5E4C 000053E0 03E00008 */  jr         $ra
    /* DD5E50 000053E4 00000000 */   nop
endlabel item_createGtr
