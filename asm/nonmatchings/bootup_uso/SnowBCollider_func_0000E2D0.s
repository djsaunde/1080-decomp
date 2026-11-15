nonmatching SnowBCollider_func_0000E2D0, 0x20C

glabel SnowBCollider_func_0000E2D0
    /* DDED3C 0000E2D0 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* DDED40 0000E2D4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDED44 0000E2D8 AFB00018 */  sw         $s0, 0x18($sp)
    /* DDED48 0000E2DC AFA50074 */  sw         $a1, 0x74($sp)
    /* DDED4C 0000E2E0 C4C40000 */  lwc1       $f4, 0x0($a2)
    /* DDED50 0000E2E4 00808025 */  or         $s0, $a0, $zero
    /* DDED54 0000E2E8 E7A40064 */  swc1       $f4, 0x64($sp)
    /* DDED58 0000E2EC C4C60004 */  lwc1       $f6, 0x4($a2)
    /* DDED5C 0000E2F0 C7A40064 */  lwc1       $f4, 0x64($sp)
    /* DDED60 0000E2F4 E7A60068 */  swc1       $f6, 0x68($sp)
    /* DDED64 0000E2F8 C4C80008 */  lwc1       $f8, 0x8($a2)
    /* DDED68 0000E2FC C7AA0068 */  lwc1       $f10, 0x68($sp)
    /* DDED6C 0000E300 E7A8006C */  swc1       $f8, 0x6C($sp)
    /* DDED70 0000E304 C4F00004 */  lwc1       $f16, 0x4($a3)
    /* DDED74 0000E308 C4E20000 */  lwc1       $f2, 0x0($a3)
    /* DDED78 0000E30C 46105482 */  mul.s      $f18, $f10, $f16
    /* DDED7C 0000E310 C7B0006C */  lwc1       $f16, 0x6C($sp)
    /* DDED80 0000E314 46022182 */  mul.s      $f6, $f4, $f2
    /* DDED84 0000E318 46123200 */  add.s      $f8, $f6, $f18
    /* DDED88 0000E31C C4E60008 */  lwc1       $f6, 0x8($a3)
    /* DDED8C 0000E320 46068482 */  mul.s      $f18, $f16, $f6
    /* DDED90 0000E324 46124000 */  add.s      $f0, $f8, $f18
    /* DDED94 0000E328 46020182 */  mul.s      $f6, $f0, $f2
    /* DDED98 0000E32C 46043201 */  sub.s      $f8, $f6, $f4
    /* DDED9C 0000E330 E7A80064 */  swc1       $f8, 0x64($sp)
    /* DDEDA0 0000E334 C4F20004 */  lwc1       $f18, 0x4($a3)
    /* DDEDA4 0000E338 46120182 */  mul.s      $f6, $f0, $f18
    /* DDEDA8 0000E33C 460A3101 */  sub.s      $f4, $f6, $f10
    /* DDEDAC 0000E340 E7A40068 */  swc1       $f4, 0x68($sp)
    /* DDEDB0 0000E344 C4E80008 */  lwc1       $f8, 0x8($a3)
    /* DDEDB4 0000E348 46080482 */  mul.s      $f18, $f0, $f8
    /* DDEDB8 0000E34C 46109181 */  sub.s      $f6, $f18, $f16
    /* DDEDBC 0000E350 E7A6006C */  swc1       $f6, 0x6C($sp)
    /* DDEDC0 0000E354 C48A03BC */  lwc1       $f10, 0x3BC($a0)
    /* DDEDC4 0000E358 E7AA0058 */  swc1       $f10, 0x58($sp)
    /* DDEDC8 0000E35C C48403C0 */  lwc1       $f4, 0x3C0($a0)
    /* DDEDCC 0000E360 C7AA0058 */  lwc1       $f10, 0x58($sp)
    /* DDEDD0 0000E364 E7A4005C */  swc1       $f4, 0x5C($sp)
    /* DDEDD4 0000E368 C48803C4 */  lwc1       $f8, 0x3C4($a0)
    /* DDEDD8 0000E36C C7B2005C */  lwc1       $f18, 0x5C($sp)
    /* DDEDDC 0000E370 E7A80060 */  swc1       $f8, 0x60($sp)
    /* DDEDE0 0000E374 C4F00004 */  lwc1       $f16, 0x4($a3)
    /* DDEDE4 0000E378 C4E20000 */  lwc1       $f2, 0x0($a3)
    /* DDEDE8 0000E37C 46109182 */  mul.s      $f6, $f18, $f16
    /* DDEDEC 0000E380 C7B00060 */  lwc1       $f16, 0x60($sp)
    /* DDEDF0 0000E384 46025102 */  mul.s      $f4, $f10, $f2
    /* DDEDF4 0000E388 46062200 */  add.s      $f8, $f4, $f6
    /* DDEDF8 0000E38C C4E40008 */  lwc1       $f4, 0x8($a3)
    /* DDEDFC 0000E390 46048182 */  mul.s      $f6, $f16, $f4
    /* DDEE00 0000E394 46064000 */  add.s      $f0, $f8, $f6
    /* DDEE04 0000E398 46020102 */  mul.s      $f4, $f0, $f2
    /* DDEE08 0000E39C 460A2201 */  sub.s      $f8, $f4, $f10
    /* DDEE0C 0000E3A0 E7A80058 */  swc1       $f8, 0x58($sp)
    /* DDEE10 0000E3A4 C4E60004 */  lwc1       $f6, 0x4($a3)
    /* DDEE14 0000E3A8 46060102 */  mul.s      $f4, $f0, $f6
    /* DDEE18 0000E3AC 46122281 */  sub.s      $f10, $f4, $f18
    /* DDEE1C 0000E3B0 E7AA005C */  swc1       $f10, 0x5C($sp)
    /* DDEE20 0000E3B4 C4E60008 */  lwc1       $f6, 0x8($a3)
    /* DDEE24 0000E3B8 AFA7007C */  sw         $a3, 0x7C($sp)
    /* DDEE28 0000E3BC 46060102 */  mul.s      $f4, $f0, $f6
    /* DDEE2C 0000E3C0 46102481 */  sub.s      $f18, $f4, $f16
    /* DDEE30 0000E3C4 46084182 */  mul.s      $f6, $f8, $f8
    /* DDEE34 0000E3C8 00000000 */  nop
    /* DDEE38 0000E3CC 460A5102 */  mul.s      $f4, $f10, $f10
    /* DDEE3C 0000E3D0 E7B20060 */  swc1       $f18, 0x60($sp)
    /* DDEE40 0000E3D4 46129202 */  mul.s      $f8, $f18, $f18
    /* DDEE44 0000E3D8 46043400 */  add.s      $f16, $f6, $f4
    /* DDEE48 0000E3DC 0C000000 */  jal        func_00000000
    /* DDEE4C 0000E3E0 46088300 */   add.s     $f12, $f16, $f8
    /* DDEE50 0000E3E4 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent1_0000098C + 0x14)
    /* DDEE54 0000E3E8 D42A09A0 */  ldc1       $f10, %lo(SndHandle_triggerEvent1_0000098C + 0x14)($at)
    /* DDEE58 0000E3EC 460001A1 */  cvt.d.s    $f6, $f0
    /* DDEE5C 0000E3F0 8FA7007C */  lw         $a3, 0x7C($sp)
    /* DDEE60 0000E3F4 4626503C */  c.lt.d     $f10, $f6
    /* DDEE64 0000E3F8 C7AA005C */  lwc1       $f10, 0x5C($sp)
    /* DDEE68 0000E3FC C7A40064 */  lwc1       $f4, 0x64($sp)
    /* DDEE6C 0000E400 C7B20058 */  lwc1       $f18, 0x58($sp)
    /* DDEE70 0000E404 45000030 */  bc1f       .L0000E4C8
    /* DDEE74 0000E408 C7A80068 */   lwc1      $f8, 0x68($sp)
    /* DDEE78 0000E40C 46122402 */  mul.s      $f16, $f4, $f18
    /* DDEE7C 0000E410 C7B2006C */  lwc1       $f18, 0x6C($sp)
    /* DDEE80 0000E414 27A40058 */  addiu      $a0, $sp, 0x58
    /* DDEE84 0000E418 460A4182 */  mul.s      $f6, $f8, $f10
    /* DDEE88 0000E41C C7A80060 */  lwc1       $f8, 0x60($sp)
    /* DDEE8C 0000E420 AFA7007C */  sw         $a3, 0x7C($sp)
    /* DDEE90 0000E424 46089282 */  mul.s      $f10, $f18, $f8
    /* DDEE94 0000E428 46068100 */  add.s      $f4, $f16, $f6
    /* DDEE98 0000E42C 460A2000 */  add.s      $f0, $f4, $f10
    /* DDEE9C 0000E430 0C000000 */  jal        func_00000000
    /* DDEEA0 0000E434 E7A00044 */   swc1      $f0, 0x44($sp)
    /* DDEEA4 0000E438 C7A00044 */  lwc1       $f0, 0x44($sp)
    /* DDEEA8 0000E43C C61009F8 */  lwc1       $f16, 0x9F8($s0)
    /* DDEEAC 0000E440 C60209FC */  lwc1       $f2, 0x9FC($s0)
    /* DDEEB0 0000E444 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDEEB4 0000E448 46100002 */  mul.s      $f0, $f0, $f16
    /* DDEEB8 0000E44C 46001307 */  neg.s      $f12, $f2
    /* DDEEBC 0000E450 C7B00080 */  lwc1       $f16, 0x80($sp)
    /* DDEEC0 0000E454 44814000 */  mtc1       $at, $f8
    /* DDEEC4 0000E458 00000000 */  nop
    /* DDEEC8 0000E45C 46104480 */  add.s      $f18, $f8, $f16
    /* DDEECC 0000E460 460C003C */  c.lt.s     $f0, $f12
    /* DDEED0 0000E464 00000000 */  nop
    /* DDEED4 0000E468 45020003 */  bc1fl      .L0000E478
    /* DDEED8 0000E46C 4600103C */   c.lt.s    $f2, $f0
    /* DDEEDC 0000E470 46006006 */  mov.s      $f0, $f12
    /* DDEEE0 0000E474 4600103C */  c.lt.s     $f2, $f0
  .L0000E478:
    /* DDEEE4 0000E478 00000000 */  nop
    /* DDEEE8 0000E47C 45000002 */  bc1f       .L0000E488
    /* DDEEEC 0000E480 00000000 */   nop
    /* DDEEF0 0000E484 46001006 */  mov.s      $f0, $f2
  .L0000E488:
    /* DDEEF4 0000E488 46120002 */  mul.s      $f0, $f0, $f18
    /* DDEEF8 0000E48C C7A40058 */  lwc1       $f4, 0x58($sp)
    /* DDEEFC 0000E490 C7AA005C */  lwc1       $f10, 0x5C($sp)
    /* DDEF00 0000E494 C7B00060 */  lwc1       $f16, 0x60($sp)
    /* DDEF04 0000E498 02002025 */  or         $a0, $s0, $zero
    /* DDEF08 0000E49C 8FA50074 */  lw         $a1, 0x74($sp)
    /* DDEF0C 0000E4A0 27A60058 */  addiu      $a2, $sp, 0x58
    /* DDEF10 0000E4A4 46002182 */  mul.s      $f6, $f4, $f0
    /* DDEF14 0000E4A8 00000000 */  nop
    /* DDEF18 0000E4AC 46005202 */  mul.s      $f8, $f10, $f0
    /* DDEF1C 0000E4B0 00000000 */  nop
    /* DDEF20 0000E4B4 46008482 */  mul.s      $f18, $f16, $f0
    /* DDEF24 0000E4B8 E7A60058 */  swc1       $f6, 0x58($sp)
    /* DDEF28 0000E4BC E7A8005C */  swc1       $f8, 0x5C($sp)
    /* DDEF2C 0000E4C0 0C000000 */  jal        func_00000000
    /* DDEF30 0000E4C4 E7B20060 */   swc1      $f18, 0x60($sp)
  .L0000E4C8:
    /* DDEF34 0000E4C8 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DDEF38 0000E4CC 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDEF3C 0000E4D0 27BD0070 */  addiu      $sp, $sp, 0x70
    /* DDEF40 0000E4D4 03E00008 */  jr         $ra
    /* DDEF44 0000E4D8 00000000 */   nop
endlabel SnowBCollider_func_0000E2D0
