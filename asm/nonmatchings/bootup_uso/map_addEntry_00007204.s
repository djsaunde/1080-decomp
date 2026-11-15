nonmatching map_addEntry_00007204, 0x84

glabel map_addEntry_00007204
    /* DD7C70 00007204 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DD7C74 00007208 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD7C78 0000720C AFA40030 */  sw         $a0, 0x30($sp)
    /* DD7C7C 00007210 AFA50034 */  sw         $a1, 0x34($sp)
    /* DD7C80 00007214 AFA60038 */  sw         $a2, 0x38($sp)
    /* DD7C84 00007218 24040008 */  addiu      $a0, $zero, 0x8
    /* DD7C88 0000721C 0C000000 */  jal        func_00000000
    /* DD7C8C 00007220 AFA6001C */   sw        $a2, 0x1C($sp)
    /* DD7C90 00007224 10400010 */  beqz       $v0, .L00007268
    /* DD7C94 00007228 00403025 */   or        $a2, $v0, $zero
    /* DD7C98 0000722C 2401FFFC */  addiu      $at, $zero, -0x4
    /* DD7C9C 00007230 14410007 */  bne        $v0, $at, .L00007250
    /* DD7CA0 00007234 24430004 */   addiu     $v1, $v0, 0x4
    /* DD7CA4 00007238 24040004 */  addiu      $a0, $zero, 0x4
    /* DD7CA8 0000723C 0C000000 */  jal        func_00000000
    /* DD7CAC 00007240 AFA20028 */   sw        $v0, 0x28($sp)
    /* DD7CB0 00007244 8FA60028 */  lw         $a2, 0x28($sp)
    /* DD7CB4 00007248 10400002 */  beqz       $v0, .L00007254
    /* DD7CB8 0000724C 00401825 */   or        $v1, $v0, $zero
  .L00007250:
    /* DD7CBC 00007250 AC600000 */  sw         $zero, 0x0($v1)
  .L00007254:
    /* DD7CC0 00007254 8FAF0034 */  lw         $t7, 0x34($sp)
    /* DD7CC4 00007258 27B8001C */  addiu      $t8, $sp, 0x1C
    /* DD7CC8 0000725C ACCF0000 */  sw         $t7, 0x0($a2)
    /* DD7CCC 00007260 8F080000 */  lw         $t0, 0x0($t8)
    /* DD7CD0 00007264 ACC80004 */  sw         $t0, 0x4($a2)
  .L00007268:
    /* DD7CD4 00007268 8FA40030 */  lw         $a0, 0x30($sp)
    /* DD7CD8 0000726C 00C02825 */  or         $a1, $a2, $zero
    /* DD7CDC 00007270 0C000000 */  jal        func_00000000
    /* DD7CE0 00007274 2484002C */   addiu     $a0, $a0, 0x2C
    /* DD7CE4 00007278 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD7CE8 0000727C 27BD0030 */  addiu      $sp, $sp, 0x30
    /* DD7CEC 00007280 03E00008 */  jr         $ra
    /* DD7CF0 00007284 00000000 */   nop
endlabel map_addEntry_00007204
