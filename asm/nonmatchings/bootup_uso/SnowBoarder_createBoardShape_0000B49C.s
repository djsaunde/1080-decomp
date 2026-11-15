nonmatching SnowBoarder_createBoardShape_0000B49C, 0x84

glabel SnowBoarder_createBoardShape_0000B49C
    /* DDBF08 0000B49C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDBF0C 0000B4A0 2881000A */  slti       $at, $a0, 0xA
    /* DDBF10 0000B4A4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDBF14 0000B4A8 14200002 */  bnez       $at, .L0000B4B4
    /* DDBF18 0000B4AC 00802825 */   or        $a1, $a0, $zero
    /* DDBF1C 0000B4B0 24050009 */  addiu      $a1, $zero, 0x9
  .L0000B4B4:
    /* DDBF20 0000B4B4 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DDBF24 0000B4B8 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DDBF28 0000B4BC 00051080 */  sll        $v0, $a1, 2
    /* DDBF2C 0000B4C0 004E1821 */  addu       $v1, $v0, $t6
    /* DDBF30 0000B4C4 8C640000 */  lw         $a0, 0x0($v1)
    /* DDBF34 0000B4C8 AFA30018 */  sw         $v1, 0x18($sp)
    /* DDBF38 0000B4CC 0C000000 */  jal        func_00000000
    /* DDBF3C 0000B4D0 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DDBF40 0000B4D4 14400005 */  bnez       $v0, .L0000B4EC
    /* DDBF44 0000B4D8 8FA30018 */   lw        $v1, 0x18($sp)
    /* DDBF48 0000B4DC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDBF4C 0000B4E0 24840000 */  addiu      $a0, $a0, 0x0
    /* DDBF50 0000B4E4 0C000000 */  jal        func_00000000
    /* DDBF54 0000B4E8 8C650000 */   lw        $a1, 0x0($v1)
  .L0000B4EC:
    /* DDBF58 0000B4EC 8FAF001C */  lw         $t7, 0x1C($sp)
    /* DDBF5C 0000B4F0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DDBF60 0000B4F4 3C050001 */  lui        $a1, %hi(func_00008A40 + 0x18)
    /* DDBF64 0000B4F8 00CF3021 */  addu       $a2, $a2, $t7
    /* DDBF68 0000B4FC 8CC60000 */  lw         $a2, 0x0($a2)
    /* DDBF6C 0000B500 24A58A58 */  addiu      $a1, $a1, %lo(func_00008A40 + 0x18)
    /* DDBF70 0000B504 00002025 */  or         $a0, $zero, $zero
    /* DDBF74 0000B508 0C000000 */  jal        func_00000000
    /* DDBF78 0000B50C 00003825 */   or        $a3, $zero, $zero
    /* DDBF7C 0000B510 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDBF80 0000B514 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDBF84 0000B518 03E00008 */  jr         $ra
    /* DDBF88 0000B51C 00000000 */   nop
endlabel SnowBoarder_createBoardShape_0000B49C
