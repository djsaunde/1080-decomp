# osGetCount, game_libs copy -- libultra handwritten assembly (os/getcount.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/getcount.s
# splat's symbol starts with the two zero words that pad the preceding
# __osProbeTLB object, and ends after the jr: the delay-slot nop is the first
# word of the following pad block (_pad_gl_func_0006D7CC sidecar).

glabel game_libs_func_0006D94C
    nop                                 # alignment padding (end of probetlb)
    nop
    # u32 osGetCount(void)
    mfc0    $v0, $9                    # v0 = C0_COUNT
    jr      $ra                         # (delay slot: next block's nop)
endlabel game_libs_func_0006D94C
