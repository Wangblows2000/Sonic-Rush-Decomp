/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4678[];
s32 PlayerDemo_LoadData();
extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 func_02057094();
s32 func_0205708c();
s32 func_02056e04();
s32 func_02056a00();
s32 PolyDemoUnder2_Create();
s32 func_02057668();
void func_02058194();

void func_02058218(u8 *p0) {
    s32 t0 = PlayerDemo_LoadData(*(s16 *)((u8 *)data_022c4678 + 0), data_022c4678);
    *(u32 *)((u8 *)p0 + 0x14) = t0;
    if (*(u32 *)((u8 *)p0 + 0x14) == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    s32 t1 = func_02057094();
    *(u16 *)((u8 *)p0 + 0x1a) = *(u32 *)((u8 *)t1 + 0xc);
    s32 t2 = func_0205708c(*(u32 *)((u8 *)p0 + 0x14), *(u16 *)((u8 *)p0 + 0x18));
    *(u32 *)((u8 *)p0 + 0x1c) = t2;
    func_02056e04();
    func_02056a00();
    PolyDemoUnder2_Create();
    func_02057668(p0);
    *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) | 1);
    *(u32 *)p0 = (u32)func_02058194;
}

#pragma thumb reset
