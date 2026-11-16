/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov000_02361404();
s32 Task_Create();
s32 SysSound_Release();
s32 func_020516b4();

void func_ov000_0236143c(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x20) = 0;
    s32 t0 = Task_Create((u32)func_ov000_02361404, 0, 0, 0x2000, 0x1000, 4);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = (u32)p0;
    SysSound_Release(t1);
    func_020516b4();
}

#pragma thumb reset
