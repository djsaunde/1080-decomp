nonmatching SnowBCollider_ctor, 0x90

glabel SnowBCollider_ctor
    /* DD95B0 00008B44 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DD95B4 00008B48 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DD95B8 00008B4C AFA50024 */  sw         $a1, 0x24($sp)
    /* DD95BC 00008B50 AFA60028 */  sw         $a2, 0x28($sp)
    /* DD95C0 00008B54 14800005 */  bnez       $a0, .L00008B6C
    /* DD95C4 00008B58 AFA7002C */   sw        $a3, 0x2C($sp)
    /* DD95C8 00008B5C 0C000000 */  jal        func_00000000
    /* DD95CC 00008B60 24040178 */   addiu     $a0, $zero, 0x178
    /* DD95D0 00008B64 10400016 */  beqz       $v0, .L00008BC0
    /* DD95D4 00008B68 00402025 */   or        $a0, $v0, $zero
  .L00008B6C:
    /* DD95D8 00008B6C C7A40034 */  lwc1       $f4, 0x34($sp)
  alabel D_00008B70
    /* DD95DC 00008B70 8FA50024 */  lw         $a1, 0x24($sp)
    /* DD95E0 00008B74 8FA6002C */  lw         $a2, 0x2C($sp)
    /* DD95E4 00008B78 8FA70030 */  lw         $a3, 0x30($sp)
    /* DD95E8 00008B7C AFA40020 */  sw         $a0, 0x20($sp)
    /* DD95EC 00008B80 0C000000 */  jal        func_00000000
    /* DD95F0 00008B84 E7A40010 */   swc1      $f4, 0x10($sp)
    /* DD95F4 00008B88 8FA40020 */  lw         $a0, 0x20($sp)
    /* DD95F8 00008B8C 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD95FC 00008B90 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD9600 00008B94 AC8E0028 */  sw         $t6, 0x28($a0)
    /* DD9604 00008B98 8FAF0024 */  lw         $t7, 0x24($sp)
    /* DD9608 00008B9C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD960C 00008BA0 44813000 */  mtc1       $at, $f6
    /* DD9610 00008BA4 3C014120 */  lui        $at, (0x41200000 >> 16)
    /* DD9614 00008BA8 AC8F0170 */  sw         $t7, 0x170($a0)
    /* DD9618 00008BAC 97B8002A */  lhu        $t8, 0x2A($sp)
    /* DD961C 00008BB0 44814000 */  mtc1       $at, $f8
    /* DD9620 00008BB4 E4860150 */  swc1       $f6, 0x150($a0)
    /* DD9624 00008BB8 A4980174 */  sh         $t8, 0x174($a0)
    /* DD9628 00008BBC E4880154 */  swc1       $f8, 0x154($a0)
  .L00008BC0:
    /* DD962C 00008BC0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD9630 00008BC4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DD9634 00008BC8 00801025 */  or         $v0, $a0, $zero
    /* DD9638 00008BCC 03E00008 */  jr         $ra
    /* DD963C 00008BD0 00000000 */   nop
endlabel SnowBCollider_ctor
