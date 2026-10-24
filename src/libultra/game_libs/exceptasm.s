# __osExceptionPreamble + __osException (to __osEnqueueThread), game_libs
# copy -- libultra handwritten assembly (os/exceptasm.s). This copy is the
# release libultra form (no rdb/KMC front end; compare the kernel's
# src/libultra/os/exceptasm_rdb.s). The routine order and register-save layout
# follow references/libreultra/src/os/exceptasm.s (savecontext, savercp,
# handle_interrupt + __osIntOffTable/__osIntTable dispatch, counter, cart,
# rcp (sp/vi/ai/si/pi/dp), prenmi, handle_break, redispatch, enqueueRunning,
# panic, send_mesg, handle_CpU, __osEnqueueAndYield, __osEnqueueThread);
# __osPopThread follows in popthread.s.
# splat's symbol starts with the zero word that pads the preceding object.
#
# USO module code: every address field is a load-time relocation (zero in the
# ROM), so data and call targets are symbols pinned to 0 in
# undefined_syms_auto.txt: gl_exc_threadsave (__osThreadSave),
# D_00000000_runthread (__osRunningThread), gl_exc_runqueue (__osRunQueue),
# D_00000000_gim (__OSGlobalIntMask), D_00000000_rcpim (__osRcpImTable),
# gl_exc_intofftable / gl_exc_inttable (__osIntOffTable / __osIntTable),
# gl_exc_hwinttable (__osHwIntTable), gl_exc_intstack (interrupt-callback
# stack), gl_exc_shutdown (__osShutdown), gl_exc_faultedthread
# (__osFaultedThread), gl_exc_eventstatetab (__osEventStateTab),
# gl_exc_exception (__osException), gl_exc_send_mesg (send_mesg),
# gl_exc_enqueuethread / gl_func_00000000_popthr / gl_exc_dispatchthread.

glabel game_libs_func_0006A5F0
    nop                                 # alignment padding (preceding object)
    # __osExceptionPreamble (copied to the exception vectors)
    lui     $k0, %hi(gl_exc_exception)   # __osException (USO load-time reloc)
    addiu   $k0, $k0, %lo(gl_exc_exception)
    jr      $k0
     nop
    # __osException: save at/t0-t2/SR in __osThreadSave, mask interrupts
    lui     $k0, %hi(gl_exc_threadsave)
    addiu   $k0, $k0, %lo(gl_exc_threadsave)
    sd      $at, 32($k0)
    mfc0    $k1, $12
    sw      $k1, 280($k0)
    addiu   $at, $zero, -4
    and     $k1, $k1, $at
    mtc0    $k1, $12
    sd      $t0, 88($k0)
    sd      $t1, 96($k0)
    sd      $t2, 104($k0)
    sw      $zero, 24($k0)
    mfc0    $t0, $13
    # savecontext: copy the scratch saves to __osRunningThread's context
    or      $t0, $k0, $zero
    lui     $k0, %hi(D_00000000_runthread)
    lw      $k0, %lo(D_00000000_runthread)($k0)
    ld      $t1, 32($t0)
    sd      $t1, 32($k0)
    ld      $t1, 280($t0)
    sd      $t1, 280($k0)
    ld      $t1, 88($t0)
    sd      $t1, 88($k0)
    ld      $t1, 96($t0)
    sd      $t1, 96($k0)
    ld      $t1, 104($t0)
    sd      $t1, 104($k0)
    lw      $k1, 280($k0)
    mflo    $t0
    sd      $t0, 264($k0)
    mfhi    $t0
    andi    $t1, $k1, 0xff00
    sd      $v0, 40($k0)
    sd      $v1, 48($k0)
    sd      $a0, 56($k0)
    sd      $a1, 64($k0)
    sd      $a2, 72($k0)
    sd      $a3, 80($k0)
    sd      $t3, 112($k0)
    sd      $t4, 120($k0)
    sd      $t5, 128($k0)
    sd      $t6, 136($k0)
    sd      $t7, 144($k0)
    sd      $s0, 152($k0)
    sd      $s1, 160($k0)
    sd      $s2, 168($k0)
    sd      $s3, 176($k0)
    sd      $s4, 184($k0)
    sd      $s5, 192($k0)
    sd      $s6, 200($k0)
    sd      $s7, 208($k0)
    sd      $t8, 216($k0)
    sd      $t9, 224($k0)
    sd      $gp, 232($k0)
    sd      $sp, 240($k0)
    sd      $s8, 248($k0)
    sd      $ra, 256($k0)
    beqz    $t1, .Lgl_savercp
     sd     $t0, 272($k0)
    lui     $t0, %hi(D_00000000_gim)
    addiu   $t0, $t0, %lo(D_00000000_gim)
    lw      $t0, 0($t0)
    addiu   $at, $zero, -1
    xor     $t2, $t0, $at
    lui     $at, 0xffff
    andi    $t2, $t2, 0xff00
    ori     $at, $at, 0xff
    or      $t4, $t1, $t2
    and     $t3, $k1, $at
    andi    $t0, $t0, 0xff00
    or      $t3, $t3, $t4
    and     $t1, $t1, $t0
    and     $k1, $k1, $at
    sw      $t3, 280($k0)
    or      $k1, $k1, $t1
