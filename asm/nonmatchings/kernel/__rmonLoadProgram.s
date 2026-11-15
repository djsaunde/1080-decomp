
nonmatching __rmonLoadProgram, 0xC

glabel __rmonLoadProgram
    /* 7C58 80006C58 AFA40000 */  sw         $a0, 0x0($sp)
    /* 7C5C 80006C5C 03E00008 */  jr         $ra
    /* 7C60 80006C60 2402FFFF */   addiu     $v0, $zero, -0x1
endlabel __rmonLoadProgram
