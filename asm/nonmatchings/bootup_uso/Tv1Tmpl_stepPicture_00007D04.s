nonmatching Tv1Tmpl_stepPicture_00007D04, 0x14C

glabel Tv1Tmpl_stepPicture_00007D04
    /* DD8770 00007D04 3C020000 */  lui        $v0, %hi(snd_setPlayer0VolumeLevel_0000023C + 0x18)
    /* DD8774 00007D08 8C420254 */  lw         $v0, %lo(snd_setPlayer0VolumeLevel_0000023C + 0x18)($v0)
    /* DD8778 00007D0C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DD877C 00007D10 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD8780 00007D14 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD8784 00007D18 8C430028 */  lw         $v1, 0x28($v0)
    /* DD8788 00007D1C 8C87005C */  lw         $a3, 0x5C($a0)
    /* DD878C 00007D20 00808025 */  or         $s0, $a0, $zero
    /* DD8790 00007D24 846E0060 */  lh         $t6, 0x60($v1)
    /* DD8794 00007D28 AFA70024 */  sw         $a3, 0x24($sp)
    /* DD8798 00007D2C 8C790064 */  lw         $t9, 0x64($v1)
    /* DD879C 00007D30 01C22021 */  addu       $a0, $t6, $v0
    /* DD87A0 00007D34 0320F809 */  jalr       $t9
    /* DD87A4 00007D38 00000000 */   nop
    /* DD87A8 00007D3C 8E03005C */  lw         $v1, 0x5C($s0)
    /* DD87AC 00007D40 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD87B0 00007D44 24010001 */  addiu      $at, $zero, 0x1
    /* DD87B4 00007D48 10610007 */  beq        $v1, $at, .L00007D68
    /* DD87B8 00007D4C AE020064 */   sw        $v0, 0x64($s0)
    /* DD87BC 00007D50 1060000C */  beqz       $v1, .L00007D84
    /* DD87C0 00007D54 24010002 */   addiu     $at, $zero, 0x2
    /* DD87C4 00007D58 50610015 */  beql       $v1, $at, .L00007DB0
    /* DD87C8 00007D5C 8E030060 */   lw        $v1, 0x60($s0)
    /* DD87CC 00007D60 10000023 */  b          .L00007DF0
    /* DD87D0 00007D64 8E030040 */   lw        $v1, 0x40($s0)
  .L00007D68:
    /* DD87D4 00007D68 8E030060 */  lw         $v1, 0x60($s0)
    /* DD87D8 00007D6C 246F0001 */  addiu      $t7, $v1, 0x1
    /* DD87DC 00007D70 2861001F */  slti       $at, $v1, 0x1F
    /* DD87E0 00007D74 1420001D */  bnez       $at, .L00007DEC
    /* DD87E4 00007D78 AE0F0060 */   sw        $t7, 0x60($s0)
    /* DD87E8 00007D7C 1000001B */  b          .L00007DEC
    /* DD87EC 00007D80 24070002 */   addiu     $a3, $zero, 0x2
  .L00007D84:
    /* DD87F0 00007D84 8E030060 */  lw         $v1, 0x60($s0)
    /* DD87F4 00007D88 24780001 */  addiu      $t8, $v1, 0x1
    /* DD87F8 00007D8C 2861003D */  slti       $at, $v1, 0x3D
    /* DD87FC 00007D90 14200002 */  bnez       $at, .L00007D9C
  alabel D_00007D94
    /* DD8800 00007D94 AE180060 */   sw        $t8, 0x60($s0)
    /* DD8804 00007D98 24070002 */  addiu      $a3, $zero, 0x2
  .L00007D9C:
    /* DD8808 00007D9C 8E080058 */  lw         $t0, 0x58($s0)
    /* DD880C 00007DA0 2509FFF6 */  addiu      $t1, $t0, -0xA
  alabel sStr_rom_heli_uso_7DA4
    /* DD8810 00007DA4 10000011 */  b          .L00007DEC
    /* DD8814 00007DA8 AE090058 */   sw        $t1, 0x58($s0)
    /* DD8818 00007DAC 8E030060 */  lw         $v1, 0x60($s0)
  .L00007DB0:
    /* DD881C 00007DB0 246A0001 */  addiu      $t2, $v1, 0x1
  alabel sStr_rom_balloon_uso_7DB4
    /* DD8820 00007DB4 2861005B */  slti       $at, $v1, 0x5B
    /* DD8824 00007DB8 1420000A */  bnez       $at, .L00007DE4
    /* DD8828 00007DBC AE0A0060 */   sw        $t2, 0x60($s0)
    /* DD882C 00007DC0 0C000000 */  jal        func_00000000
  alabel sStr_rom_balloon_uso_7DC4
    /* DD8830 00007DC4 00000000 */   nop
    /* DD8834 00007DC8 3C014040 */  lui        $at, (0x40400000 >> 16)
    /* DD8838 00007DCC 44812000 */  mtc1       $at, $f4
    /* DD883C 00007DD0 AE000060 */  sw         $zero, 0x60($s0)
  alabel sStr_rom_checkpoint_uso_7DD4
    /* DD8840 00007DD4 46040182 */  mul.s      $f6, $f0, $f4
    /* DD8844 00007DD8 4600320D */  trunc.w.s  $f8, $f6
    /* DD8848 00007DDC 44074000 */  mfc1       $a3, $f8
    /* DD884C 00007DE0 00000000 */  nop
  .L00007DE4:
    /* DD8850 00007DE4 240C0018 */  addiu      $t4, $zero, 0x18
  alabel sStr_rom_checkpoint_uso_7DE8
    /* DD8854 00007DE8 AE0C0058 */  sw         $t4, 0x58($s0)
  .L00007DEC:
    /* DD8858 00007DEC 8E030040 */  lw         $v1, 0x40($s0)
  .L00007DF0:
    /* DD885C 00007DF0 02002025 */  or         $a0, $s0, $zero
    /* DD8860 00007DF4 00003025 */  or         $a2, $zero, $zero
    /* DD8864 00007DF8 8C620084 */  lw         $v0, 0x84($v1)
  alabel sStr_rom_checkpoint_uso_7DFC
    /* DD8868 00007DFC 8C4D0000 */  lw         $t5, 0x0($v0)
    /* DD886C 00007E00 8DA5001C */  lw         $a1, 0x1C($t5)
    /* DD8870 00007E04 0C000000 */  jal        func_00000000
    /* DD8874 00007E08 AFA70024 */   sw        $a3, 0x24($sp)
    /* DD8878 00007E0C 8E030040 */  lw         $v1, 0x40($s0)
  alabel sStr_rom_fire_uso_7E10
    /* DD887C 00007E10 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD8880 00007E14 02002025 */  or         $a0, $s0, $zero
    /* DD8884 00007E18 8C620084 */  lw         $v0, 0x84($v1)
    /* DD8888 00007E1C 24060001 */  addiu      $a2, $zero, 0x1
  alabel sTag_pyr_7E20
    /* DD888C 00007E20 8C4E0004 */  lw         $t6, 0x4($v0)
  alabel D_00007E24
    /* DD8890 00007E24 24420004 */  addiu      $v0, $v0, 0x4
    /* DD8894 00007E28 8DC5001C */  lw         $a1, 0x1C($t6)
    /* DD8898 00007E2C 0C000000 */  jal        func_00000000
    /* DD889C 00007E30 AFA70024 */   sw        $a3, 0x24($sp)
  alabel D_00007E34
    /* DD88A0 00007E34 8FA70024 */  lw         $a3, 0x24($sp)
  alabel D_00007E38
    /* DD88A4 00007E38 AE07005C */  sw         $a3, 0x5C($s0)
  alabel D_00007E3C
    /* DD88A8 00007E3C 8FBF001C */  lw         $ra, 0x1C($sp)
  alabel D_00007E40
    /* DD88AC 00007E40 8FB00018 */  lw         $s0, 0x18($sp)
  alabel D_00007E44
    /* DD88B0 00007E44 27BD0028 */  addiu      $sp, $sp, 0x28
  alabel sStr_light01_7E48
    /* DD88B4 00007E48 03E00008 */  jr         $ra
    /* DD88B8 00007E4C 00000000 */   nop
endlabel Tv1Tmpl_stepPicture_00007D04
