/* -O1 donor for gl_func_0006A420 = libultra osSpTaskLoad (sptask.c), spliced
 * over the -O2 stand-in in game_libs_post1b.c (same recipe as the 6A304
 * donor just before it). The first call is the file-static
 * _VirtualToPhysicalTask, a baked in-unit jal 0x0C01FA5C (gl_ref_0007E970);
 * the rest are blank USO imports. OSTask offsets: flags 0x4, ucode_boot 0x8,
 * ucode_boot_size 0xC, ucode 0x10, ucode_data 0x18, ucode_data_size 0x1C,
 * yield_data_ptr 0x38, yield_data_size 0x3C. */
extern int gl_func_00000000();
extern char *gl_ref_0007E970();

typedef struct {
    unsigned int type, flags;
    char *ucode_boot;
    unsigned int ucode_boot_size;
    char *ucode;
    unsigned int ucode_size;
    char *ucode_data;
    unsigned int ucode_data_size;
    char *dram_stack;
    unsigned int dram_stack_size;
    char *output_buff;
    char *output_buff_size;
    char *data_ptr;
    unsigned int data_size;
    char *yield_data_ptr;
    unsigned int yield_data_size;
} OSTask_6A420;

void gl_func_0006A420(OSTask_6A420 *intp) {
    OSTask_6A420 *tp;
    tp = (OSTask_6A420 *)gl_ref_0007E970(intp);
    if (tp->flags & 1) {
        tp->ucode_data = tp->yield_data_ptr;
        tp->ucode_data_size = tp->yield_data_size;
        intp->flags &= ~1;
        if (tp->flags & 4) {
            tp->ucode = (char *)*(volatile unsigned int *)(((unsigned int)intp->yield_data_ptr + 0xC00 - 4) | 0xA0000000);
        }
    }
    gl_func_00000000(tp, sizeof(OSTask_6A420));
    gl_func_00000000(0x2B00);
    while (gl_func_00000000(0x04001000) == -1)
        ;
    while (gl_func_00000000(1, 0x04001000 - sizeof(*tp), tp, sizeof(OSTask_6A420)) == -1)
        ;
    while (gl_func_00000000())
        ;
    while (gl_func_00000000(1, 0x04001000, tp->ucode_boot, tp->ucode_boot_size) == -1)
        ;
}
