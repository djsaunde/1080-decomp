nonmatching boot_ItemType_create_000046EC, 0x90

glabel boot_ItemType_create_000046EC
    /* DD5158 000046EC 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD515C 000046F0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5160 000046F4 AFA40028 */  sw         $a0, 0x28($sp)
    /* DD5164 000046F8 0C000000 */  jal        func_00000000
    /* DD5168 000046FC 24040040 */   addiu     $a0, $zero, 0x40
    /* DD516C 00004700 10400008 */  beqz       $v0, .L00004724
    /* DD5170 00004704 00402025 */   or        $a0, $v0, $zero
    /* DD5174 00004708 0C000000 */  jal        func_00000000
    /* DD5178 0000470C AFA2001C */   sw        $v0, 0x1C($sp)
    /* DD517C 00004710 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DD5180 00004714 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD5184 00004718 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD5188 0000471C AC8E0028 */  sw         $t6, 0x28($a0)
    /* DD518C 00004720 AC80003C */  sw         $zero, 0x3C($a0)
  .L00004724:
    /* DD5190 00004724 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DD5194 00004728 00801825 */  or         $v1, $a0, $zero
    /* DD5198 0000472C 24840010 */  addiu      $a0, $a0, 0x10
    /* DD519C 00004730 8DE20040 */  lw         $v0, 0x40($t7)
    /* DD51A0 00004734 1040000C */  beqz       $v0, .L00004768
    /* DD51A4 00004738 00402825 */   or        $a1, $v0, $zero
    /* DD51A8 0000473C AFA30024 */  sw         $v1, 0x24($sp)
    /* DD51AC 00004740 0C000000 */  jal        func_00000000
    /* DD51B0 00004744 AFA20020 */   sw        $v0, 0x20($sp)
    /* DD51B4 00004748 8FA50020 */  lw         $a1, 0x20($sp)
    /* DD51B8 0000474C 8FA30024 */  lw         $v1, 0x24($sp)
    /* DD51BC 00004750 24190001 */  addiu      $t9, $zero, 0x1
    /* DD51C0 00004754 8CB80014 */  lw         $t8, 0x14($a1)
    /* DD51C4 00004758 53000003 */  beql       $t8, $zero, .L00004768
    /* DD51C8 0000475C ACA30014 */   sw        $v1, 0x14($a1)
    /* DD51CC 00004760 ACB90004 */  sw         $t9, 0x4($a1)
    /* DD51D0 00004764 ACA30014 */  sw         $v1, 0x14($a1)
  .L00004768:
    /* DD51D4 00004768 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD51D8 0000476C 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD51DC 00004770 00601025 */  or         $v0, $v1, $zero
    /* DD51E0 00004774 03E00008 */  jr         $ra
    /* DD51E4 00004778 00000000 */   nop
endlabel boot_ItemType_create_000046EC
