nonmatching SnowBoarder_func_0000E124, 0x14C

glabel SnowBoarder_func_0000E124
    /* DDEB90 0000E124 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* DDEB94 0000E128 AFBF001C */  sw         $ra, 0x1C($sp)
    /* DDEB98 0000E12C AFB00018 */  sw         $s0, 0x18($sp)
    /* DDEB9C 0000E130 8C8E0A58 */  lw         $t6, 0xA58($a0)
    /* DDEBA0 0000E134 44802000 */  mtc1       $zero, $f4
    /* DDEBA4 0000E138 00808025 */  or         $s0, $a0, $zero
    /* DDEBA8 0000E13C 39CF0080 */  xori       $t7, $t6, 0x80
    /* DDEBAC 0000E140 31F90008 */  andi       $t9, $t7, 0x8
    /* DDEBB0 0000E144 AC8F0A58 */  sw         $t7, 0xA58($a0)
    /* DDEBB4 0000E148 13200005 */  beqz       $t9, .L0000E160
    /* DDEBB8 0000E14C E4840A38 */   swc1      $f4, 0xA38($a0)
    /* DDEBBC 0000E150 0C000000 */  jal        func_00000000
    /* DDEBC0 0000E154 00000000 */   nop
    /* DDEBC4 0000E158 10000004 */  b          .L0000E16C
    /* DDEBC8 0000E15C C6060978 */   lwc1      $f6, 0x978($s0)
  .L0000E160:
    /* DDEBCC 0000E160 0C000000 */  jal        func_00000000
    /* DDEBD0 0000E164 02002025 */   or        $a0, $s0, $zero
    /* DDEBD4 0000E168 C6060978 */  lwc1       $f6, 0x978($s0)
  .L0000E16C:
    /* DDEBD8 0000E16C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDEBDC 0000E170 44810000 */  mtc1       $at, $f0
    /* DDEBE0 0000E174 46003207 */  neg.s      $f8, $f6
    /* DDEBE4 0000E178 44805000 */  mtc1       $zero, $f10
    /* DDEBE8 0000E17C 44808000 */  mtc1       $zero, $f16
    /* DDEBEC 0000E180 E6080978 */  swc1       $f8, 0x978($s0)
    /* DDEBF0 0000E184 E7A00050 */  swc1       $f0, 0x50($sp)
    /* DDEBF4 0000E188 E7AA004C */  swc1       $f10, 0x4C($sp)
    /* DDEBF8 0000E18C E7B00054 */  swc1       $f16, 0x54($sp)
    /* DDEBFC 0000E190 8E080A58 */  lw         $t0, 0xA58($s0)
    /* DDEC00 0000E194 31090080 */  andi       $t1, $t0, 0x80
    /* DDEC04 0000E198 5120001B */  beql       $t1, $zero, .L0000E208
    /* DDEC08 0000E19C 8E040844 */   lw        $a0, 0x844($s0)
    /* DDEC0C 0000E1A0 8E040844 */  lw         $a0, 0x844($s0)
    /* DDEC10 0000E1A4 3C01BF80 */  lui        $at, (0xBF800000 >> 16)
    /* DDEC14 0000E1A8 44812000 */  mtc1       $at, $f4
    /* DDEC18 0000E1AC C4920030 */  lwc1       $f18, 0x30($a0)
    /* DDEC1C 0000E1B0 24840030 */  addiu      $a0, $a0, 0x30
    /* DDEC20 0000E1B4 46049182 */  mul.s      $f6, $f18, $f4
    /* DDEC24 0000E1B8 E4860000 */  swc1       $f6, 0x0($a0)
    /* DDEC28 0000E1BC 8E020864 */  lw         $v0, 0x864($s0)
    /* DDEC2C 0000E1C0 5040001D */  beql       $v0, $zero, .L0000E238
    /* DDEC30 0000E1C4 8E0A0A58 */   lw        $t2, 0xA58($s0)
    /* DDEC34 0000E1C8 0C000000 */  jal        func_00000000
    /* DDEC38 0000E1CC 24440030 */   addiu     $a0, $v0, 0x30
    /* DDEC3C 0000E1D0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DDEC40 0000E1D4 44810000 */  mtc1       $at, $f0
    /* DDEC44 0000E1D8 8E040864 */  lw         $a0, 0x864($s0)
    /* DDEC48 0000E1DC 3C01BF80 */  lui        $at, (0xBF800000 >> 16)
    /* DDEC4C 0000E1E0 44814000 */  mtc1       $at, $f8
    /* DDEC50 0000E1E4 27A5002C */  addiu      $a1, $sp, 0x2C
    /* DDEC54 0000E1E8 24840030 */  addiu      $a0, $a0, 0x30
    /* DDEC58 0000E1EC E7A00034 */  swc1       $f0, 0x34($sp)
    /* DDEC5C 0000E1F0 E7A00030 */  swc1       $f0, 0x30($sp)
    /* DDEC60 0000E1F4 0C000000 */  jal        func_00000000
    /* DDEC64 0000E1F8 E7A8002C */   swc1      $f8, 0x2C($sp)
    /* DDEC68 0000E1FC 1000000E */  b          .L0000E238
    /* DDEC6C 0000E200 8E0A0A58 */   lw        $t2, 0xA58($s0)
    /* DDEC70 0000E204 8E040844 */  lw         $a0, 0x844($s0)
  .L0000E208:
    /* DDEC74 0000E208 3C01BF80 */  lui        $at, (0xBF800000 >> 16)
    /* DDEC78 0000E20C 44818000 */  mtc1       $at, $f16
    /* DDEC7C 0000E210 C48A0030 */  lwc1       $f10, 0x30($a0)
    /* DDEC80 0000E214 24840030 */  addiu      $a0, $a0, 0x30
    /* DDEC84 0000E218 46105482 */  mul.s      $f18, $f10, $f16
    /* DDEC88 0000E21C E4920000 */  swc1       $f18, 0x0($a0)
    /* DDEC8C 0000E220 8E020864 */  lw         $v0, 0x864($s0)
    /* DDEC90 0000E224 50400004 */  beql       $v0, $zero, .L0000E238
    /* DDEC94 0000E228 8E0A0A58 */   lw        $t2, 0xA58($s0)
    /* DDEC98 0000E22C 0C000000 */  jal        func_00000000
    /* DDEC9C 0000E230 24440030 */   addiu     $a0, $v0, 0x30
    /* DDECA0 0000E234 8E0A0A58 */  lw         $t2, 0xA58($s0)
  .L0000E238:
    /* DDECA4 0000E238 314B0020 */  andi       $t3, $t2, 0x20
    /* DDECA8 0000E23C 51600008 */  beql       $t3, $zero, .L0000E260
    /* DDECAC 0000E240 8FBF001C */   lw        $ra, 0x1C($sp)
    /* DDECB0 0000E244 8E020864 */  lw         $v0, 0x864($s0)
    /* DDECB4 0000E248 27A5004C */  addiu      $a1, $sp, 0x4C
    /* DDECB8 0000E24C 10400003 */  beqz       $v0, .L0000E25C
    /* DDECBC 0000E250 24440030 */   addiu     $a0, $v0, 0x30
    /* DDECC0 0000E254 0C000000 */  jal        func_00000000
    /* DDECC4 0000E258 3C064334 */   lui       $a2, (0x43340000 >> 16)
  .L0000E25C:
    /* DDECC8 0000E25C 8FBF001C */  lw         $ra, 0x1C($sp)
  .L0000E260:
    /* DDECCC 0000E260 8FB00018 */  lw         $s0, 0x18($sp)
    /* DDECD0 0000E264 27BD0058 */  addiu      $sp, $sp, 0x58
    /* DDECD4 0000E268 03E00008 */  jr         $ra
    /* DDECD8 0000E26C 00000000 */   nop
endlabel SnowBoarder_func_0000E124
