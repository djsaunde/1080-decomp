nonmatching input_initDefaults_00012188, 0xBC

glabel input_initDefaults_00012188
    /* DE2BF4 00012188 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* DE2BF8 0001218C AC800068 */  sw         $zero, 0x68($a0)
    /* DE2BFC 00012190 240E0001 */  addiu      $t6, $zero, 0x1
    /* DE2C00 00012194 AC8E004C */  sw         $t6, 0x4C($a0)
    /* DE2C04 00012198 AC800050 */  sw         $zero, 0x50($a0)
    /* DE2C08 0001219C 240F0001 */  addiu      $t7, $zero, 0x1
    /* DE2C0C 000121A0 AC8F0054 */  sw         $t7, 0x54($a0)
    /* DE2C10 000121A4 24180001 */  addiu      $t8, $zero, 0x1
    /* DE2C14 000121A8 AC980058 */  sw         $t8, 0x58($a0)
    /* DE2C18 000121AC AC80005C */  sw         $zero, 0x5C($a0)
    /* DE2C1C 000121B0 24190001 */  addiu      $t9, $zero, 0x1
    /* DE2C20 000121B4 AC990060 */  sw         $t9, 0x60($a0)
    /* DE2C24 000121B8 24080002 */  addiu      $t0, $zero, 0x2
    /* DE2C28 000121BC AC880064 */  sw         $t0, 0x64($a0)
    /* DE2C2C 000121C0 AC800080 */  sw         $zero, 0x80($a0)
    /* DE2C30 000121C4 AFA00004 */  sw         $zero, 0x4($sp)
  .L000121C8:
    /* DE2C34 000121C8 2409002A */  addiu      $t1, $zero, 0x2A
    /* DE2C38 000121CC 8FAA0004 */  lw         $t2, 0x4($sp)
    /* DE2C3C 000121D0 008A5821 */  addu       $t3, $a0, $t2
    /* DE2C40 000121D4 A1690158 */  sb         $t1, 0x158($t3)
    /* DE2C44 000121D8 8FAC0004 */  lw         $t4, 0x4($sp)
    /* DE2C48 000121DC 258D0001 */  addiu      $t5, $t4, 0x1
    /* DE2C4C 000121E0 AFAD0004 */  sw         $t5, 0x4($sp)
    /* DE2C50 000121E4 29A10003 */  slti       $at, $t5, 0x3
    /* DE2C54 000121E8 1420FFF7 */  bnez       $at, .L000121C8
    /* DE2C58 000121EC 00000000 */   nop
    /* DE2C5C 000121F0 AC80015C */  sw         $zero, 0x15C($a0)
    /* DE2C60 000121F4 24050004 */  addiu      $a1, $zero, 0x4
    /* DE2C64 000121F8 AC850090 */  sw         $a1, 0x90($a0)
    /* DE2C68 000121FC AC8500B4 */  sw         $a1, 0xB4($a0)
    /* DE2C6C 00012200 24050003 */  addiu      $a1, $zero, 0x3
    /* DE2C70 00012204 AC850094 */  sw         $a1, 0x94($a0)
    /* DE2C74 00012208 AC8500B0 */  sw         $a1, 0xB0($a0)
    /* DE2C78 0001220C 24050001 */  addiu      $a1, $zero, 0x1
    /* DE2C7C 00012210 AC850098 */  sw         $a1, 0x98($a0)
    /* DE2C80 00012214 AC8500A8 */  sw         $a1, 0xA8($a0)
    /* DE2C84 00012218 24050002 */  addiu      $a1, $zero, 0x2
    /* DE2C88 0001221C AC85009C */  sw         $a1, 0x9C($a0)
    /* DE2C8C 00012220 AC8500AC */  sw         $a1, 0xAC($a0)
    /* DE2C90 00012224 24050005 */  addiu      $a1, $zero, 0x5
    /* DE2C94 00012228 AC8500A0 */  sw         $a1, 0xA0($a0)
    /* DE2C98 0001222C AC8500A4 */  sw         $a1, 0xA4($a0)
    /* DE2C9C 00012230 10000001 */  b          .L00012238
    /* DE2CA0 00012234 00000000 */   nop
  .L00012238:
    /* DE2CA4 00012238 27BD0008 */  addiu      $sp, $sp, 0x8
    /* DE2CA8 0001223C 03E00008 */  jr         $ra
    /* DE2CAC 00012240 00000000 */   nop
endlabel input_initDefaults_00012188
