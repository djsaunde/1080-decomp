nonmatching item_registerTemplates, 0x958

glabel item_registerTemplates
    /* DD600C 000055A0 27BDFE80 */  addiu      $sp, $sp, -0x180
    /* DD6010 000055A4 44800000 */  mtc1       $zero, $f0
    /* DD6014 000055A8 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD6018 000055AC 8DCE0000 */  lw         $t6, 0x0($t6)
    /* DD601C 000055B0 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD6020 000055B4 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD6024 000055B8 35CF0008 */  ori        $t7, $t6, 0x8
    /* DD6028 000055BC AC2F0000 */  sw         $t7, 0x0($at)
    /* DD602C 000055C0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD6030 000055C4 44816000 */  mtc1       $at, $f12
    /* DD6034 000055C8 3C013F00 */  lui        $at, (0x3F000000 >> 16)
    /* DD6038 000055CC 44811000 */  mtc1       $at, $f2
    /* DD603C 000055D0 25080000 */  addiu      $t0, $t0, 0x0
    /* DD6040 000055D4 3C18000A */  lui        $t8, (0xAA002 >> 16)
    /* DD6044 000055D8 3718A002 */  ori        $t8, $t8, (0xAA002 & 0xFFFF)
    /* DD6048 000055DC AD180004 */  sw         $t8, 0x4($t0)
    /* DD604C 000055E0 AD000000 */  sw         $zero, 0x0($t0)
    /* DD6050 000055E4 3C010000 */  lui        $at, %hi(snd_soundObjCreate_0000057C + 0x1C)
    /* DD6054 000055E8 C4240598 */  lwc1       $f4, %lo(snd_soundObjCreate_0000057C + 0x1C)($at)
    /* DD6058 000055EC 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD605C 000055F0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD6060 000055F4 AC200000 */  sw         $zero, 0x0($at)
    /* DD6064 000055F8 34D00010 */  ori        $s0, $a2, 0x10
    /* DD6068 000055FC 24010001 */  addiu      $at, $zero, 0x1
    /* DD606C 00005600 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD6070 00005604 AFA40180 */  sw         $a0, 0x180($sp)
    /* DD6074 00005608 AFA50184 */  sw         $a1, 0x184($sp)
    /* DD6078 0000560C AFB00024 */  sw         $s0, 0x24($sp)
    /* DD607C 00005610 E7A00154 */  swc1       $f0, 0x154($sp)
    /* DD6080 00005614 E7A00158 */  swc1       $f0, 0x158($sp)
    /* DD6084 00005618 E7A0015C */  swc1       $f0, 0x15C($sp)
    /* DD6088 0000561C E7A00160 */  swc1       $f0, 0x160($sp)
    /* DD608C 00005620 E7A00144 */  swc1       $f0, 0x144($sp)
    /* DD6090 00005624 E7A00148 */  swc1       $f0, 0x148($sp)
    /* DD6094 00005628 E7A00150 */  swc1       $f0, 0x150($sp)
    /* DD6098 0000562C E7A00134 */  swc1       $f0, 0x134($sp)
    /* DD609C 00005630 E7A00138 */  swc1       $f0, 0x138($sp)
    /* DD60A0 00005634 E7A00140 */  swc1       $f0, 0x140($sp)
    /* DD60A4 00005638 E7A00130 */  swc1       $f0, 0x130($sp)
    /* DD60A8 0000563C E7A00114 */  swc1       $f0, 0x114($sp)
    /* DD60AC 00005640 E7A0011C */  swc1       $f0, 0x11C($sp)
    /* DD60B0 00005644 E7A00120 */  swc1       $f0, 0x120($sp)
    /* DD60B4 00005648 E7A00104 */  swc1       $f0, 0x104($sp)
    /* DD60B8 0000564C E7A0010C */  swc1       $f0, 0x10C($sp)
    /* DD60BC 00005650 E7A00110 */  swc1       $f0, 0x110($sp)
    /* DD60C0 00005654 E7A000F4 */  swc1       $f0, 0xF4($sp)
    /* DD60C4 00005658 E7A000FC */  swc1       $f0, 0xFC($sp)
    /* DD60C8 0000565C E7A00100 */  swc1       $f0, 0x100($sp)
    /* DD60CC 00005660 E7A000EC */  swc1       $f0, 0xEC($sp)
    /* DD60D0 00005664 E7A000F0 */  swc1       $f0, 0xF0($sp)
    /* DD60D4 00005668 E7A000D8 */  swc1       $f0, 0xD8($sp)
    /* DD60D8 0000566C E7A000DC */  swc1       $f0, 0xDC($sp)
    /* DD60DC 00005670 E7A000E0 */  swc1       $f0, 0xE0($sp)
    /* DD60E0 00005674 E7A000C8 */  swc1       $f0, 0xC8($sp)
    /* DD60E4 00005678 E7A000CC */  swc1       $f0, 0xCC($sp)
    /* DD60E8 0000567C E7A000D0 */  swc1       $f0, 0xD0($sp)
    /* DD60EC 00005680 E7AC014C */  swc1       $f12, 0x14C($sp)
    /* DD60F0 00005684 E7AC0124 */  swc1       $f12, 0x124($sp)
    /* DD60F4 00005688 E7AC0128 */  swc1       $f12, 0x128($sp)
    /* DD60F8 0000568C E7AC012C */  swc1       $f12, 0x12C($sp)
    /* DD60FC 00005690 E7AC0118 */  swc1       $f12, 0x118($sp)
    /* DD6100 00005694 E7AC00C4 */  swc1       $f12, 0xC4($sp)
    /* DD6104 00005698 E7A2013C */  swc1       $f2, 0x13C($sp)
    /* DD6108 0000569C E7A20108 */  swc1       $f2, 0x108($sp)
    /* DD610C 000056A0 E7A200E4 */  swc1       $f2, 0xE4($sp)
    /* DD6110 000056A4 E7A200E8 */  swc1       $f2, 0xE8($sp)
    /* DD6114 000056A8 E7A200D4 */  swc1       $f2, 0xD4($sp)
    /* DD6118 000056AC 14A10022 */  bne        $a1, $at, .L00005738
    /* DD611C 000056B0 E7A400F8 */   swc1      $f4, 0xF8($sp)
    /* DD6120 000056B4 3C0A0002 */  lui        $t2, (0x28002 >> 16)
    /* DD6124 000056B8 354A8002 */  ori        $t2, $t2, (0x28002 & 0xFFFF)
    /* DD6128 000056BC 3C050000 */  lui        $a1, %hi(sTag_pyr_7E20)
    /* DD612C 000056C0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD6130 000056C4 AD0A0004 */  sw         $t2, 0x4($t0)
    /* DD6134 000056C8 AD000000 */  sw         $zero, 0x0($t0)
    /* DD6138 000056CC 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD613C 000056D0 24A57E20 */  addiu      $a1, $a1, %lo(sTag_pyr_7E20)
    /* DD6140 000056D4 00002025 */  or         $a0, $zero, $zero
    /* DD6144 000056D8 0C000000 */  jal        func_00000000
    /* DD6148 000056DC 00003825 */   or        $a3, $zero, $zero
    /* DD614C 000056E0 3C0C0000 */  lui        $t4, (0x0 >> 16)
    /* DD6150 000056E4 8D8C0000 */  lw         $t4, 0x0($t4)
    /* DD6154 000056E8 3C0B0000 */  lui        $t3, (0x0 >> 16)
    /* DD6158 000056EC 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD615C 000056F0 256B0000 */  addiu      $t3, $t3, 0x0
    /* DD6160 000056F4 AC220000 */  sw         $v0, 0x0($at)
    /* DD6164 000056F8 1180000F */  beqz       $t4, .L00005738
    /* DD6168 000056FC 01603025 */   or        $a2, $t3, $zero
    /* DD616C 00005700 8D650000 */  lw         $a1, 0x0($t3)
  .L00005704:
    /* DD6170 00005704 3C040000 */  lui        $a0, %hi(D_00007E24)
    /* DD6174 00005708 24847E24 */  addiu      $a0, $a0, %lo(D_00007E24)
    /* DD6178 0000570C 0C000000 */  jal        func_00000000
    /* DD617C 00005710 AFA60098 */   sw        $a2, 0x98($sp)
    /* DD6180 00005714 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6184 00005718 8FA50098 */  lw         $a1, 0x98($sp)
    /* DD6188 0000571C 0C000000 */  jal        func_00000000
    /* DD618C 00005720 8C840000 */   lw        $a0, 0x0($a0)
    /* DD6190 00005724 8FA60098 */  lw         $a2, 0x98($sp)
    /* DD6194 00005728 8CC5000C */  lw         $a1, 0xC($a2)
    /* DD6198 0000572C 24C6000C */  addiu      $a2, $a2, 0xC
    /* DD619C 00005730 14A0FFF4 */  bnez       $a1, .L00005704
    /* DD61A0 00005734 00000000 */   nop
  .L00005738:
    /* DD61A4 00005738 8FAD0184 */  lw         $t5, 0x184($sp)
    /* DD61A8 0000573C 24030006 */  addiu      $v1, $zero, 0x6
    /* DD61AC 00005740 8FAE0184 */  lw         $t6, 0x184($sp)
    /* DD61B0 00005744 11A30003 */  beq        $t5, $v1, .L00005754
    /* DD61B4 00005748 3C040000 */   lui       $a0, (0x0 >> 16)
    /* DD61B8 0000574C 8FB00024 */  lw         $s0, 0x24($sp)
    /* DD61BC 00005750 36100020 */  ori        $s0, $s0, 0x20
  .L00005754:
    /* DD61C0 00005754 11C30003 */  beq        $t6, $v1, .L00005764
    /* DD61C4 00005758 36100004 */   ori       $s0, $s0, 0x4
    /* DD61C8 0000575C 2401000B */  addiu      $at, $zero, 0xB
    /* DD61CC 00005760 15C10002 */  bne        $t6, $at, .L0000576C
  .L00005764:
    /* DD61D0 00005764 2401F8FB */   addiu     $at, $zero, -0x705
    /* DD61D4 00005768 02018024 */  and        $s0, $s0, $at
  .L0000576C:
    /* DD61D8 0000576C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD61DC 00005770 8C820034 */  lw         $v0, 0x34($a0)
    /* DD61E0 00005774 24010005 */  addiu      $at, $zero, 0x5
    /* DD61E4 00005778 8FAF0184 */  lw         $t7, 0x184($sp)
    /* DD61E8 0000577C 14410002 */  bne        $v0, $at, .L00005788
    /* DD61EC 00005780 8FB90184 */   lw        $t9, 0x184($sp)
    /* DD61F0 00005784 36100001 */  ori        $s0, $s0, 0x1
  .L00005788:
    /* DD61F4 00005788 14620002 */  bne        $v1, $v0, .L00005794
    /* DD61F8 0000578C 24010002 */   addiu     $at, $zero, 0x2
    /* DD61FC 00005790 36100002 */  ori        $s0, $s0, 0x2
  .L00005794:
    /* DD6200 00005794 11E1000C */  beq        $t7, $at, .L000057C8
    /* DD6204 00005798 3C090000 */   lui       $t1, (0x0 >> 16)
    /* DD6208 0000579C 24010003 */  addiu      $at, $zero, 0x3
    /* DD620C 000057A0 11E10009 */  beq        $t7, $at, .L000057C8
    /* DD6210 000057A4 24010004 */   addiu     $at, $zero, 0x4
    /* DD6214 000057A8 11E10007 */  beq        $t7, $at, .L000057C8
    /* DD6218 000057AC 24010009 */   addiu     $at, $zero, 0x9
    /* DD621C 000057B0 51E10006 */  beql       $t7, $at, .L000057CC
    /* DD6220 000057B4 36100008 */   ori       $s0, $s0, 0x8
    /* DD6224 000057B8 8C980064 */  lw         $t8, 0x64($a0)
    /* DD6228 000057BC 24010007 */  addiu      $at, $zero, 0x7
    /* DD622C 000057C0 57010003 */  bnel       $t8, $at, .L000057D0
    /* DD6230 000057C4 24010001 */   addiu     $at, $zero, 0x1
  .L000057C8:
    /* DD6234 000057C8 36100008 */  ori        $s0, $s0, 0x8
  .L000057CC:
    /* DD6238 000057CC 24010001 */  addiu      $at, $zero, 0x1
  .L000057D0:
    /* DD623C 000057D0 17210002 */  bne        $t9, $at, .L000057DC
    /* DD6240 000057D4 00000000 */   nop
    /* DD6244 000057D8 3410FFFF */  ori        $s0, $zero, 0xFFFF
  .L000057DC:
    /* DD6248 000057DC 8D290000 */  lw         $t1, 0x0($t1)
    /* DD624C 000057E0 320A0004 */  andi       $t2, $s0, 0x4
    /* DD6250 000057E4 1140012C */  beqz       $t2, .L00005C98
    /* DD6254 000057E8 AD300034 */   sw        $s0, 0x34($t1)
    /* DD6258 000057EC 3C0C0000 */  lui        $t4, %hi(D_00007E34)
    /* DD625C 000057F0 8D8C7E34 */  lw         $t4, %lo(D_00007E34)($t4)
    /* DD6260 000057F4 24040074 */  addiu      $a0, $zero, 0x74
    /* DD6264 000057F8 AFAC0094 */  sw         $t4, 0x94($sp)
    /* DD6268 000057FC 0C000000 */  jal        func_00000000
    /* DD626C 00005800 AFAC007C */   sw        $t4, 0x7C($sp)
    /* DD6270 00005804 10400036 */  beqz       $v0, .L000058E0
    /* DD6274 00005808 00408025 */   or        $s0, $v0, $zero
    /* DD6278 0000580C 8FAD007C */  lw         $t5, 0x7C($sp)
    /* DD627C 00005810 00402025 */  or         $a0, $v0, $zero
    /* DD6280 00005814 14400005 */  bnez       $v0, .L0000582C
    /* DD6284 00005818 AFAD005C */   sw        $t5, 0x5C($sp)
    /* DD6288 0000581C 0C000000 */  jal        func_00000000
    /* DD628C 00005820 24040048 */   addiu     $a0, $zero, 0x48
    /* DD6290 00005824 1040000A */  beqz       $v0, .L00005850
    /* DD6294 00005828 00402025 */   or        $a0, $v0, $zero
  .L0000582C:
    /* DD6298 0000582C 8FA5005C */  lw         $a1, 0x5C($sp)
    /* DD629C 00005830 00003025 */  or         $a2, $zero, $zero
    /* DD62A0 00005834 AFA40064 */  sw         $a0, 0x64($sp)
    /* DD62A4 00005838 0C000000 */  jal        func_00000000
    /* DD62A8 0000583C AFA50004 */   sw        $a1, 0x4($sp)
    /* DD62AC 00005840 8FA40064 */  lw         $a0, 0x64($sp)
    /* DD62B0 00005844 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD62B4 00005848 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD62B8 0000584C AC8F0028 */  sw         $t7, 0x28($a0)
  .L00005850:
    /* DD62BC 00005850 3C180000 */  lui        $t8, %hi(D_00007AB8)
    /* DD62C0 00005854 27187AB8 */  addiu      $t8, $t8, %lo(D_00007AB8)
    /* DD62C4 00005858 AE180028 */  sw         $t8, 0x28($s0)
    /* DD62C8 0000585C 2604005C */  addiu      $a0, $s0, 0x5C
    /* DD62CC 00005860 2401FFA4 */  addiu      $at, $zero, -0x5C
    /* DD62D0 00005864 16010005 */  bne        $s0, $at, .L0000587C
    /* DD62D4 00005868 AFA40024 */   sw        $a0, 0x24($sp)
    /* DD62D8 0000586C 0C000000 */  jal        func_00000000
    /* DD62DC 00005870 24040018 */   addiu     $a0, $zero, 0x18
    /* DD62E0 00005874 10400003 */  beqz       $v0, .L00005884
    /* DD62E4 00005878 00402025 */   or        $a0, $v0, $zero
  .L0000587C:
    /* DD62E8 0000587C 0C000000 */  jal        func_00000000
    /* DD62EC 00005880 00000000 */   nop
  .L00005884:
    /* DD62F0 00005884 44801000 */  mtc1       $zero, $f2
    /* DD62F4 00005888 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD62F8 0000588C 44816000 */  mtc1       $at, $f12
    /* DD62FC 00005890 3C010000 */  lui        $at, %hi(snd_soundObjCreate_0000057C + 0x20)
    /* DD6300 00005894 E602004C */  swc1       $f2, 0x4C($s0)
    /* DD6304 00005898 E6020050 */  swc1       $f2, 0x50($s0)
    /* DD6308 0000589C E6020054 */  swc1       $f2, 0x54($s0)
    /* DD630C 000058A0 E60C0058 */  swc1       $f12, 0x58($s0)
    /* DD6310 000058A4 C420059C */  lwc1       $f0, %lo(snd_soundObjCreate_0000057C + 0x20)($at)
    /* DD6314 000058A8 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD6318 000058AC 27A50068 */  addiu      $a1, $sp, 0x68
    /* DD631C 000058B0 240603E1 */  addiu      $a2, $zero, 0x3E1
    /* DD6320 000058B4 240703E5 */  addiu      $a3, $zero, 0x3E5
    /* DD6324 000058B8 E7AC0074 */  swc1       $f12, 0x74($sp)
    /* DD6328 000058BC E7A00068 */  swc1       $f0, 0x68($sp)
    /* DD632C 000058C0 E7A0006C */  swc1       $f0, 0x6C($sp)
    /* DD6330 000058C4 0C000000 */  jal        func_00000000
    /* DD6334 000058C8 E7A00070 */   swc1      $f0, 0x70($sp)
    /* DD6338 000058CC 3C0A0000 */  lui        $t2, (0x0 >> 16)
    /* DD633C 000058D0 254A0000 */  addiu      $t2, $t2, 0x0
    /* DD6340 000058D4 24090001 */  addiu      $t1, $zero, 0x1
    /* DD6344 000058D8 AE090038 */  sw         $t1, 0x38($s0)
    /* DD6348 000058DC AE0A0048 */  sw         $t2, 0x48($s0)
  .L000058E0:
    /* DD634C 000058E0 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6350 000058E4 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6354 000058E8 02002825 */  or         $a1, $s0, $zero
    /* DD6358 000058EC 0C000000 */  jal        func_00000000
    /* DD635C 000058F0 24060004 */   addiu     $a2, $zero, 0x4
    /* DD6360 000058F4 3C0C0000 */  lui        $t4, %hi(D_00007E38)
    /* DD6364 000058F8 8D8C7E38 */  lw         $t4, %lo(D_00007E38)($t4)
    /* DD6368 000058FC 24040074 */  addiu      $a0, $zero, 0x74
    /* DD636C 00005900 AFAC0090 */  sw         $t4, 0x90($sp)
    /* DD6370 00005904 0C000000 */  jal        func_00000000
    /* DD6374 00005908 AFAC007C */   sw        $t4, 0x7C($sp)
    /* DD6378 0000590C 10400036 */  beqz       $v0, .L000059E8
    /* DD637C 00005910 00408025 */   or        $s0, $v0, $zero
    /* DD6380 00005914 8FAD007C */  lw         $t5, 0x7C($sp)
    /* DD6384 00005918 00402025 */  or         $a0, $v0, $zero
    /* DD6388 0000591C 14400005 */  bnez       $v0, .L00005934
    /* DD638C 00005920 AFAD005C */   sw        $t5, 0x5C($sp)
    /* DD6390 00005924 0C000000 */  jal        func_00000000
    /* DD6394 00005928 24040048 */   addiu     $a0, $zero, 0x48
    /* DD6398 0000592C 1040000A */  beqz       $v0, .L00005958
    /* DD639C 00005930 00402025 */   or        $a0, $v0, $zero
  .L00005934:
    /* DD63A0 00005934 8FA5005C */  lw         $a1, 0x5C($sp)
    /* DD63A4 00005938 00003025 */  or         $a2, $zero, $zero
    /* DD63A8 0000593C AFA40064 */  sw         $a0, 0x64($sp)
    /* DD63AC 00005940 0C000000 */  jal        func_00000000
    /* DD63B0 00005944 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD63B4 00005948 8FA40064 */  lw         $a0, 0x64($sp)
    /* DD63B8 0000594C 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD63BC 00005950 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD63C0 00005954 AC8F0028 */  sw         $t7, 0x28($a0)
  .L00005958:
    /* DD63C4 00005958 3C180000 */  lui        $t8, %hi(D_00007AB8)
    /* DD63C8 0000595C 27187AB8 */  addiu      $t8, $t8, %lo(D_00007AB8)
    /* DD63CC 00005960 AE180028 */  sw         $t8, 0x28($s0)
    /* DD63D0 00005964 2605005C */  addiu      $a1, $s0, 0x5C
    /* DD63D4 00005968 2401FFA4 */  addiu      $at, $zero, -0x5C
    /* DD63D8 0000596C 16010005 */  bne        $s0, $at, .L00005984
    /* DD63DC 00005970 AFA50024 */   sw        $a1, 0x24($sp)
    /* DD63E0 00005974 0C000000 */  jal        func_00000000
    /* DD63E4 00005978 24040018 */   addiu     $a0, $zero, 0x18
    /* DD63E8 0000597C 10400003 */  beqz       $v0, .L0000598C
    /* DD63EC 00005980 00402825 */   or        $a1, $v0, $zero
  .L00005984:
    /* DD63F0 00005984 0C000000 */  jal        func_00000000
    /* DD63F4 00005988 00A02025 */   or        $a0, $a1, $zero
  .L0000598C:
    /* DD63F8 0000598C 44801000 */  mtc1       $zero, $f2
    /* DD63FC 00005990 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD6400 00005994 44816000 */  mtc1       $at, $f12
    /* DD6404 00005998 3C010000 */  lui        $at, %hi(snd_soundObjCreate_0000057C + 0x24)
    /* DD6408 0000599C E602004C */  swc1       $f2, 0x4C($s0)
    /* DD640C 000059A0 E6020050 */  swc1       $f2, 0x50($s0)
    /* DD6410 000059A4 E6020054 */  swc1       $f2, 0x54($s0)
    /* DD6414 000059A8 E60C0058 */  swc1       $f12, 0x58($s0)
    /* DD6418 000059AC C42005A0 */  lwc1       $f0, %lo(snd_soundObjCreate_0000057C + 0x24)($at)
    /* DD641C 000059B0 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD6420 000059B4 27A50044 */  addiu      $a1, $sp, 0x44
    /* DD6424 000059B8 240603E1 */  addiu      $a2, $zero, 0x3E1
    /* DD6428 000059BC 240703E5 */  addiu      $a3, $zero, 0x3E5
    /* DD642C 000059C0 E7AC0050 */  swc1       $f12, 0x50($sp)
    /* DD6430 000059C4 E7A00044 */  swc1       $f0, 0x44($sp)
    /* DD6434 000059C8 E7A00048 */  swc1       $f0, 0x48($sp)
    /* DD6438 000059CC 0C000000 */  jal        func_00000000
    /* DD643C 000059D0 E7A0004C */   swc1      $f0, 0x4C($sp)
    /* DD6440 000059D4 3C0A0000 */  lui        $t2, (0x0 >> 16)
    /* DD6444 000059D8 254A0000 */  addiu      $t2, $t2, 0x0
    /* DD6448 000059DC 24090001 */  addiu      $t1, $zero, 0x1
    /* DD644C 000059E0 AE090038 */  sw         $t1, 0x38($s0)
    /* DD6450 000059E4 AE0A0048 */  sw         $t2, 0x48($s0)
  .L000059E8:
    /* DD6454 000059E8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6458 000059EC 8C840000 */  lw         $a0, 0x0($a0)
    /* DD645C 000059F0 02002825 */  or         $a1, $s0, $zero
    /* DD6460 000059F4 0C000000 */  jal        func_00000000
    /* DD6464 000059F8 24060004 */   addiu     $a2, $zero, 0x4
    /* DD6468 000059FC 3C0C0000 */  lui        $t4, %hi(D_00007E3C)
    /* DD646C 00005A00 8D8C7E3C */  lw         $t4, %lo(D_00007E3C)($t4)
    /* DD6470 00005A04 24040074 */  addiu      $a0, $zero, 0x74
    /* DD6474 00005A08 AFAC008C */  sw         $t4, 0x8C($sp)
    /* DD6478 00005A0C 0C000000 */  jal        func_00000000
    /* DD647C 00005A10 AFAC007C */   sw        $t4, 0x7C($sp)
    /* DD6480 00005A14 1040003E */  beqz       $v0, .L00005B10
    /* DD6484 00005A18 00408025 */   or        $s0, $v0, $zero
    /* DD6488 00005A1C 8FAD007C */  lw         $t5, 0x7C($sp)
    /* DD648C 00005A20 00402025 */  or         $a0, $v0, $zero
    /* DD6490 00005A24 14400005 */  bnez       $v0, .L00005A3C
    /* DD6494 00005A28 AFAD005C */   sw        $t5, 0x5C($sp)
    /* DD6498 00005A2C 0C000000 */  jal        func_00000000
    /* DD649C 00005A30 24040048 */   addiu     $a0, $zero, 0x48
    /* DD64A0 00005A34 1040000A */  beqz       $v0, .L00005A60
    /* DD64A4 00005A38 00402025 */   or        $a0, $v0, $zero
  .L00005A3C:
    /* DD64A8 00005A3C 8FA5005C */  lw         $a1, 0x5C($sp)
    /* DD64AC 00005A40 00003025 */  or         $a2, $zero, $zero
    /* DD64B0 00005A44 AFA40064 */  sw         $a0, 0x64($sp)
    /* DD64B4 00005A48 0C000000 */  jal        func_00000000
    /* DD64B8 00005A4C AFA50004 */   sw        $a1, 0x4($sp)
    /* DD64BC 00005A50 8FA40064 */  lw         $a0, 0x64($sp)
    /* DD64C0 00005A54 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD64C4 00005A58 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD64C8 00005A5C AC8F0028 */  sw         $t7, 0x28($a0)
  .L00005A60:
    /* DD64CC 00005A60 3C180000 */  lui        $t8, %hi(D_00007AB8)
    /* DD64D0 00005A64 27187AB8 */  addiu      $t8, $t8, %lo(D_00007AB8)
    /* DD64D4 00005A68 AE180028 */  sw         $t8, 0x28($s0)
    /* DD64D8 00005A6C 2605005C */  addiu      $a1, $s0, 0x5C
    /* DD64DC 00005A70 2401FFA4 */  addiu      $at, $zero, -0x5C
    /* DD64E0 00005A74 16010005 */  bne        $s0, $at, .L00005A8C
    /* DD64E4 00005A78 AFA50024 */   sw        $a1, 0x24($sp)
    /* DD64E8 00005A7C 0C000000 */  jal        func_00000000
    /* DD64EC 00005A80 24040018 */   addiu     $a0, $zero, 0x18
    /* DD64F0 00005A84 10400003 */  beqz       $v0, .L00005A94
    /* DD64F4 00005A88 00402825 */   or        $a1, $v0, $zero
  .L00005A8C:
    /* DD64F8 00005A8C 0C000000 */  jal        func_00000000
    /* DD64FC 00005A90 00A02025 */   or        $a0, $a1, $zero
  .L00005A94:
    /* DD6500 00005A94 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD6504 00005A98 44818000 */  mtc1       $at, $f16
    /* DD6508 00005A9C 44803000 */  mtc1       $zero, $f6
    /* DD650C 00005AA0 44804000 */  mtc1       $zero, $f8
    /* DD6510 00005AA4 44805000 */  mtc1       $zero, $f10
    /* DD6514 00005AA8 3C010000 */  lui        $at, %hi(snd_soundObjCreate_0000057C + 0x28)
    /* DD6518 00005AAC E6100058 */  swc1       $f16, 0x58($s0)
    /* DD651C 00005AB0 E606004C */  swc1       $f6, 0x4C($s0)
    /* DD6520 00005AB4 E6080050 */  swc1       $f8, 0x50($s0)
    /* DD6524 00005AB8 E60A0054 */  swc1       $f10, 0x54($s0)
    /* DD6528 00005ABC C43205A4 */  lwc1       $f18, %lo(snd_soundObjCreate_0000057C + 0x28)($at)
    /* DD652C 00005AC0 3C010000 */  lui        $at, %hi(snd_soundObjCreate_0000057C + 0x2C)
    /* DD6530 00005AC4 C42405A8 */  lwc1       $f4, %lo(snd_soundObjCreate_0000057C + 0x2C)($at)
    /* DD6534 00005AC8 3C010000 */  lui        $at, %hi(snd_soundObjCreate_0000057C + 0x30)
    /* DD6538 00005ACC C42605AC */  lwc1       $f6, %lo(snd_soundObjCreate_0000057C + 0x30)($at)
    /* DD653C 00005AD0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD6540 00005AD4 44814000 */  mtc1       $at, $f8
    /* DD6544 00005AD8 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD6548 00005ADC 27A50028 */  addiu      $a1, $sp, 0x28
    /* DD654C 00005AE0 240603E1 */  addiu      $a2, $zero, 0x3E1
    /* DD6550 00005AE4 240703E5 */  addiu      $a3, $zero, 0x3E5
    /* DD6554 00005AE8 E7B20028 */  swc1       $f18, 0x28($sp)
    /* DD6558 00005AEC E7A4002C */  swc1       $f4, 0x2C($sp)
    /* DD655C 00005AF0 E7A60030 */  swc1       $f6, 0x30($sp)
    /* DD6560 00005AF4 0C000000 */  jal        func_00000000
    /* DD6564 00005AF8 E7A80034 */   swc1      $f8, 0x34($sp)
    /* DD6568 00005AFC 3C0A0000 */  lui        $t2, (0x0 >> 16)
    /* DD656C 00005B00 254A0000 */  addiu      $t2, $t2, 0x0
    /* DD6570 00005B04 24090001 */  addiu      $t1, $zero, 0x1
    /* DD6574 00005B08 AE090038 */  sw         $t1, 0x38($s0)
    /* DD6578 00005B0C AE0A0048 */  sw         $t2, 0x48($s0)
  .L00005B10:
    /* DD657C 00005B10 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6580 00005B14 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6584 00005B18 02002825 */  or         $a1, $s0, $zero
    /* DD6588 00005B1C 0C000000 */  jal        func_00000000
    /* DD658C 00005B20 24060004 */   addiu     $a2, $zero, 0x4
    /* DD6590 00005B24 3C0C0002 */  lui        $t4, (0x28002 >> 16)
    /* DD6594 00005B28 358C8002 */  ori        $t4, $t4, (0x28002 & 0xFFFF)
    /* DD6598 00005B2C 3C010000 */  lui        $at, %hi(func_00000000 + 0x4)
    /* DD659C 00005B30 AC2C0004 */  sw         $t4, %lo(func_00000000 + 0x4)($at)
    /* DD65A0 00005B34 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD65A4 00005B38 3C050000 */  lui        $a1, %hi(D_00007E40)
    /* DD65A8 00005B3C 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD65AC 00005B40 AC200000 */  sw         $zero, 0x0($at)
    /* DD65B0 00005B44 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD65B4 00005B48 24A57E40 */  addiu      $a1, $a1, %lo(D_00007E40)
    /* DD65B8 00005B4C 00002025 */  or         $a0, $zero, $zero
    /* DD65BC 00005B50 0C000000 */  jal        func_00000000
    /* DD65C0 00005B54 00003825 */   or        $a3, $zero, $zero
    /* DD65C4 00005B58 3C0B0000 */  lui        $t3, %hi(D_00007E44)
    /* DD65C8 00005B5C 8D6B7E44 */  lw         $t3, %lo(D_00007E44)($t3)
    /* DD65CC 00005B60 AFA20084 */  sw         $v0, 0x84($sp)
    /* DD65D0 00005B64 2404006C */  addiu      $a0, $zero, 0x6C
    /* DD65D4 00005B68 AFAB0088 */  sw         $t3, 0x88($sp)
    /* DD65D8 00005B6C 0C000000 */  jal        func_00000000
    /* DD65DC 00005B70 AFAB007C */   sw        $t3, 0x7C($sp)
    /* DD65E0 00005B74 10400043 */  beqz       $v0, .L00005C84
    /* DD65E4 00005B78 00408025 */   or        $s0, $v0, $zero
    /* DD65E8 00005B7C 8FAE007C */  lw         $t6, 0x7C($sp)
    /* DD65EC 00005B80 00402025 */  or         $a0, $v0, $zero
    /* DD65F0 00005B84 14400005 */  bnez       $v0, .L00005B9C
    /* DD65F4 00005B88 AFAE005C */   sw        $t6, 0x5C($sp)
    /* DD65F8 00005B8C 0C000000 */  jal        func_00000000
    /* DD65FC 00005B90 24040048 */   addiu     $a0, $zero, 0x48
    /* DD6600 00005B94 1040000A */  beqz       $v0, .L00005BC0
    /* DD6604 00005B98 00402025 */   or        $a0, $v0, $zero
  .L00005B9C:
    /* DD6608 00005B9C 8FA5005C */  lw         $a1, 0x5C($sp)
    /* DD660C 00005BA0 8FA60084 */  lw         $a2, 0x84($sp)
    /* DD6610 00005BA4 AFA40064 */  sw         $a0, 0x64($sp)
    /* DD6614 00005BA8 0C000000 */  jal        func_00000000
    /* DD6618 00005BAC AFA50004 */   sw        $a1, 0x4($sp)
    /* DD661C 00005BB0 8FA40064 */  lw         $a0, 0x64($sp)
    /* DD6620 00005BB4 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DD6624 00005BB8 27180000 */  addiu      $t8, $t8, 0x0
    /* DD6628 00005BBC AC980028 */  sw         $t8, 0x28($a0)
  .L00005BC0:
    /* DD662C 00005BC0 3C190000 */  lui        $t9, %hi(D_00007788)
    /* DD6630 00005BC4 27397788 */  addiu      $t9, $t9, %lo(D_00007788)
    /* DD6634 00005BC8 2401FFB8 */  addiu      $at, $zero, -0x48
    /* DD6638 00005BCC AE190028 */  sw         $t9, 0x28($s0)
    /* DD663C 00005BD0 16010005 */  bne        $s0, $at, .L00005BE8
    /* DD6640 00005BD4 26050048 */   addiu     $a1, $s0, 0x48
    /* DD6644 00005BD8 0C000000 */  jal        func_00000000
    /* DD6648 00005BDC 24040010 */   addiu     $a0, $zero, 0x10
    /* DD664C 00005BE0 10400020 */  beqz       $v0, .L00005C64
    /* DD6650 00005BE4 00402825 */   or        $a1, $v0, $zero
  .L00005BE8:
    /* DD6654 00005BE8 14A00007 */  bnez       $a1, .L00005C08
    /* DD6658 00005BEC 00A01825 */   or        $v1, $a1, $zero
    /* DD665C 00005BF0 24040010 */  addiu      $a0, $zero, 0x10
    /* DD6660 00005BF4 0C000000 */  jal        func_00000000
    /* DD6664 00005BF8 AFA50058 */   sw        $a1, 0x58($sp)
    /* DD6668 00005BFC 8FA50058 */  lw         $a1, 0x58($sp)
    /* DD666C 00005C00 10400015 */  beqz       $v0, .L00005C58
    /* DD6670 00005C04 00401825 */   or        $v1, $v0, $zero
  .L00005C08:
    /* DD6674 00005C08 14600009 */  bnez       $v1, .L00005C30
    /* DD6678 00005C0C 00602025 */   or        $a0, $v1, $zero
    /* DD667C 00005C10 24040004 */  addiu      $a0, $zero, 0x4
    /* DD6680 00005C14 AFA30054 */  sw         $v1, 0x54($sp)
    /* DD6684 00005C18 0C000000 */  jal        func_00000000
    /* DD6688 00005C1C AFA50058 */   sw        $a1, 0x58($sp)
    /* DD668C 00005C20 8FA30054 */  lw         $v1, 0x54($sp)
    /* DD6690 00005C24 8FA50058 */  lw         $a1, 0x58($sp)
    /* DD6694 00005C28 10400004 */  beqz       $v0, .L00005C3C
    /* DD6698 00005C2C 00402025 */   or        $a0, $v0, $zero
  .L00005C30:
    /* DD669C 00005C30 3C090000 */  lui        $t1, %hi(D_00007528)
    /* DD66A0 00005C34 25297528 */  addiu      $t1, $t1, %lo(D_00007528)
    /* DD66A4 00005C38 AC890000 */  sw         $t1, 0x0($a0)
  .L00005C3C:
    /* DD66A8 00005C3C 3C0A0000 */  lui        $t2, %hi(D_00007520)
    /* DD66AC 00005C40 254A7520 */  addiu      $t2, $t2, %lo(D_00007520)
    /* DD66B0 00005C44 AC700004 */  sw         $s0, 0x4($v1)
    /* DD66B4 00005C48 8D4B0000 */  lw         $t3, 0x0($t2)
    /* DD66B8 00005C4C AC6B0008 */  sw         $t3, 0x8($v1)
    /* DD66BC 00005C50 8D4C0004 */  lw         $t4, 0x4($t2)
    /* DD66C0 00005C54 AC6C000C */  sw         $t4, 0xC($v1)
  .L00005C58:
    /* DD66C4 00005C58 3C0D0000 */  lui        $t5, %hi(D_00007770)
    /* DD66C8 00005C5C 25AD7770 */  addiu      $t5, $t5, %lo(D_00007770)
    /* DD66CC 00005C60 ACAD0000 */  sw         $t5, 0x0($a1)
  .L00005C64:
    /* DD66D0 00005C64 3C010000 */  lui        $at, (0x0 >> 16)
    /* DD66D4 00005C68 AC200000 */  sw         $zero, 0x0($at)
    /* DD66D8 00005C6C 240E0002 */  addiu      $t6, $zero, 0x2
    /* DD66DC 00005C70 AE0E005C */  sw         $t6, 0x5C($s0)
    /* DD66E0 00005C74 AE000064 */  sw         $zero, 0x64($s0)
    /* DD66E4 00005C78 AE000060 */  sw         $zero, 0x60($s0)
    /* DD66E8 00005C7C AE000058 */  sw         $zero, 0x58($s0)
    /* DD66EC 00005C80 AE000068 */  sw         $zero, 0x68($s0)
  .L00005C84:
    /* DD66F0 00005C84 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD66F4 00005C88 8C840000 */  lw         $a0, 0x0($a0)
    /* DD66F8 00005C8C 02002825 */  or         $a1, $s0, $zero
    /* DD66FC 00005C90 0C000000 */  jal        func_00000000
    /* DD6700 00005C94 3406FFFF */   ori       $a2, $zero, 0xFFFF
  .L00005C98:
    /* DD6704 00005C98 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6708 00005C9C 3C050000 */  lui        $a1, (0x0 >> 16)
    /* DD670C 00005CA0 24A50000 */  addiu      $a1, $a1, 0x0
    /* DD6710 00005CA4 0C000000 */  jal        func_00000000
    /* DD6714 00005CA8 8C840000 */   lw        $a0, 0x0($a0)
    /* DD6718 00005CAC 3C050000 */  lui        $a1, %hi(sStr_light01_7E48)
    /* DD671C 00005CB0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD6720 00005CB4 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD6724 00005CB8 24A57E48 */  addiu      $a1, $a1, %lo(sStr_light01_7E48)
    /* DD6728 00005CBC 00002025 */  or         $a0, $zero, $zero
    /* DD672C 00005CC0 0C000000 */  jal        func_00000000
    /* DD6730 00005CC4 00003825 */   or        $a3, $zero, $zero
    /* DD6734 00005CC8 3C0F0000 */  lui        $t7, %hi(Tv1Tmpl_update)
    /* DD6738 00005CCC 8DEF7E50 */  lw         $t7, %lo(Tv1Tmpl_update)($t7)
    /* DD673C 00005CD0 24040048 */  addiu      $a0, $zero, 0x48
    /* DD6740 00005CD4 AFA200BC */  sw         $v0, 0xBC($sp)
    /* DD6744 00005CD8 AFAF017C */  sw         $t7, 0x17C($sp)
    /* DD6748 00005CDC 0C000000 */  jal        func_00000000
    /* DD674C 00005CE0 AFAF007C */   sw        $t7, 0x7C($sp)
    /* DD6750 00005CE4 8FA600BC */  lw         $a2, 0xBC($sp)
    /* DD6754 00005CE8 10400008 */  beqz       $v0, .L00005D0C
    /* DD6758 00005CEC 00408025 */   or        $s0, $v0, $zero
    /* DD675C 00005CF0 8FA5007C */  lw         $a1, 0x7C($sp)
    /* DD6760 00005CF4 00402025 */  or         $a0, $v0, $zero
    /* DD6764 00005CF8 0C000000 */  jal        func_00000000
    /* DD6768 00005CFC AFA50004 */   sw        $a1, 0x4($sp)
    /* DD676C 00005D00 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD6770 00005D04 25290000 */  addiu      $t1, $t1, 0x0
    /* DD6774 00005D08 AE090028 */  sw         $t1, 0x28($s0)
  .L00005D0C:
    /* DD6778 00005D0C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD677C 00005D10 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6780 00005D14 02002825 */  or         $a1, $s0, $zero
    /* DD6784 00005D18 0C000000 */  jal        func_00000000
    /* DD6788 00005D1C 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD678C 00005D20 3C0A0000 */  lui        $t2, %hi(sTag_lig_7E54)
    /* DD6790 00005D24 8D4A7E54 */  lw         $t2, %lo(sTag_lig_7E54)($t2)
    /* DD6794 00005D28 24040048 */  addiu      $a0, $zero, 0x48
    /* DD6798 00005D2C AFAA0178 */  sw         $t2, 0x178($sp)
    /* DD679C 00005D30 0C000000 */  jal        func_00000000
    /* DD67A0 00005D34 AFAA007C */   sw        $t2, 0x7C($sp)
    /* DD67A4 00005D38 10400016 */  beqz       $v0, .L00005D94
    /* DD67A8 00005D3C 00408025 */   or        $s0, $v0, $zero
    /* DD67AC 00005D40 8FAB007C */  lw         $t3, 0x7C($sp)
    /* DD67B0 00005D44 00402025 */  or         $a0, $v0, $zero
    /* DD67B4 00005D48 14400005 */  bnez       $v0, .L00005D60
    /* DD67B8 00005D4C AFAB005C */   sw        $t3, 0x5C($sp)
    /* DD67BC 00005D50 0C000000 */  jal        func_00000000
    /* DD67C0 00005D54 24040048 */   addiu     $a0, $zero, 0x48
    /* DD67C4 00005D58 1040000B */  beqz       $v0, .L00005D88
    /* DD67C8 00005D5C 00402025 */   or        $a0, $v0, $zero
  .L00005D60:
    /* DD67CC 00005D60 8FA5005C */  lw         $a1, 0x5C($sp)
    /* DD67D0 00005D64 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD67D4 00005D68 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD67D8 00005D6C AFA40078 */  sw         $a0, 0x78($sp)
    /* DD67DC 00005D70 0C000000 */  jal        func_00000000
    /* DD67E0 00005D74 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD67E4 00005D78 8FA40078 */  lw         $a0, 0x78($sp)
    /* DD67E8 00005D7C 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD67EC 00005D80 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD67F0 00005D84 AC8E0028 */  sw         $t6, 0x28($a0)
  .L00005D88:
    /* DD67F4 00005D88 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD67F8 00005D8C 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD67FC 00005D90 AE0F0028 */  sw         $t7, 0x28($s0)
  .L00005D94:
    /* DD6800 00005D94 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6804 00005D98 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6808 00005D9C 02002825 */  or         $a1, $s0, $zero
    /* DD680C 00005DA0 0C000000 */  jal        func_00000000
    /* DD6810 00005DA4 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD6814 00005DA8 3C180000 */  lui        $t8, %hi(sTag_gol_7E58)
    /* DD6818 00005DAC 8F057E58 */  lw         $a1, %lo(sTag_gol_7E58)($t8)
    /* DD681C 00005DB0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD6820 00005DB4 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD6824 00005DB8 00002025 */  or         $a0, $zero, $zero
    /* DD6828 00005DBC 27A70144 */  addiu      $a3, $sp, 0x144
    /* DD682C 00005DC0 AFA50174 */  sw         $a1, 0x174($sp)
    /* DD6830 00005DC4 0C000000 */  jal        func_00000000
    /* DD6834 00005DC8 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD6838 00005DCC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD683C 00005DD0 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6840 00005DD4 00402825 */  or         $a1, $v0, $zero
    /* DD6844 00005DD8 0C000000 */  jal        func_00000000
    /* DD6848 00005DDC 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD684C 00005DE0 3C090000 */  lui        $t1, %hi(sTag_st0_7E5C)
    /* DD6850 00005DE4 8D257E5C */  lw         $a1, %lo(sTag_st0_7E5C)($t1)
    /* DD6854 00005DE8 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD6858 00005DEC 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD685C 00005DF0 00002025 */  or         $a0, $zero, $zero
    /* DD6860 00005DF4 27A70114 */  addiu      $a3, $sp, 0x114
    /* DD6864 00005DF8 AFA50170 */  sw         $a1, 0x170($sp)
    /* DD6868 00005DFC 0C000000 */  jal        func_00000000
    /* DD686C 00005E00 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD6870 00005E04 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6874 00005E08 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6878 00005E0C 00402825 */  or         $a1, $v0, $zero
    /* DD687C 00005E10 0C000000 */  jal        func_00000000
    /* DD6880 00005E14 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD6884 00005E18 3C0C0000 */  lui        $t4, %hi(sTag_en1_7E60)
    /* DD6888 00005E1C 8D857E60 */  lw         $a1, %lo(sTag_en1_7E60)($t4)
    /* DD688C 00005E20 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD6890 00005E24 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD6894 00005E28 00002025 */  or         $a0, $zero, $zero
    /* DD6898 00005E2C 27A700D4 */  addiu      $a3, $sp, 0xD4
    /* DD689C 00005E30 AFA5016C */  sw         $a1, 0x16C($sp)
    /* DD68A0 00005E34 0C000000 */  jal        func_00000000
    /* DD68A4 00005E38 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD68A8 00005E3C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD68AC 00005E40 8C840000 */  lw         $a0, 0x0($a0)
    /* DD68B0 00005E44 00402825 */  or         $a1, $v0, $zero
    /* DD68B4 00005E48 0C000000 */  jal        func_00000000
    /* DD68B8 00005E4C 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD68BC 00005E50 3C0D0000 */  lui        $t5, %hi(sTag_en2_7E64)
    /* DD68C0 00005E54 8DA57E64 */  lw         $a1, %lo(sTag_en2_7E64)($t5)
    /* DD68C4 00005E58 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD68C8 00005E5C 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD68CC 00005E60 00002025 */  or         $a0, $zero, $zero
    /* DD68D0 00005E64 27A700C4 */  addiu      $a3, $sp, 0xC4
    /* DD68D4 00005E68 AFA50168 */  sw         $a1, 0x168($sp)
    /* DD68D8 00005E6C 0C000000 */  jal        func_00000000
    /* DD68DC 00005E70 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD68E0 00005E74 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD68E4 00005E78 8C840000 */  lw         $a0, 0x0($a0)
    /* DD68E8 00005E7C 00402825 */  or         $a1, $v0, $zero
    /* DD68EC 00005E80 0C000000 */  jal        func_00000000
    /* DD68F0 00005E84 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD68F4 00005E88 3C0F0000 */  lui        $t7, %hi(sTag_cam_7E68)
    /* DD68F8 00005E8C 8DEF7E68 */  lw         $t7, %lo(sTag_cam_7E68)($t7)
    /* DD68FC 00005E90 24040058 */  addiu      $a0, $zero, 0x58
    /* DD6900 00005E94 AFAF0164 */  sw         $t7, 0x164($sp)
    /* DD6904 00005E98 0C000000 */  jal        func_00000000
    /* DD6908 00005E9C AFAF007C */   sw        $t7, 0x7C($sp)
    /* DD690C 00005EA0 1040000B */  beqz       $v0, .L00005ED0
    /* DD6910 00005EA4 00408025 */   or        $s0, $v0, $zero
    /* DD6914 00005EA8 8FA5007C */  lw         $a1, 0x7C($sp)
    /* DD6918 00005EAC 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD691C 00005EB0 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD6920 00005EB4 00402025 */  or         $a0, $v0, $zero
    /* DD6924 00005EB8 27A700E4 */  addiu      $a3, $sp, 0xE4
    /* DD6928 00005EBC 0C000000 */  jal        func_00000000
    /* DD692C 00005EC0 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD6930 00005EC4 3C090000 */  lui        $t1, %hi(D_00007600)
    /* DD6934 00005EC8 25297600 */  addiu      $t1, $t1, %lo(D_00007600)
    /* DD6938 00005ECC AE090028 */  sw         $t1, 0x28($s0)
  .L00005ED0:
    /* DD693C 00005ED0 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6940 00005ED4 8C840000 */  lw         $a0, 0x0($a0)
    /* DD6944 00005ED8 02002825 */  or         $a1, $s0, $zero
    /* DD6948 00005EDC 0C000000 */  jal        func_00000000
    /* DD694C 00005EE0 3406FFFF */   ori       $a2, $zero, 0xFFFF
    /* DD6950 00005EE4 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD6954 00005EE8 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD6958 00005EEC 27BD0180 */  addiu      $sp, $sp, 0x180
    /* DD695C 00005EF0 03E00008 */  jr         $ra
    /* DD6960 00005EF4 00000000 */   nop
endlabel item_registerTemplates
