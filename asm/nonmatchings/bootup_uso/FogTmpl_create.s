nonmatching FogTmpl_create, 0x1D4

glabel FogTmpl_create
    /* DD8B90 00008124 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DD8B94 00008128 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD8B98 0000812C AFA40050 */  sw         $a0, 0x50($sp)
    /* DD8B9C 00008130 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD8BA0 00008134 0C000000 */  jal        func_00000000
    /* DD8BA4 00008138 24040090 */   addiu     $a0, $zero, 0x90
    /* DD8BA8 0000813C 10400059 */  beqz       $v0, .L000082A4
    /* DD8BAC 00008140 00408025 */   or        $s0, $v0, $zero
    /* DD8BB0 00008144 0C000000 */  jal        func_00000000
    /* DD8BB4 00008148 00402025 */   or        $a0, $v0, $zero
    /* DD8BB8 0000814C 3C0E0000 */  lui        $t6, %hi(D_00007878)
    /* DD8BBC 00008150 25CE7878 */  addiu      $t6, $t6, %lo(D_00007878)
    /* DD8BC0 00008154 2401FFC4 */  addiu      $at, $zero, -0x3C
    /* DD8BC4 00008158 AE0E0028 */  sw         $t6, 0x28($s0)
    /* DD8BC8 0000815C 16010005 */  bne        $s0, $at, .L00008174
    /* DD8BCC 00008160 2605003C */   addiu     $a1, $s0, 0x3C
    /* DD8BD0 00008164 0C000000 */  jal        func_00000000
    /* DD8BD4 00008168 24040038 */   addiu     $a0, $zero, 0x38
    /* DD8BD8 0000816C 10400043 */  beqz       $v0, .L0000827C
    /* DD8BDC 00008170 00402825 */   or        $a1, $v0, $zero
  .L00008174:
    /* DD8BE0 00008174 14A00007 */  bnez       $a1, .L00008194
    /* DD8BE4 00008178 00A01825 */   or        $v1, $a1, $zero
    /* DD8BE8 0000817C 24040008 */  addiu      $a0, $zero, 0x8
    /* DD8BEC 00008180 0C000000 */  jal        func_00000000
    /* DD8BF0 00008184 AFA50044 */   sw        $a1, 0x44($sp)
    /* DD8BF4 00008188 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD8BF8 0000818C 10400005 */  beqz       $v0, .L000081A4
    /* DD8BFC 00008190 00401825 */   or        $v1, $v0, $zero
  .L00008194:
    /* DD8C00 00008194 3C0F0000 */  lui        $t7, %hi(sStr_Parms_7FC4)
    /* DD8C04 00008198 25EF7FC4 */  addiu      $t7, $t7, %lo(sStr_Parms_7FC4)
    /* DD8C08 0000819C AC6F0000 */  sw         $t7, 0x0($v1)
    /* DD8C0C 000081A0 AC600004 */  sw         $zero, 0x4($v1)
  .L000081A4:
    /* DD8C10 000081A4 3C180000 */  lui        $t8, %hi(sTag_fin_7FCC)
    /* DD8C14 000081A8 8F187FCC */  lw         $t8, %lo(sTag_fin_7FCC)($t8)
    /* DD8C18 000081AC 2401FFF8 */  addiu      $at, $zero, -0x8
    /* DD8C1C 000081B0 24A40008 */  addiu      $a0, $a1, 0x8
    /* DD8C20 000081B4 AFB80040 */  sw         $t8, 0x40($sp)
    /* DD8C24 000081B8 14A10007 */  bne        $a1, $at, .L000081D8
    /* DD8C28 000081BC AFB80024 */   sw        $t8, 0x24($sp)
  alabel D_000081C0
    /* DD8C2C 000081C0 24040018 */  addiu      $a0, $zero, 0x18
    /* DD8C30 000081C4 0C000000 */  jal        func_00000000
    /* DD8C34 000081C8 AFA50044 */   sw        $a1, 0x44($sp)
    /* DD8C38 000081CC 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD8C3C 000081D0 10400010 */  beqz       $v0, .L00008214
    /* DD8C40 000081D4 00402025 */   or        $a0, $v0, $zero
  .L000081D8:
    /* DD8C44 000081D8 8FA60024 */  lw         $a2, 0x24($sp)
    /* DD8C48 000081DC 24070001 */  addiu      $a3, $zero, 0x1
    /* DD8C4C 000081E0 AFA4002C */  sw         $a0, 0x2C($sp)
    /* DD8C50 000081E4 AFA50044 */  sw         $a1, 0x44($sp)
    /* DD8C54 000081E8 0C000000 */  jal        func_00000000
    /* DD8C58 000081EC AFA60008 */   sw        $a2, 0x8($sp)
    /* DD8C5C 000081F0 8FA4002C */  lw         $a0, 0x2C($sp)
    /* DD8C60 000081F4 3C014348 */  lui        $at, (0x43480000 >> 16)
    /* DD8C64 000081F8 44812000 */  mtc1       $at, $f4
    /* DD8C68 000081FC 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD8C6C 00008200 3C090000 */  lui        $t1, %hi(D_00007540)
    /* DD8C70 00008204 25297540 */  addiu      $t1, $t1, %lo(D_00007540)
    /* DD8C74 00008208 AC89000C */  sw         $t1, 0xC($a0)
    /* DD8C78 0000820C AC800014 */  sw         $zero, 0x14($a0)
    /* DD8C7C 00008210 E4840010 */  swc1       $f4, 0x10($a0)
  .L00008214:
    /* DD8C80 00008214 3C0A0000 */  lui        $t2, %hi(D_00007FD0)
    /* DD8C84 00008218 8D4A7FD0 */  lw         $t2, %lo(D_00007FD0)($t2)
    /* DD8C88 0000821C 2401FFE0 */  addiu      $at, $zero, -0x20
    /* DD8C8C 00008220 24A40020 */  addiu      $a0, $a1, 0x20
    /* DD8C90 00008224 AFAA003C */  sw         $t2, 0x3C($sp)
    /* DD8C94 00008228 14A10007 */  bne        $a1, $at, .L00008248
    /* DD8C98 0000822C AFAA0024 */   sw        $t2, 0x24($sp)
    /* DD8C9C 00008230 24040018 */  addiu      $a0, $zero, 0x18
    /* DD8CA0 00008234 0C000000 */  jal        func_00000000
    /* DD8CA4 00008238 AFA50044 */   sw        $a1, 0x44($sp)
    /* DD8CA8 0000823C 8FA50044 */  lw         $a1, 0x44($sp)
    /* DD8CAC 00008240 1040000E */  beqz       $v0, .L0000827C
    /* DD8CB0 00008244 00402025 */   or        $a0, $v0, $zero
  .L00008248:
    /* DD8CB4 00008248 8FA60024 */  lw         $a2, 0x24($sp)
    /* DD8CB8 0000824C 24070001 */  addiu      $a3, $zero, 0x1
    /* DD8CBC 00008250 AFA4002C */  sw         $a0, 0x2C($sp)
    /* DD8CC0 00008254 0C000000 */  jal        func_00000000
    /* DD8CC4 00008258 AFA60008 */   sw        $a2, 0x8($sp)
    /* DD8CC8 0000825C 8FA4002C */  lw         $a0, 0x2C($sp)
    /* DD8CCC 00008260 3C014396 */  lui        $at, (0x43960000 >> 16)
    /* DD8CD0 00008264 44813000 */  mtc1       $at, $f6
    /* DD8CD4 00008268 3C0D0000 */  lui        $t5, %hi(D_00007540)
    /* DD8CD8 0000826C 25AD7540 */  addiu      $t5, $t5, %lo(D_00007540)
    /* DD8CDC 00008270 AC8D000C */  sw         $t5, 0xC($a0)
    /* DD8CE0 00008274 AC800014 */  sw         $zero, 0x14($a0)
    /* DD8CE4 00008278 E4860010 */  swc1       $f6, 0x10($a0)
  .L0000827C:
    /* DD8CE8 0000827C 2401FF8C */  addiu      $at, $zero, -0x74
    /* DD8CEC 00008280 16010005 */  bne        $s0, $at, .L00008298
    /* DD8CF0 00008284 26050074 */   addiu     $a1, $s0, 0x74
    /* DD8CF4 00008288 0C000000 */  jal        func_00000000
    /* DD8CF8 0000828C 24040018 */   addiu     $a0, $zero, 0x18
    /* DD8CFC 00008290 10400003 */  beqz       $v0, .L000082A0
    /* DD8D00 00008294 00402825 */   or        $a1, $v0, $zero
  .L00008298:
    /* DD8D04 00008298 0C000000 */  jal        func_00000000
    /* DD8D08 0000829C 00A02025 */   or        $a0, $a1, $zero
  .L000082A0:
    /* DD8D0C 000082A0 AE00008C */  sw         $zero, 0x8C($s0)
  .L000082A4:
    /* DD8D10 000082A4 8FAE0050 */  lw         $t6, 0x50($sp)
    /* DD8D14 000082A8 02001825 */  or         $v1, $s0, $zero
    /* DD8D18 000082AC 24640010 */  addiu      $a0, $v1, 0x10
    /* DD8D1C 000082B0 8DC20040 */  lw         $v0, 0x40($t6)
    /* DD8D20 000082B4 1040000B */  beqz       $v0, .L000082E4
    /* DD8D24 000082B8 00408025 */   or        $s0, $v0, $zero
    /* DD8D28 000082BC 00402825 */  or         $a1, $v0, $zero
    /* DD8D2C 000082C0 0C000000 */  jal        func_00000000
    /* DD8D30 000082C4 AFA3004C */   sw        $v1, 0x4C($sp)
    /* DD8D34 000082C8 8E0F0014 */  lw         $t7, 0x14($s0)
    /* DD8D38 000082CC 8FA3004C */  lw         $v1, 0x4C($sp)
    /* DD8D3C 000082D0 24180001 */  addiu      $t8, $zero, 0x1
    /* DD8D40 000082D4 51E00003 */  beql       $t7, $zero, .L000082E4
    /* DD8D44 000082D8 AE030014 */   sw        $v1, 0x14($s0)
    /* DD8D48 000082DC AE180004 */  sw         $t8, 0x4($s0)
    /* DD8D4C 000082E0 AE030014 */  sw         $v1, 0x14($s0)
  .L000082E4:
    /* DD8D50 000082E4 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD8D54 000082E8 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD8D58 000082EC 27BD0050 */  addiu      $sp, $sp, 0x50
    /* DD8D5C 000082F0 03E00008 */  jr         $ra
    /* DD8D60 000082F4 00601025 */   or        $v0, $v1, $zero
endlabel FogTmpl_create
