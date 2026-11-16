/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
s32 func_ov000_022f8b70();

void func_ov000_022f4c40(s32 p0) {
    *(u16 *)((u8 *)p0 + 0x440) = (*(u16 *)((u8 *)p0 + 0x440) + 0x1);
    if ((*(u16 *)((u8 *)p0 + 0x440) & 3) != 0) {
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x440) & 0x8000) != 0) {
        func_ov000_022f8b70(*(u32 *)data_022c4560, 0);
    } else {
        func_ov000_022f8b70(*(u32 *)data_022c4560, 1);
    }
    *(u16 *)((u8 *)p0 + 0x440) = (*(u16 *)((u8 *)p0 + 0x440) & ~3);
    *(u16 *)((u8 *)p0 + 0x440) = (*(u16 *)((u8 *)p0 + 0x440) ^ 0x8000);
}
