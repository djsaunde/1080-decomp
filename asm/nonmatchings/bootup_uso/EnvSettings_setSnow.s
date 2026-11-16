nonmatching EnvSettings_setSnow, 0xD4

glabel EnvSettings_setSnow
    /* DD808C 00007620 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8090 00007624 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8094 00007628 8C8E0040 */  lw         $t6, 0x40($a0)
    /* DD8098 0000762C 00803025 */  or         $a2, $a0, $zero
    /* DD809C 00007630 00A02025 */  or         $a0, $a1, $zero
    /* DD80A0 00007634 51C0002C */  beql       $t6, $zero, .L000076E8
    /* DD80A4 00007638 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DD80A8 0000763C AFA5001C */  sw         $a1, 0x1C($sp)
    /* DD80AC 00007640 0C000000 */  jal        func_00000000
    /* DD80B0 00007644 AFA60018 */   sw        $a2, 0x18($sp)
    /* DD80B4 00007648 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD80B8 0000764C 8FA60018 */  lw         $a2, 0x18($sp)
    /* DD80BC 00007650 54A00015 */  bnel       $a1, $zero, .L000076A8
    /* DD80C0 00007654 8CC20040 */   lw        $v0, 0x40($a2)
    /* DD80C4 00007658 8CC30040 */  lw         $v1, 0x40($a2)
    /* DD80C8 0000765C 44802000 */  mtc1       $zero, $f4
    /* DD80CC 00007660 8C6F00BC */  lw         $t7, 0xBC($v1)
    /* DD80D0 00007664 24620018 */  addiu      $v0, $v1, 0x18
    /* DD80D4 00007668 15E0000C */  bnez       $t7, .L0000769C
    /* DD80D8 0000766C 00000000 */   nop
    /* DD80DC 00007670 8C580000 */  lw         $t8, 0x0($v0)
    /* DD80E0 00007674 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DD80E4 00007678 0301C824 */  and        $t9, $t8, $at
    /* DD80E8 0000767C AC590000 */  sw         $t9, 0x0($v0)
    /* DD80EC 00007680 8CC20040 */  lw         $v0, 0x40($a2)
    /* DD80F0 00007684 2401FFFB */  addiu      $at, $zero, -0x5
    /* DD80F4 00007688 8C480018 */  lw         $t0, 0x18($v0)
    /* DD80F8 0000768C 24420018 */  addiu      $v0, $v0, 0x18
    /* DD80FC 00007690 01014824 */  and        $t1, $t0, $at
    /* DD8100 00007694 AC490000 */  sw         $t1, 0x0($v0)
    /* DD8104 00007698 8CC30040 */  lw         $v1, 0x40($a2)
  .L0000769C:
    /* DD8108 0000769C 1000000F */  b          .L000076DC
    /* DD810C 000076A0 E464004C */   swc1      $f4, 0x4C($v1)
    /* DD8110 000076A4 8CC20040 */  lw         $v0, 0x40($a2)
  .L000076A8:
    /* DD8114 000076A8 3C010000 */  lui        $at, %hi(kRo_EnvSettings_setSnow_5E8)
    /* DD8118 000076AC 8C4A0018 */  lw         $t2, 0x18($v0)
    /* DD811C 000076B0 24420018 */  addiu      $v0, $v0, 0x18
    /* DD8120 000076B4 354B0008 */  ori        $t3, $t2, 0x8
    /* DD8124 000076B8 AC4B0000 */  sw         $t3, 0x0($v0)
    /* DD8128 000076BC 8CC20040 */  lw         $v0, 0x40($a2)
    /* DD812C 000076C0 8C4C0018 */  lw         $t4, 0x18($v0)
    /* DD8130 000076C4 24420018 */  addiu      $v0, $v0, 0x18
    /* DD8134 000076C8 358D0004 */  ori        $t5, $t4, 0x4
    /* DD8138 000076CC AC4D0000 */  sw         $t5, 0x0($v0)
    /* DD813C 000076D0 C42605E8 */  lwc1       $f6, %lo(kRo_EnvSettings_setSnow_5E8)($at)
    /* DD8140 000076D4 8CC20040 */  lw         $v0, 0x40($a2)
    /* DD8144 000076D8 E446004C */  swc1       $f6, 0x4C($v0)
  .L000076DC:
    /* DD8148 000076DC 8CCE0040 */  lw         $t6, 0x40($a2)
    /* DD814C 000076E0 ADC50048 */  sw         $a1, 0x48($t6)
    /* DD8150 000076E4 8FBF0014 */  lw         $ra, 0x14($sp)
  .L000076E8:
    /* DD8154 000076E8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8158 000076EC 03E00008 */  jr         $ra
    /* DD815C 000076F0 00000000 */   nop
endlabel EnvSettings_setSnow
