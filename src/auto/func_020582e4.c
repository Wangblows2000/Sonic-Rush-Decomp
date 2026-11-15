/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02057098();
s32 func_02057fb4();
s32 func_02043a88();
s32 func_020574d0();
s32 func_0205761c();
extern u8 data_022c467c[];

void func_020582e4(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_02057098(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x14));
    s32 t1 = func_02057fb4(t0);
    func_02043a88();
    func_020574d0(t0);
    func_0205761c(t0);
    *(u32 *)data_022c467c = 0;
}

#pragma thumb reset
