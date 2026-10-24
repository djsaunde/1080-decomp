# osMapTLBRdb -- libultra handwritten assembly (os/maptlbrdb.s).
# Reference: references/oot/src/libultra/os/maptlbrdb.s,
#            references/libreultra/src/os/maptlbrdb.s
# void osMapTLBRdb(void): maps the RDB port (0xC0000000) in the last TLB entry.
# splat symbol: func_800099F0.
#   CP0: $0 = C0_INX, $2 = C0_ENTRYLO0, $3 = C0_ENTRYLO1, $5 = C0_PAGEMASK,
#        $10 = C0_ENTRYHI

glabel func_800099F0
    mfc0    $t0, $10                    # save C0_ENTRYHI
    addiu   $t1, $zero, 0x1F            # NTLBENTRIES
    mtc0    $t1, $0                     # C0_INX
    mtc0    $zero, $5                   # C0_PAGEMASK = 4K
    addiu   $t2, $zero, 0x17            # TLBLO_UNCACHED|TLBLO_D|TLBLO_V|TLBLO_G
    lui     $t1, 0xC000                 # RDB_BASE_REG & TLBHI_VPN2MASK
    mtc0    $t1, $10                    # C0_ENTRYHI
    # (libultra quirk: the virtual address is used as the page frame number)
    lui     $t1, 0x8000                 # RDB_BASE_VIRTUAL_ADDR
    srl     $t3, $t1, 6                 # TLBLO_PFNSHIFT
    or      $t3, $t3, $t2
    mtc0    $t3, $2                     # C0_ENTRYLO0
    addiu   $t1, $zero, 0x1             # TLBLO_G
    mtc0    $t1, $3                     # C0_ENTRYLO1
    nop
    tlbwi                               # write indexed TLB entry
    nop
    nop
    nop
    nop
    mtc0    $t0, $10                    # restore C0_ENTRYHI
    jr      $ra
     nop
endlabel func_800099F0
    nop                                 # object alignment padding
    nop
