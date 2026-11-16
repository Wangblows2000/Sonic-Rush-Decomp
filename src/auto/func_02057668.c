/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02056e7c();
extern u8 data_022c4678[];
s32 Sound_ResetHeap();
extern u8 data_0207c5bc[];
s32 func_02036854();
extern u8 gSoundHeap[];
s32 func_0201e7cc();
s32 func_0201bf34();

void func_02057668(u8 *p0) {
    s32 t0 = func_02056e7c();
    *(u32 *)((u8 *)p0 + 0x2c) = t0;
    s32 t1 = *(u32 *)((u8 *)p0 + 0x2c);
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x2c) == 0) {
        *(u32 *)((u8 *)p0 + 0x4) = (*(u32 *)((u8 *)p0 + 0x4) | 0x10000000);
        return;
    }
    if ((*(u32 *)((u8 *)t1 + 0x4) & 1) == 0) {
        s32 t2 = *(s16 *)((u8 *)data_022c4678 + 0);
        if (t2 != 0x2a) {
            Sound_ResetHeap(t2, data_022c4678);
        }
        func_02036854(data_0207c5bc);
        func_0201e7cc(*(u32 *)*(u32 *)*(u32 *)((u8 *)p0 + 0x2c), *(u32 *)gSoundHeap);
    }
    func_0201bf34(((u8 *)p0 + 0x30));
    func_0201bf34(((u8 *)p0 + 0x34));
}

#pragma thumb reset
