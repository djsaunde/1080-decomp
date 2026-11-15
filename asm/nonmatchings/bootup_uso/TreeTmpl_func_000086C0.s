nonmatching TreeTmpl_func_000086C0, 0x84

glabel TreeTmpl_func_000086C0
    /* DD912C 000086C0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD9130 000086C4 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD9134 000086C8 00808025 */  or         $s0, $a0, $zero
    /* DD9138 000086CC AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD913C 000086D0 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD9140 000086D4 24840000 */  addiu      $a0, $a0, 0x0
    /* DD9144 000086D8 0C000000 */  jal        func_00000000
    /* DD9148 000086DC 2605004C */   addiu     $a1, $s0, 0x4C
    /* DD914C 000086E0 0C000000 */  jal        func_00000000
    /* DD9150 000086E4 2604005C */   addiu     $a0, $s0, 0x5C
    /* DD9154 000086E8 8E020034 */  lw         $v0, 0x34($s0)
    /* DD9158 000086EC 1040000D */  beqz       $v0, .L00008724
    /* DD915C 000086F0 00408025 */   or        $s0, $v0, $zero
    /* DD9160 000086F4 8E0E0018 */  lw         $t6, 0x18($s0)
  .L000086F8:
    /* DD9164 000086F8 31CF0004 */  andi       $t7, $t6, 0x4
    /* DD9168 000086FC 51E00007 */  beql       $t7, $zero, .L0000871C
    /* DD916C 00008700 8E100084 */   lw        $s0, 0x84($s0)
    /* DD9170 00008704 8E020028 */  lw         $v0, 0x28($s0)
  alabel D_00008708
    /* DD9174 00008708 8C59001C */  lw         $t9, 0x1C($v0)
    /* DD9178 0000870C 84580018 */  lh         $t8, 0x18($v0)
  alabel D_00008710
    /* DD917C 00008710 0320F809 */  jalr       $t9
  alabel D_00008714
    /* DD9180 00008714 03102021 */   addu      $a0, $t8, $s0
    /* DD9184 00008718 8E100084 */  lw         $s0, 0x84($s0)
  .L0000871C:
    /* DD9188 0000871C 5600FFF6 */  bnel       $s0, $zero, .L000086F8
    /* DD918C 00008720 8E0E0018 */   lw        $t6, 0x18($s0)
  .L00008724:
    /* DD9190 00008724 3C040000 */  lui        $a0, %hi(func_0000027C + 0x18)
    /* DD9194 00008728 0C000000 */  jal        func_00000000
    /* DD9198 0000872C 24840294 */   addiu     $a0, $a0, %lo(func_0000027C + 0x18)
  alabel D_00008730
    /* DD919C 00008730 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD91A0 00008734 8FB00018 */  lw         $s0, 0x18($sp)
  alabel D_00008738
    /* DD91A4 00008738 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD91A8 0000873C 03E00008 */  jr         $ra
  alabel D_00008740
    /* DD91AC 00008740 00000000 */   nop
endlabel TreeTmpl_func_000086C0
