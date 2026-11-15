nonmatching snd_addSoundHeap, 0x2C

glabel snd_addSoundHeap
    /* DD0DCC 00000360 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0DD0 00000364 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD0DD4 00000368 0C000000 */  jal        func_00000000
    /* DD0DD8 0000036C 00000000 */   nop
    /* DD0DDC 00000370 3C040000 */  lui        $a0, %hi(sStr_Poop_Added_sound_heap_6550)
    /* DD0DE0 00000374 0C000000 */  jal        func_00000000
    /* DD0DE4 00000378 24846550 */   addiu     $a0, $a0, %lo(sStr_Poop_Added_sound_heap_6550)
    /* DD0DE8 0000037C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD0DEC 00000380 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0DF0 00000384 03E00008 */  jr         $ra
    /* DD0DF4 00000388 00000000 */   nop
endlabel snd_addSoundHeap
