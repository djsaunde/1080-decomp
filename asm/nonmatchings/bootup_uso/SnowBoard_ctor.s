nonmatching SnowBoard_ctor, 0xBC

glabel SnowBoard_ctor
    /* DDF26C 0000E800 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DDF270 0000E804 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF274 0000E808 0C000000 */  jal        func_00000000
    /* DDF278 0000E80C 00000000 */   nop
    /* DDF27C 0000E810 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF280 0000E814 3C050001 */  lui        $a1, %hi(sStr_rom_audio_seq_uso_8D34)
    /* DDF284 0000E818 AFA00028 */  sw         $zero, 0x28($sp)
    /* DDF288 0000E81C AFA00024 */  sw         $zero, 0x24($sp)
    /* DDF28C 0000E820 AFA00020 */  sw         $zero, 0x20($sp)
    /* DDF290 0000E824 24A58D34 */  addiu      $a1, $a1, %lo(sStr_rom_audio_seq_uso_8D34)
    /* DDF294 0000E828 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF298 0000E82C 0C000000 */  jal        func_00000000
    /* DDF29C 0000E830 27A60024 */   addiu     $a2, $sp, 0x24
    /* DDF2A0 0000E834 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF2A4 0000E838 3C050001 */  lui        $a1, %hi(sStr_rom_audio_wave_uso_8D48)
    /* DDF2A8 0000E83C 24A58D48 */  addiu      $a1, $a1, %lo(sStr_rom_audio_wave_uso_8D48)
    /* DDF2AC 0000E840 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF2B0 0000E844 0C000000 */  jal        func_00000000
    /* DDF2B4 0000E848 27A60020 */   addiu     $a2, $sp, 0x20
    /* DDF2B8 0000E84C 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF2BC 0000E850 3C050001 */  lui        $a1, %hi(sStr_rom_audio_bank_uso_8D5C)
    /* DDF2C0 0000E854 24A58D5C */  addiu      $a1, $a1, %lo(sStr_rom_audio_bank_uso_8D5C)
    /* DDF2C4 0000E858 24840000 */  addiu      $a0, $a0, 0x0
    /* DDF2C8 0000E85C 0C000000 */  jal        func_00000000
    /* DDF2CC 0000E860 27A60028 */   addiu     $a2, $sp, 0x28
    /* DDF2D0 0000E864 8FA40024 */  lw         $a0, 0x24($sp)
    /* DDF2D4 0000E868 8FA50028 */  lw         $a1, 0x28($sp)
    /* DDF2D8 0000E86C 0C000000 */  jal        func_00000000
    /* DDF2DC 0000E870 8FA60020 */   lw        $a2, 0x20($sp)
    /* DDF2E0 0000E874 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DDF2E4 0000E878 0C000000 */  jal        func_00000000
    /* DDF2E8 0000E87C 24840000 */   addiu     $a0, $a0, 0x0
    /* DDF2EC 0000E880 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DDF2F0 0000E884 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DDF2F4 0000E888 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DDF2F8 0000E88C 3C010000 */  lui        $at, %hi(stream_readS32_00000008 + 0x20)
    /* DDF2FC 0000E890 AC2E0028 */  sw         $t6, %lo(stream_readS32_00000008 + 0x20)($at)
    /* DDF300 0000E894 24630000 */  addiu      $v1, $v1, 0x0
    /* DDF304 0000E898 8C620028 */  lw         $v0, 0x28($v1)
    /* DDF308 0000E89C 8C59005C */  lw         $t9, 0x5C($v0)
    /* DDF30C 0000E8A0 844F0058 */  lh         $t7, 0x58($v0)
    /* DDF310 0000E8A4 0320F809 */  jalr       $t9
    /* DDF314 0000E8A8 01E32021 */   addu      $a0, $t7, $v1
    /* DDF318 0000E8AC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF31C 0000E8B0 27BD0030 */  addiu      $sp, $sp, 0x30
    /* DDF320 0000E8B4 03E00008 */  jr         $ra
    /* DDF324 0000E8B8 00000000 */   nop
endlabel SnowBoard_ctor
