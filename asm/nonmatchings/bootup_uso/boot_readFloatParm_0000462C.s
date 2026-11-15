nonmatching boot_readFloatParm_0000462C, 0x30

glabel boot_readFloatParm_0000462C
    /* DD5098 0000462C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD509C 00004630 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD50A0 00004634 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD50A4 00004638 0C0008B8 */  jal        stream_readS32_000022E0
    /* DD50A8 0000463C 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DD50AC 00004640 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD50B0 00004644 0C0008C7 */  jal        stream_readF32_0000231C
    /* DD50B4 00004648 24840010 */   addiu     $a0, $a0, 0x10
    /* DD50B8 0000464C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD50BC 00004650 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD50C0 00004654 03E00008 */  jr         $ra
    /* DD50C4 00004658 00000000 */   nop
endlabel boot_readFloatParm_0000462C
