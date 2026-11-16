/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 MsgBox_Destroy();
s32 Task_DestroyByTag();
s32 SceneChange_ToPlayerSelectMenuMode1();
s32 SceneChange_ToPlayerSelectMenuMode2();
s32 func_ov000_023611f8();
s32 func_ov000_0236119c();

void func_ov000_02361520(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if ((u32)*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) >= (u32)0x10) {
        *(u32 *)t0 = 0;
        s32 t1 = *(u32 *)((u8 *)t0 + 0x4);
        MsgBox_Destroy(0);
        Task_DestroyByTag(0x1000);
        if (t1 != 0) {
            SceneChange_ToPlayerSelectMenuMode1();
            return;
        }
        SceneChange_ToPlayerSelectMenuMode2();
        return;
    }
    func_ov000_023611f8(t0);
    func_ov000_0236119c(t0, 0x1000, 1);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
}

#pragma thumb reset
