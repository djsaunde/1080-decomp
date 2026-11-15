nonmatching boot_ItemType_create_0000EE8C, 0x90

glabel boot_ItemType_create_0000EE8C
    /* DDF8F8 0000EE8C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DDF8FC 0000EE90 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF900 0000EE94 AFA40028 */  sw         $a0, 0x28($sp)
    /* DDF904 0000EE98 0C000000 */  jal        func_00000000
    /* DDF908 0000EE9C 24040040 */   addiu     $a0, $zero, 0x40
    /* DDF90C 0000EEA0 10400008 */  beqz       $v0, .L0000EEC4
    /* DDF910 0000EEA4 00402025 */   or        $a0, $v0, $zero
    /* DDF914 0000EEA8 0C000000 */  jal        func_00000000
    /* DDF918 0000EEAC AFA2001C */   sw        $v0, 0x1C($sp)
    /* DDF91C 0000EEB0 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DDF920 0000EEB4 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DDF924 0000EEB8 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DDF928 0000EEBC AC8E0028 */  sw         $t6, 0x28($a0)
    /* DDF92C 0000EEC0 AC80003C */  sw         $zero, 0x3C($a0)
  .L0000EEC4:
    /* DDF930 0000EEC4 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DDF934 0000EEC8 00801825 */  or         $v1, $a0, $zero
    /* DDF938 0000EECC 24840010 */  addiu      $a0, $a0, 0x10
    /* DDF93C 0000EED0 8DE20040 */  lw         $v0, 0x40($t7)
    /* DDF940 0000EED4 1040000C */  beqz       $v0, .L0000EF08
    /* DDF944 0000EED8 00402825 */   or        $a1, $v0, $zero
    /* DDF948 0000EEDC AFA30024 */  sw         $v1, 0x24($sp)
    /* DDF94C 0000EEE0 0C000000 */  jal        func_00000000
    /* DDF950 0000EEE4 AFA20020 */   sw        $v0, 0x20($sp)
    /* DDF954 0000EEE8 8FA50020 */  lw         $a1, 0x20($sp)
    /* DDF958 0000EEEC 8FA30024 */  lw         $v1, 0x24($sp)
    /* DDF95C 0000EEF0 24190001 */  addiu      $t9, $zero, 0x1
    /* DDF960 0000EEF4 8CB80014 */  lw         $t8, 0x14($a1)
    /* DDF964 0000EEF8 53000003 */  beql       $t8, $zero, .L0000EF08
    /* DDF968 0000EEFC ACA30014 */   sw        $v1, 0x14($a1)
    /* DDF96C 0000EF00 ACB90004 */  sw         $t9, 0x4($a1)
    /* DDF970 0000EF04 ACA30014 */  sw         $v1, 0x14($a1)
  .L0000EF08:
    /* DDF974 0000EF08 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF978 0000EF0C 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DDF97C 0000EF10 00601025 */  or         $v0, $v1, $zero
    /* DDF980 0000EF14 03E00008 */  jr         $ra
    /* DDF984 0000EF18 00000000 */   nop
endlabel boot_ItemType_create_0000EE8C
    /* DDF988 0000EF1C 00000000 */  nop
