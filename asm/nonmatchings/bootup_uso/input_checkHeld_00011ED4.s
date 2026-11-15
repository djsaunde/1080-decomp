nonmatching input_checkHeld_00011ED4, 0xD4

glabel input_checkHeld_00011ED4
    /* DE2940 00011ED4 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* DE2944 00011ED8 8C8E018C */  lw         $t6, 0x18C($a0)
    /* DE2948 00011EDC 15C0002C */  bnez       $t6, .L00011F90
    /* DE294C 00011EE0 00000000 */   nop
    /* DE2950 00011EE4 8C8F0128 */  lw         $t7, 0x128($a0)
    /* DE2954 00011EE8 2401FFFF */  addiu      $at, $zero, -0x1
    /* DE2958 00011EEC 15E1001B */  bne        $t7, $at, .L00011F5C
    /* DE295C 00011EF0 00000000 */   nop
    /* DE2960 00011EF4 AFA00004 */  sw         $zero, 0x4($sp)
    /* DE2964 00011EF8 8C980120 */  lw         $t8, 0x120($a0)
    /* DE2968 00011EFC 1B000015 */  blez       $t8, .L00011F54
    /* DE296C 00011F00 00000000 */   nop
  .L00011F04:
    /* DE2970 00011F04 8FB90004 */  lw         $t9, 0x4($sp)
    /* DE2974 00011F08 00194080 */  sll        $t0, $t9, 2
    /* DE2978 00011F0C 00884821 */  addu       $t1, $a0, $t0
    /* DE297C 00011F10 8D2600E0 */  lw         $a2, 0xE0($t1)
    /* DE2980 00011F14 8CCA0010 */  lw         $t2, 0x10($a2)
    /* DE2984 00011F18 01455824 */  and        $t3, $t2, $a1
    /* DE2988 00011F1C 11600006 */  beqz       $t3, .L00011F38
    /* DE298C 00011F20 00000000 */   nop
    /* DE2990 00011F24 8FAC0004 */  lw         $t4, 0x4($sp)
    /* DE2994 00011F28 240D0001 */  addiu      $t5, $zero, 0x1
    /* DE2998 00011F2C 018D1004 */  sllv       $v0, $t5, $t4
    /* DE299C 00011F30 1000001A */  b          .L00011F9C
    /* DE29A0 00011F34 00000000 */   nop
  .L00011F38:
    /* DE29A4 00011F38 8FAE0004 */  lw         $t6, 0x4($sp)
    /* DE29A8 00011F3C 25CF0001 */  addiu      $t7, $t6, 0x1
    /* DE29AC 00011F40 AFAF0004 */  sw         $t7, 0x4($sp)
    /* DE29B0 00011F44 8C980120 */  lw         $t8, 0x120($a0)
    /* DE29B4 00011F48 01F8082A */  slt        $at, $t7, $t8
    /* DE29B8 00011F4C 1420FFED */  bnez       $at, .L00011F04
    /* DE29BC 00011F50 00000000 */   nop
  .L00011F54:
    /* DE29C0 00011F54 1000000E */  b          .L00011F90
    /* DE29C4 00011F58 00000000 */   nop
  .L00011F5C:
    /* DE29C8 00011F5C 8C990128 */  lw         $t9, 0x128($a0)
    /* DE29CC 00011F60 00194080 */  sll        $t0, $t9, 2
    /* DE29D0 00011F64 00884821 */  addu       $t1, $a0, $t0
    /* DE29D4 00011F68 8D2600E0 */  lw         $a2, 0xE0($t1)
    /* DE29D8 00011F6C 8CCA0010 */  lw         $t2, 0x10($a2)
    /* DE29DC 00011F70 01455824 */  and        $t3, $t2, $a1
    /* DE29E0 00011F74 11600006 */  beqz       $t3, .L00011F90
    /* DE29E4 00011F78 00000000 */   nop
    /* DE29E8 00011F7C 8C8D0128 */  lw         $t5, 0x128($a0)
    /* DE29EC 00011F80 240C0001 */  addiu      $t4, $zero, 0x1
    /* DE29F0 00011F84 01AC1004 */  sllv       $v0, $t4, $t5
    /* DE29F4 00011F88 10000004 */  b          .L00011F9C
    /* DE29F8 00011F8C 00000000 */   nop
  .L00011F90:
    /* DE29FC 00011F90 00001025 */  or         $v0, $zero, $zero
    /* DE2A00 00011F94 10000001 */  b          .L00011F9C
    /* DE2A04 00011F98 00000000 */   nop
  .L00011F9C:
    /* DE2A08 00011F9C 27BD0008 */  addiu      $sp, $sp, 0x8
    /* DE2A0C 00011FA0 03E00008 */  jr         $ra
    /* DE2A10 00011FA4 00000000 */   nop
endlabel input_checkHeld_00011ED4
