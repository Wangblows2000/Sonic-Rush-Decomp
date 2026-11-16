/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022c87f8();
extern u8 data_ov001_0231f044[];
void func_ov001_022c8774();
void func_ov001_022c87b4();
s32 Task_Create();
extern u8 data_ov001_0231f040[];
extern u8 data_022c44f8[];
s32 WirelessMgr_StartWfsParent();

void func_ov001_022c8820(s32 p0) {
    func_ov001_022c87f8();
    *(u32 *)data_ov001_0231f044 = 0;
    s32 t0 = Task_Create((u32)func_ov001_022c8774, (u32)func_ov001_022c87b4, 0, 0x1000, 0, 4);
    *(u32 *)data_ov001_0231f040 = t0;
    *(u32 *)*(u32 *)((u8 *)t0 + 0x10) = 1;
    WirelessMgr_StartWfsParent(data_022c44f8, 1, 0x200, 0x10, p0, 8);
}

#pragma thumb reset
