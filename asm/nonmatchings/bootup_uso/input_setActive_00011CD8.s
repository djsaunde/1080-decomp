nonmatching input_setActive_00011CD8, 0x68

glabel input_setActive_00011CD8
    /* DE2744 00011CD8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE2748 00011CDC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE274C 00011CE0 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE2750 00011CE4 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE2754 00011CE8 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DE2758 00011CEC 11C00008 */  beqz       $t6, .L00011D10
    /* DE275C 00011CF0 00000000 */   nop
    /* DE2760 00011CF4 8FA40018 */  lw         $a0, 0x18($sp)
    /* DE2764 00011CF8 0C000000 */  jal        func_00000000
    /* DE2768 00011CFC 00000000 */   nop
    /* DE276C 00011D00 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE2770 00011D04 ADE00188 */  sw         $zero, 0x188($t7)
    /* DE2774 00011D08 10000007 */  b          .L00011D28
    /* DE2778 00011D0C 00000000 */   nop
  .L00011D10:
    /* DE277C 00011D10 8FA40018 */  lw         $a0, 0x18($sp)
    /* DE2780 00011D14 0C000000 */  jal        func_00000000
    /* DE2784 00011D18 00000000 */   nop
    /* DE2788 00011D1C 24180001 */  addiu      $t8, $zero, 0x1
    /* DE278C 00011D20 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE2790 00011D24 AF380188 */  sw         $t8, 0x188($t9)
  .L00011D28:
    /* DE2794 00011D28 10000001 */  b          .L00011D30
    /* DE2798 00011D2C 00000000 */   nop
  .L00011D30:
    /* DE279C 00011D30 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE27A0 00011D34 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE27A4 00011D38 03E00008 */  jr         $ra
    /* DE27A8 00011D3C 00000000 */   nop
endlabel input_setActive_00011CD8
