nonmatching Tree_handleMsg, 0x5C

glabel Tree_handleMsg
    /* DD90D0 00008664 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD90D4 00008668 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD90D8 0000866C AFA5001C */  sw         $a1, 0x1C($sp)
    /* DD90DC 00008670 0C000000 */  jal        func_00000000
    /* DD90E0 00008674 AFA40018 */   sw        $a0, 0x18($sp)
    /* DD90E4 00008678 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DD90E8 0000867C 24010007 */  addiu      $at, $zero, 0x7
    /* DD90EC 00008680 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD90F0 00008684 8DC20000 */  lw         $v0, 0x0($t6)
    /* DD90F4 00008688 10410003 */  beq        $v0, $at, .L00008698
    /* DD90F8 0000868C 24010013 */   addiu     $at, $zero, 0x13
    /* DD90FC 00008690 54410008 */  bnel       $v0, $at, .L000086B4
    /* DD9100 00008694 8FBF0014 */   lw        $ra, 0x14($sp)
  .L00008698:
    /* DD9104 00008698 C484005C */  lwc1       $f4, 0x5C($a0)
    /* DD9108 0000869C C4860060 */  lwc1       $f6, 0x60($a0)
    /* DD910C 000086A0 C4880064 */  lwc1       $f8, 0x64($a0)
    /* DD9110 000086A4 E48400A0 */  swc1       $f4, 0xA0($a0)
    /* DD9114 000086A8 E48600A4 */  swc1       $f6, 0xA4($a0)
    /* DD9118 000086AC E48800A8 */  swc1       $f8, 0xA8($a0)
    /* DD911C 000086B0 8FBF0014 */  lw         $ra, 0x14($sp)
  .L000086B4:
    /* DD9120 000086B4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD9124 000086B8 03E00008 */  jr         $ra
    /* DD9128 000086BC 00000000 */   nop
endlabel Tree_handleMsg
