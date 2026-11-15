nonmatching GameFlow_releaseFiles_00012110, 0x40

glabel GameFlow_releaseFiles_00012110
    /* DE2B7C 00012110 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE2B80 00012114 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE2B84 00012118 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE2B88 0001211C 3C040000 */  lui        $a0, %hi(func_00000148 + 0x8)
    /* DE2B8C 00012120 8C840150 */  lw         $a0, %lo(func_00000148 + 0x8)($a0)
    /* DE2B90 00012124 0C000000 */  jal        func_00000000
    /* DE2B94 00012128 00000000 */   nop
    /* DE2B98 0001212C 24040007 */  addiu      $a0, $zero, 0x7
    /* DE2B9C 00012130 0C000000 */  jal        func_00000000
    /* DE2BA0 00012134 00000000 */   nop
    /* DE2BA4 00012138 10000001 */  b          .L00012140
    /* DE2BA8 0001213C 00000000 */   nop
  .L00012140:
    /* DE2BAC 00012140 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE2BB0 00012144 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE2BB4 00012148 03E00008 */  jr         $ra
    /* DE2BB8 0001214C 00000000 */   nop
endlabel GameFlow_releaseFiles_00012110
