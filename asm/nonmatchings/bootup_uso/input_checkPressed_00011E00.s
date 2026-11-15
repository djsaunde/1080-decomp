nonmatching input_checkPressed_00011E00, 0xD4

glabel input_checkPressed_00011E00
    /* DE286C 00011E00 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* DE2870 00011E04 8C8E018C */  lw         $t6, 0x18C($a0)
    /* DE2874 00011E08 15C0002C */  bnez       $t6, .L00011EBC
    /* DE2878 00011E0C 00000000 */   nop
    /* DE287C 00011E10 8C8F0128 */  lw         $t7, 0x128($a0)
    /* DE2880 00011E14 2401FFFF */  addiu      $at, $zero, -0x1
    /* DE2884 00011E18 15E1001B */  bne        $t7, $at, .L00011E88
    /* DE2888 00011E1C 00000000 */   nop
    /* DE288C 00011E20 AFA00004 */  sw         $zero, 0x4($sp)
    /* DE2890 00011E24 8C980120 */  lw         $t8, 0x120($a0)
    /* DE2894 00011E28 1B000015 */  blez       $t8, .L00011E80
    /* DE2898 00011E2C 00000000 */   nop
  .L00011E30:
    /* DE289C 00011E30 8FB90004 */  lw         $t9, 0x4($sp)
    /* DE28A0 00011E34 00194080 */  sll        $t0, $t9, 2
    /* DE28A4 00011E38 00884821 */  addu       $t1, $a0, $t0
    /* DE28A8 00011E3C 8D2600E0 */  lw         $a2, 0xE0($t1)
    /* DE28AC 00011E40 8CCA0018 */  lw         $t2, 0x18($a2)
    /* DE28B0 00011E44 01455824 */  and        $t3, $t2, $a1
    /* DE28B4 00011E48 11600006 */  beqz       $t3, .L00011E64
    /* DE28B8 00011E4C 00000000 */   nop
    /* DE28BC 00011E50 8FAC0004 */  lw         $t4, 0x4($sp)
    /* DE28C0 00011E54 240D0001 */  addiu      $t5, $zero, 0x1
    /* DE28C4 00011E58 018D1004 */  sllv       $v0, $t5, $t4
    /* DE28C8 00011E5C 1000001A */  b          .L00011EC8
    /* DE28CC 00011E60 00000000 */   nop
  .L00011E64:
    /* DE28D0 00011E64 8FAE0004 */  lw         $t6, 0x4($sp)
    /* DE28D4 00011E68 25CF0001 */  addiu      $t7, $t6, 0x1
    /* DE28D8 00011E6C AFAF0004 */  sw         $t7, 0x4($sp)
    /* DE28DC 00011E70 8C980120 */  lw         $t8, 0x120($a0)
    /* DE28E0 00011E74 01F8082A */  slt        $at, $t7, $t8
    /* DE28E4 00011E78 1420FFED */  bnez       $at, .L00011E30
    /* DE28E8 00011E7C 00000000 */   nop
  .L00011E80:
    /* DE28EC 00011E80 1000000E */  b          .L00011EBC
    /* DE28F0 00011E84 00000000 */   nop
  .L00011E88:
    /* DE28F4 00011E88 8C990128 */  lw         $t9, 0x128($a0)
    /* DE28F8 00011E8C 00194080 */  sll        $t0, $t9, 2
    /* DE28FC 00011E90 00884821 */  addu       $t1, $a0, $t0
    /* DE2900 00011E94 8D2600E0 */  lw         $a2, 0xE0($t1)
    /* DE2904 00011E98 8CCA0018 */  lw         $t2, 0x18($a2)
    /* DE2908 00011E9C 01455824 */  and        $t3, $t2, $a1
    /* DE290C 00011EA0 11600006 */  beqz       $t3, .L00011EBC
    /* DE2910 00011EA4 00000000 */   nop
    /* DE2914 00011EA8 8C8D0128 */  lw         $t5, 0x128($a0)
    /* DE2918 00011EAC 240C0001 */  addiu      $t4, $zero, 0x1
    /* DE291C 00011EB0 01AC1004 */  sllv       $v0, $t4, $t5
    /* DE2920 00011EB4 10000004 */  b          .L00011EC8
    /* DE2924 00011EB8 00000000 */   nop
  .L00011EBC:
    /* DE2928 00011EBC 00001025 */  or         $v0, $zero, $zero
    /* DE292C 00011EC0 10000001 */  b          .L00011EC8
    /* DE2930 00011EC4 00000000 */   nop
  .L00011EC8:
    /* DE2934 00011EC8 27BD0008 */  addiu      $sp, $sp, 0x8
    /* DE2938 00011ECC 03E00008 */  jr         $ra
    /* DE293C 00011ED0 00000000 */   nop
endlabel input_checkPressed_00011E00
