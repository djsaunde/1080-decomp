# __osExceptionPreamble (second half), a second vector stub, and the rdb/KMC
# front end of __osException -- handwritten assembly.
# Variant of libultra os/exceptasm.s from the development (rdb) libultra
# linked into this kernel, with KMC partner-N64 debugger hooks (__kmc_pt_mode,
# the monitor at 0xBFF00000) and RDB host-packet handling (IP7, packets at
# 0xC0000000; rdb.h RDB_TYPE_HtoG_* types). The register-save layout and the
# __osException core follow references/libreultra/src/os/exceptasm.s; the
# rdb/KMC front end has no public reference, so it is written from the
# ROM. Handwritten: k0/k1, eret, mfc0/mtc0, sd/ld (gp=64) and addi -- none of
# which IDO emits for C.
# Labels defined in func_800031F0 (still INCLUDE_ASM: its tail is the compiled
# C __osDequeueThread, so it needs a boundary split first) keep their
# address names (.Lrdb_debug_done, .Lrdb_check_ip6, .Lrdb_send_wait, .Lrdb_restore_eret = restore
# scratch + eret, .Lsavecontext = savecontext).

glabel func_80002E78
    jr      $k0
     nop
    sw      $k0, -0x10($sp)
    b       .Lexc_vec2_jump
     lui    $k0, %hi(.Lrdb_send_wait)
    nop
    nop
.Lexc_vec2_jump:
    addiu   $k0, $k0, %lo(.Lrdb_send_wait)
    jr      $k0
     nop
    mfc0    $k0, $13
    andi    $k0, $k0, 0x7C
    bnez    $k0, .Lexc_kmc_lookup
     srl    $k0, $k0, 2
    mfc0    $k0, $13
    nop
    andi    $k0, $k0, 0x4000
    beqz    $k0, .Lexc_save_scratch
     nop
    j       .Lsavecontext
     nop
    # nonzero ExcCode: is it one the KMC debugger (partner-N64 ROM monitor) owns?
.Lexc_kmc_lookup:
    sw      $k1, -0x8($sp)
    lui     $k1, %hi(D_8000A744)
    addiu   $k1, $k1, %lo(D_8000A744)
    add     $k1, $k1, $k0
    lbu     $k0, 0x0($k1)
    beqz    $k0, .Lexc_save_scratch
     nop
    # wait for the PI to go idle, then ask the monitor whether it is attached
.Lexc_kmc_wait_pi:
    lui     $k0, %hi(D_A4600010)
    lw      $k0, %lo(D_A4600010)($k0)
    nop
    andi    $k0, $k0, 0x3
    bnez    $k0, .Lexc_kmc_wait_pi
     nop
    lui     $k0, %hi(D_BFF08004)
    lw      $k0, %lo(D_BFF08004)($k0)
    nop
    andi    $k0, $k0, 0x8
    bnez    $k0, .Lexc_kmc_resume
     lui    $k0, 0xBFF0
    ori     $k0, $k0, 0x14
    lw      $k1, -0x8($sp)
    jr      $k0
     addiu  $sp, $sp, -0x60
    # monitor attached: hand it EPC and return from the exception
.Lexc_kmc_resume:
    mfc0    $k0, $14
    lui     $k1, %hi(D_BFF08000)
    sw      $k0, %lo(D_BFF08000)($k1)
    lw      $k1, -0x8($sp)
    lw      $k0, -0x10($sp)
    eret
    nop
    # __osException proper: save at/t0-t2 and SR in the thread save area
    # (D_80019320), disable interrupts, then look at Cause
.Lexc_save_scratch:
    lui     $k0, %hi(D_80019320)
    addiu   $k0, $k0, %lo(D_80019320)
    sd      $at, 0x20($k0)
    mfc0    $k1, $12
    sw      $k1, 0x118($k0)
    addiu   $at, $zero, -0x4
    and     $k1, $k1, $at
    mtc0    $k1, $12
    sd      $t0, 0x58($k0)
    sd      $t1, 0x60($k0)
    sd      $t2, 0x68($k0)
    sw      $zero, 0x18($k0)
    mfc0    $t0, $13
