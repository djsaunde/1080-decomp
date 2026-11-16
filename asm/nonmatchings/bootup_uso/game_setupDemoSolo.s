nonmatching game_setupDemoSolo, 0x18C

glabel game_setupDemoSolo
    /* DD41A0 00003734 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* DD41A4 00003738 AFB10030 */  sw         $s1, 0x30($sp)
    /* DD41A8 0000373C 00808825 */  or         $s1, $a0, $zero
    /* DD41AC 00003740 AFBF0034 */  sw         $ra, 0x34($sp)
    /* DD41B0 00003744 AFB0002C */  sw         $s0, 0x2C($sp)
    /* DD41B4 00003748 AFA50064 */  sw         $a1, 0x64($sp)
    /* DD41B8 0000374C 0C000000 */  jal        func_00000000
    /* DD41BC 00003750 24040080 */   addiu     $a0, $zero, 0x80
    /* DD41C0 00003754 10400004 */  beqz       $v0, .L00003768
    /* DD41C4 00003758 00408025 */   or        $s0, $v0, $zero
    /* DD41C8 0000375C 00402025 */  or         $a0, $v0, $zero
    /* DD41CC 00003760 0C000000 */  jal        func_00000000
    /* DD41D0 00003764 00002825 */   or        $a1, $zero, $zero
  .L00003768:
    /* DD41D4 00003768 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD41D8 0000376C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD41DC 00003770 0C000000 */  jal        func_00000000
    /* DD41E0 00003774 02002825 */   or        $a1, $s0, $zero
    /* DD41E4 00003778 AFB00010 */  sw         $s0, 0x10($sp)
    /* DD41E8 0000377C 8E2E0080 */  lw         $t6, 0x80($s1)
    /* DD41EC 00003780 3C0F0005 */  lui        $t7, (0x58005 >> 16)
    /* DD41F0 00003784 35EF8005 */  ori        $t7, $t7, (0x58005 & 0xFFFF)
    /* DD41F4 00003788 AFAE0014 */  sw         $t6, 0x14($sp)
    /* DD41F8 0000378C 8E220098 */  lw         $v0, 0x98($s1)
    /* DD41FC 00003790 3C070000 */  lui        $a3, %hi(stream_readVec3_00000080 + 0x14)
    /* DD4200 00003794 2418001B */  addiu      $t8, $zero, 0x1B
    /* DD4204 00003798 C44400C4 */  lwc1       $f4, 0xC4($v0)
    /* DD4208 0000379C 8CE70094 */  lw         $a3, %lo(stream_readVec3_00000080 + 0x14)($a3)
    /* DD420C 000037A0 02202025 */  or         $a0, $s1, $zero
    /* DD4210 000037A4 E7A40018 */  swc1       $f4, 0x18($sp)
    /* DD4214 000037A8 C44600CC */  lwc1       $f6, 0xCC($v0)
    /* DD4218 000037AC AFB80024 */  sw         $t8, 0x24($sp)
    /* DD421C 000037B0 AFAF0020 */  sw         $t7, 0x20($sp)
    /* DD4220 000037B4 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4224 000037B8 24060001 */  addiu      $a2, $zero, 0x1
    /* DD4228 000037BC 0C000000 */  jal        func_00000000
    /* DD422C 000037C0 E7A6001C */   swc1      $f6, 0x1C($sp)
    /* DD4230 000037C4 AFA20058 */  sw         $v0, 0x58($sp)
    /* DD4234 000037C8 8E260080 */  lw         $a2, 0x80($s1)
    /* DD4238 000037CC AFB00010 */  sw         $s0, 0x10($sp)
    /* DD423C 000037D0 02202025 */  or         $a0, $s1, $zero
    /* DD4240 000037D4 00002825 */  or         $a1, $zero, $zero
    /* DD4244 000037D8 0C000000 */  jal        func_00000000
    /* DD4248 000037DC 00403825 */   or        $a3, $v0, $zero
    /* DD424C 000037E0 AFA20054 */  sw         $v0, 0x54($sp)
    /* DD4250 000037E4 8E260080 */  lw         $a2, 0x80($s1)
    /* DD4254 000037E8 AFB00010 */  sw         $s0, 0x10($sp)
    /* DD4258 000037EC 02202025 */  or         $a0, $s1, $zero
    /* DD425C 000037F0 24050001 */  addiu      $a1, $zero, 0x1
    /* DD4260 000037F4 0C000000 */  jal        func_00000000
    /* DD4264 000037F8 8FA70058 */   lw        $a3, 0x58($sp)
    /* DD4268 000037FC 8FB90064 */  lw         $t9, 0x64($sp)
    /* DD426C 00003800 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD4270 00003804 25290000 */  addiu      $t1, $t1, 0x0
    /* DD4274 00003808 001940C0 */  sll        $t0, $t9, 3
    /* DD4278 0000380C 01194023 */  subu       $t0, $t0, $t9
    /* DD427C 00003810 00084080 */  sll        $t0, $t0, 2
    /* DD4280 00003814 AFA20050 */  sw         $v0, 0x50($sp)
    /* DD4284 00003818 01092821 */  addu       $a1, $t0, $t1
    /* DD4288 0000381C 02202025 */  or         $a0, $s1, $zero
    /* DD428C 00003820 0C000000 */  jal        func_00000000
    /* DD4290 00003824 8FA60054 */   lw        $a2, 0x54($sp)
    /* DD4294 00003828 AFA2004C */  sw         $v0, 0x4C($sp)
    /* DD4298 0000382C 0C000000 */  jal        func_00000000
    /* DD429C 00003830 24040080 */   addiu     $a0, $zero, 0x80
    /* DD42A0 00003834 10400004 */  beqz       $v0, .L00003848
    /* DD42A4 00003838 00408025 */   or        $s0, $v0, $zero
    /* DD42A8 0000383C 00402025 */  or         $a0, $v0, $zero
    /* DD42AC 00003840 0C000000 */  jal        func_00000000
    /* DD42B0 00003844 24050001 */   addiu     $a1, $zero, 0x1
  .L00003848:
    /* DD42B4 00003848 8FAA0054 */  lw         $t2, 0x54($sp)
    /* DD42B8 0000384C 8FAB0050 */  lw         $t3, 0x50($sp)
    /* DD42BC 00003850 00002025 */  or         $a0, $zero, $zero
    /* DD42C0 00003854 02002825 */  or         $a1, $s0, $zero
    /* DD42C4 00003858 8FA60058 */  lw         $a2, 0x58($sp)
    /* DD42C8 0000385C 8FA7004C */  lw         $a3, 0x4C($sp)
    /* DD42CC 00003860 AFAA0010 */  sw         $t2, 0x10($sp)
    /* DD42D0 00003864 0C000000 */  jal        func_00000000
    /* DD42D4 00003868 AFAB0014 */   sw        $t3, 0x14($sp)
    /* DD42D8 0000386C 8E230084 */  lw         $v1, 0x84($s1)
    /* DD42DC 00003870 00408025 */  or         $s0, $v0, $zero
    /* DD42E0 00003874 00402825 */  or         $a1, $v0, $zero
    /* DD42E4 00003878 24640010 */  addiu      $a0, $v1, 0x10
    /* DD42E8 0000387C 0C000000 */  jal        func_00000000
    /* DD42EC 00003880 AFA30038 */   sw        $v1, 0x38($sp)
    /* DD42F0 00003884 8E0C0014 */  lw         $t4, 0x14($s0)
    /* DD42F4 00003888 8FA30038 */  lw         $v1, 0x38($sp)
    /* DD42F8 0000388C 240D0001 */  addiu      $t5, $zero, 0x1
    /* DD42FC 00003890 51800003 */  beql       $t4, $zero, .L000038A0
    /* DD4300 00003894 AE030014 */   sw        $v1, 0x14($s0)
    /* DD4304 00003898 AE0D0004 */  sw         $t5, 0x4($s0)
    /* DD4308 0000389C AE030014 */  sw         $v1, 0x14($s0)
  .L000038A0:
    /* DD430C 000038A0 8FAF0058 */  lw         $t7, 0x58($sp)
    /* DD4310 000038A4 8FAE004C */  lw         $t6, 0x4C($sp)
    /* DD4314 000038A8 ADEE08DC */  sw         $t6, 0x8DC($t7)
    /* DD4318 000038AC 8FBF0034 */  lw         $ra, 0x34($sp)
    /* DD431C 000038B0 8FB10030 */  lw         $s1, 0x30($sp)
    /* DD4320 000038B4 8FB0002C */  lw         $s0, 0x2C($sp)
    /* DD4324 000038B8 03E00008 */  jr         $ra
    /* DD4328 000038BC 27BD0060 */   addiu     $sp, $sp, 0x60
endlabel game_setupDemoSolo
