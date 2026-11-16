/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0207c470[];
extern u8 data_020768b4[];
s32 func_0202c474();
extern u8 data_0207c478[];

void func_02057044(s32 p0, u8 *p1) {
    *(u8 *)p1 = *(u8 *)data_0207c470;
    *(u8 *)((u8 *)p1 + 0x1) = *(u8 *)((u8 *)data_0207c470 + 0x1);
    *(u8 *)((u8 *)p1 + 0x2) = *(u8 *)((u8 *)data_0207c470 + 0x2);
    *(u8 *)((u8 *)p1 + 0x3) = *(u8 *)((u8 *)data_0207c470 + 0x3);
    *(u8 *)((u8 *)p1 + 0x4) = *(u8 *)((u8 *)data_0207c470 + 0x4);
    *(u8 *)((u8 *)p1 + 0x5) = *(u8 *)((u8 *)data_0207c470 + 0x5);
    func_0202c474(p1, *(u32 *)((u8 *)data_020768b4 + ((s32)p0 << 3)));
    func_0202c474(p1, data_0207c478);
}

#pragma thumb reset
