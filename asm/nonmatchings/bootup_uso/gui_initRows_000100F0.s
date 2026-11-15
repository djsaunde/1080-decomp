nonmatching gui_initRows_000100F0, 0x7C

glabel gui_initRows_000100F0
    /* DE0B5C 000100F0 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE0B60 000100F4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE0B64 000100F8 AFA40028 */  sw         $a0, 0x28($sp)
    /* DE0B68 000100FC AFA5002C */  sw         $a1, 0x2C($sp)
    /* DE0B6C 00010100 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE0B70 00010104 8FAE002C */  lw         $t6, 0x2C($sp)
    /* DE0B74 00010108 8FAF0028 */  lw         $t7, 0x28($sp)
    /* DE0B78 0001010C ADEE002C */  sw         $t6, 0x2C($t7)
    /* DE0B7C 00010110 8FB80028 */  lw         $t8, 0x28($sp)
    /* DE0B80 00010114 AF00007C */  sw         $zero, 0x7C($t8)
    /* DE0B84 00010118 8FA40028 */  lw         $a0, 0x28($sp)
    /* DE0B88 0001011C 0C000000 */  jal        func_00000000
    /* DE0B8C 00010120 00000000 */   nop
    /* DE0B90 00010124 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE0B94 00010128 8F300028 */  lw         $s0, 0x28($t9)
    /* DE0B98 0001012C 86080060 */  lh         $t0, 0x60($s0)
    /* DE0B9C 00010130 01192021 */  addu       $a0, $t0, $t9
    /* DE0BA0 00010134 8E190064 */  lw         $t9, 0x64($s0)
    /* DE0BA4 00010138 0320F809 */  jalr       $t9
    /* DE0BA8 0001013C 00000000 */   nop
    /* DE0BAC 00010140 3C014188 */  lui        $at, (0x41880000 >> 16)
    /* DE0BB0 00010144 44812000 */  mtc1       $at, $f4
    /* DE0BB4 00010148 8FA90028 */  lw         $t1, 0x28($sp)
    /* DE0BB8 0001014C E5240074 */  swc1       $f4, 0x74($t1)
    /* DE0BBC 00010150 10000001 */  b          .L00010158
    /* DE0BC0 00010154 00000000 */   nop
  .L00010158:
    /* DE0BC4 00010158 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE0BC8 0001015C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE0BCC 00010160 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE0BD0 00010164 03E00008 */  jr         $ra
    /* DE0BD4 00010168 00000000 */   nop
endlabel gui_initRows_000100F0
