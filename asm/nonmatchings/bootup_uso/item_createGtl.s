nonmatching item_createGtl, 0xB8

glabel item_createGtl
    /* DD5E54 000053E8 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD5E58 000053EC AFA40038 */  sw         $a0, 0x38($sp)
    /* DD5E5C 000053F0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5E60 000053F4 3C040000 */  lui        $a0, %hi(D_00007DFC)
    /* DD5E64 000053F8 0C000000 */  jal        func_00000000
    /* DD5E68 000053FC 24847DFC */   addiu     $a0, $a0, %lo(D_00007DFC)
    /* DD5E6C 00005400 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DD5E70 00005404 24040050 */  addiu      $a0, $zero, 0x50
    /* DD5E74 00005408 0C000000 */  jal        func_00000000
    /* DD5E78 0000540C AFAE002C */   sw        $t6, 0x2C($sp)
    /* DD5E7C 00005410 1040001E */  beqz       $v0, .L0000548C
    /* DD5E80 00005414 00401825 */   or        $v1, $v0, $zero
    /* DD5E84 00005418 8FAF002C */  lw         $t7, 0x2C($sp)
    /* DD5E88 0000541C 00402025 */  or         $a0, $v0, $zero
    /* DD5E8C 00005420 14400007 */  bnez       $v0, .L00005440
    /* DD5E90 00005424 AFAF001C */   sw        $t7, 0x1C($sp)
    /* DD5E94 00005428 24040048 */  addiu      $a0, $zero, 0x48
    /* DD5E98 0000542C 0C000000 */  jal        func_00000000
    /* DD5E9C 00005430 AFA20030 */   sw        $v0, 0x30($sp)
    /* DD5EA0 00005434 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5EA4 00005438 1040000C */  beqz       $v0, .L0000546C
    /* DD5EA8 0000543C 00402025 */   or        $a0, $v0, $zero
  .L00005440:
    /* DD5EAC 00005440 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5EB0 00005444 00003025 */  or         $a2, $zero, $zero
    /* DD5EB4 00005448 AFA30030 */  sw         $v1, 0x30($sp)
    /* DD5EB8 0000544C AFA40024 */  sw         $a0, 0x24($sp)
    /* DD5EBC 00005450 0C000000 */  jal        func_00000000
    /* DD5EC0 00005454 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5EC4 00005458 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD5EC8 0000545C 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD5ECC 00005460 27390000 */  addiu      $t9, $t9, 0x0
    /* DD5ED0 00005464 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5ED4 00005468 AC990028 */  sw         $t9, 0x28($a0)
  .L0000546C:
    /* DD5ED8 0000546C 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD5EDC 00005470 3C0A0000 */  lui        $t2, (0x0 >> 16)
    /* DD5EE0 00005474 254A0000 */  addiu      $t2, $t2, 0x0
    /* DD5EE4 00005478 25080000 */  addiu      $t0, $t0, 0x0
    /* DD5EE8 0000547C 24090001 */  addiu      $t1, $zero, 0x1
    /* DD5EEC 00005480 AC690048 */  sw         $t1, 0x48($v1)
    /* DD5EF0 00005484 AC680028 */  sw         $t0, 0x28($v1)
    /* DD5EF4 00005488 AC6A004C */  sw         $t2, 0x4C($v1)
  .L0000548C:
    /* DD5EF8 0000548C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5EFC 00005490 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD5F00 00005494 00601025 */  or         $v0, $v1, $zero
    /* DD5F04 00005498 03E00008 */  jr         $ra
    /* DD5F08 0000549C 00000000 */   nop
endlabel item_createGtl
