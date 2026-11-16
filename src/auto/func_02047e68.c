/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_02045c28();
s32 func_02047274();
s32 func_020470f8();
s32 func_02046e08();
s32 func_02046938();
extern u8 data_022c4560[];
s32 func_02045528();
s32 func_020461b4();
s32 func_02045d64();

void func_02047e68(s32 p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x344) | 0x500000);
    *(u32 *)((u8 *)p0 + 0x344) = t0;
    *(u32 *)((u8 *)p0 + 0x340) = (u32)func_02045c28;
    func_02047274(p0, 0x340, (u32)func_02045c28, t0);
    func_020470f8(p0);
    func_02046e08(p0);
    func_02046938(p0);
    if (*(u32 *)((u8 *)data_022c4560 + 0x10) == 2) {
        func_02045528(p0);
    } else {
        func_020461b4(p0);
    }
    func_02045d64(p0);
}

#pragma thumb reset
