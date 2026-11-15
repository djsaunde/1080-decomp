nonmatching game_setupDemoRace, 0x2B8

glabel game_setupDemoRace
    /* DD4B88 0000411C 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* DD4B8C 00004120 AFB10030 */  sw         $s1, 0x30($sp)
    /* DD4B90 00004124 00808825 */  or         $s1, $a0, $zero
    /* DD4B94 00004128 AFBF0034 */  sw         $ra, 0x34($sp)
    /* DD4B98 0000412C AFB0002C */  sw         $s0, 0x2C($sp)
    /* DD4B9C 00004130 AFA50074 */  sw         $a1, 0x74($sp)
    /* DD4BA0 00004134 0C000000 */  jal        func_00000000
    /* DD4BA4 00004138 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4BA8 0000413C 10400004 */  beqz       $v0, .L00004150
    /* DD4BAC 00004140 00408025 */   or        $s0, $v0, $zero
    /* DD4BB0 00004144 00402025 */  or         $a0, $v0, $zero
    /* DD4BB4 00004148 0C000000 */  jal        func_00000000
    /* DD4BB8 0000414C 00002825 */   or        $a1, $zero, $zero
  .L00004150:
    /* DD4BBC 00004150 AFB00068 */  sw         $s0, 0x68($sp)
    /* DD4BC0 00004154 0C000000 */  jal        func_00000000
    /* DD4BC4 00004158 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4BC8 0000415C 10400004 */  beqz       $v0, .L00004170
    /* DD4BCC 00004160 00408025 */   or        $s0, $v0, $zero
    /* DD4BD0 00004164 00402025 */  or         $a0, $v0, $zero
    /* DD4BD4 00004168 0C000000 */  jal        func_00000000
    /* DD4BD8 0000416C 00002825 */   or        $a1, $zero, $zero
  .L00004170:
    /* DD4BDC 00004170 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD4BE0 00004174 24840000 */  addiu      $a0, $a0, 0x0
    /* DD4BE4 00004178 0C000000 */  jal        func_00000000
    /* DD4BE8 0000417C 8FA50068 */   lw        $a1, 0x68($sp)
    /* DD4BEC 00004180 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD4BF0 00004184 24840000 */  addiu      $a0, $a0, 0x0
    /* DD4BF4 00004188 0C000000 */  jal        func_00000000
    /* DD4BF8 0000418C 02002825 */   or        $a1, $s0, $zero
    /* DD4BFC 00004190 0C000000 */  jal        func_00000000
    /* DD4C00 00004194 00000000 */   nop
    /* DD4C04 00004198 3C010000 */  lui        $at, %hi(func_000003F8 + 0x14C)
    /* DD4C08 0000419C C4240544 */  lwc1       $f4, %lo(func_000003F8 + 0x14C)($at)
    /* DD4C0C 000041A0 46040182 */  mul.s      $f6, $f0, $f4
    /* DD4C10 000041A4 4600320D */  trunc.w.s  $f8, $f6
    /* DD4C14 000041A8 44064000 */  mfc1       $a2, $f8
    /* DD4C18 000041AC 0C000000 */  jal        func_00000000
    /* DD4C1C 000041B0 AFA6005C */   sw        $a2, 0x5C($sp)
    /* DD4C20 000041B4 8FA6005C */  lw         $a2, 0x5C($sp)
    /* DD4C24 000041B8 3C010000 */  lui        $at, %hi(func_000003F8 + 0x150)
    /* DD4C28 000041BC C42A0548 */  lwc1       $f10, %lo(func_000003F8 + 0x150)($at)
    /* DD4C2C 000041C0 44869000 */  mtc1       $a2, $f18
    /* DD4C30 000041C4 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD4C34 000041C8 460A0402 */  mul.s      $f16, $f0, $f10
    /* DD4C38 000041CC 44814000 */  mtc1       $at, $f8
    /* DD4C3C 000041D0 24010003 */  addiu      $at, $zero, 0x3
    /* DD4C40 000041D4 8FA90068 */  lw         $t1, 0x68($sp)
    /* DD4C44 000041D8 3C070000 */  lui        $a3, %hi(func_00000080 + 0x10)
    /* DD4C48 000041DC 3C0B0005 */  lui        $t3, (0x58005 >> 16)
    /* DD4C4C 000041E0 46809120 */  cvt.s.w    $f4, $f18
    /* DD4C50 000041E4 AFA90010 */  sw         $t1, 0x10($sp)
    /* DD4C54 000041E8 00064080 */  sll        $t0, $a2, 2
    /* DD4C58 000041EC 00E83821 */  addu       $a3, $a3, $t0
    /* DD4C5C 000041F0 356B8005 */  ori        $t3, $t3, (0x58005 & 0xFFFF)
    /* DD4C60 000041F4 240C001B */  addiu      $t4, $zero, 0x1B
    /* DD4C64 000041F8 46048180 */  add.s      $f6, $f16, $f4
    /* DD4C68 000041FC 8CE70090 */  lw         $a3, %lo(func_00000080 + 0x10)($a3)
    /* DD4C6C 00004200 02202025 */  or         $a0, $s1, $zero
    /* DD4C70 00004204 00002825 */  or         $a1, $zero, $zero
    /* DD4C74 00004208 46083280 */  add.s      $f10, $f6, $f8
    /* DD4C78 0000420C 4600548D */  trunc.w.s  $f18, $f10
    /* DD4C7C 00004210 44189000 */  mfc1       $t8, $f18
    /* DD4C80 00004214 00000000 */  nop
    /* DD4C84 00004218 0301001A */  div        $zero, $t8, $at
    /* DD4C88 0000421C 0000C810 */  mfhi       $t9
    /* DD4C8C 00004220 AFB90058 */  sw         $t9, 0x58($sp)
    /* DD4C90 00004224 8E2A0080 */  lw         $t2, 0x80($s1)
    /* DD4C94 00004228 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4C98 0000422C 44812000 */  mtc1       $at, $f4
    /* DD4C9C 00004230 AFAA0014 */  sw         $t2, 0x14($sp)
    /* DD4CA0 00004234 8E230098 */  lw         $v1, 0x98($s1)
    /* DD4CA4 00004238 C47000C4 */  lwc1       $f16, 0xC4($v1)
    /* DD4CA8 0000423C 46048181 */  sub.s      $f6, $f16, $f4
    /* DD4CAC 00004240 E7A60018 */  swc1       $f6, 0x18($sp)
    /* DD4CB0 00004244 C46800CC */  lwc1       $f8, 0xCC($v1)
    /* DD4CB4 00004248 AFAC0024 */  sw         $t4, 0x24($sp)
    /* DD4CB8 0000424C AFAB0020 */  sw         $t3, 0x20($sp)
    /* DD4CBC 00004250 0C000000 */  jal        func_00000000
    /* DD4CC0 00004254 E7A8001C */   swc1      $f8, 0x1C($sp)
    /* DD4CC4 00004258 AFA20060 */  sw         $v0, 0x60($sp)
    /* DD4CC8 0000425C AFB00010 */  sw         $s0, 0x10($sp)
    /* DD4CCC 00004260 8E2E0080 */  lw         $t6, 0x80($s1)
    /* DD4CD0 00004264 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4CD4 00004268 44819000 */  mtc1       $at, $f18
    /* DD4CD8 0000426C AFAE0014 */  sw         $t6, 0x14($sp)
    /* DD4CDC 00004270 8E230098 */  lw         $v1, 0x98($s1)
    /* DD4CE0 00004274 8FA60058 */  lw         $a2, 0x58($sp)
    /* DD4CE4 00004278 3C070000 */  lui        $a3, %hi(func_00000080 + 0x10)
    /* DD4CE8 0000427C C46A00C4 */  lwc1       $f10, 0xC4($v1)
    /* DD4CEC 00004280 3C0F0004 */  lui        $t7, (0x48024 >> 16)
    /* DD4CF0 00004284 00066880 */  sll        $t5, $a2, 2
    /* DD4CF4 00004288 46125400 */  add.s      $f16, $f10, $f18
    /* DD4CF8 0000428C 00ED3821 */  addu       $a3, $a3, $t5
    /* DD4CFC 00004290 35EF8024 */  ori        $t7, $t7, (0x48024 & 0xFFFF)
    /* DD4D00 00004294 2418001B */  addiu      $t8, $zero, 0x1B
    /* DD4D04 00004298 E7B00018 */  swc1       $f16, 0x18($sp)
    /* DD4D08 0000429C C46400CC */  lwc1       $f4, 0xCC($v1)
    /* DD4D0C 000042A0 AFB80024 */  sw         $t8, 0x24($sp)
    /* DD4D10 000042A4 AFAF0020 */  sw         $t7, 0x20($sp)
    /* DD4D14 000042A8 8CE70090 */  lw         $a3, %lo(func_00000080 + 0x10)($a3)
    /* DD4D18 000042AC 02202025 */  or         $a0, $s1, $zero
    /* DD4D1C 000042B0 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4D20 000042B4 0C000000 */  jal        func_00000000
    /* DD4D24 000042B8 E7A4001C */   swc1      $f4, 0x1C($sp)
    /* DD4D28 000042BC 8FA70060 */  lw         $a3, 0x60($sp)
    /* DD4D2C 000042C0 AFA20064 */  sw         $v0, 0x64($sp)
    /* DD4D30 000042C4 02202025 */  or         $a0, $s1, $zero
    /* DD4D34 000042C8 ACE20908 */  sw         $v0, 0x908($a3)
    /* DD4D38 000042CC AC470908 */  sw         $a3, 0x908($v0)
    /* DD4D3C 000042D0 8FB90068 */  lw         $t9, 0x68($sp)
    /* DD4D40 000042D4 8E260080 */  lw         $a2, 0x80($s1)
    /* DD4D44 000042D8 00002825 */  or         $a1, $zero, $zero
    /* DD4D48 000042DC 0C000000 */  jal        func_00000000
    /* DD4D4C 000042E0 AFB90010 */   sw        $t9, 0x10($sp)
    /* DD4D50 000042E4 8FA80068 */  lw         $t0, 0x68($sp)
    /* DD4D54 000042E8 AFA20054 */  sw         $v0, 0x54($sp)
    /* DD4D58 000042EC 8E260080 */  lw         $a2, 0x80($s1)
    /* DD4D5C 000042F0 02202025 */  or         $a0, $s1, $zero
    /* DD4D60 000042F4 24050001 */  addiu      $a1, $zero, 0x1
    /* DD4D64 000042F8 8FA70060 */  lw         $a3, 0x60($sp)
    /* DD4D68 000042FC 0C000000 */  jal        func_00000000
    /* DD4D6C 00004300 AFA80010 */   sw        $t0, 0x10($sp)
    /* DD4D70 00004304 8FA90074 */  lw         $t1, 0x74($sp)
    /* DD4D74 00004308 3C0B0000 */  lui        $t3, (0x0 >> 16)
    /* DD4D78 0000430C 256B0000 */  addiu      $t3, $t3, 0x0
    /* DD4D7C 00004310 000950C0 */  sll        $t2, $t1, 3
    /* DD4D80 00004314 01495023 */  subu       $t2, $t2, $t1
    /* DD4D84 00004318 000A5080 */  sll        $t2, $t2, 2
    /* DD4D88 0000431C AFA20050 */  sw         $v0, 0x50($sp)
    /* DD4D8C 00004320 014B2821 */  addu       $a1, $t2, $t3
    /* DD4D90 00004324 02202025 */  or         $a0, $s1, $zero
    /* DD4D94 00004328 0C000000 */  jal        func_00000000
    /* DD4D98 0000432C 8FA60054 */   lw        $a2, 0x54($sp)
    /* DD4D9C 00004330 AFA2004C */  sw         $v0, 0x4C($sp)
    /* DD4DA0 00004334 0C000000 */  jal        func_00000000
    /* DD4DA4 00004338 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4DA8 0000433C 10400004 */  beqz       $v0, .L00004350
    /* DD4DAC 00004340 00408025 */   or        $s0, $v0, $zero
    /* DD4DB0 00004344 00402025 */  or         $a0, $v0, $zero
    /* DD4DB4 00004348 0C000000 */  jal        func_00000000
    /* DD4DB8 0000434C 24050001 */   addiu     $a1, $zero, 0x1
  .L00004350:
    /* DD4DBC 00004350 8FAC0054 */  lw         $t4, 0x54($sp)
    /* DD4DC0 00004354 8FAD0050 */  lw         $t5, 0x50($sp)
    /* DD4DC4 00004358 00002025 */  or         $a0, $zero, $zero
    /* DD4DC8 0000435C 02002825 */  or         $a1, $s0, $zero
    /* DD4DCC 00004360 8FA60060 */  lw         $a2, 0x60($sp)
    /* DD4DD0 00004364 8FA7004C */  lw         $a3, 0x4C($sp)
    /* DD4DD4 00004368 AFAC0010 */  sw         $t4, 0x10($sp)
    /* DD4DD8 0000436C 0C000000 */  jal        func_00000000
    /* DD4DDC 00004370 AFAD0014 */   sw        $t5, 0x14($sp)
    /* DD4DE0 00004374 8E230084 */  lw         $v1, 0x84($s1)
    /* DD4DE4 00004378 00408025 */  or         $s0, $v0, $zero
    /* DD4DE8 0000437C 00402825 */  or         $a1, $v0, $zero
    /* DD4DEC 00004380 24640010 */  addiu      $a0, $v1, 0x10
    /* DD4DF0 00004384 0C000000 */  jal        func_00000000
    /* DD4DF4 00004388 AFA30038 */   sw        $v1, 0x38($sp)
    /* DD4DF8 0000438C 8E0E0014 */  lw         $t6, 0x14($s0)
    /* DD4DFC 00004390 8FA30038 */  lw         $v1, 0x38($sp)
    /* DD4E00 00004394 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD4E04 00004398 51C00003 */  beql       $t6, $zero, .L000043A8
    /* DD4E08 0000439C AE030014 */   sw        $v1, 0x14($s0)
    /* DD4E0C 000043A0 AE0F0004 */  sw         $t7, 0x4($s0)
    /* DD4E10 000043A4 AE030014 */  sw         $v1, 0x14($s0)
  .L000043A8:
    /* DD4E14 000043A8 8FB8004C */  lw         $t8, 0x4C($sp)
    /* DD4E18 000043AC 8FB90060 */  lw         $t9, 0x60($sp)
    /* DD4E1C 000043B0 AF3808DC */  sw         $t8, 0x8DC($t9)
    /* DD4E20 000043B4 8FA90064 */  lw         $t1, 0x64($sp)
    /* DD4E24 000043B8 8FA8004C */  lw         $t0, 0x4C($sp)
    /* DD4E28 000043BC AD2808DC */  sw         $t0, 0x8DC($t1)
    /* DD4E2C 000043C0 8FBF0034 */  lw         $ra, 0x34($sp)
    /* DD4E30 000043C4 8FB10030 */  lw         $s1, 0x30($sp)
    /* DD4E34 000043C8 8FB0002C */  lw         $s0, 0x2C($sp)
    /* DD4E38 000043CC 03E00008 */  jr         $ra
    /* DD4E3C 000043D0 27BD0070 */   addiu     $sp, $sp, 0x70
endlabel game_setupDemoRace
