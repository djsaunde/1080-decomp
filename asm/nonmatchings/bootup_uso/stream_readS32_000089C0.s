nonmatching stream_readS32_000089C0, 0x3C

glabel stream_readS32_000089C0
    /* DD942C 000089C0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD9430 000089C4 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD9434 000089C8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD9438 000089CC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD943C 000089D0 24840000 */  addiu      $a0, $a0, 0x0
    /* DD9440 000089D4 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD9444 000089D8 0C000000 */  jal        func_00000000
    /* DD9448 000089DC 24060004 */   addiu     $a2, $zero, 0x4
    /* DD944C 000089E0 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD9450 000089E4 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD9454 000089E8 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD9458 000089EC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD945C 000089F0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD9460 000089F4 03E00008 */  jr         $ra
    /* DD9464 000089F8 00000000 */   nop
endlabel stream_readS32_000089C0
