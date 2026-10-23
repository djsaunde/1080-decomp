/* Native original-compiler source port from src/gui_uso/gui_uso.c. Target bytes unchanged. */
#include "context_gui_uso.h"
void gui_func_00002BB0(GuiTextureObject *object, int x, int y, int source_x, int count) {
    short raw_height;
    int draw_x, src_x, tile_width, remaining, width; unsigned int step_x, step_y;
    int *scratch, *info, *metadata_copy;
    scratch = *(int **)&D_00000000;
    {
        struct GfxRing_413C *ring = (struct GfxRing_413C *)scratch[0xC / 4];
        int index = ring->idx;
        ring->idx = index + 1;
        {
            int *packet = ((struct GfxRing_413C *)scratch[0xC / 4])->buf + index * 2;
            packet[0] = 0xBB000001; packet[1] = (int)0x80008000;
        }
    }
    draw_x = x;
    src_x = source_x;
    scratch = (int *)object->field_10;
    tile_width = 32;
    step_x = count;
    remaining = count;
    width = ((GuiTextureInfo *)scratch)->field_20;
    {
        int height;
        raw_height = ((GuiTextureInfo *)scratch)->field_22;
        height = raw_height;
        if (step_x == 0) return;
        step_y = (height << 10) / height;
        do {
            info = (int *)object->field_10;
            if (remaining >= 33) {
                step_x = (tile_width << 10) / tile_width;
                gui_import_2CEC(*(int **)&D_00000000, scratch = ((GuiTextureInfo *)(metadata_copy = info, info = 0, metadata_copy))->field_08, width, height, src_x, 0, tile_width, height, 0);
                gui_import_2D0C(*(int **)&D_00000000, draw_x, y, tile_width, height, step_x, step_y);
                draw_x += 32;
                src_x += 32;
                remaining -= 32;
            } else {
                int last_step_x;
                scratch = ((GuiTextureInfo *)info)->field_08;
                tile_width = remaining;
                gui_import_2D4C(*(int **)&D_00000000, scratch, width, height, src_x, 0, remaining, height, 0);
                last_step_x = (remaining << 10) / remaining;
                gui_import_2DA0(*(int **)&D_00000000, draw_x, y, remaining, height, last_step_x, step_y);
                remaining = 0;
            }
        } while (remaining != 0);
        if (src_x) {}
    }
}
