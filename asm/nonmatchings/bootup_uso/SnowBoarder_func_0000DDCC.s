nonmatching SnowBoarder_func_0000DDCC, 0x138

glabel SnowBoarder_func_0000DDCC
    /* DDE838 0000DDCC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDE83C 0000DDD0 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDE840 0000DDD4 AFB00018 */  sw         $s0, 0x18($sp)
    /* DDE844 0000DDD8 8C8208C4 */  lw         $v0, 0x8C4($a0)
    /* DDE848 0000DDDC 24010007 */  addiu      $at, $zero, 0x7
    /* DDE84C 0000DDE0 00808025 */  or         $s0, $a0, $zero
    /* DDE850 0000DDE4 10410003 */  beq        $v0, $at, .L0000DDF4
    /* DDE854 0000DDE8 24010006 */   addiu     $at, $zero, 0x6
    /* DDE858 0000DDEC 54410006 */  bnel       $v0, $at, .L0000DE08
    /* DDE85C 0000DDF0 8E0E0A58 */   lw        $t6, 0xA58($s0)
  .L0000DDF4:
    /* DDE860 0000DDF4 0C000000 */  jal        func_00000000
    /* DDE864 0000DDF8 02002025 */   or        $a0, $s0, $zero
    /* DDE868 0000DDFC 1000003D */  b          .L0000DEF4
    /* DDE86C 0000DE00 8FBF001C */   lw        $ra, 0x1C($sp)
    /* DDE870 0000DE04 8E0E0A58 */  lw         $t6, 0xA58($s0)
  .L0000DE08:
    /* DDE874 0000DE08 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE878 0000DE0C 2405000E */  addiu      $a1, $zero, 0xE
    /* DDE87C 0000DE10 31CF0080 */  andi       $t7, $t6, 0x80
    /* DDE880 0000DE14 11E00017 */  beqz       $t7, .L0000DE74
    /* DDE884 0000DE18 24840000 */   addiu     $a0, $a0, 0x0
    /* DDE888 0000DE1C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE88C 0000DE20 0C000000 */  jal        func_00000000
    /* DDE890 0000DE24 24840000 */   addiu     $a0, $a0, 0x0
    /* DDE894 0000DE28 8E1808F4 */  lw         $t8, 0x8F4($s0)
    /* DDE898 0000DE2C 8C480000 */  lw         $t0, 0x0($v0)
    /* DDE89C 0000DE30 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE8A0 0000DE34 24840000 */  addiu      $a0, $a0, 0x0
    /* DDE8A4 0000DE38 AF080000 */  sw         $t0, 0x0($t8)
    /* DDE8A8 0000DE3C 8C590004 */  lw         $t9, 0x4($v0)
    /* DDE8AC 0000DE40 2405000F */  addiu      $a1, $zero, 0xF
    /* DDE8B0 0000DE44 AF190004 */  sw         $t9, 0x4($t8)
    /* DDE8B4 0000DE48 8C480008 */  lw         $t0, 0x8($v0)
    /* DDE8B8 0000DE4C 0C000000 */  jal        func_00000000
    /* DDE8BC 0000DE50 AF080008 */   sw        $t0, 0x8($t8)
    /* DDE8C0 0000DE54 8E0908F8 */  lw         $t1, 0x8F8($s0)
    /* DDE8C4 0000DE58 8C4B0000 */  lw         $t3, 0x0($v0)
    /* DDE8C8 0000DE5C AD2B0000 */  sw         $t3, 0x0($t1)
    /* DDE8CC 0000DE60 8C4A0004 */  lw         $t2, 0x4($v0)
    /* DDE8D0 0000DE64 AD2A0004 */  sw         $t2, 0x4($t1)
    /* DDE8D4 0000DE68 8C4B0008 */  lw         $t3, 0x8($v0)
    /* DDE8D8 0000DE6C 10000015 */  b          .L0000DEC4
    /* DDE8DC 0000DE70 AD2B0008 */   sw        $t3, 0x8($t1)
  .L0000DE74:
    /* DDE8E0 0000DE74 0C000000 */  jal        func_00000000
    /* DDE8E4 0000DE78 2405000A */   addiu     $a1, $zero, 0xA
    /* DDE8E8 0000DE7C 8E0C08F4 */  lw         $t4, 0x8F4($s0)
    /* DDE8EC 0000DE80 8C4E0000 */  lw         $t6, 0x0($v0)
    /* DDE8F0 0000DE84 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE8F4 0000DE88 24840000 */  addiu      $a0, $a0, 0x0
    /* DDE8F8 0000DE8C AD8E0000 */  sw         $t6, 0x0($t4)
    /* DDE8FC 0000DE90 8C4D0004 */  lw         $t5, 0x4($v0)
    /* DDE900 0000DE94 2405000B */  addiu      $a1, $zero, 0xB
    /* DDE904 0000DE98 AD8D0004 */  sw         $t5, 0x4($t4)
    /* DDE908 0000DE9C 8C4E0008 */  lw         $t6, 0x8($v0)
    /* DDE90C 0000DEA0 0C000000 */  jal        func_00000000
    /* DDE910 0000DEA4 AD8E0008 */   sw        $t6, 0x8($t4)
    /* DDE914 0000DEA8 8E0F08F8 */  lw         $t7, 0x8F8($s0)
    /* DDE918 0000DEAC 8C590000 */  lw         $t9, 0x0($v0)
    /* DDE91C 0000DEB0 ADF90000 */  sw         $t9, 0x0($t7)
    /* DDE920 0000DEB4 8C580004 */  lw         $t8, 0x4($v0)
    /* DDE924 0000DEB8 ADF80004 */  sw         $t8, 0x4($t7)
    /* DDE928 0000DEBC 8C590008 */  lw         $t9, 0x8($v0)
    /* DDE92C 0000DEC0 ADF90008 */  sw         $t9, 0x8($t7)
  .L0000DEC4:
    /* DDE930 0000DEC4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE934 0000DEC8 24840000 */  addiu      $a0, $a0, 0x0
    /* DDE938 0000DECC 0C000000 */  jal        func_00000000
    /* DDE93C 0000DED0 2405000C */   addiu     $a1, $zero, 0xC
    /* DDE940 0000DED4 8E0808FC */  lw         $t0, 0x8FC($s0)
    /* DDE944 0000DED8 8C4A0000 */  lw         $t2, 0x0($v0)
    /* DDE948 0000DEDC AD0A0000 */  sw         $t2, 0x0($t0)
    /* DDE94C 0000DEE0 8C490004 */  lw         $t1, 0x4($v0)
    /* DDE950 0000DEE4 AD090004 */  sw         $t1, 0x4($t0)
    /* DDE954 0000DEE8 8C4A0008 */  lw         $t2, 0x8($v0)
    /* DDE958 0000DEEC AD0A0008 */  sw         $t2, 0x8($t0)
    /* DDE95C 0000DEF0 8FBF001C */  lw         $ra, 0x1C($sp)
  .L0000DEF4:
    /* DDE960 0000DEF4 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDE964 0000DEF8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDE968 0000DEFC 03E00008 */  jr         $ra
    /* DDE96C 0000DF00 00000000 */   nop
endlabel SnowBoarder_func_0000DDCC
