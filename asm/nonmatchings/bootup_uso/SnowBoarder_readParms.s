nonmatching SnowBoarder_readParms, 0xAC

glabel SnowBoarder_readParms
    /* DDEF48 0000E4DC 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DDEF4C 0000E4E0 AFB20020 */  sw         $s2, 0x20($sp)
    /* DDEF50 0000E4E4 00809025 */  or         $s2, $a0, $zero
    /* DDEF54 0000E4E8 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DDEF58 0000E4EC AFB1001C */  sw         $s1, 0x1C($sp)
    /* DDEF5C 0000E4F0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DDEF60 0000E4F4 0C000000 */  jal        func_00000000
    /* DDEF64 0000E4F8 24840540 */   addiu     $a0, $a0, 0x540
    /* DDEF68 0000E4FC 0C002270 */  jal        stream_readS32_000089C0
    /* DDEF6C 0000E500 27A40034 */   addiu     $a0, $sp, 0x34
    /* DDEF70 0000E504 8FAE0034 */  lw         $t6, 0x34($sp)
    /* DDEF74 0000E508 00008025 */  or         $s0, $zero, $zero
    /* DDEF78 0000E50C 265108C8 */  addiu      $s1, $s2, 0x8C8
    /* DDEF7C 0000E510 59C0000A */  blezl      $t6, .L0000E53C
    /* DDEF80 0000E514 8E420840 */   lw        $v0, 0x840($s2)
  .L0000E518:
    /* DDEF84 0000E518 0C00227F */  jal        func_000089FC
    /* DDEF88 0000E51C 02202025 */   or        $a0, $s1, $zero
    /* DDEF8C 0000E520 8FAF0034 */  lw         $t7, 0x34($sp)
    /* DDEF90 0000E524 26100001 */  addiu      $s0, $s0, 0x1
    /* DDEF94 0000E528 26310002 */  addiu      $s1, $s1, 0x2
    /* DDEF98 0000E52C 020F082A */  slt        $at, $s0, $t7
    /* DDEF9C 0000E530 1420FFF9 */  bnez       $at, .L0000E518
    /* DDEFA0 0000E534 00000000 */   nop
    /* DDEFA4 0000E538 8E420840 */  lw         $v0, 0x840($s2)
  .L0000E53C:
    /* DDEFA8 0000E53C 8C500028 */  lw         $s0, 0x28($v0)
    /* DDEFAC 0000E540 8E19004C */  lw         $t9, 0x4C($s0)
    /* DDEFB0 0000E544 86180048 */  lh         $t8, 0x48($s0)
    /* DDEFB4 0000E548 0320F809 */  jalr       $t9
    /* DDEFB8 0000E54C 03022021 */   addu      $a0, $t8, $v0
    /* DDEFBC 0000E550 8E420804 */  lw         $v0, 0x804($s2)
    /* DDEFC0 0000E554 8C500028 */  lw         $s0, 0x28($v0)
    /* DDEFC4 0000E558 8E19004C */  lw         $t9, 0x4C($s0)
    /* DDEFC8 0000E55C 86080048 */  lh         $t0, 0x48($s0)
    /* DDEFCC 0000E560 0320F809 */  jalr       $t9
    /* DDEFD0 0000E564 01022021 */   addu      $a0, $t0, $v0
    /* DDEFD4 0000E568 0C000000 */  jal        func_00000000
    /* DDEFD8 0000E56C 26440108 */   addiu     $a0, $s2, 0x108
    /* DDEFDC 0000E570 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DDEFE0 0000E574 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDEFE4 0000E578 8FB1001C */  lw         $s1, 0x1C($sp)
    /* DDEFE8 0000E57C 8FB20020 */  lw         $s2, 0x20($sp)
    /* DDEFEC 0000E580 03E00008 */  jr         $ra
    /* DDEFF0 0000E584 27BD0038 */   addiu     $sp, $sp, 0x38
endlabel SnowBoarder_readParms
