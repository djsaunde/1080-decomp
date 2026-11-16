nonmatching SnowBoard_onPreNmi, 0x30

glabel SnowBoard_onPreNmi
    /* DDF564 0000EAF8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDF568 0000EAFC AFA40018 */  sw         $a0, 0x18($sp)
    /* DDF56C 0000EB00 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF570 0000EB04 3C040000 */  lui        $a0, %hi(snd_seqEndDbg_00000188 + 0x8)
    /* DDF574 0000EB08 8C840190 */  lw         $a0, %lo(snd_seqEndDbg_00000188 + 0x8)($a0)
    /* DDF578 0000EB0C 24050003 */  addiu      $a1, $zero, 0x3
    /* DDF57C 0000EB10 0C000000 */  jal        func_00000000
    /* DDF580 0000EB14 00003025 */   or        $a2, $zero, $zero
    /* DDF584 0000EB18 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF588 0000EB1C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDF58C 0000EB20 03E00008 */  jr         $ra
    /* DDF590 0000EB24 00000000 */   nop
endlabel SnowBoard_onPreNmi
