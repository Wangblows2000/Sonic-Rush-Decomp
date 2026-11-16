/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov000_0235e124();
s32 func_ov000_0235e068();
s32 func_ov000_0235e018();
s32 func_ov000_0235dfc4();
s32 func_ov000_0235def0();
s32 func_ov000_0235c9f8();
s32 func_ov000_0235ce3c();
s32 func_ov000_0235ddac();
s32 func_ov000_0235cf88();
void func_ov000_0235e79c();
void func_ov000_0235e61c();

void func_ov000_0235e8ac(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_ov000_0235e124(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    func_ov000_0235e068(t0);
    func_ov000_0235e018(t0);
    func_ov000_0235dfc4(t0);
    func_ov000_0235def0(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    s32 t1 = func_ov000_0235c9f8(0x44, 0, 0xa, *(u32 *)t0, 0);
    func_ov000_0235ce3c(t0, t1);
    if (*(u32 *)((u8 *)t0 + 0x10) == 0) {
        func_ov000_0235ddac(t0);
    }
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0xa) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0x64;
        *(u32 *)((u8 *)t0 + 0x8) = 0;
        func_ov000_0235cf88(t0, 0);
        if (*(u32 *)((u8 *)t0 + 0x10) != 0) {
            *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov000_0235e79c;
            return;
        }
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov000_0235e61c;
    }
}

#pragma thumb reset
