/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022c8aa0();
s32 func_02051db0();
void func_ov001_022ccbe0();

void func_ov001_022cdca4(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    *(u32 *)((u8 *)t0 + 0x20) = 0;
    *(u32 *)*(u32 *)((u8 *)t0 + 0x3ac) = 8;
    func_ov001_022c8aa0(0, -16, 8, 8);
    func_02051db0(2, 8, 0x5000, 0x1000);
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022ccbe0;
}

#pragma thumb reset
