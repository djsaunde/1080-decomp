nonmatching WorldRegistry_create_00001E08, 0x170

glabel WorldRegistry_create_00001E08
    /* DD2874 00001E08 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* DD2878 00001E0C AFB00018 */  sw         $s0, 0x18($sp)
    /* DD287C 00001E10 00808025 */  or         $s0, $a0, $zero
    /* DD2880 00001E14 14800005 */  bnez       $a0, .L00001E2C
    /* DD2884 00001E18 AFBF001C */   sw        $ra, 0x1C($sp)
    /* DD2888 00001E1C 0C000000 */  jal        func_00000000
    /* DD288C 00001E20 24040118 */   addiu     $a0, $zero, 0x118
    /* DD2890 00001E24 1040004F */  beqz       $v0, .L00001F64
    /* DD2894 00001E28 00408025 */   or        $s0, $v0, $zero
  .L00001E2C:
    /* DD2898 00001E2C 3C050000 */  lui        $a1, %hi(D_00006E30)
    /* DD289C 00001E30 24A56E30 */  addiu      $a1, $a1, %lo(D_00006E30)
    /* DD28A0 00001E34 0C000000 */  jal        func_00000000
    /* DD28A4 00001E38 02002025 */   or        $a0, $s0, $zero
    /* DD28A8 00001E3C 3C0E0000 */  lui        $t6, (0x0 >> 16)
    /* DD28AC 00001E40 25CE0000 */  addiu      $t6, $t6, 0x0
    /* DD28B0 00001E44 AE0E0028 */  sw         $t6, 0x28($s0)
    /* DD28B4 00001E48 2607002C */  addiu      $a3, $s0, 0x2C
    /* DD28B8 00001E4C 00E02025 */  or         $a0, $a3, $zero
    /* DD28BC 00001E50 0C000000 */  jal        func_00000000
    /* DD28C0 00001E54 AFA70024 */   sw        $a3, 0x24($sp)
    /* DD28C4 00001E58 8FA70024 */  lw         $a3, 0x24($sp)
    /* DD28C8 00001E5C 3C0F0000 */  lui        $t7, %hi(D_00006E38)
    /* DD28CC 00001E60 25EF6E38 */  addiu      $t7, $t7, %lo(D_00006E38)
    /* DD28D0 00001E64 3C050000 */  lui        $a1, %hi(D_00006E48)
    /* DD28D4 00001E68 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD28D8 00001E6C AE0F000C */  sw         $t7, 0xC($s0)
    /* DD28DC 00001E70 AE000070 */  sw         $zero, 0x70($s0)
    /* DD28E0 00001E74 AE000078 */  sw         $zero, 0x78($s0)
    /* DD28E4 00001E78 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD28E8 00001E7C 24A56E48 */  addiu      $a1, $a1, %lo(D_00006E48)
    /* DD28EC 00001E80 02002025 */  or         $a0, $s0, $zero
    /* DD28F0 00001E84 0C000000 */  jal        func_00000000
    /* DD28F4 00001E88 AE070074 */   sw        $a3, 0x74($s0)
    /* DD28F8 00001E8C 3C050000 */  lui        $a1, %hi(D_00006E54)
    /* DD28FC 00001E90 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2900 00001E94 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD2904 00001E98 24A56E54 */  addiu      $a1, $a1, %lo(D_00006E54)
    /* DD2908 00001E9C 0C000000 */  jal        func_00000000
    /* DD290C 00001EA0 02002025 */   or        $a0, $s0, $zero
    /* DD2910 00001EA4 3C050000 */  lui        $a1, %hi(D_00006E60)
    /* DD2914 00001EA8 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2918 00001EAC 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD291C 00001EB0 24A56E60 */  addiu      $a1, $a1, %lo(D_00006E60)
    /* DD2920 00001EB4 0C000000 */  jal        func_00000000
    /* DD2924 00001EB8 02002025 */   or        $a0, $s0, $zero
    /* DD2928 00001EBC 3C050000 */  lui        $a1, %hi(D_00006E70)
    /* DD292C 00001EC0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2930 00001EC4 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD2934 00001EC8 24A56E70 */  addiu      $a1, $a1, %lo(D_00006E70)
    /* DD2938 00001ECC 0C000000 */  jal        func_00000000
    /* DD293C 00001ED0 02002025 */   or        $a0, $s0, $zero
    /* DD2940 00001ED4 3C050000 */  lui        $a1, %hi(D_00006E80)
    /* DD2944 00001ED8 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2948 00001EDC 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD294C 00001EE0 24A56E80 */  addiu      $a1, $a1, %lo(D_00006E80)
    /* DD2950 00001EE4 0C000000 */  jal        func_00000000
    /* DD2954 00001EE8 02002025 */   or        $a0, $s0, $zero
    /* DD2958 00001EEC 3C050000 */  lui        $a1, %hi(D_00006E8C)
    /* DD295C 00001EF0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2960 00001EF4 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD2964 00001EF8 24A56E8C */  addiu      $a1, $a1, %lo(D_00006E8C)
    /* DD2968 00001EFC 0C000000 */  jal        func_00000000
    /* DD296C 00001F00 02002025 */   or        $a0, $s0, $zero
    /* DD2970 00001F04 3C050000 */  lui        $a1, %hi(D_00006E98)
    /* DD2974 00001F08 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2978 00001F0C 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD297C 00001F10 24A56E98 */  addiu      $a1, $a1, %lo(D_00006E98)
    /* DD2980 00001F14 0C000000 */  jal        func_00000000
    /* DD2984 00001F18 02002025 */   or        $a0, $s0, $zero
    /* DD2988 00001F1C 3C050000 */  lui        $a1, %hi(.L00006EA8)
    /* DD298C 00001F20 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD2990 00001F24 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD2994 00001F28 24A56EA8 */  addiu      $a1, $a1, %lo(.L00006EA8)
    /* DD2998 00001F2C 0C000000 */  jal        func_00000000
    /* DD299C 00001F30 02002025 */   or        $a0, $s0, $zero
    /* DD29A0 00001F34 3C050000 */  lui        $a1, %hi(D_00006EB8)
    /* DD29A4 00001F38 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD29A8 00001F3C 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD29AC 00001F40 24A56EB8 */  addiu      $a1, $a1, %lo(D_00006EB8)
    /* DD29B0 00001F44 0C000000 */  jal        func_00000000
    /* DD29B4 00001F48 02002025 */   or        $a0, $s0, $zero
    /* DD29B8 00001F4C 3C050000 */  lui        $a1, %hi(D_00006EC4)
    /* DD29BC 00001F50 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD29C0 00001F54 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD29C4 00001F58 24A56EC4 */  addiu      $a1, $a1, %lo(D_00006EC4)
    /* DD29C8 00001F5C 0C000000 */  jal        func_00000000
    /* DD29CC 00001F60 02002025 */   or        $a0, $s0, $zero
  .L00001F64:
    /* DD29D0 00001F64 8FBF001C */  lw         $ra, 0x1C($sp)
    /* DD29D4 00001F68 02001025 */  or         $v0, $s0, $zero
    /* DD29D8 00001F6C 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD29DC 00001F70 03E00008 */  jr         $ra
    /* DD29E0 00001F74 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel WorldRegistry_create_00001E08
