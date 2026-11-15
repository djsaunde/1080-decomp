nonmatching stream_readS32_00013C70, 0x3C

glabel stream_readS32_00013C70
    /* DE46DC 00013C70 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DE46E0 00013C74 AFA40020 */  sw         $a0, 0x20($sp)
    /* DE46E4 00013C78 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE46E8 00013C7C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE46EC 00013C80 24840000 */  addiu      $a0, $a0, 0x0
    /* DE46F0 00013C84 27A50018 */  addiu      $a1, $sp, 0x18
    /* DE46F4 00013C88 0C000000 */  jal        func_00000000
    /* DE46F8 00013C8C 24060004 */   addiu     $a2, $zero, 0x4
    /* DE46FC 00013C90 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DE4700 00013C94 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DE4704 00013C98 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DE4708 00013C9C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE470C 00013CA0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DE4710 00013CA4 03E00008 */  jr         $ra
    /* DE4714 00013CA8 00000000 */   nop
endlabel stream_readS32_00013C70
