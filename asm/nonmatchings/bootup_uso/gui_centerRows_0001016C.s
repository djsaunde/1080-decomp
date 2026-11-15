nonmatching gui_centerRows_0001016C, 0xE0

glabel gui_centerRows_0001016C
    /* DE0BD8 0001016C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE0BDC 00010170 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE0BE0 00010174 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE0BE4 00010178 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE0BE8 0001017C 3C014320 */  lui        $at, (0x43200000 >> 16)
    /* DE0BEC 00010180 44812000 */  mtc1       $at, $f4
    /* DE0BF0 00010184 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE0BF4 00010188 E5C4006C */  swc1       $f4, 0x6C($t6)
    /* DE0BF8 0001018C 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE0BFC 00010190 C5E60074 */  lwc1       $f6, 0x74($t7)
    /* DE0C00 00010194 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* DE0C04 00010198 44814000 */  mtc1       $at, $f8
    /* DE0C08 0001019C 00000000 */  nop
    /* DE0C0C 000101A0 46083283 */  div.s      $f10, $f6, $f8
    /* DE0C10 000101A4 8DF80078 */  lw         $t8, 0x78($t7)
    /* DE0C14 000101A8 44988000 */  mtc1       $t8, $f16
    /* DE0C18 000101AC 00000000 */  nop
    /* DE0C1C 000101B0 468084A0 */  cvt.s.w    $f18, $f16
    /* DE0C20 000101B4 460A9102 */  mul.s      $f4, $f18, $f10
    /* DE0C24 000101B8 3C0142F0 */  lui        $at, (0x42F00000 >> 16)
    /* DE0C28 000101BC 44813000 */  mtc1       $at, $f6
    /* DE0C2C 000101C0 00000000 */  nop
    /* DE0C30 000101C4 46043201 */  sub.s      $f8, $f6, $f4
    /* DE0C34 000101C8 E5E80070 */  swc1       $f8, 0x70($t7)
    /* DE0C38 000101CC 241900FF */  addiu      $t9, $zero, 0xFF
    /* DE0C3C 000101D0 8FA80028 */  lw         $t0, 0x28($sp)
    /* DE0C40 000101D4 AD190034 */  sw         $t9, 0x34($t0)
    /* DE0C44 000101D8 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE0C48 000101DC AD200038 */  sw         $zero, 0x38($t1)
    /* DE0C4C 000101E0 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0C50 000101E4 2610003C */  addiu      $s0, $s0, 0x3C
    /* DE0C54 000101E8 3C010000 */  lui        $at, %hi(func_00000C10)
    /* DE0C58 000101EC C4300C10 */  lwc1       $f16, %lo(func_00000C10)($at)
    /* DE0C5C 000101F0 E6100000 */  swc1       $f16, 0x0($s0)
    /* DE0C60 000101F4 3C010000 */  lui        $at, %hi(func_00000C10 + 0x4)
    /* DE0C64 000101F8 C4320C14 */  lwc1       $f18, %lo(func_00000C10 + 0x4)($at)
    /* DE0C68 000101FC E6120004 */  swc1       $f18, 0x4($s0)
    /* DE0C6C 00010200 3C010000 */  lui        $at, %hi(func_00000C10 + 0x8)
    /* DE0C70 00010204 C42A0C18 */  lwc1       $f10, %lo(func_00000C10 + 0x8)($at)
    /* DE0C74 00010208 E60A0008 */  swc1       $f10, 0x8($s0)
    /* DE0C78 0001020C 44803000 */  mtc1       $zero, $f6
    /* DE0C7C 00010210 00000000 */  nop
    /* DE0C80 00010214 E606000C */  swc1       $f6, 0xC($s0)
    /* DE0C84 00010218 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0C88 0001021C 0C000000 */  jal        func_00000000
    /* DE0C8C 00010220 00000000 */   nop
    /* DE0C90 00010224 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0C94 00010228 0C000000 */  jal        func_00000000
    /* DE0C98 0001022C 00000000 */   nop
    /* DE0C9C 00010230 10000001 */  b          .L00010238
    /* DE0CA0 00010234 00000000 */   nop
  .L00010238:
    /* DE0CA4 00010238 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE0CA8 0001023C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE0CAC 00010240 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE0CB0 00010244 03E00008 */  jr         $ra
    /* DE0CB4 00010248 00000000 */   nop
endlabel gui_centerRows_0001016C
