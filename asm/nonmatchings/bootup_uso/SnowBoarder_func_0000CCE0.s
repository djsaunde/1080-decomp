nonmatching SnowBoarder_func_0000CCE0, 0x2C0

glabel SnowBoarder_func_0000CCE0
    /* DDD74C 0000CCE0 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* DDD750 0000CCE4 3C010000 */  lui        $at, %hi(func_00000940 + 0x1C)
    /* DDD754 0000CCE8 C424095C */  lwc1       $f4, %lo(func_00000940 + 0x1C)($at)
    /* DDD758 0000CCEC 3C01C3AF */  lui        $at, (0xC3AF0000 >> 16)
    /* DDD75C 0000CCF0 44813000 */  mtc1       $at, $f6
    /* DDD760 0000CCF4 AFB0001C */  sw         $s0, 0x1C($sp)
    /* DDD764 0000CCF8 00808025 */  or         $s0, $a0, $zero
    /* DDD768 0000CCFC AFBF0024 */  sw         $ra, 0x24($sp)
    /* DDD76C 0000CD00 AFA5005C */  sw         $a1, 0x5C($sp)
    /* DDD770 0000CD04 AFB10020 */  sw         $s1, 0x20($sp)
    /* DDD774 0000CD08 02002825 */  or         $a1, $s0, $zero
    /* DDD778 0000CD0C 00002025 */  or         $a0, $zero, $zero
    /* DDD77C 0000CD10 24060002 */  addiu      $a2, $zero, 0x2
    /* DDD780 0000CD14 3C07430C */  lui        $a3, (0x430C0000 >> 16)
    /* DDD784 0000CD18 E7A40010 */  swc1       $f4, 0x10($sp)
    /* DDD788 0000CD1C 0C000000 */  jal        func_00000000
    /* DDD78C 0000CD20 E7A60014 */   swc1      $f6, 0x14($sp)
    /* DDD790 0000CD24 2611039C */  addiu      $s1, $s0, 0x39C
    /* DDD794 0000CD28 02202025 */  or         $a0, $s1, $zero
    /* DDD798 0000CD2C AFA2002C */  sw         $v0, 0x2C($sp)
    /* DDD79C 0000CD30 0C000000 */  jal        func_00000000
    /* DDD7A0 0000CD34 00402825 */   or        $a1, $v0, $zero
    /* DDD7A4 0000CD38 3C010000 */  lui        $at, %hi(func_00000940 + 0x20)
    /* DDD7A8 0000CD3C C4280960 */  lwc1       $f8, %lo(func_00000940 + 0x20)($at)
    /* DDD7AC 0000CD40 3C01C3AF */  lui        $at, (0xC3AF0000 >> 16)
    /* DDD7B0 0000CD44 44815000 */  mtc1       $at, $f10
    /* DDD7B4 0000CD48 00002025 */  or         $a0, $zero, $zero
    /* DDD7B8 0000CD4C 02002825 */  or         $a1, $s0, $zero
    /* DDD7BC 0000CD50 24060008 */  addiu      $a2, $zero, 0x8
    /* DDD7C0 0000CD54 3C07C30C */  lui        $a3, (0xC30C0000 >> 16)
    /* DDD7C4 0000CD58 E7A80010 */  swc1       $f8, 0x10($sp)
    /* DDD7C8 0000CD5C 0C000000 */  jal        func_00000000
    /* DDD7CC 0000CD60 E7AA0014 */   swc1      $f10, 0x14($sp)
    /* DDD7D0 0000CD64 AFA20030 */  sw         $v0, 0x30($sp)
    /* DDD7D4 0000CD68 02202025 */  or         $a0, $s1, $zero
    /* DDD7D8 0000CD6C 0C000000 */  jal        func_00000000
    /* DDD7DC 0000CD70 00402825 */   or        $a1, $v0, $zero
    /* DDD7E0 0000CD74 3C010000 */  lui        $at, %hi(func_00000940 + 0x24)
    /* DDD7E4 0000CD78 C4300964 */  lwc1       $f16, %lo(func_00000940 + 0x24)($at)
    /* DDD7E8 0000CD7C 3C0143AF */  lui        $at, (0x43AF0000 >> 16)
    /* DDD7EC 0000CD80 44819000 */  mtc1       $at, $f18
    /* DDD7F0 0000CD84 00002025 */  or         $a0, $zero, $zero
    /* DDD7F4 0000CD88 02002825 */  or         $a1, $s0, $zero
    /* DDD7F8 0000CD8C 24060001 */  addiu      $a2, $zero, 0x1
    /* DDD7FC 0000CD90 3C07430C */  lui        $a3, (0x430C0000 >> 16)
    /* DDD800 0000CD94 E7B00010 */  swc1       $f16, 0x10($sp)
    /* DDD804 0000CD98 0C000000 */  jal        func_00000000
    /* DDD808 0000CD9C E7B20014 */   swc1      $f18, 0x14($sp)
    /* DDD80C 0000CDA0 AFA20034 */  sw         $v0, 0x34($sp)
    /* DDD810 0000CDA4 02202025 */  or         $a0, $s1, $zero
    /* DDD814 0000CDA8 0C000000 */  jal        func_00000000
    /* DDD818 0000CDAC 00402825 */   or        $a1, $v0, $zero
    /* DDD81C 0000CDB0 3C010000 */  lui        $at, %hi(func_00000940 + 0x28)
    /* DDD820 0000CDB4 C4240968 */  lwc1       $f4, %lo(func_00000940 + 0x28)($at)
    /* DDD824 0000CDB8 3C0143AF */  lui        $at, (0x43AF0000 >> 16)
    /* DDD828 0000CDBC 44813000 */  mtc1       $at, $f6
    /* DDD82C 0000CDC0 00002025 */  or         $a0, $zero, $zero
    /* DDD830 0000CDC4 02002825 */  or         $a1, $s0, $zero
    /* DDD834 0000CDC8 24060004 */  addiu      $a2, $zero, 0x4
    /* DDD838 0000CDCC 3C07C30C */  lui        $a3, (0xC30C0000 >> 16)
    /* DDD83C 0000CDD0 E7A40010 */  swc1       $f4, 0x10($sp)
    /* DDD840 0000CDD4 0C000000 */  jal        func_00000000
    /* DDD844 0000CDD8 E7A60014 */   swc1      $f6, 0x14($sp)
    /* DDD848 0000CDDC AFA20038 */  sw         $v0, 0x38($sp)
    /* DDD84C 0000CDE0 02202025 */  or         $a0, $s1, $zero
    /* DDD850 0000CDE4 0C000000 */  jal        func_00000000
    /* DDD854 0000CDE8 00402825 */   or        $a1, $v0, $zero
    /* DDD858 0000CDEC 02002025 */  or         $a0, $s0, $zero
    /* DDD85C 0000CDF0 0C000000 */  jal        func_00000000
    /* DDD860 0000CDF4 8FA5002C */   lw        $a1, 0x2C($sp)
    /* DDD864 0000CDF8 02002025 */  or         $a0, $s0, $zero
    /* DDD868 0000CDFC 0C000000 */  jal        func_00000000
    /* DDD86C 0000CE00 8FA50030 */   lw        $a1, 0x30($sp)
    /* DDD870 0000CE04 02002025 */  or         $a0, $s0, $zero
    /* DDD874 0000CE08 0C000000 */  jal        func_00000000
    /* DDD878 0000CE0C 8FA50034 */   lw        $a1, 0x34($sp)
    /* DDD87C 0000CE10 02002025 */  or         $a0, $s0, $zero
    /* DDD880 0000CE14 0C000000 */  jal        func_00000000
    /* DDD884 0000CE18 8FA50038 */   lw        $a1, 0x38($sp)
    /* DDD888 0000CE1C 8E0E0A58 */  lw         $t6, 0xA58($s0)
    /* DDD88C 0000CE20 00002025 */  or         $a0, $zero, $zero
    /* DDD890 0000CE24 02002825 */  or         $a1, $s0, $zero
    /* DDD894 0000CE28 31CF0200 */  andi       $t7, $t6, 0x200
    /* DDD898 0000CE2C 15E00057 */  bnez       $t7, .L0000CF8C
    /* DDD89C 0000CE30 00003025 */   or        $a2, $zero, $zero
    /* DDD8A0 0000CE34 44800000 */  mtc1       $zero, $f0
    /* DDD8A4 0000CE38 3C010000 */  lui        $at, %hi(func_00000940 + 0x2C)
    /* DDD8A8 0000CE3C C428096C */  lwc1       $f8, %lo(func_00000940 + 0x2C)($at)
    /* DDD8AC 0000CE40 44070000 */  mfc1       $a3, $f0
    /* DDD8B0 0000CE44 E7A00014 */  swc1       $f0, 0x14($sp)
    /* DDD8B4 0000CE48 0C000000 */  jal        func_00000000
    /* DDD8B8 0000CE4C E7A80010 */   swc1      $f8, 0x10($sp)
    /* DDD8BC 0000CE50 AFA2003C */  sw         $v0, 0x3C($sp)
    /* DDD8C0 0000CE54 02202025 */  or         $a0, $s1, $zero
    /* DDD8C4 0000CE58 0C000000 */  jal        func_00000000
    /* DDD8C8 0000CE5C 00402825 */   or        $a1, $v0, $zero
    /* DDD8CC 0000CE60 02002025 */  or         $a0, $s0, $zero
    /* DDD8D0 0000CE64 0C000000 */  jal        func_00000000
    /* DDD8D4 0000CE68 8FA5003C */   lw        $a1, 0x3C($sp)
    /* DDD8D8 0000CE6C 3C01C316 */  lui        $at, (0xC3160000 >> 16)
    /* DDD8DC 0000CE70 44815000 */  mtc1       $at, $f10
    /* DDD8E0 0000CE74 3C01C3C8 */  lui        $at, (0xC3C80000 >> 16)
    /* DDD8E4 0000CE78 44818000 */  mtc1       $at, $f16
    /* DDD8E8 0000CE7C 00002025 */  or         $a0, $zero, $zero
    /* DDD8EC 0000CE80 02002825 */  or         $a1, $s0, $zero
    /* DDD8F0 0000CE84 24060020 */  addiu      $a2, $zero, 0x20
    /* DDD8F4 0000CE88 3C07437A */  lui        $a3, (0x437A0000 >> 16)
    /* DDD8F8 0000CE8C E7AA0010 */  swc1       $f10, 0x10($sp)
    /* DDD8FC 0000CE90 0C000000 */  jal        func_00000000
    /* DDD900 0000CE94 E7B00014 */   swc1      $f16, 0x14($sp)
    /* DDD904 0000CE98 AFA20044 */  sw         $v0, 0x44($sp)
    /* DDD908 0000CE9C 02202025 */  or         $a0, $s1, $zero
    /* DDD90C 0000CEA0 0C000000 */  jal        func_00000000
    /* DDD910 0000CEA4 00402825 */   or        $a1, $v0, $zero
    /* DDD914 0000CEA8 3C01C316 */  lui        $at, (0xC3160000 >> 16)
    /* DDD918 0000CEAC 44819000 */  mtc1       $at, $f18
    /* DDD91C 0000CEB0 3C01C3C8 */  lui        $at, (0xC3C80000 >> 16)
    /* DDD920 0000CEB4 44812000 */  mtc1       $at, $f4
    /* DDD924 0000CEB8 00002025 */  or         $a0, $zero, $zero
    /* DDD928 0000CEBC 02002825 */  or         $a1, $s0, $zero
    /* DDD92C 0000CEC0 24060080 */  addiu      $a2, $zero, 0x80
    /* DDD930 0000CEC4 3C07C37A */  lui        $a3, (0xC37A0000 >> 16)
    /* DDD934 0000CEC8 E7B20010 */  swc1       $f18, 0x10($sp)
    /* DDD938 0000CECC 0C000000 */  jal        func_00000000
    /* DDD93C 0000CED0 E7A40014 */   swc1      $f4, 0x14($sp)
    /* DDD940 0000CED4 AFA20048 */  sw         $v0, 0x48($sp)
    /* DDD944 0000CED8 02202025 */  or         $a0, $s1, $zero
    /* DDD948 0000CEDC 0C000000 */  jal        func_00000000
    /* DDD94C 0000CEE0 00402825 */   or        $a1, $v0, $zero
    /* DDD950 0000CEE4 3C01C316 */  lui        $at, (0xC3160000 >> 16)
    /* DDD954 0000CEE8 44813000 */  mtc1       $at, $f6
    /* DDD958 0000CEEC 3C0143C8 */  lui        $at, (0x43C80000 >> 16)
    /* DDD95C 0000CEF0 44814000 */  mtc1       $at, $f8
    /* DDD960 0000CEF4 00002025 */  or         $a0, $zero, $zero
    /* DDD964 0000CEF8 02002825 */  or         $a1, $s0, $zero
    /* DDD968 0000CEFC 24060010 */  addiu      $a2, $zero, 0x10
    /* DDD96C 0000CF00 3C07437A */  lui        $a3, (0x437A0000 >> 16)
    /* DDD970 0000CF04 E7A60010 */  swc1       $f6, 0x10($sp)
    /* DDD974 0000CF08 0C000000 */  jal        func_00000000
    /* DDD978 0000CF0C E7A80014 */   swc1      $f8, 0x14($sp)
    /* DDD97C 0000CF10 AFA2004C */  sw         $v0, 0x4C($sp)
    /* DDD980 0000CF14 02202025 */  or         $a0, $s1, $zero
    /* DDD984 0000CF18 0C000000 */  jal        func_00000000
    /* DDD988 0000CF1C 00402825 */   or        $a1, $v0, $zero
    /* DDD98C 0000CF20 3C01C316 */  lui        $at, (0xC3160000 >> 16)
    /* DDD990 0000CF24 44815000 */  mtc1       $at, $f10
    /* DDD994 0000CF28 3C0143C8 */  lui        $at, (0x43C80000 >> 16)
    /* DDD998 0000CF2C 44818000 */  mtc1       $at, $f16
    /* DDD99C 0000CF30 00002025 */  or         $a0, $zero, $zero
    /* DDD9A0 0000CF34 02002825 */  or         $a1, $s0, $zero
    /* DDD9A4 0000CF38 24060040 */  addiu      $a2, $zero, 0x40
    /* DDD9A8 0000CF3C 3C07C37A */  lui        $a3, (0xC37A0000 >> 16)
    /* DDD9AC 0000CF40 E7AA0010 */  swc1       $f10, 0x10($sp)
    /* DDD9B0 0000CF44 0C000000 */  jal        func_00000000
    /* DDD9B4 0000CF48 E7B00014 */   swc1      $f16, 0x14($sp)
    /* DDD9B8 0000CF4C AFA20050 */  sw         $v0, 0x50($sp)
    /* DDD9BC 0000CF50 02202025 */  or         $a0, $s1, $zero
    /* DDD9C0 0000CF54 0C000000 */  jal        func_00000000
    /* DDD9C4 0000CF58 00402825 */   or        $a1, $v0, $zero
    /* DDD9C8 0000CF5C 02002025 */  or         $a0, $s0, $zero
    /* DDD9CC 0000CF60 0C000000 */  jal        func_00000000
    /* DDD9D0 0000CF64 8FA50044 */   lw        $a1, 0x44($sp)
    /* DDD9D4 0000CF68 02002025 */  or         $a0, $s0, $zero
    /* DDD9D8 0000CF6C 0C000000 */  jal        func_00000000
    /* DDD9DC 0000CF70 8FA50048 */   lw        $a1, 0x48($sp)
    /* DDD9E0 0000CF74 02002025 */  or         $a0, $s0, $zero
    /* DDD9E4 0000CF78 0C000000 */  jal        func_00000000
    /* DDD9E8 0000CF7C 8FA5004C */   lw        $a1, 0x4C($sp)
    /* DDD9EC 0000CF80 02002025 */  or         $a0, $s0, $zero
    /* DDD9F0 0000CF84 0C000000 */  jal        func_00000000
    /* DDD9F4 0000CF88 8FA50050 */   lw        $a1, 0x50($sp)
  .L0000CF8C:
    /* DDD9F8 0000CF8C 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DDD9FC 0000CF90 8FB0001C */  lw         $s0, 0x1C($sp)
    /* DDDA00 0000CF94 8FB10020 */  lw         $s1, 0x20($sp)
    /* DDDA04 0000CF98 03E00008 */  jr         $ra
    /* DDDA08 0000CF9C 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel SnowBoarder_func_0000CCE0
