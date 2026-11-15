nonmatching stream_readS32_0000EF20, 0x3C

glabel stream_readS32_0000EF20
    /* DDF98C 0000EF20 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF990 0000EF24 AFA40020 */  sw         $a0, 0x20($sp)
    /* DDF994 0000EF28 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF998 0000EF2C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF99C 0000EF30 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF9A0 0000EF34 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF9A4 0000EF38 0C000000 */  jal        func_00000000
    /* DDF9A8 0000EF3C 24060004 */   addiu     $a2, $zero, 0x4
    /* DDF9AC 0000EF40 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DDF9B0 0000EF44 8FAF0020 */  lw         $t7, 0x20($sp)
    /* DDF9B4 0000EF48 ADEE0000 */  sw         $t6, 0x0($t7)
    /* DDF9B8 0000EF4C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF9BC 0000EF50 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF9C0 0000EF54 03E00008 */  jr         $ra
    /* DDF9C4 0000EF58 00000000 */   nop
endlabel stream_readS32_0000EF20
