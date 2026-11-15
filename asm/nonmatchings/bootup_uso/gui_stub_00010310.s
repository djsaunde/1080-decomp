nonmatching gui_stub_00010310, 0x14

glabel gui_stub_00010310
    /* DE0D7C 00010310 AFA40000 */  sw         $a0, 0x0($sp)
    /* DE0D80 00010314 10000001 */  b          .L0001031C
    /* DE0D84 00010318 00000000 */   nop
  .L0001031C:
    /* DE0D88 0001031C 03E00008 */  jr         $ra
    /* DE0D8C 00010320 00000000 */   nop
endlabel gui_stub_00010310
