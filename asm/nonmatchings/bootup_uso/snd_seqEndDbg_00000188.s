nonmatching snd_seqEndDbg_00000188, 0x40

glabel snd_seqEndDbg_00000188
    /* DD0BF4 00000188 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0BF8 0000018C AFA40018 */  sw         $a0, 0x18($sp)
    /* DD0BFC 00000190 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0C00 00000194 3C040000 */  lui        $a0, %hi(sStr_Poop_end_6544)
    /* DD0C04 00000198 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DD0C08 0000019C AFA60020 */  sw         $a2, 0x20($sp)
    /* DD0C0C 000001A0 0C000000 */  jal        func_00000000
    /* DD0C10 000001A4 24846544 */   addiu     $a0, $a0, %lo(sStr_Poop_end_6544)
    /* DD0C14 000001A8 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD0C18 000001AC 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD0C1C 000001B0 0C000000 */  jal        func_00000000
    /* DD0C20 000001B4 8FA60020 */   lw        $a2, 0x20($sp)
    /* DD0C24 000001B8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0C28 000001BC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0C2C 000001C0 03E00008 */  jr         $ra
    /* DD0C30 000001C4 00000000 */   nop
endlabel snd_seqEndDbg_00000188
