nonmatching Process_stop, 0x98

glabel Process_stop
    /* DDFF38 0000F4CC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDFF3C 0000F4D0 24850018 */  addiu      $a1, $a0, 0x18
    /* DDFF40 0000F4D4 00A03025 */  or         $a2, $a1, $zero
    /* DDFF44 0000F4D8 00003825 */  or         $a3, $zero, $zero
    /* DDFF48 0000F4DC 10E00007 */  beqz       $a3, .L0000F4FC
    /* DDFF4C 0000F4E0 00000000 */   nop
    /* DDFF50 0000F4E4 00C04025 */  or         $t0, $a2, $zero
    /* DDFF54 0000F4E8 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DDFF58 0000F4EC 35CF0004 */  ori        $t7, $t6, 0x4
    /* DDFF5C 0000F4F0 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DDFF60 0000F4F4 10000006 */  b          .L0000F510
    /* DDFF64 0000F4F8 00000000 */   nop
  .L0000F4FC:
    /* DDFF68 0000F4FC 00C04025 */  or         $t0, $a2, $zero
    /* DDFF6C 0000F500 8D180000 */  lw         $t8, 0x0($t0)
    /* DDFF70 0000F504 2401FFFB */  addiu      $at, $zero, -0x5
    /* DDFF74 0000F508 0301C824 */  and        $t9, $t8, $at
    /* DDFF78 0000F50C AD190000 */  sw         $t9, 0x0($t0)
  .L0000F510:
    /* DDFF7C 0000F510 24850018 */  addiu      $a1, $a0, 0x18
    /* DDFF80 0000F514 00A03025 */  or         $a2, $a1, $zero
    /* DDFF84 0000F518 00003825 */  or         $a3, $zero, $zero
    /* DDFF88 0000F51C 10E00007 */  beqz       $a3, .L0000F53C
    /* DDFF8C 0000F520 00000000 */   nop
    /* DDFF90 0000F524 00C04025 */  or         $t0, $a2, $zero
    /* DDFF94 0000F528 8D0A0000 */  lw         $t2, 0x0($t0)
    /* DDFF98 0000F52C 354B0008 */  ori        $t3, $t2, 0x8
    /* DDFF9C 0000F530 AD0B0000 */  sw         $t3, 0x0($t0)
    /* DDFFA0 0000F534 10000006 */  b          .L0000F550
    /* DDFFA4 0000F538 00000000 */   nop
  .L0000F53C:
    /* DDFFA8 0000F53C 00C04825 */  or         $t1, $a2, $zero
    /* DDFFAC 0000F540 8D2C0000 */  lw         $t4, 0x0($t1)
    /* DDFFB0 0000F544 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DDFFB4 0000F548 01816824 */  and        $t5, $t4, $at
    /* DDFFB8 0000F54C AD2D0000 */  sw         $t5, 0x0($t1)
  .L0000F550:
    /* DDFFBC 0000F550 10000001 */  b          .L0000F558
    /* DDFFC0 0000F554 00000000 */   nop
  .L0000F558:
    /* DDFFC4 0000F558 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDFFC8 0000F55C 03E00008 */  jr         $ra
    /* DDFFCC 0000F560 00000000 */   nop
endlabel Process_stop
