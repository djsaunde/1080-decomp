nonmatching SnowBoarder_setReflectionDdms, 0x110

glabel SnowBoarder_setReflectionDdms
    /* DDE970 0000DF04 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDE974 0000DF08 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDE978 0000DF0C AFB00018 */  sw         $s0, 0x18($sp)
    /* DDE97C 0000DF10 8C8E0A58 */  lw         $t6, 0xA58($a0)
    /* DDE980 0000DF14 00808025 */  or         $s0, $a0, $zero
    /* DDE984 0000DF18 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE988 0000DF1C 31CF0080 */  andi       $t7, $t6, 0x80
    /* DDE98C 0000DF20 11E00018 */  beqz       $t7, .L0000DF84
    /* DDE990 0000DF24 24840000 */   addiu     $a0, $a0, 0x0
    /* DDE994 0000DF28 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE998 0000DF2C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDE99C 0000DF30 0C000000 */  jal        func_00000000
    /* DDE9A0 0000DF34 24050011 */   addiu     $a1, $zero, 0x11
    /* DDE9A4 0000DF38 8E1808F4 */  lw         $t8, 0x8F4($s0)
    /* DDE9A8 0000DF3C 8C480000 */  lw         $t0, 0x0($v0)
    /* DDE9AC 0000DF40 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDE9B0 0000DF44 24840000 */  addiu      $a0, $a0, 0x0
    /* DDE9B4 0000DF48 AF080000 */  sw         $t0, 0x0($t8)
    /* DDE9B8 0000DF4C 8C590004 */  lw         $t9, 0x4($v0)
    /* DDE9BC 0000DF50 24050012 */  addiu      $a1, $zero, 0x12
    /* DDE9C0 0000DF54 AF190004 */  sw         $t9, 0x4($t8)
    /* DDE9C4 0000DF58 8C480008 */  lw         $t0, 0x8($v0)
    /* DDE9C8 0000DF5C 0C000000 */  jal        func_00000000
    /* DDE9CC 0000DF60 AF080008 */   sw        $t0, 0x8($t8)
    /* DDE9D0 0000DF64 8E0908F8 */  lw         $t1, 0x8F8($s0)
    /* DDE9D4 0000DF68 8C4B0000 */  lw         $t3, 0x0($v0)
    /* DDE9D8 0000DF6C AD2B0000 */  sw         $t3, 0x0($t1)
    /* DDE9DC 0000DF70 8C4A0004 */  lw         $t2, 0x4($v0)
    /* DDE9E0 0000DF74 AD2A0004 */  sw         $t2, 0x4($t1)
    /* DDE9E4 0000DF78 8C4B0008 */  lw         $t3, 0x8($v0)
    /* DDE9E8 0000DF7C 10000015 */  b          .L0000DFD4
    /* DDE9EC 0000DF80 AD2B0008 */   sw        $t3, 0x8($t1)
  .L0000DF84:
    /* DDE9F0 0000DF84 0C000000 */  jal        func_00000000
    /* DDE9F4 0000DF88 24050014 */   addiu     $a1, $zero, 0x14
    /* DDE9F8 0000DF8C 8E0C08F4 */  lw         $t4, 0x8F4($s0)
    /* DDE9FC 0000DF90 8C4E0000 */  lw         $t6, 0x0($v0)
    /* DDEA00 0000DF94 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEA04 0000DF98 24840000 */  addiu      $a0, $a0, 0x0
    /* DDEA08 0000DF9C AD8E0000 */  sw         $t6, 0x0($t4)
    /* DDEA0C 0000DFA0 8C4D0004 */  lw         $t5, 0x4($v0)
    /* DDEA10 0000DFA4 24050015 */  addiu      $a1, $zero, 0x15
    /* DDEA14 0000DFA8 AD8D0004 */  sw         $t5, 0x4($t4)
    /* DDEA18 0000DFAC 8C4E0008 */  lw         $t6, 0x8($v0)
    /* DDEA1C 0000DFB0 0C000000 */  jal        func_00000000
    /* DDEA20 0000DFB4 AD8E0008 */   sw        $t6, 0x8($t4)
    /* DDEA24 0000DFB8 8E0F08F8 */  lw         $t7, 0x8F8($s0)
    /* DDEA28 0000DFBC 8C590000 */  lw         $t9, 0x0($v0)
    /* DDEA2C 0000DFC0 ADF90000 */  sw         $t9, 0x0($t7)
    /* DDEA30 0000DFC4 8C580004 */  lw         $t8, 0x4($v0)
    /* DDEA34 0000DFC8 ADF80004 */  sw         $t8, 0x4($t7)
    /* DDEA38 0000DFCC 8C590008 */  lw         $t9, 0x8($v0)
    /* DDEA3C 0000DFD0 ADF90008 */  sw         $t9, 0x8($t7)
  .L0000DFD4:
    /* DDEA40 0000DFD4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDEA44 0000DFD8 24840000 */  addiu      $a0, $a0, 0x0
    /* DDEA48 0000DFDC 0C000000 */  jal        func_00000000
    /* DDEA4C 0000DFE0 24050016 */   addiu     $a1, $zero, 0x16
    /* DDEA50 0000DFE4 8E0808FC */  lw         $t0, 0x8FC($s0)
    /* DDEA54 0000DFE8 8C4A0000 */  lw         $t2, 0x0($v0)
    /* DDEA58 0000DFEC AD0A0000 */  sw         $t2, 0x0($t0)
    /* DDEA5C 0000DFF0 8C490004 */  lw         $t1, 0x4($v0)
    /* DDEA60 0000DFF4 AD090004 */  sw         $t1, 0x4($t0)
    /* DDEA64 0000DFF8 8C4A0008 */  lw         $t2, 0x8($v0)
    /* DDEA68 0000DFFC AD0A0008 */  sw         $t2, 0x8($t0)
    /* DDEA6C 0000E000 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DDEA70 0000E004 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDEA74 0000E008 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDEA78 0000E00C 03E00008 */  jr         $ra
    /* DDEA7C 0000E010 00000000 */   nop
endlabel SnowBoarder_setReflectionDdms
