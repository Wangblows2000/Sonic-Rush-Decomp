/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02040ec0();
s32 func_02040e80();
s32 func_ov001_022d983c();
s32 func_ov001_022d6dac();
void func_ov001_022dc1e8();
extern u8 gTaskCurrent[];

void func_ov001_022da9fc(s32 p0) {
    func_02040ec0(((u8 *)p0 + 0xb44), 0);
    func_02040ec0(((u8 *)p0 + 0xb70), 1);
    func_02040ec0(((u8 *)p0 + 0xb9c), 2);
    func_02040ec0(((u8 *)p0 + 0xbc8), 3);
    func_02040ec0(((u8 *)p0 + 0xbf4), 4);
    func_02040ec0(((u8 *)p0 + 0xc20), 5);
    func_02040e80(((u8 *)p0 + 0xb44));
    func_02040e80(((u8 *)p0 + 0xb70));
    func_02040e80(((u8 *)p0 + 0xb9c));
    func_02040e80(((u8 *)p0 + 0xbc8));
    func_02040e80(((u8 *)p0 + 0xbf4));
    func_02040e80(((u8 *)p0 + 0xc20));
    func_ov001_022d983c(p0, 1);
    func_ov001_022d6dac();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022dc1e8;
}

#pragma thumb reset
