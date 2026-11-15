nonmatching boot_readIntParm_0000EBB8, 0x30

glabel boot_readIntParm_0000EBB8
    /* DDF624 0000EBB8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF628 0000EBBC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF62C 0000EBC0 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF630 0000EBC4 0C003A2F */  jal        stream_readS32_0000E8BC
    /* DDF634 0000EBC8 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF638 0000EBCC 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF63C 0000EBD0 0C003A2F */  jal        stream_readS32_0000E8BC
    /* DDF640 0000EBD4 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF644 0000EBD8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF648 0000EBDC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF64C 0000EBE0 03E00008 */  jr         $ra
    /* DDF650 0000EBE4 00000000 */   nop
endlabel boot_readIntParm_0000EBB8
