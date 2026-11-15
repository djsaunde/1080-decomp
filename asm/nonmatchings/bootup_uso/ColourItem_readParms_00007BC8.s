nonmatching ColourItem_readParms_00007BC8, 0x2C

glabel ColourItem_readParms_00007BC8
    /* DD8634 00007BC8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8638 00007BCC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD863C 00007BD0 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8640 00007BD4 0C0013FC */  jal        stream_readS32_00004FF0
    /* DD8644 00007BD8 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD8648 00007BDC 0C000000 */  jal        func_00000000
    /* DD864C 00007BE0 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8650 00007BE4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8654 00007BE8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8658 00007BEC 03E00008 */  jr         $ra
    /* DD865C 00007BF0 00000000 */   nop
endlabel ColourItem_readParms_00007BC8
