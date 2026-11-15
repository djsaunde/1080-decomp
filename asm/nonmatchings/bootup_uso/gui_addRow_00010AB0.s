nonmatching gui_addRow_00010AB0, 0xBC

glabel gui_addRow_00010AB0
    /* DE151C 00010AB0 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* DE1520 00010AB4 240E0001 */  addiu      $t6, $zero, 0x1
    /* DE1524 00010AB8 8C8F0078 */  lw         $t7, 0x78($a0)
    /* DE1528 00010ABC 000FC080 */  sll        $t8, $t7, 2
    /* DE152C 00010AC0 030FC021 */  addu       $t8, $t8, $t7
    /* DE1530 00010AC4 0018C0C0 */  sll        $t8, $t8, 3
    /* DE1534 00010AC8 0098C821 */  addu       $t9, $a0, $t8
    /* DE1538 00010ACC AF2E0088 */  sw         $t6, 0x88($t9)
    /* DE153C 00010AD0 2401FFFF */  addiu      $at, $zero, -0x1
    /* DE1540 00010AD4 10C10009 */  beq        $a2, $at, .L00010AFC
    /* DE1544 00010AD8 00000000 */   nop
    /* DE1548 00010ADC 8C890078 */  lw         $t1, 0x78($a0)
    /* DE154C 00010AE0 00095080 */  sll        $t2, $t1, 2
    /* DE1550 00010AE4 01495021 */  addu       $t2, $t2, $t1
    /* DE1554 00010AE8 000A50C0 */  sll        $t2, $t2, 3
    /* DE1558 00010AEC 008A5821 */  addu       $t3, $a0, $t2
    /* DE155C 00010AF0 AD66008C */  sw         $a2, 0x8C($t3)
    /* DE1560 00010AF4 10000008 */  b          .L00010B18
    /* DE1564 00010AF8 00000000 */   nop
  .L00010AFC:
    /* DE1568 00010AFC 8C8C0078 */  lw         $t4, 0x78($a0)
    /* DE156C 00010B00 8C8D0078 */  lw         $t5, 0x78($a0)
    /* DE1570 00010B04 000D7880 */  sll        $t7, $t5, 2
    /* DE1574 00010B08 01ED7821 */  addu       $t7, $t7, $t5
    /* DE1578 00010B0C 000F78C0 */  sll        $t7, $t7, 3
    /* DE157C 00010B10 008FC021 */  addu       $t8, $a0, $t7
    /* DE1580 00010B14 AF0C008C */  sw         $t4, 0x8C($t8)
  .L00010B18:
    /* DE1584 00010B18 8C8E0078 */  lw         $t6, 0x78($a0)
    /* DE1588 00010B1C 000EC880 */  sll        $t9, $t6, 2
    /* DE158C 00010B20 032EC821 */  addu       $t9, $t9, $t6
    /* DE1590 00010B24 0019C8C0 */  sll        $t9, $t9, 3
    /* DE1594 00010B28 00994821 */  addu       $t1, $a0, $t9
    /* DE1598 00010B2C AD250084 */  sw         $a1, 0x84($t1)
    /* DE159C 00010B30 8C8A0078 */  lw         $t2, 0x78($a0)
    /* DE15A0 00010B34 000A5880 */  sll        $t3, $t2, 2
    /* DE15A4 00010B38 016A5821 */  addu       $t3, $t3, $t2
    /* DE15A8 00010B3C 000B58C0 */  sll        $t3, $t3, 3
    /* DE15AC 00010B40 008B6821 */  addu       $t5, $a0, $t3
    /* DE15B0 00010B44 ADA70090 */  sw         $a3, 0x90($t5)
    /* DE15B4 00010B48 00804025 */  or         $t0, $a0, $zero
    /* DE15B8 00010B4C 8D0F0078 */  lw         $t7, 0x78($t0)
    /* DE15BC 00010B50 25EC0001 */  addiu      $t4, $t7, 0x1
    /* DE15C0 00010B54 AD0C0078 */  sw         $t4, 0x78($t0)
    /* DE15C4 00010B58 10000001 */  b          .L00010B60
    /* DE15C8 00010B5C 00000000 */   nop
  .L00010B60:
    /* DE15CC 00010B60 27BD0008 */  addiu      $sp, $sp, 0x8
    /* DE15D0 00010B64 03E00008 */  jr         $ra
    /* DE15D4 00010B68 00000000 */   nop
endlabel gui_addRow_00010AB0
