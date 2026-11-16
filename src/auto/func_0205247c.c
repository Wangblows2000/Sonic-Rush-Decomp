/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4560[];
s32 func_0204e6b0();
s32 func_0204e6ac();
s32 func_0204ec40();
s32 func_02064870();
s32 SceneChange_Unk_02055178();
s32 func_0204888c();

void func_0205247c(s32 p0, s32 p1) {
    *(u32 *)((u8 *)data_022c4560 + 0x10) = 0;
    s32 t0 = (*(u32 *)((u8 *)data_022c4560 + 0x18) & ~0x20);
    *(u32 *)((u8 *)data_022c4560 + 0x18) = t0;
    *(u32 *)((u8 *)data_022c4560 + 0x18) = (t0 & ~0x10);
    s32 t1 = func_0204e6b0(((u32)((s32)p0 << 0x18) >> 0x18), 0x10, data_022c4560, (t0 & ~0x10));
    *(u8 *)((u8 *)data_022c4560 + 0x1c) = t1;
    s32 t2 = func_0204e6ac(((u32)((s32)p1 << 0x18) >> 0x18), data_022c4560);
    *(u8 *)((u8 *)data_022c4560 + 0x1d) = t2;
    func_0204ec40(t2, data_022c4560);
    func_02064870();
    s32 t3 = *(u8 *)((u8 *)data_022c4560 + 0x1d);
    if ((u32)t3 < (u32)2) {
        SceneChange_Unk_02055178(t3);
        return;
    }
    func_0204888c();
}

#pragma thumb reset
