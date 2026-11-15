nonmatching game_unloadUso, 0xD8

glabel game_unloadUso
    /* DE23C8 0001195C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE23CC 00011960 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE23D0 00011964 AFA40020 */  sw         $a0, 0x20($sp)
    /* DE23D4 00011968 AFA50024 */  sw         $a1, 0x24($sp)
    /* DE23D8 0001196C AFA00018 */  sw         $zero, 0x18($sp)
    /* DE23DC 00011970 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE23E0 00011974 8DCF0130 */  lw         $t7, 0x130($t6)
    /* DE23E4 00011978 AFAF001C */  sw         $t7, 0x1C($sp)
    /* DE23E8 0001197C 11E00027 */  beqz       $t7, .L00011A1C
    /* DE23EC 00011980 00000000 */   nop
  .L00011984:
    /* DE23F0 00011984 8FB8001C */  lw         $t8, 0x1C($sp)
    /* DE23F4 00011988 8F040000 */  lw         $a0, 0x0($t8)
    /* DE23F8 0001198C 8FA50024 */  lw         $a1, 0x24($sp)
    /* DE23FC 00011990 0C000000 */  jal        func_00000000
    /* DE2400 00011994 00000000 */   nop
    /* DE2404 00011998 14400019 */  bnez       $v0, .L00011A00
    /* DE2408 0001199C 00000000 */   nop
    /* DE240C 000119A0 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE2410 000119A4 17200007 */  bnez       $t9, .L000119C4
    /* DE2414 000119A8 00000000 */   nop
    /* DE2418 000119AC 8FA8001C */  lw         $t0, 0x1C($sp)
    /* DE241C 000119B0 8D090004 */  lw         $t1, 0x4($t0)
    /* DE2420 000119B4 8FAA0020 */  lw         $t2, 0x20($sp)
    /* DE2424 000119B8 AD490130 */  sw         $t1, 0x130($t2)
    /* DE2428 000119BC 10000005 */  b          .L000119D4
    /* DE242C 000119C0 00000000 */   nop
  .L000119C4:
    /* DE2430 000119C4 8FAB001C */  lw         $t3, 0x1C($sp)
    /* DE2434 000119C8 8D6C0004 */  lw         $t4, 0x4($t3)
    /* DE2438 000119CC 8FAD0018 */  lw         $t5, 0x18($sp)
    /* DE243C 000119D0 ADAC0004 */  sw         $t4, 0x4($t5)
  .L000119D4:
    /* DE2440 000119D4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE2444 000119D8 24840000 */  addiu      $a0, $a0, 0x0
    /* DE2448 000119DC 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DE244C 000119E0 8DC50000 */  lw         $a1, 0x0($t6)
    /* DE2450 000119E4 0C000000 */  jal        func_00000000
    /* DE2454 000119E8 00000000 */   nop
    /* DE2458 000119EC 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DE245C 000119F0 0C000000 */  jal        func_00000000
    /* DE2460 000119F4 00000000 */   nop
    /* DE2464 000119F8 1000000A */  b          .L00011A24
    /* DE2468 000119FC 00000000 */   nop
  .L00011A00:
    /* DE246C 00011A00 8FAF001C */  lw         $t7, 0x1C($sp)
    /* DE2470 00011A04 AFAF0018 */  sw         $t7, 0x18($sp)
    /* DE2474 00011A08 8FB8001C */  lw         $t8, 0x1C($sp)
    /* DE2478 00011A0C 8F190004 */  lw         $t9, 0x4($t8)
    /* DE247C 00011A10 AFB9001C */  sw         $t9, 0x1C($sp)
    /* DE2480 00011A14 1720FFDB */  bnez       $t9, .L00011984
    /* DE2484 00011A18 00000000 */   nop
  .L00011A1C:
    /* DE2488 00011A1C 10000001 */  b          .L00011A24
    /* DE248C 00011A20 00000000 */   nop
  .L00011A24:
    /* DE2490 00011A24 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE2494 00011A28 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE2498 00011A2C 03E00008 */  jr         $ra
    /* DE249C 00011A30 00000000 */   nop
endlabel game_unloadUso
