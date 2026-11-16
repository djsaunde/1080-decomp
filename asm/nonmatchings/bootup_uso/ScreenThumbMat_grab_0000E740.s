nonmatching ScreenThumbMat_grab_0000E740, 0xB4

glabel ScreenThumbMat_grab_0000E740
    /* DDF1AC 0000E740 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* DDF1B0 0000E744 3C030000 */  lui        $v1, %hi(func_0000023C + 0x4)
    /* DDF1B4 0000E748 8C630240 */  lw         $v1, %lo(func_0000023C + 0x4)($v1)
    /* DDF1B8 0000E74C AFBF0014 */  sw         $ra, 0x14($sp)
    /* DDF1BC 0000E750 AFA40018 */  sw         $a0, 0x18($sp)
    /* DDF1C0 0000E754 AFA5001C */  sw         $a1, 0x1C($sp)
    /* DDF1C4 0000E758 8C620028 */  lw         $v0, 0x28($v1)
    /* DDF1C8 0000E75C 8C590064 */  lw         $t9, 0x64($v0)
    /* DDF1CC 0000E760 844E0060 */  lh         $t6, 0x60($v0)
    /* DDF1D0 0000E764 0320F809 */  jalr       $t9
    /* DDF1D4 0000E768 01C32021 */   addu      $a0, $t6, $v1
    /* DDF1D8 0000E76C 8FAF0018 */  lw         $t7, 0x18($sp)
    /* DDF1DC 0000E770 34019140 */  ori        $at, $zero, 0x9140
    /* DDF1E0 0000E774 00411821 */  addu       $v1, $v0, $at
    /* DDF1E4 0000E778 00003825 */  or         $a3, $zero, $zero
    /* DDF1E8 0000E77C 24080020 */  addiu      $t0, $zero, 0x20
    /* DDF1EC 0000E780 24060040 */  addiu      $a2, $zero, 0x40
    /* DDF1F0 0000E784 8DE4001C */  lw         $a0, 0x1C($t7)
  .L0000E788:
    /* DDF1F4 0000E788 00002825 */  or         $a1, $zero, $zero
  .L0000E78C:
    /* DDF1F8 0000E78C 94780000 */  lhu        $t8, 0x0($v1)
    /* DDF1FC 0000E790 00801025 */  or         $v0, $a0, $zero
    /* DDF200 0000E794 24840002 */  addiu      $a0, $a0, 0x2
    /* DDF204 0000E798 A4580000 */  sh         $t8, 0x0($v0)
    /* DDF208 0000E79C 94690008 */  lhu        $t1, 0x8($v1)
    /* DDF20C 0000E7A0 00801025 */  or         $v0, $a0, $zero
    /* DDF210 0000E7A4 24840002 */  addiu      $a0, $a0, 0x2
    /* DDF214 0000E7A8 A4490000 */  sh         $t1, 0x0($v0)
    /* DDF218 0000E7AC 946A0010 */  lhu        $t2, 0x10($v1)
    /* DDF21C 0000E7B0 00801025 */  or         $v0, $a0, $zero
    /* DDF220 0000E7B4 24840002 */  addiu      $a0, $a0, 0x2
    /* DDF224 0000E7B8 A44A0000 */  sh         $t2, 0x0($v0)
    /* DDF228 0000E7BC 946B0018 */  lhu        $t3, 0x18($v1)
    /* DDF22C 0000E7C0 00801025 */  or         $v0, $a0, $zero
    /* DDF230 0000E7C4 24A50004 */  addiu      $a1, $a1, 0x4
    /* DDF234 0000E7C8 24840002 */  addiu      $a0, $a0, 0x2
    /* DDF238 0000E7CC 24630020 */  addiu      $v1, $v1, 0x20
    /* DDF23C 0000E7D0 14A6FFEE */  bne        $a1, $a2, .L0000E78C
    /* DDF240 0000E7D4 A44B0000 */   sh        $t3, 0x0($v0)
    /* DDF244 0000E7D8 24E70001 */  addiu      $a3, $a3, 0x1
    /* DDF248 0000E7DC 14E8FFEA */  bne        $a3, $t0, .L0000E788
    /* DDF24C 0000E7E0 24630A80 */   addiu     $v1, $v1, 0xA80
    /* DDF250 0000E7E4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* DDF254 0000E7E8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* DDF258 0000E7EC 03E00008 */  jr         $ra
    /* DDF25C 0000E7F0 00000000 */   nop
endlabel ScreenThumbMat_grab_0000E740
    /* DDF260 0000E7F4 00000000 */  nop
    /* DDF264 0000E7F8 00000000 */  nop
    /* DDF268 0000E7FC 00000000 */  nop
