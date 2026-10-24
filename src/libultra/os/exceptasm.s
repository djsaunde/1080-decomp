# __osException core -- libultra handwritten assembly (os/exceptasm.s),
# development (rdb) variant; continues the front end in exceptasm_rdb.s.
# Reference: references/libreultra/src/os/exceptasm.s (savecontext, savercp,
# handle_interrupt, counter/cart/rcp handlers, prenmi, handle_break,
# redispatch, enqueueRunning, panic, send_mesg, handle_CpU,
# __osEnqueueAndYield, __osEnqueueThread, __osPopThread, __osDispatchThread,
# __osCleanupThread); the rdb IP6 send path at the top has no public reference.
# Data symbols keep their splat names: D_80019320 __osThreadSave,
# D_8000A420 __osRunningThread, D_8000A418 __osRunQueue, D_8000A3E0
# __OSGlobalIntMask, D_8000A680 __osRcpImTable, D_8000A3F0 __osHwIntTable,
# D_8000A3DC __osShutdown, D_8000A424 __osFaultedThread.
# Calls use runtime addresses (the kernel runs 0x400 above its link address):
# func_80003C24 = send_mesg, func_80003E0C = __osEnqueueThread,
# func_80003E54 = __osPopThread, func_80003E64 = __osDispatchThread.
# The compiled-C __osDequeueThread that follows (func_80003BF0) is C in
# src/kernel/kernel_000_b.c.

glabel func_800031F0
    addi    $t3, $t3, 0x1
    # (continues the rdb debug-read byte store of exceptasm_rdb.s / _2 / _3)
.Lrdb_debug_done:
    lui     $t2, %hi(D_8000A404)
    lw      $t2, %lo(D_8000A404)($t2)
    lui     $at, %hi(__osRdb_DbgRead_Buf)
    sw      $t3, %lo(__osRdb_DbgRead_Buf)($at)
    bne     $zero, $t2, .Lrdb_restore_eret
     ld     $t3, 0x70($k0)
    addiu   $t2, $zero, 0xA0
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
    # IP6 = host ready for the next queued RDB packet (__osRdb_IP6_*)
.Lrdb_check_ip6:
    andi    $t2, $t1, 0x2000
    beqz    $t2, .Lsavecontext
     nop
    lui     $t1, 0xC000
    sw      $zero, 0xC($t1)
    lui     $t2, %hi(__osRdb_IP6_Ct)
    lw      $t2, %lo(__osRdb_IP6_Ct)($t2)
    addiu   $t1, $t1, %lo(D_C000000C)
    bnez    $t2, .Lrdb_ip6_send_next
     nop
    addiu   $t2, $zero, 0x1
    lui     $at, %hi(D_8000A490)
    b       .Lrdb_restore_eret
     sw     $t2, %lo(D_8000A490)($at)
.Lrdb_ip6_send_next:
    lui     $t1, %hi(__osRdb_IP6_CurSend)
    lw      $t1, %lo(__osRdb_IP6_CurSend)($t1)
    lui     $t0, %hi(__osRdb_IP6_Data)
    lw      $t0, %lo(__osRdb_IP6_Data)($t0)
    addi    $t2, $t2, -0x1
    lui     $at, %hi(__osRdb_IP6_Ct)
    sw      $t2, %lo(__osRdb_IP6_Ct)($at)
alabel D_80003270
    sll     $t2, $t1, 2
    add     $t0, $t2, $t0
    lw      $t2, 0x0($t0)
    lui     $t0, %hi(__osRdb_IP6_Size)
    lw      $t0, %lo(__osRdb_IP6_Size)($t0)
    addi    $t1, $t1, 0x1
    sub     $t0, $t0, $t1
    bgtz    $t0, .Lrdb_ip6_cursend_store
     nop
    addiu   $t1, $zero, 0x0
.Lrdb_ip6_cursend_store:
    lui     $at, %hi(__osRdb_IP6_CurSend)
    sw      $t1, %lo(__osRdb_IP6_CurSend)($at)
.Lrdb_send_wait:
    mfc0    $t0, $13
    andi    $t0, $t0, 0x2000
    bne     $zero, $t0, .Lrdb_send_wait
     nop
    lui     $t0, %hi(D_C0000000)
    addiu   $t0, $t0, %lo(D_C0000000)
    sw      $t2, 0x0($t0)
    # rdb packet handled: restore the scratch registers and SR, eret
