/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_ov001_0231be10[];
s32 func_02051838();
s32 func_ov001_022dd774();
s32 func_ov001_022dd770();
s32 func_ov001_022dd728();
s32 func_ov001_022dd6b4();
extern u8 data_ov001_0231f058[];

void func_ov001_022dd854(u8 *p0) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    u32 t1 = *(u32 *)data_ov001_0231be10;
    if (t1 != 0) {
        func_02051838(t1);
    }
    func_ov001_022dd774(t0);
    func_ov001_022dd770(t0);
    func_ov001_022dd728(t0);
    func_ov001_022dd6b4(t0);
    *(u32 *)data_ov001_0231f058 = 0;
}

#pragma thumb reset
