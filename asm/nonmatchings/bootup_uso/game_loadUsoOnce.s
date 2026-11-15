nonmatching game_loadUsoOnce, 0x40

glabel game_loadUsoOnce
    /* DD93EC 00008980 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD93F0 00008984 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD93F4 00008988 0C000000 */  jal        func_00000000
    /* DD93F8 0000898C AFA40018 */   sw        $a0, 0x18($sp)
    /* DD93FC 00008990 14400006 */  bnez       $v0, .L000089AC
    /* DD9400 00008994 3C040000 */   lui       $a0, (0x0 >> 16)
    /* DD9404 00008998 24840000 */  addiu      $a0, $a0, 0x0
    /* DD9408 0000899C 0C000000 */  jal        func_00000000
    /* DD940C 000089A0 8FA50018 */   lw        $a1, 0x18($sp)
    /* DD9410 000089A4 10000002 */  b          .L000089B0
    /* DD9414 000089A8 24020001 */   addiu     $v0, $zero, 0x1
  .L000089AC:
    /* DD9418 000089AC 00001025 */  or         $v0, $zero, $zero
  .L000089B0:
    /* DD941C 000089B0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD9420 000089B4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD9424 000089B8 03E00008 */  jr         $ra
    /* DD9428 000089BC 00000000 */   nop
endlabel game_loadUsoOnce