.Lrdb_restore_eret:
    ld      $t0, 0x58($k0)
    ld      $t1, 0x60($k0)
    ld      $t2, 0x68($k0)
    ld      $at, 0x20($k0)
    lw      $k1, 0x118($k0)
    mtc0    $k1, $12
    nop                                 # object alignment padding
    nop
    nop                                 # object alignment padding
    nop
    eret
    # savecontext: copy the scratch saves into __osRunningThread (D_8000A420)
    # and save the rest of the context (libreultra exceptasm.s savecontext)
.Lsavecontext:
    or      $t0, $k0, $zero
    lui     $k0, %hi(D_8000A420)
    lw      $k0, %lo(D_8000A420)($k0)
    ld      $t1, 0x20($t0)
    sd      $t1, 0x20($k0)
    ld      $t1, 0x118($t0)
    sd      $t1, 0x118($k0)
    ld      $t1, 0x58($t0)
    sd      $t1, 0x58($k0)
    ld      $t1, 0x60($t0)
    sd      $t1, 0x60($k0)
    ld      $t1, 0x68($t0)
    sd      $t1, 0x68($k0)
    lw      $k1, 0x118($k0)
    mflo    $t0
    sd      $t0, 0x108($k0)
    mfhi    $t0
    andi    $t1, $k1, 0xFF00
    sd      $v0, 0x28($k0)
    sd      $v1, 0x30($k0)
    sd      $a0, 0x38($k0)
    sd      $a1, 0x40($k0)
    sd      $a2, 0x48($k0)
alabel D_80003344
    sd      $a3, 0x50($k0)
    sd      $t3, 0x70($k0)
    sd      $t4, 0x78($k0)
    sd      $t5, 0x80($k0)
    sd      $t6, 0x88($k0)
    sd      $t7, 0x90($k0)
    sd      $s0, 0x98($k0)
    sd      $s1, 0xA0($k0)
    sd      $s2, 0xA8($k0)
    sd      $s3, 0xB0($k0)
    sd      $s4, 0xB8($k0)
    sd      $s5, 0xC0($k0)
    sd      $s6, 0xC8($k0)
    sd      $s7, 0xD0($k0)
    sd      $t8, 0xD8($k0)
    sd      $t9, 0xE0($k0)
    sd      $gp, 0xE8($k0)
    sd      $sp, 0xF0($k0)
    sd      $fp, 0xF8($k0)
    sd      $ra, 0x100($k0)
    beqz    $t1, .Lsavercp
     sd     $t0, 0x110($k0)
    lui     $t0, %hi(D_8000A3E0)
    addiu   $t0, $t0, %lo(D_8000A3E0)
    lw      $t0, 0x0($t0)
    addiu   $at, $zero, -0x1
    xor     $t2, $t0, $at
    lui     $at, 0xFFFF
    andi    $t2, $t2, 0xFF00
    ori     $at, $at, 0xFF
    or      $t4, $t1, $t2
    and     $t3, $k1, $at
    andi    $t0, $t0, 0xFF00
    or      $t3, $t3, $t4
    and     $t1, $t1, $t0
    and     $k1, $k1, $at
    sw      $t3, 0x118($k0)
    or      $k1, $k1, $t1
.Lsavercp:
    lui     $t1, %hi(D_A430000C)
    lw      $t1, %lo(D_A430000C)($t1)
    beqz    $t1, .Lendrcp
     nop
    lui     $t0, %hi(D_8000A3E0)
    addiu   $t0, $t0, %lo(D_8000A3E0)
    lw      $t0, 0x0($t0)
    lw      $t4, 0x128($k0)
    addiu   $at, $zero, -0x1
    srl     $t0, $t0, 16
    xor     $t0, $t0, $at
    andi    $t0, $t0, 0x3F
    and     $t0, $t0, $t4
    or      $t1, $t1, $t0
