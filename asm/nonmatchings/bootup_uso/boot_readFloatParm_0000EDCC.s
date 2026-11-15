nonmatching boot_readFloatParm_0000EDCC, 0x30

glabel boot_readFloatParm_0000EDCC
    /* DDF838 0000EDCC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF83C 0000EDD0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF840 0000EDD4 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF844 0000EDD8 0C003B20 */  jal        stream_readS32_0000EC80
    /* DDF848 0000EDDC 27A4001C */   addiu     $a0, $sp, 0x1C
    /* DDF84C 0000EDE0 8FA40020 */  lw         $a0, 0x20($sp)
    /* DDF850 0000EDE4 0C003B2F */  jal        stream_readF32_0000ECBC
    /* DDF854 0000EDE8 24840010 */   addiu     $a0, $a0, 0x10
    /* DDF858 0000EDEC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF85C 0000EDF0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF860 0000EDF4 03E00008 */  jr         $ra
    /* DDF864 0000EDF8 00000000 */   nop
endlabel boot_readFloatParm_0000EDCC
