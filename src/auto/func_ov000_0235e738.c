/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov000_0235e124();
s32 func_ov000_0235e068();
s32 func_ov000_0235e018();
s32 func_ov000_0235dfc4();
s32 func_ov000_0235def0();
s32 func_ov000_0235ddec();
s32 func_ov000_0235cf04();
void func_ov000_0235e4c0();

void func_ov000_0235e738(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov000_0235e124(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    func_ov000_0235e068(t0);
    func_ov000_0235e018(t0);
    func_ov000_0235dfc4(t0);
    func_ov000_0235def0(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    func_ov000_0235ddec(t0, 1);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0x10) {
        *(u32 *)t0 = 0;
        func_ov000_0235cf04(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_0235e4c0;
    }
}

#pragma thumb reset