.Lendrcp:
    sw      $t1, 0x128($k0)
    mfc0    $t0, $14
    sw      $t0, 0x11C($k0)
    lw      $t0, 0x18($k0)
    beqz    $t0, .Lsave_cause
     nop
    cfc1    $t0, $31
    nop
    sw      $t0, 0x12C($k0)
    sdc1    $f0, 0x130($k0)
    sdc1    $f2, 0x138($k0)
    sdc1    $f4, 0x140($k0)
    sdc1    $f6, 0x148($k0)
    sdc1    $f8, 0x150($k0)
    sdc1    $f10, 0x158($k0)
    sdc1    $f12, 0x160($k0)
    sdc1    $f14, 0x168($k0)
    sdc1    $f16, 0x170($k0)
    sdc1    $f18, 0x178($k0)
    sdc1    $f20, 0x180($k0)
    sdc1    $f22, 0x188($k0)
    sdc1    $f24, 0x190($k0)
    sdc1    $f26, 0x198($k0)
    sdc1    $f28, 0x1A0($k0)
    sdc1    $f30, 0x1A8($k0)
.Lsave_cause:
    mfc0    $t0, $13
    sw      $t0, 0x120($k0)
    addiu   $t1, $zero, 0x2
    sh      $t1, 0x10($k0)
    lui     $a0, %hi(D_8000A40C)
    lw      $a0, %lo(D_8000A40C)($a0)
    beqz    $a0, .Lrdb_check_event
     lui    $at, %hi(D_8000A40C)
    jal     func_80004DB8
     sw     $zero, %lo(D_8000A40C)($at)
    b       .LdispatchThread_restore
     nop
.Lrdb_check_event:
    lui     $a0, %hi(D_8000A408)
    lw      $a0, %lo(D_8000A408)($a0)
    beqz    $a0, .Lcheck_exccode
     lui    $at, %hi(D_8000A408)
    jal     func_80003C24
     sw     $zero, %lo(D_8000A408)($at)
    lw      $t0, 0x120($k0)
.Lcheck_exccode:
    andi    $t1, $t0, 0x7C
    addiu   $t2, $zero, 0x24
    beq     $t1, $t2, .Lhandle_break
     nop
    addiu   $t2, $zero, 0x2C
    beq     $t1, $t2, .Lhandle_CpU
     nop
    addiu   $t2, $zero, 0x0
    bne     $t1, $t2, .Lpanic
     nop
    and     $s0, $k1, $t0
    # handle_interrupt: dispatch the highest pending IP via __osIntOffTable /
    # __osIntTable (D_8000A700 / jtbl_8000A720)
.Lnext_interrupt:
    andi    $t1, $s0, 0xFF00
    srl     $t2, $t1, 12
    bnez    $t2, .Linterrupt_dispatch
     nop
    srl     $t2, $t1, 8
    addi    $t2, $t2, 0x10
.Linterrupt_dispatch:
    lui     $at, %hi(D_8000A700)
    addu    $at, $at, $t2
    lbu     $t2, %lo(D_8000A700)($at)
    lui     $at, %hi(jtbl_8000A720)
    addu    $at, $at, $t2
    lw      $t2, %lo(jtbl_8000A720)($at)
    jr      $t2
     nop
    addiu   $at, $zero, -0x2001
    b       .Lnext_interrupt
     and    $s0, $s0, $at
    addiu   $at, $zero, -0x4001
    b       .Lnext_interrupt
     and    $s0, $s0, $at
    mfc0    $t1, $11
    mtc0    $t1, $11
    jal     func_80003C24
     addiu  $a0, $zero, 0x18
    lui     $at, 0xFFFF
    ori     $at, $at, 0x7FFF
    b       .Lnext_interrupt
     and    $s0, $s0, $at
    addiu   $at, $zero, -0x801
    and     $s0, $s0, $at
    addiu   $t2, $zero, 0x4
    lui     $at, %hi(D_8000A3F0)
    addu    $at, $at, $t2
    lw      $t2, %lo(D_8000A3F0)($at)
    lui     $sp, %hi(D_80018300)
    addiu   $sp, $sp, %lo(D_80018300)
    addiu   $a0, $zero, 0x10
    beqz    $t2, .Lcart_send
     addiu  $sp, $sp, 0xFF0
    jalr    $t2
     nop
    beqz    $v0, .Lcart_send
     addiu  $a0, $zero, 0x10
    b       .Lredispatch
     nop
