nonmatching game_isUsoLoaded, 0x60

glabel game_isUsoLoaded
    /* DD938C 00008920 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD9390 00008924 AFB00014 */  sw         $s0, 0x14($sp)
    /* DD9394 00008928 3C100000 */  lui        $s0, %hi(stream_readQuad_000000F0 + 0x40)
    /* DD9398 0000892C 8E100130 */  lw         $s0, %lo(stream_readQuad_000000F0 + 0x40)($s0)
    /* DD939C 00008930 AFB10018 */  sw         $s1, 0x18($sp)
    /* DD93A0 00008934 00808825 */  or         $s1, $a0, $zero
    /* DD93A4 00008938 1200000B */  beqz       $s0, .L00008968
    /* DD93A8 0000893C AFBF001C */   sw        $ra, 0x1C($sp)
    /* DD93AC 00008940 8E040000 */  lw         $a0, 0x0($s0)
  .L00008944:
    /* DD93B0 00008944 0C000000 */  jal        func_00000000
    /* DD93B4 00008948 02202825 */   or        $a1, $s1, $zero
    /* DD93B8 0000894C 54400004 */  bnel       $v0, $zero, .L00008960
    /* DD93BC 00008950 8E100004 */   lw        $s0, 0x4($s0)
    /* DD93C0 00008954 10000005 */  b          .L0000896C
    /* DD93C4 00008958 24020001 */   addiu     $v0, $zero, 0x1
    /* DD93C8 0000895C 8E100004 */  lw         $s0, 0x4($s0)
  .L00008960:
    /* DD93CC 00008960 5600FFF8 */  bnel       $s0, $zero, .L00008944
    /* DD93D0 00008964 8E040000 */   lw        $a0, 0x0($s0)
  .L00008968:
    /* DD93D4 00008968 00001025 */  or         $v0, $zero, $zero
  .L0000896C:
    /* DD93D8 0000896C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD93DC 00008970 8FB00014 */  lw         $s0, 0x14($sp)
    /* DD93E0 00008974 8FB10018 */  lw         $s1, 0x18($sp)
    /* DD93E4 00008978 03E00008 */  jr         $ra
    /* DD93E8 0000897C 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel game_isUsoLoaded
