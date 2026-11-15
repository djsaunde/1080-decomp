nonmatching SnowBoarder_reset, 0x1F8

glabel SnowBoarder_reset
    /* DDD554 0000CAE8 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DDD558 0000CAEC AFB00018 */  sw         $s0, 0x18($sp)
    /* DDD55C 0000CAF0 00808025 */  or         $s0, $a0, $zero
    /* DDD560 0000CAF4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDD564 0000CAF8 8E0E08C4 */  lw         $t6, 0x8C4($s0)
    /* DDD568 0000CAFC 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DDD56C 0000CB00 24050001 */  addiu      $a1, $zero, 0x1
    /* DDD570 0000CB04 000E7880 */  sll        $t7, $t6, 2
    /* DDD574 0000CB08 00CF3021 */  addu       $a2, $a2, $t7
    /* DDD578 0000CB0C 0C000000 */  jal        func_00000000
    /* DDD57C 0000CB10 8CC60000 */   lw        $a2, 0x0($a2)
    /* DDD580 0000CB14 86180904 */  lh         $t8, 0x904($s0)
    /* DDD584 0000CB18 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DDD588 0000CB1C 260403EC */  addiu      $a0, $s0, 0x3EC
    /* DDD58C 0000CB20 0018C840 */  sll        $t9, $t8, 1
    /* DDD590 0000CB24 02194021 */  addu       $t0, $s0, $t9
    /* DDD594 0000CB28 850908C6 */  lh         $t1, 0x8C6($t0)
    /* DDD598 0000CB2C 24050001 */  addiu      $a1, $zero, 0x1
    /* DDD59C 0000CB30 00095080 */  sll        $t2, $t1, 2
    /* DDD5A0 0000CB34 00CA3021 */  addu       $a2, $a2, $t2
    /* DDD5A4 0000CB38 0C000000 */  jal        func_00000000
    /* DDD5A8 0000CB3C 8CC60000 */   lw        $a2, 0x0($a2)
    /* DDD5AC 0000CB40 8E0B0A58 */  lw         $t3, 0xA58($s0)
    /* DDD5B0 0000CB44 316C0080 */  andi       $t4, $t3, 0x80
    /* DDD5B4 0000CB48 51800004 */  beql       $t4, $zero, .L0000CB5C
    /* DDD5B8 0000CB4C 02102823 */   subu      $a1, $s0, $s0
    /* DDD5BC 0000CB50 0C000000 */  jal        func_00000000
    /* DDD5C0 0000CB54 02002025 */   or        $a0, $s0, $zero
    /* DDD5C4 0000CB58 02102823 */  subu       $a1, $s0, $s0
  .L0000CB5C:
    /* DDD5C8 0000CB5C 24A5017C */  addiu      $a1, $a1, 0x17C
    /* DDD5CC 0000CB60 0C000000 */  jal        func_00000000
    /* DDD5D0 0000CB64 2604090C */   addiu     $a0, $s0, 0x90C
    /* DDD5D4 0000CB68 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent3_00000940 + 0x18)
    /* DDD5D8 0000CB6C C4240958 */  lwc1       $f4, %lo(SndHandle_triggerEvent3_00000940 + 0x18)($at)
    /* DDD5DC 0000CB70 C6060768 */  lwc1       $f6, 0x768($s0)
    /* DDD5E0 0000CB74 3C020000 */  lui        $v0, %hi(func_00000008 + 0x2C)
    /* DDD5E4 0000CB78 E60409D0 */  swc1       $f4, 0x9D0($s0)
    /* DDD5E8 0000CB7C E6060A78 */  swc1       $f6, 0xA78($s0)
    /* DDD5EC 0000CB80 8C420034 */  lw         $v0, %lo(func_00000008 + 0x2C)($v0)
    /* DDD5F0 0000CB84 24010002 */  addiu      $at, $zero, 0x2
    /* DDD5F4 0000CB88 44800000 */  mtc1       $zero, $f0
    /* DDD5F8 0000CB8C 1041000A */  beq        $v0, $at, .L0000CBB8
    /* DDD5FC 0000CB90 24180001 */   addiu     $t8, $zero, 0x1
    /* DDD600 0000CB94 24010003 */  addiu      $at, $zero, 0x3
    /* DDD604 0000CB98 1041000A */  beq        $v0, $at, .L0000CBC4
    /* DDD608 0000CB9C 240F004B */   addiu     $t7, $zero, 0x4B
    /* DDD60C 0000CBA0 24010004 */  addiu      $at, $zero, 0x4
    /* DDD610 0000CBA4 10410007 */  beq        $v0, $at, .L0000CBC4
    /* DDD614 0000CBA8 3C0D000F */   lui       $t5, (0xF4240 >> 16)
    /* DDD618 0000CBAC 35AD4240 */  ori        $t5, $t5, (0xF4240 & 0xFFFF)
    /* DDD61C 0000CBB0 10000005 */  b          .L0000CBC8
    /* DDD620 0000CBB4 AE0D0A14 */   sw        $t5, 0xA14($s0)
  .L0000CBB8:
    /* DDD624 0000CBB8 240E005A */  addiu      $t6, $zero, 0x5A
    /* DDD628 0000CBBC 10000002 */  b          .L0000CBC8
    /* DDD62C 0000CBC0 AE0E0A14 */   sw        $t6, 0xA14($s0)
  .L0000CBC4:
    /* DDD630 0000CBC4 AE0F0A14 */  sw         $t7, 0xA14($s0)
  .L0000CBC8:
    /* DDD634 0000CBC8 E60009F4 */  swc1       $f0, 0x9F4($s0)
    /* DDD638 0000CBCC E60009F0 */  swc1       $f0, 0x9F0($s0)
    /* DDD63C 0000CBD0 E60009EC */  swc1       $f0, 0x9EC($s0)
    /* DDD640 0000CBD4 E60009E8 */  swc1       $f0, 0x9E8($s0)
    /* DDD644 0000CBD8 AE1803DC */  sw         $t8, 0x3DC($s0)
    /* DDD648 0000CBDC AE000A54 */  sw         $zero, 0xA54($s0)
    /* DDD64C 0000CBE0 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DDD650 0000CBE4 8F390000 */  lw         $t9, 0x0($t9)
    /* DDD654 0000CBE8 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DDD658 0000CBEC 03214024 */  and        $t0, $t9, $at
    /* DDD65C 0000CBF0 3C010000 */  lui        $at, (0x0 >> 16)
    /* DDD660 0000CBF4 AC280000 */  sw         $t0, 0x0($at)
    /* DDD664 0000CBF8 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDD668 0000CBFC 44814000 */  mtc1       $at, $f8
    /* DDD66C 0000CC00 3C010000 */  lui        $at, (0x0 >> 16)
    /* DDD670 0000CC04 E6080A0C */  swc1       $f8, 0xA0C($s0)
    /* DDD674 0000CC08 AC200000 */  sw         $zero, 0x0($at)
    /* DDD678 0000CC0C 8E0208BC */  lw         $v0, 0x8BC($s0)
    /* DDD67C 0000CC10 30490020 */  andi       $t1, $v0, 0x20
    /* DDD680 0000CC14 11200003 */  beqz       $t1, .L0000CC24
    /* DDD684 0000CC18 AE020A58 */   sw        $v0, 0xA58($s0)
    /* DDD688 0000CC1C 0C000000 */  jal        func_00000000
    /* DDD68C 0000CC20 02002025 */   or        $a0, $s0, $zero
  .L0000CC24:
    /* DDD690 0000CC24 02002025 */  or         $a0, $s0, $zero
    /* DDD694 0000CC28 0C000000 */  jal        func_00000000
    /* DDD698 0000CC2C 260503A4 */   addiu     $a1, $s0, 0x3A4
    /* DDD69C 0000CC30 0C000000 */  jal        func_00000000
    /* DDD6A0 0000CC34 02002025 */   or        $a0, $s0, $zero
    /* DDD6A4 0000CC38 0C000000 */  jal        func_00000000
    /* DDD6A8 0000CC3C 8E040840 */   lw        $a0, 0x840($s0)
    /* DDD6AC 0000CC40 0C000000 */  jal        func_00000000
    /* DDD6B0 0000CC44 8E040804 */   lw        $a0, 0x804($s0)
    /* DDD6B4 0000CC48 8E0A0A58 */  lw         $t2, 0xA58($s0)
    /* DDD6B8 0000CC4C 3C040001 */  lui        $a0, %hi(func_00008A7C + 0x40)
    /* DDD6BC 0000CC50 24848ABC */  addiu      $a0, $a0, %lo(func_00008A7C + 0x40)
    /* DDD6C0 0000CC54 000A5B80 */  sll        $t3, $t2, 14
    /* DDD6C4 0000CC58 05630004 */  bgezl      $t3, .L0000CC6C
    /* DDD6C8 0000CC5C 8E0D00B4 */   lw        $t5, 0xB4($s0)
    /* DDD6CC 0000CC60 0C000000 */  jal        func_00000000
    /* DDD6D0 0000CC64 8E05000C */   lw        $a1, 0xC($s0)
    /* DDD6D4 0000CC68 8E0D00B4 */  lw         $t5, 0xB4($s0)
  .L0000CC6C:
    /* DDD6D8 0000CC6C 27A20024 */  addiu      $v0, $sp, 0x24
    /* DDD6DC 0000CC70 AC4D0000 */  sw         $t5, 0x0($v0)
    /* DDD6E0 0000CC74 8E0C00B8 */  lw         $t4, 0xB8($s0)
    /* DDD6E4 0000CC78 AC4C0004 */  sw         $t4, 0x4($v0)
    /* DDD6E8 0000CC7C 8E0D00BC */  lw         $t5, 0xBC($s0)
    /* DDD6EC 0000CC80 AC4D0008 */  sw         $t5, 0x8($v0)
    /* DDD6F0 0000CC84 C7AA0024 */  lwc1       $f10, 0x24($sp)
    /* DDD6F4 0000CC88 8E0F0318 */  lw         $t7, 0x318($s0)
    /* DDD6F8 0000CC8C E60A0890 */  swc1       $f10, 0x890($s0)
    /* DDD6FC 0000CC90 C7B00028 */  lwc1       $f16, 0x28($sp)
    /* DDD700 0000CC94 E6100894 */  swc1       $f16, 0x894($s0)
    /* DDD704 0000CC98 C7B2002C */  lwc1       $f18, 0x2C($sp)
    /* DDD708 0000CC9C E6120898 */  swc1       $f18, 0x898($s0)
    /* DDD70C 0000CCA0 AC4F0000 */  sw         $t7, 0x0($v0)
    /* DDD710 0000CCA4 8E0E031C */  lw         $t6, 0x31C($s0)
    /* DDD714 0000CCA8 AC4E0004 */  sw         $t6, 0x4($v0)
    /* DDD718 0000CCAC 8E0F0320 */  lw         $t7, 0x320($s0)
    /* DDD71C 0000CCB0 AC4F0008 */  sw         $t7, 0x8($v0)
    /* DDD720 0000CCB4 C7A40024 */  lwc1       $f4, 0x24($sp)
    /* DDD724 0000CCB8 E604089C */  swc1       $f4, 0x89C($s0)
    /* DDD728 0000CCBC C7A60028 */  lwc1       $f6, 0x28($sp)
    /* DDD72C 0000CCC0 E60608A0 */  swc1       $f6, 0x8A0($s0)
    /* DDD730 0000CCC4 C7A8002C */  lwc1       $f8, 0x2C($sp)
    /* DDD734 0000CCC8 E60808A4 */  swc1       $f8, 0x8A4($s0)
    /* DDD738 0000CCCC 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DDD73C 0000CCD0 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDD740 0000CCD4 27BD0050 */  addiu      $sp, $sp, 0x50
    /* DDD744 0000CCD8 03E00008 */  jr         $ra
    /* DDD748 0000CCDC 00000000 */   nop
endlabel SnowBoarder_reset
