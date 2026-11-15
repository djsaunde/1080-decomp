nonmatching game_setupReplay, 0x1C4

glabel game_setupReplay
    /* DD45E4 00003B78 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* DD45E8 00003B7C AFB10030 */  sw         $s1, 0x30($sp)
    /* DD45EC 00003B80 00808825 */  or         $s1, $a0, $zero
    /* DD45F0 00003B84 AFBF0034 */  sw         $ra, 0x34($sp)
    /* DD45F4 00003B88 AFB0002C */  sw         $s0, 0x2C($sp)
    /* DD45F8 00003B8C AFA50064 */  sw         $a1, 0x64($sp)
    /* DD45FC 00003B90 0C000000 */  jal        func_00000000
    /* DD4600 00003B94 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4604 00003B98 10400004 */  beqz       $v0, .L00003BAC
    /* DD4608 00003B9C 00408025 */   or        $s0, $v0, $zero
    /* DD460C 00003BA0 00402025 */  or         $a0, $v0, $zero
    /* DD4610 00003BA4 0C000000 */  jal        func_00000000
    /* DD4614 00003BA8 00002825 */   or        $a1, $zero, $zero
  .L00003BAC:
    /* DD4618 00003BAC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD461C 00003BB0 24840000 */  addiu      $a0, $a0, 0x0
    /* DD4620 00003BB4 0C000000 */  jal        func_00000000
    /* DD4624 00003BB8 02002825 */   or        $a1, $s0, $zero
    /* DD4628 00003BBC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD462C 00003BC0 24840000 */  addiu      $a0, $a0, 0x0
    /* DD4630 00003BC4 02002825 */  or         $a1, $s0, $zero
    /* DD4634 00003BC8 0C000000 */  jal        func_00000000
    /* DD4638 00003BCC 00003025 */   or        $a2, $zero, $zero
    /* DD463C 00003BD0 AFB00010 */  sw         $s0, 0x10($sp)
    /* DD4640 00003BD4 8E2E0080 */  lw         $t6, 0x80($s1)
    /* DD4644 00003BD8 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD4648 00003BDC 24630000 */  addiu      $v1, $v1, 0x0
    /* DD464C 00003BE0 AFAE0014 */  sw         $t6, 0x14($sp)
    /* DD4650 00003BE4 8E220098 */  lw         $v0, 0x98($s1)
    /* DD4654 00003BE8 90780182 */  lbu        $t8, 0x182($v1)
    /* DD4658 00003BEC 3C010001 */  lui        $at, (0x10000 >> 16)
    /* DD465C 00003BF0 C44400C4 */  lwc1       $f4, 0xC4($v0)
    /* DD4660 00003BF4 37198001 */  ori        $t9, $t8, 0x8001
    /* DD4664 00003BF8 03214025 */  or         $t0, $t9, $at
    /* DD4668 00003BFC E7A40018 */  swc1       $f4, 0x18($sp)
    /* DD466C 00003C00 C44600CC */  lwc1       $f6, 0xCC($v0)
    /* DD4670 00003C04 3C010004 */  lui        $at, (0x40000 >> 16)
    /* DD4674 00003C08 01014825 */  or         $t1, $t0, $at
    /* DD4678 00003C0C 240A001B */  addiu      $t2, $zero, 0x1B
    /* DD467C 00003C10 AFAA0024 */  sw         $t2, 0x24($sp)
    /* DD4680 00003C14 AFA90020 */  sw         $t1, 0x20($sp)
    /* DD4684 00003C18 90660181 */  lbu        $a2, 0x181($v1)
    /* DD4688 00003C1C 90670183 */  lbu        $a3, 0x183($v1)
    /* DD468C 00003C20 02202025 */  or         $a0, $s1, $zero
    /* DD4690 00003C24 00002825 */  or         $a1, $zero, $zero
    /* DD4694 00003C28 0C000000 */  jal        func_00000000
    /* DD4698 00003C2C E7A6001C */   swc1      $f6, 0x1C($sp)
    /* DD469C 00003C30 AFA20058 */  sw         $v0, 0x58($sp)
    /* DD46A0 00003C34 8E260080 */  lw         $a2, 0x80($s1)
    /* DD46A4 00003C38 AFB00010 */  sw         $s0, 0x10($sp)
    /* DD46A8 00003C3C 02202025 */  or         $a0, $s1, $zero
    /* DD46AC 00003C40 00002825 */  or         $a1, $zero, $zero
    /* DD46B0 00003C44 0C000000 */  jal        func_00000000
    /* DD46B4 00003C48 00403825 */   or        $a3, $v0, $zero
    /* DD46B8 00003C4C AFA20054 */  sw         $v0, 0x54($sp)
    /* DD46BC 00003C50 8E260080 */  lw         $a2, 0x80($s1)
    /* DD46C0 00003C54 AFB00010 */  sw         $s0, 0x10($sp)
    /* DD46C4 00003C58 02202025 */  or         $a0, $s1, $zero
    /* DD46C8 00003C5C 24050001 */  addiu      $a1, $zero, 0x1
    /* DD46CC 00003C60 0C000000 */  jal        func_00000000
    /* DD46D0 00003C64 8FA70058 */   lw        $a3, 0x58($sp)
    /* DD46D4 00003C68 8FAB0064 */  lw         $t3, 0x64($sp)
    /* DD46D8 00003C6C 3C0D0000 */  lui        $t5, (0x0 >> 16)
    /* DD46DC 00003C70 25AD0000 */  addiu      $t5, $t5, 0x0
    /* DD46E0 00003C74 000B60C0 */  sll        $t4, $t3, 3
    /* DD46E4 00003C78 018B6023 */  subu       $t4, $t4, $t3
    /* DD46E8 00003C7C 000C6080 */  sll        $t4, $t4, 2
    /* DD46EC 00003C80 AFA20050 */  sw         $v0, 0x50($sp)
    /* DD46F0 00003C84 018D2821 */  addu       $a1, $t4, $t5
    /* DD46F4 00003C88 02202025 */  or         $a0, $s1, $zero
    /* DD46F8 00003C8C 0C000000 */  jal        func_00000000
    /* DD46FC 00003C90 8FA60054 */   lw        $a2, 0x54($sp)
    /* DD4700 00003C94 3C0142AA */  lui        $at, (0x42AA0000 >> 16)
    /* DD4704 00003C98 44814000 */  mtc1       $at, $f8
    /* DD4708 00003C9C 8FAE0054 */  lw         $t6, 0x54($sp)
    /* DD470C 00003CA0 AFA2004C */  sw         $v0, 0x4C($sp)
    /* DD4710 00003CA4 24040080 */  addiu      $a0, $zero, 0x80
    /* DD4714 00003CA8 0C000000 */  jal        func_00000000
    /* DD4718 00003CAC E5C8014C */   swc1      $f8, 0x14C($t6)
    /* DD471C 00003CB0 10400004 */  beqz       $v0, .L00003CC4
    /* DD4720 00003CB4 00408025 */   or        $s0, $v0, $zero
    /* DD4724 00003CB8 00402025 */  or         $a0, $v0, $zero
    /* DD4728 00003CBC 0C000000 */  jal        func_00000000
    /* DD472C 00003CC0 24050001 */   addiu     $a1, $zero, 0x1
  .L00003CC4:
    /* DD4730 00003CC4 8FAF0054 */  lw         $t7, 0x54($sp)
    /* DD4734 00003CC8 8FB80050 */  lw         $t8, 0x50($sp)
    /* DD4738 00003CCC 00002025 */  or         $a0, $zero, $zero
    /* DD473C 00003CD0 02002825 */  or         $a1, $s0, $zero
    /* DD4740 00003CD4 8FA60058 */  lw         $a2, 0x58($sp)
    /* DD4744 00003CD8 8FA7004C */  lw         $a3, 0x4C($sp)
    /* DD4748 00003CDC AFAF0010 */  sw         $t7, 0x10($sp)
    /* DD474C 00003CE0 0C000000 */  jal        func_00000000
    /* DD4750 00003CE4 AFB80014 */   sw        $t8, 0x14($sp)
    /* DD4754 00003CE8 8E230084 */  lw         $v1, 0x84($s1)
    /* DD4758 00003CEC 00408025 */  or         $s0, $v0, $zero
    /* DD475C 00003CF0 00402825 */  or         $a1, $v0, $zero
    /* DD4760 00003CF4 24640010 */  addiu      $a0, $v1, 0x10
    /* DD4764 00003CF8 0C000000 */  jal        func_00000000
    /* DD4768 00003CFC AFA30038 */   sw        $v1, 0x38($sp)
    /* DD476C 00003D00 8E190014 */  lw         $t9, 0x14($s0)
    /* DD4770 00003D04 8FA30038 */  lw         $v1, 0x38($sp)
    /* DD4774 00003D08 24080001 */  addiu      $t0, $zero, 0x1
    /* DD4778 00003D0C 53200003 */  beql       $t9, $zero, .L00003D1C
    /* DD477C 00003D10 AE030014 */   sw        $v1, 0x14($s0)
    /* DD4780 00003D14 AE080004 */  sw         $t0, 0x4($s0)
    /* DD4784 00003D18 AE030014 */  sw         $v1, 0x14($s0)
  .L00003D1C:
    /* DD4788 00003D1C 8FAA0058 */  lw         $t2, 0x58($sp)
    /* DD478C 00003D20 8FA9004C */  lw         $t1, 0x4C($sp)
    /* DD4790 00003D24 AD4908DC */  sw         $t1, 0x8DC($t2)
    /* DD4794 00003D28 8FBF0034 */  lw         $ra, 0x34($sp)
    /* DD4798 00003D2C 8FB10030 */  lw         $s1, 0x30($sp)
    /* DD479C 00003D30 8FB0002C */  lw         $s0, 0x2C($sp)
    /* DD47A0 00003D34 03E00008 */  jr         $ra
    /* DD47A4 00003D38 27BD0060 */   addiu     $sp, $sp, 0x60
endlabel game_setupReplay
