nonmatching Mountain_display, 0xB8

glabel Mountain_display
    /* DD52C8 0000485C 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* DD52CC 00004860 AFA40050 */  sw         $a0, 0x50($sp)
    /* DD52D0 00004864 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD52D4 00004868 3C040000 */  lui        $a0, %hi(func_00000080 + 0x20)
    /* DD52D8 0000486C 0C000000 */  jal        func_00000000
    /* DD52DC 00004870 248400A0 */   addiu     $a0, $a0, %lo(func_00000080 + 0x20)
    /* DD52E0 00004874 8FA60050 */  lw         $a2, 0x50($sp)
    /* DD52E4 00004878 3C040000 */  lui        $a0, %hi(func_00000080 + 0x20)
    /* DD52E8 0000487C 248400A0 */  addiu      $a0, $a0, %lo(func_00000080 + 0x20)
    /* DD52EC 00004880 C4CC0030 */  lwc1       $f12, 0x30($a2)
    /* DD52F0 00004884 C4C20034 */  lwc1       $f2, 0x34($a2)
    /* DD52F4 00004888 27A50040 */  addiu      $a1, $sp, 0x40
    /* DD52F8 0000488C E7AC0048 */  swc1       $f12, 0x48($sp)
    /* DD52FC 00004890 E7AC0040 */  swc1       $f12, 0x40($sp)
    /* DD5300 00004894 0C000000 */  jal        func_00000000
    /* DD5304 00004898 E7A20044 */   swc1      $f2, 0x44($sp)
    /* DD5308 0000489C 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD530C 000048A0 8DCE0000 */  lw         $t6, 0x0($t6)
    /* DD5310 000048A4 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD5314 000048A8 24630000 */  addiu      $v1, $v1, 0x0
    /* DD5318 000048AC 8DC20070 */  lw         $v0, 0x70($t6)
    /* DD531C 000048B0 C4660128 */  lwc1       $f6, 0x128($v1)
    /* DD5320 000048B4 C46A012C */  lwc1       $f10, 0x12C($v1)
    /* DD5324 000048B8 C44400A0 */  lwc1       $f4, 0xA0($v0)
    /* DD5328 000048BC C44800A4 */  lwc1       $f8, 0xA4($v0)
    /* DD532C 000048C0 8FA60050 */  lw         $a2, 0x50($sp)
    /* DD5330 000048C4 46062002 */  mul.s      $f0, $f4, $f6
    /* DD5334 000048C8 C4660130 */  lwc1       $f6, 0x130($v1)
    /* DD5338 000048CC C44400A8 */  lwc1       $f4, 0xA8($v0)
    /* DD533C 000048D0 460A4402 */  mul.s      $f16, $f8, $f10
    /* DD5340 000048D4 C4D2002C */  lwc1       $f18, 0x2C($a2)
    /* DD5344 000048D8 3C040000 */  lui        $a0, %hi(func_00000080 + 0x20)
    /* DD5348 000048DC 46062302 */  mul.s      $f12, $f4, $f6
    /* DD534C 000048E0 248400A0 */  addiu      $a0, $a0, %lo(func_00000080 + 0x20)
    /* DD5350 000048E4 E7A00020 */  swc1       $f0, 0x20($sp)
    /* DD5354 000048E8 27A50020 */  addiu      $a1, $sp, 0x20
    /* DD5358 000048EC 46128080 */  add.s      $f2, $f16, $f18
    /* DD535C 000048F0 E7AC0028 */  swc1       $f12, 0x28($sp)
    /* DD5360 000048F4 0C000000 */  jal        func_00000000
    /* DD5364 000048F8 E7A20024 */   swc1      $f2, 0x24($sp)
    /* DD5368 000048FC 0C000000 */  jal        func_00000000
    /* DD536C 00004900 8FA40050 */   lw        $a0, 0x50($sp)
    /* DD5370 00004904 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD5374 00004908 27BD0050 */  addiu      $sp, $sp, 0x50
    /* DD5378 0000490C 03E00008 */  jr         $ra
    /* DD537C 00004910 00000000 */   nop
endlabel Mountain_display
