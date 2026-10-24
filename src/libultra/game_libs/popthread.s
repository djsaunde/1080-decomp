# __osPopThread, game_libs copy -- libultra handwritten assembly
# (os/exceptasm.s; it sits between the exception handler pieces
# game_libs_func_0006A5F0 and game_libs_func_0006AD78).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/exceptasm.s (__osPopThread)
# OSThread *__osPopThread(OSThread **queue): unlink and return the head.

glabel game_libs_func_0006AD68
    lw      $v0, 0x0($a0)              # v0 = *queue
    lw      $t9, 0x0($v0)              # t9 = v0->next
    jr      $ra
     sw     $t9, 0x0($a0)              # *queue = next
endlabel game_libs_func_0006AD68
