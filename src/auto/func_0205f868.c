/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208922c[];
s32 func_02060e38();
s32 func_02060e54();

void func_0205f868(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x4) = (*(s16 *)((u8 *)p0 + 4) + 0x1);
    *(u16 *)((u8 *)data_0208922c + 0x24) = (8 - ((*(s16 *)((u8 *)p0 + 4) << 3) / 0x1e));
    if (*(s16 *)((u8 *)p0 + 4) >= 0x1e) {
        *(u32 *)p0 = 0;
        func_02060e38(2, 0x20);
        func_02060e54(2, 0x80000040);
    }
}

#pragma thumb reset
