nonmatching TreeTmpl_create, 0x170

glabel TreeTmpl_create
    /* DD9210 000087A4 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DD9214 000087A8 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD9218 000087AC AFB10020 */  sw         $s1, 0x20($sp)
    /* DD921C 000087B0 AFB0001C */  sw         $s0, 0x1C($sp)
    /* DD9220 000087B4 AFA40050 */  sw         $a0, 0x50($sp)
    /* DD9224 000087B8 8C8F0040 */  lw         $t7, 0x40($a0)
    /* DD9228 000087BC 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DD922C 000087C0 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD9230 000087C4 15E00012 */  bnez       $t7, .L00008810
    /* DD9234 000087C8 3C020000 */   lui       $v0, (0x0 >> 16)
    /* DD9238 000087CC 8F180000 */  lw         $t8, 0x0($t8)
    /* DD923C 000087D0 24420000 */  addiu      $v0, $v0, 0x0
    /* DD9240 000087D4 3C08000A */  lui        $t0, (0xAA002 >> 16)
    /* DD9244 000087D8 37190008 */  ori        $t9, $t8, 0x8
    /* DD9248 000087DC AC390000 */  sw         $t9, 0x0($at)
    /* DD924C 000087E0 8FA90050 */  lw         $t1, 0x50($sp)
    /* DD9250 000087E4 3508A002 */  ori        $t0, $t0, (0xAA002 & 0xFFFF)
    /* DD9254 000087E8 AC480004 */  sw         $t0, 0x4($v0)
    /* DD9258 000087EC AC400000 */  sw         $zero, 0x0($v0)
    /* DD925C 000087F0 3C050000 */  lui        $a1, %hi(sStr_tree_7FF4)
    /* DD9260 000087F4 24A57FF4 */  addiu      $a1, $a1, %lo(sStr_tree_7FF4)
    /* DD9264 000087F8 00002025 */  or         $a0, $zero, $zero
    /* DD9268 000087FC 00003825 */  or         $a3, $zero, $zero
    /* DD926C 00008800 0C000000 */  jal        func_00000000
    /* DD9270 00008804 8D260048 */   lw        $a2, 0x48($t1)
    /* DD9274 00008808 8FAA0050 */  lw         $t2, 0x50($sp)
    /* DD9278 0000880C AD420040 */  sw         $v0, 0x40($t2)
  .L00008810:
    /* DD927C 00008810 0C000000 */  jal        func_00000000
  alabel sStr_DispTrails_8814
    /* DD9280 00008814 240400C8 */   addiu     $a0, $zero, 0xC8
    /* DD9284 00008818 10400012 */  beqz       $v0, .L00008864
    /* DD9288 0000881C 00408825 */   or        $s1, $v0, $zero
  alabel sStr_DispTrails_8820
    /* DD928C 00008820 0C000000 */  jal        func_00000000
    /* DD9290 00008824 00402025 */   or        $a0, $v0, $zero
    /* DD9294 00008828 3C0B0000 */  lui        $t3, %hi(boot_readFloatParm_00007A48 + 0x10)
  alabel sStr_Node_882C
    /* DD9298 0000882C 256B7A58 */  addiu      $t3, $t3, %lo(boot_readFloatParm_00007A48 + 0x10)
    /* DD929C 00008830 2401FF78 */  addiu      $at, $zero, -0x88
  alabel sStr_Node_8834
    /* DD92A0 00008834 AE2B0028 */  sw         $t3, 0x28($s1)
    /* DD92A4 00008838 16210005 */  bne        $s1, $at, .L00008850
  alabel sStr_Node_883C
    /* DD92A8 0000883C 26300088 */   addiu     $s0, $s1, 0x88
    /* DD92AC 00008840 0C000000 */  jal        func_00000000
  alabel sStr_Node_8844
    /* DD92B0 00008844 24040040 */   addiu     $a0, $zero, 0x40
    /* DD92B4 00008848 10400006 */  beqz       $v0, .L00008864
  alabel sStr_AnimationData_884C
    /* DD92B8 0000884C 00408025 */   or        $s0, $v0, $zero
  .L00008850:
    /* DD92BC 00008850 0C000000 */  jal        func_00000000
    /* DD92C0 00008854 02002025 */   or        $a0, $s0, $zero
    /* DD92C4 00008858 3C0C0000 */  lui        $t4, (0x0 >> 16)
  alabel sStr_lighttarget_885C
    /* DD92C8 0000885C 258C0000 */  addiu      $t4, $t4, 0x0
    /* DD92CC 00008860 AE0C0030 */  sw         $t4, 0x30($s0)
  .L00008864:
    /* DD92D0 00008864 3C014120 */  lui        $at, (0x41200000 >> 16)
  alabel sFmt_Global_out_of_range_8868
    /* DD92D4 00008868 44812000 */  mtc1       $at, $f4
    /* DD92D8 0000886C 240D0013 */  addiu      $t5, $zero, 0x13
    /* DD92DC 00008870 02208025 */  or         $s0, $s1, $zero
    /* DD92E0 00008874 E6240070 */  swc1       $f4, 0x70($s1)
    /* DD92E4 00008878 AFAD0044 */  sw         $t5, 0x44($sp)
    /* DD92E8 0000887C 8E220028 */  lw         $v0, 0x28($s1)
  alabel sStr_Trails_8880
    /* DD92EC 00008880 27A50044 */  addiu      $a1, $sp, 0x44
    /* DD92F0 00008884 8C59002C */  lw         $t9, 0x2C($v0)
  alabel sStr_Trails_8888
    /* DD92F4 00008888 844E0028 */  lh         $t6, 0x28($v0)
    /* DD92F8 0000888C 0320F809 */  jalr       $t9
    /* DD92FC 00008890 01D12021 */   addu      $a0, $t6, $s1
    /* DD9300 00008894 8FAF0050 */  lw         $t7, 0x50($sp)
    /* DD9304 00008898 26040010 */  addiu      $a0, $s0, 0x10
    /* DD9308 0000889C 8DF10040 */  lw         $s1, 0x40($t7)
    /* DD930C 000088A0 0C000000 */  jal        func_00000000
    /* DD9310 000088A4 02202825 */   or        $a1, $s1, $zero
    /* DD9314 000088A8 8E380014 */  lw         $t8, 0x14($s1)
    /* DD9318 000088AC 26040088 */  addiu      $a0, $s0, 0x88
    /* DD931C 000088B0 02002825 */  or         $a1, $s0, $zero
    /* DD9320 000088B4 13000003 */  beqz       $t8, .L000088C4
    /* DD9324 000088B8 24060074 */   addiu     $a2, $zero, 0x74
    /* DD9328 000088BC 24080001 */  addiu      $t0, $zero, 0x1
    /* DD932C 000088C0 AE280004 */  sw         $t0, 0x4($s1)
  .L000088C4:
    /* DD9330 000088C4 AE300014 */  sw         $s0, 0x14($s1)
    /* DD9334 000088C8 3C010000 */  lui        $at, %hi(kRo_TreeTmpl_create_5F0)
    /* DD9338 000088CC C42605F0 */  lwc1       $f6, %lo(kRo_TreeTmpl_create_5F0)($at)
    /* DD933C 000088D0 AFA4002C */  sw         $a0, 0x2C($sp)
    /* DD9340 000088D4 3C074316 */  lui        $a3, (0x43160000 >> 16)
    /* DD9344 000088D8 0C000000 */  jal        func_00000000
    /* DD9348 000088DC E7A60010 */   swc1      $f6, 0x10($sp)
    /* DD934C 000088E0 8E090098 */  lw         $t1, 0x98($s0)
    /* DD9350 000088E4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD9354 000088E8 352A0001 */  ori        $t2, $t1, 0x1
    /* DD9358 000088EC AE0A0098 */  sw         $t2, 0x98($s0)
    /* DD935C 000088F0 8FA5002C */  lw         $a1, 0x2C($sp)
    /* DD9360 000088F4 0C000000 */  jal        func_00000000
    /* DD9364 000088F8 8C840000 */   lw        $a0, 0x0($a0)
    /* DD9368 000088FC 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DD936C 00008900 02001025 */  or         $v0, $s0, $zero
    /* DD9370 00008904 8FB0001C */  lw         $s0, 0x1C($sp)
    /* DD9374 00008908 8FB10020 */  lw         $s1, 0x20($sp)
    /* DD9378 0000890C 03E00008 */  jr         $ra
    /* DD937C 00008910 27BD0050 */   addiu     $sp, $sp, 0x50
endlabel TreeTmpl_create
    /* DD9380 00008914 00000000 */  nop
    /* DD9384 00008918 00000000 */  nop
    /* DD9388 0000891C 00000000 */  nop
