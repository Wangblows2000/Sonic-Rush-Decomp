/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b2c[];
void StageObj_Main();
void func_ov000_022dcf2c();
s32 Task_Create();
s32 MI_CpuFill8();
s32 func_ov000_022fba50();
void func_ov000_022dcf38();
void func_ov000_022de860();

s32 func_ov000_022df3c4(s32 p0, s32 p1, s32 p2) {
    s32 t0 = *(u32 *)data_ov000_02369b2c;
    s32 t1 = Task_Create((u32)StageObj_Main, (u32)func_ov000_022dcf2c, 0, 0x502, 2, 0x214);
    s32 t2 = *(u32 *)((u8 *)t1 + 0x10);
    MI_CpuFill8(*(u32 *)((u8 *)t1 + 0x10), 0, 0x214);
    func_ov000_022fba50(t2, p0, p1, p2);
    *(u32 *)((u8 *)t2 + 0x70) = (u32)func_ov000_022dcf38;
    *(u32 *)((u8 *)t2 + 0x68) = 0;
    *(u32 *)((u8 *)t2 + 0x78) = (u32)func_ov000_022de860;
    *(u32 *)((u8 *)t2 + 0x54) = (*(u32 *)((u8 *)t2 + 0x54) | 0x10);
    *(u32 *)((u8 *)t2 + 0x58) = (*(u32 *)((u8 *)t2 + 0x58) | 0xa300);
    *(u32 *)((u8 *)t2 + 0x20c) = (t0 + 0x498);
    return t2;
}
