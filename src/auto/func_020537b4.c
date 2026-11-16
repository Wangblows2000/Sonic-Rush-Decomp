/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gNaviMsgTask[];
extern u8 data_022c4560[];
extern u8 gSaveData[];
extern u8 data_0207b9d4[];
s32 func_02052c90();

void func_020537b4(s32 p0) {
    s32 t0 = *(u32 *)gNaviMsgTask;
    if (t0 == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    s32 v2;
    if (*(u32 *)*(u32 *)((u8 *)t0 + 0x10) == 0) {
        s32 t3 = *(u32 *)data_022c4560;
        if (t3 == 0) {
            if ((u32)*(u16 *)((u8 *)gSaveData + 0xc) < (u32)3) {
                return;
            }
        }
        if (t3 == 1) {
            s32 t4 = *(u16 *)((u8 *)gSaveData + 0x64);
            if ((u32)t4 < (u32)4) {
                return;
            }
            if ((u32)t4 >= (u32)0x1b) {
                if ((u32)t4 < (u32)0x1c) {
                    return;
                }
            }
        }
        v2 = t3;
    }
    s32 t5 = *(u32 *)((u8 *)data_0207b9d4 + ((s32)p0 << 2));
    *(u32 *)((u8 *)t1 + 0xd8) = t5;
    func_02052c90(t1, p0, t5, v2);
}
