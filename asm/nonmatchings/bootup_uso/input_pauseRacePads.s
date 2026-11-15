nonmatching input_pauseRacePads, 0x94

glabel input_pauseRacePads
    /* DE25C8 00011B5C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE25CC 00011B60 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE25D0 00011B64 AFA40020 */  sw         $a0, 0x20($sp)
    /* DE25D4 00011B68 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE25D8 00011B6C 8DCF0188 */  lw         $t7, 0x188($t6)
    /* DE25DC 00011B70 AFAF001C */  sw         $t7, 0x1C($sp)
    /* DE25E0 00011B74 24180001 */  addiu      $t8, $zero, 0x1
    /* DE25E4 00011B78 8FB90020 */  lw         $t9, 0x20($sp)
    /* DE25E8 00011B7C AF380188 */  sw         $t8, 0x188($t9)
    /* DE25EC 00011B80 AFA00018 */  sw         $zero, 0x18($sp)
    /* DE25F0 00011B84 8FA80020 */  lw         $t0, 0x20($sp)
    /* DE25F4 00011B88 8D090124 */  lw         $t1, 0x124($t0)
    /* DE25F8 00011B8C 19200011 */  blez       $t1, .L00011BD4
    /* DE25FC 00011B90 00000000 */   nop
  .L00011B94:
    /* DE2600 00011B94 8FAA0020 */  lw         $t2, 0x20($sp)
    /* DE2604 00011B98 8FAB0018 */  lw         $t3, 0x18($sp)
    /* DE2608 00011B9C 000B6080 */  sll        $t4, $t3, 2
    /* DE260C 00011BA0 014C6821 */  addu       $t5, $t2, $t4
    /* DE2610 00011BA4 8DA40100 */  lw         $a0, 0x100($t5)
    /* DE2614 00011BA8 24050001 */  addiu      $a1, $zero, 0x1
    /* DE2618 00011BAC 0C000000 */  jal        func_00000000
    /* DE261C 00011BB0 00000000 */   nop
    /* DE2620 00011BB4 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DE2624 00011BB8 25CF0001 */  addiu      $t7, $t6, 0x1
    /* DE2628 00011BBC AFAF0018 */  sw         $t7, 0x18($sp)
    /* DE262C 00011BC0 8FB80020 */  lw         $t8, 0x20($sp)
    /* DE2630 00011BC4 8F190124 */  lw         $t9, 0x124($t8)
    /* DE2634 00011BC8 01F9082A */  slt        $at, $t7, $t9
    /* DE2638 00011BCC 1420FFF1 */  bnez       $at, .L00011B94
    /* DE263C 00011BD0 00000000 */   nop
  .L00011BD4:
    /* DE2640 00011BD4 8FA2001C */  lw         $v0, 0x1C($sp)
    /* DE2644 00011BD8 10000001 */  b          .L00011BE0
    /* DE2648 00011BDC 00000000 */   nop
  .L00011BE0:
    /* DE264C 00011BE0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE2650 00011BE4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE2654 00011BE8 03E00008 */  jr         $ra
    /* DE2658 00011BEC 00000000 */   nop
endlabel input_pauseRacePads
