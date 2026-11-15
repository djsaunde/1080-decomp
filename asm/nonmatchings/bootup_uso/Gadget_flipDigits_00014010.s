nonmatching Gadget_flipDigits_00014010, 0xB4

glabel Gadget_flipDigits_00014010
    /* DE4A7C 00014010 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DE4A80 00014014 AFB30024 */  sw         $s3, 0x24($sp)
    /* DE4A84 00014018 AFB20020 */  sw         $s2, 0x20($sp)
    /* DE4A88 0001401C 00809825 */  or         $s3, $a0, $zero
    /* DE4A8C 00014020 AFBF0034 */  sw         $ra, 0x34($sp)
    /* DE4A90 00014024 AFB1001C */  sw         $s1, 0x1C($sp)
    /* DE4A94 00014028 249200E4 */  addiu      $s2, $a0, 0xE4
    /* DE4A98 0001402C AFB60030 */  sw         $s6, 0x30($sp)
    /* DE4A9C 00014030 AFB5002C */  sw         $s5, 0x2C($sp)
    /* DE4AA0 00014034 AFB40028 */  sw         $s4, 0x28($sp)
    /* DE4AA4 00014038 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE4AA8 0001403C 02402025 */  or         $a0, $s2, $zero
    /* DE4AAC 00014040 0C000000 */  jal        func_00000000
    /* DE4AB0 00014044 00008825 */   or        $s1, $zero, $zero
    /* DE4AB4 00014048 10400014 */  beqz       $v0, .L0001409C
    /* DE4AB8 0001404C 02718021 */   addu      $s0, $s3, $s1
    /* DE4ABC 00014050 24160069 */  addiu      $s6, $zero, 0x69
    /* DE4AC0 00014054 24150020 */  addiu      $s5, $zero, 0x20
    /* DE4AC4 00014058 24140062 */  addiu      $s4, $zero, 0x62
    /* DE4AC8 0001405C 24130061 */  addiu      $s3, $zero, 0x61
    /* DE4ACC 00014060 920200E4 */  lbu        $v0, 0xE4($s0)
  .L00014064:
    /* DE4AD0 00014064 26310001 */  addiu      $s1, $s1, 0x1
    /* DE4AD4 00014068 02402025 */  or         $a0, $s2, $zero
    /* DE4AD8 0001406C 12620006 */  beq        $s3, $v0, .L00014088
    /* DE4ADC 00014070 00000000 */   nop
    /* DE4AE0 00014074 12820004 */  beq        $s4, $v0, .L00014088
    /* DE4AE4 00014078 00000000 */   nop
    /* DE4AE8 0001407C 12A20002 */  beq        $s5, $v0, .L00014088
    /* DE4AEC 00014080 02C27023 */   subu      $t6, $s6, $v0
    /* DE4AF0 00014084 A20E00E4 */  sb         $t6, 0xE4($s0)
  .L00014088:
    /* DE4AF4 00014088 0C000000 */  jal        func_00000000
    /* DE4AF8 0001408C 26100001 */   addiu     $s0, $s0, 0x1
    /* DE4AFC 00014090 0222082B */  sltu       $at, $s1, $v0
    /* DE4B00 00014094 5420FFF3 */  bnel       $at, $zero, .L00014064
    /* DE4B04 00014098 920200E4 */   lbu       $v0, 0xE4($s0)
  .L0001409C:
    /* DE4B08 0001409C 8FBF0034 */  lw         $ra, 0x34($sp)
    /* DE4B0C 000140A0 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE4B10 000140A4 8FB1001C */  lw         $s1, 0x1C($sp)
    /* DE4B14 000140A8 8FB20020 */  lw         $s2, 0x20($sp)
    /* DE4B18 000140AC 8FB30024 */  lw         $s3, 0x24($sp)
    /* DE4B1C 000140B0 8FB40028 */  lw         $s4, 0x28($sp)
    /* DE4B20 000140B4 8FB5002C */  lw         $s5, 0x2C($sp)
    /* DE4B24 000140B8 8FB60030 */  lw         $s6, 0x30($sp)
    /* DE4B28 000140BC 03E00008 */  jr         $ra
    /* DE4B2C 000140C0 27BD0038 */   addiu     $sp, $sp, 0x38
endlabel Gadget_flipDigits_00014010
