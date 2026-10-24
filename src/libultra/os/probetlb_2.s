# __osProbeTLB, part 2 of 2 (splat symbol func_80004DB8).
# Continues src/libultra/os/probetlb.s -- see the header there.
# Reference: references/oot/src/libultra/os/probetlb.s

glabel func_80004DB8
    and     $t5, $a0, $t3               # page offset of vaddr
    add     $v0, $v0, $t5               # paddr = pfn + offset
    b       .LprobeTLB_done
     nop
.LprobeTLB_fail:
    addiu   $v0, $zero, -1              # no mapping: return -1
.LprobeTLB_done:
    mtc0    $t0, $10                    # restore C0_ENTRYHI
    jr      $ra
     nop
endlabel func_80004DB8
    nop                                 # object alignment padding
    nop
