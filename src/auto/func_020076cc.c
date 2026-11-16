/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_Terminate();
s32 OS_GetLockID();
s32 func_0200f27c();
s32 func_02007ab8();
s32 OS_SetIrqMask();
s32 OS_ResetRequestIrqMask();
s32 func_0200774c();
s32 func_01ff8480();

void func_020076cc(s32 p0) {
    s32 t0 = *(u16 *)0x27ffc40;
    if (t0 == 2) {
        OS_Terminate(t0, 0x27ffc40);
    }
    s32 t1 = OS_GetLockID();
    func_0200f27c(((u32)(t1 << 0x10) >> 0x10));
    func_02007ab8(0);
    func_02007ab8(1);
    func_02007ab8(2);
    func_02007ab8(3);
    OS_SetIrqMask(0x40000);
    OS_ResetRequestIrqMask(-1);
    *(u32 *)0x27ffc20 = p0;
    func_0200774c(0x10, 0x27ffc20);
    func_01ff8480();
}
