nonmatching settings_vol0Down_000124C4, 0x68

glabel settings_vol0Down_000124C4
    /* DE2F30 000124C4 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* DE2F34 000124C8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DE2F38 000124CC AFA40028 */  sw         $a0, 0x28($sp)
    /* DE2F3C 000124D0 AFB00018 */  sw         $s0, 0x18($sp)
    /* DE2F40 000124D4 8FAE0028 */  lw         $t6, 0x28($sp)
    /* DE2F44 000124D8 8DCF0154 */  lw         $t7, 0x154($t6)
    /* DE2F48 000124DC 91F80013 */  lbu        $t8, 0x13($t7)
    /* DE2F4C 000124E0 1B000006 */  blez       $t8, .L000124FC
    /* DE2F50 000124E4 00000000 */   nop
    /* DE2F54 000124E8 8FB90028 */  lw         $t9, 0x28($sp)
    /* DE2F58 000124EC 8F300154 */  lw         $s0, 0x154($t9)
    /* DE2F5C 000124F0 92080013 */  lbu        $t0, 0x13($s0)
    /* DE2F60 000124F4 2509FFFF */  addiu      $t1, $t0, -0x1
    /* DE2F64 000124F8 A2090013 */  sb         $t1, 0x13($s0)
  .L000124FC:
    /* DE2F68 000124FC 8FAA0028 */  lw         $t2, 0x28($sp)
    /* DE2F6C 00012500 8D4B0154 */  lw         $t3, 0x154($t2)
    /* DE2F70 00012504 91640013 */  lbu        $a0, 0x13($t3)
    /* DE2F74 00012508 0C000000 */  jal        func_00000000
    /* DE2F78 0001250C 00000000 */   nop
    /* DE2F7C 00012510 10000001 */  b          .L00012518
    /* DE2F80 00012514 00000000 */   nop
  .L00012518:
    /* DE2F84 00012518 8FB00018 */  lw         $s0, 0x18($sp)
    /* DE2F88 0001251C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DE2F8C 00012520 27BD0028 */  addiu      $sp, $sp, 0x28
    /* DE2F90 00012524 03E00008 */  jr         $ra
    /* DE2F94 00012528 00000000 */   nop
endlabel settings_vol0Down_000124C4