alabel func_80002F78
    lui     $t2, %hi(__kmc_pt_mode)
    lw      $t2, %lo(__kmc_pt_mode)($t2)
    bnez    $t2, .Lsavecontext
     nop
    andi    $t1, $t0, 0x7C
    addiu   $t2, $zero, 0x0
    bne     $t1, $t2, .Lsavecontext
     nop
    and     $t1, $k1, $t0
    andi    $t2, $t1, 0x4000
    beqz    $t2, .Lrdb_check_ip6
     nop
    lui     $t1, %hi(D_C0000008)
    addiu   $t1, $t1, %lo(D_C0000008)
    sw      $zero, 0x0($t1)
    # IP7 = RDB packet from the host: ack it and decode the type (word >> 26)
.Lrdb_wait_ip7:
    mfc0    $t0, $13
    andi    $t0, $t0, 0x4000
    bne     $zero, $t0, .Lrdb_wait_ip7
     nop
    lui     $t2, 0xC000
    lw      $t0, 0x0($t2)
    addiu   $t2, $t2, %lo(D_C0000000)
    addiu   $t2, $zero, 0x10
    srl     $t1, $t0, 26
    andi    $t1, $t1, 0x3F
    beq     $t1, $t2, .Lrdb_data
     nop
    addiu   $t2, $zero, 0xE
    beq     $t1, $t2, .Lrdb_debug
     nop
    addiu   $t2, $zero, 0x14
    beq     $t1, $t2, .Lrdb_kdebug
     nop
    addiu   $t2, $zero, 0xF
    beq     $t1, $t2, .Lrdb_debug_ct
     nop
    addiu   $t2, $zero, 0x11
    beq     $t1, $t2, .Lrdb_data_done
     nop
    addiu   $t2, $zero, 0xD
    beq     $t1, $t2, .Lrdb_log_done
     nop
    addiu   $t2, $zero, 0x12
    beq     $t1, $t2, .Lrdb_req_ramrom
     nop
    addiu   $t2, $zero, 0x13
    beq     $t1, $t2, .Lrdb_free_ramrom
     nop
    addiu   $t2, $zero, 0x15
    beq     $t1, $t2, .Lrdb_prof_signal
     nop
    b       .Lrdb_restore_eret
     nop
    # RDB_TYPE_HtoG_PROF_SIGNAL: queue OS_EVENT_RDB_FLUSH_PROF or _ACK_PROF
.Lrdb_prof_signal:
    srl     $t1, $t0, 16
    addiu   $t2, $zero, 0x1
    andi    $t1, $t1, 0xFF
    beq     $t1, $t2, .Lrdb_prof_ack
     nop
    addiu   $t2, $zero, 0xA8
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
.Lrdb_prof_ack:
    addiu   $t2, $zero, 0xB0
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
    # RDB_TYPE_HtoG_KDEBUG: remember the packet for the kdebug handler
.Lrdb_kdebug:
    lui     $at, %hi(D_8000A40C)
    b       .Lsavecontext
     sw     $t0, %lo(D_8000A40C)($at)
    # RDB_TYPE_HtoG_DEBUG_CT: byte count for the next debug read
.Lrdb_debug_ct:
    lui     $t2, 0xFF
    ori     $t2, $t2, 0xFFFF
    and     $t1, $t0, $t2
    lui     $at, %hi(D_8000A404)
    b       .Lrdb_restore_eret
     sw     $t1, %lo(D_8000A404)($at)
    # RDB_TYPE_HtoG_DATA_DONE: queue OS_EVENT_RDB_DATA_DONE (17 * 8)
.Lrdb_data_done:
    addiu   $t2, $zero, 0x88
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
    # RDB_TYPE_HtoG_LOG_DONE: queue OS_EVENT_RDB_LOG_DONE (16 * 8)
