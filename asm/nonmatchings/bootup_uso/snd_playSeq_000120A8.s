nonmatching snd_playSeq_000120A8, 0x68

glabel snd_playSeq_000120A8
    /* DE2B14 000120A8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE2B18 000120AC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE2B1C 000120B0 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE2B20 000120B4 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE2B24 000120B8 AFA60020 */  sw         $a2, 0x20($sp)
    /* DE2B28 000120BC AFA70024 */  sw         $a3, 0x24($sp)
    /* DE2B2C 000120C0 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DE2B30 000120C4 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE2B34 000120C8 ADEE013C */  sw         $t6, 0x13C($t7)
    /* DE2B38 000120CC 8FB80020 */  lw         $t8, 0x20($sp)
    /* DE2B3C 000120D0 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE2B40 000120D4 AF380144 */  sw         $t8, 0x144($t9)
    /* DE2B44 000120D8 8FA80018 */  lw         $t0, 0x18($sp)
    /* DE2B48 000120DC 8D04013C */  lw         $a0, 0x13C($t0)
    /* DE2B4C 000120E0 8D050144 */  lw         $a1, 0x144($t0)
    /* DE2B50 000120E4 00003025 */  or         $a2, $zero, $zero
    /* DE2B54 000120E8 0C000000 */  jal        func_00000000
    /* DE2B58 000120EC 00000000 */   nop
    /* DE2B5C 000120F0 8FA90018 */  lw         $t1, 0x18($sp)
    /* DE2B60 000120F4 AD200144 */  sw         $zero, 0x144($t1)
    /* DE2B64 000120F8 10000001 */  b          .L00012100
    /* DE2B68 000120FC 00000000 */   nop
  .L00012100:
    /* DE2B6C 00012100 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE2B70 00012104 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE2B74 00012108 03E00008 */  jr         $ra
    /* DE2B78 0001210C 00000000 */   nop
endlabel snd_playSeq_000120A8
