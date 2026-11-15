nonmatching Process_resume_0000FAE8, 0xE4

glabel Process_resume_0000FAE8
    /* DE0554 0000FAE8 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DE0558 0000FAEC AFBF0024 */  sw         $ra, 0x24($sp)
    /* DE055C 0000FAF0 AFA40038 */  sw         $a0, 0x38($sp)
    /* DE0560 0000FAF4 AFB30020 */  sw         $s3, 0x20($sp)
    /* DE0564 0000FAF8 AFB2001C */  sw         $s2, 0x1C($sp)
    /* DE0568 0000FAFC AFB10018 */  sw         $s1, 0x18($sp)
    /* DE056C 0000FB00 AFB00014 */  sw         $s0, 0x14($sp)
    /* DE0570 0000FB04 0C000000 */  jal        func_00000000
    /* DE0574 0000FB08 00000000 */   nop
    /* DE0578 0000FB0C 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DE057C 0000FB10 8DCF0038 */  lw         $t7, 0x38($t6)
    /* DE0580 0000FB14 15E00005 */  bnez       $t7, .L0000FB2C
    /* DE0584 0000FB18 00000000 */   nop
    /* DE0588 0000FB1C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE058C 0000FB20 24840000 */  addiu      $a0, $a0, 0x0
    /* DE0590 0000FB24 0C000000 */  jal        func_00000000
    /* DE0594 0000FB28 00000000 */   nop
  .L0000FB2C:
    /* DE0598 0000FB2C 8FB80038 */  lw         $t8, 0x38($sp)
    /* DE059C 0000FB30 8F040048 */  lw         $a0, 0x48($t8)
    /* DE05A0 0000FB34 0C000000 */  jal        func_00000000
    /* DE05A4 0000FB38 00000000 */   nop
    /* DE05A8 0000FB3C 8FB90038 */  lw         $t9, 0x38($sp)
    /* DE05AC 0000FB40 AF200030 */  sw         $zero, 0x30($t9)
    /* DE05B0 0000FB44 8FA80038 */  lw         $t0, 0x38($sp)
    /* DE05B4 0000FB48 AD000034 */  sw         $zero, 0x34($t0)
    /* DE05B8 0000FB4C 8FA90038 */  lw         $t1, 0x38($sp)
    /* DE05BC 0000FB50 8D2A004C */  lw         $t2, 0x4C($t1)
    /* DE05C0 0000FB54 11400013 */  beqz       $t2, .L0000FBA4
    /* DE05C4 0000FB58 00000000 */   nop
    /* DE05C8 0000FB5C 8FAB0038 */  lw         $t3, 0x38($sp)
    /* DE05CC 0000FB60 8D70004C */  lw         $s0, 0x4C($t3)
    /* DE05D0 0000FB64 26100018 */  addiu      $s0, $s0, 0x18
    /* DE05D4 0000FB68 02008825 */  or         $s1, $s0, $zero
    /* DE05D8 0000FB6C 24120001 */  addiu      $s2, $zero, 0x1
    /* DE05DC 0000FB70 12400007 */  beqz       $s2, .L0000FB90
    /* DE05E0 0000FB74 00000000 */   nop
    /* DE05E4 0000FB78 02209825 */  or         $s3, $s1, $zero
    /* DE05E8 0000FB7C 8E6C0000 */  lw         $t4, 0x0($s3)
    /* DE05EC 0000FB80 358D0008 */  ori        $t5, $t4, 0x8
    /* DE05F0 0000FB84 AE6D0000 */  sw         $t5, 0x0($s3)
    /* DE05F4 0000FB88 10000006 */  b          .L0000FBA4
    /* DE05F8 0000FB8C 00000000 */   nop
  .L0000FB90:
    /* DE05FC 0000FB90 02209825 */  or         $s3, $s1, $zero
    /* DE0600 0000FB94 8E6E0000 */  lw         $t6, 0x0($s3)
    /* DE0604 0000FB98 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DE0608 0000FB9C 01C17824 */  and        $t7, $t6, $at
    /* DE060C 0000FBA0 AE6F0000 */  sw         $t7, 0x0($s3)
  .L0000FBA4:
    /* DE0610 0000FBA4 10000001 */  b          .L0000FBAC
    /* DE0614 0000FBA8 00000000 */   nop
  .L0000FBAC:
    /* DE0618 0000FBAC 8FB00014 */  lw         $s0, 0x14($sp)
    /* DE061C 0000FBB0 8FB10018 */  lw         $s1, 0x18($sp)
    /* DE0620 0000FBB4 8FB2001C */  lw         $s2, 0x1C($sp)
    /* DE0624 0000FBB8 8FB30020 */  lw         $s3, 0x20($sp)
    /* DE0628 0000FBBC 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DE062C 0000FBC0 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DE0630 0000FBC4 03E00008 */  jr         $ra
    /* DE0634 0000FBC8 00000000 */   nop
endlabel Process_resume_0000FAE8
