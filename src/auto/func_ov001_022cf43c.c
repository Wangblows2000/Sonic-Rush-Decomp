/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4560[];
extern u8 data_022c4680[];
s32 func_ov001_022ceef4();
s32 func_0203a590();
void func_ov001_022cf2a8();
s32 func_020372d0();
extern u8 data_022c4da0[];
s32 func_02060f40();
s32 func_ov001_022cf4a0();

void func_ov001_022cf43c(void) {
    if (*(u32 *)data_022c4560 == 0) {
        s32 t0 = (*(u8 *)((u8 *)data_022c4680 + 0xf) & 0x7f);
        if (t0 != 0x7f) {
            func_ov001_022ceef4(0x7f, t0);
            func_0203a590(0);
            func_020372d0((u32)func_ov001_022cf2a8, 0, 0, 0x1000, 0, 4);
            *(u16 *)data_022c4da0 = 0x37;
            func_02060f40(data_022c4da0, 0x37);
            return;
        }
    }
    func_ov001_022cf4a0(0);
}

#pragma thumb reset
