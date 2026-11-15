nonmatching item_createBal, 0xB0

glabel item_createBal
    /* DD5B90 00005124 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DD5B94 00005128 AFA40038 */  sw         $a0, 0x38($sp)
    /* DD5B98 0000512C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD5B9C 00005130 3C040000 */  lui        $a0, %hi(D_00007DB4)
    /* DD5BA0 00005134 0C000000 */  jal        func_00000000
    /* DD5BA4 00005138 24847DB4 */   addiu     $a0, $a0, %lo(D_00007DB4)
    /* DD5BA8 0000513C 8FAE0038 */  lw         $t6, 0x38($sp)
    /* DD5BAC 00005140 2404004C */  addiu      $a0, $zero, 0x4C
    /* DD5BB0 00005144 0C000000 */  jal        func_00000000
    /* DD5BB4 00005148 AFAE002C */   sw        $t6, 0x2C($sp)
    /* DD5BB8 0000514C 1040001C */  beqz       $v0, .L000051C0
    /* DD5BBC 00005150 00401825 */   or        $v1, $v0, $zero
    /* DD5BC0 00005154 8FAF002C */  lw         $t7, 0x2C($sp)
    /* DD5BC4 00005158 00402025 */  or         $a0, $v0, $zero
    /* DD5BC8 0000515C 14400007 */  bnez       $v0, .L0000517C
    /* DD5BCC 00005160 AFAF001C */   sw        $t7, 0x1C($sp)
    /* DD5BD0 00005164 24040048 */  addiu      $a0, $zero, 0x48
    /* DD5BD4 00005168 0C000000 */  jal        func_00000000
    /* DD5BD8 0000516C AFA20030 */   sw        $v0, 0x30($sp)
    /* DD5BDC 00005170 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5BE0 00005174 1040000C */  beqz       $v0, .L000051A8
    /* DD5BE4 00005178 00402025 */   or        $a0, $v0, $zero
  .L0000517C:
    /* DD5BE8 0000517C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DD5BEC 00005180 00003025 */  or         $a2, $zero, $zero
    /* DD5BF0 00005184 AFA30030 */  sw         $v1, 0x30($sp)
    /* DD5BF4 00005188 AFA40024 */  sw         $a0, 0x24($sp)
    /* DD5BF8 0000518C 0C000000 */  jal        func_00000000
    /* DD5BFC 00005190 AFA50004 */   sw        $a1, 0x4($sp)
    /* DD5C00 00005194 8FA40024 */  lw         $a0, 0x24($sp)
    /* DD5C04 00005198 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD5C08 0000519C 27390000 */  addiu      $t9, $t9, 0x0
    /* DD5C0C 000051A0 8FA30030 */  lw         $v1, 0x30($sp)
    /* DD5C10 000051A4 AC990028 */  sw         $t9, 0x28($a0)
  .L000051A8:
    /* DD5C14 000051A8 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DD5C18 000051AC 3C090000 */  lui        $t1, (0x0 >> 16)
    /* DD5C1C 000051B0 25290000 */  addiu      $t1, $t1, 0x0
    /* DD5C20 000051B4 25080000 */  addiu      $t0, $t0, 0x0
    /* DD5C24 000051B8 AC680028 */  sw         $t0, 0x28($v1)
    /* DD5C28 000051BC AC690048 */  sw         $t1, 0x48($v1)
  .L000051C0:
    /* DD5C2C 000051C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5C30 000051C4 27BD0038 */  addiu      $sp, $sp, 0x38
    /* DD5C34 000051C8 00601025 */  or         $v0, $v1, $zero
    /* DD5C38 000051CC 03E00008 */  jr         $ra
    /* DD5C3C 000051D0 00000000 */   nop
endlabel item_createBal
