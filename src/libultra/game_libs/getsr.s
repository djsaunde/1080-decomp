# __osGetSR, game_libs copy -- libultra handwritten assembly (os/getsr.s).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/getsr.s; kernel copy
# src/libultra/os/getsr.s.
# splat's symbol ends after the jr: its delay-slot nop is the first word of the
# following pad block (_pad_gl_func_0006C90C sidecar in the host unit).

glabel game_libs_func_0006C9E4
    mfc0    $v0, $12                   # v0 = C0_SR
    jr      $ra                         # (delay slot: next block's nop)
endlabel game_libs_func_0006C9E4
