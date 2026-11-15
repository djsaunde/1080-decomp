nonmatching SndHandle_triggerEvent2_000009D8, 0x4C

glabel SndHandle_triggerEvent2_000009D8
    /* DD1444 000009D8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD1448 000009DC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD144C 000009E0 AFA50024 */  sw         $a1, 0x24($sp)
    /* DD1450 000009E4 8C850008 */  lw         $a1, 0x8($a0)
    /* DD1454 000009E8 0C000000 */  jal        func_00000000
    /* DD1458 000009EC AFA40020 */   sw        $a0, 0x20($sp)
    /* DD145C 000009F0 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD1460 000009F4 00402825 */  or         $a1, $v0, $zero
    /* DD1464 000009F8 24060002 */  addiu      $a2, $zero, 0x2
    /* DD1468 000009FC 8C8E0000 */  lw         $t6, 0x0($a0)
    /* DD146C 00000A00 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD1470 00000A04 11C00003 */  beqz       $t6, .L00000A14
    /* DD1474 00000A08 AFAE0018 */   sw        $t6, 0x18($sp)
    /* DD1478 00000A0C 0C000000 */  jal        func_00000000
    /* DD147C 00000A10 01C02025 */   or        $a0, $t6, $zero
  .L00000A14:
    /* DD1480 00000A14 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1484 00000A18 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD1488 00000A1C 03E00008 */  jr         $ra
    /* DD148C 00000A20 00000000 */   nop
endlabel SndHandle_triggerEvent2_000009D8