.Lgl_savercp:
    lui     $t1, 0xa430
    lw      $t1, 12($t1)
    beqz    $t1, .Lgl_endrcp
     nop
    lui     $t0, %hi(D_00000000_gim)
    addiu   $t0, $t0, %lo(D_00000000_gim)
    lw      $t0, 0($t0)
    lw      $t4, 296($k0)
    addiu   $at, $zero, -1
    srl     $t0, $t0, 0x10
    xor     $t0, $t0, $at
    andi    $t0, $t0, 0x3f
    and     $t0, $t0, $t4
    or      $t1, $t1, $t0
.Lgl_endrcp:
    sw      $t1, 296($k0)
    mfc0    $t0, $14
    sw      $t0, 284($k0)
    lw      $t0, 24($k0)
    beqz    $t0, .Lgl_save_cause
     nop
    cfc1    $t0, $31        # C1_FPCSR
    nop
    sw      $t0, 300($k0)
    sdc1    $f0, 304($k0)
    sdc1    $f2, 312($k0)
    sdc1    $f4, 320($k0)
    sdc1    $f6, 328($k0)
    sdc1    $f8, 336($k0)
    sdc1    $f10, 344($k0)
    sdc1    $f12, 352($k0)
    sdc1    $f14, 360($k0)
    sdc1    $f16, 368($k0)
    sdc1    $f18, 376($k0)
    sdc1    $f20, 384($k0)
    sdc1    $f22, 392($k0)
    sdc1    $f24, 400($k0)
    sdc1    $f26, 408($k0)
    sdc1    $f28, 416($k0)
    sdc1    $f30, 424($k0)
.Lgl_save_cause:
    mfc0    $t0, $13
    sw      $t0, 288($k0)
    addiu   $t1, $zero, 2
    sh      $t1, 16($k0)
    andi    $t1, $t0, 0x7c
    addiu   $t2, $zero, 36
    beq     $t1, $t2, .Lgl_handle_break
     nop
    addiu   $t2, $zero, 44
    beq     $t1, $t2, .Lgl_handle_CpU
     nop
    addiu   $t2, $zero, 0
    bne     $t1, $t2, .Lgl_panic
     nop
    and     $s0, $k1, $t0
.Lgl_next_interrupt:
    andi    $t1, $s0, 0xff00
    srl     $t2, $t1, 0xc
    bnez    $t2, .Lgl_interrupt_dispatch
     nop
    srl     $t2, $t1, 0x8
    addi    $t2, $t2, 16
