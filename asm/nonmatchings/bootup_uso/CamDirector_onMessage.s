nonmatching CamDirector_onMessage, 0x28

glabel CamDirector_onMessage
    /* DD322C 000027C0 8CA20000 */  lw         $v0, 0x0($a1)
    /* DD3230 000027C4 240103ED */  addiu      $at, $zero, 0x3ED
    /* DD3234 000027C8 14410005 */  bne        $v0, $at, .L000027E0
    /* DD3238 000027CC 24820018 */   addiu     $v0, $a0, 0x18
    /* DD323C 000027D0 8C4E0000 */  lw         $t6, 0x0($v0)
    /* DD3240 000027D4 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DD3244 000027D8 01C17824 */  and        $t7, $t6, $at
    /* DD3248 000027DC AC4F0000 */  sw         $t7, 0x0($v0)
  .L000027E0:
    /* DD324C 000027E0 03E00008 */  jr         $ra
    /* DD3250 000027E4 00000000 */   nop
endlabel CamDirector_onMessage
