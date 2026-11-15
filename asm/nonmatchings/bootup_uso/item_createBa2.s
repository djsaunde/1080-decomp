nonmatching item_createBa2, 0xB0

glabel item_createBa2
    /* DD5C40 000051D4 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD5C44 000051D8 AFA40038 */  sw         $a0, 0x38($sp)
    /* DD5C48 000051DC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5C4C 000051E0 3C040000 */  lui        $a0, %hi(D_00007DC4)
    /* DD5C50 000051E4 0C000000 */  jal        func_00000000
    /* DD5C54 000051E8 24847DC4 */   addiu     $a0, $a0, %lo(D_00007DC4)
    /* DD5C58 000051EC 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DD5C5C 000051F0 2404004C */  addiu      $a0, $zero, 0x4C
    /* DD5C60 000051F4 0C000000 */  jal        func_00000000
    /* DD5C64 000051F8 AFAE002C */   sw        $t6, 0x2C($sp)
    /* DD5C68 000051FC 1040001C */  beqz       $v0, .L00005270
    /* DD5C6C 00005200 00401825 */   or        $v1, $v0, $zero
    /* DD5C70 00005204 8FAF002C */  lw         $t7, 0x2C($sp)
    /* DD5C74 00005208 00402025 */  or         $a0, $v0, $zero
    /* DD5C78 0000520C 14400007 */  bnez       $v0, .L0000522C
    /* DD5C7C 00005210 AFAF001C */   sw        $t7, 0x1C($sp)
    /* DD5C80 00005214 24040048 */  addiu      $a0, $zero, 0x48
    /* DD5C84 00005218 0C000000 */  jal        func_00000000
    /* DD5C88 0000521C AFA20030 */   sw        $v0, 0x30($sp)
    /* DD5C8C 00005220 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5C90 00005224 1040000C */  beqz       $v0, .L00005258
    /* DD5C94 00005228 00402025 */   or        $a0, $v0, $zero
  .L0000522C:
    /* DD5C98 0000522C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5C9C 00005230 00003025 */  or         $a2, $zero, $zero
    /* DD5CA0 00005234 AFA30030 */  sw         $v1, 0x30($sp)
    /* DD5CA4 00005238 AFA40024 */  sw         $a0, 0x24($sp)
    /* DD5CA8 0000523C 0C000000 */  jal        func_00000000
    /* DD5CAC 00005240 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5CB0 00005244 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD5CB4 00005248 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD5CB8 0000524C 27390000 */  addiu      $t9, $t9, 0x0
    /* DD5CBC 00005250 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5CC0 00005254 AC990028 */  sw         $t9, 0x28($a0)
  .L00005258:
    /* DD5CC4 00005258 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD5CC8 0000525C 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD5CCC 00005260 25290000 */  addiu      $t1, $t1, 0x0
    /* DD5CD0 00005264 25080000 */  addiu      $t0, $t0, 0x0
    /* DD5CD4 00005268 AC680028 */  sw         $t0, 0x28($v1)
    /* DD5CD8 0000526C AC690048 */  sw         $t1, 0x48($v1)
  .L00005270:
    /* DD5CDC 00005270 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5CE0 00005274 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD5CE4 00005278 00601025 */  or         $v0, $v1, $zero
    /* DD5CE8 0000527C 03E00008 */  jr         $ra
    /* DD5CEC 00005280 00000000 */   nop
endlabel item_createBa2