.Lgl_interrupt_dispatch:
    lui     $at, %hi(gl_exc_intofftable)
    addu    $at, $at, $t2
    lbu     $t2, 9056($at)
    lui     $at, %hi(gl_exc_inttable)
    addu    $at, $at, $t2
    lw      $t2, 9088($at)
    jr      $t2
     nop
    addiu   $at, $zero, -8193
    b       .Lgl_next_interrupt
     and    $s0, $s0, $at
    addiu   $at, $zero, -16385
    b       .Lgl_next_interrupt
     and    $s0, $s0, $at
    mfc0    $t1, $11
    mtc0    $t1, $11
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 24
    lui     $at, 0xffff
    ori     $at, $at, 0x7fff
    b       .Lgl_next_interrupt
     and    $s0, $s0, $at
    addiu   $at, $zero, -2049
    and     $s0, $s0, $at
    addiu   $t2, $zero, 4
    lui     $at, %hi(gl_exc_hwinttable)
    addu    $at, $at, $t2
    lw      $t2, %lo(gl_exc_hwinttable)($at)
    lui     $sp, %hi(gl_exc_intstack)
    addiu   $sp, $sp, %lo(gl_exc_intstack)
    addiu   $a0, $zero, 16
    beqz    $t2, .Lgl_cart_send
     addiu  $sp, $sp, 4080
    jalr    $t2
     nop
    beqz    $v0, .Lgl_cart_send
     addiu  $a0, $zero, 16
    b       .Lgl_redispatch
     nop
.Lgl_cart_send:
    jal     gl_exc_send_mesg
     nop
    b       .Lgl_next_interrupt
     nop
    lui     $t0, %hi(D_00000000_gim)
    addiu   $t0, $t0, %lo(D_00000000_gim)
    lw      $t0, 0($t0)
    lui     $s1, 0xa430
    lw      $s1, 8($s1)
    srl     $t0, $t0, 0x10
    and     $s1, $s1, $t0
    andi    $t1, $s1, 0x1
    beqz    $t1, .Lgl_vi
     nop
    lui     $t4, 0xa404
    lw      $t4, 16($t4)
    ori     $t1, $zero, 0x8008
    lui     $at, 0xa404
    andi    $t4, $t4, 0x300
    andi    $s1, $s1, 0x3e
    beqz    $t4, .Lgl_sp_other_break
     sw     $t1, 16($at)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 32
    beqz    $s1, .Lgl_NoMoreRcpInts
     nop
    b       .Lgl_vi
     nop
.Lgl_sp_other_break:
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 88
    beqz    $s1, .Lgl_NoMoreRcpInts
     nop
.Lgl_vi:
    andi    $t1, $s1, 0x8
    beqz    $t1, .Lgl_ai
     lui    $at, 0xa440
    andi    $s1, $s1, 0x37
    sw      $zero, 16($at)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 56
    beqz    $s1, .Lgl_NoMoreRcpInts
     nop
.Lgl_ai:
    andi    $t1, $s1, 0x4
    beqz    $t1, .Lgl_si
     nop
    addiu   $t1, $zero, 1
    lui     $at, 0xa450
    andi    $s1, $s1, 0x3b
    sw      $t1, 12($at)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 48
    beqz    $s1, .Lgl_NoMoreRcpInts
     nop
.Lgl_si:
    andi    $t1, $s1, 0x2
    beqz    $t1, .Lgl_pi
     lui    $at, 0xa480
    andi    $s1, $s1, 0x3d
    sw      $zero, 24($at)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 40
    beqz    $s1, .Lgl_NoMoreRcpInts
     nop
.Lgl_pi:
    andi    $t1, $s1, 0x10
    beqz    $t1, .Lgl_dp
     nop
    addiu   $t1, $zero, 2
    lui     $at, 0xa460
    andi    $s1, $s1, 0x2f
    sw      $t1, 16($at)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 64
    beqz    $s1, .Lgl_NoMoreRcpInts
     nop
.Lgl_dp:
    andi    $t1, $s1, 0x20
    beqz    $t1, .Lgl_NoMoreRcpInts
     nop
    addiu   $t1, $zero, 2048
    lui     $at, 0xa430
    andi    $s1, $s1, 0x1f
    sw      $t1, 0($at)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 72
.Lgl_NoMoreRcpInts:
    addiu   $at, $zero, -1025
    b       .Lgl_next_interrupt
     and    $s0, $s0, $at
    lw      $k1, 280($k0)
    addiu   $at, $zero, -4097
    lui     $t1, %hi(gl_exc_shutdown)
    and     $k1, $k1, $at
    sw      $k1, 280($k0)
    addiu   $t1, $t1, %lo(gl_exc_shutdown)
    lw      $t2, 0($t1)
    beqz    $t2, .Lgl_firstnmi
     addiu  $at, $zero, -4097
    b       .Lgl_redispatch
     and    $s0, $s0, $at
