nonmatching stream_readS32_0000502C, 0x3C

glabel stream_readS32_0000502C
    /* DD5A98 0000502C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD5A9C 00005030 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD5AA0 00005034 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5AA4 00005038 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD5AA8 0000503C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD5AAC 00005040 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD5AB0 00005044 0C000000 */  jal        func_00000000
    /* DD5AB4 00005048 24060004 */   addiu     $a2, $zero, 0x4
    /* DD5AB8 0000504C 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD5ABC 00005050 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD5AC0 00005054 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD5AC4 00005058 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5AC8 0000505C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD5ACC 00005060 03E00008 */  jr         $ra
    /* DD5AD0 00005064 00000000 */   nop
endlabel stream_readS32_0000502C
