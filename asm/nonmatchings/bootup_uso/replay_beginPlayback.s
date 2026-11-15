nonmatching replay_beginPlayback, 0x7C

glabel replay_beginPlayback
    /* DE35E8 00012B7C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DE35EC 00012B80 AFBF0014 */  sw         $ra, 0x14($sp)
    /* DE35F0 00012B84 AFA40018 */  sw         $a0, 0x18($sp)
    /* DE35F4 00012B88 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DE35F8 00012B8C AFA60020 */  sw         $a2, 0x20($sp)
    /* DE35FC 00012B90 8FAE001C */  lw         $t6, 0x1C($sp)
    /* DE3600 00012B94 11C00007 */  beqz       $t6, .L00012BB4
    /* DE3604 00012B98 00000000 */   nop
    /* DE3608 00012B9C 8FA4001C */  lw         $a0, 0x1C($sp)
    /* DE360C 00012BA0 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DE3610 00012BA4 8DE50168 */  lw         $a1, 0x168($t7)
    /* DE3614 00012BA8 8DE60170 */  lw         $a2, 0x170($t7)
    /* DE3618 00012BAC 0C000000 */  jal        func_00000000
    /* DE361C 00012BB0 00000000 */   nop
  .L00012BB4:
    /* DE3620 00012BB4 8FB80020 */  lw         $t8, 0x20($sp)
    /* DE3624 00012BB8 13000009 */  beqz       $t8, .L00012BE0
    /* DE3628 00012BBC 00000000 */   nop
    /* DE362C 00012BC0 8FA40020 */  lw         $a0, 0x20($sp)
    /* DE3630 00012BC4 8FB90018 */  lw         $t9, 0x18($sp)
    /* DE3634 00012BC8 8F28014C */  lw         $t0, 0x14C($t9)
    /* DE3638 00012BCC 8D090008 */  lw         $t1, 0x8($t0)
    /* DE363C 00012BD0 8D250008 */  lw         $a1, 0x8($t1)
    /* DE3640 00012BD4 8D260004 */  lw         $a2, 0x4($t1)
    /* DE3644 00012BD8 0C000000 */  jal        func_00000000
    /* DE3648 00012BDC 00000000 */   nop
  .L00012BE0:
    /* DE364C 00012BE0 10000001 */  b          .L00012BE8
    /* DE3650 00012BE4 00000000 */   nop
  .L00012BE8:
    /* DE3654 00012BE8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DE3658 00012BEC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DE365C 00012BF0 03E00008 */  jr         $ra
    /* DE3660 00012BF4 00000000 */   nop
endlabel replay_beginPlayback
