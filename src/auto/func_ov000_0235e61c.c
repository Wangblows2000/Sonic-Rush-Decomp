/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov000_0235cde0();
s32 func_ov000_0235cdc0();
s32 func_ov000_0235e124();
s32 func_ov000_0235e068();
s32 func_ov000_0235e018();
s32 func_ov000_0235dfc4();
s32 func_ov000_0235def0();
s32 func_ov000_0235ddec();
s32 func_ov000_0235ddac();
s32 func_02051474();
s32 MI_CpuCopy8();
s32 func_02052104();
void func_ov000_0235e5b4();

void func_ov000_0235e61c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_ov000_0235cde0(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    *(u32 *)((u8 *)t0 + 0x8) = t1;
    s32 t2 = func_ov000_0235cdc0(t0);
    func_ov000_0235e124(t0);
    func_ov000_0235e068(t0);
    func_ov000_0235e018(t0);
    func_ov000_0235dfc4(t0);
    func_ov000_0235def0(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    func_ov000_0235ddec(t0, 0);
    func_ov000_0235ddac(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    s32 t3 = (*(u32 *)((u8 *)t0 + 0x4) + 0x1);
    *(u32 *)((u8 *)t0 + 0x4) = t3;
    if (t2 != 0) {
        s32 t4 = func_02051474(t3);
        MI_CpuCopy8((t4 + 0x6), (t0 + 0xc14), 6);
        func_02052104(7);
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov000_0235e5b4;
    }
}

#pragma thumb reset
