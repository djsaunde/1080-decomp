nonmatching stream_readS32_00005EF8, 0x3C

glabel stream_readS32_00005EF8
    /* DD6964 00005EF8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD6968 00005EFC AFA40020 */  sw         $a0, 0x20($sp)
    /* DD696C 00005F00 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD6970 00005F04 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD6974 00005F08 24840000 */  addiu      $a0, $a0, 0x0
    /* DD6978 00005F0C 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD697C 00005F10 0C000000 */  jal        func_00000000
    /* DD6980 00005F14 24060004 */   addiu     $a2, $zero, 0x4
    /* DD6984 00005F18 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD6988 00005F1C 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD698C 00005F20 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD6990 00005F24 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD6994 00005F28 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD6998 00005F2C 03E00008 */  jr         $ra
    /* DD699C 00005F30 00000000 */   nop
endlabel stream_readS32_00005EF8
