nonmatching TimeGadget_display_000140C4, 0x164

glabel TimeGadget_display_000140C4
    /* DE4B30 000140C4 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DE4B34 000140C8 44810000 */  mtc1       $at, $f0
    /* DE4B38 000140CC 27BDFF98 */  addiu      $sp, $sp, -0x68
    /* DE4B3C 000140D0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE4B40 000140D4 00808025 */  or         $s0, $a0, $zero
    /* DE4B44 000140D8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE4B48 000140DC E7A00058 */  swc1       $f0, 0x58($sp)
    /* DE4B4C 000140E0 E7A0005C */  swc1       $f0, 0x5C($sp)
    /* DE4B50 000140E4 E7A00060 */  swc1       $f0, 0x60($sp)
    /* DE4B54 000140E8 E7A00064 */  swc1       $f0, 0x64($sp)
    /* DE4B58 000140EC 8C840104 */  lw         $a0, 0x104($a0)
    /* DE4B5C 000140F0 5080001A */  beql       $a0, $zero, .L0001415C
    /* DE4B60 000140F4 8E040108 */   lw        $a0, 0x108($s0)
    /* DE4B64 000140F8 0C000000 */  jal        func_00000000
    /* DE4B68 000140FC 00000000 */   nop
    /* DE4B6C 00014100 8E0E00DC */  lw         $t6, 0xDC($s0)
    /* DE4B70 00014104 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE4B74 00014108 C4280000 */  lwc1       $f8, 0x0($at)
    /* DE4B78 0001410C 448E2000 */  mtc1       $t6, $f4
    /* DE4B7C 00014110 8E040104 */  lw         $a0, 0x104($s0)
    /* DE4B80 00014114 27A60058 */  addiu      $a2, $sp, 0x58
    /* DE4B84 00014118 468021A0 */  cvt.s.w    $f6, $f4
    /* DE4B88 0001411C 00003825 */  or         $a3, $zero, $zero
    /* DE4B8C 00014120 46083282 */  mul.s      $f10, $f6, $f8
    /* DE4B90 00014124 4600540D */  trunc.w.s  $f16, $f10
    /* DE4B94 00014128 44058000 */  mfc1       $a1, $f16
    /* DE4B98 0001412C 0C000000 */  jal        func_00000000
    /* DE4B9C 00014130 00000000 */   nop
    /* DE4BA0 00014134 8E180044 */  lw         $t8, 0x44($s0)
    /* DE4BA4 00014138 8E190074 */  lw         $t9, 0x74($s0)
    /* DE4BA8 0001413C 8E08005C */  lw         $t0, 0x5C($s0)
    /* DE4BAC 00014140 8E09008C */  lw         $t1, 0x8C($s0)
    /* DE4BB0 00014144 8E040104 */  lw         $a0, 0x104($s0)
    /* DE4BB4 00014148 00003825 */  or         $a3, $zero, $zero
    /* DE4BB8 0001414C 03192821 */  addu       $a1, $t8, $t9
    /* DE4BBC 00014150 0C000000 */  jal        func_00000000
    /* DE4BC0 00014154 01093021 */   addu      $a2, $t0, $t1
    /* DE4BC4 00014158 8E040108 */  lw         $a0, 0x108($s0)
  .L0001415C:
    /* DE4BC8 0001415C 10800019 */  beqz       $a0, .L000141C4
    /* DE4BCC 00014160 00000000 */   nop
    /* DE4BD0 00014164 0C000000 */  jal        func_00000000
    /* DE4BD4 00014168 00000000 */   nop
    /* DE4BD8 0001416C 8E0A00D8 */  lw         $t2, 0xD8($s0)
    /* DE4BDC 00014170 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE4BE0 00014174 C4260000 */  lwc1       $f6, 0x0($at)
    /* DE4BE4 00014178 448A9000 */  mtc1       $t2, $f18
    /* DE4BE8 0001417C 8E040108 */  lw         $a0, 0x108($s0)
    /* DE4BEC 00014180 27A60058 */  addiu      $a2, $sp, 0x58
    /* DE4BF0 00014184 46809120 */  cvt.s.w    $f4, $f18
    /* DE4BF4 00014188 00003825 */  or         $a3, $zero, $zero
    /* DE4BF8 0001418C 46062202 */  mul.s      $f8, $f4, $f6
    /* DE4BFC 00014190 4600428D */  trunc.w.s  $f10, $f8
    /* DE4C00 00014194 44055000 */  mfc1       $a1, $f10
    /* DE4C04 00014198 0C000000 */  jal        func_00000000
    /* DE4C08 0001419C 00000000 */   nop
    /* DE4C0C 000141A0 8E0C0044 */  lw         $t4, 0x44($s0)
    /* DE4C10 000141A4 8E0D00A4 */  lw         $t5, 0xA4($s0)
    /* DE4C14 000141A8 8E0E005C */  lw         $t6, 0x5C($s0)
    /* DE4C18 000141AC 8E0F00BC */  lw         $t7, 0xBC($s0)
    /* DE4C1C 000141B0 8E040108 */  lw         $a0, 0x108($s0)
    /* DE4C20 000141B4 00003825 */  or         $a3, $zero, $zero
    /* DE4C24 000141B8 018D2821 */  addu       $a1, $t4, $t5
    /* DE4C28 000141BC 0C000000 */  jal        func_00000000
    /* DE4C2C 000141C0 01CF3021 */   addu      $a2, $t6, $t7
  .L000141C4:
    /* DE4C30 000141C4 0C000000 */  jal        func_00000000
    /* DE4C34 000141C8 8E0400E0 */   lw        $a0, 0xE0($s0)
    /* DE4C38 000141CC 8E1800D8 */  lw         $t8, 0xD8($s0)
    /* DE4C3C 000141D0 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE4C40 000141D4 C4240000 */  lwc1       $f4, 0x0($at)
    /* DE4C44 000141D8 44988000 */  mtc1       $t8, $f16
    /* DE4C48 000141DC 8E0400E0 */  lw         $a0, 0xE0($s0)
    /* DE4C4C 000141E0 260600C4 */  addiu      $a2, $s0, 0xC4
    /* DE4C50 000141E4 468084A0 */  cvt.s.w    $f18, $f16
    /* DE4C54 000141E8 8E0700D4 */  lw         $a3, 0xD4($s0)
    /* DE4C58 000141EC 46049182 */  mul.s      $f6, $f18, $f4
    /* DE4C5C 000141F0 4600320D */  trunc.w.s  $f8, $f6
    /* DE4C60 000141F4 44054000 */  mfc1       $a1, $f8
    /* DE4C64 000141F8 0C000000 */  jal        func_00000000
    /* DE4C68 000141FC 00000000 */   nop
    /* DE4C6C 00014200 8E0400E0 */  lw         $a0, 0xE0($s0)
    /* DE4C70 00014204 8E050044 */  lw         $a1, 0x44($s0)
    /* DE4C74 00014208 8E06005C */  lw         $a2, 0x5C($s0)
    /* DE4C78 0001420C 0C000000 */  jal        func_00000000
    /* DE4C7C 00014210 260700E4 */   addiu     $a3, $s0, 0xE4
    /* DE4C80 00014214 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE4C84 00014218 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE4C88 0001421C 27BD0068 */  addiu      $sp, $sp, 0x68
    /* DE4C8C 00014220 03E00008 */  jr         $ra
    /* DE4C90 00014224 00000000 */   nop
endlabel TimeGadget_display_000140C4
