nonmatching boot_ItemType_create_00002244, 0x90

glabel boot_ItemType_create_00002244
    /* DD2CB0 00002244 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD2CB4 00002248 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD2CB8 0000224C AFA40028 */  sw         $a0, 0x28($sp)
    /* DD2CBC 00002250 0C000000 */  jal        func_00000000
    /* DD2CC0 00002254 24040040 */   addiu     $a0, $zero, 0x40
    /* DD2CC4 00002258 10400008 */  beqz       $v0, .L0000227C
    /* DD2CC8 0000225C 00402025 */   or        $a0, $v0, $zero
    /* DD2CCC 00002260 0C000000 */  jal        func_00000000
    /* DD2CD0 00002264 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DD2CD4 00002268 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DD2CD8 0000226C 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD2CDC 00002270 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD2CE0 00002274 AC8E0028 */  sw         $t6, 0x28($a0)
    /* DD2CE4 00002278 AC80003C */  sw         $zero, 0x3C($a0)
  .L0000227C:
    /* DD2CE8 0000227C 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DD2CEC 00002280 00801825 */  or         $v1, $a0, $zero
    /* DD2CF0 00002284 24840010 */  addiu      $a0, $a0, 0x10
    /* DD2CF4 00002288 8DE20040 */  lw         $v0, 0x40($t7)
    /* DD2CF8 0000228C 1040000C */  beqz       $v0, .L000022C0
    /* DD2CFC 00002290 00402825 */   or        $a1, $v0, $zero
    /* DD2D00 00002294 AFA30024 */  sw         $v1, 0x24($sp)
    /* DD2D04 00002298 0C000000 */  jal        func_00000000
    /* DD2D08 0000229C AFA20020 */   sw        $v0, 0x20($sp)
    /* DD2D0C 000022A0 8FA50020 */  lw         $a1, 0x20($sp)
    /* DD2D10 000022A4 8FA30024 */  lw         $v1, 0x24($sp)
    /* DD2D14 000022A8 24190001 */  addiu      $t9, $zero, 0x1
    /* DD2D18 000022AC 8CB80014 */  lw         $t8, 0x14($a1)
    /* DD2D1C 000022B0 53000003 */  beql       $t8, $zero, .L000022C0
    /* DD2D20 000022B4 ACA30014 */   sw        $v1, 0x14($a1)
    /* DD2D24 000022B8 ACB90004 */  sw         $t9, 0x4($a1)
    /* DD2D28 000022BC ACA30014 */  sw         $v1, 0x14($a1)
  .L000022C0:
    /* DD2D2C 000022C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD2D30 000022C4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD2D34 000022C8 00601025 */  or         $v0, $v1, $zero
    /* DD2D38 000022CC 03E00008 */  jr         $ra
    /* DD2D3C 000022D0 00000000 */   nop
endlabel boot_ItemType_create_00002244
    /* DD2D40 000022D4 00000000 */  nop
    /* DD2D44 000022D8 00000000 */  nop
    /* DD2D48 000022DC 00000000 */  nop
