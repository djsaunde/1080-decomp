/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
char *game_uso_func_000041C0(char *owner) {
    char *result;
    union { char *allocated; char *nodes[1]; } carrier;
    char *embedded;
    One1 slot0, slot1, slot2;
    int pad_mid[1];
    char *header;
    char *allocation;
    char *child;
    One1 marsh;
    char *node;
    allocation = (char *)game_uso_func_055750(0x8C);
    carrier.allocated = (char *)allocation;
    if (carrier.allocated != 0) {
        node = 0;
        uso_base_init_051C28((SnwBase3D5BC *)((int)allocation | (int)node));
        ((SnwBase3D5BC *)carrier.nodes[0])->field_28 = &game_uso_D_807FEB68;
        embedded = carrier.allocated + 0x3C;
        if (embedded != 0 || (embedded = (char *)game_uso_func_055750(0x50)) != 0) {
            header = embedded;
            if (header != 0 || (header = (char *)game_uso_func_055750(8)) != 0) {
                *(int *)header = (int)((char *)&game_uso_D_807FECA0 + 0x6B0);
                *(int *)(header + 4) = 0;
            }
            slot0.a = *(int *)((char *)&game_uso_D_807FECB4 + 0x6C4);
            marsh = slot0;
            child = embedded + 8;
            if (child != 0 || (child = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(child, embedded, marsh, 1);
                *(int *)(child + 0xC) = (int)((char *)&game_uso_D_807FE9B8 + 0x3C8);
                *(int *)(child + 0x14) = 0;
                *(float *)(child + 0x10) = 1.0f;
            }
            slot1.a = *(int *)((char *)&game_uso_D_807FECB8 + 0x6C8);
            marsh = slot1;
            child = embedded + 0x20;
            if (child != 0 || (child = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(child, embedded, marsh, 1);
                *(int *)(child + 0xC) = (int)((char *)&game_uso_D_807FEA00 + 0x410);
                *(int *)(child + 0x10) = 0;
                *(int *)(child + 0x14) = 0;
            }
            slot2.a = *(int *)((char *)&game_uso_D_807FECBC + 0x6CC);
            marsh = slot2;
            child = embedded + 0x38;
            if (child != 0 || (child = (char *)game_uso_func_055750(0x18)) != 0) {
                game_uso_func_04A188(child, embedded, marsh, 0);
                *(int *)(child + 0xC) = (int)((char *)&game_uso_D_41C0_last + 0x410);
                *(int *)(child + 0x14) = 0;
                *(int *)(child + 0x10) = 0;
            }
        }
    }
    result = carrier.allocated;
    node = *(char **)(owner + 0x40);
    if (node != 0) {
        result = carrier.allocated;
        carrier.nodes[0] = node;
        game_uso_func_07ACE0(result + 0x10, carrier.nodes[0]);
        if (*(int *)(carrier.nodes[0] + 0x14) != 0) *(int *)(carrier.nodes[0] + 4) = 1;
        *(int *)(carrier.nodes[0] + 0x14) = (int)result;
    }
    return result;
}
