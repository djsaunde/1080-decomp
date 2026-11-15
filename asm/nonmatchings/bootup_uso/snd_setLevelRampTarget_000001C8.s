nonmatching snd_setLevelRampTarget_000001C8, 0x34

glabel snd_setLevelRampTarget_000001C8
    /* DD0C34 000001C8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD0C38 000001CC 10800005 */  beqz       $a0, .L000001E4
    /* DD0C3C 000001D0 AFBF0014 */   sw        $ra, 0x14($sp)
    /* DD0C40 000001D4 0C000000 */  jal        func_00000000
    /* DD0C44 000001D8 24040002 */   addiu     $a0, $zero, 0x2
    /* DD0C48 000001DC 10000004 */  b          .L000001F0
    /* DD0C4C 000001E0 8FBF0014 */   lw        $ra, 0x14($sp)
  .L000001E4:
    /* DD0C50 000001E4 0C000000 */  jal        func_00000000
    /* DD0C54 000001E8 00002025 */   or        $a0, $zero, $zero
    /* DD0C58 000001EC 8FBF0014 */  lw         $ra, 0x14($sp)
  .L000001F0:
    /* DD0C5C 000001F0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD0C60 000001F4 03E00008 */  jr         $ra
    /* DD0C64 000001F8 00000000 */   nop
endlabel snd_setLevelRampTarget_000001C8
