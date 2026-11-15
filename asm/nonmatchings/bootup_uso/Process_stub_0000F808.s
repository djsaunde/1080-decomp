nonmatching Process_stub_0000F808, 0x14

glabel Process_stub_0000F808
    /* DE0274 0000F808 AFA40000 */  sw         $a0, 0x0($sp)
    /* DE0278 0000F80C 10000001 */  b          .L0000F814
    /* DE027C 0000F810 00000000 */   nop
  .L0000F814:
    /* DE0280 0000F814 03E00008 */  jr         $ra
    /* DE0284 0000F818 00000000 */   nop
endlabel Process_stub_0000F808
