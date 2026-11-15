nonmatching game_forEachPlayer_000020D8, 0x74

glabel game_forEachPlayer_000020D8
    /* DD2B44 000020D8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD2B48 000020DC AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD2B4C 000020E0 AFB30020 */  sw         $s3, 0x20($sp)
    /* DD2B50 000020E4 AFB2001C */  sw         $s2, 0x1C($sp)
    /* DD2B54 000020E8 AFB10018 */  sw         $s1, 0x18($sp)
    /* DD2B58 000020EC AFB00014 */  sw         $s0, 0x14($sp)
    /* DD2B5C 000020F0 8C8E00C0 */  lw         $t6, 0xC0($a0)
    /* DD2B60 000020F4 00809025 */  or         $s2, $a0, $zero
    /* DD2B64 000020F8 00A09825 */  or         $s3, $a1, $zero
    /* DD2B68 000020FC 19C0000C */  blez       $t6, .L00002130
    /* DD2B6C 00002100 00008825 */   or        $s1, $zero, $zero
    /* DD2B70 00002104 00808025 */  or         $s0, $a0, $zero
    /* DD2B74 00002108 02602025 */  or         $a0, $s3, $zero
  .L0000210C:
    /* DD2B78 0000210C 8E0500C4 */  lw         $a1, 0xC4($s0)
    /* DD2B7C 00002110 0C000000 */  jal        func_00000000
    /* DD2B80 00002114 8E0600C8 */   lw        $a2, 0xC8($s0)
    /* DD2B84 00002118 8E4F00C0 */  lw         $t7, 0xC0($s2)
    /* DD2B88 0000211C 26310001 */  addiu      $s1, $s1, 0x1
    /* DD2B8C 00002120 26100008 */  addiu      $s0, $s0, 0x8
    /* DD2B90 00002124 022F082A */  slt        $at, $s1, $t7
    /* DD2B94 00002128 5420FFF8 */  bnel       $at, $zero, .L0000210C
    /* DD2B98 0000212C 02602025 */   or        $a0, $s3, $zero
  .L00002130:
    /* DD2B9C 00002130 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DD2BA0 00002134 8FB00014 */  lw         $s0, 0x14($sp)
    /* DD2BA4 00002138 8FB10018 */  lw         $s1, 0x18($sp)
    /* DD2BA8 0000213C 8FB2001C */  lw         $s2, 0x1C($sp)
    /* DD2BAC 00002140 8FB30020 */  lw         $s3, 0x20($sp)
    /* DD2BB0 00002144 03E00008 */  jr         $ra
    /* DD2BB4 00002148 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel game_forEachPlayer_000020D8
