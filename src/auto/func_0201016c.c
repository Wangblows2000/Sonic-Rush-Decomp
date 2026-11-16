/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207f2c0[];
s32 func_02007ab8();
s32 func_02005348();
s32 OS_ResetRequestIrqMask();
s32 func_020102d8();
s32 func_020101e0();

void func_0201016c(void) {
    func_02007ab8(*(u32 *)((u8 *)data_0207f2c0 + 0x24));
    s32 t0 = (*(u32 *)((u8 *)data_0207f2c0 + 0x18) + 0x200);
    s32 t1 = (*(u32 *)((u8 *)data_0207f2c0 + 0x1c) + 0x200);
    s32 t2 = (*(u32 *)((u8 *)data_0207f2c0 + 0x20) - 0x200);
    *(u32 *)((u8 *)data_0207f2c0 + 0x18) = t0;
    *(u32 *)((u8 *)data_0207f2c0 + 0x1c) = t1;
    *(u32 *)((u8 *)data_0207f2c0 + 0x20) = t2;
    if (t2 == 0) {
        func_02005348(0x80000, t2, t1, t0);
        OS_ResetRequestIrqMask(0x80000);
        func_020102d8();
        return;
    }
    func_020101e0();
}
