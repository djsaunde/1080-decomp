nonmatching game_createProcessWindow, 0x138

glabel game_createProcessWindow
    /* DE0288 0000F81C 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* DE028C 0000F820 AFBF0064 */  sw         $ra, 0x64($sp)
    /* DE0290 0000F824 AFA40088 */  sw         $a0, 0x88($sp)
    /* DE0294 0000F828 AFA5008C */  sw         $a1, 0x8C($sp)
    /* DE0298 0000F82C AFB10060 */  sw         $s1, 0x60($sp)
    /* DE029C 0000F830 AFB0005C */  sw         $s0, 0x5C($sp)
    /* DE02A0 0000F834 F7B80050 */  sdc1       $f24, 0x50($sp)
    /* DE02A4 0000F838 F7B60048 */  sdc1       $f22, 0x48($sp)
    /* DE02A8 0000F83C F7B40040 */  sdc1       $f20, 0x40($sp)
    /* DE02AC 0000F840 AFA00084 */  sw         $zero, 0x84($sp)
    /* DE02B0 0000F844 8FAE008C */  lw         $t6, 0x8C($sp)
    /* DE02B4 0000F848 11C00004 */  beqz       $t6, .L0000F85C
    /* DE02B8 0000F84C 00000000 */   nop
    /* DE02BC 0000F850 8FAF0084 */  lw         $t7, 0x84($sp)
    /* DE02C0 0000F854 35F80006 */  ori        $t8, $t7, 0x6
    /* DE02C4 0000F858 AFB80084 */  sw         $t8, 0x84($sp)
  .L0000F85C:
    /* DE02C8 0000F85C 3C100000 */  lui        $s0, %hi(func_00000008 + 0x28)
    /* DE02CC 0000F860 26100030 */  addiu      $s0, $s0, %lo(func_00000008 + 0x28)
    /* DE02D0 0000F864 4480A000 */  mtc1       $zero, $f20
    /* DE02D4 0000F868 00000000 */  nop
    /* DE02D8 0000F86C E614000C */  swc1       $f20, 0xC($s0)
    /* DE02DC 0000F870 4600A586 */  mov.s      $f22, $f20
    /* DE02E0 0000F874 E6160008 */  swc1       $f22, 0x8($s0)
    /* DE02E4 0000F878 4600B606 */  mov.s      $f24, $f22
    /* DE02E8 0000F87C E6180004 */  swc1       $f24, 0x4($s0)
    /* DE02EC 0000F880 E6180000 */  swc1       $f24, 0x0($s0)
    /* DE02F0 0000F884 00002025 */  or         $a0, $zero, $zero
    /* DE02F4 0000F888 24050064 */  addiu      $a1, $zero, 0x64
    /* DE02F8 0000F88C 3C060001 */  lui        $a2, %hi(D_0000C57C)
    /* DE02FC 0000F890 24C6C57C */  addiu      $a2, $a2, %lo(D_0000C57C)
    /* DE0300 0000F894 2407006D */  addiu      $a3, $zero, 0x6D
    /* DE0304 0000F898 AFA00010 */  sw         $zero, 0x10($sp)
    /* DE0308 0000F89C 2419006E */  addiu      $t9, $zero, 0x6E
    /* DE030C 0000F8A0 AFB90014 */  sw         $t9, 0x14($sp)
    /* DE0310 0000F8A4 AFA00018 */  sw         $zero, 0x18($sp)
    /* DE0314 0000F8A8 AFA0001C */  sw         $zero, 0x1C($sp)
    /* DE0318 0000F8AC 2408006F */  addiu      $t0, $zero, 0x6F
    /* DE031C 0000F8B0 AFA80020 */  sw         $t0, 0x20($sp)
    /* DE0320 0000F8B4 24090140 */  addiu      $t1, $zero, 0x140
    /* DE0324 0000F8B8 AFA90024 */  sw         $t1, 0x24($sp)
    /* DE0328 0000F8BC 240A00F0 */  addiu      $t2, $zero, 0xF0
    /* DE032C 0000F8C0 AFAA0028 */  sw         $t2, 0x28($sp)
    /* DE0330 0000F8C4 240B0070 */  addiu      $t3, $zero, 0x70
    /* DE0334 0000F8C8 AFAB002C */  sw         $t3, 0x2C($sp)
    /* DE0338 0000F8CC 8FAC0084 */  lw         $t4, 0x84($sp)
    /* DE033C 0000F8D0 358D0400 */  ori        $t5, $t4, 0x400
    /* DE0340 0000F8D4 AFAD0030 */  sw         $t5, 0x30($sp)
    /* DE0344 0000F8D8 240E0071 */  addiu      $t6, $zero, 0x71
    /* DE0348 0000F8DC AFAE0034 */  sw         $t6, 0x34($sp)
    /* DE034C 0000F8E0 240F0002 */  addiu      $t7, $zero, 0x2
    /* DE0350 0000F8E4 AFAF0038 */  sw         $t7, 0x38($sp)
    /* DE0354 0000F8E8 AFA0003C */  sw         $zero, 0x3C($sp)
    /* DE0358 0000F8EC 0C000000 */  jal        func_00000000
    /* DE035C 0000F8F0 00000000 */   nop
    /* DE0360 0000F8F4 AFA20080 */  sw         $v0, 0x80($sp)
    /* DE0364 0000F8F8 8FB00080 */  lw         $s0, 0x80($sp)
    /* DE0368 0000F8FC 3C110000 */  lui        $s1, %hi(func_00000008 + 0x24)
    /* DE036C 0000F900 2631002C */  addiu      $s1, $s1, %lo(func_00000008 + 0x24)
    /* DE0370 0000F904 02202025 */  or         $a0, $s1, $zero
    /* DE0374 0000F908 02002825 */  or         $a1, $s0, $zero
    /* DE0378 0000F90C 0C000000 */  jal        func_00000000
    /* DE037C 0000F910 00000000 */   nop
    /* DE0380 0000F914 8FB80080 */  lw         $t8, 0x80($sp)
    /* DE0384 0000F918 3C190000 */  lui        $t9, %hi(func_000000F0 + 0x44)
    /* DE0388 0000F91C 8F390134 */  lw         $t9, %lo(func_000000F0 + 0x44)($t9)
    /* DE038C 0000F920 AF380114 */  sw         $t8, 0x114($t9)
    /* DE0390 0000F924 8FA20080 */  lw         $v0, 0x80($sp)
    /* DE0394 0000F928 10000001 */  b          .L0000F930
    /* DE0398 0000F92C 00000000 */   nop
  .L0000F930:
    /* DE039C 0000F930 D7B40040 */  ldc1       $f20, 0x40($sp)
    /* DE03A0 0000F934 D7B60048 */  ldc1       $f22, 0x48($sp)
    /* DE03A4 0000F938 D7B80050 */  ldc1       $f24, 0x50($sp)
    /* DE03A8 0000F93C 8FB0005C */  lw         $s0, 0x5C($sp)
    /* DE03AC 0000F940 8FB10060 */  lw         $s1, 0x60($sp)
    /* DE03B0 0000F944 8FBF0064 */  lw         $ra, 0x64($sp)
    /* DE03B4 0000F948 27BD0088 */  addiu      $sp, $sp, 0x88
    /* DE03B8 0000F94C 03E00008 */  jr         $ra
    /* DE03BC 0000F950 00000000 */   nop
endlabel game_createProcessWindow
