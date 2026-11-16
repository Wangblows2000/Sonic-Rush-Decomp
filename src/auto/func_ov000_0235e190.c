/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208926c[];
extern u8 data_02087d44[];
s32 MsgBox_IsClosed();
s32 Task_DestroyAll();
s32 Touch_Disable();
s32 func_ov000_0235caf8();

void func_ov000_0235e190(void) {
    *(u16 *)((u8 *)data_0208926c + 0x8) = 0;
    *(u16 *)((u8 *)data_02087d44 + 0x8) = 0;
    s32 t0 = MsgBox_IsClosed(data_02087d44, 0);
    if (t0 != 0) {
        Task_DestroyAll();
        Touch_Disable();
        func_ov000_0235caf8(2);
    }
}

#pragma thumb reset
