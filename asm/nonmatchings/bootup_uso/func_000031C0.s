nonmatching func_000031C0, 0x328

glabel func_000031C0
    /* DD3C2C 000031C0 27BDFF70 */  addiu      $sp, $sp, -0x90
    /* DD3C30 000031C4 AFBF0024 */  sw         $ra, 0x24($sp)
    /* DD3C34 000031C8 AFA40090 */  sw         $a0, 0x90($sp)
    /* DD3C38 000031CC AFA50094 */  sw         $a1, 0x94($sp)
    /* DD3C3C 000031D0 AFA60098 */  sw         $a2, 0x98($sp)
    /* DD3C40 000031D4 AFA7009C */  sw         $a3, 0x9C($sp)
    /* DD3C44 000031D8 0C000000 */  jal        func_00000000
    /* DD3C48 000031DC 240400B8 */   addiu     $a0, $zero, 0xB8
    /* DD3C4C 000031E0 1040002F */  beqz       $v0, .L000032A0
    /* DD3C50 000031E4 AFA20074 */   sw        $v0, 0x74($sp)
    /* DD3C54 000031E8 14400005 */  bnez       $v0, .L00003200
    /* DD3C58 000031EC 00403025 */   or        $a2, $v0, $zero
    /* DD3C5C 000031F0 0C000000 */  jal        func_00000000
    /* DD3C60 000031F4 240400B4 */   addiu     $a0, $zero, 0xB4
    /* DD3C64 000031F8 10400029 */  beqz       $v0, .L000032A0
    /* DD3C68 000031FC 00403025 */   or        $a2, $v0, $zero
  .L00003200:
    /* DD3C6C 00003200 3C050000 */  lui        $a1, %hi(D_00007460)
    /* DD3C70 00003204 24A57460 */  addiu      $a1, $a1, %lo(D_00007460)
    /* DD3C74 00003208 00C02025 */  or         $a0, $a2, $zero
    /* DD3C78 0000320C 0C000000 */  jal        func_00000000
    /* DD3C7C 00003210 AFA6006C */   sw        $a2, 0x6C($sp)
    /* DD3C80 00003214 8FA6006C */  lw         $a2, 0x6C($sp)
    /* DD3C84 00003218 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD3C88 0000321C 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD3C8C 00003220 2401FFD4 */  addiu      $at, $zero, -0x2C
    /* DD3C90 00003224 ACCE0028 */  sw         $t6, 0x28($a2)
    /* DD3C94 00003228 14C10007 */  bne        $a2, $at, .L00003248
    /* DD3C98 0000322C 24C3002C */   addiu     $v1, $a2, 0x2C
    /* DD3C9C 00003230 24040004 */  addiu      $a0, $zero, 0x4
    /* DD3CA0 00003234 0C000000 */  jal        func_00000000
    /* DD3CA4 00003238 AFA6006C */   sw        $a2, 0x6C($sp)
    /* DD3CA8 0000323C 8FA6006C */  lw         $a2, 0x6C($sp)
    /* DD3CAC 00003240 1040000A */  beqz       $v0, .L0000326C
    /* DD3CB0 00003244 00401825 */   or        $v1, $v0, $zero
  .L00003248:
    /* DD3CB4 00003248 14600007 */  bnez       $v1, .L00003268
    /* DD3CB8 0000324C 00602025 */   or        $a0, $v1, $zero
    /* DD3CBC 00003250 24040004 */  addiu      $a0, $zero, 0x4
    /* DD3CC0 00003254 0C000000 */  jal        func_00000000
    /* DD3CC4 00003258 AFA6006C */   sw        $a2, 0x6C($sp)
    /* DD3CC8 0000325C 8FA6006C */  lw         $a2, 0x6C($sp)
    /* DD3CCC 00003260 10400002 */  beqz       $v0, .L0000326C
    /* DD3CD0 00003264 00402025 */   or        $a0, $v0, $zero
  .L00003268:
    /* DD3CD4 00003268 AC800000 */  sw         $zero, 0x0($a0)
  .L0000326C:
    /* DD3CD8 0000326C 00C02025 */  or         $a0, $a2, $zero
    /* DD3CDC 00003270 0C000000 */  jal        func_00000000
    /* DD3CE0 00003274 AFA6006C */   sw        $a2, 0x6C($sp)
    /* DD3CE4 00003278 44800000 */  mtc1       $zero, $f0
    /* DD3CE8 0000327C 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD3CEC 00003280 44812000 */  mtc1       $at, $f4
    /* DD3CF0 00003284 8FA6006C */  lw         $a2, 0x6C($sp)
    /* DD3CF4 00003288 27A50048 */  addiu      $a1, $sp, 0x48
    /* DD3CF8 0000328C E7A00048 */  swc1       $f0, 0x48($sp)
    /* DD3CFC 00003290 E7A00050 */  swc1       $f0, 0x50($sp)
    /* DD3D00 00003294 24C40030 */  addiu      $a0, $a2, 0x30
    /* DD3D04 00003298 0C000000 */  jal        func_00000000
    /* DD3D08 0000329C E7A4004C */   swc1      $f4, 0x4C($sp)
  .L000032A0:
    /* DD3D0C 000032A0 8FA20094 */  lw         $v0, 0x94($sp)
    /* DD3D10 000032A4 8FAF0074 */  lw         $t7, 0x74($sp)
    /* DD3D14 000032A8 24010002 */  addiu      $at, $zero, 0x2
    /* DD3D18 000032AC 10400007 */  beqz       $v0, .L000032CC
    /* DD3D1C 000032B0 AFAF0078 */   sw        $t7, 0x78($sp)
    /* DD3D20 000032B4 10410005 */  beq        $v0, $at, .L000032CC
    /* DD3D24 000032B8 24010001 */   addiu     $at, $zero, 0x1
    /* DD3D28 000032BC 10410031 */  beq        $v0, $at, .L00003384
    /* DD3D2C 000032C0 00000000 */   nop
    /* DD3D30 000032C4 10000055 */  b          .L0000341C
    /* DD3D34 000032C8 8FA40098 */   lw        $a0, 0x98($sp)
  .L000032CC:
    /* DD3D38 000032CC 3C0143FA */  lui        $at, (0x43FA0000 >> 16)
    /* DD3D3C 000032D0 44813000 */  mtc1       $at, $f6
    /* DD3D40 000032D4 3C010000 */  lui        $at, %hi(func_000003F8 + 0x138)
    /* DD3D44 000032D8 C4280530 */  lwc1       $f8, %lo(func_000003F8 + 0x138)($at)
    /* DD3D48 000032DC 8FB800A0 */  lw         $t8, 0xA0($sp)
    /* DD3D4C 000032E0 3C190000 */  lui        $t9, %hi(D_0000746C)
    /* DD3D50 000032E4 2739746C */  addiu      $t9, $t9, %lo(D_0000746C)
    /* DD3D54 000032E8 AFB9001C */  sw         $t9, 0x1C($sp)
    /* DD3D58 000032EC 00002025 */  or         $a0, $zero, $zero
    /* DD3D5C 000032F0 8FA5009C */  lw         $a1, 0x9C($sp)
    /* DD3D60 000032F4 8FA60074 */  lw         $a2, 0x74($sp)
    /* DD3D64 000032F8 24070000 */  addiu      $a3, $zero, 0x0
    /* DD3D68 000032FC E7A60010 */  swc1       $f6, 0x10($sp)
    /* DD3D6C 00003300 E7A80014 */  swc1       $f8, 0x14($sp)
    /* DD3D70 00003304 0C000000 */  jal        func_00000000
    /* DD3D74 00003308 AFB80018 */   sw        $t8, 0x18($sp)
    /* DD3D78 0000330C 8FA5009C */  lw         $a1, 0x9C($sp)
    /* DD3D7C 00003310 AFA2008C */  sw         $v0, 0x8C($sp)
    /* DD3D80 00003314 AFA20088 */  sw         $v0, 0x88($sp)
    /* DD3D84 00003318 3C070000 */  lui        $a3, %hi(D_00007480)
    /* DD3D88 0000331C 24E77480 */  addiu      $a3, $a3, %lo(D_00007480)
    /* DD3D8C 00003320 00002025 */  or         $a0, $zero, $zero
    /* DD3D90 00003324 0C000000 */  jal        func_00000000
    /* DD3D94 00003328 8CA60874 */   lw        $a2, 0x874($a1)
    /* DD3D98 0000332C 8FA4008C */  lw         $a0, 0x8C($sp)
    /* DD3D9C 00003330 00402825 */  or         $a1, $v0, $zero
    /* DD3DA0 00003334 0C000000 */  jal        func_00000000
    /* DD3DA4 00003338 AFA20080 */   sw        $v0, 0x80($sp)
    /* DD3DA8 0000333C 8FA5008C */  lw         $a1, 0x8C($sp)
    /* DD3DAC 00003340 8FA60080 */  lw         $a2, 0x80($sp)
    /* DD3DB0 00003344 3C070000 */  lui        $a3, %hi(D_00007490)
    /* DD3DB4 00003348 24E77490 */  addiu      $a3, $a3, %lo(D_00007490)
    /* DD3DB8 0000334C 00002025 */  or         $a0, $zero, $zero
    /* DD3DBC 00003350 0C000000 */  jal        func_00000000
    /* DD3DC0 00003354 ACA60154 */   sw        $a2, 0x154($a1)
    /* DD3DC4 00003358 8FA40078 */  lw         $a0, 0x78($sp)
    /* DD3DC8 0000335C 00402825 */  or         $a1, $v0, $zero
    /* DD3DCC 00003360 0C000000 */  jal        func_00000000
    /* DD3DD0 00003364 AFA2007C */   sw        $v0, 0x7C($sp)
    /* DD3DD4 00003368 8FA3007C */  lw         $v1, 0x7C($sp)
    /* DD3DD8 0000336C 8FA90078 */  lw         $t1, 0x78($sp)
    /* DD3DDC 00003370 3C0A0000 */  lui        $t2, %hi(D_000074A4)
    /* DD3DE0 00003374 254A74A4 */  addiu      $t2, $t2, %lo(D_000074A4)
    /* DD3DE4 00003378 AD2300B4 */  sw         $v1, 0xB4($t1)
    /* DD3DE8 0000337C 10000026 */  b          .L00003418
    /* DD3DEC 00003380 AC6A000C */   sw        $t2, 0xC($v1)
  .L00003384:
    /* DD3DF0 00003384 0C000000 */  jal        func_00000000
    /* DD3DF4 00003388 2404015C */   addiu     $a0, $zero, 0x15C
    /* DD3DF8 0000338C 10400012 */  beqz       $v0, .L000033D8
    /* DD3DFC 00003390 AFA20068 */   sw        $v0, 0x68($sp)
    /* DD3E00 00003394 3C010000 */  lui        $at, %hi(func_000003F8 + 0x13C)
    /* DD3E04 00003398 C42A0534 */  lwc1       $f10, %lo(func_000003F8 + 0x13C)($at)
    /* DD3E08 0000339C 00402025 */  or         $a0, $v0, $zero
    /* DD3E0C 000033A0 8FA50074 */  lw         $a1, 0x74($sp)
    /* DD3E10 000033A4 24060000 */  addiu      $a2, $zero, 0x0
    /* DD3E14 000033A8 3C0743FA */  lui        $a3, (0x43FA0000 >> 16)
    /* DD3E18 000033AC 0C000000 */  jal        func_00000000
    /* DD3E1C 000033B0 E7AA0010 */   swc1      $f10, 0x10($sp)
    /* DD3E20 000033B4 8FA30068 */  lw         $v1, 0x68($sp)
    /* DD3E24 000033B8 8FA5009C */  lw         $a1, 0x9C($sp)
    /* DD3E28 000033BC 3C0B0000 */  lui        $t3, %hi(D_00007120)
    /* DD3E2C 000033C0 C470014C */  lwc1       $f16, 0x14C($v1)
    /* DD3E30 000033C4 256B7120 */  addiu      $t3, $t3, %lo(D_00007120)
    /* DD3E34 000033C8 AC6B0028 */  sw         $t3, 0x28($v1)
    /* DD3E38 000033CC AC600158 */  sw         $zero, 0x158($v1)
    /* DD3E3C 000033D0 AC650154 */  sw         $a1, 0x154($v1)
    /* DD3E40 000033D4 E4700150 */  swc1       $f16, 0x150($v1)
  .L000033D8:
    /* DD3E44 000033D8 8FA5009C */  lw         $a1, 0x9C($sp)
    /* DD3E48 000033DC 8FAC0068 */  lw         $t4, 0x68($sp)
    /* DD3E4C 000033E0 3C070000 */  lui        $a3, %hi(D_000074B0)
    /* DD3E50 000033E4 24E774B0 */  addiu      $a3, $a3, %lo(D_000074B0)
    /* DD3E54 000033E8 00002025 */  or         $a0, $zero, $zero
    /* DD3E58 000033EC 00A03025 */  or         $a2, $a1, $zero
    /* DD3E5C 000033F0 0C000000 */  jal        func_00000000
    /* DD3E60 000033F4 AFAC0088 */   sw        $t4, 0x88($sp)
    /* DD3E64 000033F8 AFA20084 */  sw         $v0, 0x84($sp)
    /* DD3E68 000033FC 8FA40078 */  lw         $a0, 0x78($sp)
    /* DD3E6C 00003400 0C000000 */  jal        func_00000000
    /* DD3E70 00003404 00402825 */   or        $a1, $v0, $zero
    /* DD3E74 00003408 8FAE0084 */  lw         $t6, 0x84($sp)
    /* DD3E78 0000340C 3C0D0000 */  lui        $t5, %hi(D_000074C4)
    /* DD3E7C 00003410 25AD74C4 */  addiu      $t5, $t5, %lo(D_000074C4)
    /* DD3E80 00003414 ADCD000C */  sw         $t5, 0xC($t6)
  .L00003418:
    /* DD3E84 00003418 8FA40098 */  lw         $a0, 0x98($sp)
  .L0000341C:
    /* DD3E88 0000341C 8FA50088 */  lw         $a1, 0x88($sp)
    /* DD3E8C 00003420 24840010 */  addiu      $a0, $a0, 0x10
    /* DD3E90 00003424 0C000000 */  jal        func_00000000
    /* DD3E94 00003428 AFA40028 */   sw        $a0, 0x28($sp)
    /* DD3E98 0000342C 8FA20088 */  lw         $v0, 0x88($sp)
    /* DD3E9C 00003430 8FA40028 */  lw         $a0, 0x28($sp)
    /* DD3EA0 00003434 24180001 */  addiu      $t8, $zero, 0x1
    /* DD3EA4 00003438 8C4F0014 */  lw         $t7, 0x14($v0)
    /* DD3EA8 0000343C 51E00003 */  beql       $t7, $zero, .L0000344C
    /* DD3EAC 00003440 8FB90098 */   lw        $t9, 0x98($sp)
    /* DD3EB0 00003444 AC580004 */  sw         $t8, 0x4($v0)
    /* DD3EB4 00003448 8FB90098 */  lw         $t9, 0x98($sp)
  .L0000344C:
    /* DD3EB8 0000344C AC590014 */  sw         $t9, 0x14($v0)
    /* DD3EBC 00003450 0C000000 */  jal        func_00000000
    /* DD3EC0 00003454 8FA50078 */   lw        $a1, 0x78($sp)
    /* DD3EC4 00003458 8FA20078 */  lw         $v0, 0x78($sp)
    /* DD3EC8 0000345C 24090001 */  addiu      $t1, $zero, 0x1
    /* DD3ECC 00003460 8C480014 */  lw         $t0, 0x14($v0)
    /* DD3ED0 00003464 51000003 */  beql       $t0, $zero, .L00003474
    /* DD3ED4 00003468 8FAA0098 */   lw        $t2, 0x98($sp)
    /* DD3ED8 0000346C AC490004 */  sw         $t1, 0x4($v0)
    /* DD3EDC 00003470 8FAA0098 */  lw         $t2, 0x98($sp)
  .L00003474:
    /* DD3EE0 00003474 8FA3009C */  lw         $v1, 0x9C($sp)
    /* DD3EE4 00003478 8FA60088 */  lw         $a2, 0x88($sp)
    /* DD3EE8 0000347C AC4A0014 */  sw         $t2, 0x14($v0)
    /* DD3EEC 00003480 C47203A4 */  lwc1       $f18, 0x3A4($v1)
    /* DD3EF0 00003484 44800000 */  mtc1       $zero, $f0
    /* DD3EF4 00003488 3C01C47A */  lui        $at, (0xC47A0000 >> 16)
    /* DD3EF8 0000348C E4520060 */  swc1       $f18, 0x60($v0)
    /* DD3EFC 00003490 C46403A8 */  lwc1       $f4, 0x3A8($v1)
    /* DD3F00 00003494 44819000 */  mtc1       $at, $f18
    /* DD3F04 00003498 27A5002C */  addiu      $a1, $sp, 0x2C
    /* DD3F08 0000349C E4440064 */  swc1       $f4, 0x64($v0)
    /* DD3F0C 000034A0 C46603AC */  lwc1       $f6, 0x3AC($v1)
    /* DD3F10 000034A4 24C40030 */  addiu      $a0, $a2, 0x30
    /* DD3F14 000034A8 E4460068 */  swc1       $f6, 0x68($v0)
    /* DD3F18 000034AC C46803A4 */  lwc1       $f8, 0x3A4($v1)
    /* DD3F1C 000034B0 E4C80060 */  swc1       $f8, 0x60($a2)
    /* DD3F20 000034B4 C46A03A8 */  lwc1       $f10, 0x3A8($v1)
    /* DD3F24 000034B8 E4CA0064 */  swc1       $f10, 0x64($a2)
    /* DD3F28 000034BC C47003AC */  lwc1       $f16, 0x3AC($v1)
    /* DD3F2C 000034C0 E4D00068 */  swc1       $f16, 0x68($a2)
    /* DD3F30 000034C4 E7A00030 */  swc1       $f0, 0x30($sp)
    /* DD3F34 000034C8 E7A0002C */  swc1       $f0, 0x2C($sp)
    /* DD3F38 000034CC 0C000000 */  jal        func_00000000
    /* DD3F3C 000034D0 E7B20034 */   swc1      $f18, 0x34($sp)
    /* DD3F40 000034D4 8FBF0024 */  lw         $ra, 0x24($sp)
    /* DD3F44 000034D8 8FA20088 */  lw         $v0, 0x88($sp)
    /* DD3F48 000034DC 27BD0090 */  addiu      $sp, $sp, 0x90
    /* DD3F4C 000034E0 03E00008 */  jr         $ra
    /* DD3F50 000034E4 00000000 */   nop
endlabel func_000031C0
