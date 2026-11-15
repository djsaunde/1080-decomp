nonmatching func_0000E270, 0x60

glabel func_0000E270
    /* DDECDC 0000E270 3C010000 */  lui        $at, %hi(SndHandle_triggerEvent1_0000098C + 0xC)
    /* DDECE0 0000E274 44856000 */  mtc1       $a1, $f12
    /* DDECE4 0000E278 C4240998 */  lwc1       $f4, %lo(SndHandle_triggerEvent1_0000098C + 0xC)($at)
    /* DDECE8 0000E27C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* DDECEC 0000E280 00803825 */  or         $a3, $a0, $zero
    /* DDECF0 0000E284 460C2003 */  div.s      $f0, $f4, $f12
    /* DDECF4 0000E288 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDECF8 0000E28C 24E503B0 */  addiu      $a1, $a3, 0x3B0
    /* DDECFC 0000E290 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DDED00 0000E294 AFA70020 */  sw         $a3, 0x20($sp)
    /* DDED04 0000E298 248400CC */  addiu      $a0, $a0, 0xCC
    /* DDED08 0000E29C 44060000 */  mfc1       $a2, $f0
    /* DDED0C 0000E2A0 0C000000 */  jal        func_00000000
    /* DDED10 0000E2A4 E7A00018 */   swc1      $f0, 0x18($sp)
    /* DDED14 0000E2A8 C7A00018 */  lwc1       $f0, 0x18($sp)
    /* DDED18 0000E2AC 8FA70020 */  lw         $a3, 0x20($sp)
    /* DDED1C 0000E2B0 8FA5001C */  lw         $a1, 0x1C($sp)
    /* DDED20 0000E2B4 44060000 */  mfc1       $a2, $f0
    /* DDED24 0000E2B8 0C000000 */  jal        func_00000000
    /* DDED28 0000E2BC 24E400F4 */   addiu     $a0, $a3, 0xF4
    /* DDED2C 0000E2C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDED30 0000E2C4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* DDED34 0000E2C8 03E00008 */  jr         $ra
    /* DDED38 0000E2CC 00000000 */   nop
endlabel func_0000E270
