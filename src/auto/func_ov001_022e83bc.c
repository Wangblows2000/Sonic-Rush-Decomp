/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Task_DestroyByTag();
s32 SceneChange_Unk_02054e68();
s32 func_ov001_022e19bc();

void func_ov001_022e83bc(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) == 1) {
        s32 t1 = *(u32 *)((u8 *)t0 + 0x1cc8);
        s32 t2 = *(u32 *)((u8 *)t0 + 0x1ccc);
        s32 t3 = *(u32 *)((u8 *)t0 + 0x1cd0);
        Task_DestroyByTag(0x1000, t0);
        if (t2 < 0) {
            SceneChange_Unk_02054e68();
            return;
        }
        func_ov001_022e19bc(t1, t2, t3);
    }
}

#pragma thumb reset
