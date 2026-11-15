nonmatching game_setupVs2P, 0x21C

glabel game_setupVs2P
    /* DD496C 00003F00 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* DD4970 00003F04 AFB10030 */  sw         $s1, 0x30($sp)
    /* DD4974 00003F08 00808825 */  or         $s1, $a0, $zero
    /* DD4978 00003F0C AFBF003C */  sw         $ra, 0x3C($sp)
    /* DD497C 00003F10 AFB30038 */  sw         $s3, 0x38($sp)
    /* DD4980 00003F14 AFB20034 */  sw         $s2, 0x34($sp)
    /* DD4984 00003F18 AFB0002C */  sw         $s0, 0x2C($sp)
    /* DD4988 00003F1C AFA50074 */  sw         $a1, 0x74($sp)
    /* DD498C 00003F20 0C000000 */  jal        func_00000000
    /* DD4990 00003F24 24040080 */   addiu     $a0, $zero, 0x80
    /* DD4994 00003F28 10400004 */  beqz       $v0, .L00003F3C
    /* DD4998 00003F2C 00409825 */   or        $s3, $v0, $zero
    /* DD499C 00003F30 00402025 */  or         $a0, $v0, $zero
    /* DD49A0 00003F34 0C000000 */  jal        func_00000000
    /* DD49A4 00003F38 24050001 */   addiu     $a1, $zero, 0x1
  .L00003F3C:
    /* DD49A8 00003F3C AFB30068 */  sw         $s3, 0x68($sp)
    /* DD49AC 00003F40 0C000000 */  jal        func_00000000
    /* DD49B0 00003F44 24040080 */   addiu     $a0, $zero, 0x80
    /* DD49B4 00003F48 10400004 */  beqz       $v0, .L00003F5C
    /* DD49B8 00003F4C 00409825 */   or        $s3, $v0, $zero
    /* DD49BC 00003F50 00402025 */  or         $a0, $v0, $zero
    /* DD49C0 00003F54 0C000000 */  jal        func_00000000
    /* DD49C4 00003F58 24050002 */   addiu     $a1, $zero, 0x2
  .L00003F5C:
    /* DD49C8 00003F5C 3C100000 */  lui        $s0, (0x0 >> 16)
    /* DD49CC 00003F60 26100000 */  addiu      $s0, $s0, 0x0
    /* DD49D0 00003F64 AFB3006C */  sw         $s3, 0x6C($sp)
    /* DD49D4 00003F68 02002025 */  or         $a0, $s0, $zero
    /* DD49D8 00003F6C 0C000000 */  jal        func_00000000
    /* DD49DC 00003F70 8FA50068 */   lw        $a1, 0x68($sp)
    /* DD49E0 00003F74 02002025 */  or         $a0, $s0, $zero
    /* DD49E4 00003F78 0C000000 */  jal        func_00000000
    /* DD49E8 00003F7C 02602825 */   or        $a1, $s3, $zero
    /* DD49EC 00003F80 02002025 */  or         $a0, $s0, $zero
    /* DD49F0 00003F84 8FA50068 */  lw         $a1, 0x68($sp)
    /* DD49F4 00003F88 0C000000 */  jal        func_00000000
    /* DD49F8 00003F8C 02603025 */   or        $a2, $s3, $zero
    /* DD49FC 00003F90 8FAE0068 */  lw         $t6, 0x68($sp)
    /* DD4A00 00003F94 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4A04 00003F98 44813000 */  mtc1       $at, $f6
    /* DD4A08 00003F9C AFAE0010 */  sw         $t6, 0x10($sp)
    /* DD4A0C 00003FA0 8E2F0080 */  lw         $t7, 0x80($s1)
    /* DD4A10 00003FA4 8E180050 */  lw         $t8, 0x50($s0)
    /* DD4A14 00003FA8 24190015 */  addiu      $t9, $zero, 0x15
    /* DD4A18 00003FAC AFAF0014 */  sw         $t7, 0x14($sp)
    /* DD4A1C 00003FB0 8E230098 */  lw         $v1, 0x98($s1)
    /* DD4A20 00003FB4 02202025 */  or         $a0, $s1, $zero
    /* DD4A24 00003FB8 00002825 */  or         $a1, $zero, $zero
    /* DD4A28 00003FBC C46400C4 */  lwc1       $f4, 0xC4($v1)
    /* DD4A2C 00003FC0 8E06004C */  lw         $a2, 0x4C($s0)
    /* DD4A30 00003FC4 8E070054 */  lw         $a3, 0x54($s0)
    /* DD4A34 00003FC8 46062201 */  sub.s      $f8, $f4, $f6
    /* DD4A38 00003FCC E7A80018 */  swc1       $f8, 0x18($sp)
    /* DD4A3C 00003FD0 C46A00CC */  lwc1       $f10, 0xCC($v1)
    /* DD4A40 00003FD4 AFB90024 */  sw         $t9, 0x24($sp)
    /* DD4A44 00003FD8 AFB80020 */  sw         $t8, 0x20($sp)
    /* DD4A48 00003FDC 0C000000 */  jal        func_00000000
    /* DD4A4C 00003FE0 E7AA001C */   swc1      $f10, 0x1C($sp)
    /* DD4A50 00003FE4 AFB30010 */  sw         $s3, 0x10($sp)
    /* DD4A54 00003FE8 8E280080 */  lw         $t0, 0x80($s1)
    /* DD4A58 00003FEC 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD4A5C 00003FF0 44819000 */  mtc1       $at, $f18
    /* DD4A60 00003FF4 AFA80014 */  sw         $t0, 0x14($sp)
    /* DD4A64 00003FF8 8E230098 */  lw         $v1, 0x98($s1)
    /* DD4A68 00003FFC 8E09005C */  lw         $t1, 0x5C($s0)
    /* DD4A6C 00004000 240A0015 */  addiu      $t2, $zero, 0x15
    /* DD4A70 00004004 C47000C4 */  lwc1       $f16, 0xC4($v1)
    /* DD4A74 00004008 00409025 */  or         $s2, $v0, $zero
    /* DD4A78 0000400C 02202025 */  or         $a0, $s1, $zero
    /* DD4A7C 00004010 46128100 */  add.s      $f4, $f16, $f18
    /* DD4A80 00004014 24050001 */  addiu      $a1, $zero, 0x1
    /* DD4A84 00004018 8E060058 */  lw         $a2, 0x58($s0)
    /* DD4A88 0000401C 8E070060 */  lw         $a3, 0x60($s0)
    /* DD4A8C 00004020 E7A40018 */  swc1       $f4, 0x18($sp)
    /* DD4A90 00004024 C46600CC */  lwc1       $f6, 0xCC($v1)
    /* DD4A94 00004028 AFAA0024 */  sw         $t2, 0x24($sp)
    /* DD4A98 0000402C AFA90020 */  sw         $t1, 0x20($sp)
    /* DD4A9C 00004030 0C000000 */  jal        func_00000000
    /* DD4AA0 00004034 E7A6001C */   swc1      $f6, 0x1C($sp)
    /* DD4AA4 00004038 3C0B0000 */  lui        $t3, %hi(D_000074E4)
    /* DD4AA8 0000403C 256B74E4 */  addiu      $t3, $t3, %lo(D_000074E4)
    /* DD4AAC 00004040 3C0C0000 */  lui        $t4, %hi(D_000074EC)
    /* DD4AB0 00004044 AE4B000C */  sw         $t3, 0xC($s2)
    /* DD4AB4 00004048 258C74EC */  addiu      $t4, $t4, %lo(D_000074EC)
    /* DD4AB8 0000404C AC4C000C */  sw         $t4, 0xC($v0)
    /* DD4ABC 00004050 AE420908 */  sw         $v0, 0x908($s2)
    /* DD4AC0 00004054 AC520908 */  sw         $s2, 0x908($v0)
    /* DD4AC4 00004058 8FAD0068 */  lw         $t5, 0x68($sp)
    /* DD4AC8 0000405C 8E260080 */  lw         $a2, 0x80($s1)
    /* DD4ACC 00004060 00408025 */  or         $s0, $v0, $zero
    /* DD4AD0 00004064 02202025 */  or         $a0, $s1, $zero
    /* DD4AD4 00004068 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4AD8 0000406C 02403825 */  or         $a3, $s2, $zero
    /* DD4ADC 00004070 0C000000 */  jal        func_00000000
    /* DD4AE0 00004074 AFAD0010 */   sw        $t5, 0x10($sp)
    /* DD4AE4 00004078 8FAE006C */  lw         $t6, 0x6C($sp)
    /* DD4AE8 0000407C 8E260080 */  lw         $a2, 0x80($s1)
    /* DD4AEC 00004080 00409825 */  or         $s3, $v0, $zero
    /* DD4AF0 00004084 02202025 */  or         $a0, $s1, $zero
    /* DD4AF4 00004088 24050002 */  addiu      $a1, $zero, 0x2
    /* DD4AF8 0000408C 02003825 */  or         $a3, $s0, $zero
    /* DD4AFC 00004090 0C000000 */  jal        func_00000000
    /* DD4B00 00004094 AFAE0010 */   sw        $t6, 0x10($sp)
    /* DD4B04 00004098 3C01425C */  lui        $at, (0x425C0000 >> 16)
    /* DD4B08 0000409C 44810000 */  mtc1       $at, $f0
    /* DD4B0C 000040A0 AFA20050 */  sw         $v0, 0x50($sp)
    /* DD4B10 000040A4 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD4B14 000040A8 E660014C */  swc1       $f0, 0x14C($s3)
    /* DD4B18 000040AC E440014C */  swc1       $f0, 0x14C($v0)
    /* DD4B1C 000040B0 8FAF0074 */  lw         $t7, 0x74($sp)
    /* DD4B20 000040B4 27390000 */  addiu      $t9, $t9, 0x0
    /* DD4B24 000040B8 02202025 */  or         $a0, $s1, $zero
    /* DD4B28 000040BC 000FC0C0 */  sll        $t8, $t7, 3
    /* DD4B2C 000040C0 030FC023 */  subu       $t8, $t8, $t7
    /* DD4B30 000040C4 0018C080 */  sll        $t8, $t8, 2
    /* DD4B34 000040C8 03193821 */  addu       $a3, $t8, $t9
    /* DD4B38 000040CC 00E02825 */  or         $a1, $a3, $zero
    /* DD4B3C 000040D0 AFA70044 */  sw         $a3, 0x44($sp)
    /* DD4B40 000040D4 0C000000 */  jal        func_00000000
    /* DD4B44 000040D8 02603025 */   or        $a2, $s3, $zero
    /* DD4B48 000040DC 8FA70044 */  lw         $a3, 0x44($sp)
    /* DD4B4C 000040E0 00409825 */  or         $s3, $v0, $zero
    /* DD4B50 000040E4 02202025 */  or         $a0, $s1, $zero
    /* DD4B54 000040E8 8FA60050 */  lw         $a2, 0x50($sp)
    /* DD4B58 000040EC 0C000000 */  jal        func_00000000
    /* DD4B5C 000040F0 24E5001C */   addiu     $a1, $a3, 0x1C
    /* DD4B60 000040F4 AE5308DC */  sw         $s3, 0x8DC($s2)
    /* DD4B64 000040F8 AE0208DC */  sw         $v0, 0x8DC($s0)
    /* DD4B68 000040FC AC5301B8 */  sw         $s3, 0x1B8($v0)
    /* DD4B6C 00004100 8FBF003C */  lw         $ra, 0x3C($sp)
    /* DD4B70 00004104 8FB30038 */  lw         $s3, 0x38($sp)
    /* DD4B74 00004108 8FB20034 */  lw         $s2, 0x34($sp)
    /* DD4B78 0000410C 8FB10030 */  lw         $s1, 0x30($sp)
    /* DD4B7C 00004110 8FB0002C */  lw         $s0, 0x2C($sp)
    /* DD4B80 00004114 03E00008 */  jr         $ra
    /* DD4B84 00004118 27BD0070 */   addiu     $sp, $sp, 0x70
endlabel game_setupVs2P
