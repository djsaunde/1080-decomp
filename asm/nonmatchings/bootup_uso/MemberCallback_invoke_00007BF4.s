nonmatching MemberCallback_invoke_00007BF4, 0x80

glabel MemberCallback_invoke_00007BF4
    /* DD8660 00007BF4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8664 00007BF8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8668 00007BFC 848F000A */  lh         $t7, 0xA($a0)
    /* DD866C 00007C00 848E0008 */  lh         $t6, 0x8($a0)
    /* DD8670 00007C04 8C850004 */  lw         $a1, 0x4($a0)
    /* DD8674 00007C08 24830008 */  addiu      $v1, $a0, 0x8
    /* DD8678 00007C0C 05E10003 */  bgez       $t7, .L00007C1C
    /* DD867C 00007C10 01C52821 */   addu      $a1, $t6, $a1
    /* DD8680 00007C14 10000011 */  b          .L00007C5C
    /* DD8684 00007C18 8C86000C */   lw        $a2, 0xC($a0)
  .L00007C1C:
    /* DD8688 00007C1C 8C620004 */  lw         $v0, 0x4($v1)
    /* DD868C 00007C20 14400006 */  bnez       $v0, .L00007C3C
    /* DD8690 00007C24 00402025 */   or        $a0, $v0, $zero
    /* DD8694 00007C28 84780000 */  lh         $t8, 0x0($v1)
    /* DD8698 00007C2C 17000003 */  bnez       $t8, .L00007C3C
    /* DD869C 00007C30 00000000 */   nop
    /* DD86A0 00007C34 10000001 */  b          .L00007C3C
    /* DD86A4 00007C38 24040028 */   addiu     $a0, $zero, 0x28
  .L00007C3C:
    /* DD86A8 00007C3C 84690002 */  lh         $t1, 0x2($v1)
    /* DD86AC 00007C40 00A4C821 */  addu       $t9, $a1, $a0
    /* DD86B0 00007C44 8F280000 */  lw         $t0, 0x0($t9)
    /* DD86B4 00007C48 000950C0 */  sll        $t2, $t1, 3
    /* DD86B8 00007C4C 010A1021 */  addu       $v0, $t0, $t2
    /* DD86BC 00007C50 844B0000 */  lh         $t3, 0x0($v0)
    /* DD86C0 00007C54 8C460004 */  lw         $a2, 0x4($v0)
    /* DD86C4 00007C58 01652821 */  addu       $a1, $t3, $a1
  .L00007C5C:
    /* DD86C8 00007C5C 00C0F809 */  jalr       $a2
    /* DD86CC 00007C60 00A02025 */   or        $a0, $a1, $zero
    /* DD86D0 00007C64 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD86D4 00007C68 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD86D8 00007C6C 03E00008 */  jr         $ra
    /* DD86DC 00007C70 00000000 */   nop
endlabel MemberCallback_invoke_00007BF4