.Lcart_send:
    jal     func_80003C24
     nop
    b       .Lnext_interrupt
     nop
    lui     $t0, %hi(D_8000A3E0)
    addiu   $t0, $t0, %lo(D_8000A3E0)
    lw      $t0, 0x0($t0)
    lui     $s1, %hi(D_A4300008)
    lw      $s1, %lo(D_A4300008)($s1)
    srl     $t0, $t0, 16
    and     $s1, $s1, $t0
    andi    $t1, $s1, 0x1
    beqz    $t1, .Lvi
     nop
    lui     $t4, %hi(D_A4040010)
    lw      $t4, %lo(D_A4040010)($t4)
    ori     $t1, $zero, 0x8008
    lui     $at, %hi(D_A4040010)
    andi    $t4, $t4, 0x300
    andi    $s1, $s1, 0x3E
    beqz    $t4, .Lsp_other_break
     sw     $t1, %lo(D_A4040010)($at)
    jal     func_80003C24
     addiu  $a0, $zero, 0x20
    beqz    $s1, .LNoMoreRcpInts
     nop
    b       .Lvi
     nop
.Lsp_other_break:
    jal     func_80003C24
     addiu  $a0, $zero, 0x58
    beqz    $s1, .LNoMoreRcpInts
     nop
.Lvi:
    andi    $t1, $s1, 0x8
    beqz    $t1, .Lai
     lui    $at, %hi(D_A4400010)
    andi    $s1, $s1, 0x37
    sw      $zero, %lo(D_A4400010)($at)
    jal     func_80003C24
     addiu  $a0, $zero, 0x38
    beqz    $s1, .LNoMoreRcpInts
     nop
.Lai:
    andi    $t1, $s1, 0x4
    beqz    $t1, .Lsi
     nop
    addiu   $t1, $zero, 0x1
    lui     $at, %hi(D_A450000C)
    andi    $s1, $s1, 0x3B
    sw      $t1, %lo(D_A450000C)($at)
    jal     func_80003C24
     addiu  $a0, $zero, 0x30
    beqz    $s1, .LNoMoreRcpInts
     nop
.Lsi:
    andi    $t1, $s1, 0x2
    beqz    $t1, .Lpi
     lui    $at, %hi(D_A4800018)
    andi    $s1, $s1, 0x3D
    sw      $zero, %lo(D_A4800018)($at)
    jal     func_80003C24
     addiu  $a0, $zero, 0x28
    beqz    $s1, .LNoMoreRcpInts
     nop
.Lpi:
    andi    $t1, $s1, 0x10
    beqz    $t1, .Ldp
     nop
    addiu   $t1, $zero, 0x2
    lui     $at, %hi(D_A4600010)
    andi    $s1, $s1, 0x2F
    sw      $t1, %lo(D_A4600010)($at)
    jal     func_80003C24
     addiu  $a0, $zero, 0x40
    beqz    $s1, .LNoMoreRcpInts
     nop
.Ldp:
    andi    $t1, $s1, 0x20
    beqz    $t1, .LNoMoreRcpInts
     nop
    addiu   $t1, $zero, 0x800
    lui     $at, %hi(D_A4300000)
    andi    $s1, $s1, 0x1F
    sw      $t1, %lo(D_A4300000)($at)
    jal     func_80003C24
     addiu  $a0, $zero, 0x48
.LNoMoreRcpInts:
    addiu   $at, $zero, -0x401
    b       .Lnext_interrupt
     and    $s0, $s0, $at
    lw      $k1, 0x118($k0)
    addiu   $at, $zero, -0x1001
    lui     $t1, %hi(D_8000A3DC)
    and     $k1, $k1, $at
    sw      $k1, 0x118($k0)
    addiu   $t1, $t1, %lo(D_8000A3DC)
    lw      $t2, 0x0($t1)
    beqz    $t2, .Lfirstnmi
     addiu  $at, $zero, -0x1001
    b       .Lredispatch
     and    $s0, $s0, $at
.Lfirstnmi:
    addiu   $t2, $zero, 0x1
    sw      $t2, 0x0($t1)
    jal     func_80003C24
     addiu  $a0, $zero, 0x70
    lui     $t2, %hi(D_8000A418)
    lw      $t2, %lo(D_8000A418)($t2)
    addiu   $at, $zero, -0x1001
    and     $s0, $s0, $at
    lw      $k1, 0x118($t2)
    and     $k1, $k1, $at
    b       .Lredispatch
     sw     $k1, 0x118($t2)
    addiu   $at, $zero, -0x201
    and     $t0, $t0, $at
    mtc0    $t0, $13
    jal     func_80003C24
     addiu  $a0, $zero, 0x8
    addiu   $at, $zero, -0x201
    b       .Lnext_interrupt
     and    $s0, $s0, $at
    addiu   $at, $zero, -0x101
    and     $t0, $t0, $at
    mtc0    $t0, $13
    jal     func_80003C24
     addiu  $a0, $zero, 0x0
    addiu   $at, $zero, -0x101
    b       .Lnext_interrupt
     and    $s0, $s0, $at
