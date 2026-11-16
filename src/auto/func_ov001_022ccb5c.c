/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022cc17c();
s32 func_ov001_022cbf30();
void func_ov001_022cca00();

void func_ov001_022ccb5c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x1ec);
    *(u32 *)((u8 *)t1 + 0x10) = 0;
    *(u32 *)((u8 *)t1 + 0x14) = 0;
    *(u32 *)((u8 *)t1 + 0x8) = 0;
    func_ov001_022cc17c(t0);
    func_ov001_022cbf30(t0);
    *(u32 *)((u8 *)t0 + 0x1ec) = 0;
    *(u32 *)((u8 *)t0 + 0x14) = 0x64;
    *(u32 *)((u8 *)t0 + 0x18) = 0;
    *(u32 *)((u8 *)t1 + 0x4) = 1;
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022cca00;
}

#pragma thumb reset
