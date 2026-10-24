# __osDispatchThread + __osCleanupThread, game_libs copy -- libultra
# handwritten assembly (os/exceptasm.s; release libultra form).
# Reference: references/libreultra/src/os/exceptasm.s; kernel copy at the end
# of src/libultra/os/exceptasm.s. Follows __osPopThread (popthread.s).
# The compiled-C __osDequeueThread that follows is gl_func_0006AF0C (C, -O1
# donor game_libs_o1_6AF0C.c); its first two instructions used to be counted
# in this symbol (boundary fixed 2026-10-24, agent-g run 158).
# USO load-time relocations go through symbols pinned to 0 (see exceptasm.s):
# gl_exc_runqueue, D_00000000_runthread, D_00000000_gim, D_00000000_rcpim,
# gl_func_00000000_popthr, gl_exc_destroythread (osDestroyThread).

glabel game_libs_func_0006AD78
    # __osDispatchThread: pop the run queue and make it the running thread
    lui     $a0, %hi(gl_exc_runqueue)     # __osRunQueue
    jal     gl_func_00000000_popthr       # __osPopThread
     addiu  $a0, $a0, %lo(gl_exc_runqueue)
    lui     $at, %hi(D_00000000_runthread)
    sw      $v0, %lo(D_00000000_runthread)($at)  # __osRunningThread = popped
    addiu   $t0, $zero, 4             # OS_STATE_RUNNING
    sh      $t0, 16($v0)
    or      $k0, $v0, $zero
    lui     $t0, %hi(D_00000000_gim)      # __OSGlobalIntMask
    lw      $k1, 280($k0)
    addiu   $t0, $t0, %lo(D_00000000_gim)
    lw      $t0, 0($t0)
    lui     $at, 0xffff
    andi    $t1, $k1, 0xff00
    ori     $at, $at, 0xff
    andi    $t0, $t0, 0xff00
    and     $t1, $t1, $t0
    and     $k1, $k1, $at
    or      $k1, $k1, $t1
    mtc0    $k1, $12
    ld      $k1, 264($k0)
    ld      $at, 32($k0)
    ld      $v0, 40($k0)
    mtlo    $k1
    ld      $k1, 272($k0)
    ld      $v1, 48($k0)
    ld      $a0, 56($k0)
    ld      $a1, 64($k0)
    ld      $a2, 72($k0)
    ld      $a3, 80($k0)
    ld      $t0, 88($k0)
    ld      $t1, 96($k0)
    ld      $t2, 104($k0)
    ld      $t3, 112($k0)
    ld      $t4, 120($k0)
    ld      $t5, 128($k0)
    ld      $t6, 136($k0)
    ld      $t7, 144($k0)
    ld      $s0, 152($k0)
    ld      $s1, 160($k0)
    ld      $s2, 168($k0)
    ld      $s3, 176($k0)
    ld      $s4, 184($k0)
    ld      $s5, 192($k0)
    ld      $s6, 200($k0)
    ld      $s7, 208($k0)
    ld      $t8, 216($k0)
    ld      $t9, 224($k0)
    ld      $gp, 232($k0)
    mthi    $k1
    ld      $sp, 240($k0)
    ld      $s8, 248($k0)
    ld      $ra, 256($k0)
    lw      $k1, 284($k0)
    mtc0    $k1, $14
    lw      $k1, 24($k0)
    beqz    $k1, .Lgl_dispatch_nofpu
     nop
    lw      $k1, 300($k0)
    ctc1    $k1, $31        # C1_FPCSR
    ldc1    $f0, 304($k0)
    ldc1    $f2, 312($k0)
    ldc1    $f4, 320($k0)
    ldc1    $f6, 328($k0)
    ldc1    $f8, 336($k0)
    ldc1    $f10, 344($k0)
    ldc1    $f12, 352($k0)
    ldc1    $f14, 360($k0)
    ldc1    $f16, 368($k0)
    ldc1    $f18, 376($k0)
    ldc1    $f20, 384($k0)
    ldc1    $f22, 392($k0)
    ldc1    $f24, 400($k0)
    ldc1    $f26, 408($k0)
    ldc1    $f28, 416($k0)
    ldc1    $f30, 424($k0)
.Lgl_dispatch_nofpu:
    lw      $k1, 296($k0)
    lui     $k0, %hi(D_00000000_gim)
    addiu   $k0, $k0, %lo(D_00000000_gim)
    lw      $k0, 0($k0)
    srl     $k0, $k0, 0x10
    and     $k1, $k1, $k0
    sll     $k1, $k1, 0x1
    lui     $k0, %hi(D_00000000_rcpim)    # __osRcpImTable
    addiu   $k0, $k0, %lo(D_00000000_rcpim)
    addu    $k1, $k1, $k0
    lhu     $k1, 0($k1)
    lui     $k0, 0xa430               # MI_INTR_MASK_REG
    addiu   $k0, $k0, 12
    sw      $k1, 0($k0)
    nop
    nop
    nop
    nop
    eret
    # __osCleanupThread
    jal     gl_exc_destroythread          # osDestroyThread(NULL)
     or     $a0, $zero, $zero
endlabel game_libs_func_0006AD78
    nop                                 # object alignment padding
    nop
