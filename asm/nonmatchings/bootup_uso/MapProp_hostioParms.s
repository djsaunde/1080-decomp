nonmatching MapProp_hostioParms, 0xD4

glabel MapProp_hostioParms
    /* DD71A0 00006734 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD71A4 00006738 AFA40020 */  sw         $a0, 0x20($sp)
    /* DD71A8 0000673C AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD71AC 00006740 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD71B0 00006744 3C050000 */  lui        $a1, %hi(sStr_Map_7EA4)
    /* DD71B4 00006748 24A57EA4 */  addiu      $a1, $a1, %lo(sStr_Map_7EA4)
    /* DD71B8 0000674C 24840000 */  addiu      $a0, $a0, 0x0
    /* DD71BC 00006750 0C000000 */  jal        func_00000000
    /* DD71C0 00006754 00003025 */   or        $a2, $zero, $zero
    /* DD71C4 00006758 8FA60020 */  lw         $a2, 0x20($sp)
    /* DD71C8 0000675C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD71CC 00006760 3C050000 */  lui        $a1, %hi(sStr_end_7EA8)
    /* DD71D0 00006764 240E007F */  addiu      $t6, $zero, 0x7F
    /* DD71D4 00006768 240F0001 */  addiu      $t7, $zero, 0x1
    /* DD71D8 0000676C AFAF0014 */  sw         $t7, 0x14($sp)
    /* DD71DC 00006770 AFAE0010 */  sw         $t6, 0x10($sp)
    /* DD71E0 00006774 24A57EA8 */  addiu      $a1, $a1, %lo(sStr_end_7EA8)
    /* DD71E4 00006778 24840000 */  addiu      $a0, $a0, 0x0
    /* DD71E8 0000677C 00003825 */  or         $a3, $zero, $zero
    /* DD71EC 00006780 0C000000 */  jal        func_00000000
    /* DD71F0 00006784 24C600B8 */   addiu     $a2, $a2, 0xB8
    /* DD71F4 00006788 8FA60020 */  lw         $a2, 0x20($sp)
    /* DD71F8 0000678C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD71FC 00006790 3C050000 */  lui        $a1, %hi(sStr_add_7EAC)
    /* DD7200 00006794 2418007F */  addiu      $t8, $zero, 0x7F
    /* DD7204 00006798 24190001 */  addiu      $t9, $zero, 0x1
    /* DD7208 0000679C AFB90014 */  sw         $t9, 0x14($sp)
    /* DD720C 000067A0 AFB80010 */  sw         $t8, 0x10($sp)
    /* DD7210 000067A4 24A57EAC */  addiu      $a1, $a1, %lo(sStr_add_7EAC)
    /* DD7214 000067A8 24840000 */  addiu      $a0, $a0, 0x0
    /* DD7218 000067AC 00003825 */  or         $a3, $zero, $zero
    /* DD721C 000067B0 0C000000 */  jal        func_00000000
    /* DD7220 000067B4 24C600C0 */   addiu     $a2, $a2, 0xC0
    /* DD7224 000067B8 8FA60020 */  lw         $a2, 0x20($sp)
    /* DD7228 000067BC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD722C 000067C0 3C050000 */  lui        $a1, %hi(sStr_angle_7EB0)
    /* DD7230 000067C4 24080059 */  addiu      $t0, $zero, 0x59
    /* DD7234 000067C8 AFA80010 */  sw         $t0, 0x10($sp)
    /* DD7238 000067CC 24A57EB0 */  addiu      $a1, $a1, %lo(sStr_angle_7EB0)
    /* DD723C 000067D0 24840000 */  addiu      $a0, $a0, 0x0
    /* DD7240 000067D4 00003825 */  or         $a3, $zero, $zero
    /* DD7244 000067D8 AFA00014 */  sw         $zero, 0x14($sp)
    /* DD7248 000067DC 0C000000 */  jal        func_00000000
    /* DD724C 000067E0 24C600BC */   addiu     $a2, $a2, 0xBC
    /* DD7250 000067E4 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD7254 000067E8 0C000000 */  jal        func_00000000
    /* DD7258 000067EC 24840000 */   addiu     $a0, $a0, 0x0
    /* DD725C 000067F0 0C000000 */  jal        func_00000000
    /* DD7260 000067F4 8FA40020 */   lw        $a0, 0x20($sp)
    /* DD7264 000067F8 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD7268 000067FC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD726C 00006800 03E00008 */  jr         $ra
    /* DD7270 00006804 00000000 */   nop
endlabel MapProp_hostioParms
