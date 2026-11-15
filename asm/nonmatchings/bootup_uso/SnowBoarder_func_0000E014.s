nonmatching SnowBoarder_func_0000E014, 0x110

glabel SnowBoarder_func_0000E014
    /* DDEA80 0000E014 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDEA84 0000E018 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDEA88 0000E01C AFB00018 */  sw         $s0, 0x18($sp)
    /* DDEA8C 0000E020 8C8E0A58 */  lw         $t6, 0xA58($a0)
    /* DDEA90 0000E024 00808025 */  or         $s0, $a0, $zero
    /* DDEA94 0000E028 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEA98 0000E02C 31CF0080 */  andi       $t7, $t6, 0x80
    /* DDEA9C 0000E030 11E00018 */  beqz       $t7, .L0000E094
    /* DDEAA0 0000E034 24840000 */   addiu     $a0, $a0, 0x0
    /* DDEAA4 0000E038 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEAA8 0000E03C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDEAAC 0000E040 0C000000 */  jal        func_00000000
    /* DDEAB0 0000E044 86050902 */   lh        $a1, 0x902($s0)
    /* DDEAB4 0000E048 8E1808F4 */  lw         $t8, 0x8F4($s0)
    /* DDEAB8 0000E04C 8C480000 */  lw         $t0, 0x0($v0)
    /* DDEABC 0000E050 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEAC0 0000E054 24840000 */  addiu      $a0, $a0, 0x0
    /* DDEAC4 0000E058 AF080000 */  sw         $t0, 0x0($t8)
    /* DDEAC8 0000E05C 8C590004 */  lw         $t9, 0x4($v0)
    /* DDEACC 0000E060 24050008 */  addiu      $a1, $zero, 0x8
    /* DDEAD0 0000E064 AF190004 */  sw         $t9, 0x4($t8)
    /* DDEAD4 0000E068 8C480008 */  lw         $t0, 0x8($v0)
    /* DDEAD8 0000E06C 0C000000 */  jal        func_00000000
    /* DDEADC 0000E070 AF080008 */   sw        $t0, 0x8($t8)
    /* DDEAE0 0000E074 8E0908F8 */  lw         $t1, 0x8F8($s0)
    /* DDEAE4 0000E078 8C4B0000 */  lw         $t3, 0x0($v0)
    /* DDEAE8 0000E07C AD2B0000 */  sw         $t3, 0x0($t1)
    /* DDEAEC 0000E080 8C4A0004 */  lw         $t2, 0x4($v0)
    /* DDEAF0 0000E084 AD2A0004 */  sw         $t2, 0x4($t1)
    /* DDEAF4 0000E088 8C4B0008 */  lw         $t3, 0x8($v0)
    /* DDEAF8 0000E08C 10000015 */  b          .L0000E0E4
    /* DDEAFC 0000E090 AD2B0008 */   sw        $t3, 0x8($t1)
  .L0000E094:
    /* DDEB00 0000E094 0C000000 */  jal        func_00000000
    /* DDEB04 0000E098 86050900 */   lh        $a1, 0x900($s0)
    /* DDEB08 0000E09C 8E0C08F4 */  lw         $t4, 0x8F4($s0)
    /* DDEB0C 0000E0A0 8C4E0000 */  lw         $t6, 0x0($v0)
    /* DDEB10 0000E0A4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEB14 0000E0A8 24840000 */  addiu      $a0, $a0, 0x0
    /* DDEB18 0000E0AC AD8E0000 */  sw         $t6, 0x0($t4)
    /* DDEB1C 0000E0B0 8C4D0004 */  lw         $t5, 0x4($v0)
    /* DDEB20 0000E0B4 24050005 */  addiu      $a1, $zero, 0x5
    /* DDEB24 0000E0B8 AD8D0004 */  sw         $t5, 0x4($t4)
    /* DDEB28 0000E0BC 8C4E0008 */  lw         $t6, 0x8($v0)
    /* DDEB2C 0000E0C0 0C000000 */  jal        func_00000000
    /* DDEB30 0000E0C4 AD8E0008 */   sw        $t6, 0x8($t4)
    /* DDEB34 0000E0C8 8E0F08F8 */  lw         $t7, 0x8F8($s0)
    /* DDEB38 0000E0CC 8C590000 */  lw         $t9, 0x0($v0)
    /* DDEB3C 0000E0D0 ADF90000 */  sw         $t9, 0x0($t7)
    /* DDEB40 0000E0D4 8C580004 */  lw         $t8, 0x4($v0)
    /* DDEB44 0000E0D8 ADF80004 */  sw         $t8, 0x4($t7)
    /* DDEB48 0000E0DC 8C590008 */  lw         $t9, 0x8($v0)
    /* DDEB4C 0000E0E0 ADF90008 */  sw         $t9, 0x8($t7)
  .L0000E0E4:
    /* DDEB50 0000E0E4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEB54 0000E0E8 24840000 */  addiu      $a0, $a0, 0x0
    /* DDEB58 0000E0EC 0C000000 */  jal        func_00000000
    /* DDEB5C 0000E0F0 24050006 */   addiu     $a1, $zero, 0x6
    /* DDEB60 0000E0F4 8E0808FC */  lw         $t0, 0x8FC($s0)
    /* DDEB64 0000E0F8 8C4A0000 */  lw         $t2, 0x0($v0)
    /* DDEB68 0000E0FC AD0A0000 */  sw         $t2, 0x0($t0)
    /* DDEB6C 0000E100 8C490004 */  lw         $t1, 0x4($v0)
    /* DDEB70 0000E104 AD090004 */  sw         $t1, 0x4($t0)
    /* DDEB74 0000E108 8C4A0008 */  lw         $t2, 0x8($v0)
    /* DDEB78 0000E10C AD0A0008 */  sw         $t2, 0x8($t0)
    /* DDEB7C 0000E110 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DDEB80 0000E114 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDEB84 0000E118 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDEB88 0000E11C 03E00008 */  jr         $ra
    /* DDEB8C 0000E120 00000000 */   nop
endlabel SnowBoarder_func_0000E014
