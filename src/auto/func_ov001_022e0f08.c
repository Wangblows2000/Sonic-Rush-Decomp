/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022e0dfc();
s32 func_ov001_022e0d30();
s32 func_ov001_022e0968();
s32 func_ov001_022e08e0();
s32 func_ov001_022e09d0();
s32 func_ov001_022e0ad0();
s32 func_ov001_022e0c5c();
s32 func_ov001_022e0884();
extern u8 data_ov001_0231c3ec[];
s32 func_ov001_022e0858();
void func_ov001_022e114c();
s32 func_02051e7c();

void func_ov001_022e0f08(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_ov001_022e0dfc(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    func_ov001_022e0d30(t0);
    func_ov001_022e0968(t0);
    func_ov001_022e08e0(t0);
    func_ov001_022e09d0(t0);
    func_ov001_022e0ad0(t0);
    func_ov001_022e0c5c(t0);
    func_ov001_022e0dfc(t0);
    func_ov001_022e0d30(t0);
    func_ov001_022e0884(t0);
    func_ov001_022e0858(t0, *(u32 *)data_ov001_0231c3ec);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    *(u32 *)((u8 *)t0 + 0x8) = (*(u32 *)((u8 *)t0 + 0x8) + 0x1);
    *(u32 *)((u8 *)t0 + 0x4) = (*(u32 *)((u8 *)t0 + 0x4) + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0x3c) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022e114c;
        func_02051e7c(0x1e, (u32)func_ov001_022e114c);
    }
}

#pragma thumb reset
