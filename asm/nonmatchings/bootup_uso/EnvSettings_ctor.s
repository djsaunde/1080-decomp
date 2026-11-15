nonmatching EnvSettings_ctor, 0xB4

glabel EnvSettings_ctor
    /* DD7BBC 00007150 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD7BC0 00007154 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD7BC4 00007158 14800005 */  bnez       $a0, .L00007170
    /* DD7BC8 0000715C 00803025 */   or        $a2, $a0, $zero
    /* DD7BCC 00007160 0C000000 */  jal        func_00000000
    /* DD7BD0 00007164 24040044 */   addiu     $a0, $zero, 0x44
    /* DD7BD4 00007168 10400021 */  beqz       $v0, .L000071F0
    /* DD7BD8 0000716C 00403025 */   or        $a2, $v0, $zero
  .L00007170:
    /* DD7BDC 00007170 3C050000 */  lui        $a1, %hi(sStr_EnvSettings_7F84)
    /* DD7BE0 00007174 24A57F84 */  addiu      $a1, $a1, %lo(sStr_EnvSettings_7F84)
    /* DD7BE4 00007178 00C02025 */  or         $a0, $a2, $zero
    /* DD7BE8 0000717C 0C000000 */  jal        func_00000000
    /* DD7BEC 00007180 AFA60018 */   sw        $a2, 0x18($sp)
    /* DD7BF0 00007184 8FA60018 */  lw         $a2, 0x18($sp)
    /* DD7BF4 00007188 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD7BF8 0000718C 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD7BFC 00007190 2401FFD4 */  addiu      $at, $zero, -0x2C
    /* DD7C00 00007194 ACCE0028 */  sw         $t6, 0x28($a2)
    /* DD7C04 00007198 14C10007 */  bne        $a2, $at, .L000071B8
    /* DD7C08 0000719C 24C3002C */   addiu     $v1, $a2, 0x2C
    /* DD7C0C 000071A0 24040004 */  addiu      $a0, $zero, 0x4
    /* DD7C10 000071A4 0C000000 */  jal        func_00000000
    /* DD7C14 000071A8 AFA60018 */   sw        $a2, 0x18($sp)
    /* DD7C18 000071AC 8FA60018 */  lw         $a2, 0x18($sp)
    /* DD7C1C 000071B0 10400002 */  beqz       $v0, .L000071BC
    /* DD7C20 000071B4 00401825 */   or        $v1, $v0, $zero
  .L000071B8:
    /* DD7C24 000071B8 AC600000 */  sw         $zero, 0x0($v1)
  .L000071BC:
    /* DD7C28 000071BC 24C20018 */  addiu      $v0, $a2, 0x18
    /* DD7C2C 000071C0 8C4F0000 */  lw         $t7, 0x0($v0)
    /* DD7C30 000071C4 2401FFF7 */  addiu      $at, $zero, -0x9
    /* DD7C34 000071C8 24190004 */  addiu      $t9, $zero, 0x4
    /* DD7C38 000071CC 01E1C024 */  and        $t8, $t7, $at
    /* DD7C3C 000071D0 AC580000 */  sw         $t8, 0x0($v0)
    /* DD7C40 000071D4 24080B00 */  addiu      $t0, $zero, 0xB00
    /* DD7C44 000071D8 24090001 */  addiu      $t1, $zero, 0x1
    /* DD7C48 000071DC ACC00040 */  sw         $zero, 0x40($a2)
    /* DD7C4C 000071E0 ACC0003C */  sw         $zero, 0x3C($a2)
    /* DD7C50 000071E4 ACD90030 */  sw         $t9, 0x30($a2)
    /* DD7C54 000071E8 ACC80034 */  sw         $t0, 0x34($a2)
    /* DD7C58 000071EC ACC90038 */  sw         $t1, 0x38($a2)
  .L000071F0:
    /* DD7C5C 000071F0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD7C60 000071F4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD7C64 000071F8 00C01025 */  or         $v0, $a2, $zero
    /* DD7C68 000071FC 03E00008 */  jr         $ra
    /* DD7C6C 00007200 00000000 */   nop
endlabel EnvSettings_ctor
