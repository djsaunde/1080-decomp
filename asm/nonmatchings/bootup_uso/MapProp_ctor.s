nonmatching MapProp_ctor, 0x160

glabel MapProp_ctor
    /* DD6CC0 00006254 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DD6CC4 00006258 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD6CC8 0000625C 00808025 */  or         $s0, $a0, $zero
    /* DD6CCC 00006260 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD6CD0 00006264 AFA50054 */  sw         $a1, 0x54($sp)
    /* DD6CD4 00006268 AFA60058 */  sw         $a2, 0x58($sp)
    /* DD6CD8 0000626C 14800005 */  bnez       $a0, .L00006284
    /* DD6CDC 00006270 AFA7005C */   sw        $a3, 0x5C($sp)
    /* DD6CE0 00006274 0C000000 */  jal        func_00000000
    /* DD6CE4 00006278 240400DC */   addiu     $a0, $zero, 0xDC
    /* DD6CE8 0000627C 10400048 */  beqz       $v0, .L000063A0
    /* DD6CEC 00006280 00408025 */   or        $s0, $v0, $zero
  .L00006284:
    /* DD6CF0 00006284 16000005 */  bnez       $s0, .L0000629C
    /* DD6CF4 00006288 02003025 */   or        $a2, $s0, $zero
    /* DD6CF8 0000628C 0C000000 */  jal        func_00000000
    /* DD6CFC 00006290 240400B4 */   addiu     $a0, $zero, 0xB4
    /* DD6D00 00006294 10400027 */  beqz       $v0, .L00006334
    /* DD6D04 00006298 00403025 */   or        $a2, $v0, $zero
  .L0000629C:
    /* DD6D08 0000629C 3C050000 */  lui        $a1, %hi(sStr_MNode_7E74)
    /* DD6D0C 000062A0 24A57E74 */  addiu      $a1, $a1, %lo(sStr_MNode_7E74)
    /* DD6D10 000062A4 00C02025 */  or         $a0, $a2, $zero
    /* DD6D14 000062A8 0C000000 */  jal        func_00000000
    /* DD6D18 000062AC AFA6004C */   sw        $a2, 0x4C($sp)
    /* DD6D1C 000062B0 8FA6004C */  lw         $a2, 0x4C($sp)
    /* DD6D20 000062B4 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD6D24 000062B8 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD6D28 000062BC 2401FFD4 */  addiu      $at, $zero, -0x2C
    /* DD6D2C 000062C0 ACCE0028 */  sw         $t6, 0x28($a2)
    /* DD6D30 000062C4 14C10007 */  bne        $a2, $at, .L000062E4
    /* DD6D34 000062C8 24C3002C */   addiu     $v1, $a2, 0x2C
    /* DD6D38 000062CC 24040004 */  addiu      $a0, $zero, 0x4
    /* DD6D3C 000062D0 0C000000 */  jal        func_00000000
    /* DD6D40 000062D4 AFA6004C */   sw        $a2, 0x4C($sp)
    /* DD6D44 000062D8 8FA6004C */  lw         $a2, 0x4C($sp)
    /* DD6D48 000062DC 1040000A */  beqz       $v0, .L00006308
    /* DD6D4C 000062E0 00401825 */   or        $v1, $v0, $zero
  .L000062E4:
    /* DD6D50 000062E4 14600007 */  bnez       $v1, .L00006304
    /* DD6D54 000062E8 00602025 */   or        $a0, $v1, $zero
    /* DD6D58 000062EC 24040004 */  addiu      $a0, $zero, 0x4
    /* DD6D5C 000062F0 0C000000 */  jal        func_00000000
    /* DD6D60 000062F4 AFA6004C */   sw        $a2, 0x4C($sp)
    /* DD6D64 000062F8 8FA6004C */  lw         $a2, 0x4C($sp)
    /* DD6D68 000062FC 10400002 */  beqz       $v0, .L00006308
    /* DD6D6C 00006300 00402025 */   or        $a0, $v0, $zero
  .L00006304:
    /* DD6D70 00006304 AC800000 */  sw         $zero, 0x0($a0)
  .L00006308:
    /* DD6D74 00006308 00C02025 */  or         $a0, $a2, $zero
    /* DD6D78 0000630C 0C000000 */  jal        func_00000000
    /* DD6D7C 00006310 AFA6004C */   sw        $a2, 0x4C($sp)
    /* DD6D80 00006314 44800000 */  mtc1       $zero, $f0
    /* DD6D84 00006318 8FA6004C */  lw         $a2, 0x4C($sp)
    /* DD6D88 0000631C 27A50034 */  addiu      $a1, $sp, 0x34
    /* DD6D8C 00006320 E7A00034 */  swc1       $f0, 0x34($sp)
    /* DD6D90 00006324 24C40030 */  addiu      $a0, $a2, 0x30
    /* DD6D94 00006328 E7A00038 */  swc1       $f0, 0x38($sp)
    /* DD6D98 0000632C 0C000000 */  jal        func_00000000
    /* DD6D9C 00006330 E7A0003C */   swc1      $f0, 0x3C($sp)
  .L00006334:
    /* DD6DA0 00006334 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD6DA4 00006338 3C180000 */  lui        $t8, %hi(sStr_Course_7E7C)
    /* DD6DA8 0000633C 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD6DAC 00006340 27187E7C */  addiu      $t8, $t8, %lo(sStr_Course_7E7C)
    /* DD6DB0 00006344 AE0F0028 */  sw         $t7, 0x28($s0)
    /* DD6DB4 00006348 AE18000C */  sw         $t8, 0xC($s0)
    /* DD6DB8 0000634C 8FB90054 */  lw         $t9, 0x54($sp)
    /* DD6DBC 00006350 02002025 */  or         $a0, $s0, $zero
    /* DD6DC0 00006354 AE1900B4 */  sw         $t9, 0xB4($s0)
    /* DD6DC4 00006358 0C000000 */  jal        func_00000000
    /* DD6DC8 0000635C 8FA50054 */   lw        $a1, 0x54($sp)
    /* DD6DCC 00006360 8E0800B4 */  lw         $t0, 0xB4($s0)
    /* DD6DD0 00006364 3C040000 */  lui        $a0, %hi(sStr_shp_pGrid_NULL_7E84)
    /* DD6DD4 00006368 3C050000 */  lui        $a1, %hi(sStr_mapprop_cpp_7E98)
    /* DD6DD8 0000636C 8D0900A0 */  lw         $t1, 0xA0($t0)
    /* DD6DDC 00006370 24A57E98 */  addiu      $a1, $a1, %lo(sStr_mapprop_cpp_7E98)
    /* DD6DE0 00006374 24847E84 */  addiu      $a0, $a0, %lo(sStr_shp_pGrid_NULL_7E84)
    /* DD6DE4 00006378 55200004 */  bnel       $t1, $zero, .L0000638C
    /* DD6DE8 0000637C 8FAA005C */   lw        $t2, 0x5C($sp)
    /* DD6DEC 00006380 0C000000 */  jal        func_00000000
    /* DD6DF0 00006384 240603E4 */   addiu     $a2, $zero, 0x3E4
    /* DD6DF4 00006388 8FAA005C */  lw         $t2, 0x5C($sp)
  .L0000638C:
    /* DD6DF8 0000638C AE0A00B8 */  sw         $t2, 0xB8($s0)
    /* DD6DFC 00006390 8FAB0058 */  lw         $t3, 0x58($sp)
    /* DD6E00 00006394 AE0B00BC */  sw         $t3, 0xBC($s0)
    /* DD6E04 00006398 8FAC0060 */  lw         $t4, 0x60($sp)
    /* DD6E08 0000639C AE0C00C0 */  sw         $t4, 0xC0($s0)
  .L000063A0:
    /* DD6E0C 000063A0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD6E10 000063A4 02001025 */  or         $v0, $s0, $zero
    /* DD6E14 000063A8 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD6E18 000063AC 03E00008 */  jr         $ra
    /* DD6E1C 000063B0 27BD0050 */   addiu     $sp, $sp, 0x50
endlabel MapProp_ctor
