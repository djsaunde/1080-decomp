nonmatching Process_fadeOut_0000F954, 0x94

glabel Process_fadeOut_0000F954
    /* DE03C0 0000F954 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE03C4 0000F958 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE03C8 0000F95C AFA40018 */  sw         $a0, 0x18($sp)
    /* DE03CC 0000F960 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE03D0 0000F964 AFA60020 */  sw         $a2, 0x20($sp)
    /* DE03D4 0000F968 8FAE0018 */  lw         $t6, 0x18($sp)
    /* DE03D8 0000F96C 8DCF0048 */  lw         $t7, 0x48($t6)
    /* DE03DC 0000F970 11E00005 */  beqz       $t7, .L0000F988
    /* DE03E0 0000F974 00000000 */   nop
    /* DE03E4 0000F978 8FB80018 */  lw         $t8, 0x18($sp)
    /* DE03E8 0000F97C 8F040048 */  lw         $a0, 0x48($t8)
    /* DE03EC 0000F980 0C000000 */  jal        func_00000000
    /* DE03F0 0000F984 00000000 */   nop
  .L0000F988:
    /* DE03F4 0000F988 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DE03F8 0000F98C 24840000 */  addiu      $a0, $a0, 0x0
    /* DE03FC 0000F990 0C000000 */  jal        func_00000000
    /* DE0400 0000F994 00000000 */   nop
    /* DE0404 0000F998 3C040000 */  lui        $a0, %hi(snd_seqEndDbg_00000188 + 0x8)
    /* DE0408 0000F99C 8C840190 */  lw         $a0, %lo(snd_seqEndDbg_00000188 + 0x8)($a0)
    /* DE040C 0000F9A0 24050003 */  addiu      $a1, $zero, 0x3
    /* DE0410 0000F9A4 00003025 */  or         $a2, $zero, $zero
    /* DE0414 0000F9A8 0C000000 */  jal        func_00000000
    /* DE0418 0000F9AC 00000000 */   nop
    /* DE041C 0000F9B0 24190001 */  addiu      $t9, $zero, 0x1
    /* DE0420 0000F9B4 8FA80018 */  lw         $t0, 0x18($sp)
    /* DE0424 0000F9B8 AD19002C */  sw         $t9, 0x2C($t0)
    /* DE0428 0000F9BC 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DE042C 0000F9C0 8FA50020 */  lw         $a1, 0x20($sp)
    /* DE0430 0000F9C4 00003025 */  or         $a2, $zero, $zero
    /* DE0434 0000F9C8 0C000000 */  jal        func_00000000
    /* DE0438 0000F9CC 00000000 */   nop
    /* DE043C 0000F9D0 10000001 */  b          .L0000F9D8
    /* DE0440 0000F9D4 00000000 */   nop
  .L0000F9D8:
    /* DE0444 0000F9D8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE0448 0000F9DC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE044C 0000F9E0 03E00008 */  jr         $ra
    /* DE0450 0000F9E4 00000000 */   nop
endlabel Process_fadeOut_0000F954
