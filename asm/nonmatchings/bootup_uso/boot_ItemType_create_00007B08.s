nonmatching boot_ItemType_create_00007B08, 0x90

glabel boot_ItemType_create_00007B08
    /* DD8574 00007B08 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD8578 00007B0C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD857C 00007B10 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD8580 00007B14 0C000000 */  jal        func_00000000
    /* DD8584 00007B18 24040040 */   addiu     $a0, $zero, 0x40
    /* DD8588 00007B1C 10400008 */  beqz       $v0, .L00007B40
    /* DD858C 00007B20 00402025 */   or        $a0, $v0, $zero
    /* DD8590 00007B24 0C000000 */  jal        func_00000000
    /* DD8594 00007B28 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DD8598 00007B2C 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DD859C 00007B30 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD85A0 00007B34 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD85A4 00007B38 AC8E0028 */  sw         $t6, 0x28($a0)
    /* DD85A8 00007B3C AC80003C */  sw         $zero, 0x3C($a0)
  .L00007B40:
    /* DD85AC 00007B40 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DD85B0 00007B44 00801825 */  or         $v1, $a0, $zero
    /* DD85B4 00007B48 24840010 */  addiu      $a0, $a0, 0x10
    /* DD85B8 00007B4C 8DE20040 */  lw         $v0, 0x40($t7)
    /* DD85BC 00007B50 1040000C */  beqz       $v0, .L00007B84
    /* DD85C0 00007B54 00402825 */   or        $a1, $v0, $zero
    /* DD85C4 00007B58 AFA30024 */  sw         $v1, 0x24($sp)
    /* DD85C8 00007B5C 0C000000 */  jal        func_00000000
    /* DD85CC 00007B60 AFA20020 */   sw        $v0, 0x20($sp)
    /* DD85D0 00007B64 8FA50020 */  lw         $a1, 0x20($sp)
    /* DD85D4 00007B68 8FA30024 */  lw         $v1, 0x24($sp)
    /* DD85D8 00007B6C 24190001 */  addiu      $t9, $zero, 0x1
    /* DD85DC 00007B70 8CB80014 */  lw         $t8, 0x14($a1)
    /* DD85E0 00007B74 53000003 */  beql       $t8, $zero, .L00007B84
    /* DD85E4 00007B78 ACA30014 */   sw        $v1, 0x14($a1)
    /* DD85E8 00007B7C ACB90004 */  sw         $t9, 0x4($a1)
    /* DD85EC 00007B80 ACA30014 */  sw         $v1, 0x14($a1)
  .L00007B84:
    /* DD85F0 00007B84 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD85F4 00007B88 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD85F8 00007B8C 00601025 */  or         $v0, $v1, $zero
    /* DD85FC 00007B90 03E00008 */  jr         $ra
    /* DD8600 00007B94 00000000 */   nop
endlabel boot_ItemType_create_00007B08
