nonmatching SnowBoard_InitInstance, 0xCC

glabel SnowBoard_InitInstance
    /* DDF498 0000EA2C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDF49C 0000EA30 3C0E0001 */  lui        $t6, %hi(sStr_SnowBoard_8D70)
    /* DDF4A0 0000EA34 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF4A4 0000EA38 25CE8D70 */  addiu      $t6, $t6, %lo(sStr_SnowBoard_8D70)
    /* DDF4A8 0000EA3C AC8E000C */  sw         $t6, 0xC($a0)
    /* DDF4AC 0000EA40 3C050001 */  lui        $a1, %hi(sStr_rom_version_bin_8D7C)
    /* DDF4B0 0000EA44 24A58D7C */  addiu      $a1, $a1, %lo(sStr_rom_version_bin_8D7C)
    /* DDF4B4 0000EA48 0C000000 */  jal        func_00000000
    /* DDF4B8 0000EA4C AFA40020 */   sw        $a0, 0x20($sp)
    /* DDF4BC 0000EA50 0C000000 */  jal        func_00000000
    /* DDF4C0 0000EA54 00002025 */   or        $a0, $zero, $zero
    /* DDF4C4 0000EA58 3C050001 */  lui        $a1, %hi(sStr_rom_anims_bin_8D8C)
    /* DDF4C8 0000EA5C AFA20018 */  sw         $v0, 0x18($sp)
    /* DDF4CC 0000EA60 24A58D8C */  addiu      $a1, $a1, %lo(sStr_rom_anims_bin_8D8C)
    /* DDF4D0 0000EA64 00002025 */  or         $a0, $zero, $zero
    /* DDF4D4 0000EA68 24060020 */  addiu      $a2, $zero, 0x20
    /* DDF4D8 0000EA6C 0C000000 */  jal        func_00000000
    /* DDF4DC 0000EA70 2407000C */   addiu     $a3, $zero, 0xC
    /* DDF4E0 0000EA74 3C010000 */  lui        $at, (0x0 >> 16)
    /* DDF4E4 0000EA78 AC220000 */  sw         $v0, 0x0($at)
    /* DDF4E8 0000EA7C 0C000000 */  jal        func_00000000
    /* DDF4EC 0000EA80 8FA40018 */   lw        $a0, 0x18($sp)
    /* DDF4F0 0000EA84 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF4F4 0000EA88 3C050001 */  lui        $a1, %hi(sStr_Node_8D9C)
    /* DDF4F8 0000EA8C 24A58D9C */  addiu      $a1, $a1, %lo(sStr_Node_8D9C)
    /* DDF4FC 0000EA90 0C000000 */  jal        func_00000000
    /* DDF500 0000EA94 24840000 */   addiu     $a0, $a0, 0x0
    /* DDF504 0000EA98 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF508 0000EA9C 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF50C 0000EAA0 0C000000 */  jal        func_00000000
    /* DDF510 0000EAA4 8FA50020 */   lw        $a1, 0x20($sp)
    /* DDF514 0000EAA8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF518 0000EAAC 0C000000 */  jal        func_00000000
    /* DDF51C 0000EAB0 24840000 */   addiu     $a0, $a0, 0x0
    /* DDF520 0000EAB4 10400008 */  beqz       $v0, .L0000EAD8
    /* DDF524 0000EAB8 3C040000 */   lui       $a0, (0x0 >> 16)
    /* DDF528 0000EABC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF52C 0000EAC0 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF530 0000EAC4 24050001 */  addiu      $a1, $zero, 0x1
    /* DDF534 0000EAC8 0C000000 */  jal        func_00000000
    /* DDF538 0000EACC 00003025 */   or        $a2, $zero, $zero
    /* DDF53C 0000EAD0 10000006 */  b          .L0000EAEC
    /* DDF540 0000EAD4 8FBF0014 */   lw        $ra, 0x14($sp)
  .L0000EAD8:
    /* DDF544 0000EAD8 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF548 0000EADC 00002825 */  or         $a1, $zero, $zero
    /* DDF54C 0000EAE0 0C000000 */  jal        func_00000000
    /* DDF550 0000EAE4 00003025 */   or        $a2, $zero, $zero
    /* DDF554 0000EAE8 8FBF0014 */  lw         $ra, 0x14($sp)
  .L0000EAEC:
    /* DDF558 0000EAEC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDF55C 0000EAF0 03E00008 */  jr         $ra
    /* DDF560 0000EAF4 00000000 */   nop
endlabel SnowBoard_InitInstance
