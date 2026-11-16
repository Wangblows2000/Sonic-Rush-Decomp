/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
s32 func_0204e6e8();
s32 func_0204e6e4();
extern u8 data_022c4680[];
extern u8 data_02074bbc[];
extern u8 data_02074bb4[];

s32 func_0204eb7c(void) {
    s32 t0 = func_0204e6e8(*(u8 *)((u8 *)data_022c4560 + 0x1c));
    s32 t1 = func_0204e6e4(*(u8 *)((u8 *)data_022c4560 + 0x1d), data_022c4560);
    if (t0 == 8) {
        return ((*(u32 *)((u8 *)data_022c4680 + 0x8) & 0x1000) == 0);
    }
    if (*(u32 *)data_022c4560 != 1) {
        s32 t2 = *(u8 *)((u8 *)data_02074bbc + t0);
        s32 v3 = t2;
        if ((u32)t1 <= (u32)1) {
            v3 = ((v3 - 0x1) & 0xff);
        }
        return (v3 > *(u16 *)((u8 *)data_022c4680 + 0xc));
    }
    s32 t4 = *(u8 *)((u8 *)data_02074bb4 + t0);
    s32 v5 = t4;
    if ((u32)t1 <= (u32)1) {
        v5 = ((v5 - 0x1) & 0xff);
    }
    return (v5 > *(u16 *)((u8 *)data_022c4680 + 0x64));
}
