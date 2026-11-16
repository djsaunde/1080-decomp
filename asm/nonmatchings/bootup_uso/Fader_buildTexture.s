nonmatching Fader_buildTexture, 0x144

glabel Fader_buildTexture
    /* DE458C 00013B20 3C030000 */  lui        $v1, %hi(snd_setPlayer0VolumeLevel_0000023C + 0x4)
    /* DE4590 00013B24 8C630240 */  lw         $v1, %lo(snd_setPlayer0VolumeLevel_0000023C + 0x4)($v1)
    /* DE4594 00013B28 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE4598 00013B2C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE459C 00013B30 8C620028 */  lw         $v0, 0x28($v1)
    /* DE45A0 00013B34 00802825 */  or         $a1, $a0, $zero
    /* DE45A4 00013B38 844E0060 */  lh         $t6, 0x60($v0)
    /* DE45A8 00013B3C AFA50018 */  sw         $a1, 0x18($sp)
    /* DE45AC 00013B40 8C590064 */  lw         $t9, 0x64($v0)
    /* DE45B0 00013B44 01C32021 */  addu       $a0, $t6, $v1
    /* DE45B4 00013B48 0320F809 */  jalr       $t9
    /* DE45B8 00013B4C 00000000 */   nop
    /* DE45BC 00013B50 8FA50018 */  lw         $a1, 0x18($sp)
    /* DE45C0 00013B54 24061000 */  addiu      $a2, $zero, 0x1000
    /* DE45C4 00013B58 00001825 */  or         $v1, $zero, $zero
    /* DE45C8 00013B5C 8CAF0068 */  lw         $t7, 0x68($a1)
    /* DE45CC 00013B60 24040800 */  addiu      $a0, $zero, 0x800
    /* DE45D0 00013B64 51E0002D */  beql       $t7, $zero, .L00013C1C
    /* DE45D4 00013B68 24020001 */   addiu     $v0, $zero, 0x1
    /* DE45D8 00013B6C 00002025 */  or         $a0, $zero, $zero
    /* DE45DC 00013B70 00001825 */  or         $v1, $zero, $zero
  .L00013B74:
    /* DE45E0 00013B74 8CB80054 */  lw         $t8, 0x54($a1)
    /* DE45E4 00013B78 8CAD004C */  lw         $t5, 0x4C($a1)
    /* DE45E8 00013B7C 03034021 */  addu       $t0, $t8, $v1
    /* DE45EC 00013B80 8D090000 */  lw         $t1, 0x0($t0)
    /* DE45F0 00013B84 01A47021 */  addu       $t6, $t5, $a0
    /* DE45F4 00013B88 00095040 */  sll        $t2, $t1, 1
    /* DE45F8 00013B8C 004A5821 */  addu       $t3, $v0, $t2
    /* DE45FC 00013B90 956C0000 */  lhu        $t4, 0x0($t3)
    /* DE4600 00013B94 A5CC0000 */  sh         $t4, 0x0($t6)
    /* DE4604 00013B98 8CB90054 */  lw         $t9, 0x54($a1)
    /* DE4608 00013B9C 8CAB004C */  lw         $t3, 0x4C($a1)
    /* DE460C 00013BA0 03237821 */  addu       $t7, $t9, $v1
    /* DE4610 00013BA4 8DF80004 */  lw         $t8, 0x4($t7)
    /* DE4614 00013BA8 01646821 */  addu       $t5, $t3, $a0
    /* DE4618 00013BAC 00184040 */  sll        $t0, $t8, 1
    /* DE461C 00013BB0 00484821 */  addu       $t1, $v0, $t0
    /* DE4620 00013BB4 952A0000 */  lhu        $t2, 0x0($t1)
    /* DE4624 00013BB8 A5AA0002 */  sh         $t2, 0x2($t5)
    /* DE4628 00013BBC 8CAC0054 */  lw         $t4, 0x54($a1)
    /* DE462C 00013BC0 8CA9004C */  lw         $t1, 0x4C($a1)
    /* DE4630 00013BC4 01837021 */  addu       $t6, $t4, $v1
    /* DE4634 00013BC8 8DD90008 */  lw         $t9, 0x8($t6)
    /* DE4638 00013BCC 01245821 */  addu       $t3, $t1, $a0
    /* DE463C 00013BD0 00197840 */  sll        $t7, $t9, 1
    /* DE4640 00013BD4 004FC021 */  addu       $t8, $v0, $t7
    /* DE4644 00013BD8 97080000 */  lhu        $t0, 0x0($t8)
    /* DE4648 00013BDC A5680004 */  sh         $t0, 0x4($t3)
    /* DE464C 00013BE0 8CAA0054 */  lw         $t2, 0x54($a1)
    /* DE4650 00013BE4 8CB8004C */  lw         $t8, 0x4C($a1)
    /* DE4654 00013BE8 01436821 */  addu       $t5, $t2, $v1
    /* DE4658 00013BEC 8DAC000C */  lw         $t4, 0xC($t5)
    /* DE465C 00013BF0 24630010 */  addiu      $v1, $v1, 0x10
    /* DE4660 00013BF4 03044821 */  addu       $t1, $t8, $a0
    /* DE4664 00013BF8 000C7040 */  sll        $t6, $t4, 1
    /* DE4668 00013BFC 004EC821 */  addu       $t9, $v0, $t6
    /* DE466C 00013C00 972F0000 */  lhu        $t7, 0x0($t9)
    /* DE4670 00013C04 24840008 */  addiu      $a0, $a0, 0x8
    /* DE4674 00013C08 1466FFDA */  bne        $v1, $a2, .L00013B74
    /* DE4678 00013C0C A52F0006 */   sh        $t7, 0x6($t1)
    /* DE467C 00013C10 10000011 */  b          .L00013C58
    /* DE4680 00013C14 8FBF0014 */   lw        $ra, 0x14($sp)
    /* DE4684 00013C18 24020001 */  addiu      $v0, $zero, 0x1
  .L00013C1C:
    /* DE4688 00013C1C 8CA8004C */  lw         $t0, 0x4C($a1)
    /* DE468C 00013C20 01035821 */  addu       $t3, $t0, $v1
    /* DE4690 00013C24 A5620000 */  sh         $v0, 0x0($t3)
    /* DE4694 00013C28 8CAA004C */  lw         $t2, 0x4C($a1)
    /* DE4698 00013C2C 01436821 */  addu       $t5, $t2, $v1
    /* DE469C 00013C30 A5A20002 */  sh         $v0, 0x2($t5)
    /* DE46A0 00013C34 8CAC004C */  lw         $t4, 0x4C($a1)
    /* DE46A4 00013C38 01837021 */  addu       $t6, $t4, $v1
    /* DE46A8 00013C3C A5C20004 */  sh         $v0, 0x4($t6)
    /* DE46AC 00013C40 8CB9004C */  lw         $t9, 0x4C($a1)
    /* DE46B0 00013C44 0323C021 */  addu       $t8, $t9, $v1
    /* DE46B4 00013C48 24630008 */  addiu      $v1, $v1, 0x8
    /* DE46B8 00013C4C 1464FFF3 */  bne        $v1, $a0, .L00013C1C
    /* DE46BC 00013C50 A7020006 */   sh        $v0, 0x6($t8)
    /* DE46C0 00013C54 8FBF0014 */  lw         $ra, 0x14($sp)
  .L00013C58:
    /* DE46C4 00013C58 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE46C8 00013C5C 03E00008 */  jr         $ra
    /* DE46CC 00013C60 00000000 */   nop
endlabel Fader_buildTexture
    /* DE46D0 00013C64 00000000 */  nop
    /* DE46D4 00013C68 00000000 */  nop
    /* DE46D8 00013C6C 00000000 */  nop
