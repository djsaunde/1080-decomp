nonmatching boot_ItemType_create_0000E690, 0x90

glabel boot_ItemType_create_0000E690
    /* DDF0FC 0000E690 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDF100 0000E694 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF104 0000E698 AFA40028 */  sw         $a0, 0x28($sp)
    /* DDF108 0000E69C 0C000000 */  jal        func_00000000
    /* DDF10C 0000E6A0 24040040 */   addiu     $a0, $zero, 0x40
    /* DDF110 0000E6A4 10400008 */  beqz       $v0, .L0000E6C8
    /* DDF114 0000E6A8 00402025 */   or        $a0, $v0, $zero
    /* DDF118 0000E6AC 0C000000 */  jal        func_00000000
    /* DDF11C 0000E6B0 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DDF120 0000E6B4 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DDF124 0000E6B8 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DDF128 0000E6BC 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DDF12C 0000E6C0 AC8E0028 */  sw         $t6, 0x28($a0)
    /* DDF130 0000E6C4 AC80003C */  sw         $zero, 0x3C($a0)
  .L0000E6C8:
    /* DDF134 0000E6C8 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DDF138 0000E6CC 00801825 */  or         $v1, $a0, $zero
    /* DDF13C 0000E6D0 24840010 */  addiu      $a0, $a0, 0x10
    /* DDF140 0000E6D4 8DE20040 */  lw         $v0, 0x40($t7)
    /* DDF144 0000E6D8 1040000C */  beqz       $v0, .L0000E70C
    /* DDF148 0000E6DC 00402825 */   or        $a1, $v0, $zero
    /* DDF14C 0000E6E0 AFA30024 */  sw         $v1, 0x24($sp)
    /* DDF150 0000E6E4 0C000000 */  jal        func_00000000
    /* DDF154 0000E6E8 AFA20020 */   sw        $v0, 0x20($sp)
    /* DDF158 0000E6EC 8FA50020 */  lw         $a1, 0x20($sp)
    /* DDF15C 0000E6F0 8FA30024 */  lw         $v1, 0x24($sp)
    /* DDF160 0000E6F4 24190001 */  addiu      $t9, $zero, 0x1
    /* DDF164 0000E6F8 8CB80014 */  lw         $t8, 0x14($a1)
    /* DDF168 0000E6FC 53000003 */  beql       $t8, $zero, .L0000E70C
    /* DDF16C 0000E700 ACA30014 */   sw        $v1, 0x14($a1)
    /* DDF170 0000E704 ACB90004 */  sw         $t9, 0x4($a1)
    /* DDF174 0000E708 ACA30014 */  sw         $v1, 0x14($a1)
  .L0000E70C:
    /* DDF178 0000E70C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF17C 0000E710 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDF180 0000E714 00601025 */  or         $v0, $v1, $zero
    /* DDF184 0000E718 03E00008 */  jr         $ra
    /* DDF188 0000E71C 00000000 */   nop
endlabel boot_ItemType_create_0000E690