.Lgl_firstnmi:
    addiu   $t2, $zero, 1
    sw      $t2, 0($t1)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 112
    lui     $t2, %hi(gl_exc_runqueue)
    lw      $t2, %lo(gl_exc_runqueue)($t2)
    addiu   $at, $zero, -4097
    and     $s0, $s0, $at
    lw      $k1, 280($t2)
    and     $k1, $k1, $at
    b       .Lgl_redispatch
     sw     $k1, 280($t2)
    addiu   $at, $zero, -513
    and     $t0, $t0, $at
    mtc0    $t0, $13
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 8
    addiu   $at, $zero, -513
    b       .Lgl_next_interrupt
     and    $s0, $s0, $at
    addiu   $at, $zero, -257
    and     $t0, $t0, $at
    mtc0    $t0, $13
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 0
    addiu   $at, $zero, -257
    b       .Lgl_next_interrupt
     and    $s0, $s0, $at
.Lgl_handle_break:
    addiu   $t1, $zero, 1
    sh      $t1, 18($k0)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 80
    b       .Lgl_redispatch
     nop
.Lgl_redispatch:
    lui     $t2, %hi(gl_exc_runqueue)
    lw      $t2, %lo(gl_exc_runqueue)($t2)
    lw      $t1, 4($k0)
    lw      $t3, 4($t2)
    slt     $at, $t1, $t3
    beqz    $at, .Lgl_enqueueRunning
     nop
    lui     $a0, %hi(gl_exc_runqueue)
    or      $a1, $k0, $zero
    jal     gl_exc_enqueuethread
     addiu  $a0, $a0, %lo(gl_exc_runqueue)
    j       gl_exc_dispatchthread
     nop
.Lgl_enqueueRunning:
    lui     $t1, %hi(gl_exc_runqueue)
    addiu   $t1, $t1, %lo(gl_exc_runqueue)
    lw      $t2, 0($t1)
    sw      $t2, 0($k0)
    j       gl_exc_dispatchthread
     sw     $k0, 0($t1)
.Lgl_panic:
    lui     $at, %hi(gl_exc_faultedthread)
    sw      $k0, %lo(gl_exc_faultedthread)($at)
    addiu   $t1, $zero, 1
    sh      $t1, 16($k0)
    addiu   $t1, $zero, 2
    sh      $t1, 18($k0)
    mfc0    $t2, $8
    sw      $t2, 292($k0)
    jal     gl_exc_send_mesg
     addiu  $a0, $zero, 96
    j       gl_exc_dispatchthread
     nop
    lui     $t2, %hi(gl_exc_eventstatetab)
    addiu   $t2, $t2, %lo(gl_exc_eventstatetab)
    addu    $t2, $t2, $a0
    lw      $t1, 0($t2)
    or      $s2, $ra, $zero
    beqz    $t1, .Lgl_send_done
     nop
    lw      $t3, 8($t1)
    lw      $t4, 16($t1)
    slt     $at, $t3, $t4
    beqz    $at, .Lgl_send_done
     nop
    lw      $t5, 12($t1)
    addu    $t5, $t5, $t3
    div     $zero, $t5, $t4
    bnez    $t4, .Lgl_send_mesg_divok
     nop
    break   0x7
.Lgl_send_mesg_divok:
    addiu   $at, $zero, -1
    bne     $t4, $at, .Lgl_send_mesg_store
     lui    $at, 0x8000
    bne     $t5, $at, .Lgl_send_mesg_store
     nop
    break   0x6
.Lgl_send_mesg_store:
    lw      $t4, 20($t1)
    mfhi    $t5
    sll     $t5, $t5, 0x2
    addu    $t4, $t4, $t5
    lw      $t5, 4($t2)
    addiu   $t2, $t3, 1
    sw      $t5, 0($t4)
    sw      $t2, 8($t1)
    lw      $t2, 0($t1)
    lw      $t3, 0($t2)
    beqz    $t3, .Lgl_send_done
     nop
    jal     gl_func_00000000_popthr
     or     $a0, $t1, $zero
    or      $t2, $v0, $zero
    lui     $a0, %hi(gl_exc_runqueue)
    or      $a1, $t2, $zero
    jal     gl_exc_enqueuethread
     addiu  $a0, $a0, %lo(gl_exc_runqueue)
