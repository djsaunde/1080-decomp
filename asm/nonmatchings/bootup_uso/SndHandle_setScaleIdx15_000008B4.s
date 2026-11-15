nonmatching SndHandle_setScaleIdx15_000008B4, 0x20

glabel SndHandle_setScaleIdx15_000008B4
    /* DD1320 000008B4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DD1324 000008B8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DD1328 000008BC 0C000000 */  jal        func_00000000
    /* DD132C 000008C0 8C840000 */   lw        $a0, 0x0($a0)
    /* DD1330 000008C4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DD1334 000008C8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DD1338 000008CC 03E00008 */  jr         $ra
    /* DD133C 000008D0 00000000 */   nop
endlabel SndHandle_setScaleIdx15_000008B4
