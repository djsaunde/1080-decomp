nonmatching snd_removeSoundHeap_0000038C, 0x2C

glabel snd_removeSoundHeap_0000038C
    /* DD0DF8 0000038C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0DFC 00000390 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0E00 00000394 0C000000 */  jal        func_00000000
    /* DD0E04 00000398 00000000 */   nop
    /* DD0E08 0000039C 3C040000 */  lui        $a0, %hi(D_00006568)
    /* DD0E0C 000003A0 0C000000 */  jal        func_00000000
    /* DD0E10 000003A4 24846568 */   addiu     $a0, $a0, %lo(D_00006568)
    /* DD0E14 000003A8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0E18 000003AC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0E1C 000003B0 03E00008 */  jr         $ra
    /* DD0E20 000003B4 00000000 */   nop
endlabel snd_removeSoundHeap_0000038C
