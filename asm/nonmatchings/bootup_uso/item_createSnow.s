nonmatching item_createSnow, 0x64

glabel item_createSnow
    /* DD5FA8 0000553C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD5FAC 00005540 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5FB0 00005544 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD5FB4 00005548 AFA4001C */  sw         $a0, 0x1C($sp)
    /* DD5FB8 0000554C 0C000000 */  jal        func_00000000
    /* DD5FBC 00005550 24040058 */   addiu     $a0, $zero, 0x58
    /* DD5FC0 00005554 1040000D */  beqz       $v0, .L0000558C
    /* DD5FC4 00005558 00402025 */   or        $a0, $v0, $zero
    /* DD5FC8 0000555C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5FCC 00005560 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD5FD0 00005564 3C070000 */  lui        $a3, (0x0 >> 16)
    /* DD5FD4 00005568 24E70000 */  addiu      $a3, $a3, 0x0
    /* DD5FD8 0000556C 8CC60000 */  lw         $a2, 0x0($a2)
    /* DD5FDC 00005570 AFA20020 */  sw         $v0, 0x20($sp)
    /* DD5FE0 00005574 0C000000 */  jal        func_00000000
    /* DD5FE4 00005578 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5FE8 0000557C 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD5FEC 00005580 3C180000 */  lui        $t8, %hi(SnowTmpl_vtbl_79C8)
    /* DD5FF0 00005584 271879C8 */  addiu      $t8, $t8, %lo(SnowTmpl_vtbl_79C8)
    /* DD5FF4 00005588 AC980028 */  sw         $t8, 0x28($a0)
  .L0000558C:
    /* DD5FF8 0000558C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5FFC 00005590 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD6000 00005594 00801025 */  or         $v0, $a0, $zero
    /* DD6004 00005598 03E00008 */  jr         $ra
    /* DD6008 0000559C 00000000 */   nop
endlabel item_createSnow
