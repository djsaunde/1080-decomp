nonmatching stream_read16_000089FC, 0x3C

glabel stream_read16_000089FC
    /* DD9468 000089FC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD946C 00008A00 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD9470 00008A04 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD9474 00008A08 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD9478 00008A0C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD947C 00008A10 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD9480 00008A14 0C000000 */  jal        func_00000000
    /* DD9484 00008A18 24060002 */   addiu     $a2, $zero, 0x2
    /* DD9488 00008A1C 87AE0018 */  lh         $t6, 0x18($sp)
    /* DD948C 00008A20 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD9490 00008A24 A5EE0000 */  sh         $t6, 0x0($t7)
    /* DD9494 00008A28 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD9498 00008A2C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD949C 00008A30 03E00008 */  jr         $ra
    /* DD94A0 00008A34 00000000 */   nop
endlabel stream_read16_000089FC