.Lhandle_break:
    addiu   $t1, $zero, 0x1
    sh      $t1, 0x12($k0)
    jal     func_80003C24
     addiu  $a0, $zero, 0x50
    b       .Lredispatch
     nop
    # redispatch: requeue the interrupted thread if a higher priority one is
    # runnable (__osRunQueue = D_8000A418)
.Lredispatch:
    lui     $t2, %hi(D_8000A418)
    lw      $t2, %lo(D_8000A418)($t2)
    lw      $t1, 0x4($k0)
    lw      $t3, 0x4($t2)
    slt     $at, $t1, $t3
    beqz    $at, .LenqueueRunning
     nop
    lui     $a0, %hi(D_8000A418)
    or      $a1, $k0, $zero
    jal     func_80003E0C
     addiu  $a0, $a0, %lo(D_8000A418)
    j       func_80003E64
     nop
.LenqueueRunning:
    lui     $t1, %hi(D_8000A418)
    addiu   $t1, $t1, %lo(D_8000A418)
    lw      $t2, 0x0($t1)
    sw      $t2, 0x0($k0)
    j       func_80003E64
     sw     $k0, 0x0($t1)
.Lpanic:
    lui     $at, %hi(D_8000A424)
    sw      $k0, %lo(D_8000A424)($at)
    addiu   $t1, $zero, 0x1
    sh      $t1, 0x10($k0)
    addiu   $t1, $zero, 0x2
    sh      $t1, 0x12($k0)
    mfc0    $t2, $8
    sw      $t2, 0x124($k0)
    jal     func_80003C24
     addiu  $a0, $zero, 0x60
    j       func_80003E64
     nop
    # send_mesg (a0 = event * 8): post __osEventStateTab[event] (jal'd as
    # func_80003C24 = its runtime address; the kernel runs 0x400 above link)
    lui     $t2, %hi(__osEventStateTab)
    addiu   $t2, $t2, %lo(__osEventStateTab)
    addu    $t2, $t2, $a0
    lw      $t1, 0x0($t2)
    or      $s2, $ra, $zero
    beqz    $t1, .Lsend_done
     nop
    lw      $t3, 0x8($t1)
    lw      $t4, 0x10($t1)
    slt     $at, $t3, $t4
    beqz    $at, .Lsend_done
     nop
    lw      $t5, 0xC($t1)
    addu    $t5, $t5, $t3
    div     $zero, $t5, $t4
    bnez    $t4, .Lsend_mesg_divok
     nop
    break   7
.Lsend_mesg_divok:
    addiu   $at, $zero, -0x1
    bne     $t4, $at, .Lsend_mesg_store
     lui    $at, 0x8000
    bne     $t5, $at, .Lsend_mesg_store
     nop
    break   6
.Lsend_mesg_store:
    lw      $t4, 0x14($t1)
    mfhi    $t5
    sll     $t5, $t5, 2
    addu    $t4, $t4, $t5
    lw      $t5, 0x4($t2)
    addiu   $t2, $t3, 0x1
    sw      $t5, 0x0($t4)
    sw      $t2, 0x8($t1)
    lw      $t2, 0x0($t1)
    lw      $t3, 0x0($t2)
    beqz    $t3, .Lsend_done
     nop
    jal     func_80003E54
     or     $a0, $t1, $zero
    or      $t2, $v0, $zero
    lui     $a0, %hi(D_8000A418)
    or      $a1, $t2, $zero
    jal     func_80003E0C
     addiu  $a0, $a0, %lo(D_8000A418)
.Lsend_done:
    jr      $s2
     nop
    # handle_CpU: coprocessor-unusable -> mark the thread FPU-using, redispatch
