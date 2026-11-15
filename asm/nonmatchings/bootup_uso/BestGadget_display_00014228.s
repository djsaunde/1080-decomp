nonmatching BestGadget_display_00014228, 0x164

glabel BestGadget_display_00014228
    /* DE4C94 00014228 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DE4C98 0001422C 44810000 */  mtc1       $at, $f0
    /* DE4C9C 00014230 27BDFF98 */  addiu      $sp, $sp, -0x68
    /* DE4CA0 00014234 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE4CA4 00014238 00808025 */  or         $s0, $a0, $zero
    /* DE4CA8 0001423C AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE4CAC 00014240 E7A00058 */  swc1       $f0, 0x58($sp)
    /* DE4CB0 00014244 E7A0005C */  swc1       $f0, 0x5C($sp)
    /* DE4CB4 00014248 E7A00060 */  swc1       $f0, 0x60($sp)
    /* DE4CB8 0001424C E7A00064 */  swc1       $f0, 0x64($sp)
    /* DE4CBC 00014250 8C840104 */  lw         $a0, 0x104($a0)
    /* DE4CC0 00014254 5080001A */  beql       $a0, $zero, .L000142C0
    /* DE4CC4 00014258 8E040108 */   lw        $a0, 0x108($s0)
    /* DE4CC8 0001425C 0C000000 */  jal        func_00000000
    /* DE4CCC 00014260 00000000 */   nop
    /* DE4CD0 00014264 8E0E00DC */  lw         $t6, 0xDC($s0)
    /* DE4CD4 00014268 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE4CD8 0001426C C4280000 */  lwc1       $f8, 0x0($at)
    /* DE4CDC 00014270 448E2000 */  mtc1       $t6, $f4
    /* DE4CE0 00014274 8E040104 */  lw         $a0, 0x104($s0)
    /* DE4CE4 00014278 27A60058 */  addiu      $a2, $sp, 0x58
    /* DE4CE8 0001427C 468021A0 */  cvt.s.w    $f6, $f4
    /* DE4CEC 00014280 00003825 */  or         $a3, $zero, $zero
    /* DE4CF0 00014284 46083282 */  mul.s      $f10, $f6, $f8
    /* DE4CF4 00014288 4600540D */  trunc.w.s  $f16, $f10
    /* DE4CF8 0001428C 44058000 */  mfc1       $a1, $f16
    /* DE4CFC 00014290 0C000000 */  jal        func_00000000
    /* DE4D00 00014294 00000000 */   nop
    /* DE4D04 00014298 8E180044 */  lw         $t8, 0x44($s0)
    /* DE4D08 0001429C 8E190074 */  lw         $t9, 0x74($s0)
    /* DE4D0C 000142A0 8E08005C */  lw         $t0, 0x5C($s0)
    /* DE4D10 000142A4 8E09008C */  lw         $t1, 0x8C($s0)
    /* DE4D14 000142A8 8E040104 */  lw         $a0, 0x104($s0)
    /* DE4D18 000142AC 00003825 */  or         $a3, $zero, $zero
    /* DE4D1C 000142B0 03192821 */  addu       $a1, $t8, $t9
    /* DE4D20 000142B4 0C000000 */  jal        func_00000000
    /* DE4D24 000142B8 01093021 */   addu      $a2, $t0, $t1
    /* DE4D28 000142BC 8E040108 */  lw         $a0, 0x108($s0)
  .L000142C0:
    /* DE4D2C 000142C0 10800019 */  beqz       $a0, .L00014328
    /* DE4D30 000142C4 00000000 */   nop
    /* DE4D34 000142C8 0C000000 */  jal        func_00000000
    /* DE4D38 000142CC 00000000 */   nop
    /* DE4D3C 000142D0 8E0A00D8 */  lw         $t2, 0xD8($s0)
    /* DE4D40 000142D4 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE4D44 000142D8 C4260000 */  lwc1       $f6, 0x0($at)
    /* DE4D48 000142DC 448A9000 */  mtc1       $t2, $f18
    /* DE4D4C 000142E0 8E040108 */  lw         $a0, 0x108($s0)
    /* DE4D50 000142E4 27A60058 */  addiu      $a2, $sp, 0x58
    /* DE4D54 000142E8 46809120 */  cvt.s.w    $f4, $f18
    /* DE4D58 000142EC 00003825 */  or         $a3, $zero, $zero
    /* DE4D5C 000142F0 46062202 */  mul.s      $f8, $f4, $f6
    /* DE4D60 000142F4 4600428D */  trunc.w.s  $f10, $f8
    /* DE4D64 000142F8 44055000 */  mfc1       $a1, $f10
    /* DE4D68 000142FC 0C000000 */  jal        func_00000000
    /* DE4D6C 00014300 00000000 */   nop
    /* DE4D70 00014304 8E0C0044 */  lw         $t4, 0x44($s0)
    /* DE4D74 00014308 8E0D00A4 */  lw         $t5, 0xA4($s0)
    /* DE4D78 0001430C 8E0E005C */  lw         $t6, 0x5C($s0)
    /* DE4D7C 00014310 8E0F00BC */  lw         $t7, 0xBC($s0)
    /* DE4D80 00014314 8E040108 */  lw         $a0, 0x108($s0)
    /* DE4D84 00014318 00003825 */  or         $a3, $zero, $zero
    /* DE4D88 0001431C 018D2821 */  addu       $a1, $t4, $t5
    /* DE4D8C 00014320 0C000000 */  jal        func_00000000
    /* DE4D90 00014324 01CF3021 */   addu      $a2, $t6, $t7
  .L00014328:
    /* DE4D94 00014328 0C000000 */  jal        func_00000000
    /* DE4D98 0001432C 8E0400E0 */   lw        $a0, 0xE0($s0)
    /* DE4D9C 00014330 8E1800D8 */  lw         $t8, 0xD8($s0)
    /* DE4DA0 00014334 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE4DA4 00014338 C4240000 */  lwc1       $f4, 0x0($at)
    /* DE4DA8 0001433C 44988000 */  mtc1       $t8, $f16
    /* DE4DAC 00014340 8E0400E0 */  lw         $a0, 0xE0($s0)
    /* DE4DB0 00014344 260600C4 */  addiu      $a2, $s0, 0xC4
    /* DE4DB4 00014348 468084A0 */  cvt.s.w    $f18, $f16
    /* DE4DB8 0001434C 8E0700D4 */  lw         $a3, 0xD4($s0)
    /* DE4DBC 00014350 46049182 */  mul.s      $f6, $f18, $f4
    /* DE4DC0 00014354 4600320D */  trunc.w.s  $f8, $f6
    /* DE4DC4 00014358 44054000 */  mfc1       $a1, $f8
    /* DE4DC8 0001435C 0C000000 */  jal        func_00000000
    /* DE4DCC 00014360 00000000 */   nop
    /* DE4DD0 00014364 8E0400E0 */  lw         $a0, 0xE0($s0)
    /* DE4DD4 00014368 8E050044 */  lw         $a1, 0x44($s0)
    /* DE4DD8 0001436C 8E06005C */  lw         $a2, 0x5C($s0)
    /* DE4DDC 00014370 0C000000 */  jal        func_00000000
    /* DE4DE0 00014374 260700E4 */   addiu     $a3, $s0, 0xE4
    /* DE4DE4 00014378 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE4DE8 0001437C 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE4DEC 00014380 27BD0068 */  addiu      $sp, $sp, 0x68
    /* DE4DF0 00014384 03E00008 */  jr         $ra
    /* DE4DF4 00014388 00000000 */   nop
endlabel BestGadget_display_00014228
