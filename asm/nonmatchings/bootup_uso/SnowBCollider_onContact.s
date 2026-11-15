nonmatching SnowBCollider_onContact, 0x344

glabel SnowBCollider_onContact
    /* DD9640 00008BD4 27BDFF48 */  addiu      $sp, $sp, -0xB8
    /* DD9644 00008BD8 AFBF002C */  sw         $ra, 0x2C($sp)
    /* DD9648 00008BDC AFB10028 */  sw         $s1, 0x28($sp)
    /* DD964C 00008BE0 AFB00024 */  sw         $s0, 0x24($sp)
    /* DD9650 00008BE4 F7B40018 */  sdc1       $f20, 0x18($sp)
    /* DD9654 00008BE8 AFA700C4 */  sw         $a3, 0xC4($sp)
    /* DD9658 00008BEC 94820174 */  lhu        $v0, 0x174($a0)
    /* DD965C 00008BF0 4485A000 */  mtc1       $a1, $f20
    /* DD9660 00008BF4 00808025 */  or         $s0, $a0, $zero
    /* DD9664 00008BF8 304E01F0 */  andi       $t6, $v0, 0x1F0
    /* DD9668 00008BFC 11C00084 */  beqz       $t6, .L00008E10
    /* DD966C 00008C00 00C08825 */   or        $s1, $a2, $zero
    /* DD9670 00008C04 44802000 */  mtc1       $zero, $f4
    /* DD9674 00008C08 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD9678 00008C0C 4604A03C */  c.lt.s     $f20, $f4
    /* DD967C 00008C10 00000000 */  nop
    /* DD9680 00008C14 450200BB */  bc1fl      .L00008F04
    /* DD9684 00008C18 8FBF002C */   lw        $ra, 0x2C($sp)
    /* DD9688 00008C1C 8DEF0000 */  lw         $t7, 0x0($t7)
    /* DD968C 00008C20 8E05014C */  lw         $a1, 0x14C($s0)
    /* DD9690 00008C24 0C000000 */  jal        func_00000000
    /* DD9694 00008C28 8DE40084 */   lw        $a0, 0x84($t7)
    /* DD9698 00008C2C 38440061 */  xori       $a0, $v0, 0x61
    /* DD969C 00008C30 2C830001 */  sltiu      $v1, $a0, 0x1
    /* DD96A0 00008C34 14600007 */  bnez       $v1, .L00008C54
    /* DD96A4 00008C38 00402825 */   or        $a1, $v0, $zero
    /* DD96A8 00008C3C 38420062 */  xori       $v0, $v0, 0x62
    /* DD96AC 00008C40 2C430001 */  sltiu      $v1, $v0, 0x1
    /* DD96B0 00008C44 14600003 */  bnez       $v1, .L00008C54
    /* DD96B4 00008C48 00000000 */   nop
    /* DD96B8 00008C4C 38A30063 */  xori       $v1, $a1, 0x63
    /* DD96BC 00008C50 2C630001 */  sltiu      $v1, $v1, 0x1
  .L00008C54:
    /* DD96C0 00008C54 546000AB */  bnel       $v1, $zero, .L00008F04
    /* DD96C4 00008C58 8FBF002C */   lw        $ra, 0x2C($sp)
    /* DD96C8 00008C5C 8E030170 */  lw         $v1, 0x170($s0)
    /* DD96CC 00008C60 3C01C1F0 */  lui        $at, (0xC1F00000 >> 16)
    /* DD96D0 00008C64 44810000 */  mtc1       $at, $f0
    /* DD96D4 00008C68 8C780938 */  lw         $t8, 0x938($v1)
    /* DD96D8 00008C6C 3C013FE0 */  lui        $at, (0x3FE00000 >> 16)
    /* DD96DC 00008C70 4600A03C */  c.lt.s     $f20, $f0
    /* DD96E0 00008C74 27190001 */  addiu      $t9, $t8, 0x1
    /* DD96E4 00008C78 AC790938 */  sw         $t9, 0x938($v1)
    /* DD96E8 00008C7C 8E080170 */  lw         $t0, 0x170($s0)
    /* DD96EC 00008C80 44814800 */  mtc1       $at, $f9
    /* DD96F0 00008C84 45000002 */  bc1f       .L00008C90
    /* DD96F4 00008C88 A50509A4 */   sh        $a1, 0x9A4($t0)
    /* DD96F8 00008C8C 46000506 */  mov.s      $f20, $f0
  .L00008C90:
    /* DD96FC 00008C90 4600A407 */  neg.s      $f16, $f20
    /* DD9700 00008C94 44804000 */  mtc1       $zero, $f8
    /* DD9704 00008C98 460081A1 */  cvt.d.s    $f6, $f16
    /* DD9708 00008C9C 8E030170 */  lw         $v1, 0x170($s0)
    /* DD970C 00008CA0 46283282 */  mul.d      $f10, $f6, $f8
    /* DD9710 00008CA4 960A0174 */  lhu        $t2, 0x174($s0)
    /* DD9714 00008CA8 94690A10 */  lhu        $t1, 0xA10($v1)
    /* DD9718 00008CAC 27AC0074 */  addiu      $t4, $sp, 0x74
    /* DD971C 00008CB0 27A70084 */  addiu      $a3, $sp, 0x84
    /* DD9720 00008CB4 012A5825 */  or         $t3, $t1, $t2
    /* DD9724 00008CB8 A46B0A10 */  sh         $t3, 0xA10($v1)
    /* DD9728 00008CBC C6320000 */  lwc1       $f18, 0x0($s1)
    /* DD972C 00008CC0 46205020 */  cvt.s.d    $f0, $f10
    /* DD9730 00008CC4 C6240004 */  lwc1       $f4, 0x4($s1)
    /* DD9734 00008CC8 C6260008 */  lwc1       $f6, 0x8($s1)
    /* DD9738 00008CCC 27AF00A0 */  addiu      $t7, $sp, 0xA0
    /* DD973C 00008CD0 46009082 */  mul.s      $f2, $f18, $f0
    /* DD9740 00008CD4 24630A10 */  addiu      $v1, $v1, 0xA10
    /* DD9744 00008CD8 27A80040 */  addiu      $t0, $sp, 0x40
    /* DD9748 00008CDC 46002302 */  mul.s      $f12, $f4, $f0
    /* DD974C 00008CE0 27A60094 */  addiu      $a2, $sp, 0x94
    /* DD9750 00008CE4 26050120 */  addiu      $a1, $s0, 0x120
    /* DD9754 00008CE8 46003382 */  mul.s      $f14, $f6, $f0
    /* DD9758 00008CEC E7A20074 */  swc1       $f2, 0x74($sp)
    /* DD975C 00008CF0 E7AC0078 */  swc1       $f12, 0x78($sp)
    /* DD9760 00008CF4 E7AE007C */  swc1       $f14, 0x7C($sp)
    /* DD9764 00008CF8 8D8E0000 */  lw         $t6, 0x0($t4)
    /* DD9768 00008CFC ACEE0000 */  sw         $t6, 0x0($a3)
    /* DD976C 00008D00 8D8D0004 */  lw         $t5, 0x4($t4)
    /* DD9770 00008D04 8CF90000 */  lw         $t9, 0x0($a3)
    /* DD9774 00008D08 ACED0004 */  sw         $t5, 0x4($a3)
    /* DD9778 00008D0C 8D8E0008 */  lw         $t6, 0x8($t4)
    /* DD977C 00008D10 ACEE0008 */  sw         $t6, 0x8($a3)
    /* DD9780 00008D14 ADF90000 */  sw         $t9, 0x0($t7)
    /* DD9784 00008D18 8CF80004 */  lw         $t8, 0x4($a3)
    /* DD9788 00008D1C ADF80004 */  sw         $t8, 0x4($t7)
    /* DD978C 00008D20 8CF90008 */  lw         $t9, 0x8($a3)
    /* DD9790 00008D24 ADF90008 */  sw         $t9, 0x8($t7)
    /* DD9794 00008D28 8E040170 */  lw         $a0, 0x170($s0)
    /* DD9798 00008D2C C7AA00A0 */  lwc1       $f10, 0xA0($sp)
    /* DD979C 00008D30 C48800B4 */  lwc1       $f8, 0xB4($a0)
  alabel D_00008D34
    /* DD97A0 00008D34 248400B4 */  addiu      $a0, $a0, 0xB4
    /* DD97A4 00008D38 C4840004 */  lwc1       $f4, 0x4($a0)
    /* DD97A8 00008D3C 460A4480 */  add.s      $f18, $f8, $f10
    /* DD97AC 00008D40 C48A0008 */  lwc1       $f10, 0x8($a0)
    /* DD97B0 00008D44 E4920000 */  swc1       $f18, 0x0($a0)
  alabel D_00008D48
    /* DD97B4 00008D48 C7A600A4 */  lwc1       $f6, 0xA4($sp)
    /* DD97B8 00008D4C 46062200 */  add.s      $f8, $f4, $f6
    /* DD97BC 00008D50 E4880004 */  swc1       $f8, 0x4($a0)
    /* DD97C0 00008D54 C7B200A8 */  lwc1       $f18, 0xA8($sp)
    /* DD97C4 00008D58 46125100 */  add.s      $f4, $f10, $f18
  alabel D_00008D5C
    /* DD97C8 00008D5C E4840008 */  swc1       $f4, 0x8($a0)
    /* DD97CC 00008D60 8E020170 */  lw         $v0, 0x170($s0)
    /* DD97D0 00008D64 C6280000 */  lwc1       $f8, 0x0($s1)
    /* DD97D4 00008D68 C446093C */  lwc1       $f6, 0x93C($v0)
    /* DD97D8 00008D6C C4520940 */  lwc1       $f18, 0x940($v0)
  alabel D_00008D70
    /* DD97DC 00008D70 2442093C */  addiu      $v0, $v0, 0x93C
    /* DD97E0 00008D74 46083280 */  add.s      $f10, $f6, $f8
    /* DD97E4 00008D78 C4480008 */  lwc1       $f8, 0x8($v0)
  alabel D_00008D7C
    /* DD97E8 00008D7C E44A0000 */  swc1       $f10, 0x0($v0)
    /* DD97EC 00008D80 C6240004 */  lwc1       $f4, 0x4($s1)
    /* DD97F0 00008D84 46049180 */  add.s      $f6, $f18, $f4
    /* DD97F4 00008D88 E4460004 */  swc1       $f6, 0x4($v0)
  alabel D_00008D8C
    /* DD97F8 00008D8C C62A0008 */  lwc1       $f10, 0x8($s1)
    /* DD97FC 00008D90 460A4480 */  add.s      $f18, $f8, $f10
    /* DD9800 00008D94 E4520008 */  swc1       $f18, 0x8($v0)
    /* DD9804 00008D98 8E030170 */  lw         $v1, 0x170($s0)
  alabel D_00008D9C
    /* DD9808 00008D9C C4640968 */  lwc1       $f4, 0x968($v1)
    /* DD980C 00008DA0 24630968 */  addiu      $v1, $v1, 0x968
    /* DD9810 00008DA4 46142180 */  add.s      $f6, $f4, $f20
    /* DD9814 00008DA8 E4660000 */  swc1       $f6, 0x0($v1)
    /* DD9818 00008DAC C6280000 */  lwc1       $f8, 0x0($s1)
    /* DD981C 00008DB0 C62A0004 */  lwc1       $f10, 0x4($s1)
    /* DD9820 00008DB4 C6320008 */  lwc1       $f18, 0x8($s1)
    /* DD9824 00008DB8 46104082 */  mul.s      $f2, $f8, $f16
    /* DD9828 00008DBC 00000000 */  nop
    /* DD982C 00008DC0 46105302 */  mul.s      $f12, $f10, $f16
    /* DD9830 00008DC4 00000000 */  nop
    /* DD9834 00008DC8 46109382 */  mul.s      $f14, $f18, $f16
    /* DD9838 00008DCC E7A20040 */  swc1       $f2, 0x40($sp)
    /* DD983C 00008DD0 E7AC0044 */  swc1       $f12, 0x44($sp)
    /* DD9840 00008DD4 E7AE0048 */  swc1       $f14, 0x48($sp)
    /* DD9844 00008DD8 8D0A0000 */  lw         $t2, 0x0($t0)
    /* DD9848 00008DDC ACEA0000 */  sw         $t2, 0x0($a3)
    /* DD984C 00008DE0 8D090004 */  lw         $t1, 0x4($t0)
    /* DD9850 00008DE4 8CEC0000 */  lw         $t4, 0x0($a3)
    /* DD9854 00008DE8 ACE90004 */  sw         $t1, 0x4($a3)
    /* DD9858 00008DEC 8D0A0008 */  lw         $t2, 0x8($t0)
    /* DD985C 00008DF0 ACC90004 */  sw         $t1, 0x4($a2)
    /* DD9860 00008DF4 ACCC0000 */  sw         $t4, 0x0($a2)
    /* DD9864 00008DF8 ACEA0008 */  sw         $t2, 0x8($a3)
    /* DD9868 00008DFC ACCA0008 */  sw         $t2, 0x8($a2)
    /* DD986C 00008E00 0C000000 */  jal        func_00000000
    /* DD9870 00008E04 8E040170 */   lw        $a0, 0x170($s0)
    /* DD9874 00008E08 1000003E */  b          .L00008F04
    /* DD9878 00008E0C 8FBF002C */   lw        $ra, 0x2C($sp)
  .L00008E10:
    /* DD987C 00008E10 304D000F */  andi       $t5, $v0, 0xF
    /* DD9880 00008E14 11A0003A */  beqz       $t5, .L00008F00
    /* DD9884 00008E18 3C014248 */   lui       $at, (0x42480000 >> 16)
    /* DD9888 00008E1C 44812000 */  mtc1       $at, $f4
    /* DD988C 00008E20 00000000 */  nop
    /* DD9890 00008E24 4604A03C */  c.lt.s     $f20, $f4
    /* DD9894 00008E28 00000000 */  nop
    /* DD9898 00008E2C 45020035 */  bc1fl      .L00008F04
    /* DD989C 00008E30 8FBF002C */   lw        $ra, 0x2C($sp)
    /* DD98A0 00008E34 8E030170 */  lw         $v1, 0x170($s0)
    /* DD98A4 00008E38 8C6E0938 */  lw         $t6, 0x938($v1)
    /* DD98A8 00008E3C 25CF0001 */  addiu      $t7, $t6, 0x1
    /* DD98AC 00008E40 AC6F0938 */  sw         $t7, 0x938($v1)
    /* DD98B0 00008E44 8E030170 */  lw         $v1, 0x170($s0)
    /* DD98B4 00008E48 96190174 */  lhu        $t9, 0x174($s0)
    /* DD98B8 00008E4C 94780A10 */  lhu        $t8, 0xA10($v1)
    /* DD98BC 00008E50 24630A10 */  addiu      $v1, $v1, 0xA10
    /* DD98C0 00008E54 03194025 */  or         $t0, $t8, $t9
    /* DD98C4 00008E58 A4680000 */  sh         $t0, 0x0($v1)
    /* DD98C8 00008E5C 8E030170 */  lw         $v1, 0x170($s0)
    /* DD98CC 00008E60 C4660968 */  lwc1       $f6, 0x968($v1)
    /* DD98D0 00008E64 24630968 */  addiu      $v1, $v1, 0x968
    /* DD98D4 00008E68 46143200 */  add.s      $f8, $f6, $f20
    /* DD98D8 00008E6C E4680000 */  swc1       $f8, 0x0($v1)
    /* DD98DC 00008E70 8E020170 */  lw         $v0, 0x170($s0)
    /* DD98E0 00008E74 C6320000 */  lwc1       $f18, 0x0($s1)
    /* DD98E4 00008E78 C44A093C */  lwc1       $f10, 0x93C($v0)
    /* DD98E8 00008E7C C4460940 */  lwc1       $f6, 0x940($v0)
    /* DD98EC 00008E80 2442093C */  addiu      $v0, $v0, 0x93C
    /* DD98F0 00008E84 46125100 */  add.s      $f4, $f10, $f18
    /* DD98F4 00008E88 C4520008 */  lwc1       $f18, 0x8($v0)
    /* DD98F8 00008E8C E4440000 */  swc1       $f4, 0x0($v0)
    /* DD98FC 00008E90 C6280004 */  lwc1       $f8, 0x4($s1)
    /* DD9900 00008E94 46083280 */  add.s      $f10, $f6, $f8
    /* DD9904 00008E98 44804000 */  mtc1       $zero, $f8
    /* DD9908 00008E9C E44A0004 */  swc1       $f10, 0x4($v0)
    /* DD990C 00008EA0 C6240008 */  lwc1       $f4, 0x8($s1)
    /* DD9910 00008EA4 46049180 */  add.s      $f6, $f18, $f4
    /* DD9914 00008EA8 E4460008 */  swc1       $f6, 0x8($v0)
    /* DD9918 00008EAC 8E040170 */  lw         $a0, 0x170($s0)
    /* DD991C 00008EB0 C4820970 */  lwc1       $f2, 0x970($a0)
    /* DD9920 00008EB4 4608103C */  c.lt.s     $f2, $f8
    /* DD9924 00008EB8 00000000 */  nop
    /* DD9928 00008EBC 45020004 */  bc1fl      .L00008ED0
    /* DD992C 00008EC0 46001006 */   mov.s     $f0, $f2
    /* DD9930 00008EC4 10000002 */  b          .L00008ED0
    /* DD9934 00008EC8 46001007 */   neg.s     $f0, $f2
    /* DD9938 00008ECC 46001006 */  mov.s      $f0, $f2
  .L00008ED0:
    /* DD993C 00008ED0 46000282 */  mul.s      $f10, $f0, $f0
    /* DD9940 00008ED4 26050120 */  addiu      $a1, $s0, 0x120
    /* DD9944 00008ED8 26060114 */  addiu      $a2, $s0, 0x114
    /* DD9948 00008EDC 02203825 */  or         $a3, $s1, $zero
    /* DD994C 00008EE0 0C000000 */  jal        func_00000000
    /* DD9950 00008EE4 E7AA0010 */   swc1      $f10, 0x10($sp)
    /* DD9954 00008EE8 4405A000 */  mfc1       $a1, $f20
    /* DD9958 00008EEC 3C073DCC */  lui        $a3, (0x3DCCCCCD >> 16)
    /* DD995C 00008EF0 34E7CCCD */  ori        $a3, $a3, (0x3DCCCCCD & 0xFFFF)
    /* DD9960 00008EF4 02002025 */  or         $a0, $s0, $zero
    /* DD9964 00008EF8 0C000000 */  jal        func_00000000
    /* DD9968 00008EFC 02203025 */   or        $a2, $s1, $zero
  .L00008F00:
    /* DD996C 00008F00 8FBF002C */  lw         $ra, 0x2C($sp)
  .L00008F04:
    /* DD9970 00008F04 D7B40018 */  ldc1       $f20, 0x18($sp)
    /* DD9974 00008F08 8FB00024 */  lw         $s0, 0x24($sp)
    /* DD9978 00008F0C 8FB10028 */  lw         $s1, 0x28($sp)
    /* DD997C 00008F10 03E00008 */  jr         $ra
    /* DD9980 00008F14 27BD00B8 */   addiu     $sp, $sp, 0xB8
endlabel SnowBCollider_onContact
