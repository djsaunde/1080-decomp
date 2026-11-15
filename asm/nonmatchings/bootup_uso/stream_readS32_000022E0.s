nonmatching stream_readS32_000022E0, 0x3C

glabel stream_readS32_000022E0
    /* DD2D4C 000022E0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2D50 000022E4 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2D54 000022E8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2D58 000022EC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD2D5C 000022F0 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2D60 000022F4 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD2D64 000022F8 0C000000 */  jal        func_00000000
    /* DD2D68 000022FC 24060004 */   addiu     $a2, $zero, 0x4
    /* DD2D6C 00002300 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD2D70 00002304 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD2D74 00002308 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD2D78 0000230C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2D7C 00002310 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD2D80 00002314 03E00008 */  jr         $ra
    /* DD2D84 00002318 00000000 */   nop
endlabel stream_readS32_000022E0