.Lhandle_CpU:
    lui     $at, 0x3000
    and     $t1, $t0, $at
    srl     $t1, $t1, 28
    addiu   $t2, $zero, 0x1
    bne     $t1, $t2, .Lpanic
     nop
    lw      $k1, 0x118($k0)
    lui     $at, 0x2000
    addiu   $t1, $zero, 0x1
    or      $k1, $k1, $at
    sw      $t1, 0x18($k0)
    b       .LenqueueRunning
     sw     $k1, 0x118($k0)
    # __osEnqueueAndYield(a0 = queue): save the callee context and dispatch
    lui     $a1, %hi(D_8000A420)
    lw      $a1, %lo(D_8000A420)($a1)
    mfc0    $t0, $12
    lw      $k1, 0x18($a1)
    ori     $t0, $t0, 0x2
    sw      $t0, 0x118($a1)
    sd      $s0, 0x98($a1)
    sd      $s1, 0xA0($a1)
    sd      $s2, 0xA8($a1)
    sd      $s3, 0xB0($a1)
    sd      $s4, 0xB8($a1)
    sd      $s5, 0xC0($a1)
    sd      $s6, 0xC8($a1)
    sd      $s7, 0xD0($a1)
    sd      $gp, 0xE8($a1)
    sd      $sp, 0xF0($a1)
    sd      $fp, 0xF8($a1)
    sd      $ra, 0x100($a1)
    beqz    $k1, .Lyield_savefpu_done
     sw     $ra, 0x11C($a1)
    cfc1    $k1, $31
    sdc1    $f20, 0x180($a1)
    sdc1    $f22, 0x188($a1)
    sdc1    $f24, 0x190($a1)
    sdc1    $f26, 0x198($a1)
    sdc1    $f28, 0x1A0($a1)
    sdc1    $f30, 0x1A8($a1)
    sw      $k1, 0x12C($a1)
.Lyield_savefpu_done:
    lw      $k1, 0x118($a1)
    andi    $t1, $k1, 0xFF00
    beqz    $t1, .Lyield_savercp
     nop
    lui     $t0, %hi(D_8000A3E0)
    addiu   $t0, $t0, %lo(D_8000A3E0)
    lw      $t0, 0x0($t0)
    addiu   $at, $zero, -0x1
    xor     $t0, $t0, $at
    lui     $at, 0xFFFF
    andi    $t0, $t0, 0xFF00
    ori     $at, $at, 0xFF
    or      $t1, $t1, $t0
    and     $k1, $k1, $at
    or      $k1, $k1, $t1
    sw      $k1, 0x118($a1)
.Lyield_savercp:
    lui     $k1, %hi(D_A430000C)
    lw      $k1, %lo(D_A430000C)($k1)
    beqz    $k1, .Lyield_endrcp
     nop
    lui     $k0, %hi(D_8000A3E0)
    addiu   $k0, $k0, %lo(D_8000A3E0)
    lw      $k0, 0x0($k0)
    lw      $t0, 0x128($a1)
    addiu   $at, $zero, -0x1
    srl     $k0, $k0, 16
    xor     $k0, $k0, $at
    andi    $k0, $k0, 0x3F
    and     $k0, $k0, $t0
    or      $k1, $k1, $k0
.Lyield_endrcp:
    beqz    $a0, .LnoEnqueue
     sw     $k1, 0x128($a1)
    jal     func_80003E0C
     nop
.LnoEnqueue:
    j       func_80003E64
     nop
    # __osEnqueueThread(queue, thread): priority-ordered insert
    lw      $t8, 0x0($a0)
    lw      $t7, 0x4($a1)
    or      $t9, $a0, $zero
    lw      $t6, 0x4($t8)
    slt     $at, $t6, $t7
    bnez    $at, .LenqueueThread_insert
     nop
.LenqueueThread_loop:
    or      $t9, $t8, $zero
    lw      $t8, 0x0($t8)
    lw      $t6, 0x4($t8)
    slt     $at, $t6, $t7
    beqz    $at, .LenqueueThread_loop
     nop
.LenqueueThread_insert:
    lw      $t8, 0x0($t9)
    sw      $t8, 0x0($a1)
    sw      $a1, 0x0($t9)
    jr      $ra
     sw     $a0, 0x8($a1)
    # __osPopThread(queue)
    lw      $v0, 0x0($a0)
    lw      $t9, 0x0($v0)
    jr      $ra
     sw     $t9, 0x0($a0)
    # __osDispatchThread: pop the run queue and restore its context
    lui     $a0, %hi(D_8000A418)
    jal     func_80003E54
     addiu  $a0, $a0, %lo(D_8000A418)
    lui     $at, %hi(D_8000A420)
    sw      $v0, %lo(D_8000A420)($at)
    addiu   $t0, $zero, 0x4
    sh      $t0, 0x10($v0)
    or      $k0, $v0, $zero
