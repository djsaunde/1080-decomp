nonmatching input_ctor_0000F390, 0x44

glabel input_ctor_0000F390
    /* DDFDFC 0000F390 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDFE00 0000F394 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDFE04 0000F398 AFB00018 */  sw         $s0, 0x18($sp)
    /* DDFE08 0000F39C 3C100000 */  lui        $s0, (0x0 >> 16)
    /* DDFE0C 0000F3A0 26100000 */  addiu      $s0, $s0, 0x0
    /* DDFE10 0000F3A4 02002025 */  or         $a0, $s0, $zero
    /* DDFE14 0000F3A8 3C050001 */  lui        $a1, %hi(.L0000C574)
    /* DDFE18 0000F3AC 24A5C574 */  addiu      $a1, $a1, %lo(.L0000C574)
    /* DDFE1C 0000F3B0 0C000000 */  jal        func_00000000
    /* DDFE20 0000F3B4 00000000 */   nop
    /* DDFE24 0000F3B8 10000001 */  b          .L0000F3C0
    /* DDFE28 0000F3BC 00000000 */   nop
  .L0000F3C0:
    /* DDFE2C 0000F3C0 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDFE30 0000F3C4 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DDFE34 0000F3C8 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDFE38 0000F3CC 03E00008 */  jr         $ra
    /* DDFE3C 0000F3D0 00000000 */   nop
endlabel input_ctor_0000F390
