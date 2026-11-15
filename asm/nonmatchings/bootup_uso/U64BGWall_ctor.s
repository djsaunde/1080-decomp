nonmatching U64BGWall_ctor, 0xBDC

glabel U64BGWall_ctor
    /* DD18D4 00000E68 27BDFE28 */  addiu      $sp, $sp, -0x1D8
    /* DD18D8 00000E6C AFB20020 */  sw         $s2, 0x20($sp)
    /* DD18DC 00000E70 AFB1001C */  sw         $s1, 0x1C($sp)
    /* DD18E0 00000E74 AFB00018 */  sw         $s0, 0x18($sp)
    /* DD18E4 00000E78 00A08025 */  or         $s0, $a1, $zero
    /* DD18E8 00000E7C 00C08825 */  or         $s1, $a2, $zero
    /* DD18EC 00000E80 00E09025 */  or         $s2, $a3, $zero
    /* DD18F0 00000E84 AFBF003C */  sw         $ra, 0x3C($sp)
    /* DD18F4 00000E88 AFBE0038 */  sw         $fp, 0x38($sp)
    /* DD18F8 00000E8C AFB70034 */  sw         $s7, 0x34($sp)
    /* DD18FC 00000E90 AFB60030 */  sw         $s6, 0x30($sp)
    /* DD1900 00000E94 AFB5002C */  sw         $s5, 0x2C($sp)
    /* DD1904 00000E98 AFB40028 */  sw         $s4, 0x28($sp)
    /* DD1908 00000E9C AFB30024 */  sw         $s3, 0x24($sp)
    /* DD190C 00000EA0 14800005 */  bnez       $a0, .L00000EB8
    /* DD1910 00000EA4 AFA401D8 */   sw        $a0, 0x1D8($sp)
    /* DD1914 00000EA8 0C000000 */  jal        func_00000000
    /* DD1918 00000EAC 2404004C */   addiu     $a0, $zero, 0x4C
    /* DD191C 00000EB0 104002D7 */  beqz       $v0, .L00001A10
    /* DD1920 00000EB4 AFA201D8 */   sw        $v0, 0x1D8($sp)
  .L00000EB8:
    /* DD1924 00000EB8 3C050000 */  lui        $a1, %hi(sStr_Node_66B0)
    /* DD1928 00000EBC 24A566B0 */  addiu      $a1, $a1, %lo(sStr_Node_66B0)
    /* DD192C 00000EC0 0C000000 */  jal        func_00000000
    /* DD1930 00000EC4 8FA401D8 */   lw        $a0, 0x1D8($sp)
    /* DD1934 00000EC8 8FB801D8 */  lw         $t8, 0x1D8($sp)
    /* DD1938 00000ECC 3C0F0000 */  lui        $t7, (0x0 >> 16)
    /* DD193C 00000ED0 25EF0000 */  addiu      $t7, $t7, 0x0
    /* DD1940 00000ED4 AF0F0028 */  sw         $t7, 0x28($t8)
    /* DD1944 00000ED8 8FAE01D8 */  lw         $t6, 0x1D8($sp)
    /* DD1948 00000EDC 3C190000 */  lui        $t9, %hi(sStr_U64BGWall_66B8)
    /* DD194C 00000EE0 273966B8 */  addiu      $t9, $t9, %lo(sStr_U64BGWall_66B8)
    /* DD1950 00000EE4 ADD9000C */  sw         $t9, 0xC($t6)
    /* DD1954 00000EE8 8E0F0008 */  lw         $t7, 0x8($s0)
    /* DD1958 00000EEC 8FB901D8 */  lw         $t9, 0x1D8($sp)
    /* DD195C 00000EF0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* DD1960 00000EF4 AFAF01D4 */  sw         $t7, 0x1D4($sp)
    /* DD1964 00000EF8 8E18000C */  lw         $t8, 0xC($s0)
    /* DD1968 00000EFC 44810000 */  mtc1       $at, $f0
    /* DD196C 00000F00 24040800 */  addiu      $a0, $zero, 0x800
    /* DD1970 00000F04 AFB801D0 */  sw         $t8, 0x1D0($sp)
    /* DD1974 00000F08 AF31002C */  sw         $s1, 0x2C($t9)
    /* DD1978 00000F0C 8FAE01D8 */  lw         $t6, 0x1D8($sp)
    /* DD197C 00000F10 24050010 */  addiu      $a1, $zero, 0x10
    /* DD1980 00000F14 ADD20030 */  sw         $s2, 0x30($t6)
    /* DD1984 00000F18 8FB801D8 */  lw         $t8, 0x1D8($sp)
    /* DD1988 00000F1C 8FAF01E8 */  lw         $t7, 0x1E8($sp)
    /* DD198C 00000F20 AF0F0034 */  sw         $t7, 0x34($t8)
    /* DD1990 00000F24 8FB901D8 */  lw         $t9, 0x1D8($sp)
    /* DD1994 00000F28 E7200048 */  swc1       $f0, 0x48($t9)
    /* DD1998 00000F2C 8FAE01D8 */  lw         $t6, 0x1D8($sp)
    /* DD199C 00000F30 E5C00044 */  swc1       $f0, 0x44($t6)
    /* DD19A0 00000F34 8FAF01D8 */  lw         $t7, 0x1D8($sp)
    /* DD19A4 00000F38 E5E00040 */  swc1       $f0, 0x40($t7)
    /* DD19A8 00000F3C 8FB801D8 */  lw         $t8, 0x1D8($sp)
    /* DD19AC 00000F40 0C000000 */  jal        func_00000000
    /* DD19B0 00000F44 E700003C */   swc1      $f0, 0x3C($t8)
    /* DD19B4 00000F48 AFA201CC */  sw         $v0, 0x1CC($sp)
    /* DD19B8 00000F4C 00402025 */  or         $a0, $v0, $zero
    /* DD19BC 00000F50 24050800 */  addiu      $a1, $zero, 0x800
    /* DD19C0 00000F54 0C000000 */  jal        func_00000000
    /* DD19C4 00000F58 00003025 */   or        $a2, $zero, $zero
    /* DD19C8 00000F5C 3C070000 */  lui        $a3, %hi(func_00000000 + 0x4)
    /* DD19CC 00000F60 8CE70004 */  lw         $a3, %lo(func_00000000 + 0x4)($a3)
    /* DD19D0 00000F64 0000B025 */  or         $s6, $zero, $zero
    /* DD19D4 00000F68 00006025 */  or         $t4, $zero, $zero
    /* DD19D8 00000F6C 04E10003 */  bgez       $a3, .L00000F7C
    /* DD19DC 00000F70 00075843 */   sra       $t3, $a3, 1
    /* DD19E0 00000F74 24E10001 */  addiu      $at, $a3, 0x1
    /* DD19E4 00000F78 00015843 */  sra        $t3, $at, 1
  .L00000F7C:
    /* DD19E8 00000F7C 256BFFF0 */  addiu      $t3, $t3, -0x10
    /* DD19EC 00000F80 04E10003 */  bgez       $a3, .L00000F90
    /* DD19F0 00000F84 00071143 */   sra       $v0, $a3, 5
    /* DD19F4 00000F88 24E1001F */  addiu      $at, $a3, 0x1F
    /* DD19F8 00000F8C 00011143 */  sra        $v0, $at, 5
  .L00000F90:
    /* DD19FC 00000F90 18400057 */  blez       $v0, .L000010F0
    /* DD1A00 00000F94 0000A825 */   or        $s5, $zero, $zero
    /* DD1A04 00000F98 3C050000 */  lui        $a1, (0x0 >> 16)
    /* DD1A08 00000F9C 8CA50000 */  lw         $a1, 0x0($a1)
    /* DD1A0C 00000FA0 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD1A10 00000FA4 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD1A14 00000FA8 240A07C0 */  addiu      $t2, $zero, 0x7C0
    /* DD1A18 00000FAC 24090FC0 */  addiu      $t1, $zero, 0xFC0
    /* DD1A1C 00000FB0 04A10003 */  bgez       $a1, .L00000FC0
    /* DD1A20 00000FB4 00051983 */   sra       $v1, $a1, 6
    /* DD1A24 00000FB8 24A1003F */  addiu      $at, $a1, 0x3F
    /* DD1A28 00000FBC 00011983 */  sra        $v1, $at, 6
  .L00000FC0:
    /* DD1A2C 00000FC0 04A10003 */  bgez       $a1, .L00000FD0
    /* DD1A30 00000FC4 00052043 */   sra       $a0, $a1, 1
    /* DD1A34 00000FC8 24A10001 */  addiu      $at, $a1, 0x1
    /* DD1A38 00000FCC 00012043 */  sra        $a0, $at, 1
  .L00000FD0:
    /* DD1A3C 00000FD0 2484FFE0 */  addiu      $a0, $a0, -0x20
    /* DD1A40 00000FD4 18600042 */  blez       $v1, .L000010E0
    /* DD1A44 00000FD8 00009825 */   or        $s3, $zero, $zero
    /* DD1A48 00000FDC 8FB901CC */  lw         $t9, 0x1CC($sp)
    /* DD1A4C 00000FE0 00167100 */  sll        $t6, $s6, 4
    /* DD1A50 00000FE4 25670010 */  addiu      $a3, $t3, 0x10
    /* DD1A54 00000FE8 2568FFF0 */  addiu      $t0, $t3, -0x10
    /* DD1A58 00000FEC 032E1021 */  addu       $v0, $t9, $t6
    /* DD1A5C 00000FF0 24830020 */  addiu      $v1, $a0, 0x20
  .L00000FF4:
    /* DD1A60 00000FF4 A4430000 */  sh         $v1, 0x0($v0)
    /* DD1A64 00000FF8 A4470002 */  sh         $a3, 0x2($v0)
    /* DD1A68 00000FFC 8CCF0000 */  lw         $t7, 0x0($a2)
    /* DD1A6C 00001000 2485FFE0 */  addiu      $a1, $a0, -0x20
    /* DD1A70 00001004 A4400008 */  sh         $zero, 0x8($v0)
    /* DD1A74 00001008 05E10003 */  bgez       $t7, .L00001018
    /* DD1A78 0000100C 000FC043 */   sra       $t8, $t7, 1
    /* DD1A7C 00001010 25E10001 */  addiu      $at, $t7, 0x1
    /* DD1A80 00001014 0001C043 */  sra        $t8, $at, 1
  .L00001018:
    /* DD1A84 00001018 A4580004 */  sh         $t8, 0x4($v0)
    /* DD1A88 0000101C A440000A */  sh         $zero, 0xA($v0)
    /* DD1A8C 00001020 A4450010 */  sh         $a1, 0x10($v0)
    /* DD1A90 00001024 A4470012 */  sh         $a3, 0x12($v0)
    /* DD1A94 00001028 8CD90000 */  lw         $t9, 0x0($a2)
    /* DD1A98 0000102C A4490018 */  sh         $t1, 0x18($v0)
    /* DD1A9C 00001030 A440001A */  sh         $zero, 0x1A($v0)
    /* DD1AA0 00001034 07210003 */  bgez       $t9, .L00001044
    /* DD1AA4 00001038 00197043 */   sra       $t6, $t9, 1
    /* DD1AA8 0000103C 27210001 */  addiu      $at, $t9, 0x1
    /* DD1AAC 00001040 00017043 */  sra        $t6, $at, 1
  .L00001044:
    /* DD1AB0 00001044 A44E0014 */  sh         $t6, 0x14($v0)
    /* DD1AB4 00001048 A4450020 */  sh         $a1, 0x20($v0)
    /* DD1AB8 0000104C A4480022 */  sh         $t0, 0x22($v0)
    /* DD1ABC 00001050 8CCF0000 */  lw         $t7, 0x0($a2)
    /* DD1AC0 00001054 A4490028 */  sh         $t1, 0x28($v0)
    /* DD1AC4 00001058 A44A002A */  sh         $t2, 0x2A($v0)
    /* DD1AC8 0000105C 05E10003 */  bgez       $t7, .L0000106C
    /* DD1ACC 00001060 000FC043 */   sra       $t8, $t7, 1
    /* DD1AD0 00001064 25E10001 */  addiu      $at, $t7, 0x1
    /* DD1AD4 00001068 0001C043 */  sra        $t8, $at, 1
  .L0000106C:
    /* DD1AD8 0000106C A4580024 */  sh         $t8, 0x24($v0)
    /* DD1ADC 00001070 A4430030 */  sh         $v1, 0x30($v0)
    /* DD1AE0 00001074 A4480032 */  sh         $t0, 0x32($v0)
    /* DD1AE4 00001078 8CD90000 */  lw         $t9, 0x0($a2)
    /* DD1AE8 0000107C A4400038 */  sh         $zero, 0x38($v0)
    /* DD1AEC 00001080 A44A003A */  sh         $t2, 0x3A($v0)
    /* DD1AF0 00001084 07210003 */  bgez       $t9, .L00001094
    /* DD1AF4 00001088 00197043 */   sra       $t6, $t9, 1
    /* DD1AF8 0000108C 27210001 */  addiu      $at, $t9, 0x1
    /* DD1AFC 00001090 00017043 */  sra        $t6, $at, 1
  .L00001094:
    /* DD1B00 00001094 A44E0034 */  sh         $t6, 0x34($v0)
    /* DD1B04 00001098 8CC50000 */  lw         $a1, 0x0($a2)
    /* DD1B08 0000109C 26730001 */  addiu      $s3, $s3, 0x1
    /* DD1B0C 000010A0 2484FFC0 */  addiu      $a0, $a0, -0x40
    /* DD1B10 000010A4 26D60004 */  addiu      $s6, $s6, 0x4
    /* DD1B14 000010A8 24420040 */  addiu      $v0, $v0, 0x40
    /* DD1B18 000010AC 04A10003 */  bgez       $a1, .L000010BC
    /* DD1B1C 000010B0 00051983 */   sra       $v1, $a1, 6
    /* DD1B20 000010B4 24A1003F */  addiu      $at, $a1, 0x3F
    /* DD1B24 000010B8 00011983 */  sra        $v1, $at, 6
  .L000010BC:
    /* DD1B28 000010BC 0263082A */  slt        $at, $s3, $v1
    /* DD1B2C 000010C0 5420FFCC */  bnel       $at, $zero, .L00000FF4
    /* DD1B30 000010C4 24830020 */   addiu     $v1, $a0, 0x20
    /* DD1B34 000010C8 3C070000 */  lui        $a3, %hi(func_00000000 + 0x4)
    /* DD1B38 000010CC 8CE70004 */  lw         $a3, %lo(func_00000000 + 0x4)($a3)
    /* DD1B3C 000010D0 04E10003 */  bgez       $a3, .L000010E0
    /* DD1B40 000010D4 00071143 */   sra       $v0, $a3, 5
    /* DD1B44 000010D8 24E1001F */  addiu      $at, $a3, 0x1F
    /* DD1B48 000010DC 00011143 */  sra        $v0, $at, 5
  .L000010E0:
    /* DD1B4C 000010E0 258C0001 */  addiu      $t4, $t4, 0x1
    /* DD1B50 000010E4 0182082A */  slt        $at, $t4, $v0
    /* DD1B54 000010E8 1420FFB5 */  bnez       $at, .L00000FC0
    /* DD1B58 000010EC 256BFFE0 */   addiu     $t3, $t3, -0x20
  .L000010F0:
    /* DD1B5C 000010F0 3C080000 */  lui        $t0, %hi(func_000001FC + 0x18)
    /* DD1B60 000010F4 8D080214 */  lw         $t0, %lo(func_000001FC + 0x18)($t0)
    /* DD1B64 000010F8 3C060000 */  lui        $a2, (0x0 >> 16)
    /* DD1B68 000010FC 24C60000 */  addiu      $a2, $a2, 0x0
    /* DD1B6C 00001100 00004825 */  or         $t1, $zero, $zero
    /* DD1B70 00001104 AFA00124 */  sw         $zero, 0x124($sp)
    /* DD1B74 00001108 01001025 */  or         $v0, $t0, $zero
    /* DD1B78 0000110C 04E10003 */  bgez       $a3, .L0000111C
    /* DD1B7C 00001110 00077983 */   sra       $t7, $a3, 6
    /* DD1B80 00001114 24E1003F */  addiu      $at, $a3, 0x3F
    /* DD1B84 00001118 00017983 */  sra        $t7, $at, 6
  .L0000111C:
    /* DD1B88 0000111C 59E000F2 */  blezl      $t7, .L000014E8
    /* DD1B8C 00001120 8D03000C */   lw        $v1, 0xC($t0)
    /* DD1B90 00001124 8CC30000 */  lw         $v1, 0x0($a2)
    /* DD1B94 00001128 3C14BF00 */  lui        $s4, (0xBF000000 >> 16)
    /* DD1B98 0000112C 04610002 */  bgez       $v1, .L00001138
    /* DD1B9C 00001130 00600821 */   addu      $at, $v1, $zero
    /* DD1BA0 00001134 2461003F */  addiu      $at, $v1, 0x3F
  .L00001138:
    /* DD1BA4 00001138 00011983 */  sra        $v1, $at, 6
  .L0000113C:
    /* DD1BA8 0000113C 0000B025 */  or         $s6, $zero, $zero
    /* DD1BAC 00001140 00002025 */  or         $a0, $zero, $zero
  .L00001144:
    /* DD1BB0 00001144 00006025 */  or         $t4, $zero, $zero
    /* DD1BB4 00001148 186000D4 */  blez       $v1, .L0000149C
    /* DD1BB8 0000114C 00009825 */   or        $s3, $zero, $zero
    /* DD1BBC 00001150 0015C080 */  sll        $t8, $s5, 2
    /* DD1BC0 00001154 27B90130 */  addiu      $t9, $sp, 0x130
    /* DD1BC4 00001158 252E001F */  addiu      $t6, $t1, 0x1F
    /* DD1BC8 0000115C 000E7880 */  sll        $t7, $t6, 2
    /* DD1BCC 00001160 0319F821 */  addu       $ra, $t8, $t9
    /* DD1BD0 00001164 31F80FFF */  andi       $t8, $t7, 0xFFF
    /* DD1BD4 00001168 01297023 */  subu       $t6, $t1, $t1
    /* DD1BD8 0000116C 25CF001F */  addiu      $t7, $t6, 0x1F
    /* DD1BDC 00001170 AFB80054 */  sw         $t8, 0x54($sp)
    /* DD1BE0 00001174 2419003F */  addiu      $t9, $zero, 0x3F
    /* DD1BE4 00001178 00166840 */  sll        $t5, $s6, 1
    /* DD1BE8 0000117C AFB90050 */  sw         $t9, 0x50($sp)
    /* DD1BEC 00001180 000FC080 */  sll        $t8, $t7, 2
    /* DD1BF0 00001184 33190FFF */  andi       $t9, $t8, 0xFFF
    /* DD1BF4 00001188 0009F080 */  sll        $fp, $t1, 2
    /* DD1BF8 0000118C 33DE0FFF */  andi       $fp, $fp, 0xFFF
    /* DD1BFC 00001190 AFB9004C */  sw         $t9, 0x4C($sp)
    /* DD1C00 00001194 25B00006 */  addiu      $s0, $t5, 0x6
    /* DD1C04 00001198 25B10002 */  addiu      $s1, $t5, 0x2
    /* DD1C08 0000119C 25B20004 */  addiu      $s2, $t5, 0x4
    /* DD1C0C 000011A0 AFA901B0 */  sw         $t1, 0x1B0($sp)
    /* DD1C10 000011A4 AFA401C4 */  sw         $a0, 0x1C4($sp)
    /* DD1C14 000011A8 24170047 */  addiu      $s7, $zero, 0x47
    /* DD1C18 000011AC 8C43000C */  lw         $v1, 0xC($v0)
  .L000011B0:
    /* DD1C1C 000011B0 3C01BE00 */  lui        $at, (0xBE000000 >> 16)
    /* DD1C20 000011B4 001748C3 */  sra        $t1, $s7, 3
    /* DD1C24 000011B8 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1C28 000011BC 312901FF */  andi       $t1, $t1, 0x1FF
    /* DD1C2C 000011C0 00094A40 */  sll        $t1, $t1, 9
    /* DD1C30 000011C4 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD1C34 000011C8 AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD1C38 000011CC 8C4F000C */  lw         $t7, 0xC($v0)
    /* DD1C3C 000011D0 0005C8C0 */  sll        $t9, $a1, 3
    /* DD1C40 000011D4 31AEFFFF */  andi       $t6, $t5, 0xFFFF
    /* DD1C44 000011D8 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD1C48 000011DC 01C17825 */  or         $t7, $t6, $at
    /* DD1C4C 000011E0 3C01FD48 */  lui        $at, (0xFD480000 >> 16)
    /* DD1C50 000011E4 03193021 */  addu       $a2, $t8, $t9
    /* DD1C54 000011E8 AFE60000 */  sw         $a2, 0x0($ra)
    /* DD1C58 000011EC 3218FFFF */  andi       $t8, $s0, 0xFFFF
    /* DD1C5C 000011F0 ACD80004 */  sw         $t8, 0x4($a2)
    /* DD1C60 000011F4 ACCF0000 */  sw         $t7, 0x0($a2)
    /* DD1C64 000011F8 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1C68 000011FC 26730001 */  addiu      $s3, $s3, 0x1
    /* DD1C6C 00001200 01A03825 */  or         $a3, $t5, $zero
    /* DD1C70 00001204 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1C74 00001208 02004025 */  or         $t0, $s0, $zero
    /* DD1C78 0000120C 26B50001 */  addiu      $s5, $s5, 0x1
    /* DD1C7C 00001210 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD1C80 00001214 AC790004 */  sw         $t9, 0x4($v1)
    /* DD1C84 00001218 8C4E000C */  lw         $t6, 0xC($v0)
    /* DD1C88 0000121C 3C190000 */  lui        $t9, (0x0 >> 16)
    /* DD1C8C 00001220 8F390000 */  lw         $t9, 0x0($t9)
    /* DD1C90 00001224 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD1C94 00001228 0005C0C0 */  sll        $t8, $a1, 3
    /* DD1C98 0000122C 272EFFFF */  addiu      $t6, $t9, -0x1
    /* DD1C9C 00001230 01F82021 */  addu       $a0, $t7, $t8
    /* DD1CA0 00001234 31CF0FFF */  andi       $t7, $t6, 0xFFF
    /* DD1CA4 00001238 01E1C025 */  or         $t8, $t7, $at
    /* DD1CA8 0000123C AC980000 */  sw         $t8, 0x0($a0)
    /* DD1CAC 00001240 8FB901D4 */  lw         $t9, 0x1D4($sp)
    /* DD1CB0 00001244 3C01F548 */  lui        $at, (0xF5480000 >> 16)
    /* DD1CB4 00001248 01214825 */  or         $t1, $t1, $at
    /* DD1CB8 0000124C AC990004 */  sw         $t9, 0x4($a0)
    /* DD1CBC 00001250 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1CC0 00001254 3C01F400 */  lui        $at, (0xF4000000 >> 16)
    /* DD1CC4 00001258 27FF0004 */  addiu      $ra, $ra, 0x4
    /* DD1CC8 0000125C 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1CCC 00001260 26D60004 */  addiu      $s6, $s6, 0x4
    /* DD1CD0 00001264 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD1CD4 00001268 AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD1CD8 0000126C 8C4F000C */  lw         $t7, 0xC($v0)
    /* DD1CDC 00001270 0005C8C0 */  sll        $t9, $a1, 3
    /* DD1CE0 00001274 3C0E0701 */  lui        $t6, (0x7014060 >> 16)
    /* DD1CE4 00001278 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD1CE8 0000127C 35CE4060 */  ori        $t6, $t6, (0x7014060 & 0xFFFF)
    /* DD1CEC 00001280 03192021 */  addu       $a0, $t8, $t9
    /* DD1CF0 00001284 AC890000 */  sw         $t1, 0x0($a0)
    /* DD1CF4 00001288 AC8E0004 */  sw         $t6, 0x4($a0)
    /* DD1CF8 0000128C 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1CFC 00001290 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1D00 00001294 24AF0001 */  addiu      $t7, $a1, 0x1
    /* DD1D04 00001298 AC6F0004 */  sw         $t7, 0x4($v1)
    /* DD1D08 0000129C 8C58000C */  lw         $t8, 0xC($v0)
    /* DD1D0C 000012A0 000570C0 */  sll        $t6, $a1, 3
    /* DD1D10 000012A4 3C0FE600 */  lui        $t7, (0xE6000000 >> 16)
    /* DD1D14 000012A8 8F190000 */  lw         $t9, 0x0($t8)
    /* DD1D18 000012AC 032E3021 */  addu       $a2, $t9, $t6
    /* DD1D1C 000012B0 ACCF0000 */  sw         $t7, 0x0($a2)
    /* DD1D20 000012B4 ACC00004 */  sw         $zero, 0x4($a2)
    /* DD1D24 000012B8 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1D28 000012BC 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1D2C 000012C0 24B80001 */  addiu      $t8, $a1, 0x1
    /* DD1D30 000012C4 AC780004 */  sw         $t8, 0x4($v1)
    /* DD1D34 000012C8 8C59000C */  lw         $t9, 0xC($v0)
    /* DD1D38 000012CC 000CC080 */  sll        $t8, $t4, 2
    /* DD1D3C 000012D0 000578C0 */  sll        $t7, $a1, 3
    /* DD1D40 000012D4 8F2E0000 */  lw         $t6, 0x0($t9)
    /* DD1D44 000012D8 33190FFF */  andi       $t9, $t8, 0xFFF
    /* DD1D48 000012DC 01CF5021 */  addu       $t2, $t6, $t7
    /* DD1D4C 000012E0 00197300 */  sll        $t6, $t9, 12
    /* DD1D50 000012E4 01C17825 */  or         $t7, $t6, $at
    /* DD1D54 000012E8 01FEC025 */  or         $t8, $t7, $fp
    /* DD1D58 000012EC 2599003F */  addiu      $t9, $t4, 0x3F
    /* DD1D5C 000012F0 00197080 */  sll        $t6, $t9, 2
    /* DD1D60 000012F4 31CF0FFF */  andi       $t7, $t6, 0xFFF
    /* DD1D64 000012F8 AD580000 */  sw         $t8, 0x0($t2)
    /* DD1D68 000012FC 8FAE0054 */  lw         $t6, 0x54($sp)
    /* DD1D6C 00001300 000FC300 */  sll        $t8, $t7, 12
    /* DD1D70 00001304 3C010700 */  lui        $at, (0x7000000 >> 16)
    /* DD1D74 00001308 0301C825 */  or         $t9, $t8, $at
    /* DD1D78 0000130C 032E7825 */  or         $t7, $t9, $t6
    /* DD1D7C 00001310 AD4F0004 */  sw         $t7, 0x4($t2)
    /* DD1D80 00001314 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1D84 00001318 324A00FF */  andi       $t2, $s2, 0xFF
    /* DD1D88 0000131C 258C0040 */  addiu      $t4, $t4, 0x40
    /* DD1D8C 00001320 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1D90 00001324 26520008 */  addiu      $s2, $s2, 0x8
    /* DD1D94 00001328 24B80001 */  addiu      $t8, $a1, 0x1
    /* DD1D98 0000132C AC780004 */  sw         $t8, 0x4($v1)
    /* DD1D9C 00001330 8C59000C */  lw         $t9, 0xC($v0)
    /* DD1DA0 00001334 000578C0 */  sll        $t7, $a1, 3
    /* DD1DA4 00001338 3C18E700 */  lui        $t8, (0xE7000000 >> 16)
    /* DD1DA8 0000133C 8F2E0000 */  lw         $t6, 0x0($t9)
    /* DD1DAC 00001340 01CF2021 */  addu       $a0, $t6, $t7
    /* DD1DB0 00001344 AC980000 */  sw         $t8, 0x0($a0)
    /* DD1DB4 00001348 AC800004 */  sw         $zero, 0x4($a0)
    /* DD1DB8 0000134C 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1DBC 00001350 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1DC0 00001354 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD1DC4 00001358 AC790004 */  sw         $t9, 0x4($v1)
    /* DD1DC8 0000135C 8C4E000C */  lw         $t6, 0xC($v0)
    /* DD1DCC 00001360 0005C0C0 */  sll        $t8, $a1, 3
    /* DD1DD0 00001364 3C190001 */  lui        $t9, (0x14060 >> 16)
    /* DD1DD4 00001368 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD1DD8 0000136C 37394060 */  ori        $t9, $t9, (0x14060 & 0xFFFF)
    /* DD1DDC 00001370 01F85821 */  addu       $t3, $t7, $t8
    /* DD1DE0 00001374 AD690000 */  sw         $t1, 0x0($t3)
    /* DD1DE4 00001378 AD790004 */  sw         $t9, 0x4($t3)
    /* DD1DE8 0000137C 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1DEC 00001380 31A900FF */  andi       $t1, $t5, 0xFF
    /* DD1DF0 00001384 00094C00 */  sll        $t1, $t1, 16
    /* DD1DF4 00001388 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1DF8 0000138C 25AD0008 */  addiu      $t5, $t5, 0x8
    /* DD1DFC 00001390 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD1E00 00001394 AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD1E04 00001398 8C4F000C */  lw         $t7, 0xC($v0)
    /* DD1E08 0000139C 0005C8C0 */  sll        $t9, $a1, 3
    /* DD1E0C 000013A0 3C0EF200 */  lui        $t6, (0xF2000000 >> 16)
    /* DD1E10 000013A4 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD1E14 000013A8 03192021 */  addu       $a0, $t8, $t9
    /* DD1E18 000013AC AC8E0000 */  sw         $t6, 0x0($a0)
    /* DD1E1C 000013B0 8FAF0050 */  lw         $t7, 0x50($sp)
    /* DD1E20 000013B4 000FC080 */  sll        $t8, $t7, 2
    /* DD1E24 000013B8 8FAF004C */  lw         $t7, 0x4C($sp)
    /* DD1E28 000013BC 33190FFF */  andi       $t9, $t8, 0xFFF
    /* DD1E2C 000013C0 00197300 */  sll        $t6, $t9, 12
    /* DD1E30 000013C4 01CFC025 */  or         $t8, $t6, $t7
    /* DD1E34 000013C8 AC980004 */  sw         $t8, 0x4($a0)
    /* DD1E38 000013CC 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1E3C 000013D0 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1E40 000013D4 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD1E44 000013D8 AC790004 */  sw         $t9, 0x4($v1)
    /* DD1E48 000013DC 8C4E000C */  lw         $t6, 0xC($v0)
    /* DD1E4C 000013E0 0005C0C0 */  sll        $t8, $a1, 3
    /* DD1E50 000013E4 323900FF */  andi       $t9, $s1, 0xFF
    /* DD1E54 000013E8 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD1E58 000013EC 00197200 */  sll        $t6, $t9, 8
    /* DD1E5C 000013F0 26310008 */  addiu      $s1, $s1, 0x8
    /* DD1E60 000013F4 01F83021 */  addu       $a2, $t7, $t8
    /* DD1E64 000013F8 012E7825 */  or         $t7, $t1, $t6
    /* DD1E68 000013FC 01EAC025 */  or         $t8, $t7, $t2
    /* DD1E6C 00001400 ACD80004 */  sw         $t8, 0x4($a2)
    /* DD1E70 00001404 ACD40000 */  sw         $s4, 0x0($a2)
    /* DD1E74 00001408 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1E78 0000140C 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1E7C 00001410 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD1E80 00001414 AC790004 */  sw         $t9, 0x4($v1)
    /* DD1E84 00001418 8C4E000C */  lw         $t6, 0xC($v0)
    /* DD1E88 0000141C 0005C0C0 */  sll        $t8, $a1, 3
    /* DD1E8C 00001420 000ACA00 */  sll        $t9, $t2, 8
    /* DD1E90 00001424 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD1E94 00001428 01397025 */  or         $t6, $t1, $t9
    /* DD1E98 0000142C 01F82021 */  addu       $a0, $t7, $t8
    /* DD1E9C 00001430 320F00FF */  andi       $t7, $s0, 0xFF
    /* DD1EA0 00001434 01CFC025 */  or         $t8, $t6, $t7
    /* DD1EA4 00001438 AC980004 */  sw         $t8, 0x4($a0)
    /* DD1EA8 0000143C AC940000 */  sw         $s4, 0x0($a0)
    /* DD1EAC 00001440 8C43000C */  lw         $v1, 0xC($v0)
    /* DD1EB0 00001444 26100008 */  addiu      $s0, $s0, 0x8
    /* DD1EB4 00001448 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1EB8 0000144C 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD1EBC 00001450 AC790004 */  sw         $t9, 0x4($v1)
    /* DD1EC0 00001454 8C4E000C */  lw         $t6, 0xC($v0)
    /* DD1EC4 00001458 0005C0C0 */  sll        $t8, $a1, 3
    /* DD1EC8 0000145C 3C19B800 */  lui        $t9, (0xB8000000 >> 16)
    /* DD1ECC 00001460 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD1ED0 00001464 3C030000 */  lui        $v1, (0x0 >> 16)
    /* DD1ED4 00001468 01F83021 */  addu       $a2, $t7, $t8
    /* DD1ED8 0000146C ACD90000 */  sw         $t9, 0x0($a2)
    /* DD1EDC 00001470 ACC00004 */  sw         $zero, 0x4($a2)
    /* DD1EE0 00001474 8C630000 */  lw         $v1, 0x0($v1)
    /* DD1EE4 00001478 04610002 */  bgez       $v1, .L00001484
    /* DD1EE8 0000147C 00600821 */   addu      $at, $v1, $zero
    /* DD1EEC 00001480 2461003F */  addiu      $at, $v1, 0x3F
  .L00001484:
    /* DD1EF0 00001484 00011983 */  sra        $v1, $at, 6
    /* DD1EF4 00001488 0263082A */  slt        $at, $s3, $v1
    /* DD1EF8 0000148C 5420FF48 */  bnel       $at, $zero, .L000011B0
    /* DD1EFC 00001490 8C43000C */   lw        $v1, 0xC($v0)
    /* DD1F00 00001494 8FA901B0 */  lw         $t1, 0x1B0($sp)
    /* DD1F04 00001498 8FA401C4 */  lw         $a0, 0x1C4($sp)
  .L0000149C:
    /* DD1F08 0000149C 24840001 */  addiu      $a0, $a0, 0x1
    /* DD1F0C 000014A0 24010002 */  addiu      $at, $zero, 0x2
    /* DD1F10 000014A4 1481FF27 */  bne        $a0, $at, .L00001144
    /* DD1F14 000014A8 25290020 */   addiu     $t1, $t1, 0x20
    /* DD1F18 000014AC 3C040000 */  lui        $a0, %hi(func_00000000 + 0x4)
    /* DD1F1C 000014B0 8C840004 */  lw         $a0, %lo(func_00000000 + 0x4)($a0)
    /* DD1F20 000014B4 8FA50124 */  lw         $a1, 0x124($sp)
    /* DD1F24 000014B8 04810002 */  bgez       $a0, .L000014C4
    /* DD1F28 000014BC 00800821 */   addu      $at, $a0, $zero
    /* DD1F2C 000014C0 2481003F */  addiu      $at, $a0, 0x3F
  .L000014C4:
    /* DD1F30 000014C4 00012183 */  sra        $a0, $at, 6
    /* DD1F34 000014C8 24A50001 */  addiu      $a1, $a1, 0x1
    /* DD1F38 000014CC 00A4082A */  slt        $at, $a1, $a0
    /* DD1F3C 000014D0 1420FF1A */  bnez       $at, .L0000113C
    /* DD1F40 000014D4 AFA50124 */   sw        $a1, 0x124($sp)
    /* DD1F44 000014D8 3C080000 */  lui        $t0, %hi(func_000001FC + 0x18)
    /* DD1F48 000014DC 8D080214 */  lw         $t0, %lo(func_000001FC + 0x18)($t0)
    /* DD1F4C 000014E0 AFA00124 */  sw         $zero, 0x124($sp)
    /* DD1F50 000014E4 8D03000C */  lw         $v1, 0xC($t0)
  .L000014E8:
    /* DD1F54 000014E8 3C040000 */  lui        $a0, (0x0 >> 16)
    /* DD1F58 000014EC 01008825 */  or         $s1, $t0, $zero
    /* DD1F5C 000014F0 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1F60 000014F4 24840000 */  addiu      $a0, $a0, 0x0
    /* DD1F64 000014F8 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD1F68 000014FC AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD1F6C 00001500 8D0F000C */  lw         $t7, 0xC($t0)
    /* DD1F70 00001504 3C0E0400 */  lui        $t6, (0x400103F >> 16)
    /* DD1F74 00001508 0005C8C0 */  sll        $t9, $a1, 3
    /* DD1F78 0000150C 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD1F7C 00001510 35CE103F */  ori        $t6, $t6, (0x400103F & 0xFFFF)
    /* DD1F80 00001514 03191021 */  addu       $v0, $t8, $t9
    /* DD1F84 00001518 AC4E0000 */  sw         $t6, 0x0($v0)
    /* DD1F88 0000151C 0C000000 */  jal        func_00000000
    /* DD1F8C 00001520 AFA20120 */   sw        $v0, 0x120($sp)
    /* DD1F90 00001524 8FAF0120 */  lw         $t7, 0x120($sp)
    /* DD1F94 00001528 3C07E700 */  lui        $a3, (0xE7000000 >> 16)
    /* DD1F98 0000152C 3C130000 */  lui        $s3, (0x0 >> 16)
    /* DD1F9C 00001530 ADE20004 */  sw         $v0, 0x4($t7)
    /* DD1FA0 00001534 8E23000C */  lw         $v1, 0xC($s1)
    /* DD1FA4 00001538 3C1E0000 */  lui        $fp, %hi(func_00000000 + 0x4)
    /* DD1FA8 0000153C 27DE0004 */  addiu      $fp, $fp, %lo(func_00000000 + 0x4)
    /* DD1FAC 00001540 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1FB0 00001544 3C120600 */  lui        $s2, (0x6000000 >> 16)
    /* DD1FB4 00001548 24B80001 */  addiu      $t8, $a1, 0x1
    /* DD1FB8 0000154C AC780004 */  sw         $t8, 0x4($v1)
    /* DD1FBC 00001550 8E39000C */  lw         $t9, 0xC($s1)
    /* DD1FC0 00001554 000578C0 */  sll        $t7, $a1, 3
    /* DD1FC4 00001558 3C18BE00 */  lui        $t8, (0xBE000000 >> 16)
    /* DD1FC8 0000155C 8F2E0000 */  lw         $t6, 0x0($t9)
    /* DD1FCC 00001560 24190006 */  addiu      $t9, $zero, 0x6
    /* DD1FD0 00001564 01CF2021 */  addu       $a0, $t6, $t7
    /* DD1FD4 00001568 AC980000 */  sw         $t8, 0x0($a0)
    /* DD1FD8 0000156C AC990004 */  sw         $t9, 0x4($a0)
    /* DD1FDC 00001570 8E23000C */  lw         $v1, 0xC($s1)
    /* DD1FE0 00001574 8C650004 */  lw         $a1, 0x4($v1)
    /* DD1FE4 00001578 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD1FE8 0000157C AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD1FEC 00001580 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD1FF0 00001584 0005C8C0 */  sll        $t9, $a1, 3
    /* DD1FF4 00001588 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD1FF8 0000158C 03193021 */  addu       $a2, $t8, $t9
    /* DD1FFC 00001590 ACC70000 */  sw         $a3, 0x0($a2)
    /* DD2000 00001594 ACC00004 */  sw         $zero, 0x4($a2)
    /* DD2004 00001598 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2008 0000159C 8C650004 */  lw         $a1, 0x4($v1)
    /* DD200C 000015A0 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD2010 000015A4 AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD2014 000015A8 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD2018 000015AC 0005C8C0 */  sll        $t9, $a1, 3
    /* DD201C 000015B0 3C0EBA00 */  lui        $t6, (0xBA001402 >> 16)
    /* DD2020 000015B4 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD2024 000015B8 35CE1402 */  ori        $t6, $t6, (0xBA001402 & 0xFFFF)
    /* DD2028 000015BC 03191021 */  addu       $v0, $t8, $t9
    /* DD202C 000015C0 AC4E0000 */  sw         $t6, 0x0($v0)
    /* DD2030 000015C4 AC400004 */  sw         $zero, 0x4($v0)
    /* DD2034 000015C8 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2038 000015CC 8C650004 */  lw         $a1, 0x4($v1)
    /* DD203C 000015D0 24AF0001 */  addiu      $t7, $a1, 0x1
    /* DD2040 000015D4 AC6F0004 */  sw         $t7, 0x4($v1)
    /* DD2044 000015D8 8E38000C */  lw         $t8, 0xC($s1)
    /* DD2048 000015DC 000570C0 */  sll        $t6, $a1, 3
    /* DD204C 000015E0 3C0FBB00 */  lui        $t7, (0xBB000001 >> 16)
    /* DD2050 000015E4 8F190000 */  lw         $t9, 0x0($t8)
    /* DD2054 000015E8 3C188000 */  lui        $t8, (0x80008000 >> 16)
    /* DD2058 000015EC 37188000 */  ori        $t8, $t8, (0x80008000 & 0xFFFF)
    /* DD205C 000015F0 032E2021 */  addu       $a0, $t9, $t6
    /* DD2060 000015F4 35EF0001 */  ori        $t7, $t7, (0xBB000001 & 0xFFFF)
    /* DD2064 000015F8 AC8F0000 */  sw         $t7, 0x0($a0)
    /* DD2068 000015FC AC980004 */  sw         $t8, 0x4($a0)
    /* DD206C 00001600 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2070 00001604 8C650004 */  lw         $a1, 0x4($v1)
    /* DD2074 00001608 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD2078 0000160C AC790004 */  sw         $t9, 0x4($v1)
    /* DD207C 00001610 8E2E000C */  lw         $t6, 0xC($s1)
    /* DD2080 00001614 0005C0C0 */  sll        $t8, $a1, 3
    /* DD2084 00001618 3C19B600 */  lui        $t9, (0xB6000000 >> 16)
    /* DD2088 0000161C 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD208C 00001620 3C0E0003 */  lui        $t6, (0x33205 >> 16)
    /* DD2090 00001624 35CE3205 */  ori        $t6, $t6, (0x33205 & 0xFFFF)
    /* DD2094 00001628 01F83021 */  addu       $a2, $t7, $t8
    /* DD2098 0000162C ACD90000 */  sw         $t9, 0x0($a2)
    /* DD209C 00001630 ACCE0004 */  sw         $t6, 0x4($a2)
    /* DD20A0 00001634 8E23000C */  lw         $v1, 0xC($s1)
    /* DD20A4 00001638 8C650004 */  lw         $a1, 0x4($v1)
    /* DD20A8 0000163C 24AF0001 */  addiu      $t7, $a1, 0x1
    /* DD20AC 00001640 AC6F0004 */  sw         $t7, 0x4($v1)
    /* DD20B0 00001644 8E38000C */  lw         $t8, 0xC($s1)
    /* DD20B4 00001648 000570C0 */  sll        $t6, $a1, 3
    /* DD20B8 0000164C 3C0FB900 */  lui        $t7, (0xB900031D >> 16)
    /* DD20BC 00001650 8F190000 */  lw         $t9, 0x0($t8)
    /* DD20C0 00001654 3C180C08 */  lui        $t8, (0xC084000 >> 16)
    /* DD20C4 00001658 37184000 */  ori        $t8, $t8, (0xC084000 & 0xFFFF)
    /* DD20C8 0000165C 032E1021 */  addu       $v0, $t9, $t6
    /* DD20CC 00001660 35EF031D */  ori        $t7, $t7, (0xB900031D & 0xFFFF)
    /* DD20D0 00001664 AC4F0000 */  sw         $t7, 0x0($v0)
    /* DD20D4 00001668 AC580004 */  sw         $t8, 0x4($v0)
    /* DD20D8 0000166C 8E23000C */  lw         $v1, 0xC($s1)
    /* DD20DC 00001670 8C650004 */  lw         $a1, 0x4($v1)
    /* DD20E0 00001674 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD20E4 00001678 AC790004 */  sw         $t9, 0x4($v1)
    /* DD20E8 0000167C 8E2E000C */  lw         $t6, 0xC($s1)
    /* DD20EC 00001680 0005C0C0 */  sll        $t8, $a1, 3
    /* DD20F0 00001684 3C19FC35 */  lui        $t9, (0xFC357E6A >> 16)
    /* DD20F4 00001688 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD20F8 0000168C 3C0E11FC */  lui        $t6, (0x11FCF67B >> 16)
    /* DD20FC 00001690 35CEF67B */  ori        $t6, $t6, (0x11FCF67B & 0xFFFF)
    /* DD2100 00001694 01F82021 */  addu       $a0, $t7, $t8
    /* DD2104 00001698 37397E6A */  ori        $t9, $t9, (0xFC357E6A & 0xFFFF)
    /* DD2108 0000169C AC990000 */  sw         $t9, 0x0($a0)
    /* DD210C 000016A0 AC8E0004 */  sw         $t6, 0x4($a0)
    /* DD2110 000016A4 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2114 000016A8 8C650004 */  lw         $a1, 0x4($v1)
    /* DD2118 000016AC 24AF0001 */  addiu      $t7, $a1, 0x1
    /* DD211C 000016B0 AC6F0004 */  sw         $t7, 0x4($v1)
    /* DD2120 000016B4 8E38000C */  lw         $t8, 0xC($s1)
    /* DD2124 000016B8 000570C0 */  sll        $t6, $a1, 3
    /* DD2128 000016BC 3C0FBA00 */  lui        $t7, (0xBA000E02 >> 16)
    /* DD212C 000016C0 8F190000 */  lw         $t9, 0x0($t8)
    /* DD2130 000016C4 34188000 */  ori        $t8, $zero, 0x8000
    /* DD2134 000016C8 35EF0E02 */  ori        $t7, $t7, (0xBA000E02 & 0xFFFF)
    /* DD2138 000016CC 032E3021 */  addu       $a2, $t9, $t6
    /* DD213C 000016D0 ACCF0000 */  sw         $t7, 0x0($a2)
    /* DD2140 000016D4 ACD80004 */  sw         $t8, 0x4($a2)
    /* DD2144 000016D8 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2148 000016DC 8C650004 */  lw         $a1, 0x4($v1)
    /* DD214C 000016E0 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD2150 000016E4 AC790004 */  sw         $t9, 0x4($v1)
    /* DD2154 000016E8 8E2E000C */  lw         $t6, 0xC($s1)
    /* DD2158 000016EC 0005C0C0 */  sll        $t8, $a1, 3
    /* DD215C 000016F0 3C19FD10 */  lui        $t9, (0xFD100000 >> 16)
    /* DD2160 000016F4 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD2164 000016F8 01F81021 */  addu       $v0, $t7, $t8
    /* DD2168 000016FC AC590000 */  sw         $t9, 0x0($v0)
    /* DD216C 00001700 8FAE01D0 */  lw         $t6, 0x1D0($sp)
    /* DD2170 00001704 AC4E0004 */  sw         $t6, 0x4($v0)
    /* DD2174 00001708 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2178 0000170C 8C650004 */  lw         $a1, 0x4($v1)
    /* DD217C 00001710 24AF0001 */  addiu      $t7, $a1, 0x1
    /* DD2180 00001714 AC6F0004 */  sw         $t7, 0x4($v1)
    /* DD2184 00001718 8E38000C */  lw         $t8, 0xC($s1)
    /* DD2188 0000171C 000570C0 */  sll        $t6, $a1, 3
    /* DD218C 00001720 3C0FE800 */  lui        $t7, (0xE8000000 >> 16)
    /* DD2190 00001724 8F190000 */  lw         $t9, 0x0($t8)
    /* DD2194 00001728 032E2021 */  addu       $a0, $t9, $t6
    /* DD2198 0000172C AC8F0000 */  sw         $t7, 0x0($a0)
    /* DD219C 00001730 AC800004 */  sw         $zero, 0x4($a0)
    /* DD21A0 00001734 8E23000C */  lw         $v1, 0xC($s1)
    /* DD21A4 00001738 8C650004 */  lw         $a1, 0x4($v1)
    /* DD21A8 0000173C 24B80001 */  addiu      $t8, $a1, 0x1
    /* DD21AC 00001740 AC780004 */  sw         $t8, 0x4($v1)
    /* DD21B0 00001744 8E39000C */  lw         $t9, 0xC($s1)
    /* DD21B4 00001748 000578C0 */  sll        $t7, $a1, 3
    /* DD21B8 0000174C 3C18F500 */  lui        $t8, (0xF5000100 >> 16)
    /* DD21BC 00001750 8F2E0000 */  lw         $t6, 0x0($t9)
    /* DD21C0 00001754 3C190700 */  lui        $t9, (0x7000000 >> 16)
    /* DD21C4 00001758 37180100 */  ori        $t8, $t8, (0xF5000100 & 0xFFFF)
    /* DD21C8 0000175C 01CF3021 */  addu       $a2, $t6, $t7
    /* DD21CC 00001760 ACD80000 */  sw         $t8, 0x0($a2)
    /* DD21D0 00001764 ACD90004 */  sw         $t9, 0x4($a2)
    /* DD21D4 00001768 8E23000C */  lw         $v1, 0xC($s1)
    /* DD21D8 0000176C 8C650004 */  lw         $a1, 0x4($v1)
    /* DD21DC 00001770 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD21E0 00001774 AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD21E4 00001778 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD21E8 0000177C 0005C8C0 */  sll        $t9, $a1, 3
    /* DD21EC 00001780 3C0EE600 */  lui        $t6, (0xE6000000 >> 16)
    /* DD21F0 00001784 8DF80000 */  lw         $t8, 0x0($t7)
    /* DD21F4 00001788 03191021 */  addu       $v0, $t8, $t9
    /* DD21F8 0000178C AC4E0000 */  sw         $t6, 0x0($v0)
    /* DD21FC 00001790 AC400004 */  sw         $zero, 0x4($v0)
    /* DD2200 00001794 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2204 00001798 8C650004 */  lw         $a1, 0x4($v1)
    /* DD2208 0000179C 24AF0001 */  addiu      $t7, $a1, 0x1
    /* DD220C 000017A0 AC6F0004 */  sw         $t7, 0x4($v1)
    /* DD2210 000017A4 8E38000C */  lw         $t8, 0xC($s1)
    /* DD2214 000017A8 000570C0 */  sll        $t6, $a1, 3
    /* DD2218 000017AC 3C0FF000 */  lui        $t7, (0xF0000000 >> 16)
    /* DD221C 000017B0 8F190000 */  lw         $t9, 0x0($t8)
    /* DD2220 000017B4 3C18073F */  lui        $t8, (0x73FC000 >> 16)
    /* DD2224 000017B8 3718C000 */  ori        $t8, $t8, (0x73FC000 & 0xFFFF)
    /* DD2228 000017BC 032E2021 */  addu       $a0, $t9, $t6
    /* DD222C 000017C0 AC8F0000 */  sw         $t7, 0x0($a0)
    /* DD2230 000017C4 AC980004 */  sw         $t8, 0x4($a0)
    /* DD2234 000017C8 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2238 000017CC 8C650004 */  lw         $a1, 0x4($v1)
    /* DD223C 000017D0 24B90001 */  addiu      $t9, $a1, 0x1
    /* DD2240 000017D4 AC790004 */  sw         $t9, 0x4($v1)
    /* DD2244 000017D8 8E2E000C */  lw         $t6, 0xC($s1)
    /* DD2248 000017DC 0005C0C0 */  sll        $t8, $a1, 3
    /* DD224C 000017E0 8DCF0000 */  lw         $t7, 0x0($t6)
    /* DD2250 000017E4 01F81021 */  addu       $v0, $t7, $t8
    /* DD2254 000017E8 AC470000 */  sw         $a3, 0x0($v0)
    /* DD2258 000017EC AC400004 */  sw         $zero, 0x4($v0)
    /* DD225C 000017F0 8E730000 */  lw         $s3, 0x0($s3)
    /* DD2260 000017F4 8FD90000 */  lw         $t9, 0x0($fp)
    /* DD2264 000017F8 8FB501CC */  lw         $s5, 0x1CC($sp)
    /* DD2268 000017FC 06610002 */  bgez       $s3, .L00001808
    /* DD226C 00001800 02600821 */   addu      $at, $s3, $zero
    /* DD2270 00001804 2661001F */  addiu      $at, $s3, 0x1F
  .L00001808:
    /* DD2274 00001808 00019943 */  sra        $s3, $at, 5
    /* DD2278 0000180C 07210003 */  bgez       $t9, .L0000181C
    /* DD227C 00001810 00197183 */   sra       $t6, $t9, 6
    /* DD2280 00001814 2721003F */  addiu      $at, $t9, 0x3F
    /* DD2284 00001818 00017183 */  sra        $t6, $at, 6
  .L0000181C:
    /* DD2288 0000181C 19C0006E */  blez       $t6, .L000019D8
    /* DD228C 00001820 00131080 */   sll       $v0, $s3, 2
    /* DD2290 00001824 0002C100 */  sll        $t8, $v0, 4
    /* DD2294 00001828 2719FFFF */  addiu      $t9, $t8, -0x1
    /* DD2298 0000182C 00027A80 */  sll        $t7, $v0, 10
    /* DD229C 00001830 01F9B825 */  or         $s7, $t7, $t9
    /* DD22A0 00001834 32F7FFFF */  andi       $s7, $s7, 0xFFFF
    /* DD22A4 00001838 3C010400 */  lui        $at, (0x4000000 >> 16)
    /* DD22A8 0000183C 02E1B825 */  or         $s7, $s7, $at
    /* DD22AC 00001840 8FB60124 */  lw         $s6, 0x124($sp)
    /* DD22B0 00001844 8E23000C */  lw         $v1, 0xC($s1)
  .L00001848:
    /* DD22B4 00001848 02A02025 */  or         $a0, $s5, $zero
    /* DD22B8 0000184C 00008025 */  or         $s0, $zero, $zero
    /* DD22BC 00001850 8C650004 */  lw         $a1, 0x4($v1)
    /* DD22C0 00001854 24AE0001 */  addiu      $t6, $a1, 0x1
    /* DD22C4 00001858 AC6E0004 */  sw         $t6, 0x4($v1)
    /* DD22C8 0000185C 8E38000C */  lw         $t8, 0xC($s1)
    /* DD22CC 00001860 0005C8C0 */  sll        $t9, $a1, 3
    /* DD22D0 00001864 8F0F0000 */  lw         $t7, 0x0($t8)
    /* DD22D4 00001868 01F9A021 */  addu       $s4, $t7, $t9
    /* DD22D8 0000186C 0C000000 */  jal        func_00000000
    /* DD22DC 00001870 AE970000 */   sw        $s7, 0x0($s4)
    /* DD22E0 00001874 1A60004B */  blez       $s3, .L000019A4
    /* DD22E4 00001878 AE820004 */   sw        $v0, 0x4($s4)
    /* DD22E8 0000187C 02D30019 */  multu      $s6, $s3
    /* DD22EC 00001880 27AF0130 */  addiu      $t7, $sp, 0x130
    /* DD22F0 00001884 32620003 */  andi       $v0, $s3, 0x3
    /* DD22F4 00001888 00403825 */  or         $a3, $v0, $zero
    /* DD22F8 0000188C 0010C880 */  sll        $t9, $s0, 2
    /* DD22FC 00001890 00007012 */  mflo       $t6
    /* DD2300 00001894 000EC080 */  sll        $t8, $t6, 2
    /* DD2304 00001898 10400011 */  beqz       $v0, .L000018E0
    /* DD2308 0000189C 030F4021 */   addu      $t0, $t8, $t7
    /* DD230C 000018A0 01192821 */  addu       $a1, $t0, $t9
  .L000018A4:
    /* DD2310 000018A4 8E22000C */  lw         $v0, 0xC($s1)
    /* DD2314 000018A8 8CA40000 */  lw         $a0, 0x0($a1)
    /* DD2318 000018AC 26100001 */  addiu      $s0, $s0, 0x1
    /* DD231C 000018B0 8C460004 */  lw         $a2, 0x4($v0)
    /* DD2320 000018B4 24A50004 */  addiu      $a1, $a1, 0x4
    /* DD2324 000018B8 24CE0001 */  addiu      $t6, $a2, 0x1
    /* DD2328 000018BC AC4E0004 */  sw         $t6, 0x4($v0)
    /* DD232C 000018C0 8E38000C */  lw         $t8, 0xC($s1)
    /* DD2330 000018C4 0006C8C0 */  sll        $t9, $a2, 3
    /* DD2334 000018C8 8F0F0000 */  lw         $t7, 0x0($t8)
    /* DD2338 000018CC 01F91821 */  addu       $v1, $t7, $t9
    /* DD233C 000018D0 AC720000 */  sw         $s2, 0x0($v1)
    /* DD2340 000018D4 14F0FFF3 */  bne        $a3, $s0, .L000018A4
    /* DD2344 000018D8 AC640004 */   sw        $a0, 0x4($v1)
    /* DD2348 000018DC 12130031 */  beq        $s0, $s3, .L000019A4
  .L000018E0:
    /* DD234C 000018E0 00107080 */   sll       $t6, $s0, 2
    /* DD2350 000018E4 010E2821 */  addu       $a1, $t0, $t6
  .L000018E8:
    /* DD2354 000018E8 8E22000C */  lw         $v0, 0xC($s1)
    /* DD2358 000018EC 8CA40000 */  lw         $a0, 0x0($a1)
    /* DD235C 000018F0 26100004 */  addiu      $s0, $s0, 0x4
    /* DD2360 000018F4 8C460004 */  lw         $a2, 0x4($v0)
    /* DD2364 000018F8 24A50010 */  addiu      $a1, $a1, 0x10
    /* DD2368 000018FC 24D80001 */  addiu      $t8, $a2, 0x1
    /* DD236C 00001900 AC580004 */  sw         $t8, 0x4($v0)
    /* DD2370 00001904 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD2374 00001908 000670C0 */  sll        $t6, $a2, 3
    /* DD2378 0000190C 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD237C 00001910 032E1821 */  addu       $v1, $t9, $t6
    /* DD2380 00001914 AC720000 */  sw         $s2, 0x0($v1)
    /* DD2384 00001918 AC640004 */  sw         $a0, 0x4($v1)
    /* DD2388 0000191C 8E22000C */  lw         $v0, 0xC($s1)
    /* DD238C 00001920 8CA4FFF4 */  lw         $a0, -0xC($a1)
    /* DD2390 00001924 8C460004 */  lw         $a2, 0x4($v0)
    /* DD2394 00001928 24D80001 */  addiu      $t8, $a2, 0x1
    /* DD2398 0000192C AC580004 */  sw         $t8, 0x4($v0)
    /* DD239C 00001930 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD23A0 00001934 000670C0 */  sll        $t6, $a2, 3
    /* DD23A4 00001938 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD23A8 0000193C 032E1821 */  addu       $v1, $t9, $t6
    /* DD23AC 00001940 AC720000 */  sw         $s2, 0x0($v1)
    /* DD23B0 00001944 AC640004 */  sw         $a0, 0x4($v1)
    /* DD23B4 00001948 8E22000C */  lw         $v0, 0xC($s1)
    /* DD23B8 0000194C 8CA4FFF8 */  lw         $a0, -0x8($a1)
    /* DD23BC 00001950 8C460004 */  lw         $a2, 0x4($v0)
    /* DD23C0 00001954 24D80001 */  addiu      $t8, $a2, 0x1
    /* DD23C4 00001958 AC580004 */  sw         $t8, 0x4($v0)
    /* DD23C8 0000195C 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD23CC 00001960 000670C0 */  sll        $t6, $a2, 3
    /* DD23D0 00001964 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD23D4 00001968 032E1821 */  addu       $v1, $t9, $t6
    /* DD23D8 0000196C AC720000 */  sw         $s2, 0x0($v1)
    /* DD23DC 00001970 AC640004 */  sw         $a0, 0x4($v1)
    /* DD23E0 00001974 8E22000C */  lw         $v0, 0xC($s1)
    /* DD23E4 00001978 8CA4FFFC */  lw         $a0, -0x4($a1)
    /* DD23E8 0000197C 8C460004 */  lw         $a2, 0x4($v0)
    /* DD23EC 00001980 24D80001 */  addiu      $t8, $a2, 0x1
    /* DD23F0 00001984 AC580004 */  sw         $t8, 0x4($v0)
    /* DD23F4 00001988 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD23F8 0000198C 000670C0 */  sll        $t6, $a2, 3
    /* DD23FC 00001990 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD2400 00001994 032E1821 */  addu       $v1, $t9, $t6
    /* DD2404 00001998 AC720000 */  sw         $s2, 0x0($v1)
    /* DD2408 0000199C 1613FFD2 */  bne        $s0, $s3, .L000018E8
    /* DD240C 000019A0 AC640004 */   sw        $a0, 0x4($v1)
  .L000019A4:
    /* DD2410 000019A4 8FD90000 */  lw         $t9, 0x0($fp)
    /* DD2414 000019A8 26D60001 */  addiu      $s6, $s6, 0x1
    /* DD2418 000019AC 0013C080 */  sll        $t8, $s3, 2
    /* DD241C 000019B0 00187900 */  sll        $t7, $t8, 4
    /* DD2420 000019B4 02AFA821 */  addu       $s5, $s5, $t7
    /* DD2424 000019B8 07210003 */  bgez       $t9, .L000019C8
    /* DD2428 000019BC 00197183 */   sra       $t6, $t9, 6
    /* DD242C 000019C0 2721003F */  addiu      $at, $t9, 0x3F
    /* DD2430 000019C4 00017183 */  sra        $t6, $at, 6
  .L000019C8:
    /* DD2434 000019C8 02CE082A */  slt        $at, $s6, $t6
    /* DD2438 000019CC 5420FF9E */  bnel       $at, $zero, .L00001848
    /* DD243C 000019D0 8E23000C */   lw        $v1, 0xC($s1)
    /* DD2440 000019D4 AFB60124 */  sw         $s6, 0x124($sp)
  .L000019D8:
    /* DD2444 000019D8 8E23000C */  lw         $v1, 0xC($s1)
    /* DD2448 000019DC 8C650004 */  lw         $a1, 0x4($v1)
    /* DD244C 000019E0 24B80001 */  addiu      $t8, $a1, 0x1
    /* DD2450 000019E4 AC780004 */  sw         $t8, 0x4($v1)
    /* DD2454 000019E8 8E2F000C */  lw         $t7, 0xC($s1)
    /* DD2458 000019EC 000570C0 */  sll        $t6, $a1, 3
    /* DD245C 000019F0 3C18B800 */  lui        $t8, (0xB8000000 >> 16)
    /* DD2460 000019F4 8DF90000 */  lw         $t9, 0x0($t7)
    /* DD2464 000019F8 032E1021 */  addu       $v0, $t9, $t6
    /* DD2468 000019FC AC580000 */  sw         $t8, 0x0($v0)
    /* DD246C 00001A00 AC400004 */  sw         $zero, 0x4($v0)
    /* DD2470 00001A04 8FB901D8 */  lw         $t9, 0x1D8($sp)
    /* DD2474 00001A08 8FAF0120 */  lw         $t7, 0x120($sp)
    /* DD2478 00001A0C AF2F0038 */  sw         $t7, 0x38($t9)
  .L00001A10:
    /* DD247C 00001A10 8FBF003C */  lw         $ra, 0x3C($sp)
    /* DD2480 00001A14 8FA201D8 */  lw         $v0, 0x1D8($sp)
    /* DD2484 00001A18 8FB00018 */  lw         $s0, 0x18($sp)
    /* DD2488 00001A1C 8FB1001C */  lw         $s1, 0x1C($sp)
    /* DD248C 00001A20 8FB20020 */  lw         $s2, 0x20($sp)
    /* DD2490 00001A24 8FB30024 */  lw         $s3, 0x24($sp)
    /* DD2494 00001A28 8FB40028 */  lw         $s4, 0x28($sp)
    /* DD2498 00001A2C 8FB5002C */  lw         $s5, 0x2C($sp)
    /* DD249C 00001A30 8FB60030 */  lw         $s6, 0x30($sp)
    /* DD24A0 00001A34 8FB70034 */  lw         $s7, 0x34($sp)
    /* DD24A4 00001A38 8FBE0038 */  lw         $fp, 0x38($sp)
    /* DD24A8 00001A3C 03E00008 */  jr         $ra
    /* DD24AC 00001A40 27BD01D8 */   addiu     $sp, $sp, 0x1D8
endlabel U64BGWall_ctor
