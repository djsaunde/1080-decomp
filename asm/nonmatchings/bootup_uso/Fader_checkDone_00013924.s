nonmatching Fader_checkDone_00013924, 0x5C

glabel Fader_checkDone_00013924
    /* DE4390 00013924 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE4394 00013928 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE4398 0001392C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DE439C 00013930 44812000 */  mtc1       $at, $f4
    /* DE43A0 00013934 C4860048 */  lwc1       $f6, 0x48($a0)
    /* DE43A4 00013938 00001025 */  or         $v0, $zero, $zero
    /* DE43A8 0001393C 46062032 */  c.eq.s     $f4, $f6
    /* DE43AC 00013940 00000000 */  nop
    /* DE43B0 00013944 4500000A */  bc1f       .L00013970
    /* DE43B4 00013948 00000000 */   nop
    /* DE43B8 0001394C 8C8E0068 */  lw         $t6, 0x68($a0)
    /* DE43BC 00013950 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE43C0 00013954 24840000 */  addiu      $a0, $a0, 0x0
    /* DE43C4 00013958 15C00003 */  bnez       $t6, .L00013968
    /* DE43C8 0001395C 00000000 */   nop
    /* DE43CC 00013960 0C000000 */  jal        func_00000000
    /* DE43D0 00013964 24050001 */   addiu     $a1, $zero, 0x1
  .L00013968:
    /* DE43D4 00013968 10000001 */  b          .L00013970
    /* DE43D8 0001396C 24020001 */   addiu     $v0, $zero, 0x1
  .L00013970:
    /* DE43DC 00013970 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE43E0 00013974 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE43E4 00013978 03E00008 */  jr         $ra
    /* DE43E8 0001397C 00000000 */   nop
endlabel Fader_checkDone_00013924
