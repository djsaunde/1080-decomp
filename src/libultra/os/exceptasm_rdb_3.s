# rdb debug-read byte store, part 3 (splat symbol exceptasm_rdbStore_part3); continues
# exceptasm_rdb_2.s, falls through into __osException (.Lrdb_debug_done).

glabel exceptasm_rdbStore_part3
    beq     $zero, $t1, .Lrdb_debug_done
     addi   $t3, $t3, 0x1
    andi    $t0, $t0, 0xFF
    sb      $t0, 0x0($t3)
endlabel exceptasm_rdbStore_part3

