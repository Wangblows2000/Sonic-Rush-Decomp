/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022e6dec();
s32 ZoneMapPause_IsClosed();
s32 func_ov001_022e955c();
s32 func_ov001_022e6870();
s32 func_ov001_022e5d08();

void func_ov001_022e76ec(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov001_022e6dec(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    *(u32 *)((u8 *)t0 + 0x74) = t1;
    s32 t2 = ZoneMapPause_IsClosed(t1);
    if (t2 != 0) {
        s32 t3 = func_ov001_022e955c();
        if (t3 == 0) {
            func_ov001_022e6870();
            return;
        }
        func_ov001_022e5d08();
    }
}

#pragma thumb reset
