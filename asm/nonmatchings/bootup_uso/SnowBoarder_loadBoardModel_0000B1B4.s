nonmatching SnowBoarder_loadBoardModel_0000B1B4, 0x2E8

glabel SnowBoarder_loadBoardModel_0000B1B4
    /* DDBC20 0000B1B4 27BDFF80 */  addiu      $sp, $sp, -0x80
    /* DDBC24 0000B1B8 2CA1000A */  sltiu      $at, $a1, 0xA
    /* DDBC28 0000B1BC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDBC2C 0000B1C0 AFA40080 */  sw         $a0, 0x80($sp)
    /* DDBC30 0000B1C4 AFA50084 */  sw         $a1, 0x84($sp)
    /* DDBC34 0000B1C8 14200003 */  bnez       $at, .L0000B1D8
    /* DDBC38 0000B1CC AFA7008C */   sw        $a3, 0x8C($sp)
    /* DDBC3C 0000B1D0 240F0009 */  addiu      $t7, $zero, 0x9
    /* DDBC40 0000B1D4 AFAF0084 */  sw         $t7, 0x84($sp)
  .L0000B1D8:
    /* DDBC44 0000B1D8 8FB80080 */  lw         $t8, 0x80($sp)
    /* DDBC48 0000B1DC 24190008 */  addiu      $t9, $zero, 0x8
    /* DDBC4C 0000B1E0 2F010009 */  sltiu      $at, $t8, 0x9
    /* DDBC50 0000B1E4 54200003 */  bnel       $at, $zero, .L0000B1F4
    /* DDBC54 0000B1E8 8FA80090 */   lw        $t0, 0x90($sp)
    /* DDBC58 0000B1EC AFB90080 */  sw         $t9, 0x80($sp)
    /* DDBC5C 0000B1F0 8FA80090 */  lw         $t0, 0x90($sp)
  .L0000B1F4:
    /* DDBC60 0000B1F4 8FA90080 */  lw         $t1, 0x80($sp)
    /* DDBC64 0000B1F8 8FAA0084 */  lw         $t2, 0x84($sp)
    /* DDBC68 0000B1FC 24010007 */  addiu      $at, $zero, 0x7
    /* DDBC6C 0000B200 AFA6007C */  sw         $a2, 0x7C($sp)
    /* DDBC70 0000B204 AFA80040 */  sw         $t0, 0x40($sp)
    /* DDBC74 0000B208 AFA80044 */  sw         $t0, 0x44($sp)
    /* DDBC78 0000B20C AFA90074 */  sw         $t1, 0x74($sp)
    /* DDBC7C 0000B210 11210003 */  beq        $t1, $at, .L0000B220
    /* DDBC80 0000B214 AFAA0078 */   sw        $t2, 0x78($sp)
    /* DDBC84 0000B218 24010006 */  addiu      $at, $zero, 0x6
    /* DDBC88 0000B21C 1521006C */  bne        $t1, $at, .L0000B3D0
  .L0000B220:
    /* DDBC8C 0000B220 3C0B0000 */   lui       $t3, %hi(snd_seqEventDbg_00000148)
    /* DDBC90 0000B224 8D6B0148 */  lw         $t3, %lo(snd_seqEventDbg_00000148)($t3)
    /* DDBC94 0000B228 3C040001 */  lui        $a0, %hi(stream_read16_000089FC + 0x2C)
    /* DDBC98 0000B22C 15600068 */  bnez       $t3, .L0000B3D0
    /* DDBC9C 0000B230 00000000 */   nop
    /* DDBCA0 0000B234 0C000000 */  jal        func_00000000
    /* DDBCA4 0000B238 24848A28 */   addiu     $a0, $a0, %lo(stream_read16_000089FC + 0x2C)
    /* DDBCA8 0000B23C 0C000000 */  jal        func_00000000
    /* DDBCAC 0000B240 24040090 */   addiu     $a0, $zero, 0x90
    /* DDBCB0 0000B244 10400051 */  beqz       $v0, .L0000B38C
    /* DDBCB4 0000B248 00403025 */   or        $a2, $v0, $zero
    /* DDBCB8 0000B24C 14400007 */  bnez       $v0, .L0000B26C
    /* DDBCBC 0000B250 00403825 */   or        $a3, $v0, $zero
    /* DDBCC0 0000B254 24040090 */  addiu      $a0, $zero, 0x90
    /* DDBCC4 0000B258 0C000000 */  jal        func_00000000
    /* DDBCC8 0000B25C AFA2002C */   sw        $v0, 0x2C($sp)
    /* DDBCCC 0000B260 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DDBCD0 0000B264 10400034 */  beqz       $v0, .L0000B338
    /* DDBCD4 0000B268 00403825 */   or        $a3, $v0, $zero
  .L0000B26C:
    /* DDBCD8 0000B26C 14E00009 */  bnez       $a3, .L0000B294
    /* DDBCDC 0000B270 00E02825 */   or        $a1, $a3, $zero
    /* DDBCE0 0000B274 24040060 */  addiu      $a0, $zero, 0x60
    /* DDBCE4 0000B278 AFA6002C */  sw         $a2, 0x2C($sp)
    /* DDBCE8 0000B27C 0C000000 */  jal        func_00000000
    /* DDBCEC 0000B280 AFA70028 */   sw        $a3, 0x28($sp)
    /* DDBCF0 0000B284 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DDBCF4 0000B288 8FA70028 */  lw         $a3, 0x28($sp)
    /* DDBCF8 0000B28C 10400027 */  beqz       $v0, .L0000B32C
    /* DDBCFC 0000B290 00402825 */   or        $a1, $v0, $zero
  .L0000B294:
    /* DDBD00 0000B294 3C0C0000 */  lui        $t4, (0x0 >> 16)
    /* DDBD04 0000B298 258C0000 */  addiu      $t4, $t4, 0x0
    /* DDBD08 0000B29C 2401FFCC */  addiu      $at, $zero, -0x34
    /* DDBD0C 0000B2A0 ACAC005C */  sw         $t4, 0x5C($a1)
    /* DDBD10 0000B2A4 14A1000B */  bne        $a1, $at, .L0000B2D4
    /* DDBD14 0000B2A8 24A30034 */   addiu     $v1, $a1, 0x34
    /* DDBD18 0000B2AC 24040008 */  addiu      $a0, $zero, 0x8
    /* DDBD1C 0000B2B0 AFA50024 */  sw         $a1, 0x24($sp)
    /* DDBD20 0000B2B4 AFA6002C */  sw         $a2, 0x2C($sp)
    /* DDBD24 0000B2B8 0C000000 */  jal        func_00000000
    /* DDBD28 0000B2BC AFA70028 */   sw        $a3, 0x28($sp)
    /* DDBD2C 0000B2C0 8FA50024 */  lw         $a1, 0x24($sp)
    /* DDBD30 0000B2C4 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DDBD34 0000B2C8 8FA70028 */  lw         $a3, 0x28($sp)
    /* DDBD38 0000B2CC 10400003 */  beqz       $v0, .L0000B2DC
    /* DDBD3C 0000B2D0 00401825 */   or        $v1, $v0, $zero
  .L0000B2D4:
    /* DDBD40 0000B2D4 AC600004 */  sw         $zero, 0x4($v1)
    /* DDBD44 0000B2D8 AC600000 */  sw         $zero, 0x0($v1)
  .L0000B2DC:
    /* DDBD48 0000B2DC 2401FFA8 */  addiu      $at, $zero, -0x58
    /* DDBD4C 0000B2E0 14A1000B */  bne        $a1, $at, .L0000B310
    /* DDBD50 0000B2E4 24A30058 */   addiu     $v1, $a1, 0x58
    /* DDBD54 0000B2E8 24040004 */  addiu      $a0, $zero, 0x4
    /* DDBD58 0000B2EC AFA50024 */  sw         $a1, 0x24($sp)
    /* DDBD5C 0000B2F0 AFA6002C */  sw         $a2, 0x2C($sp)
    /* DDBD60 0000B2F4 0C000000 */  jal        func_00000000
    /* DDBD64 0000B2F8 AFA70028 */   sw        $a3, 0x28($sp)
    /* DDBD68 0000B2FC 8FA50024 */  lw         $a1, 0x24($sp)
    /* DDBD6C 0000B300 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DDBD70 0000B304 8FA70028 */  lw         $a3, 0x28($sp)
    /* DDBD74 0000B308 10400002 */  beqz       $v0, .L0000B314
    /* DDBD78 0000B30C 00401825 */   or        $v1, $v0, $zero
  .L0000B310:
    /* DDBD7C 0000B310 AC600000 */  sw         $zero, 0x0($v1)
  .L0000B314:
    /* DDBD80 0000B314 00A02025 */  or         $a0, $a1, $zero
    /* DDBD84 0000B318 AFA6002C */  sw         $a2, 0x2C($sp)
    /* DDBD88 0000B31C 0C000000 */  jal        func_00000000
    /* DDBD8C 0000B320 AFA70028 */   sw        $a3, 0x28($sp)
    /* DDBD90 0000B324 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DDBD94 0000B328 8FA70028 */  lw         $a3, 0x28($sp)
  .L0000B32C:
    /* DDBD98 0000B32C 3C0D0000 */  lui        $t5, (0x0 >> 16)
    /* DDBD9C 0000B330 25AD0000 */  addiu      $t5, $t5, 0x0
    /* DDBDA0 0000B334 ACED005C */  sw         $t5, 0x5C($a3)
  .L0000B338:
    /* DDBDA4 0000B338 3C0E0001 */  lui        $t6, %hi(Snower_hostioParms + 0x70)
    /* DDBDA8 0000B33C 25CE8440 */  addiu      $t6, $t6, %lo(Snower_hostioParms + 0x70)
    /* DDBDAC 0000B340 240F0110 */  addiu      $t7, $zero, 0x110
    /* DDBDB0 0000B344 ACCF0024 */  sw         $t7, 0x24($a2)
    /* DDBDB4 0000B348 ACCE005C */  sw         $t6, 0x5C($a2)
    /* DDBDB8 0000B34C AFA6002C */  sw         $a2, 0x2C($sp)
    /* DDBDBC 0000B350 0C000000 */  jal        func_00000000
    /* DDBDC0 0000B354 24041000 */   addiu     $a0, $zero, 0x1000
    /* DDBDC4 0000B358 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DDBDC8 0000B35C 3C0A0040 */  lui        $t2, (0x402C4B >> 16)
    /* DDBDCC 0000B360 354A2C4B */  ori        $t2, $t2, (0x402C4B & 0xFFFF)
    /* DDBDD0 0000B364 24180040 */  addiu      $t8, $zero, 0x40
    /* DDBDD4 0000B368 24190020 */  addiu      $t9, $zero, 0x20
    /* DDBDD8 0000B36C 24081100 */  addiu      $t0, $zero, 0x1100
    /* DDBDDC 0000B370 ACC80028 */  sw         $t0, 0x28($a2)
    /* DDBDE0 0000B374 A4D90022 */  sh         $t9, 0x22($a2)
    /* DDBDE4 0000B378 A4D80020 */  sh         $t8, 0x20($a2)
    /* DDBDE8 0000B37C ACCA0038 */  sw         $t2, 0x38($a2)
    /* DDBDEC 0000B380 ACC2001C */  sw         $v0, 0x1C($a2)
    /* DDBDF0 0000B384 ACC00034 */  sw         $zero, 0x34($a2)
    /* DDBDF4 0000B388 ACC00018 */  sw         $zero, 0x18($a2)
  .L0000B38C:
    /* DDBDF8 0000B38C 3C090000 */  lui        $t1, %hi(snd_seqEventDbg_00000148 + 0x14)
    /* DDBDFC 0000B390 8D29015C */  lw         $t1, %lo(snd_seqEventDbg_00000148 + 0x14)($t1)
    /* DDBE00 0000B394 29210005 */  slti       $at, $t1, 0x5
    /* DDBE04 0000B398 14200004 */  bnez       $at, .L0000B3AC
    /* DDBE08 0000B39C 00000000 */   nop
    /* DDBE0C 0000B3A0 0C000000 */  jal        func_00000000
    /* DDBE10 0000B3A4 AFA6002C */   sw        $a2, 0x2C($sp)
    /* DDBE14 0000B3A8 8FA6002C */  lw         $a2, 0x2C($sp)
  .L0000B3AC:
    /* DDBE18 0000B3AC 3C050000 */  lui        $a1, %hi(snd_seqEventDbg_00000148 + 0x14)
    /* DDBE1C 0000B3B0 8CA5015C */  lw         $a1, %lo(snd_seqEventDbg_00000148 + 0x14)($a1)
    /* DDBE20 0000B3B4 3C010000 */  lui        $at, %hi(snd_seqEventDbg_00000148 + 0x14)
    /* DDBE24 0000B3B8 24AB0001 */  addiu      $t3, $a1, 0x1
    /* DDBE28 0000B3BC AC2B015C */  sw         $t3, %lo(snd_seqEventDbg_00000148 + 0x14)($at)
    /* DDBE2C 0000B3C0 3C010000 */  lui        $at, %hi(snd_seqEventDbg_00000148)
    /* DDBE30 0000B3C4 00056080 */  sll        $t4, $a1, 2
    /* DDBE34 0000B3C8 002C0821 */  addu       $at, $at, $t4
    /* DDBE38 0000B3CC AC260148 */  sw         $a2, %lo(snd_seqEventDbg_00000148)($at)
  .L0000B3D0:
    /* DDBE3C 0000B3D0 3C040001 */  lui        $a0, %hi(stream_readF32_00008A40 + 0x8)
    /* DDBE40 0000B3D4 24848A48 */  addiu      $a0, $a0, %lo(stream_readF32_00008A40 + 0x8)
    /* DDBE44 0000B3D8 0C000000 */  jal        func_00000000
    /* DDBE48 0000B3DC 00002825 */   or        $a1, $zero, $zero
    /* DDBE4C 0000B3E0 3C040001 */  lui        $a0, %hi(stream_readF32_00008A40 + 0x14)
    /* DDBE50 0000B3E4 24848A54 */  addiu      $a0, $a0, %lo(stream_readF32_00008A40 + 0x14)
    /* DDBE54 0000B3E8 0C000000 */  jal        func_00000000
    /* DDBE58 0000B3EC 00002825 */   or        $a1, $zero, $zero
    /* DDBE5C 0000B3F0 8FAD0084 */  lw         $t5, 0x84($sp)
    /* DDBE60 0000B3F4 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DDBE64 0000B3F8 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DDBE68 0000B3FC 11A0000B */  beqz       $t5, .L0000B42C
    /* DDBE6C 0000B400 000D7080 */   sll       $t6, $t5, 2
    /* DDBE70 0000B404 01CF1821 */  addu       $v1, $t6, $t7
    /* DDBE74 0000B408 8C640000 */  lw         $a0, 0x0($v1)
    /* DDBE78 0000B40C 0C000000 */  jal        func_00000000
    /* DDBE7C 0000B410 AFA30018 */   sw        $v1, 0x18($sp)
    /* DDBE80 0000B414 14400005 */  bnez       $v0, .L0000B42C
    /* DDBE84 0000B418 8FA30018 */   lw        $v1, 0x18($sp)
    /* DDBE88 0000B41C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDBE8C 0000B420 24840000 */  addiu      $a0, $a0, 0x0
    /* DDBE90 0000B424 0C000000 */  jal        func_00000000
    /* DDBE94 0000B428 8C650000 */   lw        $a1, 0x0($v1)
  .L0000B42C:
    /* DDBE98 0000B42C 8FB80080 */  lw         $t8, 0x80($sp)
    /* DDBE9C 0000B430 3C080000 */  lui        $t0, (0x0 >> 16)
    /* DDBEA0 0000B434 25080000 */  addiu      $t0, $t0, 0x0
    /* DDBEA4 0000B438 0018C880 */  sll        $t9, $t8, 2
    /* DDBEA8 0000B43C 03281821 */  addu       $v1, $t9, $t0
    /* DDBEAC 0000B440 8C640000 */  lw         $a0, 0x0($v1)
    /* DDBEB0 0000B444 0C000000 */  jal        func_00000000
    /* DDBEB4 0000B448 AFA30018 */   sw        $v1, 0x18($sp)
    /* DDBEB8 0000B44C 14400005 */  bnez       $v0, .L0000B464
    /* DDBEBC 0000B450 8FA30018 */   lw        $v1, 0x18($sp)
    /* DDBEC0 0000B454 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDBEC4 0000B458 24840000 */  addiu      $a0, $a0, 0x0
    /* DDBEC8 0000B45C 0C000000 */  jal        func_00000000
    /* DDBECC 0000B460 8C650000 */   lw        $a1, 0x0($v1)
  .L0000B464:
    /* DDBED0 0000B464 0C000000 */  jal        func_00000000
    /* DDBED4 0000B468 00000000 */   nop
    /* DDBED8 0000B46C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDBEDC 0000B470 24840000 */  addiu      $a0, $a0, 0x0
    /* DDBEE0 0000B474 8FA50090 */  lw         $a1, 0x90($sp)
    /* DDBEE4 0000B478 8FA6008C */  lw         $a2, 0x8C($sp)
    /* DDBEE8 0000B47C 0C000000 */  jal        func_00000000
    /* DDBEEC 0000B480 27A70034 */   addiu     $a3, $sp, 0x34
    /* DDBEF0 0000B484 0C000000 */  jal        func_00000000
    /* DDBEF4 0000B488 00000000 */   nop
    /* DDBEF8 0000B48C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDBEFC 0000B490 27BD0080 */  addiu      $sp, $sp, 0x80
    /* DDBF00 0000B494 03E00008 */  jr         $ra
    /* DDBF04 0000B498 00000000 */   nop
endlabel SnowBoarder_loadBoardModel_0000B1B4
