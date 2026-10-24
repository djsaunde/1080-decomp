# rdb debug-read byte store, part 3 (splat symbol func_800031E0); continues
# exceptasm_rdb_2.s, falls through into func_800031F0 (.L800031F4).

glabel func_800031E0
    beq     $zero, $t1, .L800031F4
     addi   $t3, $t3, 0x1
    andi    $t0, $t0, 0xFF
    sb      $t0, 0x0($t3)
endlabel func_800031E0

