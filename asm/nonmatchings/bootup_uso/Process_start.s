nonmatching Process_start, 0x98

glabel Process_start
    /* DDFEA0 0000F434 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDFEA4 0000F438 24850018 */  addiu      $a1, $a0, 0x18
    /* DDFEA8 0000F43C 00A03025 */  or         $a2, $a1, $zero
    /* DDFEAC 0000F440 24070001 */  addiu      $a3, $zero, 0x1
    /* DDFEB0 0000F444 10E00007 */  beqz       $a3, .L0000F464
    /* DDFEB4 0000F448 00000000 */   nop
    /* DDFEB8 0000F44C 00C04025 */  or         $t0, $a2, $zero
    /* DDFEBC 0000F450 8D0E0000 */  lw         $t6, 0x0($t0)
    /* DDFEC0 0000F454 35CF0004 */  ori        $t7, $t6, 0x4
    /* DDFEC4 0000F458 AD0F0000 */  sw         $t7, 0x0($t0)
    /* DDFEC8 0000F45C 10000006 */  b          .L0000F478
    /* DDFECC 0000F460 00000000 */   nop
  .L0000F464:
    /* DDFED0 0000F464 00C04025 */  or         $t0, $a2, $zero
    /* DDFED4 0000F468 8D180000 */  lw         $t8, 0x0($t0)
    /* DDFED8 0000F46C 2401FFFB */  addiu      $at, $zero, -0x5
    /* DDFEDC 0000F470 0301C824 */  and        $t9, $t8, $at
    /* DDFEE0 0000F474 AD190000 */  sw         $t9, 0x0($t0)
  .L0000F478:
    /* DDFEE4 0000F478 24850018 */  addiu      $a1, $a0, 0x18
    /* DDFEE8 0000F47C 00A03025 */  or         $a2, $a1, $zero
    /* DDFEEC 0000F480 24070001 */  addiu      $a3, $zero, 0x1
    /* DDFEF0 0000F484 10E00007 */  beqz       $a3, .L0000F4A4
    /* DDFEF4 0000F488 00000000 */   nop
    /* DDFEF8 0000F48C 00C04025 */  or         $t0, $a2, $zero
    /* DDFEFC 0000F490 8D0A0000 */  lw         $t2, 0x0($t0)
    /* DDFF00 0000F494 354B0008 */  ori        $t3, $t2, 0x8
    /* DDFF04 0000F498 AD0B0000 */  sw         $t3, 0x0($t0)
    /* DDFF08 0000F49C 10000006 */  b          .L0000F4B8
    /* DDFF0C 0000F4A0 00000000 */   nop
  .L0000F4A4:
    /* DDFF10 0000F4A4 00C04825 */  or         $t1, $a2, $zero
    /* DDFF14 0000F4A8 8D2C0000 */  lw         $t4, 0x0($t1)
    /* DDFF18 0000F4AC 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DDFF1C 0000F4B0 01816824 */  and        $t5, $t4, $at
    /* DDFF20 0000F4B4 AD2D0000 */  sw         $t5, 0x0($t1)
  .L0000F4B8:
    /* DDFF24 0000F4B8 10000001 */  b          .L0000F4C0
    /* DDFF28 0000F4BC 00000000 */   nop
  .L0000F4C0:
    /* DDFF2C 0000F4C0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDFF30 0000F4C4 03E00008 */  jr         $ra
    /* DDFF34 0000F4C8 00000000 */   nop
endlabel Process_start
