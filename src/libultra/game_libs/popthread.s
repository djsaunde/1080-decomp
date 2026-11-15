# __osPopThread, game_libs copy -- libultra handwritten assembly
# (os/exceptasm.s; it sits between the exception handler pieces
# gl___osExceptionPreamble and gl___osDispatchThread).
# (libultra is statically linked into game_libs as well as the kernel;
# the code is byte-identical, only the splat symbol boundaries differ.)
# Reference: references/oot/src/libultra/os/exceptasm.s (__osPopThread)
# OSThread *__osPopThread(OSThread **queue): unlink and return the head.

glabel gl___osPopThread
    lw      $v0, 0x0($a0)              # v0 = *queue
    lw      $t9, 0x0($v0)              # t9 = v0->next
    jr      $ra
     sw     $t9, 0x0($a0)              # *queue = next
endlabel gl___osPopThread
