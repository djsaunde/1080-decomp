nonmatching replay_beginRecordWithGhost, 0xF4

glabel replay_beginRecordWithGhost
    /* DE3318 000128AC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE331C 000128B0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE3320 000128B4 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE3324 000128B8 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE3328 000128BC AFA60020 */  sw         $a2, 0x20($sp)
    /* DE332C 000128C0 8FAE0020 */  lw         $t6, 0x20($sp)
    /* DE3330 000128C4 11C0000D */  beqz       $t6, .L000128FC
    /* DE3334 000128C8 00000000 */   nop
    /* DE3338 000128CC 3C0FFFFB */  lui        $t7, (0xFFFBFFFF >> 16)
    /* DE333C 000128D0 35EFFFFF */  ori        $t7, $t7, (0xFFFBFFFF & 0xFFFF)
    /* DE3340 000128D4 8FB80020 */  lw         $t8, 0x20($sp)
    /* DE3344 000128D8 AF0F006C */  sw         $t7, 0x6C($t8)
    /* DE3348 000128DC 8FA40020 */  lw         $a0, 0x20($sp)
    /* DE334C 000128E0 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE3350 000128E4 8F28014C */  lw         $t0, 0x14C($t9)
    /* DE3354 000128E8 8D090008 */  lw         $t1, 0x8($t0)
    /* DE3358 000128EC 8D250008 */  lw         $a1, 0x8($t1)
    /* DE335C 000128F0 8D260004 */  lw         $a2, 0x4($t1)
    /* DE3360 000128F4 0C000000 */  jal        func_00000000
    /* DE3364 000128F8 00000000 */   nop
  .L000128FC:
    /* DE3368 000128FC 8FAA001C */  lw         $t2, 0x1C($sp)
    /* DE336C 00012900 11400021 */  beqz       $t2, .L00012988
    /* DE3370 00012904 00000000 */   nop
    /* DE3374 00012908 8FAB0018 */  lw         $t3, 0x18($sp)
    /* DE3378 0001290C 8D6C0168 */  lw         $t4, 0x168($t3)
    /* DE337C 00012910 15800004 */  bnez       $t4, .L00012924
    /* DE3380 00012914 00000000 */   nop
    /* DE3384 00012918 8FAD0018 */  lw         $t5, 0x18($sp)
    /* DE3388 0001291C 8DAE0160 */  lw         $t6, 0x160($t5)
    /* DE338C 00012920 ADAE0168 */  sw         $t6, 0x168($t5)
  .L00012924:
    /* DE3390 00012924 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE3394 00012928 8DF80064 */  lw         $t8, 0x64($t7)
    /* DE3398 0001292C A1F80178 */  sb         $t8, 0x178($t7)
    /* DE339C 00012930 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE33A0 00012934 8F28004C */  lw         $t0, 0x4C($t9)
    /* DE33A4 00012938 A3280179 */  sb         $t0, 0x179($t9)
    /* DE33A8 0001293C 8FA90018 */  lw         $t1, 0x18($sp)
    /* DE33AC 00012940 8D2A0050 */  lw         $t2, 0x50($t1)
    /* DE33B0 00012944 A12A017A */  sb         $t2, 0x17A($t1)
    /* DE33B4 00012948 8FAB0018 */  lw         $t3, 0x18($sp)
    /* DE33B8 0001294C 8D6C0054 */  lw         $t4, 0x54($t3)
    /* DE33BC 00012950 A16C017B */  sb         $t4, 0x17B($t3)
    /* DE33C0 00012954 3C0EFFFB */  lui        $t6, (0xFFFBFFFF >> 16)
    /* DE33C4 00012958 35CEFFFF */  ori        $t6, $t6, (0xFFFBFFFF & 0xFFFF)
    /* DE33C8 0001295C 8FAD001C */  lw         $t5, 0x1C($sp)
    /* DE33CC 00012960 ADAE006C */  sw         $t6, 0x6C($t5)
    /* DE33D0 00012964 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DE33D4 00012968 8FB80018 */  lw         $t8, 0x18($sp)
    /* DE33D8 0001296C 8F050168 */  lw         $a1, 0x168($t8)
    /* DE33DC 00012970 8F060164 */  lw         $a2, 0x164($t8)
    /* DE33E0 00012974 27070170 */  addiu      $a3, $t8, 0x170
    /* DE33E4 00012978 0C000000 */  jal        func_00000000
    /* DE33E8 0001297C 00000000 */   nop
    /* DE33EC 00012980 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE33F0 00012984 ADE00184 */  sw         $zero, 0x184($t7)
  .L00012988:
    /* DE33F4 00012988 10000001 */  b          .L00012990
    /* DE33F8 0001298C 00000000 */   nop
  .L00012990:
    /* DE33FC 00012990 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE3400 00012994 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE3404 00012998 03E00008 */  jr         $ra
    /* DE3408 0001299C 00000000 */   nop
endlabel replay_beginRecordWithGhost
