nonmatching PointsPopup_slideStep, 0xA4

glabel PointsPopup_slideStep
    /* DE4F60 000144F4 3C014180 */  lui        $at, (0x41800000 >> 16)
    /* DE4F64 000144F8 44812000 */  mtc1       $at, $f4
    /* DE4F68 000144FC C48000A8 */  lwc1       $f0, 0xA8($a0)
    /* DE4F6C 00014500 00001825 */  or         $v1, $zero, $zero
    /* DE4F70 00014504 3C013FE0 */  lui        $at, (0x3FE00000 >> 16)
    /* DE4F74 00014508 4604003C */  c.lt.s     $f0, $f4
    /* DE4F78 0001450C 00000000 */  nop
    /* DE4F7C 00014510 45020009 */  bc1fl      .L00014538
    /* DE4F80 00014514 C492005C */   lwc1      $f18, 0x5C($a0)
    /* DE4F84 00014518 44814800 */  mtc1       $at, $f9
    /* DE4F88 0001451C 44804000 */  mtc1       $zero, $f8
    /* DE4F8C 00014520 460001A1 */  cvt.d.s    $f6, $f0
    /* DE4F90 00014524 46283280 */  add.d      $f10, $f6, $f8
    /* DE4F94 00014528 46205420 */  cvt.s.d    $f16, $f10
    /* DE4F98 0001452C E49000A8 */  swc1       $f16, 0xA8($a0)
    /* DE4F9C 00014530 C48000A8 */  lwc1       $f0, 0xA8($a0)
    /* DE4FA0 00014534 C492005C */  lwc1       $f18, 0x5C($a0)
  .L00014538:
    /* DE4FA4 00014538 44853000 */  mtc1       $a1, $f6
    /* DE4FA8 0001453C 3C0140C0 */  lui        $at, (0x40C00000 >> 16)
    /* DE4FAC 00014540 46009100 */  add.s      $f4, $f18, $f0
    /* DE4FB0 00014544 46803220 */  cvt.s.w    $f8, $f6
    /* DE4FB4 00014548 E484005C */  swc1       $f4, 0x5C($a0)
    /* DE4FB8 0001454C C482005C */  lwc1       $f2, 0x5C($a0)
    /* DE4FBC 00014550 4602403C */  c.lt.s     $f8, $f2
    /* DE4FC0 00014554 00000000 */  nop
    /* DE4FC4 00014558 4500000D */  bc1f       .L00014590
    /* DE4FC8 0001455C 00000000 */   nop
    /* DE4FCC 00014560 44815000 */  mtc1       $at, $f10
    /* DE4FD0 00014564 3C014080 */  lui        $at, (0x40800000 >> 16)
    /* DE4FD4 00014568 44819000 */  mtc1       $at, $f18
    /* DE4FD8 0001456C 4600503C */  c.lt.s     $f10, $f0
    /* DE4FDC 00014570 46120103 */  div.s      $f4, $f0, $f18
    /* DE4FE0 00014574 45020003 */  bc1fl      .L00014584
    /* DE4FE4 00014578 46001401 */   sub.s     $f16, $f2, $f0
    /* DE4FE8 0001457C 24030001 */  addiu      $v1, $zero, 0x1
    /* DE4FEC 00014580 46001401 */  sub.s      $f16, $f2, $f0
  .L00014584:
    /* DE4FF0 00014584 46002187 */  neg.s      $f6, $f4
    /* DE4FF4 00014588 E490005C */  swc1       $f16, 0x5C($a0)
    /* DE4FF8 0001458C E48600A8 */  swc1       $f6, 0xA8($a0)
  .L00014590:
    /* DE4FFC 00014590 03E00008 */  jr         $ra
    /* DE5000 00014594 00601025 */   or        $v0, $v1, $zero
endlabel PointsPopup_slideStep
