nonmatching CamDirector_setActiveCam, 0x4C

glabel CamDirector_setActiveCam
    /* DD31E0 00002774 8C8E0018 */  lw         $t6, 0x18($a0)
    /* DD31E4 00002778 0005C080 */  sll        $t8, $a1, 2
    /* DD31E8 0000277C 0098C821 */  addu       $t9, $a0, $t8
    /* DD31EC 00002780 31CF0008 */  andi       $t7, $t6, 0x8
    /* DD31F0 00002784 11E0000C */  beqz       $t7, .L000027B8
    /* DD31F4 00002788 00000000 */   nop
    /* DD31F8 0000278C AC85003C */  sw         $a1, 0x3C($a0)
    /* DD31FC 00002790 8F280030 */  lw         $t0, 0x30($t9)
    /* DD3200 00002794 8C89002C */  lw         $t1, 0x2C($a0)
    /* DD3204 00002798 44800000 */  mtc1       $zero, $f0
    /* DD3208 0000279C AD280070 */  sw         $t0, 0x70($t1)
    /* DD320C 000027A0 8C8A002C */  lw         $t2, 0x2C($a0)
    /* DD3210 000027A4 8D420070 */  lw         $v0, 0x70($t2)
    /* DD3214 000027A8 24420134 */  addiu      $v0, $v0, 0x134
    /* DD3218 000027AC E4400008 */  swc1       $f0, 0x8($v0)
    /* DD321C 000027B0 E4400004 */  swc1       $f0, 0x4($v0)
    /* DD3220 000027B4 E4400000 */  swc1       $f0, 0x0($v0)
  .L000027B8:
    /* DD3224 000027B8 03E00008 */  jr         $ra
    /* DD3228 000027BC 00000000 */   nop
endlabel CamDirector_setActiveCam