.Lrdb_log_done:
    addiu   $t2, $zero, 0x80
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
    # RDB_TYPE_HtoG_REQ_RAMROM: queue OS_EVENT_RDB_REQ_RAMROM (18 * 8)
.Lrdb_req_ramrom:
    addiu   $t2, $zero, 0x90
    lui     $at, 0x8001
    b       .Lsavecontext
alabel func_800030D0
    sw      $t2, -0x5BF8($at)
    # RDB_TYPE_HtoG_FREE_RAMROM: queue OS_EVENT_RDB_FREE_RAMROM (19 * 8)
.Lrdb_free_ramrom:
    addiu   $t2, $zero, 0x98
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
    # RDB_TYPE_HtoG_DATA: up to 3 payload bytes into __osRdb_Read_Data_Buf
.Lrdb_data:
    srl     $t1, $t0, 24
    andi    $t1, $t1, 0x3
    beq     $zero, $t1, .Lrdb_restore_eret
     nop
    lui     $t2, %hi(__osRdb_Read_Data_Ct)
    lw      $t2, %lo(__osRdb_Read_Data_Ct)($t2)
    lui     $at, %hi(__osRdb_Read_Data_Ct)
    subu    $t2, $t2, $t1
    sw      $t2, %lo(__osRdb_Read_Data_Ct)($at)
    sd      $t3, 0x70($k0)
    lui     $t3, %hi(__osRdb_Read_Data_Buf)
    lw      $t3, %lo(__osRdb_Read_Data_Buf)($t3)
    srl     $t2, $t0, 16
    andi    $t2, $t2, 0xFF
    addi    $t1, $t1, -0x1
    sb      $t2, 0x0($t3)
    beq     $zero, $t1, .Lrdb_data_stored
     addi   $t3, $t3, 0x1
    srl     $t2, $t0, 8
    andi    $t2, $t2, 0xFF
    addi    $t1, $t1, -0x1
    sb      $t2, 0x0($t3)
    beq     $zero, $t1, .Lrdb_data_stored
     addi   $t3, $t3, 0x1
    andi    $t0, $t0, 0xFF
    sb      $t0, 0x0($t3)
    addi    $t3, $t3, 0x1
.Lrdb_data_stored:
    lui     $t2, %hi(__osRdb_Read_Data_Ct)
    lw      $t2, %lo(__osRdb_Read_Data_Ct)($t2)
    lui     $at, %hi(__osRdb_Read_Data_Buf)
    sw      $t3, %lo(__osRdb_Read_Data_Buf)($at)
    bne     $zero, $t2, .Lrdb_restore_eret
     ld     $t3, 0x70($k0)
    addiu   $t2, $zero, 0x78
    lui     $at, %hi(D_8000A408)
    b       .Lsavecontext
     sw     $t2, %lo(D_8000A408)($at)
    # RDB_TYPE_HtoG_DEBUG: up to 3 payload bytes into __osRdb_DbgRead_Buf
.Lrdb_debug:
    srl     $t1, $t0, 24
    andi    $t1, $t1, 0x3
    beq     $zero, $t1, .Lrdb_restore_eret
     nop
    lui     $t2, %hi(D_8000A404)
    lw      $t2, %lo(D_8000A404)($t2)
    lui     $at, %hi(D_8000A404)
    subu    $t2, $t2, $t1
    sw      $t2, %lo(D_8000A404)($at)
    sd      $t3, 0x70($k0)
    lui     $t3, %hi(__osRdb_DbgRead_Buf)
    lw      $t3, %lo(__osRdb_DbgRead_Buf)($t3)
    bne     $zero, $t3, .Lrdb_debug_store
     nop
    b       .Lrdb_restore_eret
     ld     $t3, 0x70($k0)
.Lrdb_debug_store:
    srl     $t2, $t0, 16
    andi    $t2, $t2, 0xFF
    addi    $t1, $t1, -0x1
    sb      $t2, 0x0($t3)
    beq     $zero, $t1, .Lrdb_debug_done
     addi   $t3, $t3, 0x1
endlabel func_80002E78

