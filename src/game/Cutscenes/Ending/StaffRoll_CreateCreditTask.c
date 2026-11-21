/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void StaffRollCredit_Main();
s32 Task_Create();

s32 StaffRoll_CreateCreditTask(s32 p0) {
    s32 t0 = Task_Create((u32)StaffRollCredit_Main, 0, 0, 0x5000, 0x1173, 0xc);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
    *(u32 *)((u8 *)t1 + 0x4) = 0x80;
    *(u32 *)((u8 *)t1 + 0x8) = 0x1d0;
    return t0;
}

#pragma thumb reset
