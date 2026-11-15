nonmatching TreeTmpl_hostioParms, 0x60

glabel TreeTmpl_hostioParms
    /* DD91B0 00008744 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD91B4 00008748 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD91B8 0000874C AFA40018 */  sw         $a0, 0x18($sp)
    /* DD91BC 00008750 8C85000C */  lw         $a1, 0xC($a0)
    /* DD91C0 00008754 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD91C4 00008758 24840000 */  addiu      $a0, $a0, 0x0
    /* DD91C8 0000875C 0C000000 */  jal        func_00000000
    /* DD91CC 00008760 00003025 */   or        $a2, $zero, $zero
    /* DD91D0 00008764 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD91D4 00008768 00002825 */  or         $a1, $zero, $zero
    /* DD91D8 0000876C 0C000000 */  jal        func_00000000
    /* DD91DC 00008770 2484004C */   addiu     $a0, $a0, 0x4C
    /* DD91E0 00008774 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD91E4 00008778 0C000000 */  jal        func_00000000
    /* DD91E8 0000877C 2484005C */   addiu     $a0, $a0, 0x5C
    /* DD91EC 00008780 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD91F0 00008784 0C000000 */  jal        func_00000000
    /* DD91F4 00008788 24840000 */   addiu     $a0, $a0, 0x0
    /* DD91F8 0000878C 0C000000 */  jal        func_00000000
    /* DD91FC 00008790 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD9200 00008794 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD9204 00008798 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD9208 0000879C 03E00008 */  jr         $ra
    /* DD920C 000087A0 00000000 */   nop
endlabel TreeTmpl_hostioParms
