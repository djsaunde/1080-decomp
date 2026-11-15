nonmatching QuitProcess_func_0000FD4C, 0x154

glabel QuitProcess_func_0000FD4C
    /* DE07B8 0000FD4C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE07BC 0000FD50 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE07C0 0000FD54 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE07C4 0000FD58 AFA5002C */  sw         $a1, 0x2C($sp)
    /* DE07C8 0000FD5C AFA60030 */  sw         $a2, 0x30($sp)
    /* DE07CC 0000FD60 AFB10018 */  sw         $s1, 0x18($sp)
    /* DE07D0 0000FD64 AFB00014 */  sw         $s0, 0x14($sp)
    /* DE07D4 0000FD68 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE07D8 0000FD6C 15C00008 */  bnez       $t6, .L0000FD90
    /* DE07DC 0000FD70 00000000 */   nop
    /* DE07E0 0000FD74 24040050 */  addiu      $a0, $zero, 0x50
    /* DE07E4 0000FD78 0C000000 */  jal        func_00000000
    /* DE07E8 0000FD7C 00000000 */   nop
    /* DE07EC 0000FD80 AFA20028 */  sw         $v0, 0x28($sp)
    /* DE07F0 0000FD84 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE07F4 0000FD88 11E0003C */  beqz       $t7, .L0000FE7C
    /* DE07F8 0000FD8C 00000000 */   nop
  .L0000FD90:
    /* DE07FC 0000FD90 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0800 0000FD94 16000007 */  bnez       $s0, .L0000FDB4
    /* DE0804 0000FD98 00000000 */   nop
    /* DE0808 0000FD9C 24040050 */  addiu      $a0, $zero, 0x50
    /* DE080C 0000FDA0 0C000000 */  jal        func_00000000
    /* DE0810 0000FDA4 00000000 */   nop
    /* DE0814 0000FDA8 00408025 */  or         $s0, $v0, $zero
    /* DE0818 0000FDAC 12000015 */  beqz       $s0, .L0000FE04
    /* DE081C 0000FDB0 00000000 */   nop
  .L0000FDB4:
    /* DE0820 0000FDB4 02008825 */  or         $s1, $s0, $zero
    /* DE0824 0000FDB8 16200007 */  bnez       $s1, .L0000FDD8
    /* DE0828 0000FDBC 00000000 */   nop
    /* DE082C 0000FDC0 2404002C */  addiu      $a0, $zero, 0x2C
    /* DE0830 0000FDC4 0C000000 */  jal        func_00000000
    /* DE0834 0000FDC8 00000000 */   nop
    /* DE0838 0000FDCC 00408825 */  or         $s1, $v0, $zero
    /* DE083C 0000FDD0 12200009 */  beqz       $s1, .L0000FDF8
    /* DE0840 0000FDD4 00000000 */   nop
  .L0000FDD8:
    /* DE0844 0000FDD8 02202025 */  or         $a0, $s1, $zero
    /* DE0848 0000FDDC 3C050001 */  lui        $a1, %hi(D_0000C5A4)
    /* DE084C 0000FDE0 24A5C5A4 */  addiu      $a1, $a1, %lo(D_0000C5A4)
    /* DE0850 0000FDE4 0C000000 */  jal        func_00000000
    /* DE0854 0000FDE8 00000000 */   nop
    /* DE0858 0000FDEC 3C180000 */  lui        $t8, (0x0 >> 16)
    /* DE085C 0000FDF0 27180000 */  addiu      $t8, $t8, 0x0
    /* DE0860 0000FDF4 AE380028 */  sw         $t8, 0x28($s1)
  .L0000FDF8:
    /* DE0864 0000FDF8 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DE0868 0000FDFC 27390000 */  addiu      $t9, $t9, 0x0
    /* DE086C 0000FE00 AE190028 */  sw         $t9, 0x28($s0)
  .L0000FE04:
    /* DE0870 0000FE04 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DE0874 0000FE08 25080000 */  addiu      $t0, $t0, 0x0
    /* DE0878 0000FE0C 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE087C 0000FE10 AD280028 */  sw         $t0, 0x28($t1)
    /* DE0880 0000FE14 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE0884 0000FE18 3C110001 */  lui        $s1, %hi(D_0000C5AC)
    /* DE0888 0000FE1C 2631C5AC */  addiu      $s1, $s1, %lo(D_0000C5AC)
    /* DE088C 0000FE20 AE11000C */  sw         $s1, 0xC($s0)
    /* DE0890 0000FE24 8FAA002C */  lw         $t2, 0x2C($sp)
    /* DE0894 0000FE28 3C010000 */  lui        $at, %hi(func_00000008 + 0x2C)
    /* DE0898 0000FE2C AC2A0034 */  sw         $t2, %lo(func_00000008 + 0x2C)($at)
    /* DE089C 0000FE30 8FAB0030 */  lw         $t3, 0x30($sp)
    /* DE08A0 0000FE34 3C010000 */  lui        $at, %hi(func_00000008 + 0x38)
    /* DE08A4 0000FE38 AC2B0040 */  sw         $t3, %lo(func_00000008 + 0x38)($at)
    /* DE08A8 0000FE3C 8FAC0028 */  lw         $t4, 0x28($sp)
    /* DE08AC 0000FE40 AD800044 */  sw         $zero, 0x44($t4)
    /* DE08B0 0000FE44 8FAD0028 */  lw         $t5, 0x28($sp)
    /* DE08B4 0000FE48 ADA00048 */  sw         $zero, 0x48($t5)
    /* DE08B8 0000FE4C 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE08BC 0000FE50 00002825 */  or         $a1, $zero, $zero
    /* DE08C0 0000FE54 0C000000 */  jal        func_00000000
    /* DE08C4 0000FE58 00000000 */   nop
    /* DE08C8 0000FE5C 3C010000 */  lui        $at, (0x0 >> 16)
    /* DE08CC 0000FE60 AC220000 */  sw         $v0, 0x0($at)
    /* DE08D0 0000FE64 8FB00028 */  lw         $s0, 0x28($sp)
    /* DE08D4 0000FE68 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE08D8 0000FE6C 8C840000 */  lw         $a0, 0x0($a0)
    /* DE08DC 0000FE70 02002825 */  or         $a1, $s0, $zero
    /* DE08E0 0000FE74 0C000000 */  jal        func_00000000
    /* DE08E4 0000FE78 00000000 */   nop
  .L0000FE7C:
    /* DE08E8 0000FE7C 8FA20028 */  lw         $v0, 0x28($sp)
    /* DE08EC 0000FE80 10000001 */  b          .L0000FE88
    /* DE08F0 0000FE84 00000000 */   nop
  .L0000FE88:
    /* DE08F4 0000FE88 8FB00014 */  lw         $s0, 0x14($sp)
    /* DE08F8 0000FE8C 8FB10018 */  lw         $s1, 0x18($sp)
    /* DE08FC 0000FE90 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE0900 0000FE94 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE0904 0000FE98 03E00008 */  jr         $ra
    /* DE0908 0000FE9C 00000000 */   nop
endlabel QuitProcess_func_0000FD4C
