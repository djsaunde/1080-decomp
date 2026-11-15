nonmatching func_00000148, 0x40

glabel func_00000148
    /* DD0BB4 00000148 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0BB8 0000014C AFA40018 */  sw         $a0, 0x18($sp)
    /* DD0BBC 00000150 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0BC0 00000154 3C040000 */  lui        $a0, %hi(sStr_Poop_start_6538)
    /* DD0BC4 00000158 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DD0BC8 0000015C AFA60020 */  sw         $a2, 0x20($sp)
    /* DD0BCC 00000160 0C000000 */  jal        func_00000000
    /* DD0BD0 00000164 24846538 */   addiu     $a0, $a0, %lo(sStr_Poop_start_6538)
    /* DD0BD4 00000168 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD0BD8 0000016C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD0BDC 00000170 0C000000 */  jal        func_00000000
    /* DD0BE0 00000174 8FA60020 */   lw        $a2, 0x20($sp)
    /* DD0BE4 00000178 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0BE8 0000017C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0BEC 00000180 03E00008 */  jr         $ra
    /* DD0BF0 00000184 00000000 */   nop
endlabel func_00000148
