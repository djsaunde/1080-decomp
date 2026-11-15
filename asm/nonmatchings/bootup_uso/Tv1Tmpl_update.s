nonmatching Tv1Tmpl_update, 0x20

glabel Tv1Tmpl_update
    /* DD88BC 00007E50 27BDFFE8 */  addiu      $sp, $sp, -0x18
  alabel sTag_lig_7E54
    /* DD88C0 00007E54 AFBF0014 */  sw         $ra, 0x14($sp)
  alabel sTag_gol_7E58
    /* DD88C4 00007E58 0C000000 */  jal        func_00000000
  alabel sTag_st0_7E5C
    /* DD88C8 00007E5C AC800068 */   sw        $zero, 0x68($a0)
  alabel sTag_en1_7E60
    /* DD88CC 00007E60 8FBF0014 */  lw         $ra, 0x14($sp)
  alabel sTag_en2_7E64
    /* DD88D0 00007E64 27BD0018 */  addiu      $sp, $sp, 0x18
  alabel sTag_cam_7E68
    /* DD88D4 00007E68 03E00008 */  jr         $ra
  alabel sStr_BGBox_7E6C
    /* DD88D8 00007E6C 00000000 */   nop
endlabel Tv1Tmpl_update
