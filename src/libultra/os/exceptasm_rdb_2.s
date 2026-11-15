# rdb debug-read byte store, part 2 (splat symbol exceptasm_rdbStore_part2); continues
# .Lrdb_debug_store in exceptasm_rdb.s, ends in exceptasm_rdb_3.s.

glabel exceptasm_rdbStore_part2
    srl     $t2, $t0, 8
    andi    $t2, $t2, 0xFF
    addi    $t1, $t1, -0x1
    sb      $t2, 0x0($t3)
endlabel exceptasm_rdbStore_part2

