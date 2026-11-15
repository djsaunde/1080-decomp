nonmatching stream_readS32_00001C10, 0x3C

glabel stream_readS32_00001C10
    /* DD267C 00001C10 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD2680 00001C14 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD2684 00001C18 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2688 00001C1C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD268C 00001C20 24840000 */  addiu      $a0, $a0, 0x0
    /* DD2690 00001C24 27A50018 */  addiu      $a1, $sp, 0x18
    /* DD2694 00001C28 0C000000 */  jal        func_00000000
    /* DD2698 00001C2C 24060004 */   addiu     $a2, $zero, 0x4
    /* DD269C 00001C30 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DD26A0 00001C34 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DD26A4 00001C38 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DD26A8 00001C3C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD26AC 00001C40 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD26B0 00001C44 03E00008 */  jr         $ra
    /* DD26B4 00001C48 00000000 */   nop
endlabel stream_readS32_00001C10
