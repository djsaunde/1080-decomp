/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
void game_uso_func_00007448(char *a0) {
    char *snapshot_slot;
    char *linked;
    char *replacement_slot;
    char *factor_slot;
    char *write_slot;
    snapshot_slot = a0;
    while (0) { snapshot_slot += 1; }
    linked = *(char**)(a0 + 0x30);
    write_slot = a0;
    while (0) { write_slot += 1; }
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x758; }
    *(float*)(a0 + 0x4C8) = *(float*)(snapshot_slot + 0x10);
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x6F8; }
    *(float*)(a0 + 0x4CC) = *(float*)(snapshot_slot + 0x10);
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x6E0; }
    *(float*)(a0 + 0x4D0) = *(float*)(snapshot_slot + 0x10);
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x498; }
    replacement_slot = write_slot;
    factor_slot = a0;
    if (1) { factor_slot += 0x35C; }
    *(float*)(a0 + 0x4D4) = *(float*)(snapshot_slot + 0x10);
    *(float*)(linked + 0x768) = *(float*)(a0 + 0x33C) * *(float*)(a0 + 0x4C8);
    write_slot = *(char**)(a0 + 0x30) + 0x6F8;
    *(float*)(write_slot + 0x10) = *(float*)(a0 + 0x36C) * *(float*)(a0 + 0x4CC);
    if (replacement_slot) {}
    write_slot = *(char**)(a0 + 0x30) + 0x6E0;
    *(float*)(write_slot + 0x10) = *(float*)(factor_slot + 0x10) * *(float*)(a0 + 0x4D0);
    linked = *(char**)(a0 + 0x30);
    replacement_slot = linked;
    if (1) { replacement_slot += 0x4C8; }
    write_slot = linked;
    if (1) { write_slot += 0x498; }
    *(float*)(write_slot + 0x10) = *(float*)(replacement_slot + 0x10);
}
