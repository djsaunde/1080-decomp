nonmatching stream_readQuad_0000ED68, 0x58

glabel stream_readQuad_0000ED68
    /* DDF7D4 0000ED68 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDF7D8 0000ED6C AFA40028 */  sw         $a0, 0x28($sp)
    /* DDF7DC 0000ED70 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF7E0 0000ED74 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF7E4 0000ED78 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF7E8 0000ED7C 27A50018 */  addiu      $a1, $sp, 0x18
    /* DDF7EC 0000ED80 0C000000 */  jal        func_00000000
    /* DDF7F0 0000ED84 24060010 */   addiu     $a2, $zero, 0x10
    /* DDF7F4 0000ED88 27AF0018 */  addiu      $t7, $sp, 0x18
    /* DDF7F8 0000ED8C 8DF90000 */  lw         $t9, 0x0($t7)
    /* DDF7FC 0000ED90 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DDF800 0000ED94 ADD90000 */  sw         $t9, 0x0($t6)
    /* DDF804 0000ED98 8DF80004 */  lw         $t8, 0x4($t7)
    /* DDF808 0000ED9C ADD80004 */  sw         $t8, 0x4($t6)
    /* DDF80C 0000EDA0 8DF90008 */  lw         $t9, 0x8($t7)
    /* DDF810 0000EDA4 ADD90008 */  sw         $t9, 0x8($t6)
    /* DDF814 0000EDA8 8DF8000C */  lw         $t8, 0xC($t7)
    /* DDF818 0000EDAC ADD8000C */  sw         $t8, 0xC($t6)
    /* DDF81C 0000EDB0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF820 0000EDB4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDF824 0000EDB8 03E00008 */  jr         $ra
    /* DDF828 0000EDBC 00000000 */   nop
endlabel stream_readQuad_0000ED68
