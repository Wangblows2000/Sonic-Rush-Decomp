/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 BrightnessFade_IsDone();
s32 Task_Destroy();
extern u8 data_022c4560[];
void func_020554d0();
s32 func_0204d684();
s32 func_ov001_02300214();
s32 Task_DestroyByTag();
s32 SceneChange_Unk_02055098();
s32 func_ov001_0230c0e0();
void func_020554dc();
s32 func_02053bec();
s32 func_020552bc();

void func_0205557c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4);
    s32 t2 = BrightnessFade_IsDone(*(u32 *)gTaskCurrent);
    if (t2 != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        if (*(u32 *)((u8 *)data_022c4560 + 0x10) == 2) {
            func_0204d684((u32)func_020554d0, 3);
            return;
        }
        s32 t3 = *(u8 *)((u8 *)data_022c4560 + 0x1c);
        if (t3 == 8) {
            s32 t4 = *(u8 *)((u8 *)data_022c4560 + 0x1d);
            if (t4 == 2) {
                func_ov001_02300214(t4, t3);
                Task_DestroyByTag(1);
                Task_DestroyByTag(2);
                Task_DestroyByTag(3);
                Task_DestroyByTag(4);
                SceneChange_Unk_02055098();
                return;
            }
        }
        if ((t1 & 0x20) != 0) {
            func_ov001_0230c0e0(0);
            return;
        }
        func_0204d684((u32)func_020554dc, 3);
        s32 t5 = *(u8 *)((u8 *)data_022c4560 + 0x1c);
        if ((u32)t5 < (u32)9) {
            if ((u32)*(u8 *)((u8 *)data_022c4560 + 0x1d) < (u32)2) {
                return;
            }
        }
        func_02053bec(1, t5);
        return;
    }
    func_020552bc(t0);
}