.Lgl_send_done:
    jr      $s2
     nop
.Lgl_handle_CpU:
    lui     $at, 0x3000
    and     $t1, $t0, $at
    srl     $t1, $t1, 0x1c
    addiu   $t2, $zero, 1
    bne     $t1, $t2, .Lgl_panic
     nop
    lw      $k1, 280($k0)
    lui     $at, 0x2000
    addiu   $t1, $zero, 1
    or      $k1, $k1, $at
    sw      $t1, 24($k0)
    b       .Lgl_enqueueRunning
     sw     $k1, 280($k0)
    lui     $a1, %hi(D_00000000_runthread)
    lw      $a1, %lo(D_00000000_runthread)($a1)
    mfc0    $t0, $12
    lw      $k1, 24($a1)
    ori     $t0, $t0, 0x2
    sw      $t0, 280($a1)
    sd      $s0, 152($a1)
    sd      $s1, 160($a1)
    sd      $s2, 168($a1)
    sd      $s3, 176($a1)
    sd      $s4, 184($a1)
    sd      $s5, 192($a1)
    sd      $s6, 200($a1)
    sd      $s7, 208($a1)
    sd      $gp, 232($a1)
    sd      $sp, 240($a1)
    sd      $s8, 248($a1)
    sd      $ra, 256($a1)
    beqz    $k1, .Lgl_yield_savefpu_done
     sw     $ra, 284($a1)
    cfc1    $k1, $31        # C1_FPCSR
    sdc1    $f20, 384($a1)
    sdc1    $f22, 392($a1)
    sdc1    $f24, 400($a1)
    sdc1    $f26, 408($a1)
    sdc1    $f28, 416($a1)
    sdc1    $f30, 424($a1)
    sw      $k1, 300($a1)
.Lgl_yield_savefpu_done:
    lw      $k1, 280($a1)
    andi    $t1, $k1, 0xff00
    beqz    $t1, .Lgl_yield_savercp
     nop
    lui     $t0, %hi(D_00000000_gim)
    addiu   $t0, $t0, %lo(D_00000000_gim)
    lw      $t0, 0($t0)
    addiu   $at, $zero, -1
    xor     $t0, $t0, $at
    lui     $at, 0xffff
    andi    $t0, $t0, 0xff00
    ori     $at, $at, 0xff
    or      $t1, $t1, $t0
    and     $k1, $k1, $at
    or      $k1, $k1, $t1
    sw      $k1, 280($a1)
.Lgl_yield_savercp:
    lui     $k1, 0xa430
    lw      $k1, 12($k1)
    beqz    $k1, .Lgl_yield_endrcp
     nop
    lui     $k0, %hi(D_00000000_gim)
    addiu   $k0, $k0, %lo(D_00000000_gim)
    lw      $k0, 0($k0)
    lw      $t0, 296($a1)
    addiu   $at, $zero, -1
    srl     $k0, $k0, 0x10
    xor     $k0, $k0, $at
    andi    $k0, $k0, 0x3f
    and     $k0, $k0, $t0
    or      $k1, $k1, $k0
.Lgl_yield_endrcp:
    beqz    $a0, .Lgl_noEnqueue
     sw     $k1, 296($a1)
    jal     gl_exc_enqueuethread
     nop
.Lgl_noEnqueue:
    j       gl_exc_dispatchthread
     nop
    lw      $t8, 0($a0)
    lw      $t7, 4($a1)
    or      $t9, $a0, $zero
    lw      $t6, 4($t8)
    slt     $at, $t6, $t7
    bnez    $at, .Lgl_enqueueThread_insert
     nop
.Lgl_enqueueThread_loop:
    or      $t9, $t8, $zero
    lw      $t8, 0($t8)
    lw      $t6, 4($t8)
    slt     $at, $t6, $t7
    beqz    $at, .Lgl_enqueueThread_loop
     nop
.Lgl_enqueueThread_insert:
    lw      $t8, 0($t9)
    sw      $t8, 0($a1)
    sw      $a1, 0($t9)
    jr      $ra
     sw     $a0, 8($a1)
endlabel game_libs_func_0006A5F0
