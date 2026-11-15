nonmatching boot_ItemType_create_0000EBE8, 0x90

glabel boot_ItemType_create_0000EBE8
    /* DDF654 0000EBE8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDF658 0000EBEC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF65C 0000EBF0 AFA40028 */  sw         $a0, 0x28($sp)
    /* DDF660 0000EBF4 0C000000 */  jal        func_00000000
    /* DDF664 0000EBF8 24040040 */   addiu     $a0, $zero, 0x40
    /* DDF668 0000EBFC 10400008 */  beqz       $v0, .L0000EC20
    /* DDF66C 0000EC00 00402025 */   or        $a0, $v0, $zero
    /* DDF670 0000EC04 0C000000 */  jal        func_00000000
    /* DDF674 0000EC08 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DDF678 0000EC0C 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DDF67C 0000EC10 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DDF680 0000EC14 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DDF684 0000EC18 AC8E0028 */  sw         $t6, 0x28($a0)
    /* DDF688 0000EC1C AC80003C */  sw         $zero, 0x3C($a0)
  .L0000EC20:
    /* DDF68C 0000EC20 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DDF690 0000EC24 00801825 */  or         $v1, $a0, $zero
    /* DDF694 0000EC28 24840010 */  addiu      $a0, $a0, 0x10
    /* DDF698 0000EC2C 8DE20040 */  lw         $v0, 0x40($t7)
    /* DDF69C 0000EC30 1040000C */  beqz       $v0, .L0000EC64
    /* DDF6A0 0000EC34 00402825 */   or        $a1, $v0, $zero
    /* DDF6A4 0000EC38 AFA30024 */  sw         $v1, 0x24($sp)
    /* DDF6A8 0000EC3C 0C000000 */  jal        func_00000000
    /* DDF6AC 0000EC40 AFA20020 */   sw        $v0, 0x20($sp)
    /* DDF6B0 0000EC44 8FA50020 */  lw         $a1, 0x20($sp)
    /* DDF6B4 0000EC48 8FA30024 */  lw         $v1, 0x24($sp)
    /* DDF6B8 0000EC4C 24190001 */  addiu      $t9, $zero, 0x1
    /* DDF6BC 0000EC50 8CB80014 */  lw         $t8, 0x14($a1)
    /* DDF6C0 0000EC54 53000003 */  beql       $t8, $zero, .L0000EC64
    /* DDF6C4 0000EC58 ACA30014 */   sw        $v1, 0x14($a1)
    /* DDF6C8 0000EC5C ACB90004 */  sw         $t9, 0x4($a1)
    /* DDF6CC 0000EC60 ACA30014 */  sw         $v1, 0x14($a1)
  .L0000EC64:
    /* DDF6D0 0000EC64 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF6D4 0000EC68 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDF6D8 0000EC6C 00601025 */  or         $v0, $v1, $zero
    /* DDF6DC 0000EC70 03E00008 */  jr         $ra
    /* DDF6E0 0000EC74 00000000 */   nop
endlabel boot_ItemType_create_0000EBE8
    /* DDF6E4 0000EC78 00000000 */  nop
    /* DDF6E8 0000EC7C 00000000 */  nop
