nonmatching gadget_decodeStringB4_00014598, 0xB4

glabel gadget_decodeStringB4_00014598
    /* DE5004 00014598 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* DE5008 0001459C AFB30024 */  sw         $s3, 0x24($sp)
    /* DE500C 000145A0 AFB20020 */  sw         $s2, 0x20($sp)
    /* DE5010 000145A4 00809825 */  or         $s3, $a0, $zero
    /* DE5014 000145A8 AFBF0034 */  sw         $ra, 0x34($sp)
    /* DE5018 000145AC AFB1001C */  sw         $s1, 0x1C($sp)
    /* DE501C 000145B0 249200B4 */  addiu      $s2, $a0, 0xB4
    /* DE5020 000145B4 AFB60030 */  sw         $s6, 0x30($sp)
    /* DE5024 000145B8 AFB5002C */  sw         $s5, 0x2C($sp)
    /* DE5028 000145BC AFB40028 */  sw         $s4, 0x28($sp)
    /* DE502C 000145C0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE5030 000145C4 02402025 */  or         $a0, $s2, $zero
    /* DE5034 000145C8 0C000000 */  jal        func_00000000
    /* DE5038 000145CC 00008825 */   or        $s1, $zero, $zero
    /* DE503C 000145D0 10400014 */  beqz       $v0, .L00014624
    /* DE5040 000145D4 02718021 */   addu      $s0, $s3, $s1
    /* DE5044 000145D8 24160069 */  addiu      $s6, $zero, 0x69
    /* DE5048 000145DC 24150020 */  addiu      $s5, $zero, 0x20
    /* DE504C 000145E0 24140062 */  addiu      $s4, $zero, 0x62
    /* DE5050 000145E4 24130061 */  addiu      $s3, $zero, 0x61
    /* DE5054 000145E8 920200B4 */  lbu        $v0, 0xB4($s0)
  .L000145EC:
    /* DE5058 000145EC 26310001 */  addiu      $s1, $s1, 0x1
    /* DE505C 000145F0 02402025 */  or         $a0, $s2, $zero
    /* DE5060 000145F4 12620006 */  beq        $s3, $v0, .L00014610
    /* DE5064 000145F8 00000000 */   nop
    /* DE5068 000145FC 12820004 */  beq        $s4, $v0, .L00014610
    /* DE506C 00014600 00000000 */   nop
    /* DE5070 00014604 12A20002 */  beq        $s5, $v0, .L00014610
    /* DE5074 00014608 02C27023 */   subu      $t6, $s6, $v0
    /* DE5078 0001460C A20E00B4 */  sb         $t6, 0xB4($s0)
  .L00014610:
    /* DE507C 00014610 0C000000 */  jal        func_00000000
    /* DE5080 00014614 26100001 */   addiu     $s0, $s0, 0x1
    /* DE5084 00014618 0222082B */  sltu       $at, $s1, $v0
    /* DE5088 0001461C 5420FFF3 */  bnel       $at, $zero, .L000145EC
    /* DE508C 00014620 920200B4 */   lbu       $v0, 0xB4($s0)
  .L00014624:
    /* DE5090 00014624 8FBF0034 */  lw         $ra, 0x34($sp)
    /* DE5094 00014628 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE5098 0001462C 8FB1001C */  lw         $s1, 0x1C($sp)
    /* DE509C 00014630 8FB20020 */  lw         $s2, 0x20($sp)
    /* DE50A0 00014634 8FB30024 */  lw         $s3, 0x24($sp)
    /* DE50A4 00014638 8FB40028 */  lw         $s4, 0x28($sp)
    /* DE50A8 0001463C 8FB5002C */  lw         $s5, 0x2C($sp)
    /* DE50AC 00014640 8FB60030 */  lw         $s6, 0x30($sp)
    /* DE50B0 00014644 03E00008 */  jr         $ra
    /* DE50B4 00014648 27BD0038 */   addiu     $sp, $sp, 0x38
endlabel gadget_decodeStringB4_00014598
