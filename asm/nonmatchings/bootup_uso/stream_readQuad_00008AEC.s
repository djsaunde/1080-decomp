nonmatching stream_readQuad_00008AEC, 0x58

glabel stream_readQuad_00008AEC
    /* DD9558 00008AEC 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD955C 00008AF0 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD9560 00008AF4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD9564 00008AF8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD9568 00008AFC 24840000 */  addiu      $a0, $a0, 0x0
    /* DD956C 00008B00 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD9570 00008B04 0C000000 */  jal        func_00000000
    /* DD9574 00008B08 24060010 */   addiu     $a2, $zero, 0x10
    /* DD9578 00008B0C 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DD957C 00008B10 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD9580 00008B14 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DD9584 00008B18 ADD90000 */  sw         $t9, 0x0($t6)
    /* DD9588 00008B1C 8DF80004 */  lw         $t8, 0x4($t7)
    /* DD958C 00008B20 ADD80004 */  sw         $t8, 0x4($t6)
    /* DD9590 00008B24 8DF90008 */  lw         $t9, 0x8($t7)
    /* DD9594 00008B28 ADD90008 */  sw         $t9, 0x8($t6)
    /* DD9598 00008B2C 8DF8000C */  lw         $t8, 0xC($t7)
    /* DD959C 00008B30 ADD8000C */  sw         $t8, 0xC($t6)
    /* DD95A0 00008B34 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD95A4 00008B38 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD95A8 00008B3C 03E00008 */  jr         $ra
    /* DD95AC 00008B40 00000000 */   nop
endlabel stream_readQuad_00008AEC
