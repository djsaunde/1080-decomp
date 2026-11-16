nonmatching Tv1Tmpl_create, 0x90

glabel Tv1Tmpl_create
    /* DD86E0 00007C74 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD86E4 00007C78 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD86E8 00007C7C AFA40028 */  sw         $a0, 0x28($sp)
    /* DD86EC 00007C80 0C000000 */  jal        func_00000000
    /* DD86F0 00007C84 24040088 */   addiu     $a0, $zero, 0x88
    /* DD86F4 00007C88 10400008 */  beqz       $v0, .L00007CAC
    /* DD86F8 00007C8C 00403025 */   or        $a2, $v0, $zero
    /* DD86FC 00007C90 00402025 */  or         $a0, $v0, $zero
    /* DD8700 00007C94 0C000000 */  jal        func_00000000
    /* DD8704 00007C98 AFA2001C */   sw        $v0, 0x1C($sp)
    /* DD8708 00007C9C 8FA6001C */  lw         $a2, 0x1C($sp)
    /* DD870C 00007CA0 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD8710 00007CA4 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD8714 00007CA8 ACCE0028 */  sw         $t6, 0x28($a2)
  .L00007CAC:
    /* DD8718 00007CAC 3C010000 */  lui        $at, %hi(kRo_Tv1Tmpl_create_5EC)
    /* DD871C 00007CB0 C42405EC */  lwc1       $f4, %lo(kRo_Tv1Tmpl_create_5EC)($at)
    /* DD8720 00007CB4 24C40010 */  addiu      $a0, $a2, 0x10
    /* DD8724 00007CB8 E4C40080 */  swc1       $f4, 0x80($a2)
    /* DD8728 00007CBC 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DD872C 00007CC0 8DE50040 */  lw         $a1, 0x40($t7)
    /* DD8730 00007CC4 AFA6001C */  sw         $a2, 0x1C($sp)
    /* DD8734 00007CC8 0C000000 */  jal        func_00000000
    /* DD8738 00007CCC AFA50020 */   sw        $a1, 0x20($sp)
    /* DD873C 00007CD0 8FA50020 */  lw         $a1, 0x20($sp)
    /* DD8740 00007CD4 8FA6001C */  lw         $a2, 0x1C($sp)
    /* DD8744 00007CD8 24190001 */  addiu      $t9, $zero, 0x1
    /* DD8748 00007CDC 8CB80014 */  lw         $t8, 0x14($a1)
    /* DD874C 00007CE0 53000003 */  beql       $t8, $zero, .L00007CF0
    /* DD8750 00007CE4 ACA60014 */   sw        $a2, 0x14($a1)
    /* DD8754 00007CE8 ACB90004 */  sw         $t9, 0x4($a1)
    /* DD8758 00007CEC ACA60014 */  sw         $a2, 0x14($a1)
  .L00007CF0:
    /* DD875C 00007CF0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8760 00007CF4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DD8764 00007CF8 00C01025 */  or         $v0, $a2, $zero
    /* DD8768 00007CFC 03E00008 */  jr         $ra
    /* DD876C 00007D00 00000000 */   nop
endlabel Tv1Tmpl_create
