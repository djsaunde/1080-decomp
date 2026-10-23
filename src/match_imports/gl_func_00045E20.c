/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00045E20(void *a0, GL470E4Object *a1, GLRenderNode *a2) {
    unsigned int m = a2->field38;
    int nodeIndex;
    int packetOffset;
    int hi, lo;
    unsigned int enabledScale = 1984;
    if ((int)(m << 0xB) < 0) {
        lo = enabledScale;
        if ((int)(m << 0x7) < 0) {
            { GL470E4List *d_ = (a1)->list; int i_ = d_->count; d_->count = i_ + 1; { unsigned int *p_ = (unsigned int *)((char *)(((a1)->list)->packets) + i_ * 8); p_[0] = (0xB7000000); p_[1] = (0x000C0000); } };
        } else {
            { GL470E4List *d_ = (a1)->list; int i_ = d_->count; d_->count = i_ + 1; { unsigned int *p_ = (unsigned int *)((char *)(((a1)->list)->packets) + i_ * 8); p_[0] = (0xB7000000); p_[1] = (0x00040000); } };
        }
        a2->field28 |= 0x1100;
        if (a2->field20 == 64) {
            hi = 3968;
        } else {
            hi = 1984;
            lo = 1984;
        }
    } else {
        { GL470E4List *d_ = (a1)->list; int i_ = d_->count; d_->count = i_ + 1; { unsigned int *p_ = (unsigned int *)((char *)(((a1)->list)->packets) + i_ * 8); p_[0] = (0xB6000000); p_[1] = (0x00040000); } };
        lo = 0xFFFF;
        hi = 0xFFFF;
    }
    { GL470E4List *d_ = (a1)->list; int i_ = d_->count; d_->count = i_ + 1; { unsigned int *p_ = (unsigned int *)((char *)(((a1)->list)->packets) + i_ * 8); p_[0] = (0xBB040801); p_[1] = ((((unsigned int)hi & 0xFFFF) << 16) | (lo & 0xFFFF)); } };
    nodeIndex = 0;
    packetOffset = 0;
    {
        GLRenderNode *node = a2;
        if (node != 0) {
            do {
                int v;
                int index;
                do { index = nodeIndex++; } while (0); node->field3C = index;
                gl_45E20_emit_node(a1, node, packetOffset);
                v = gl_45E20_node_bytes(node);
                node = node->field40;
                packetOffset += v / 8;
            } while (node != 0);
        }
    }
    {
        int syncIndex = *(int *)((int)a1->list + 4);
        int tail = (int)a1->list;
        unsigned int *syncCmd;
        ((GL470E4List *)tail)->count = syncIndex + 1;
        syncCmd = (unsigned int *)(*(int *)a1->list + syncIndex * 8);
        syncCmd[0] = 0xE7000000;
        syncCmd[1] = 0;
    }
}