.LdispatchThread_restore:
    lui     $t0, %hi(D_8000A3E0)
    lw      $k1, 0x118($k0)
    addiu   $t0, $t0, %lo(D_8000A3E0)
    lw      $t0, 0x0($t0)
    lui     $at, 0xFFFF
    andi    $t1, $k1, 0xFF00
    ori     $at, $at, 0xFF
    andi    $t0, $t0, 0xFF00
    and     $t1, $t1, $t0
    and     $k1, $k1, $at
    or      $k1, $k1, $t1
    mtc0    $k1, $12
    ld      $k1, 0x108($k0)
    ld      $at, 0x20($k0)
    ld      $v0, 0x28($k0)
    mtlo    $k1
    ld      $k1, 0x110($k0)
    ld      $v1, 0x30($k0)
    ld      $a0, 0x38($k0)
    ld      $a1, 0x40($k0)
    ld      $a2, 0x48($k0)
    ld      $a3, 0x50($k0)
    ld      $t0, 0x58($k0)
    ld      $t1, 0x60($k0)
    ld      $t2, 0x68($k0)
    ld      $t3, 0x70($k0)
    ld      $t4, 0x78($k0)
    ld      $t5, 0x80($k0)
    ld      $t6, 0x88($k0)
    ld      $t7, 0x90($k0)
    ld      $s0, 0x98($k0)
    ld      $s1, 0xA0($k0)
    ld      $s2, 0xA8($k0)
    ld      $s3, 0xB0($k0)
    ld      $s4, 0xB8($k0)
    ld      $s5, 0xC0($k0)
    ld      $s6, 0xC8($k0)
    ld      $s7, 0xD0($k0)
    ld      $t8, 0xD8($k0)
    ld      $t9, 0xE0($k0)
    ld      $gp, 0xE8($k0)
    mthi    $k1
    ld      $sp, 0xF0($k0)
    ld      $fp, 0xF8($k0)
    ld      $ra, 0x100($k0)
    lw      $k1, 0x11C($k0)
    mtc0    $k1, $14
    lw      $k1, 0x18($k0)
    beqz    $k1, .LdispatchThread_nofpu
     nop
    lw      $k1, 0x12C($k0)
    ctc1    $k1, $31
    ldc1    $f0, 0x130($k0)
    ldc1    $f2, 0x138($k0)
    ldc1    $f4, 0x140($k0)
    ldc1    $f6, 0x148($k0)
    ldc1    $f8, 0x150($k0)
    ldc1    $f10, 0x158($k0)
    ldc1    $f12, 0x160($k0)
    ldc1    $f14, 0x168($k0)
    ldc1    $f16, 0x170($k0)
    ldc1    $f18, 0x178($k0)
    ldc1    $f20, 0x180($k0)
    ldc1    $f22, 0x188($k0)
    ldc1    $f24, 0x190($k0)
    ldc1    $f26, 0x198($k0)
    ldc1    $f28, 0x1A0($k0)
    ldc1    $f30, 0x1A8($k0)
.LdispatchThread_nofpu:
    lw      $k1, 0x128($k0)
    lui     $k0, %hi(D_8000A3E0)
    addiu   $k0, $k0, %lo(D_8000A3E0)
    lw      $k0, 0x0($k0)
    srl     $k0, $k0, 16
    and     $k1, $k1, $k0
    sll     $k1, $k1, 1
    lui     $k0, %hi(D_8000A680)
    addiu   $k0, $k0, %lo(D_8000A680)
    addu    $k1, $k1, $k0
    lhu     $k1, 0x0($k1)
    lui     $k0, %hi(D_A430000C)
    addiu   $k0, $k0, %lo(D_A430000C)
    sw      $k1, 0x0($k0)
    nop                                 # object alignment padding
    nop
    nop                                 # object alignment padding
    nop
    eret
    # __osCleanupThread: osDestroyThread(NULL)
    jal     func_80004030
     or     $a0, $zero, $zero
endlabel func_800031F0
    nop                                 # object alignment padding
    nop
