# __osSetFpcCsr -- libultra handwritten assembly (os/setfpccsr.s).
# Reference: references/oot/src/libultra/os/setfpccsr.s,
#            references/libreultra/src/os/setfpccsr.s
# u32 __osSetFpcCsr(u32 value): returns the old FPU control/status register.
# Assembled in place via GLOBAL_ASM (asm-processor; .set noreorder, so delay
# slots are explicit). See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources.

glabel __osSetFpcCsr
    cfc1    $v0, $31        # v0 = C1_FPCSR (old value)
    ctc1    $a0, $31        # C1_FPCSR = value
    jr      $ra
     nop
endlabel __osSetFpcCsr
