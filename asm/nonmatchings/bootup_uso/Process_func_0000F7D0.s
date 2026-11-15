nonmatching Process_func_0000F7D0, 0x24

glabel Process_func_0000F7D0
    /* DE023C 0000F7D0 AFA40000 */  sw         $a0, 0x0($sp)
    /* DE0240 0000F7D4 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DE0244 0000F7D8 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DE0248 0000F7DC 3C010000 */  lui        $at, %hi(func_00000008 + 0x28)
    /* DE024C 0000F7E0 AC2E0030 */  sw         $t6, %lo(func_00000008 + 0x28)($at)
    /* DE0250 0000F7E4 10000001 */  b          .L0000F7EC
    /* DE0254 0000F7E8 00000000 */   nop
  .L0000F7EC:
    /* DE0258 0000F7EC 03E00008 */  jr         $ra
    /* DE025C 0000F7F0 00000000 */   nop
endlabel Process_func_0000F7D0
