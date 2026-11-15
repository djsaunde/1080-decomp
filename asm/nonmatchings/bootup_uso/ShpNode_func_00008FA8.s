nonmatching ShpNode_func_00008FA8, 0x124

glabel ShpNode_func_00008FA8
    /* DD9A14 00008FA8 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* DD9A18 00008FAC AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD9A1C 00008FB0 8C8F00B4 */  lw         $t7, 0xB4($a0)
    /* DD9A20 00008FB4 27A2002C */  addiu      $v0, $sp, 0x2C
    /* DD9A24 00008FB8 AC4F0000 */  sw         $t7, 0x0($v0)
    /* DD9A28 00008FBC 8C8E00B8 */  lw         $t6, 0xB8($a0)
    /* DD9A2C 00008FC0 AC4E0004 */  sw         $t6, 0x4($v0)
    /* DD9A30 00008FC4 8C8F00BC */  lw         $t7, 0xBC($a0)
    /* DD9A34 00008FC8 AC4F0008 */  sw         $t7, 0x8($v0)
    /* DD9A38 00008FCC C7A4002C */  lwc1       $f4, 0x2C($sp)
    /* DD9A3C 00008FD0 8C9900C0 */  lw         $t9, 0xC0($a0)
    /* DD9A40 00008FD4 E48400DC */  swc1       $f4, 0xDC($a0)
    /* DD9A44 00008FD8 C7A60030 */  lwc1       $f6, 0x30($sp)
    /* DD9A48 00008FDC E48600E0 */  swc1       $f6, 0xE0($a0)
    /* DD9A4C 00008FE0 C7A80034 */  lwc1       $f8, 0x34($sp)
    /* DD9A50 00008FE4 E48800E4 */  swc1       $f8, 0xE4($a0)
    /* DD9A54 00008FE8 AC590000 */  sw         $t9, 0x0($v0)
    /* DD9A58 00008FEC 8C9800C4 */  lw         $t8, 0xC4($a0)
    /* DD9A5C 00008FF0 AC580004 */  sw         $t8, 0x4($v0)
    /* DD9A60 00008FF4 8C9900C8 */  lw         $t9, 0xC8($a0)
    /* DD9A64 00008FF8 AC590008 */  sw         $t9, 0x8($v0)
    /* DD9A68 00008FFC C7AA002C */  lwc1       $f10, 0x2C($sp)
    /* DD9A6C 00009000 8C8900CC */  lw         $t1, 0xCC($a0)
    /* DD9A70 00009004 E48A00E8 */  swc1       $f10, 0xE8($a0)
    /* DD9A74 00009008 C7B00030 */  lwc1       $f16, 0x30($sp)
    /* DD9A78 0000900C E49000EC */  swc1       $f16, 0xEC($a0)
    /* DD9A7C 00009010 C7B20034 */  lwc1       $f18, 0x34($sp)
    /* DD9A80 00009014 E49200F0 */  swc1       $f18, 0xF0($a0)
    /* DD9A84 00009018 AC490000 */  sw         $t1, 0x0($v0)
    /* DD9A88 0000901C 8C8800D0 */  lw         $t0, 0xD0($a0)
    /* DD9A8C 00009020 AC480004 */  sw         $t0, 0x4($v0)
    /* DD9A90 00009024 8C8900D4 */  lw         $t1, 0xD4($a0)
    /* DD9A94 00009028 AC490008 */  sw         $t1, 0x8($v0)
    /* DD9A98 0000902C C7A4002C */  lwc1       $f4, 0x2C($sp)
    /* DD9A9C 00009030 C48A00D8 */  lwc1       $f10, 0xD8($a0)
    /* DD9AA0 00009034 E48400F4 */  swc1       $f4, 0xF4($a0)
    /* DD9AA4 00009038 C7A60030 */  lwc1       $f6, 0x30($sp)
    /* DD9AA8 0000903C E48600F8 */  swc1       $f6, 0xF8($a0)
    /* DD9AAC 00009040 C7A80034 */  lwc1       $f8, 0x34($sp)
    /* DD9AB0 00009044 E48A0100 */  swc1       $f10, 0x100($a0)
    /* DD9AB4 00009048 E48800FC */  swc1       $f8, 0xFC($a0)
    /* DD9AB8 0000904C 0C000000 */  jal        func_00000000
    /* DD9ABC 00009050 AFA40048 */   sw        $a0, 0x48($sp)
    /* DD9AC0 00009054 8FA50048 */  lw         $a1, 0x48($sp)
    /* DD9AC4 00009058 8CA203D4 */  lw         $v0, 0x3D4($a1)
    /* DD9AC8 0000905C 244AFFFF */  addiu      $t2, $v0, -0x1
    /* DD9ACC 00009060 1C400016 */  bgtz       $v0, .L000090BC
    /* DD9AD0 00009064 ACAA03D4 */   sw        $t2, 0x3D4($a1)
    /* DD9AD4 00009068 3C020000 */  lui        $v0, (0x0 >> 16)
    /* DD9AD8 0000906C 8C420000 */  lw         $v0, 0x0($v0)
    /* DD9ADC 00009070 8C44000C */  lw         $a0, 0xC($v0)
    /* DD9AE0 00009074 8C4B0008 */  lw         $t3, 0x8($v0)
    /* DD9AE4 00009078 008B082A */  slt        $at, $a0, $t3
    /* DD9AE8 0000907C 5420000A */  bnel       $at, $zero, .L000090A8
    /* DD9AEC 00009080 8C4D0000 */   lw        $t5, 0x0($v0)
    /* DD9AF0 00009084 3C040001 */  lui        $a0, %hi(sStr_Too_many_objects_for_List_8714)
    /* DD9AF4 00009088 24848714 */  addiu      $a0, $a0, %lo(sStr_Too_many_objects_for_List_8714)
    /* DD9AF8 0000908C AFA20040 */  sw         $v0, 0x40($sp)
    /* DD9AFC 00009090 0C000000 */  jal        func_00000000
    /* DD9B00 00009094 AFA50048 */   sw        $a1, 0x48($sp)
    /* DD9B04 00009098 8FA20040 */  lw         $v0, 0x40($sp)
    /* DD9B08 0000909C 8FA50048 */  lw         $a1, 0x48($sp)
    /* DD9B0C 000090A0 8C44000C */  lw         $a0, 0xC($v0)
    /* DD9B10 000090A4 8C4D0000 */  lw         $t5, 0x0($v0)
  .L000090A8:
    /* DD9B14 000090A8 248C0001 */  addiu      $t4, $a0, 0x1
    /* DD9B18 000090AC 00047080 */  sll        $t6, $a0, 2
    /* DD9B1C 000090B0 AC4C000C */  sw         $t4, 0xC($v0)
    /* DD9B20 000090B4 01AE7821 */  addu       $t7, $t5, $t6
    /* DD9B24 000090B8 ADE50000 */  sw         $a1, 0x0($t7)
  .L000090BC:
    /* DD9B28 000090BC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD9B2C 000090C0 27BD0048 */  addiu      $sp, $sp, 0x48
    /* DD9B30 000090C4 03E00008 */  jr         $ra
    /* DD9B34 000090C8 00000000 */   nop
endlabel ShpNode_func_00008FA8
