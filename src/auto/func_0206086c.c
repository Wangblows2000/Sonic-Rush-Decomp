/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02060384();
s32 func_02060e54();
s32 func_02036894();
extern u8 data_0207cb7c[];
s32 func_02036854();
extern u8 data_02250458[];
s32 func_0201e7cc();
s32 func_0201bf34();

void func_0206086c(u8 *p0) {
    s32 t0 = func_02060384();
    *(u32 *)((u8 *)p0 + 0x28) = t0;
    s32 t1 = *(u32 *)((u8 *)p0 + 0x28);
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x28) == 0) {
        func_02060e54(0, 0x10);
        return;
    }
    s32 t2 = (*(u32 *)((u8 *)t1 + 0x4) & 1);
    s32 t3 = *(u32 *)((u8 *)t1 + 0x4);
    if (t2 == 0) {
        if ((t3 & 2) == 0) {
            func_02036894(2, t2, (t3 & 2));
            func_02036854(data_0207cb7c);
        }
        func_0201e7cc(*(u32 *)*(u32 *)*(u32 *)((u8 *)p0 + 0x28), *(u32 *)data_02250458);
    }
    func_0201bf34(((u8 *)p0 + 0x2c));
    func_0201bf34(((u8 *)p0 + 0x30));
}

#pragma thumb reset
