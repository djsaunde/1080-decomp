nonmatching func_000083D0, 0x78

glabel func_000083D0
    /* DD8E3C 000083D0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD8E40 000083D4 AFA40018 */  sw         $a0, 0x18($sp)
    /* DD8E44 000083D8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD8E48 000083DC 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD8E4C 000083E0 3C050000 */  lui        $a1, %hi(D_00007FD4)
    /* DD8E50 000083E4 24A57FD4 */  addiu      $a1, $a1, %lo(D_00007FD4)
    /* DD8E54 000083E8 24840000 */  addiu      $a0, $a0, 0x0
    /* DD8E58 000083EC 0C000000 */  jal        func_00000000
    /* DD8E5C 000083F0 00003025 */   or        $a2, $zero, $zero
    /* DD8E60 000083F4 8FA60018 */  lw         $a2, 0x18($sp)
    /* DD8E64 000083F8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD8E68 000083FC 3C050000 */  lui        $a1, %hi(D_00007FDC)
    /* DD8E6C 00008400 24A57FDC */  addiu      $a1, $a1, %lo(D_00007FDC)
    /* DD8E70 00008404 24840000 */  addiu      $a0, $a0, 0x0
    /* DD8E74 00008408 24070001 */  addiu      $a3, $zero, 0x1
    /* DD8E78 0000840C 0C000000 */  jal        func_00000000
    /* DD8E7C 00008410 24C6006C */   addiu     $a2, $a2, 0x6C
    /* DD8E80 00008414 8FA40018 */  lw         $a0, 0x18($sp)
    /* DD8E84 00008418 00002825 */  or         $a1, $zero, $zero
    /* DD8E88 0000841C 0C000000 */  jal        func_00000000
    /* DD8E8C 00008420 2484003C */   addiu     $a0, $a0, 0x3C
    /* DD8E90 00008424 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD8E94 00008428 0C000000 */  jal        func_00000000
    /* DD8E98 0000842C 24840000 */   addiu     $a0, $a0, 0x0
    /* DD8E9C 00008430 0C000000 */  jal        func_00000000
    /* DD8EA0 00008434 8FA40018 */   lw        $a0, 0x18($sp)
    /* DD8EA4 00008438 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD8EA8 0000843C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD8EAC 00008440 03E00008 */  jr         $ra
    /* DD8EB0 00008444 00000000 */   nop
endlabel func_000083D0
