/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022da3ac();
s32 func_ov001_022da33c();
s32 func_ov001_022da2cc();
s32 func_ov001_022da288();
s32 func_ov001_022da244();
s32 func_02040ec0();
void func_ov001_022dc81c();

void func_ov001_022dd574(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_ov001_022da3ac(*(u32 *)((u8 *)p0 + 0x10));
    func_ov001_022da33c(t0);
    func_ov001_022da2cc(t0, 0);
    func_ov001_022da2cc(t0, 1);
    func_ov001_022da288(t0, 0);
    func_ov001_022da244(t0, 1);
    func_02040ec0((t0 + 0xb44), 0);
    func_02040ec0((t0 + 0xb70), 1);
    func_02040ec0((t0 + 0xb9c), 2);
    func_02040ec0((t0 + 0xbc8), 3);
    func_02040ec0((t0 + 0xbf4), 4);
    func_02040ec0((t0 + 0xc20), 5);
    *(u32 *)((u8 *)t0 + 0x20) = 0;
    *(u32 *)((u8 *)t0 + 0x48) = 1;
    *(u32 *)((u8 *)p0 + 0x8) = (u32)func_ov001_022dc81c;
}

#pragma thumb reset
