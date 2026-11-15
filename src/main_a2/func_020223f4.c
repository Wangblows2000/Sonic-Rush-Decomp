#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_020223f4;

extern UnkGlb_020223f4 data_02084d38;
extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084db4[];

void func_02025268(u32 op, const void *args, u32 n);

void func_020223f4(void) {
    u32 arg;
    func_02025268(0x1610, data_02084d3c, 0x11);
    func_02025268(0x19, data_02084d84, 0xC);
    arg = 2;
    func_02025268(0x10, &arg, 1);
    func_02025268(0x15, data_02084db4, 0x20);
    data_02084d38.flag &= ~1;
    data_02084d38.flag |= 2;
}
