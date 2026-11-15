nonmatching QuitProcess_stub_00012DA4, 0x14

glabel QuitProcess_stub_00012DA4
    /* DE3810 00012DA4 AFA40000 */  sw         $a0, 0x0($sp)
    /* DE3814 00012DA8 10000001 */  b          .L00012DB0
    /* DE3818 00012DAC 00000000 */   nop
  .L00012DB0:
    /* DE381C 00012DB0 03E00008 */  jr         $ra
    /* DE3820 00012DB4 00000000 */   nop
endlabel QuitProcess_stub_00012DA4
