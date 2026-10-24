# rdb debug-read byte store, part 2 (splat symbol func_800031D0); continues
# .Lrdb_debug_store in exceptasm_rdb.s, ends in exceptasm_rdb_3.s.

glabel func_800031D0
    srl     $t2, $t0, 8
    andi    $t2, $t2, 0xFF
    addi    $t1, $t1, -0x1
    sb      $t2, 0x0($t3)
endlabel func_800031D0

