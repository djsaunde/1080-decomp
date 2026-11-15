nonmatching game_loadUso, 0x78

glabel game_loadUso
    /* DE2350 000118E4 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE2354 000118E8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE2358 000118EC AFA40020 */  sw         $a0, 0x20($sp)
    /* DE235C 000118F0 AFA50024 */  sw         $a1, 0x24($sp)
    /* DE2360 000118F4 24040008 */  addiu      $a0, $zero, 0x8
    /* DE2364 000118F8 0C000000 */  jal        func_00000000
    /* DE2368 000118FC 00000000 */   nop
    /* DE236C 00011900 AFA2001C */  sw         $v0, 0x1C($sp)
    /* DE2370 00011904 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE2374 00011908 8DCF0130 */  lw         $t7, 0x130($t6)
    /* DE2378 0001190C 8FB8001C */  lw         $t8, 0x1C($sp)
    /* DE237C 00011910 AF0F0004 */  sw         $t7, 0x4($t8)
    /* DE2380 00011914 8FB9001C */  lw         $t9, 0x1C($sp)
    /* DE2384 00011918 8FA80020 */  lw         $t0, 0x20($sp)
    /* DE2388 0001191C AD190130 */  sw         $t9, 0x130($t0)
    /* DE238C 00011920 8FA90024 */  lw         $t1, 0x24($sp)
    /* DE2390 00011924 8FAA001C */  lw         $t2, 0x1C($sp)
    /* DE2394 00011928 AD490000 */  sw         $t1, 0x0($t2)
    /* DE2398 0001192C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE239C 00011930 24840000 */  addiu      $a0, $a0, 0x0
    /* DE23A0 00011934 8FAB001C */  lw         $t3, 0x1C($sp)
    /* DE23A4 00011938 8D650000 */  lw         $a1, 0x0($t3)
    /* DE23A8 0001193C 0C000000 */  jal        func_00000000
    /* DE23AC 00011940 00000000 */   nop
    /* DE23B0 00011944 10000001 */  b          .L0001194C
    /* DE23B4 00011948 00000000 */   nop
  .L0001194C:
    /* DE23B8 0001194C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE23BC 00011950 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE23C0 00011954 03E00008 */  jr         $ra
    /* DE23C4 00011958 00000000 */   nop
endlabel game_loadUso
